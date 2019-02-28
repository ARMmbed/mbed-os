/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Connectable BLE baseband porting implementation file.
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

#include "bb_api.h"
#include "bb_drv.h"
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
void BbBleTxData(BbBleDrvTxBufDesc_t descs[], uint8_t cnt)
{
  if ((BbGetCurrentBod()->prot.pBle->chan.opType == BB_BLE_OP_MST_CONN_EVENT) &&
      (bbBleCb.evtState == 0))
  {
    bbBleSetIfs();     /* master always Rx's after Tx */
    BbBleDrvTxData(descs, cnt);
  }
  else
  {
    BB_ISR_MARK(bbConnStats.txSetupUsec);

    /* TODO set only if master or if slave and Rx may follow in CE. */
    bbBleSetIfs();
    BbBleDrvTxTifsData(descs, cnt);
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
    BbBleDrvRxData(pBuf, len);
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
