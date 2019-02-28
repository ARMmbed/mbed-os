/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Multi-protocol scheduler interface file.
 *
 *  Copyright (c) 2013-2018 ARM Ltd. All Rights Reserved.
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
