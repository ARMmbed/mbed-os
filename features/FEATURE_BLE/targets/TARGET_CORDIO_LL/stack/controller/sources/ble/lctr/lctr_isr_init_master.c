/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller master advertising event ISR callbacks.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include "lctr_int_init_master.h"
#include "lctr_int_adv_master.h"
#include "lctr_int_conn_master.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "bb_ble_api_reslist.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  End an initiate scan operation in the master role.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstInitiateEndOp(BbOpDesc_t *pOp)
{
  if (!lctrMstInit.selfTerm)    /* implies not a LL_CONN_IND Tx completion */
  {
    lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(lctrMstInit.data.init.connHandle);

    /* Service RM. */
    SchRmSetReference(lctrMstInit.data.init.connHandle);

    /* Connection cleanup. */
    if (lctrMstInit.data.init.connBodLoaded)
    {
      lctrMstInit.data.init.connBodLoaded = FALSE;

      bool_t result = SchRemove(&pCtx->connBod);
      (void)result;
      WSF_ASSERT(result);       /* non-head BOD always remove-able */
    }
  }

  /* Same as discovery scan. */
  pOp->pCtx = &lctrMstInit;
  lctrMstDiscoverEndOp(pOp);
}

/*************************************************************************************************/
/*!
 *  \brief      Initiate state advertising packet receive completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pAdvBuf Received advertising buffer.
 *
 *  \return     TRUE if connection indication transmission is required, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrMstInitiateAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(lctrMstInit.data.init.connHandle);
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

  uint32_t advEndTs = pScan->advStartTs + BB_US_TO_BB_TICKS(LCTR_ADV_PKT_1M_US(pScan->filtResults.pduLen));

  /*** Transmit response PDU processing. ***/

  bool_t txConnInd = FALSE;

  if (!pScan->filtResults.peerMatch)
  {
    /* Require peer match. */
  }
  else if (!lctrMstInit.data.init.connBodLoaded)
  {
    BbSetBodTerminateFlag();
  }
  else if (pScan->pTxReqBuf)
  {
    /* Update connection indication header with advertiser's address. */
    uint8_t *pConnIndAdvA = pScan->pTxReqBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN;
    Bda64ToBstream(pConnIndAdvA, pScan->filtResults.peerAddr);
    lctrMstInit.reqPduHdr.rxAddrRnd = pScan->filtResults.peerAddrRand;

    /* Update connection indication header with scanner's address. */
    if (lctrMstInit.scanParam.ownAddrType & LL_ADDR_IDENTITY_BIT)
    {
      bool_t  localAddrRand;
      uint64_t localAddr;

      if (pScan->filtResults.pduType == LL_PDU_ADV_DIRECT_IND)
      {
        /* If peer is using directed advertising then use the InitA from the    */
        /* ADV_DIRECT_IND as our local address in the connect indication.       */
        localAddrRand = ((pAdvBuf[0] >> LCTR_ADV_HDR_RX_ADD_SHIFT) & 1);
        localAddr     = BstreamToBda64(pAdvBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN);

        /* Update the local RPA if the received one is RPA. */
        if (BDA64_ADDR_IS_RPA(localAddr))
        {
          lctrMstInit.data.init.localRpa = localAddr;
          BbBleResListUpdateLocal(pScan->filtResults.peerIdAddrRand, pScan->filtResults.peerIdAddr, &localAddr);
        }
      }
      else
      {
        localAddrRand = BB_BLE_PDU_FILT_FLAG_IS_SET(&pBle->pduFilt, LOCAL_ADDR_MATCH_RAND);
        localAddr     = pBle->pduFilt.localAddrMatch;
        if (BbBleResListReadLocal(pScan->filtResults.peerIdAddrRand, pScan->filtResults.peerIdAddr, &localAddr))
        {
          localAddrRand = TRUE;
          lctrMstInit.data.init.localRpa = localAddr;
        }
        else
        {
          lctrMstInit.data.init.localRpa = 0;
        }
      }
      uint8_t *pConnIndInitA = pScan->pTxReqBuf + LL_ADV_HDR_LEN;
      Bda64ToBstream(pConnIndInitA, localAddr);
      lctrMstInit.reqPduHdr.txAddrRnd = localAddrRand;
    }
    else
    {
      lctrMstInit.data.init.localRpa = 0;
    }

    lctrPackAdvbPduHdr(pScan->pTxReqBuf, &lctrMstInit.reqPduHdr);

    /* Update txWinOffset field in CONN_IND PDU. */
    uint32_t txWinOffsetUsec = BB_TICKS_TO_US(lctrMstInit.data.init.firstCeDue - advEndTs) - LL_BLE_TIFS_US - LCTR_CONN_IND_PKT_1M_US;
    uint16_t txWinOffset = LCTR_US_TO_CONN_IND(txWinOffsetUsec) - LCTR_DATA_CHAN_DLY;
    UINT16_TO_BUF(pScan->pTxReqBuf + LCTR_CONN_IND_TX_WIN_OFFSET, txWinOffset);
    lctrMstInit.data.init.connInd.txWinOffset = txWinOffset;

    if (txWinOffsetUsec < LCTR_CONN_IND_US(LCTR_DATA_CHAN_DLY))
    {
      LL_TRACE_WARN1("AE too close to initial CE deltaUsec=%u, suppressed LL_CONN_IND delivery", BB_TICKS_TO_US(lctrMstInit.data.init.firstCeDue - advEndTs));

      /* Restart scanning with better initial CE margin. */
      BbSetBodTerminateFlag();
    }
    else if (txWinOffset > pCtx->connInterval)
    {
      LL_TRACE_WARN1("Scan period exceeded initial CE time by deltaUsec=%u, suppressed LL_CONN_IND delivery", BB_TICKS_TO_US(advEndTs - lctrMstInit.data.init.firstCeDue));

      /* Restart scanning with CE in the future. */
      BbSetBodTerminateFlag();
    }
    else
    {
      txConnInd = TRUE;

      /*** Received advertising PDU post-processing. ***/

      if ((lctrMstInit.reqPduHdr.chSel == LL_CH_SEL_2) &&   /* local initiator supports CS#2 */
          (pAdvBuf[0] & (1 << LCTR_ADV_HDR_CH_SEL_SHIFT)))  /* peer advertiser supports CS#2 */
      {
        lctrMstInit.data.init.usedChSel = LL_CH_SEL_2;
      }
      else
      {
        lctrMstInit.data.init.usedChSel = LL_CH_SEL_1;
      }
    }
  }

  return txConnInd;
}

/*************************************************************************************************/
/*!
 *  \brief      Connection indication Tx completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pIndBuf Transmitted request buffer.
 *
 *  \return     FALSE to signal no scan response.
 */
/*************************************************************************************************/
bool_t lctrMstConnIndTxCompHandler(BbOpDesc_t *pOp, const uint8_t *pIndBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  lctrMstInit.selfTerm = TRUE;
  BbSetBodTerminateFlag();
  return FALSE;
}
