/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller scanning master interface file.
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

#ifndef LCTR_INT_INIT_MASTER_H
#define LCTR_INT_INIT_MASTER_H

#include "lctr_api_init_master.h"
#include "lctr_int.h"
#include "lctr_int_adv_master.h"
#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master initiate states. */
enum
{
  LCTR_INIT_STATE_DISABLED,             /*!< Initiate disabled state. */
  LCTR_INIT_STATE_ENABLED,              /*!< Initiate enabled state. */
  LCTR_INIT_STATE_SHUTDOWN,             /*!< Scan shutdown in progress. */
  LCTR_INIT_STATE_RESET,                /*!< Scan reset in progress. */
  LCTR_INIT_STATE_TOTAL                 /*!< Total number of scan states. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern lctrMstScanCtx_t lctrMstInit;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Builder */
void lctrMstInitiateBuildOp(LlConnSpec_t *pConnSpec, uint8_t peerAddrType, uint64_t peerAddr);
void lctrMstInitiateOpCommit(void);

/* ISR */
void lctrMstInitiateEndOp(BbOpDesc_t *pOp);
bool_t lctrMstInitiateAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
bool_t lctrMstConnIndTxCompHandler(BbOpDesc_t *pOp, const uint8_t *pIndBuf);

/* Action routines. */
void lctrInitActInitiate(void);
void lctrInitActConnect(void);
void lctrInitActShutdown(void);
void lctrInitActScanTerm(void);
void lctrInitActResetTerm(void);
void lctrInitActDisallowInitiate(void);
void lctrInitActDisallowCancel(void);

/* Helper routines. */
void lctrScanCleanup(lctrMstScanCtx_t *pCtx);
void lctrScanNotifyHostInitiateError(uint8_t reason, uint8_t peerAddrType, uint64_t peerAddr);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_INIT_MASTER_H */
