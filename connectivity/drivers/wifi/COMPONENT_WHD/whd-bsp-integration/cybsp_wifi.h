/***********************************************************************************************//**
 * \file cybsp_wifi.h
 *
 * \brief
 * Basic abstraction layer for dealing with boards containing a Cypress MCU. This
 * API provides convenience methods for initializing and manipulating different
 * hardware found on the board.
 *
 ***************************************************************************************************
 * \copyright
 * Copyright 2018-2021 Cypress Semiconductor Corporation
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
 **************************************************************************************************/

/**
 * \addtogroup group_bsp_wifi WiFi Initialization
 * \{
 * Basic integration code for interfacing the WiFi Host Driver (WHD) with the Board
 * Support Packages (BSPs).
 */
#pragma once

#include "cy_result.h"
#include "whd_wifi_api.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Initialization of the WiFi driver failed. */
#define CYBSP_RSLT_WIFI_INIT_FAILED \
    (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_BOARD_LIB_WHD_INTEGRATION, 0))

/** SDIO enumeration failed. */
#define CYBSP_RSLT_WIFI_SDIO_ENUM_TIMEOUT \
    (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_BOARD_LIB_WHD_INTEGRATION, 1))

/** Initializes the primary interface for the WiFi driver on the board. This sets up
 * the WHD interface to use the \ref group_bsp_network_buffer APIs and to communicate
 * over the SDIO interface on the board. This function does the following:<br>
 * 1) Initializes the WiFi driver.<br>
 * 2) Turns on the WiFi chip.
 *
 * @note This function cannot be called multiple times. If the interface needs to be
 * reinitialized, \ref cybsp_wifi_deinit must be called before calling this function
 * again.
 *
 * @param[out] interface     Interface to be initialized
 * @param[in]  resource_if   Pointer to resource interface to provide resources to the driver
 *                           initialization process. Passing NULL will use the default.
 * @param[in]  buffer_if     Pointer to a buffer interface to provide buffer related services to the
 *                           driver instance. Passing NULL will use the default.
 * @param[in]  netif_if      Pointer to a whd_netif_funcs_t to provide network stack services to the
 *                           driver instance. Passing NULL will use the default.
 *
 * @return CY_RSLT_SUCCESS for successful initialization or error if initialization failed.
 */
cy_rslt_t cybsp_wifi_init_primary_extended(whd_interface_t* interface,
                                           whd_resource_source_t* resource_if,
                                           whd_buffer_funcs_t* buffer_if,
                                           whd_netif_funcs_t* netif_if);

/**
 * Initializes the primary interface for the WiFi driver on the board using the default resource,
 * buffer, and network interfaces.
 * See cybsp_wifi_init_primary_extended() for more details.
 *
 * @param[out] interface     Interface to be initialized
 *
 * @return CY_RSLT_SUCCESS for successful initialization or error if initialization failed.
 */
static inline cy_rslt_t cybsp_wifi_init_primary(whd_interface_t* interface)
{
    return cybsp_wifi_init_primary_extended(interface, NULL, NULL, NULL);
}


/** This function initializes and adds a secondary interface to the WiFi driver.
 *  @note This function does not initialize the WiFi driver or turn on the WiFi chip.
 * That is required to be done by first calling \ref cybsp_wifi_init_primary.
 *
 * @param[out] interface Interface to be initialized
 * @param[in] mac_address Mac address for secondary interface
 *
 * @return CY_RSLT_SUCCESS for successful initialization or error if initialization failed.
 */
cy_rslt_t cybsp_wifi_init_secondary(whd_interface_t* interface, whd_mac_t* mac_address);

/** De-initializes all WiFi interfaces and the WiFi driver. This function does the
 * following:<br>
 * 1) Deinitializes all WiFi interfaces and WiFi driver.<br>
 * 2) Turns off the WiFi chip.
 *
 * @param[in] interface Interface to be de-initialized.
 *
 * @return CY_RSLT_SUCCESS for successful de-initialization or error if de-initialization failed.
 */
cy_rslt_t cybsp_wifi_deinit(whd_interface_t interface);

/** Gets the wifi driver instance initialized by the driver. This should only be called
 * after being initialized by \ref cybsp_wifi_init_primary() and before being
 * deinitialized by \ref cybsp_wifi_deinit(). This is also the only time where the
 * driver reference is valid.
 *
 * @return Wifi driver instance pointer.
 */
whd_driver_t cybsp_get_wifi_driver(void);

#ifdef __cplusplus
}
#endif // __cplusplus

/** \} group_bsp_wifi */
