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
 * \brief Generic BLE baseband driver implementation file.
 */
/*************************************************************************************************/

#include "bb_api.h"
#include "bb_ble_int.h"
#include "ll_math.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

BbBleDataPktStats_t bbTestStats;          /*!< Test data packet statistics. */
extern BbRtCfg_t *pBbRtCfg;

/*************************************************************************************************/
/*!
 *  \brief      Tx completion for transmit test mode.
 *
 *  \param      status      Completion status.
 *
 *  \return     None.
 *
 *  Completion handler for the BLE transmit test operation.
 */
/*************************************************************************************************/
static void bbTestTxCompCback(uint8_t status)
{
  WSF_ASSERT(BbGetCurrentBod());

  BbOpDesc_t * const pCur = BbGetCurrentBod();
  BbBleData_t * const pBle = pCur->prot.pBle;
  BbBleTestTx_t * const pTx = &pBle->op.testTx;

  bool_t bodComplete = FALSE;

  switch (status)
  {
    case BB_STATUS_SUCCESS:
      bodComplete = !pTx->testCback(pCur, status);
      break;

    case BB_STATUS_FAILED:
    default:
      (void)pTx->testCback(pCur, status);
      bodComplete = TRUE;
      break;
  }

  if (bodComplete)
  {
    BbTerminateBod();
  }
  else
  {
    const uint32_t pktInterUsec = pTx->pktInterUsec + bbBleCb.bbParam.dueOffsetUsec;
    const uint32_t pktInter     = BB_US_TO_BB_TICKS(pktInterUsec);
    int16_t dueOffsetUsec       = pktInterUsec - BB_TICKS_TO_US(pktInter);

    PalBbBleCancelTifs();

    bbBleCb.bbParam.due           = bbBleCb.bbParam.due + pktInter;
    bbBleCb.bbParam.dueOffsetUsec = WSF_MAX(dueOffsetUsec, 0);
    PalBbBleSetChannelParam(&pBle->chan);
    PalBbBleSetDataParams(&bbBleCb.bbParam);

    bbBleClrIfs();     /* never setup for TIFS */
    PalBbBleTxBufDesc_t desc = {.pBuf = pTx->pTxBuf, .len = pTx->txLen};
    PalBbBleTxData(&desc, 1);
  }

  switch (status)
  {
    case BB_STATUS_SUCCESS:
      bbTestStats.txData++;
      break;
    case BB_STATUS_FAILED:
    default:
      bbTestStats.errData++;
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Rx completion for continuous receive test mode.
 *
 *  \param      status          Completed receive buffer.
 *  \param      rssi            RSSI value.
 *  \param      crc             CRC value.
 *  \param      timestamp       Start of packet timestamp.
 *  \param      rxPhyOptions    Rx PHY options.
 *
 *  \return     None.
 *
 *  Completion handler for the BLE receive test operation.
 */
/*************************************************************************************************/
static void bbTestRxCompCback(uint8_t status, int8_t rssi, uint32_t crc, uint32_t timestamp, uint8_t rxPhyOptions)
{
  WSF_ASSERT(BbGetCurrentBod());

  BbOpDesc_t * const pCur = BbGetCurrentBod();
  BbBleData_t * const pBle = pCur->prot.pBle;
  BbBleTestRx_t * const pRx = &pBle->op.testRx;

  bool_t bodComplete = FALSE;

  switch (status)
  {
    case BB_STATUS_SUCCESS:
    case BB_STATUS_CRC_FAILED:
      bodComplete = !pRx->testCback(pCur, status);
      break;

    case BB_STATUS_RX_TIMEOUT:
    case BB_STATUS_FAILED:
    default:
      (void)pRx->testCback(pCur, status);
      bodComplete = TRUE;
      break;
  }

  if (bodComplete)
  {
    BbTerminateBod();
  }
  else
  {
    PalBbBleCancelTifs();

    const uint32_t pktInterUsec = pBbRtCfg->rfSetupDelayUs;
    const uint32_t pktInter     = BB_US_TO_BB_TICKS(pktInterUsec);
    int16_t dueOffsetUsec       = pktInterUsec - BB_TICKS_TO_US(pktInter);

    bbBleCb.bbParam.due           = PalBbGetCurrentTime(USE_RTC_BB_CLK) + pktInter;
    bbBleCb.bbParam.dueOffsetUsec = WSF_MAX(dueOffsetUsec, 0);
    bbBleCb.bbParam.rxTimeoutUsec = pRx->rxSyncDelayUsec;
    PalBbBleSetChannelParam(&pBle->chan);
    PalBbBleSetDataParams(&bbBleCb.bbParam);

    bbBleClrIfs();     /* never setup for TIFS */
    PalBbBleRxData(pRx->pRxBuf, pRx->rxLen);
  }

  switch (status)
  {
    case BB_STATUS_SUCCESS:
      bbTestStats.rxData++;
      break;
    case BB_STATUS_RX_TIMEOUT:
      bbTestStats.rxDataTimeout++;
      break;
    case BB_STATUS_CRC_FAILED:
      bbTestStats.rxDataCrc++;
      break;
    case BB_STATUS_FAILED:
    default:
      bbTestStats.errData++;
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Cleanup test BOD.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *  \param      pBle    BLE operation parameters.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbTestCleanupOp(BbOpDesc_t *pBod, BbBleData_t *pBle)
{
  PalBbBleCancelData();
  BbTerminateBod();
}

/*************************************************************************************************/
/*!
 *  \brief      Execute direct test mode transmit BOD.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *  \param      pBle    BLE operation parameters.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbSlvExecuteTestTxOp(BbOpDesc_t *pBod, BbBleData_t *pBle)
{
  BbBleTestTx_t *pTx = &pBle->op.testTx;

  bbBleCb.bbParam.txCback       = bbTestTxCompCback;
  bbBleCb.bbParam.rxCback       = bbTestRxCompCback;
  bbBleCb.bbParam.due           = pBod->due;
  bbBleCb.bbParam.dueOffsetUsec = pBod->dueOffsetUsec;

  PalBbBleSetChannelParam(&pBle->chan);
  PalBbBleSetDataParams(&bbBleCb.bbParam);

  bbBleClrIfs();     /* never setup for TIFS */
  PalBbBleTxBufDesc_t desc = {.pBuf = pTx->pTxBuf, .len = pTx->txLen};
  PalBbBleTxData(&desc, 1);
}

/*************************************************************************************************/
/*!
 *  \brief      Execute direct test mode receive BOD.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *  \param      pBle    BLE operation parameters.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbSlvExecuteTestRxOp(BbOpDesc_t *pBod, BbBleData_t *pBle)
{
  BbBleTestRx_t *pRx = &pBle->op.testRx;

  bbBleCb.bbParam.txCback       = bbTestTxCompCback;
  bbBleCb.bbParam.rxCback       = bbTestRxCompCback;
  bbBleCb.bbParam.due           = pBod->due;
  bbBleCb.bbParam.dueOffsetUsec = pBod->dueOffsetUsec;
  bbBleCb.bbParam.rxTimeoutUsec = pRx->rxSyncDelayUsec;

  PalBbBleSetChannelParam(&pBle->chan);
  PalBbBleSetDataParams(&bbBleCb.bbParam);

  bbBleClrIfs();     /* never setup for TIFS */
  PalBbBleRxData(pRx->pRxBuf, pRx->rxLen);
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize for direct test mode operations.
 *
 *  \return     None.
 *
 *  Update the operation table with direct test mode operations routines.
 */
/*************************************************************************************************/
void BbBleTestInit(void)
{
  bbBleRegisterOp(BB_BLE_OP_TEST_TX, bbSlvExecuteTestTxOp, bbTestCleanupOp);
  bbBleRegisterOp(BB_BLE_OP_TEST_RX, bbSlvExecuteTestRxOp, bbTestCleanupOp);

  memset(&bbTestStats, 0, sizeof(bbTestStats));
}

/*************************************************************************************************/
/*!
 *  \brief      Get test mode packet statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetTestStats(BbBleDataPktStats_t *pStats)
{
  *pStats = bbTestStats;
}
