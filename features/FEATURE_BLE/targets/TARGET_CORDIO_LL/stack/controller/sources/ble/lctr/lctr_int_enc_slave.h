/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller slave connection interface file.
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

#ifndef LCTR_INT_ENC_SLAVE_H
#define LCTR_INT_ENC_SLAVE_H

#include "lctr_int.h"
#include "lctr_int_conn.h"
#include "lctr_api_conn.h"
#include "lctr_pdu_conn.h"
#include "ll_defs.h"
#include "bb_ble_api.h"
#include "wsf_msg.h"
#include "wsf_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Master SKD offset. */
#define LCTR_SKD_M_OFFS         0

/*! \brief      Master IV offset. */
#define LCTR_IV_M_OFFS          0

/*! \brief      Slave SKD offset. */
#define LCTR_SKD_S_OFFS         (LL_SKD_LEN / 2)

/*! \brief      Slave IV offset. */
#define LCTR_IV_S_OFFS          (LL_IV_LEN / 2)

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine */
bool_t lctrSlvExecuteEncryptSm(lctrConnCtx_t *pCtx, uint8_t event);
bool_t lctrExecutePingSm(lctrConnCtx_t *pCtx, uint8_t event);

/* Action routines. */
void lctrEnableTxDataEnc(lctrConnCtx_t *pCtx);
void lctrDisableTxDataEnc(lctrConnCtx_t *pCtx);
void lctrEnableRxDataEnc(lctrConnCtx_t *pCtx);
void lctrDisableRxDataEnc(lctrConnCtx_t *pCtx);
void lctrGenerateSlvVectors(lctrConnCtx_t *pCtx);
void lctrStoreLtkReply(lctrConnCtx_t *pCtx);
void lctrStoreLtkNegRepTerminateReason(lctrConnCtx_t *pCtx);
void lctrCalcSessionKey(lctrConnCtx_t *pCtx);
void lctrInvalidEncPduSeq(lctrConnCtx_t *pCtx);
void lctrSendEncRsp(lctrConnCtx_t *pCtx);
void lctrSendStartEncReq(lctrConnCtx_t *pCtx);
void lctrSendStartEncRsp(lctrConnCtx_t *pCtx);
void lctrSendPauseEncReq(lctrConnCtx_t *pCtx);
void lctrSendPauseEncRsp(lctrConnCtx_t *pCtx);
void lctrSendPingReq(lctrConnCtx_t *pCtx);
void lctrSendPingRsp(lctrConnCtx_t *pCtx);
void lctrEncNotifyHostLtkReqInd(lctrConnCtx_t *pCtx);
void lctrNotifyEncChangeInd(lctrConnCtx_t *pCtx, uint8_t status);
void lctrNotifyEncKeyRefreshInd(lctrConnCtx_t *pCtx);
void lctrNotifyAuthPayloadTimeout(lctrConnCtx_t *pCtx);
void lctrRestartAuthPayloadTimer(lctrConnCtx_t *pCtx);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ENC_SLAVE_H */
