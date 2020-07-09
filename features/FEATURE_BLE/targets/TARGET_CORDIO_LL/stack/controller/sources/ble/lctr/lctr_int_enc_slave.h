/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller slave connection interface file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
