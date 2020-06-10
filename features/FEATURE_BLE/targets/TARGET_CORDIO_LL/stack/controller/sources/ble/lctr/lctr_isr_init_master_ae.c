/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller master advertising event ISR callbacks.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
  pOp->dueUsec = pExtInitCtx->data.init.scanWinStartUsec;

  if ((pExtInitCtx->data.init.param.scanInterval != pExtInitCtx->data.init.param.scanWindow) &&
      ((pScan->elapsedUsec + pOp->minDurUsec) < LCTR_BLE_TO_US(pExtInitCtx->data.init.param.scanWindow)))
  {
    const uint32_t min = pScan->elapsedUsec;
    const uint32_t max = BB_BLE_TO_US(pExtInitCtx->data.init.param.scanWindow);

    if (SchInsertEarlyAsPossible(pOp, min, max))
    {
      /* Continue interrupted operation. */
      pScan->elapsedUsec = BbGetTargetTimeDelta(pOp->dueUsec, pExtInitCtx->data.init.scanWinStartUsec);
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
    pExtInitCtx->data.init.scanWinStartUsec = pOp->dueUsec;
  }
  else
  {
    /* Next scan interval. */
    const uint32_t min = BB_BLE_TO_US(pExtInitCtx->data.init.param.scanInterval);
    const uint32_t max = min + BB_BLE_TO_US(pExtInitCtx->data.init.param.scanWindow);

    while (TRUE)
    {
      /* Store start of next scan window. */
      pExtInitCtx->data.init.scanWinStartUsec = pOp->dueUsec + min;

      if (SchInsertEarlyAsPossible(pOp, min, max))
      {
        pScan->elapsedUsec = BbGetTargetTimeDelta(pOp->dueUsec, pExtInitCtx->data.init.scanWinStartUsec);
        WSF_ASSERT(pScan->elapsedUsec < pOp->maxDurUsec);
        break;
      }
      else
      {
        /* Advance to next scan window. */
        pOp->dueUsec = pExtInitCtx->data.init.scanWinStartUsec;

        LL_TRACE_WARN1("!!! Scan schedule conflict at dueUsec=%u", pOp->dueUsec + min);
        LL_TRACE_WARN1("!!!                           scanWindowUsec=%u", LCTR_BLE_TO_US(pExtInitCtx->data.init.param.scanWindow));
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  End a initiate scan operation in the master role.
 *
 *  \param  pOp     Completed operation.
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
 */
/*************************************************************************************************/
void lctrMstExtInitiateEndOp(BbOpDesc_t *pOp)
{
  lctrExtScanCtx_t * const pExtInitCtx = pOp->pCtx;
  lctrConnCtx_t * pCtx = LCTR_GET_CONN_CTX(pExtInitCtx->data.init.connHandle);
  BbOpDesc_t * pConnBod = &pCtx->connBod;

  if (pExtInitCtx->selfTerm &&    /* LL_CONN_IND Tx Complete. */
      !pExtInitCtx->auxOpPending) /* No aux scan bod pending
                                   * (means that the self termination was generated from a legacy advertisement connection). */
  {
    uint16_t numIntervals = 0;
    uint32_t anchorPointUsec = pConnBod->dueUsec;

    while (TRUE)
    {
      if (SchInsertAtDueTime(pConnBod, lctrConnResolveConflict))
      {
        break;
      }

      LL_TRACE_WARN2("!!!Establish CE Schedule conflict handle=%u, evtCtr=%u", LCTR_GET_CONN_HANDLE(pCtx), pCtx->eventCounter);

      numIntervals++;
      pCtx->eventCounter++;

      uint32_t connInterUsec = LCTR_CONN_IND_US(numIntervals * pCtx->connInterval);

      pConnBod->dueUsec = anchorPointUsec + connInterUsec;
    }
  }

  lctrMstExtInitiateScanEndOp(pOp);
}

/*************************************************************************************************/
/*!
 *  \brief  End an auxiliary initiate operation in the master role.
 *
 *  \param  pOp     Completed operation.
 */
/*************************************************************************************************/
void lctrMstAuxInitiateEndOp(BbOpDesc_t *pOp)
{
  lctrExtScanCtx_t * const pExtInitCtx = pOp->pCtx;
  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(pExtInitCtx->data.init.connHandle);
  BbOpDesc_t * pConnBod = &pCtx->connBod;

  if (pExtInitCtx->selfTerm)       /* Implies a LL_CONN_IND Tx completion. */
  {
    uint16_t numIntervals = 0;
    uint32_t anchorPointUsec = pConnBod->dueUsec;

    while (TRUE)
    {
      if (SchInsertAtDueTime(pConnBod, lctrConnResolveConflict))
      {
        break;
      }

      LL_TRACE_WARN2("!!!Establish CE Schedule conflict handle=%u, evtCtr=%u", LCTR_GET_CONN_HANDLE(pCtx), pCtx->eventCounter);

      numIntervals++;
      pCtx->eventCounter++;

      uint32_t connInterUsec = LCTR_CONN_IND_US(numIntervals * pCtx->connInterval);

      pConnBod->dueUsec = anchorPointUsec + connInterUsec;
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
  lctrConnCtx_t *pConnCtx = LCTR_GET_CONN_CTX(pCtx->data.init.connHandle);
  BbOpDesc_t * const pConnBod = &pConnCtx->connBod;
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

  uint32_t advEndTs = pScan->advStartTsUsec +
                      SchBleCalcAdvPktDurationUsec(pBle->chan.rxPhy, pScan->advRxPhyOptions,
                                                                     LL_ADV_HDR_LEN + pScan->filtResults.pduLen);
  uint32_t refTime = advEndTs;

  /*** Transmit response PDU processing. ***/

  bool_t txConnInd = FALSE;

  if (!pScan->filtResults.peerMatch)
  {
    /* Require peer match. */
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
    refTime += LL_BLE_TIFS_US;
    refTime += LCTR_CONN_IND_PKT_1M_US;
    refTime += LCTR_CONN_IND_US(LCTR_DATA_CHAN_DLY);

    /* Now reference time is the start of transmitWindowOffset. */
    uint32_t txWinOffsetUsec = SchRmGetOffsetUsec(LCTR_CONN_IND_US(pCtx->data.init.connInterval), LCTR_GET_CONN_HANDLE(pConnCtx), refTime);
    uint16_t txWinOffset = LCTR_US_TO_CONN_IND(txWinOffsetUsec);

    UINT16_TO_BUF(pScan->pTxReqBuf + LCTR_CONN_IND_TX_WIN_OFFSET, txWinOffset);
    pCtx->data.init.connInd.txWinOffset = txWinOffset;

    /*** Update due time of first CE. ***/
    pConnBod->dueUsec = refTime + txWinOffsetUsec;

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
          uint8_t auxPhy = lctrConvertAuxPtrPhyToBbPhy(auxPtr.auxPhy);
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

          uint32_t endTs = pScan->advStartTsUsec +
                           SchBleCalcAdvPktDurationUsec(pBle->chan.rxPhy, pScan->advRxPhyOptions, LL_ADV_HDR_LEN + pScan->filtResults.pduLen);
          lctrMstAuxDiscoverOpCommit(pExtInitCtx, &auxPtr, pScan->advStartTsUsec, endTs);

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
    LL_TRACE_WARN0("Ignoring LL_PDU_AUX_ADV_IND due to missing mandatory advA");
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
  else if (pAuxScan->pTxAuxReqBuf)
  {
    lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(pExtInitCtx->data.init.connHandle);
    BbOpDesc_t * const pConnBod = &pCtx->connBod;

    uint32_t connReqEndTs = pAuxScan->auxStartTsUsec +
                            SchBleCalcAdvPktDurationUsec(pBle->chan.rxPhy, pAuxScan->auxRxPhyOptions, LL_ADV_HDR_LEN + advHdr.len) +
                                              LL_BLE_TIFS_US +
                                              SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, (pBle->chan.tifsTxPhyOptions != BB_PHY_OPTIONS_DEFAULT) ? pBle->chan.tifsTxPhyOptions : pAuxScan->auxRxPhyOptions, LL_ADV_HDR_LEN + LL_CONN_IND_PDU_LEN);
    uint32_t refTime = connReqEndTs;

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
    uint16_t chanDelay = (pBle->chan.rxPhy == BB_PHY_BLE_CODED) ? LCTR_DATA_CHAN_DLY_AUX_CODED : LCTR_DATA_CHAN_DLY_AUX_UNCODED;
    refTime += LCTR_CONN_IND_US(chanDelay);

    /* Now reference time is the start of transmitWindowOffset. */
    uint32_t txWinOffsetUsec = SchRmGetOffsetUsec(LCTR_CONN_IND_US(pCtx->connInterval), LCTR_GET_CONN_HANDLE(pCtx), refTime);
    uint16_t txWinOffset = LCTR_US_TO_CONN_IND(txWinOffsetUsec);

    UINT16_TO_BUF(pExtInitCtx->reqBuf + LCTR_CONN_IND_TX_WIN_OFFSET, txWinOffset);

    /*** Update due time of first CE. ***/
    pConnBod->dueUsec = refTime + txWinOffsetUsec;

    txConnInd = TRUE;

    pExtInitCtx->data.init.usedChSel = LL_CH_SEL_2;   /* LL_PDU_AUX_CONNECT_REQ always uses Channel Selection #2. */
    pExtInitCtx->data.init.phy = pBle->chan.txPhy;

    pExtInitCtx->data.init.isLegacy = FALSE;
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
    LL_TRACE_WARN0("LL_PDU_AUX_CONNECT_RSP missing mandatory advA and tgtA");
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
    LL_TRACE_WARN0("LL_PDU_AUX_CONNECT_RSP failed PDU filtering");
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
