/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller PHY features (master) implementation file.
 *
 *  Copyright (c) 2016-2017 ARM Ltd. All Rights Reserved.
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

#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"
#include "lctr_int_master_phy.h"
#include "lctr_api_phy.h"

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer controller resources for PHY features (master).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LctrMstPhyConnInit(void)
{
  /* Add LLCP SM handler. */
  lctrMstLlcpSmTbl[LCTR_LLCP_SM_PHY_UPD] = lctrMstLlcpExecutePhyUpdateSm;
}
