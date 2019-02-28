/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link manager connection implementation file.
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

#include "lmgr_api_conn.h"
#include "bb_ble_api.h"
#include "lctr_api.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Slave role device parameters. */
lmgrConnCtrlBlk_t lmgrConnCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer manager connection resources.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrConnInit(void)
{
  memset(&lmgrConnCb, 0, sizeof(lmgrConnCb));

  lmgrConnCb.availTxBuf = pLctrRtCfg->numTxBufs;
  lmgrConnCb.availRxBuf = pLctrRtCfg->numRxBufs;

  lmgrConnCb.maxTxLen = LL_MAX_DATA_LEN_MIN;
  lmgrConnCb.maxTxTime = LL_MAX_DATA_TIME_MIN;

  lmgrConnCb.allPhys = LL_ALL_PHY_TX_PREFERENCE_BIT | LL_ALL_PHY_RX_PREFERENCE_BIT;
  lmgrConnCb.rxPhys = LL_PHYS_NONE;
  lmgrConnCb.txPhys = LL_PHYS_NONE;
}
