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
 * \brief Connectable master BLE baseband porting implementation file.
 */
/*************************************************************************************************/

#include "bb_api.h"
#include "pal_bb.h"
#include "bb_ble_int.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern BbBleDataPktStats_t  bbConnStats;    /*!< Connection packet statistics. */

/*************************************************************************************************/
/*!
 *  \brief      Tx completion for connected master operation.
 *
 *  \param      status      Transmission status
 *
 *  \return     None.
 *
 *  Setup for next action in the operation or complete the operation.
 */
/*************************************************************************************************/
static void bbMstConnTxCompCback(uint8_t status)
{
  BB_ISR_START();

  WSF_ASSERT(BbGetCurrentBod());

  BbOpDesc_t * const pCur = BbGetCurrentBod();
  BbBleMstConnEvent_t * const pConn = &pCur->prot.pBle->op.mstConn;

  WSF_ASSERT(pCur);

  pConn->txDataCback(pCur, status);

  if (bbBleCb.pRxDataBuf &&
      (status == BB_STATUS_SUCCESS))
  {
    BB_ISR_MARK(bbConnStats.rxSetupUsec);

    bbBleSetIfs();     /* TODO set only if Tx may follow in CE */
    PalBbBleRxTifsData(bbBleCb.pRxDataBuf, bbBleCb.rxDataLen);
  }
  else
  {
    /* Cancel TIFS timer if active. */
    switch (status)
    {
      case BB_STATUS_SUCCESS:
        PalBbBleCancelTifs();
        break;
      case BB_STATUS_FAILED:
      default:
        /* Free Rx data buffer before BOD end. */
        if (bbBleCb.pRxDataBuf != NULL)        /* buffer should always exist, but still check */
        {
          uint8_t *pBuf = bbBleCb.pRxDataBuf;
          bbBleCb.pRxDataBuf = NULL;
          pConn->rxDataCback(pCur, pBuf, BB_STATUS_CANCELED);
        }
        break;
    }

    BbTerminateBod();
  }

  /* Update statistics. */
  switch (status)
  {
    case BB_STATUS_SUCCESS:
      BB_INC_STAT(bbConnStats.txData);
      break;
    case BB_STATUS_FAILED:
    default:
      BB_INC_STAT(bbConnStats.errData);
      break;
  }

  BB_ISR_MARK(bbConnStats.txIsrUsec);
}

/*************************************************************************************************/
/*!
 *  \brief      Rx completion for connected master operation.
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
static void bbMstConnRxCompCback(uint8_t status, int8_t rssi, uint32_t crc, uint32_t timestamp, uint8_t rxPhyOptions)
{
  BB_ISR_START();

  WSF_ASSERT(BbGetCurrentBod());

  BbOpDesc_t * const pCur = BbGetCurrentBod();
  BbBleMstConnEvent_t * const pConn = &pCur->prot.pBle->op.mstConn;

  pConn->rssi = rssi;
  pConn->rxPhyOptions = rxPhyOptions;

  if (bbBleCb.evtState == 0)
  {
    bbBleCb.evtState = 1;
  }

  WSF_ASSERT(bbBleCb.pRxDataBuf);
  uint8_t *pBuf = bbBleCb.pRxDataBuf;
  bbBleCb.pRxDataBuf = NULL;

  /* Set Tx buffer or BOD cancel expected to be called during this routine. */
  pConn->rxDataCback(pCur, pBuf, status);

  if (BbGetBodTerminateFlag())
  {
    WSF_ASSERT(!bbBleCb.pRxDataBuf);

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

    BbTerminateBod();               /* auto terminated on missed; just end */
  }

  /* Update statistics. */
  switch (status)
  {
    case BB_STATUS_SUCCESS:
      BB_INC_STAT(bbConnStats.rxData);
      break;
    case BB_STATUS_RX_TIMEOUT:
      BB_INC_STAT(bbConnStats.rxDataTimeout);
      break;
    case BB_STATUS_CRC_FAILED:
      BB_INC_STAT(bbConnStats.rxDataCrc);
      break;
    case BB_STATUS_FAILED:
    default:
      BB_INC_STAT(bbConnStats.errData);
      break;
  }

  BB_ISR_MARK(bbConnStats.rxIsrUsec);
}

/*************************************************************************************************/
/*!
 *  \brief      Execute connectable master BOD.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *  \param      pBle    BLE operation parameters.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbMstExecuteConnOp(BbOpDesc_t *pBod, BbBleData_t *pBle)
{
  PalBbBleSetChannelParam(&pBle->chan);

  WSF_ASSERT(pBle->op.mstConn.txDataCback);
  WSF_ASSERT(pBle->op.mstConn.rxDataCback);

  bbBleCb.bbParam.txCback = bbMstConnTxCompCback;
  bbBleCb.bbParam.rxCback = bbMstConnRxCompCback;
  bbBleCb.bbParam.due = pBod->due;
  bbBleCb.bbParam.dueOffsetUsec = pBod->dueOffsetUsec;
  bbBleCb.bbParam.rxTimeoutUsec = 2 * LL_MAX_TIFS_DEVIATION;

  PalBbBleSetDataParams(&bbBleCb.bbParam);

  bbBleCb.evtState = 0;

  pBle->op.mstConn.execCback(pBod);
}

/*************************************************************************************************/
/*!
 *  \brief      Cancel connectable master BOD.
 *
 *  \param      pBod    Pointer to the BOD to cancel.
 *  \param      pBle    BLE operation parameters.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbMstCancelConnOp(BbOpDesc_t *pBod, BbBleData_t *pBle)
{
  WSF_ASSERT(pBod);
  WSF_ASSERT(pBle);
  WSF_ASSERT(pBle->op.mstConn.rxDataCback);

  PalBbBleCancelData();

  if (bbBleCb.pRxDataBuf)
  {
    uint8_t *pBuf = bbBleCb.pRxDataBuf;
    bbBleCb.pRxDataBuf = NULL;

    /* Buffer free expected to be called during this routine. */
    pBle->op.mstConn.rxDataCback(pBod, pBuf, BB_STATUS_CANCELED);
  }

  pBle->op.mstConn.cancelCback(pBod);
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize for connectable master operations.
 *
 *  \return     None.
 *
 *  Update the operation table with connectable master operations routines.
 */
/*************************************************************************************************/
void BbBleConnMasterInit(void)
{
  bbBleRegisterOp(BB_BLE_OP_MST_CONN_EVENT, bbMstExecuteConnOp, bbMstCancelConnOp);

  memset(&bbConnStats, 0, sizeof(bbConnStats));
}
