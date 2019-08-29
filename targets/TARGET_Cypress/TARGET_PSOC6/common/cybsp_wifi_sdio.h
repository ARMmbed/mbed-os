/***************************************************************************//**
* \file cybsp_wifi_sdio.h
*
* \brief
* Utility functions to intialize the SDIO communication bus used to 
* communicate with the WiFi radio.
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

/**
* \addtogroup group_bsp_board_wifi WiFi
* \ingroup group_bsp_abstraction
* \{
* Basic abstraction layer for dealing with boards containing a Cypress MCU. This
* API provides convenience methods for initializing and manipulating different
* hardware found on the board.
*
* \defgroup group_bsp_board_wifi_macros Macros
* \defgroup group_bsp_board_wifi_functions Functions
*/
#pragma once 

#include "cy_result.h"
#include "cyhal.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_bsp_board_wifi_macros
* \{
*/

/** SDIO enumeration failed. */
#define CYBSP_RSLT_WIFI_SDIO_ENUM_TIMEOUT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_BSP, 5))

/** \} group_bsp_board_wifi_macros */

/**
* \addtogroup group_bsp_board_wifi_functions
* \{
*/

/** Initializes and enumerates the SDIO object connected to the wifi chip. 
 * The following operations are performed,
 * -# Initializes the SDIO interface on the board connect to the wifi chip.
 * -# Resets the wifi chip.
 *
 * @return CY_RSLT_SUCCESS for successful initialization or error if initialization failed.
 */
cy_rslt_t cybsp_wifi_sdio_init(void);

/** Frees up any resources allocated by the cybsp_wifi_sdio_init() */
void cybsp_wifi_sdio_deinit(void);

/** Get the initialized sdio object. This should only be called after cybsp_wifi_sdio_init();
 * 
 * @return The initialized and enumerated sdio object.
 */
cyhal_sdio_t* cybsp_get_wifi_sdio_obj(void);

/** \} group_bsp_board_wifi_functions */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** \} group_bsp_board_wifi */