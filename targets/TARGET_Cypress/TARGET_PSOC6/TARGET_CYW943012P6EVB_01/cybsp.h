/***************************************************************************//**
* \file CYW943012P6EVB-01/cybsp.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CYW943012P6EVB-01 kit.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#pragma once

#include "cybsp_types.h"
#include "cybsp_core.h"
#if defined(CYBSP_WIFI_CAPABLE)
#include "cyhal_sdio.h"
#endif
#ifndef __MBED__
#include "cybsp_retarget.h"
#include "cybsp_rgb_led.h"
#endif /* __MBED__ */

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_bsp_functions Functions
* \{
*/

#if defined(CYBSP_WIFI_CAPABLE)
/**
 * \brief Get the initialized sdio object used for communicating with the WiFi Chip. 
 * \note This function should only be called after cybsp_init();
 * \returns The initialized sdio object.
 */
cyhal_sdio_t* cybsp_get_wifi_sdio_obj(void);
#endif /* defined(CYBSP_WIFI_CAPABLE) */

/** \} group_bsp_functions */

#ifdef __cplusplus
}
#endif /* __cplusplus */
