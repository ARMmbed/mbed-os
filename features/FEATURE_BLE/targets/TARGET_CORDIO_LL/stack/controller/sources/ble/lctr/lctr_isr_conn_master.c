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
 * \brief Link layer controller master connection ISR callbacks.
 */
/*************************************************************************************************/

#include "lctr_int.h"
#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"
#include "lctr_int_adv_master.h"
#include "lmgr_api_conn.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "bb_ble_api.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_os.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Master connection ISR control block. */
static struct
{
  uint8_t consCrcFailed;        /*!< Number of consecutive CRC failures. Used only by active operation. */
  bool_t rxFromSlave;           /*!< At least one packet received from slave. */
} lctrMstConnIsr;

/*! \brief  Check BB meets data PDU requirements. */
WSF_CT_ASSERT((BB_FIXED_DATA_PKT_LEN == 0) ||
              (BB_FIXED_DATA_PKT_LEN >= LCTR_DATA_PDU_MAX_LEN));

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

#if (LL_ENABLE_TESTER)
extern void LctrProcessRxTxAck(lctrConnCtx_t *pCtx, uint8_t *pRxBuf, uint8_t **pNextRxBuf, bool_t *pTxPduIsAcked);
#endif

/*************************************************************************************************/
/*!
 *  \brief  Update a connection operation.
 *
 *  \param  pCtx        Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lctrMstConnUpdateOp(lctrConnCtx_t *pCtx)
{
  /* Pre-resolve common structures for efficient access. */
  BbOpDesc_t * const pOp = &pCtx->connBod;
  lctrConnUpdInd_t * const pConnUpdInd = &pCtx->connUpd;

  /*** Connection context setup ***/

  const uint16_t connIntervalOld = pCtx->connInterval;
  const uint16_t latencyOld      = pCtx->maxLatency;
  const uint16_t supTimeoutMsOld = pCtx->supTimeoutMs;

  pCtx->connInterval = pConnUpdInd->interval;
  pCtx->maxLatency   = pConnUpdInd->latency;
  pCtx->supTimeoutMs = LCTR_CONN_IND_TO_MS(pConnUpdInd->timeout);

  SchRmCommitUpdate(LCTR_GET_CONN_HANDLE(pCtx));

  /*** General setup ***/

  const uint32_t txWinOffset = BB_BLE_TO_BB_TICKS(LCTR_CONN_IND_TICKS(pConnUpdInd->txWinOffset));

  pOp->due += txWinOffset;
  pOp->maxDurUsec = LCTR_CONN_IND_US(pCtx->connInterval);

  /* Unconditionally reset supervision timer with transitional value.
   *     connIntervalOld + supervisionTimeoutNew */
  WsfTimerStartMs(&pCtx->tmrSupTimeout, LCTR_CONN_IND_MS(connIntervalOld) + pCtx->supTimeoutMs);

  /*** Notifications ***/

  /* Notify host only if connection parameters changed. */
  if ((connIntervalOld != pCtx->connInterval) ||
      (latencyOld      != pCtx->maxLatency) ||
      (supTimeoutMsOld != pCtx->supTimeoutMs))
  {
    pCtx->llcpNotifyMask |= 1 << LCTR_PROC_CONN_UPD;
  }

  /* Delay notification until CE starts. */
  pCtx->llcpInstantComp = TRUE;

  LL_TRACE_INFO2("    >>> Connection updated, handle=%u, eventCounter=%u <<<", LCTR_GET_CONN_HANDLE(pCtx), pCtx->eventCounter);
  LL_TRACE_INFO1("                            connIntervalUsec=%u", LCTR_CONN_IND_US(pCtx->connInterval));
}

/*************************************************************************************************/
/*!
 *  \brief  Update the channel map.
 *
 *  \param  pCtx        Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lctrMstChanMapUpdateOp(lctrConnCtx_t *pCtx)
{
  pCtx->chanMask = pCtx->chanMapUpd.chanMask;
  lctrBuildRemapTable(pCtx);

  /* Delay notification until CE starts. */
  pCtx->llcpInstantComp = TRUE;

  LL_TRACE_INFO2("    >>> Channel map updated, handle=%u, eventCounter=%u <<<", LCTR_GET_CONN_HANDLE(pCtx), pCtx->eventCounter);
}

