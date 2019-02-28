/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer scheduler interface file.
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

#ifndef SCH_API_BLE_H
#define SCH_API_BLE_H

#include "wsf_types.h"
#include "bb_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Resource manager */
void SchRmInit(void);
uint32_t SchRmPreferredPeriodUsec(void);
bool_t SchRmAdd(uint8_t handle, uint32_t minUsec, uint32_t maxUsec, uint32_t durUsec, uint32_t *pInterUsec);
bool_t SchRmStartUpdate(uint8_t handle, uint32_t minUsec, uint32_t maxUsec, uint32_t durUsec, uint32_t *pInterUsec);
void SchRmCommitUpdate(uint8_t handle);
void SchRmRemove(uint8_t handle);
void SchRmSetReference(uint8_t handle);
uint32_t SchRmGetOffsetUsec(uint32_t rsvnOffs[], uint32_t maxOffsUsec, uint8_t handle);

/* BLE time utilities */
uint32_t SchBleCalcDataPktDurationUsec(uint8_t phy, uint16_t len);
uint32_t SchBleCalcAdvPktDurationUsec(uint8_t phy, uint8_t phyOptions, uint16_t len);
uint32_t SchBleCalcAuxPktDurationUsec(uint8_t phy, uint8_t phyOptions, uint16_t len);
void SchBleCalcAdvOpDuration(BbOpDesc_t *pBod);
bool_t SchBleGetNextMstConnDueTime(uint32_t *pDueTime);

#ifdef __cplusplus
};
#endif

#endif /* SCH_API_BLE_H */
