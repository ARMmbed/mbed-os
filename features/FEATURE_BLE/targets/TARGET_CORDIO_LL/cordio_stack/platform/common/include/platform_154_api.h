/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Platform interface file: mac154-specific.
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
