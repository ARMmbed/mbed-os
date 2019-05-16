/* Copyright (c) 2009-2019 Arm Limited
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
 *  \brief Link layer scheduler interface file.
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
