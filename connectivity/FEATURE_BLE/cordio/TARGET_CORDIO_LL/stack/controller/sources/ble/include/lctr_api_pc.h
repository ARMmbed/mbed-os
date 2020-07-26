/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller power control interface file.
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

#ifndef LCTR_API_PC_H
#define LCTR_API_PC_H

#include "lctr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization functions. */
void LctrPowerControlInit(void);

/* HCI functions. */
uint8_t lctrSetPathLossReportingParams(uint16_t handle, uint8_t highThresh, uint8_t highHyst, uint8_t lowThresh, uint8_t lowHyst, uint16_t minTime);
uint8_t lctrSetPathLossReportingEnable(uint16_t handle, uint8_t enable);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_PC_H */
