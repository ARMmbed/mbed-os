/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer scheduler interface file.
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
