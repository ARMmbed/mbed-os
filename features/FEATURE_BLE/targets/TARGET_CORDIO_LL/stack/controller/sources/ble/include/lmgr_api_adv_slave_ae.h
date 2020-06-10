/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager advertising extension interface file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
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

#ifndef LMGR_API_ADV_SLAVE_AE_H
#define LMGR_API_ADV_SLAVE_AE_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Events */
void LmgrSendExtAdvEnableCnf(uint8_t handle, uint8_t status);
void LmgrSendScanReqReceivedInd(uint8_t handle, uint8_t scanAddrType, uint64_t scanAddr);
void LmgrSendAdvSetTermInd(uint8_t handle, uint8_t status, uint16_t connHandle, uint8_t numEvents);
void LmgrSendPeriodicAdvEnableCnf(uint8_t handle, uint8_t status);

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_ADV_SLAVE_AE_H */
