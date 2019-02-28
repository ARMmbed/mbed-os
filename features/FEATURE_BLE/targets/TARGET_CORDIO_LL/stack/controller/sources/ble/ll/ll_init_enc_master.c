/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) encryptable master initialization implementation file.
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

#include "lctr_api_adv_master.h"
#include "lctr_api_conn.h"
#include "lmgr_api_adv_master.h"
#include "lmgr_api_conn.h"
#include "wsf_assert.h"

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation as a encryptable connectable master.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for use as an advertising and encryptable
 *  connectable master.
 */
/*************************************************************************************************/
void LlEncConnMasterInit(void)
{
  WSF_ASSERT(pLctrRtCfg);     /* Runtime configuration must be available. */

  LmgrConnInit();
  LctrMstConnEncInit();
}
