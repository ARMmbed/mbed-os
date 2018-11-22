/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller master connection interface file.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef LCTR_INT_CONN_MASTER_H
#define LCTR_INT_CONN_MASTER_H

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
  Constants
**************************************************************************************************/

/*! \brief      Buffer offset of txWinOffset in a CONN_IND PDU. */
#define LCTR_CONN_IND_TX_WIN_OFFSET     (LL_DATA_HDR_LEN + 6 + 6 + 8)

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern LctrLlcpHdlr_t lctrMstLlcpSmTbl[LCTR_LLCP_SM_TOTAL];

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine */
void lctrMstConnExecuteSm(lctrConnCtx_t *pConnCtx, uint8_t event);
void lctrMstLlcpExecuteSm(lctrConnCtx_t *pCtx, uint8_t event);
bool_t lctrMstLlcpExecuteConnUpdSm(lctrConnCtx_t *pCtx, uint8_t event);

/* Builder */
void lctrMstConnBuildOp(lctrConnCtx_t *pCtx, lctrConnInd_t *pConnInd);

/* ISR */
void lctrMstConnBeginOp(BbOpDesc_t *pOp);
void lctrMstConnCleanupOp(BbOpDesc_t *pOp);
void lctrMstConnEndOp(BbOpDesc_t *pOp);
void lctrMstConnAbortOp(BbOpDesc_t *pOp);
void lctrMstConnTxCompletion(BbOpDesc_t *pOp, uint8_t status);
void lctrMstConnRxCompletion(BbOpDesc_t *pOp, uint8_t *pRxBuf, uint8_t status);

/* Action routines. */
void lctrMstSetEstablishConn(lctrConnCtx_t *pCtx);
void lctrMstReloadDataPdu(lctrConnCtx_t *pCtx);

/* Helper */
uint32_t lctrMstConnAdjustOpStart(lctrConnCtx_t *pCtx, uint32_t scanRefTime, lctrConnInd_t *pConnInd);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_CONN_MASTER_H */