/*************************************************************************************************/
/*!
 *  \brief  Update the selected PHY.
 *
 *  \param  pCtx        Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lctrMstPhyUpdateOp(lctrConnCtx_t *pCtx)
{
  uint8_t txPhyOld = lctrPhyToPhysBit(pCtx->bleData.chan.txPhy);
  uint8_t rxPhyOld = lctrPhyToPhysBit(pCtx->bleData.chan.rxPhy);

  /* Notify host only if PHY changed. */
  if ((pCtx->phyUpd.masterToSlavePhy != LL_PHYS_NONE) &&
      (pCtx->phyUpd.masterToSlavePhy != txPhyOld))
  {
    pCtx->bleData.chan.txPhy = lctrPhysBitToPhy(pCtx->phyUpd.masterToSlavePhy);
    pCtx->llcpNotifyMask    |= 1 << LCTR_PROC_PHY_UPD;
  }
  if ((pCtx->phyUpd.slaveToMasterPhy != LL_PHYS_NONE) &&
      (pCtx->phyUpd.slaveToMasterPhy != rxPhyOld))
  {
    pCtx->bleData.chan.rxPhy = lctrPhysBitToPhy(pCtx->phyUpd.slaveToMasterPhy);
    pCtx->llcpNotifyMask    |= 1 << LCTR_PROC_PHY_UPD;
  }

  /* Delay notification until CE starts. */
  pCtx->llcpInstantComp = TRUE;

  /* Update connection event durations. */
  pCtx->effConnDurUsec   = lctrCalcConnDurationUsec(pCtx, &pCtx->effDataPdu);
  pCtx->localConnDurUsec = lctrCalcConnDurationUsec(pCtx, &pCtx->localDataPdu);

  LL_TRACE_INFO2("    >>> PHY updated, handle=%u, eventCounter=%u <<<", LCTR_GET_CONN_HANDLE(pCtx), pCtx->eventCounter);
}

/*************************************************************************************************/
/*!
 *  \brief  Begin a connection operation.
 *
 *  \param  pOp     Begin operation.
 *
 *  \return None.
 *
 *  \note   Scheduler may call this routine multiple times in the following situations:
 *          (1) this BOD is pending and a BOD is inserted before, or (2) a Data PDU is queued
 *          into an empty list.
 */
