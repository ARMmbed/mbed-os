/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller isochronous slave interface file.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
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

#ifndef LCTR_INT_BIS_SLAVE_H
#define LCTR_INT_BIS_SLAVE_H

#include "lctr_api_bis_slave.h"
#include "lctr_int_bis.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Get reservation manager handle from BIG context. */
#define LCTR_BIG_TO_RM_HANDLE(pBigCtx)   (LL_MAX_CONN + LL_MAX_ADV_SETS + LL_MAX_CIG + (pBigCtx - &pLctrBigTbl[0]))

/*! \brief      Get BIG context from reservation manager handle. */
#define LCTR_RM_HANDLE_TO_BIG(rmHandle)  (&pLctrBigTbl[(rmHandle) - LL_MAX_CONN - LL_MAX_ADV_SETS - LL_MAX_CIG])

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Slave BIS broadcasting states. */
enum
{
  LCTR_SLV_BIG_STATE_DISABLED,          /*!< BIS slave broadcasting disabled state. */
  LCTR_SLV_BIG_STATE_ENABLED,           /*!< BIS slave broadcasting enabled state. */
  LCTR_SLV_BIG_STATE_SHUTDOWN,          /*!< BIS slave broadcasting shutdown in progress. */
  LCTR_SLV_BIG_STATE_RESET,             /*!< BIS slave broadcasting reset in progress. */
  LCTR_SLV_BIG_STATE_TOTAL              /*!< Total number of extended advertising states. */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Host events */
void lctrNotifyHostCreateBigComplete(lctrBigCtx_t *pBigCtx, uint8_t status);
void lctrNotifyHostTerminateBigComplete(lctrBigCtx_t *pBigCtx);

/* State machine for BIS slave */
void lctrSlvBigSendMsg(lctrBigCtx_t *pBigCtx, uint8_t event);
void lctrSlvBigSendAcadMsg(lctrBigCtx_t *pBigCtx, uint8_t event);
void lctrSlvBigExecuteSm(lctrBigCtx_t *pBigCtx, uint8_t event);

/* Action routines */
void lctrSlvBigActStart(lctrBigCtx_t *pBigCtx);
void lctrSlvBigActSendChMapUpd(lctrBigCtx_t *pBigCtx);
void lctrSlvBigActSendTerm(lctrBigCtx_t *pBigCtx);
void lctrSlvBigActShutdown(lctrBigCtx_t *pBigCtx);
void lctrSlvBigActCleanup(lctrBigCtx_t *pBigCtx);

/* Builder */
uint8_t lctrSlvBigBuildOp(lctrBigCtx_t *pBigCtx);

/* ISR: Packet handlers */
void lctrSlvBisTxCompletionSequential(BbOpDesc_t *pOp, uint8_t status);
void lctrSlvBisTxCompletionInterleaved(BbOpDesc_t *pOp, uint8_t status);

/* ISR: BOD handlers */
void lctrSlvBigBeginOp(BbOpDesc_t *pOp);
void lctrSlvBigEndOp(BbOpDesc_t *pOp);
void lctrSlvBigAbortOp(BbOpDesc_t *pOp);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_BIS_SLAVE_H */
