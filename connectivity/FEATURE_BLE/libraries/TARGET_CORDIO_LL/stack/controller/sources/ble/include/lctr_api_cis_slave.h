/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller CIS slave interface file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_API_CIS_SLAVE_H
#define LCTR_API_CIS_SLAVE_H

#include "lctr_api.h"
#include "cfg_mac_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
uint16_t LctrInitCisMem(uint8_t *pFreeMem, uint32_t freeMemSize);

/* Control */
uint8_t LctrRejectCisReq(uint16_t cisHandle, uint8_t reason);
uint8_t LctrAcceptCisReq(uint16_t cisHandle);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_CIS_SLAVE_H */
