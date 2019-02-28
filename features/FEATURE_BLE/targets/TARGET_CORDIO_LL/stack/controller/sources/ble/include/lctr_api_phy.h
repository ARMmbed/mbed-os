/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller PHY features interface file.
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

#ifndef LCTR_API_PHY_H
#define LCTR_API_PHY_H

#include "lctr_api.h"
#include "lmgr_api_priv.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrSlvPhyConnInit(void);
void LctrMstPhyConnInit(void);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_PHY_H */
