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
 * \brief Connectable BLE baseband porting implementation file.
 */
/*************************************************************************************************/

#include "bb_api.h"
#include "pal_bb.h"
#include "bb_ble_int.h"

#if BB_DATA_PLD_MAX_LEN < LL_MAX_DATA_LEN_MIN
#error "Unsupported BB_DATA_PLD_MAX_LEN value, must be greater than 27 bytes"
#endif

#if BB_DATA_PLD_MAX_LEN > LL_MAX_DATA_LEN_ABS_MAX
#error "Unsupported BB_DATA_PLD_MAX_LEN value, must be less or equal to 251 bytes"
#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

BbBleDataPktStats_t bbConnStats;          /*!< Connection packet statistics. */

/*************************************************************************************************/
/*!
 *  \brief      Transmit data PDU at next transmit slot.
 *
 *  \param      descs       Array of transmit buffer descriptor.
 *  \param      cnt         Number of descriptors.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the call context of
 *              \ref BbBleMstConnEvent_t::rxDataCback or \ref BbBleSlvConnEvent_t::rxDataCback
 *              callback routine.
 */
/*************************************************************************************************/
void BbBleTxData(PalBbBleTxBufDesc_t descs[], uint8_t cnt)
{
  if ((BbGetCurrentBod()->prot.pBle->chan.opType == BB_BLE_OP_MST_CONN_EVENT) &&
      (bbBleCb.evtState == 0))
  {
    bbBleSetIfs();     /* master always Rx's after Tx */
    PalBbBleTxData(descs, cnt);
  }
  else
  {
    BB_ISR_MARK(bbConnStats.txSetupUsec);

    /* TODO set only if master or if slave and Rx may follow in CE. */
    bbBleSetIfs();
    PalBbBleTxTifsData(descs, cnt);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Set receive data buffer for next receive slot.
 *
 *  \param      pBuf        Receive data buffer.
 *  \param      len         Maximum length of data buffer.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the call context of
 *              \ref BbBleMstConnEvent_t::rxDataCback or
 *              \ref BbBleSlvConnEvent_t::rxDataCback callback routine.
 *
 *  \note       BB must always call the \ref BbBleMstConnEvent_t::rxDataCback or
 *              \ref BbBleSlvConnEvent_t::rxDataCback callback routine of the
 *              currently executing BOD with the given buffer.
 */
/*************************************************************************************************/
void BbBleRxData(uint8_t *pBuf, uint16_t len)
{
  WSF_ASSERT(!bbBleCb.pRxDataBuf);

  bbBleCb.pRxDataBuf = pBuf;
  bbBleCb.rxDataLen = len;

  if ((BbGetCurrentBod()->prot.pBle->chan.opType == BB_BLE_OP_SLV_CONN_EVENT) &&
      (bbBleCb.evtState == 0))
  {
    bbBleSetIfs();       /* slave always Tx's after Rx */
    PalBbBleRxData(pBuf, len);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Get connection packet statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetConnStats(BbBleDataPktStats_t *pStats)
{
  *pStats = bbConnStats;
}
