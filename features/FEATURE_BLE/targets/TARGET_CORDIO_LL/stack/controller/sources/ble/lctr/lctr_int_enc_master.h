/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller master connection interface file.
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

#ifndef LCTR_INT_ENC_MASTER_H
#define LCTR_INT_ENC_MASTER_H

#include "lctr_int_conn.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine */
bool_t lctrMstExecuteEncryptSm(lctrConnCtx_t *pCtx, uint8_t event);

/* Action routines. */
void lctrGenerateMstVectors(lctrConnCtx_t *pCtx);
void lctrStoreSlvVectors(lctrConnCtx_t *pCtx);
void lctrSendEncReq(lctrConnCtx_t *pCtx);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ENC_MASTER_H */
