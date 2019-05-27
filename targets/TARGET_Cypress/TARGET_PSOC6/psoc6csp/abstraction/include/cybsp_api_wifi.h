/***************************************************************************//**
* \file cybsp_api_wifi.h
*
* \brief
* Basic abstraction layer for dealing with boards containing a Cypress MCU. This
* API provides convenience methods for initializing and manipulating different
* hardware found on the board.
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
* \addtogroup group_abstraction_board_wifi Board Wifi abstraction
* \ingroup group_abstraction
* \{
* Basic abstraction layer for dealing with boards containing a Cypress MCU. This
* API provides convenience methods for initializing and manipulating different
* hardware found on the board.
*
* \defgroup group_abstraction_board_wifi_macros Macros
* \defgroup group_abstraction_board_wifi_functions Functions
*/
#pragma once

#include "cybsp_api_core.h"
#include "whd_wifi_api.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_abstraction_board_wifi_macros
* \{
*/

/** Indicates that the wifi driver is available and can be used. */
#define CYBSP_WIFI_CAPABLE 1

/** Initialization of the wifi driver failed. */
#define CYBSP_RSLT_WIFI_INIT_FAILED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_BSP, 4))
/** SDIO enumeration failed. */
#define CYBSP_RSLT_WIFI_SDIO_ENUM_TIMEOUT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_BSP, 5))

/** \} group_abstraction_board_wifi_macros */

/**
* \addtogroup group_abstraction_board_wifi_functions
* \{
*/

/** Initializes the SDIO interface on the board. This only needs to be called if the
 * SDIO interface needs to be initialized before the general wifi interface. If not
 * called directly, it will automatically be called by cybsp_wifi_init().
 *
 * @return CY_RSLT_SUCCESS for successful initialization or error if initialization failed.
 */
cy_rslt_t cybsp_sdio_init(void);

/** Initializes the wifi chip on the board.
 *
 * @return CY_RSLT_SUCCESS for successful initialization or error if initialization failed.
 */
cy_rslt_t cybsp_wifi_init(void);

/** Gets the wifi driver instance initialized by the driver. This should only be called
 * after the interface is initialized by cybsp_wifi_init().
 *
 * @return Wifi driver instance pointer.
 */
whd_driver_t* cybsp_get_wifi_driver(void);

/** \} group_abstraction_board_wifi_functions */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** \} group_abstraction_board_wifi */