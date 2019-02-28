/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link manager master role implementation file.
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

#include "lmgr_api_adv_master.h"
#include "bb_ble_api.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Master role device parameters. */
lmgrMstScanCtrlBlk_t lmgrMstScanCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer manager master mode resources.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrMstInit(void)
{
  static const lmgrScanParam_t defScanParam =
  {
    .scanType       = LL_SCAN_PASSIVE,
    .scanInterval   = 0x0010,
    .scanWindow     = 0x0010,
    .ownAddrType    = LL_ADDR_PUBLIC,
    .scanFiltPolicy = LL_SCAN_FILTER_NONE
  };

  memset(&lmgrMstScanCb, 0, sizeof(lmgrMstScanCb));
  lmgrMstScanCb.scanParam = defScanParam;
  lmgrMstScanCb.chanClass = LL_CHAN_DATA_ALL;
  lmgrMstScanCb.scanChanMap = LL_ADV_CHAN_ALL;
}
