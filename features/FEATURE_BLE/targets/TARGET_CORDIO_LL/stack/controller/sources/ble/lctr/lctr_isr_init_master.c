/* Copyright (c) 2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 * \file
 * \brief Link layer controller master advertising event ISR callbacks.
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
        else
        {
          lctrMstInit.data.init.localRpa = 0;
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

#if (LL_ENABLE_TESTER)
    if (llTesterCb.auxReq.len)
    {
      /* Overriding CONNECT_IND from test script. */
      memcpy(pScan->pTxReqBuf, llTesterCb.auxReq.buf, llTesterCb.auxReq.len);
      pScan->txReqLen = llTesterCb.auxReq.len;
    }
#endif

    lctrPackAdvbPduHdr(pScan->pTxReqBuf, &lctrMstInit.reqPduHdr);

    /* Update txWinOffset field in CONN_IND PDU. */
    uint32_t txWinOffsetUsec = BB_TICKS_TO_US(lctrMstInit.data.init.firstCeDue - advEndTs) - LL_BLE_TIFS_US - LCTR_CONN_IND_PKT_1M_US;
    uint16_t txWinOffset = LCTR_US_TO_CONN_IND(txWinOffsetUsec) - LCTR_DATA_CHAN_DLY;
    UINT16_TO_BUF(pScan->pTxReqBuf + LCTR_CONN_IND_TX_WIN_OFFSET, txWinOffset);
    lctrMstInit.data.init.connInd.txWinOffset = txWinOffset;

#if (LL_ENABLE_TESTER)
    switch (llTesterCb.connFirstCePos)
    {
    case LL_TESTER_FIRST_CE_POS_BEGIN:
      /* BOD is not scheduled; force time adjustment here. */
      pCtx->connBod.due = advEndTs + BB_US_TO_BB_TICKS(txWinOffsetUsec + LL_BLE_TIFS_US + LCTR_CONN_IND_PKT_1M_US);
      break;
    case LL_TESTER_FIRST_CE_POS_END:
      /* BOD is not scheduled; force time adjustment here. */
      pCtx->connBod.due = advEndTs + BB_US_TO_BB_TICKS(txWinOffsetUsec + LCTR_CONN_IND_US(lctrMstInit.data.init.connInd.txWinSize - 1) + LL_BLE_TIFS_US + LCTR_CONN_IND_PKT_1M_US);
      break;
    case LL_TESTER_FIRST_CE_POS_NORMAL:
    default:
      break;
    }
    llTesterCb.connFirstCePos = LL_TESTER_FIRST_CE_POS_NORMAL;
#endif

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
#if (LL_ENABLE_TESTER == TRUE)
  if (llTesterCb.auxReq.len)
  {
    /* Do not establish connection when CONNECT_IND is overridden. */
    return FALSE;
  }
#endif

  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  lctrMstInit.selfTerm = TRUE;
  BbSetBodTerminateFlag();
  return FALSE;
}
