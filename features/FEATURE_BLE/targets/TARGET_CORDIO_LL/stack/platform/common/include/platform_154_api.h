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
 *  \brief Platform interface file: mac154-specific.
 */
/*************************************************************************************************/

#ifndef PLATFORM_154_API_H
#define PLATFORM_154_API_H

#include "wsf_types.h"

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Load extended 802.15.4 MAC device address.
 *
 *  \param      pDevAddr    Return buffer for address.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PlatformLoadExtMac154Address(uint8_t *pDevAddr);

#endif /* PLATFORM_154_API_H */
