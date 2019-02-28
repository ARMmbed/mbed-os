/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link manager privacy implementation file.
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

#include "lmgr_api_priv.h"
#include "bb_ble_api.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Privacy parameters. */
lmgrPrivCtrlBlk_t lmgrPrivCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer manager privacy resources.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrPrivInit(void)
{
  lmgrPrivCb.resPrivAddrTimeout = LL_DEF_RES_ADDR_TO_SEC; /* seconds */
  lmgrPrivCb.numPendingAddrRes  = 0;
}
