/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Multi-protocol scheduler interface file.
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

/* List maintenance */
void SchInsertNextAvailable(BbOpDesc_t *pBod);
bool_t SchInsertAtDueTime(BbOpDesc_t *pBod, BbConflictAct_t conflictCback);
bool_t SchInsertEarlyAsPossible(BbOpDesc_t *pBod, uint32_t min, uint32_t max);
bool_t SchInsertLateAsPossible(BbOpDesc_t *pBod, uint32_t min, uint32_t max);
bool_t SchRemove(BbOpDesc_t *pBod);
void SchReload(BbOpDesc_t *pBod);
void SchRemoveBackground(void);
void SchInsertBackground(BbOpDesc_t *pBod);
void SchTerminateBackground(void);

#ifdef __cplusplus
};
#endif

#endif /* SCH_API_H */
