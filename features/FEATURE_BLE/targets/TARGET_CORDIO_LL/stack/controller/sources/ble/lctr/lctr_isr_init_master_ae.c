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

#include "lctr_int_init_master_ae.h"
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

/**************************************************************************************************
  Functions: Initiate BOD handlers
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Reschedule primary scan operation for initiation.
 *
 *  \param  pExtInitCtx     Extended initiate context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lctrMstExtInitiateScanReschedule(lctrExtScanCtx_t *pExtInitCtx)
{
  BbOpDesc_t *pOp = &pExtInitCtx->scanBod;
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

  /*** Reschedule primary operation ***/

  /* Recover primary scan BOD min duration so that its run will be guaranteed in BB. */
  pOp->minDurUsec = LCTR_MIN_SCAN_USEC;

  /* Reset due time to start of scan window. */
  pOp->due = pExtInitCtx->data.init.scanWinStart;

  if ((pExtInitCtx->data.init.param.scanInterval != pExtInitCtx->data.init.param.scanWindow) &&
      ((pScan->elapsedUsec + pOp->minDurUsec) < LCTR_BLE_TO_US(pExtInitCtx->data.init.param.scanWindow)))
  {
    const uint32_t min = BB_US_TO_BB_TICKS(pScan->elapsedUsec);
    const uint32_t max = BB_BLE_TO_BB_TICKS(pExtInitCtx->data.init.param.scanWindow);

    if (SchInsertEarlyAsPossible(pOp, min, max))
    {
      /* Continue interrupted operation. */
      pScan->elapsedUsec = BB_TICKS_TO_US(pOp->due - pExtInitCtx->data.init.scanWinStart);
      WSF_ASSERT(pScan->elapsedUsec < pOp->maxDurUsec);
      return;
    }
  }

  /* Advance to next scanInterval. */

  pScan->elapsedUsec = 0;

  /* Compute next channel. */
  pBle->chan.chanIdx = lctrScanChanSelectNext(pBle->chan.chanIdx, pScan->scanChMap);

  if (pExtInitCtx->data.init.param.scanInterval == pExtInitCtx->data.init.param.scanWindow)
  {
    /* Continuous scan. */
    SchInsertNextAvailable(pOp);
    pExtInitCtx->data.init.scanWinStart = pOp->due;
  }
  else
  {
    /* Next scan interval. */
    const uint32_t min = BB_BLE_TO_BB_TICKS(pExtInitCtx->data.init.param.scanInterval);
    const uint32_t max = min + BB_BLE_TO_BB_TICKS(pExtInitCtx->data.init.param.scanWindow);

    while (TRUE)
    {
      /* Store start of next scan window. */
      pExtInitCtx->data.init.scanWinStart = pOp->due + min;

      if (SchInsertEarlyAsPossible(pOp, min, max))
      {
        pScan->elapsedUsec = BB_TICKS_TO_US(pOp->due - pExtInitCtx->data.init.scanWinStart);
        WSF_ASSERT(pScan->elapsedUsec < pOp->maxDurUsec);
        break;
      }
      else
      {
        /* Advance to next scan window. */
        pOp->due = pExtInitCtx->data.init.scanWinStart;

        LL_TRACE_WARN1("!!! Scan schedule conflict at due=%u", pOp->due + min);
        LL_TRACE_WARN1("!!!                           scanWindowUsec=%u", LCTR_BLE_TO_US(pExtInitCtx->data.init.param.scanWindow));
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handler for pre-initiate scan execution.
 *
 *  \param  pOp         BB operation descriptor.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstExtPreInitiateExecHandler(BbOpDesc_t *pOp)
{
  lctrExtScanCtx_t * const pExtInitCtx = pOp->pCtx;

  if (!pExtInitCtx->shutdown || !pExtInitCtx->selfTerm)
  {
    /* Setup connection's initial CE now that RM is synchronized BB. This step must be performed
     * before initiate's scan operation sets up its executing duration (i.e. "pre-execute"). */
    pExtInitCtx->data.init.firstCeDue = lctrMstConnAdjustOpStart(LCTR_GET_CONN_CTX(pExtInitCtx->data.init.connHandle),
                                                                 pOp->due,
                                                                 pOp->minDurUsec,
                                                                 &pExtInitCtx->data.init.connInd);
    pExtInitCtx->data.init.connBodLoaded = TRUE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  End a initiate scan operation in the master role.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstExtInitiateScanEndOp(BbOpDesc_t *pOp)
{
  lctrExtScanCtx_t * const pExtInitCtx = pOp->pCtx;

  if (pExtInitCtx->shutdown || pExtInitCtx->selfTerm)
  {
    pExtInitCtx->bodTermCnt++;
    if (( pExtInitCtx->auxOpPending && (pExtInitCtx->bodTermCnt >= 2)) ||     /* Wait for both ExtScan and AuxScan operations. */
        (!pExtInitCtx->auxOpPending && (pExtInitCtx->bodTermCnt >= 1)))       /* Wait only for ExtScan operation. */
    {
      lctrSendExtInitMsg(pExtInitCtx, LCTR_EXT_INIT_MSG_TERMINATE);
    }
    return;
  }

  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  /*** Reschedule operation ***/

  if (!pExtInitCtx->auxOpPending)
  {
    lctrMstExtInitiateScanReschedule(pExtInitCtx);
  }
  /* else postpone until lctrMstAuxDiscoverEndOp(). */
}

/*************************************************************************************************/
/*!
 *  \brief  End an auxiliary initiate scan operation in the master role.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstAuxInitiateScanEndOp(BbOpDesc_t *pOp)
{
  lctrExtScanCtx_t * const pExtInitCtx = pOp->pCtx;

  if (pExtInitCtx->shutdown || pExtInitCtx->selfTerm)
  {
    lctrSendExtInitMsg(pExtInitCtx, LCTR_EXT_INIT_MSG_TERMINATE);
    return;
  }

  pExtInitCtx->auxOpPending = FALSE;

  /* Reschedule primary channel scan operation. */
  lctrMstExtInitiateScanReschedule(pExtInitCtx);
}

/*************************************************************************************************/
/*!
 *  \brief  End an initiate operation in the master role.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstExtInitiateEndOp(BbOpDesc_t *pOp)
{
  lctrExtScanCtx_t * const pExtInitCtx = pOp->pCtx;

  if (!pExtInitCtx->selfTerm &&     /* Implies not a LL_CONN_IND Tx completion. */
      !pExtInitCtx->auxOpPending)   /* Auxiliary EndOp handles cleanup. */
  {
    lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(pExtInitCtx->data.init.connHandle);

    /* Service RM. */
    SchRmSetReference(pExtInitCtx->data.init.connHandle);

    /* Connection cleanup. */
    if (pExtInitCtx->data.init.connBodLoaded)
    {
      pExtInitCtx->data.init.connBodLoaded = FALSE;

      bool_t result = SchRemove(&pCtx->connBod);
      (void)result;
      WSF_ASSERT(result);       /* Non-head BOD always remove-able. */
    }
  }
  else if (!pExtInitCtx->selfTerm &&    /* Implies not a LL_CONN_IND Tx completion. */
      pExtInitCtx->auxOpPending)   /* AuxBod is scheduled. */
  {
    lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(pExtInitCtx->data.init.connHandle);

    if (pExtInitCtx->data.init.connBodLoaded &&
        pCtx->connBod.due < pExtInitCtx->auxScanBod.due)
    {
      /* connBod is scheduled and due before auxScanBod, need to reschedule connBod. */
      pExtInitCtx->data.init.connBodLoaded = FALSE;

      bool_t result = SchRemove(&pCtx->connBod);
      (void)result;
      WSF_ASSERT(result);       /* Non-head BOD always remove-able. */

      pExtInitCtx->data.init.firstCeDue = lctrMstConnAdjustOpStart(LCTR_GET_CONN_CTX(pExtInitCtx->data.init.connHandle),
                                                                   pExtInitCtx->auxScanBod.due,
                                                                   pExtInitCtx->auxScanBod.minDurUsec,
                                                                   &pExtInitCtx->data.init.connInd);
      pExtInitCtx->data.init.connBodLoaded = TRUE;
    }
  }

  lctrMstExtInitiateScanEndOp(pOp);
}

/*************************************************************************************************/
/*!
 *  \brief  End an auxiliary initiate operation in the master role.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstAuxInitiateEndOp(BbOpDesc_t *pOp)
{
  lctrExtScanCtx_t * const pExtInitCtx = pOp->pCtx;

  if (!pExtInitCtx->selfTerm)       /* Implies not a LL_CONN_IND Tx completion. */
  {
    lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(pExtInitCtx->data.init.connHandle);

    /* Service RM. */
    SchRmSetReference(pExtInitCtx->data.init.connHandle);

    /* Connection cleanup. */
    if (pExtInitCtx->data.init.connBodLoaded)
    {
      pExtInitCtx->data.init.connBodLoaded = FALSE;

      bool_t result = SchRemove(&pCtx->connBod);
      (void)result;
      WSF_ASSERT(result);       /* Non-head BOD always remove-able. */
    }
  }

  lctrMstAuxInitiateScanEndOp(pOp);
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
bool_t lctrMstExtConnIndTxCompHandler(BbOpDesc_t *pOp, const uint8_t *pIndBuf)
{
#if (LL_ENABLE_TESTER == TRUE)
  if (llTesterCb.auxReq.len)
  {
    /* Do not establish connection when AUX_CONNECT_REQ is overridden. */
    return FALSE;
  }
#endif

  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  lctrExtScanCtx_t * const pExtInitCtx = pOp->pCtx;

  pExtInitCtx->selfTerm = TRUE;
  BbSetBodTerminateFlag();

  return FALSE;
}

/**************************************************************************************************
  Functions: Initiate packet handlers
**************************************************************************************************/

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
bool_t lctrMstInitiateExtAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  lctrExtScanCtx_t *pCtx = pOp->pCtx;
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

  uint32_t advEndTs = pScan->advStartTs +
                      BB_US_TO_BB_TICKS(SchBleCalcAdvPktDurationUsec(pBle->chan.rxPhy, pScan->advRxPhyOptions,
                                                                     LL_ADV_HDR_LEN + pScan->filtResults.pduLen));

  /*** Transmit response PDU processing. ***/

  bool_t txConnInd = FALSE;

  if (!pScan->filtResults.peerMatch)
  {
    /* Require peer match. */
  }
  else if (!pCtx->data.init.connBodLoaded)
  {
    BbSetBodTerminateFlag();
  }
  else if (pScan->pTxReqBuf)
  {
    /* Update connection indication header with advertiser's address. */
    uint8_t *pConnIndAdvA = pScan->pTxReqBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN;
    Bda64ToBstream(pConnIndAdvA, pScan->filtResults.peerAddr);
    pCtx->reqPduHdr.rxAddrRnd = pScan->filtResults.peerAddrRand;

    /* Update connection indication header with scanner's address. */
    if (pCtx->data.init.ownAddrType & LL_ADDR_IDENTITY_BIT)
    {
      bool_t  localAddrRand;
      uint64_t localAddr;

      if (pScan->filtResults.pduType == LL_PDU_ADV_DIRECT_IND)
      {
        /* If peer is using directed advertising then use the InitA from the    */
        /* ADV_DIRECT_IND as our local address in the connect indication.       */
        localAddrRand = ((pAdvBuf[0] >> LCTR_ADV_HDR_RX_ADD_SHIFT) & 1);
        localAddr     = BstreamToBda64(pAdvBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN);
        pCtx->data.init.localRpa = localAddr;
        /* Update the local RPA if the received one is RPA. */
        if (BDA64_ADDR_IS_RPA(localAddr))
        {
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
          pCtx->data.init.localRpa = localAddr;
        }
        else
        {
          pCtx->data.init.localRpa = 0;
        }
      }
      uint8_t *pConnIndInitA = pScan->pTxReqBuf + LL_ADV_HDR_LEN;
      Bda64ToBstream(pConnIndInitA, localAddr);
      pCtx->reqPduHdr.txAddrRnd = localAddrRand;
    }
    else
    {
      pCtx->data.init.localRpa = 0;
    }

    if (lmgrCb.features & LL_FEAT_CH_SEL_2)
    {
      pCtx->reqPduHdr.chSel = LL_CH_SEL_2;
    }
    else
    {
      pCtx->reqPduHdr.chSel = LL_CH_SEL_1;
    }

    lctrPackAdvbPduHdr(pScan->pTxReqBuf, &pCtx->reqPduHdr);

    /* Update txWinOffset field in CONN_IND PDU. */
    uint32_t txWinOffsetUsec = BB_TICKS_TO_US(pCtx->data.init.firstCeDue - advEndTs) - LL_BLE_TIFS_US -
                               SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy,  pScan->advRxPhyOptions, LL_ADV_HDR_LEN + LL_CONN_IND_PDU_LEN); /* Assume conn_ind uses the same PHY as adv_ind. */
    uint16_t txWinOffset = LCTR_US_TO_CONN_IND(txWinOffsetUsec) - LCTR_DATA_CHAN_DLY;
    UINT16_TO_BUF(pScan->pTxReqBuf + LCTR_CONN_IND_TX_WIN_OFFSET, txWinOffset);
    pCtx->data.init.connInd.txWinOffset = txWinOffset;

    if (txWinOffsetUsec < LCTR_CONN_IND_US(LCTR_DATA_CHAN_DLY))
    {
      LL_TRACE_WARN1("AE too close to initial CE deltaUsec=%u, suppressed LL_CONN_IND delivery", BB_TICKS_TO_US(pCtx->data.init.firstCeDue - advEndTs));

      /* Restart scanning with better initial CE margin. */
      BbSetBodTerminateFlag();
    }
    else if (txWinOffset > pCtx->data.init.connInterval)
    {
      LL_TRACE_WARN1("Scan period exceeded initial CE time by deltaUsec=%u, suppressed LL_CONN_IND delivery", BB_TICKS_TO_US(advEndTs - pCtx->data.init.firstCeDue));

      /* Restart scanning with CE in the future. */
      BbSetBodTerminateFlag();
    }
    else
    {
      txConnInd = TRUE;

      /*** Received advertising PDU post-processing. ***/

      if ((pCtx->reqPduHdr.chSel == LL_CH_SEL_2) &&         /* local initiator supports CS#2 */
          (pAdvBuf[0] & (1 << LCTR_ADV_HDR_CH_SEL_SHIFT)))  /* peer advertiser supports CS#2 */
      {
        pCtx->data.init.usedChSel = LL_CH_SEL_2;
      }
      else
      {
        pCtx->data.init.usedChSel = LL_CH_SEL_1;
      }

      pCtx->data.init.phy = pBle->chan.txPhy;

      pCtx->data.init.isLegacy = TRUE;
    }
  }

  return txConnInd;
}

