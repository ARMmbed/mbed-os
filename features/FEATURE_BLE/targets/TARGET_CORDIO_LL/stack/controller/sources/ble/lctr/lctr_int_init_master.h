/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller scanning master interface file.
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