/*************************************************************************************************/
void lctrMstConnBeginOp(BbOpDesc_t *pOp)
{
  lctrConnCtx_t * const pCtx = pOp->pCtx;
  uint8_t *pBuf;

  if (lctrCheckForLinkTerm(pCtx))
  {
    BbSetBodTerminateFlag();
    return;
  }

  if (pLctrVsHdlrs && pLctrVsHdlrs->ceSetup)
  {
    pLctrVsHdlrs->ceSetup(LCTR_GET_CONN_HANDLE(pCtx));
  }

  /*** Initialize connection event resources. ***/

  lctrMstConnIsr.consCrcFailed = 0;
  lctrMstConnIsr.rxFromSlave = FALSE;

  /*** Setup for transmit ***/

  pCtx->rxHdr.md   = 0;                                       /* clear Rx header */
  pCtx->rxHdr.len  = 0;
  pCtx->rxHdr.llid = LL_LLID_EMPTY_PDU;

  /* Guarantee Tx at least 1 packet per CE. */
  (void)lctrSetupForTx(pCtx, BB_STATUS_SUCCESS /* does not matter */, TRUE);    /* always returns TRUE */

  /* Check for Tx failure. */
  if (BbGetBodTerminateFlag())
  {
    return;
  }

  /*** Setup for receive ***/

  if ((pBuf = lctrRxPduAlloc(pCtx->localDataPdu.maxRxLen)) != NULL)
  {
    lctrSetBbPacketCounterRx(pCtx);
    BbBleRxData(pBuf, LCTR_DATA_PDU_LEN(pCtx->localDataPdu.maxRxLen));

    /* Rx may fail; no more important statements in this code path */
  }
  else
  {
    LL_TRACE_ERR1("!!! OOM while initializing receive buffer at start of CE, handle=%u", LCTR_GET_CONN_HANDLE(pCtx));
    BbSetBodTerminateFlag();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Cleanup a connection operation.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstConnCleanupOp(BbOpDesc_t *pOp)
{
  lctrConnCtx_t * const pCtx = pOp->pCtx;

  /* Reset operation to state before BOD began */
  if (pCtx->emptyPduPend &&
      pCtx->emptyPduFirstAtt)
  {
    /* A Tx was setup for a first empty PDU, but that empty PDU was never transmitted. */
    pCtx->emptyPduPend = FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  End a connection operation.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstConnEndOp(BbOpDesc_t *pOp)
{
  /* Pre-resolve common structures for efficient access. */
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstConnEvent_t * const pConn = &pBle->op.mstConn;
  lctrConnCtx_t * const pCtx = pOp->pCtx;

  /* Process event completion */

  if (!pCtx->connEst && (lctrMstConnIsr.rxFromSlave || (lctrMstConnIsr.consCrcFailed > 0)))
  {
    lctrStoreConnTimeoutTerminateReason(pCtx);
    WsfTimerStartMs(&pCtx->tmrSupTimeout, pCtx->supTimeoutMs);

    pCtx->connEst = TRUE;
  }
  else if (lctrMstConnIsr.rxFromSlave)
  {
    /* Reset supervision timer. */
    WsfTimerStartMs(&pCtx->tmrSupTimeout, pCtx->supTimeoutMs);
  }

  pCtx->rssi = pConn->rssi;

  SchRmSetReference(LCTR_GET_CONN_HANDLE(pCtx));

  if (pCtx->checkCisTerm)
  {
    (pCtx->checkCisTerm)(LCTR_GET_CONN_HANDLE(pCtx));
  }

  /* Terminate connection */
  if (lctrCheckForLinkTerm(pCtx))
  {
    lctrSendConnMsg(pCtx, LCTR_CONN_TERMINATED);
    WsfTimerStop(&pCtx->tmrSupTimeout);
    return;
  }

  if (pCtx->data.mst.sendConnUpdInd)
  {
    uint8_t *pPdu;

    if ((pPdu = lctrTxCtrlPduAlloc(LL_CONN_UPD_IND_PDU_LEN)) != NULL)
    {
      pCtx->data.mst.sendConnUpdInd = FALSE;

      uint16_t ceOffset;
#if (LL_ENABLE_TESTER)
      if (llTesterCb.eventCounterOverride == TRUE)
      {
        ceOffset = llTesterCb.eventCounterOffset + 1;          /* +1 for next CE */
      }
      else
#endif
      {
        ceOffset = LL_MIN_INSTANT + 1 +          /* +1 for next CE */
                   pCtx->maxLatency;             /* ensure slave will listen this packet */

        /* TODO: accommodate pCtx->connParam.offset[]. */
      }
      pCtx->connUpd.instant = pCtx->eventCounter + ceOffset;

      /* Use smallest txWindowOffset (i.e. 0) to minimize data loss. */
      uint32_t txWinOffsetUsec = SchRmGetOffsetUsec(0, LCTR_GET_CONN_HANDLE(pCtx), pOp->due);
      pCtx->connUpd.txWinOffset = LCTR_US_TO_CONN_IND(txWinOffsetUsec);

      lctrPackConnUpdInd(pPdu, &pCtx->connUpd);
      lctrTxCtrlPduQueue(pCtx, pPdu);
    }
    /* else retry at next lctrMstConnEndOp() event. */
  }

  if (pCtx->sendPerSync)
  {
    pCtx->sendPerSync = FALSE;
    if (pCtx->perSyncSrc == LCTR_SYNC_SRC_SCAN)
    {
      if (lctrSendPerSyncFromScanFn)
      {
        lctrSendPerSyncFromScanFn(pCtx);
      }
    }
    else  /* (pCtx->perSyncSrc == LCTR_SYNC_SRC_BCST) */
    {
      if (lctrSendPerSyncFromBcstFn)
      {
        lctrSendPerSyncFromBcstFn(pCtx);
      }
    }
  }

  /*** Update for next operation ***/

  uint32_t anchorPoint           = pOp->due;
  uint16_t anchorPointOffsetUsec = pOp->dueOffsetUsec;
  uint16_t numIntervals          = 0;

  if (pBle->chan.tifsTxPhyOptions != BB_PHY_OPTIONS_DEFAULT)
  {
    /* Set PHY options to host defined behavior. */
    pBle->chan.initTxPhyOptions = pBle->chan.tifsTxPhyOptions;
  }
  else
  {
    /* Set PHY options to RX PHY Options*/
    pBle->chan.initTxPhyOptions = pConn->rxPhyOptions;
  }

  while (TRUE)
  {
    numIntervals += 1;
    pCtx->eventCounter += 1;

    uint32_t connInterUsec = LCTR_CONN_IND_US(numIntervals * pCtx->connInterval) + anchorPointOffsetUsec;
    uint32_t connInter     = BB_US_TO_BB_TICKS(connInterUsec);
    int16_t  dueOffsetUsec = connInterUsec - BB_TICKS_TO_US(connInter);
#if (LL_ENABLE_TESTER)
    if (llTesterCb.connIntervalUs)
    {
      connInterUsec = (numIntervals * llTesterCb.connIntervalUs) + anchorPointOffsetUsec;
      connInter     = BB_US_TO_BB_TICKS(connInterUsec);
      dueOffsetUsec = 0;
    }
#endif

    /* Advance to next interval. */
    pOp->due           = anchorPoint + connInter;
    pOp->dueOffsetUsec = WSF_MAX(dueOffsetUsec, 0);

    if ((pCtx->llcpActiveProc == LCTR_PROC_CONN_UPD) &&
        (pCtx->eventCounter == pCtx->connUpd.instant))
    {
      lctrMstConnUpdateOp(pCtx);
    }
    else if ((pCtx->llcpActiveProc == LCTR_PROC_CMN_CH_MAP_UPD) &&
             (pCtx->eventCounter == pCtx->chanMapUpd.instant))
    {
      lctrMstChanMapUpdateOp(pCtx);
    }
    else if ((pCtx->llcpActiveProc == LCTR_PROC_PHY_UPD) &&
             (pCtx->eventCounter == pCtx->phyUpd.instant))
    {
      lctrMstPhyUpdateOp(pCtx);
    }

    pBle->chan.chanIdx = lctrChSelHdlr[pCtx->usedChSel](pCtx, 0);

    if (SchInsertAtDueTime(pOp, lctrConnResolveConflict))
    {
      break;
    }

    LL_TRACE_WARN2("!!! CE schedule conflict handle=%u, eventCounter=%u", LCTR_GET_CONN_HANDLE(pCtx), pCtx->eventCounter);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Abort a connection operation.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstConnAbortOp(BbOpDesc_t *pOp)
{
  lctrConnCtx_t * const pCtx = pOp->pCtx;

  /* Reset operation to state before BOD began */
  if (pCtx->emptyPduPend &&
      pCtx->emptyPduFirstAtt)
  {
    /* A Tx was setup for a first empty PDU, but that empty PDU was never transmitted. */
    pCtx->emptyPduPend = FALSE;
  }

  if (pCtx->state == LCTR_CONN_STATE_INITIALIZED)
  {
    return;
  }

  lctrMstConnEndOp(pOp);
}

/*************************************************************************************************/
/*!
 *  \brief      Complete a transmitted data buffer.
 *
 *  \param      pOp     Operation context.
 *  \param      status  Transmit status.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstConnTxCompletion(BbOpDesc_t *pOp, uint8_t status)
{
  if (status == BB_STATUS_SUCCESS)
  {
    lctrConnCtx_t * const pCtx = pOp->pCtx;

    lctrSetControlPduAck(pCtx);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Complete a received data buffer.
 *
 *  \param      pOp     Operation context.
 *  \param      pRxBuf  Next receive buffer or NULL to flow control.
 *  \param      status  Receive status.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstConnRxCompletion(BbOpDesc_t *pOp, uint8_t *pRxBuf, uint8_t status)
{
  lctrConnCtx_t * const pCtx = pOp->pCtx;

  /* Local state */
  uint8_t *pNextRxBuf = NULL;
  bool_t loadRxBuf = FALSE;
  bool_t txPduIsAcked = FALSE;

  /*** LLCP instant completion processing ***/

  if (pCtx->llcpInstantComp)
  {
    pCtx->llcpInstantComp = FALSE;
    lctrSendConnMsg(pCtx, LCTR_CONN_LLCP_PROC_CMPL);
  }

  /*** Cancellation processing ***/

  if (status == BB_STATUS_CANCELED)
  {
    lctrRxPduFree(pRxBuf);
    goto Done;
  }

  /*** Connection event pre-processing ***/

  if (lctrCheckForLinkTerm(pCtx) ||
      (status == BB_STATUS_FAILED) ||
      (status == BB_STATUS_RX_TIMEOUT))
  {
    if (status == BB_STATUS_RX_TIMEOUT)
    {
      LL_TRACE_WARN3("lctrMstConnRxCompletion: BB failed with status=RX_TIMEOUT, eventCounter=%u, bleChan=%u, handle=%u", pCtx->eventCounter, pCtx->bleData.chan.chanIdx, LCTR_GET_CONN_HANDLE(pCtx));
    }

    if (status == BB_STATUS_FAILED)
    {
      LL_TRACE_ERR3("lctrMstConnRxCompletion: BB failed with status=FAILED, eventCounter=%u, bleChan=%u, handle=%u", pCtx->eventCounter, pCtx->bleData.chan.chanIdx, LCTR_GET_CONN_HANDLE(pCtx));
    }

    BbSetBodTerminateFlag();
    lctrRxPduFree(pRxBuf);
    goto Done;
  }

  /*** Receive packet pre-processing ***/

  if (status == BB_STATUS_SUCCESS)
  {
    lctrMstConnIsr.rxFromSlave = TRUE;

    /* Reset consecutive CRC failure counter. */
    lctrMstConnIsr.consCrcFailed = 0;
  }
  else if (status == BB_STATUS_CRC_FAILED)
  {
    lctrMstConnIsr.consCrcFailed++;
    if (lctrMstConnIsr.consCrcFailed >= LCTR_MAX_CONS_CRC)
    {
      /* Close connection event. */
      BbSetBodTerminateFlag();
      lctrRxPduFree(pRxBuf);
      goto Done;
    }
    goto SetupTx;
  }

  lctrUnpackDataPduHdr(&pCtx->rxHdr, pRxBuf);

#if (LL_ENABLE_TESTER)
  if (llTesterCb.ackMode != LL_TESTER_ACK_MODE_NORMAL)
  {
    LctrProcessRxTxAck(pCtx, pRxBuf, &pNextRxBuf, &txPduIsAcked);
  }
  else
#endif
  {
    pNextRxBuf = lctrProcessRxAck(pCtx);
    txPduIsAcked = lctrProcessTxAck(pCtx);
  }

  /*** Scheduler termination ***/

  if (lctrExceededMaxDur(pCtx, pOp->due, pCtx->effConnDurUsec))
  {
    BbSetBodTerminateFlag();
    goto PostProcessing;
  }

  /*** Setup for transmit ***/
SetupTx:

  /* Optionally require Tx if last packet was NACK'ed. */
  txPduIsAcked = txPduIsAcked || !lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_MST_RETX_AFTER_RX_NACK);
  if (lctrSetupForTx(pCtx, status, !txPduIsAcked) == 0)
  {
    BbSetBodTerminateFlag();
    goto PostProcessing;
  }

  /* If Tx failed, return immediately after queueing Rx data. */
  if (BbGetBodTerminateFlag())
  {
    goto PostProcessing;
  }

  /*** Setup for receive ***/

  /* Always Rx after a Tx. */
  loadRxBuf = TRUE;

  /*** Packet post-processing ***/
PostProcessing:

  /* Tx post-processing. */
  lctrProcessTxAckCleanup(pCtx);

  /* Rx post-processing. */
  lctrCheckControlPduAck(pCtx);
  lctrRxPostProcessing(pCtx, pRxBuf, pNextRxBuf, loadRxBuf);

  /*** ISR complete ***/
Done:
  return;
}