/*************************************************************************************************/
/*!
 *  \brief      Extended initiate state advertising packet receive (primary channel)
 *              completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pAdvBuf Received advertising buffer.
 *
 *  \return     TRUE if connect indication transmission is required, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrMstInitiateRxExtAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  lctrAdvbPduHdr_t advHdr;
  lctrUnpackAdvbPduHdr(&advHdr, pAdvBuf);

  /*** Transmit response PDU processing (legacy advertising). ***/

  bool_t txConnInd = FALSE;

  switch (advHdr.pduType)
  {
    case LL_PDU_ADV_EXT_IND:
      break;
    default:
      /* Legacy advertising. */
      txConnInd = lctrMstInitiateExtAdvPktHandler(pOp, pAdvBuf);
      break;
  }

  return txConnInd;
}

/*************************************************************************************************/
/*!
 *  \brief      Extended initiate state advertising packet receive (primary channel)
 *              post process handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pAdvBuf Received advertising buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstInitiateRxExtAdvPktPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;
  lctrExtScanCtx_t * const pExtInitCtx = pOp->pCtx;

  lctrAdvbPduHdr_t advHdr;
  lctrUnpackAdvbPduHdr(&advHdr, pAdvBuf);

  /*** Received advertising PDU post-processing. ***/

  switch (advHdr.pduType)
  {
    /* Extended advertising. */
    case LL_PDU_ADV_EXT_IND:
    {
      pExtInitCtx->extAdvHdr.extHdrFlags = 0;
      const uint8_t *pExtAdvHdr = pAdvBuf + LL_ADV_HDR_LEN;
      uint8_t extAdvHdrFlags;
      lctrUnpackExtAdvHeader(&pExtInitCtx->extAdvHdr, &extAdvHdrFlags, pExtAdvHdr);

      if (extAdvHdrFlags & LL_EXT_HDR_AUX_PTR_BIT)
      {
        lctrAuxPtr_t auxPtr;
        lctrUnpackAuxPtr(&auxPtr, pExtInitCtx->extAdvHdr.pAuxPtr);

        do
        {
          /* Confirm auxiliary PHY is supported. */
          bool_t auxPhy = lctrConvertAuxPtrPhyToBbPhy(auxPtr.auxPhy);
          if ((auxPhy == BB_PHY_BLE_2M) && ((lmgrCb.features & LL_FEAT_LE_2M_PHY) == 0))
          {
            /* 2M PHY requested, but not supported. */
            break;
          }
          if ((auxPhy == BB_PHY_BLE_CODED) && ((lmgrCb.features & LL_FEAT_LE_CODED_PHY) == 0))
          {
            /* Coded PHY requested, but not supported. */
            break;
          }

          pOp->minDurUsec = 0;  /* Update primary scan BOD min duration so that secondary scan can be scheduled. */

          /* TODO */
          /* Because of scheduling conflict between preallocated connection bod and aux scan bod,             */
          /* connection cannot be established with small connection interval(< 30ms) when coded phy is used.  */
          lctrInitiateMsg_t *pInitMsg = (lctrInitiateMsg_t *)pLctrMsg;
          if ((auxPhy == BB_PHY_BLE_CODED) && (LCTR_CONN_IND_US(pExtInitCtx->data.init.connInterval) < 30000))
          {
            lctrScanNotifyHostInitiateError(LL_ERROR_CODE_CONN_FAILED_TO_ESTABLISH, pInitMsg->peerAddrType, pInitMsg->peerAddr);
            lctrSendExtInitMsg(pExtInitCtx, LCTR_EXT_INIT_MSG_RESET);
            break;
          }

          uint32_t endTs = pScan->advStartTs +
                           BB_US_TO_BB_TICKS(SchBleCalcAdvPktDurationUsec(pBle->chan.rxPhy, pScan->advRxPhyOptions, LL_ADV_HDR_LEN + pScan->filtResults.pduLen));
          lctrMstAuxDiscoverOpCommit(pExtInitCtx, &auxPtr, pScan->advStartTs, endTs);

          if (pExtInitCtx->auxOpPending)
          {
            /* Extended advertising event continues with auxiliary BOD */
            BbSetBodTerminateFlag();
          }
        } while (FALSE);
      }
      break;
    }

    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Auxiliary initiate state advertising packet receive completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pAdvBuf Received advertising buffer.
 *
 *  \return     TRUE if scan request transmission is required, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrMstInitiateRxAuxAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_AUX_ADV_EVENT);

  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAuxAdvEvent_t * const pAuxScan = &pBle->op.mstAuxAdv;
  lctrExtScanCtx_t * const pExtInitCtx = pOp->pCtx;

  lctrAdvbPduHdr_t advHdr;
  const uint8_t *pExtAdvHdr = pAdvBuf + lctrUnpackAdvbPduHdr(&advHdr, pAdvBuf);
  uint8_t extAdvHdrFlags;
  lctrUnpackExtAdvHeader(&pExtInitCtx->extAdvHdr, &extAdvHdrFlags, pExtAdvHdr);

  /*** Extended advertising event filtering. ***/

  if ((pExtInitCtx->extAdvHdr.advMode & LCTR_ADV_MODE_CONN_BIT) != LCTR_ADV_MODE_CONN_BIT)
  {
    return FALSE;
  }

  /* AdvA is mandatory. */
  if ((extAdvHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT) == 0)
  {
    LL_TRACE_WARN0("Ignoring LL_PDU_AUX_ADV_IND due to missing mandatory advA.");
    return FALSE;
  }

  bbBlePduExtFiltParams_t params;

  params.pduType = advHdr.pduType;
  params.extHdrFlags = pExtInitCtx->extAdvHdr.extHdrFlags;
  params.peerAddr = pExtInitCtx->extAdvHdr.advAddr;
  params.peerAddrRand = advHdr.txAddrRnd;
  params.localAddr = pExtInitCtx->extAdvHdr.tgtAddr;
  params.localAddrRand = advHdr.rxAddrRnd;

  /* Go through the extended PDU filter. */
  if (BbBleExtPduFiltCheck(&params, &pOp->prot.pBle->pduFilt, FALSE, &pAuxScan->filtResults) == FALSE)
  {
    LL_TRACE_WARN0("Ignoring LL_PDU_AUX_ADV_IND due to PDU filtering.");
    return FALSE;
  }

  /*** ACAD processing. ***/

  /* TODO route ACAD */
  (void)pExtInitCtx->extAdvHdr.pAcad;

  /*** Transmit request PDU processing. ***/

  bool_t txConnInd = FALSE;

  if (!pAuxScan->filtResults.peerMatch)
  {
    /* Require peer match. */
  }
  else if (!pExtInitCtx->data.init.connBodLoaded)
  {
    BbSetBodTerminateFlag();
  }
  else if (pAuxScan->pTxAuxReqBuf)
  {
    lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(pExtInitCtx->data.init.connHandle);

    uint32_t connReqEndTs = pAuxScan->auxStartTs +
                            BB_US_TO_BB_TICKS(SchBleCalcAdvPktDurationUsec(pBle->chan.rxPhy, pAuxScan->auxRxPhyOptions, LL_ADV_HDR_LEN + advHdr.len) +
                                              LL_BLE_TIFS_US +
                                              SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, (pBle->chan.tifsTxPhyOptions != BB_PHY_OPTIONS_DEFAULT) ? pBle->chan.tifsTxPhyOptions : pAuxScan->auxRxPhyOptions, LL_ADV_HDR_LEN + LL_CONN_IND_PDU_LEN));

    /* Update auxiliary connection request header's advertiser address. */
    uint8_t *pAuxConnReqAdvA = pAuxScan->pTxAuxReqBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN;
    Bda64ToBstream(pAuxConnReqAdvA, pAuxScan->filtResults.peerAddr);
    pExtInitCtx->reqPduHdr.rxAddrRnd = pAuxScan->filtResults.peerAddrRand;

    /* Update auxiliary connection request header's initiator's address. */
    if (pExtInitCtx->data.init.ownAddrType & LL_ADDR_IDENTITY_BIT)
    {
      bool_t  localAddrRand;
      uint64_t localAddr;

      if (extAdvHdrFlags & LL_EXT_HDR_TGT_ADDR_BIT)
      {
        /* If peer is using directed advertising then use tgtA from the    */
        /* AUX_ADV_IND as our InitA in the auxiliary connect request.       */
        localAddrRand = advHdr.rxAddrRnd;
        localAddr     = pExtInitCtx->extAdvHdr.tgtAddr;
        /* Update the local RPA if the received one is RPA. */
        if (BDA64_ADDR_IS_RPA(localAddr))
        {
          pExtInitCtx->data.init.localRpa = localAddr;
          BbBleResListUpdateLocal(pAuxScan->filtResults.peerIdAddrRand, pAuxScan->filtResults.peerIdAddr, &localAddr);
        }
      }
      else
      {
        /* Otherwise, use local address, could be public, static random or RPA. */
        localAddrRand = BB_BLE_PDU_FILT_FLAG_IS_SET(&pBle->pduFilt, LOCAL_ADDR_MATCH_RAND);
        localAddr     = pBle->pduFilt.localAddrMatch;
        if (BbBleResListReadLocal(pAuxScan->filtResults.peerIdAddrRand, pAuxScan->filtResults.peerIdAddr, &localAddr))
        {
          localAddrRand = TRUE;
          pExtInitCtx->data.init.localRpa = localAddr;
        }
        else
        {
          pExtInitCtx->data.init.localRpa = 0;
        }
      }
      uint8_t *pAuxConnReqInitA = pAuxScan->pTxAuxReqBuf + LL_ADV_HDR_LEN;
      Bda64ToBstream(pAuxConnReqInitA, localAddr);
      pExtInitCtx->reqPduHdr.txAddrRnd = localAddrRand;
    }
    else
    {
      pExtInitCtx->data.init.localRpa = 0;
    }

