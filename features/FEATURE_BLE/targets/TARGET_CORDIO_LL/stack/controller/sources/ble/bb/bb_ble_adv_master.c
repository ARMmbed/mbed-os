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
 * \brief Scanning master BLE baseband porting implementation file.
 */
/*************************************************************************************************/

#include "bb_api.h"
#include "pal_bb.h"
#include "bb_ble_int.h"
#include "sch_api.h"
#include "wsf_math.h"
#include <string.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief    Event states for scan operations. */
enum
{
  BB_EVT_STATE_RX_ADV_IND,
  BB_EVT_STATE_TX_SCAN_OR_CONN_IND,
  BB_EVT_STATE_RX_SCAN_RSP
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

BbBleScanPktStats_t bbScanStats;          /*!< Scan packet statistics. */

extern const BbRtCfg_t *pBbRtCfg;

/*************************************************************************************************/
/*!
 *  \brief      Calculate remaining scan duration.
 *
 *  \param      pBod        Pointer to the BOD.
 *  \param      pScan       Pointer to the scan event operation data.
 *  \param      refTime     Reference time.
 *  \param      setupUsec   Setup delay required in microseconds.
 *
 *  \return     Scan duration in microseconds.
 *
 *  This function is calculates the duration of the scan examining:
 *      - total elapsed scan time
 *      - neighboring BOD
 *      - maximum scan period
 */
/*************************************************************************************************/
static uint32_t bbBleCalcScanDurationUsec(BbOpDesc_t *pBod, BbBleMstAdvEvent_t *pScan, uint32_t refTime, uint32_t setupUsec)
{
  WSF_ASSERT(pBod->maxDurUsec > 0);

  const uint32_t totalGapUsec = BB_SCAN_GUARD_US + setupUsec;

  if ((pScan->elapsedUsec + totalGapUsec) > pBod->maxDurUsec)
  {
    /* Scan window completed. */
    pScan->elapsedUsec = pBod->maxDurUsec;
    return 0;
  }

  uint32_t remTimeUsec = pBod->maxDurUsec - pScan->elapsedUsec;

  if (pBod->pNext)
  {
    uint32_t timeToNextOpUsec = BB_TICKS_TO_US(pBod->pNext->due - refTime);

    /* Limit scanning to the edge of neighboring BOD. */
    remTimeUsec = WSF_MIN(remTimeUsec, timeToNextOpUsec);

    if (remTimeUsec < totalGapUsec)
    {
      pScan->elapsedUsec += remTimeUsec;
      return 0;
    }
  }

  /* Reduce time such that advertising exchange can complete before end of the receive window. */
  remTimeUsec -= totalGapUsec;

  /* Limit scanning to the maximum scan period. */
  remTimeUsec = WSF_MIN(remTimeUsec, (pBbRtCfg->maxScanPeriodMs * 1000));

  return remTimeUsec;
}

/*************************************************************************************************/
/*!
 *  \brief      Continue scan operation.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *  \param      pScan   Pointer to the scan event operation data.
 *
 *  \return     TRUE if the BOD is completed, FALSE if operation continues.
 *
 *  This function is called to continue scan operation.
 */
/*************************************************************************************************/
static bool_t bbContScanOp(BbOpDesc_t *pBod, BbBleMstAdvEvent_t *pScan)
{
  if (BbGetBodTerminateFlag())
  {
    /* Client terminated. */
    return TRUE;
  }

  uint32_t curTime = PalBbGetCurrentTime(USE_RTC_BB_CLK);
  pScan->elapsedUsec += BB_TICKS_TO_US(curTime - bbBleCb.lastScanStart);
  bbBleCb.lastScanStart = curTime;

  uint32_t scanDurUsec;

  if ((scanDurUsec = bbBleCalcScanDurationUsec(pBod, pScan, curTime, BbGetSchSetupDelayUs())) == 0)
  {
    return TRUE;
  }

  bbBleCb.bbParam.due = bbBleCb.lastScanStart + BB_US_TO_BB_TICKS(BbGetSchSetupDelayUs());
  bbBleCb.bbParam.dueOffsetUsec = 0;
  bbBleCb.bbParam.rxTimeoutUsec = scanDurUsec;
  PalBbBleSetDataParams(&bbBleCb.bbParam);

  bbBleCb.evtState = BB_EVT_STATE_RX_ADV_IND;

  if (pScan->pTxReqBuf)
  {
    bbBleSetIfs();    /* active scan or initiating */
  }
  else
  {
    bbBleClrIfs();    /* passive scan */
  }
  PalBbBleRxData(pScan->pRxAdvBuf, BB_ADVB_MAX_LEN);

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Tx completion for scanning master operation.
 *
 *  \param      status      Completion status.
 *
 *  \return     None.
 *
 *  Setup for next action in the operation or complete the operation.
 */
/*************************************************************************************************/
static void bbMstScanTxCompCback(uint8_t status)
{
  BB_ISR_START();

  WSF_ASSERT(BbGetCurrentBod());

  BbOpDesc_t * const pCur = BbGetCurrentBod();
  BbBleMstAdvEvent_t * const pScan = &pCur->prot.pBle->op.mstAdv;

  bool_t bodComplete = FALSE;
  bool_t bodCont = FALSE;

  switch (bbBleCb.evtState++)
  {
    case BB_EVT_STATE_TX_SCAN_OR_CONN_IND:
    {
      switch (status)
      {
        case BB_STATUS_SUCCESS:
        {
          WSF_ASSERT(pScan->txReqCback);
          WSF_ASSERT(pScan->pTxReqBuf);

          if (pScan->txReqCback(pCur, pScan->pTxReqBuf))
          {
            WSF_ASSERT(pScan->pRxRspBuf);

            BB_ISR_MARK(bbScanStats.rxSetupUsec);

            bbBleClrIfs();      /* last operation in event */
            PalBbBleRxTifsData(pScan->pRxRspBuf, BB_ADVB_MAX_LEN);

            /* Rx may fail; no more important statements in the !bodComplete code path */
          }
          else
          {
            bodCont = TRUE;
          }

          break;
        }

        case BB_STATUS_FAILED:
        default:
          bodComplete = TRUE;
          break;
      }

      /* Update statistics. */
      switch (status)
      {
        case BB_STATUS_SUCCESS:
          BB_INC_STAT(bbScanStats.txReq);
          break;
        default:
          BB_INC_STAT(bbScanStats.errScan);
          break;
      }

      break;
    }

    default:          /* unexpected state */
      WSF_ASSERT(FALSE);
      break;
  }

  if (bodCont)
  {
    /* Cancel TIFS timer if active. */
    /* coverity[dead_error_condition] */
    switch (status)
    {
      case BB_STATUS_SUCCESS:
        PalBbBleCancelTifs();
        break;
      default:
        /* coverity[dead_error_begin] */
        break;
    }

    bodComplete = bbContScanOp(pCur, pScan);
  }

  if (bodComplete)
  {
    BbTerminateBod();
  }

  BB_ISR_MARK(bbScanStats.txIsrUsec);
}

/*************************************************************************************************/
/*!
 *  \brief      Rx completion for scanning master operation.
 *
 *  \param      status          Reception status.
 *  \param      rssi            RSSI value.
 *  \param      crc             CRC value.
 *  \param      timestamp       Start of packet timestamp.
 *  \param      rxPhyOptions    Rx PHY options.
 *
 *  \return     None.
 *
 *  Setup for next action in the operation or complete the operation.
 */
/*************************************************************************************************/
static void bbMstScanRxCompCback(uint8_t status, int8_t rssi, uint32_t crc, uint32_t timestamp, uint8_t rxPhyOptions)
{
  BB_ISR_START();

  WSF_ASSERT(BbGetCurrentBod());

  BbOpDesc_t * const pCur = BbGetCurrentBod();
  BbBleData_t * const pBle = pCur->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

  bool_t bodComplete = FALSE;
  bool_t bodCont = FALSE;

  switch (bbBleCb.evtState++)
  {
    case BB_EVT_STATE_RX_ADV_IND:
    {
      switch (status)
      {
        case BB_STATUS_SUCCESS:
        {
          WSF_ASSERT(pScan->rxAdvCback);
          WSF_ASSERT(pScan->pRxAdvBuf);

          pScan->advRssi = rssi;
          pScan->advCrc = crc;
          pScan->advStartTs = timestamp;
          pScan->advRxPhyOptions = rxPhyOptions;

          bool_t pduAllow = BbBlePduFiltCheck(pScan->pRxAdvBuf, &pBle->pduFilt, FALSE, &pScan->filtResults);

          if (pduAllow && pScan->rxAdvCback(pCur, pScan->pRxAdvBuf))
          {
            WSF_ASSERT(pScan->pTxReqBuf);

            BB_ISR_MARK(bbScanStats.txSetupUsec);

            PalBbBleTxBufDesc_t desc = {.pBuf = pScan->pTxReqBuf, .len = pScan->txReqLen};

            bbBleSetIfs();
            PalBbBleTxTifsData(&desc, 1);

            /* Tx may fail; no more important statements in the !bodComplete code path */
          }
          else
          {
            bodCont = TRUE;
          }

          if (pduAllow && pScan->rxAdvPostCback)
          {
            pScan->rxAdvPostCback(pCur, pScan->pRxAdvBuf);
          }

          break;
        }

        case BB_STATUS_RX_TIMEOUT:
        case BB_STATUS_CRC_FAILED:
          bodCont = TRUE;
          break;

        case BB_STATUS_FAILED:
        default:
          bodComplete = TRUE;
          break;
      }

      /* Update statistics. */
      switch (status)
      {
        case BB_STATUS_SUCCESS:
          BB_INC_STAT(bbScanStats.rxAdv);
          break;
        case BB_STATUS_RX_TIMEOUT:
          BB_INC_STAT(bbScanStats.rxAdvTimeout);
          break;
        case BB_STATUS_CRC_FAILED:
          BB_INC_STAT(bbScanStats.rxAdvCrc);
          break;
        case BB_STATUS_FAILED:
        default:
          BB_INC_STAT(bbScanStats.errScan);
          break;
      }

      break;
    }

    case BB_EVT_STATE_RX_SCAN_RSP:
    {
      WSF_ASSERT(pScan->rxRspCback);
      WSF_ASSERT(pScan->pRxRspBuf);

      switch (status)
      {
        case BB_STATUS_SUCCESS:
          if (BbBlePduFiltCheck(pScan->pRxRspBuf, &pBle->pduFilt, FALSE, &pScan->filtResults))
          {
            pScan->rxRspCback(pCur, pScan->pRxRspBuf);
          }
          bodCont = TRUE;
          break;

        case BB_STATUS_RX_TIMEOUT:
        case BB_STATUS_CRC_FAILED:
          pScan->rxRspCback(pCur, NULL);
          bodCont = TRUE;
          break;

        case BB_STATUS_FAILED:
        default:
          bodComplete = TRUE;
          break;
      }

      /* Update statistics. */
      switch (status)
      {
        case BB_STATUS_SUCCESS:
          BB_INC_STAT(bbScanStats.rxRsp);
          break;
        case BB_STATUS_RX_TIMEOUT:
          BB_INC_STAT(bbScanStats.rxRspTimeout);
          break;
        case BB_STATUS_CRC_FAILED:
          BB_INC_STAT(bbScanStats.rxRspCrc);
          break;
        case BB_STATUS_FAILED:
        default:
          BB_INC_STAT(bbScanStats.errScan);
          break;
      }

      break;
    }

    default:        /* invalid state */
      WSF_ASSERT(FALSE);
      break;
  }

  if (bodCont)
  {
    /* Cancel TIFS timer if active. */
    switch (status)
    {
      case BB_STATUS_SUCCESS:
      case BB_STATUS_CRC_FAILED:
        PalBbBleCancelTifs();
        break;
      default:
        break;
    }

    bodComplete = bbContScanOp(pCur, pScan);
  }

  if (bodComplete)
  {
    BbTerminateBod();
  }

  BB_ISR_MARK(bbScanStats.rxIsrUsec);
}

/*************************************************************************************************/
/*!
 *  \brief      Execute scanning master BOD.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *  \param      pBle    BLE operation parameters.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbMstExecuteScanOp(BbOpDesc_t *pBod, BbBleData_t *pBle)
{
  BbBleMstAdvEvent_t * const pScan = &pBod->prot.pBle->op.mstAdv;

  if (pScan->preExecCback)
  {
    pScan->preExecCback(pBod);
  }

  uint32_t scanDurUsec;

  if ((scanDurUsec = bbBleCalcScanDurationUsec(pBod, pScan, pBod->due, 0)) == 0)
  {
    BbSetBodTerminateFlag();
    return;
  }

  bbBleCb.lastScanStart = pBod->due;

  PalBbBleSetChannelParam(&pBle->chan);

  bbBleCb.bbParam.txCback       = bbMstScanTxCompCback;
  bbBleCb.bbParam.rxCback       = bbMstScanRxCompCback;
  bbBleCb.bbParam.rxTimeoutUsec = scanDurUsec;
  bbBleCb.bbParam.due           = pBod->due;
  bbBleCb.bbParam.dueOffsetUsec = pBod->dueOffsetUsec;
  PalBbBleSetDataParams(&bbBleCb.bbParam);

  bbBleCb.evtState = 0;

  if (pScan->pTxReqBuf)
  {
    bbBleSetIfs();    /* active scan or initiating */
  }
  else
  {
    bbBleClrIfs();    /* passive scan */
  }
  PalBbBleRxData(pScan->pRxAdvBuf, BB_ADVB_MAX_LEN);

  /* Rx may fail; no more important statements in the !bodComplete code path */
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize for scanning master operations.
 *
 *  \return     None.
 *
 *  Update the operation table with scanning master operations routines.
 */
/*************************************************************************************************/
void BbBleScanMasterInit(void)
{
  bbBleRegisterOp(BB_BLE_OP_MST_ADV_EVENT, bbMstExecuteScanOp, NULL);

  memset(&bbScanStats, 0, sizeof(bbScanStats));
}

/*************************************************************************************************/
/*!
 *  \brief      Get advertising packet statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetScanStats(BbBleScanPktStats_t *pStats)
{
  *pStats = bbScanStats;
}
