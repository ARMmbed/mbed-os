/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link manager slave role implementation file.
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

#include "lmgr_api_adv_slave.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Slave role device parameters. */
lmgrSlvAdvCtrlBlk_t lmgrSlvAdvCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer manager slave mode resources.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSlvInit(void)
{
  static const lmgrAdvParam_t defAdvParam =
  {
    .advInterMin    = 0x0800,
    .advInterMax    = 0x0800,
    .advType        = LL_ADV_CONN_UNDIRECT,
    .ownAddrType    = LL_ADDR_PUBLIC,
    .peerAddrType   = LL_ADDR_PUBLIC,
    .advChanMap     = LL_ADV_CHAN_ALL,
    .advFiltPolicy  = LL_ADV_FILTER_NONE
  };

  memset(&lmgrSlvAdvCb, 0, sizeof(lmgrSlvAdvCb));

  /* Set default Tx PHY options */
  lmgrSlvAdvCb.defTxPhyOpts = BB_PHY_OPTIONS_BLE_S8;

  lmgrSlvAdvCb.advParam = defAdvParam;
  /* The maximum adv interval is 0x4000 and it times 625 still fits in uint32_t. */
  /* coverity[overflow_before_widen] */
  lmgrSlvAdvCb.advParam.advInterMin = BB_BLE_TO_BB_TICKS(lmgrSlvAdvCb.advParam.advInterMin);
  /* coverity[overflow_before_widen] */
  lmgrSlvAdvCb.advParam.advInterMax = BB_BLE_TO_BB_TICKS(lmgrSlvAdvCb.advParam.advInterMax);
}