#if (LL_ENABLE_TESTER)
    if (llTesterCb.auxReq.len)
    {
      /* Overriding AUX_CONNECT_REQ from test script. */
      memcpy(pAuxScan->pTxAuxReqBuf, llTesterCb.auxReq.buf, llTesterCb.auxReq.len);
      pAuxScan->txAuxReqLen = llTesterCb.auxReq.len;

      uint16_t hdr;
      uint8_t  *ptr = llTesterCb.auxReq.buf;
      BSTREAM_TO_UINT16(hdr, ptr);

      pExtInitCtx->reqPduHdr.txAddrRnd   = (hdr >> LCTR_ADV_HDR_TX_ADD_SHIFT)   & 0x0001;
      pExtInitCtx->reqPduHdr.rxAddrRnd   = (hdr >> LCTR_ADV_HDR_RX_ADD_SHIFT)   & 0x0001;

    }
#endif

    lctrPackAdvbPduHdr(pExtInitCtx->reqBuf, &pExtInitCtx->reqPduHdr);

    /* Update txWinOffset field in AUX_CONN_REQ PDU. */
    uint32_t txWinOffsetUsec = BB_TICKS_TO_US(pExtInitCtx->data.init.firstCeDue - connReqEndTs);
    uint16_t txWinOffset = LCTR_US_TO_CONN_IND(txWinOffsetUsec);
    uint16_t chanDelay;
    switch (pBle->chan.rxPhy)
    {
      case BB_PHY_BLE_1M:
      case BB_PHY_BLE_2M:
      default:
        chanDelay = LCTR_DATA_CHAN_DLY_AUX_UNCODED;
        break;
      case BB_PHY_BLE_CODED:
        chanDelay = LCTR_DATA_CHAN_DLY_AUX_CODED;
        break;
    }
    UINT16_TO_BUF(pExtInitCtx->reqBuf + LCTR_CONN_IND_TX_WIN_OFFSET, txWinOffset - chanDelay);

    if (txWinOffsetUsec < LCTR_CONN_IND_US(chanDelay))
    {
      LL_TRACE_WARN1("AE too close to initial CE deltaUsec=%u, suppressed LL_CONN_IND delivery", BB_TICKS_TO_US(pExtInitCtx->data.init.firstCeDue - connReqEndTs));

      /* Restart scanning with better initial CE margin. */
      BbSetBodTerminateFlag();
    }
    else if (txWinOffset > pCtx->connInterval)
    {
      LL_TRACE_WARN1("Scan period exceeded initial CE time by deltaUsec=%u, suppressed LL_CONN_IND delivery", BB_TICKS_TO_US(connReqEndTs - pExtInitCtx->data.init.firstCeDue));

      /* Restart scanning with CE in the future. */
      BbSetBodTerminateFlag();
    }
    else
    {
      txConnInd = TRUE;

      pExtInitCtx->data.init.usedChSel = LL_CH_SEL_2;   /* LL_PDU_AUX_CONNECT_REQ always uses Channel Selection #2. */
      pExtInitCtx->data.init.phy = pBle->chan.txPhy;

      pExtInitCtx->data.init.isLegacy = FALSE;
    }
  }

  return txConnInd;
}

