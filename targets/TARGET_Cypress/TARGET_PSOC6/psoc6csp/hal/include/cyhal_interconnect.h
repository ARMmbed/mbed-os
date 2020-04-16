/***************************************************************************//**
* \file cyhal_interconnect.h
*
* \brief
* Provides a high level interface for interacting with the internal digital
* routing on the chip. This interface abstracts out the chip specific details.
* If any chip specific functionality is necessary, or performance is critical
* the low level functions can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
* \addtogroup group_hal_interconnect INTERCONNECT (Internal digital routing)
* \ingroup group_hal
* \{
* High level interface for interacting with the digital routing.
*
* This provides limited facilities for runtime manipulation of the on chip routing.
* The following types of connections are supported:
* * Connection from a peripheral to a pin. (A dedicated connection must exist
    between the pin and the peripheral; see the device datasheet for more details)
* * Experimental support for connecting between two on-chip "trigger" terminals.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"
#include "cyhal_modules.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** No connection is available */
#define CYHAL_CONNECT_RSLT_NO_CONNECTION (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_INTERCONNECT, 0))
/** The connections source and destination are already connected */
#define CYHAL_CONNECT_RSLT_ALREADY_CONNECTED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_INTERCONNECT, 1))
/** Invalid trigger connection */
#define CYHAL_CONNECT_RSLT_INVALID_TRIGGER_CONNECTION (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_INTERCONNECT, 2))

/** Indicates that a mux output does not continue to another mux */
#define CYHAL_INTERCONNECT_MUX_NOT_CONTINUATION 0xFF

/** Connect a pin to a peripheral terminal. This will route a direct connect from the pin to the peripheral.
 * Any previous direct connection from the pin will be overriden.
 * @param[in] pin_connection  Details about the pin and its target connection
 * @return The status of the connect request
 */
cy_rslt_t cyhal_connect_pin(const cyhal_resource_pin_mapping_t *pin_connection);

/** Disconnect a peripheral from a pin. This will also reset the pin's drive mode to High-Z.
 * @param[in] pin The pin to disconnect
 * @return The status of the disconnect request
 */
cy_rslt_t cyhal_disconnect_pin(cyhal_gpio_t pin);

/** WORK IN PROGRESS. This function is not yet fully implemented.
 * Connects two digital terminals on the device using any internal interconnect. As expected, a single
 * source can drive multiple destinations, but a destination can only be driven by a single source.
 * If the output is already connected, or the connection can not be established an error will be returned.
 * @param[in] source The source of the signal to connect
 * @param[in] dest   The destination of the signal to connect
 * @return The status of the connect request
 */
cy_rslt_t cyhal_connect_trigger(cyhal_source_t source, cyhal_dest_t dest);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_INTERCONNECT_IMPL_HEADER
#include CYHAL_INTERCONNECT_IMPL_HEADER
#endif /* CYHAL_INTERCONNECT_IMPL_HEADER */

/** \} group_hal_interconnect */
