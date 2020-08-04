/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      BIG master baseband porting implementation file.
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

#include "bb_api.h"
#include "bb_ble_api.h"
#include "pal_bb.h"
#include "bb_ble_int.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Combine BIS statistics; use instantiation in bb_ble_bis_slave.c. */
extern BbBleDataPktStats_t bbBisStats;    /*!< BIS packet statistics. */

/*************************************************************************************************/
/*!
 *  \brief      Rx completion for BIS master operation.
 *
 *  \param      status      Completion status.
 *  \param      rssi            RSSI value.
 *  \param      crc             CRC value.
 *  \param      timestamp       Start of packet timestamp in microseconds.
 *  \param      rxPhyOptions    Rx PHY options.
 *
 *  Setup for next action in the operation or complete the operation.
 */
/*************************************************************************************************/
static void bbMstBisRxCompCback(uint8_t status, int8_t rssi, uint32_t crc, uint32_t timestamp, uint8_t rxPhyOptions)
{
  BB_ISR_START();

  bbBleCb.evtState += 1;

  WSF_ASSERT(BbGetCurrentBod());

  BbOpDesc_t * const pCur = BbGetCurrentBod();
  BbBleMstBisEvent_t * const pBis = &pCur->prot.pBle->op.mstBis;

  /* Store initial Rx timestamp, regardless of receive status. */
  pBis->startTsUsec = timestamp;

  WSF_ASSERT(bbBleCb.pRxDataBuf);
  uint8_t *pBuf = bbBleCb.pRxDataBuf;
  bbBleCb.pRxDataBuf = NULL;

  pBis->rxDataCback(pCur, pBuf, status);

  if (BbGetBodTerminateFlag())
  {
    PalBbBleCancelTifs();
    BbTerminateBod();
  }

  /* Update statistics. */
  switch (status)
  {
    case BB_STATUS_SUCCESS:
      BB_INC_STAT(bbBisStats.rxData);
      break;
    case BB_STATUS_RX_TIMEOUT:
      BB_INC_STAT(bbBisStats.rxDataTimeout);
      break;
    case BB_STATUS_CRC_FAILED:
      BB_INC_STAT(bbBisStats.rxDataCrc);
      break;
    case BB_STATUS_FAILED:
    default:
      BB_INC_STAT(bbBisStats.errData);
      break;
  }

  BB_ISR_MARK(bbBisStats.rxIsrUsec);
}

/*************************************************************************************************/
/*!
 *  \brief      Cancel BIS master BOD.
 *
 *  \param      pBod    Pointer to the BOD to cancel.
 *  \param      pBle    BLE operation parameters.
 */
/*************************************************************************************************/
static void bbMstCancelBisOp(BbOpDesc_t *pBod, BbBleData_t *pBle)
{
  PalBbBleCancelData();
}

/*************************************************************************************************/
/*!
 *  \brief      Execute BIS master BOD.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *  \param      pBle    BLE operation parameters.
 */
/*************************************************************************************************/
static void bbMstExecuteBisOp(BbOpDesc_t *pBod, BbBleData_t *pBle)
{
  BbBleMstBisEvent_t * const pBis = &pBod->prot.pBle->op.mstBis;

  PalBbBleSetChannelParam(&pBle->chan);

  /* bbBleCb.bbParam.txCback = NULL; */         /* Unused */
  bbBleCb.bbParam.rxCback = bbMstBisRxCompCback;
  bbBleCb.bbParam.dueUsec = BbAdjustTime(pBod->dueUsec);
  bbBleCb.bbParam.rxTimeoutUsec = pBis->rxSyncDelayUsec;
  PalBbBleSetDataParams(&bbBleCb.bbParam);

  bbBleCb.evtState = 0;

  pBis->execCback(pBod);

  if (BbGetBodTerminateFlag() &&    /* Client signaled cancel. */
      BbGetCurrentBod())            /* Termination still pending. */
  {
    BbTerminateBod();
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize for BIS master operations.
 *
 *  Update the operation table with BIS master operations routines.
 */
/*************************************************************************************************/
void BbBleBisMasterInit(void)
{
  bbBleRegisterOp(BB_BLE_OP_MST_BIS_EVENT, bbMstExecuteBisOp, bbMstCancelBisOp);

  memset(&bbBisStats, 0, sizeof(bbBisStats));
}

/*************************************************************************************************/
/*!
 *  \brief      Set receive BIS Data PDU buffer for next receive slot.
 *
 *  \param      pBuf        Receive data buffer.
 *  \param      len         Maximum length of data buffer.
 *  \param      nextPduTime Next PDU time.
 *  \param      pNextChan   Next PDU channel.
 *  \param      reAcq       Rx train re-acquisition required.
 */
/*************************************************************************************************/
void BbBleBisRxData(uint8_t *pBuf, uint16_t len, uint32_t nextPduTime, PalBbBleChan_t *pNextChan, bool_t reAcq)
{
  WSF_ASSERT(!bbBleCb.pRxDataBuf);

  bbBleCb.pRxDataBuf = pBuf;
  bbBleCb.rxDataLen = len;

  bbBleSetAbsIfs(nextPduTime, pNextChan);

  if ((bbBleCb.evtState == 0) || reAcq)
  {
    PalBbBleRxData(pBuf, len);
  }
  else
  {
    BB_ISR_MARK(bbBisStats.rxSetupUsec);
    PalBbBleRxTifsData(pBuf, len);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Receive data re-acquisition.
 *
 *  \param      syncTime    Due time for the next Rx operation.
 *
 *  Update due time for next Rx operation. Called after a missed Rx operation for re-acquisition
 *  of the receive train.
 */
/*************************************************************************************************/
void BbBleBisRxDataReAcq(uint32_t syncTime, PalBbBleChan_t *pChan)
{
  PalBbBleCancelTifs();
  bbBleCb.bbParam.dueUsec = syncTime;
  PalBbBleSetChannelParam(pChan);
  PalBbBleSetDataParams(&bbBleCb.bbParam);
}
