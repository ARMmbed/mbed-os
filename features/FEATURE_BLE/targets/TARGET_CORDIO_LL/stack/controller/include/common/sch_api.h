/* Copyright (c) 2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 * \file
 * \brief Multi-protocol scheduler interface file.
 */
/*************************************************************************************************/

#ifndef SCH_API_H
#define SCH_API_H

#include "wsf_types.h"
#include "wsf_os.h"
#include "bb_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Conflict action call signature. */
typedef BbOpDesc_t*(*BbConflictAct_t)(BbOpDesc_t *pNewBod, BbOpDesc_t *pExistBod);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void SchInit(void);
void SchHandlerInit(wsfHandlerId_t handlerId);
void SchReset(void);
uint16_t SchStatsGetHandlerWatermarkUsec(void);

/* Control */
void SchHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);
void SchLoadHandler(void);

/* List maintenance */
void SchInsertNextAvailable(BbOpDesc_t *pBod);
bool_t SchInsertAtDueTime(BbOpDesc_t *pBod, BbConflictAct_t conflictCback);
bool_t SchInsertEarlyAsPossible(BbOpDesc_t *pBod, uint32_t min, uint32_t max);
bool_t SchInsertLateAsPossible(BbOpDesc_t *pBod, uint32_t min, uint32_t max);
bool_t SchRemove(BbOpDesc_t *pBod);
void SchReload(BbOpDesc_t *pBod);
bool_t SchIsBodCancellable(BbOpDesc_t *pBod);

#ifdef __cplusplus
};
#endif

#endif /* SCH_API_H */