/*************************************************************************************************/
/*!
 *  \brief      Master auxiliary connection response receive completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pRspBuf Received response buffer, NULL if receive failed.
 *
 *  \return     Always return FALSE (ignored by BB).
 */
/*************************************************************************************************/
bool_t lctrMstInitiateRxAuxConnRspHandler(BbOpDesc_t *pOp, const uint8_t *pRspBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_AUX_ADV_EVENT);

  lctrExtScanCtx_t * pExtInitCtx = pOp->pCtx;
  BbBleMstAuxAdvEvent_t * const pAuxScan = &pOp->prot.pBle->op.mstAuxAdv;

  if (pRspBuf == NULL)
  {
    return FALSE;
  }

  lctrAdvbPduHdr_t advHdr;
  const uint8_t *pExtAdvHdr = pRspBuf + lctrUnpackAdvbPduHdr(&advHdr, pRspBuf);
  uint8_t extAdvHdrFlags;
  lctrUnpackExtAdvHeader(&pExtInitCtx->extAdvHdr, &extAdvHdrFlags, pExtAdvHdr);

  /*** Extended advertising event filtering. ***/
  /*** We may bypass this PDU filtering, since the response is irrelevant. ***/

  /* AdvA and tgtA are mandatory. */
  if (((extAdvHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT) == 0) ||
      ((extAdvHdrFlags & LL_EXT_HDR_TGT_ADDR_BIT) == 0))
  {
    LL_TRACE_WARN0("LL_PDU_AUX_CONNECT_RSP missing mandatory advA and tgtA.");
  }

  bbBlePduExtFiltParams_t params;

  params.pduType = advHdr.pduType;
  params.extHdrFlags = pExtInitCtx->extAdvHdr.extHdrFlags;
  params.peerAddr = pExtInitCtx->extAdvHdr.advAddr;
  params.peerAddrRand = advHdr.txAddrRnd;
  params.localAddr = pExtInitCtx->extAdvHdr.tgtAddr;
  params.localAddrRand = advHdr.rxAddrRnd;

  if (BbBleExtPduFiltCheck(&params, &pOp->prot.pBle->pduFilt, TRUE, &pAuxScan->filtResults) == FALSE)
  {
    LL_TRACE_WARN0("LL_PDU_AUX_CONNECT_RSP failed PDU filtering.");
  }

  if ((pExtInitCtx->extAdvHdr.advMode & (LCTR_ADV_MODE_SCAN_BIT | LCTR_ADV_MODE_CONN_BIT)) != 0)
  {
    LL_TRACE_WARN1("Received auxiliary PDU with invalid advMode=0x%02x", pExtInitCtx->extAdvHdr.advMode);
  }

  /*** ACAD processing. ***/

  /* TODO route ACAD */
  (void)pExtInitCtx->extAdvHdr.pAcad;

  /* Signal termination only; initiate termination processing handled in lctrMstAuxInitiateEndOp(). */
  pExtInitCtx->selfTerm = TRUE;
  BbSetBodTerminateFlag();

  return FALSE;
}
