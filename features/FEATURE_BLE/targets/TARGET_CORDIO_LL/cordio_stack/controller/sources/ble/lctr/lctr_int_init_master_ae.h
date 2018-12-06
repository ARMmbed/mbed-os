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

#ifndef LCTR_INT_INIT_MASTER_AE_H
#define LCTR_INT_INIT_MASTER_AE_H

#include "lctr_api_init_master_ae.h"
#include "lctr_int.h"
#include "lctr_int_adv_master_ae.h"
#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/
/*! \brief      Resolve the extended initiate handle from the context pointer. */
#define LCTR_GET_EXT_INIT_HANDLE(pCtx)  (pCtx - lctrMstExtInitTbl)

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master initiate states. */
enum
{
  LCTR_EXT_INIT_STATE_DISABLED,             /*!< Initiate disabled state. */
  LCTR_EXT_INIT_STATE_ENABLED,              /*!< Initiate enabled state. */
  LCTR_EXT_INIT_STATE_SHUTDOWN,             /*!< Initiate shutdown in progress. */
  LCTR_EXT_INIT_STATE_RESET,                /*!< Initiate reset in progress. */
  LCTR_EXT_INIT_STATE_TOTAL                 /*!< Total number of initiate states. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! \brief      Extended initiating control block. */
typedef struct
{
  /* Initiate */
  uint64_t          peerAddr;           /*!< Initiating peer address. */
  uint8_t           peerAddrType;       /*!< Initiating peer address type. */
  uint8_t           estConnPhys;        /*!< PHYs which established connections. */

  /* State. */
  uint8_t           enaPhys;            /*!< Enabled PHYs. */
  bool_t            initTermByHost;     /*!< Host initiated initiate disable. */

} lctrExtInitCtrlBlk_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern lctrExtScanCtx_t lctrMstExtInitTbl[LCTR_SCAN_PHY_TOTAL];
extern lctrExtInitCtrlBlk_t lctrMstExtInit;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine */
void lctrMstExtInitExecuteSm(lctrExtScanCtx_t *pExtInitCtx, uint8_t event);

/* Builder */
uint8_t lctrMstExtInitiateBuildOp(lctrExtScanCtx_t *pExtInitCtx, LlConnSpec_t *pConnSpec, uint64_t peerAddr, uint8_t peerAddrType);
uint8_t lctrMstAuxInitiateBuildOp(lctrExtScanCtx_t *pExtInitCtx, LlConnSpec_t *pConnSpec, uint64_t peerAddr, uint8_t peerAddrType);
void lctrMstExtInitiateOpCommit(lctrExtScanCtx_t *pExtInitCtx);

/* ISR: Initiate packet handlers */
void lctrMstExtPreInitiateExecHandler(BbOpDesc_t *pOp);
bool_t lctrMstInitiateRxExtAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
void lctrMstInitiateRxExtAdvPktPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
bool_t lctrMstInitiateRxAuxAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
bool_t lctrMstInitiateRxAuxConnRspHandler(BbOpDesc_t *pOp, const uint8_t *pRspBuf);
bool_t lctrMstExtConnIndTxCompHandler(BbOpDesc_t *pOp, const uint8_t *pIndBuf);

/* ISR: Initiate BOD callback */
void lctrMstExtInitiateEndOp(BbOpDesc_t *pOp);
void lctrMstAuxInitiateEndOp(BbOpDesc_t *pOp);
void lctrMstExtInitiateScanEndOp(BbOpDesc_t *pOp);
void lctrMstAuxInitiateScanEndOp(BbOpDesc_t *pOp);

/* Action routines. */
void lctrExtInitActConnect(lctrExtScanCtx_t *pExtInitCtx);
void lctrExtInitActShutdown(lctrExtScanCtx_t *pExtInitCtx);
void lctrExtInitActScanTerm(lctrExtScanCtx_t *pExtInitCtx);
void lctrExtInitActDisallowInitiate(lctrExtScanCtx_t *pExtInitCtx);
void lctrExtInitActDisallowCancel(lctrExtScanCtx_t *pExtInitCtx);

/* Helper routines. */
void lctrMstExtInitCleanupOp(lctrExtScanCtx_t *pExtInitCtx);
void lctrScanNotifyHostInitiateError(uint8_t reason, uint8_t peerAddrType, uint64_t peerAddr);
uint8_t lctrExtInitSetupInitiate(lctrExtScanCtx_t *pExtInitCtx, uint8_t peerAddrType, uint64_t peerAddr, uint8_t filtPolicy, uint8_t ownAddrType);

/* Messaging */
void lctrSendExtInitMsg(lctrExtScanCtx_t *pExtScanCtx, uint8_t event);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_INIT_MASTER_AE_H */
