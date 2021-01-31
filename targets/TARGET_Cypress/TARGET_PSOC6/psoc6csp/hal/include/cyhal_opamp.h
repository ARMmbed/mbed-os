/***************************************************************************//**
* \file cyhal_opamp.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Opamp.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is required, the low level functions can be used directly.
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
* \addtogroup group_hal_opamp Opamp (Operational Amplifier)
* \ingroup group_hal
* \{
* High level interface for interacting with the Operational Amplifier (Opamp).
*
* \section cyhal_opamp_features Features
* Each opamp can operate in one of two modes:
* - Opamp: Bare opamp with two input pins.
* - Follower: Also known as unity gain. Buffers the signal on a single input pin
*             and drives the same voltage on the output.
*
* In both modes, the output is driven off chip via another pin.
*
* \section cyhal_opamp_quickstart Quickstart
* Call \ref cyhal_opamp_init to initialize an opamp instance by providing the opamp
* object (**obj**), non-inverting input pin (**vin_p**), inverting input pin (**vin_m**),
* and output pin (**vout**). If follower mode is desired, pass `NC` for **vin_m**.
*
* Use \ref cyhal_opamp_set_power to configure the opamp power.
*
* \section subsection_opamp_snippets Code Snippets
* \note Error checking is omitted for clarity
* \section subsection_opamp_snippet_1 Snippet 1: Bare opamp initialization
* The following snippet initializes a bare opamp. Note that any passive components
* (e.g. resistive feedback) must be implemented off-chip.
* \snippet opamp.c snippet_cyhal_opamp_init_diff
* \section subsection_opamp_snippet_2 Snippet 2: Opamp follower initialization
* The following snippet initializes an opamp as a follower.
* \snippet opamp.c snippet_cyhal_opamp_init_follower
* \section subsection_opamp_snippet_3 Snippet 3: Opamp powering-off and on
* The following snippet demonstrates temporarily powering-off the opamp without freeing it.
* \snippet opamp.c snippet_cyhal_opamp_start_stop
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

/** \addtogroup group_hal_results_opamp Opamp HAL Results
 *  Opamp specific return codes
 *  \ingroup group_hal_results
 *  \{ *//**
 */

/** The requested pins are invalid */
#define CYHAL_OPAMP_RSLT_ERR_INVALID_PIN                  \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_OPAMP, 1))
/** Bad argument */
#define CYHAL_OPAMP_RSLT_BAD_ARGUMENT                     \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_OPAMP, 2))
/**
 * \}
 */

#if defined(__cplusplus)
extern "C" {
#endif

/** Initialize the opamp peripheral.
 *  If vin_m is @ref NC, the opamp will be initialized in follower mode (unity gain).
 *
 * The opamp will be initialized but not powered-on until @ref cyhal_opamp_set_power is called.
 *
 * @param[out] obj  Pointer to an opamp object. The caller must allocate the memory
 *                  for this object but the init function will initialize its contents.
 * @param[in] vin_p Non-inverting input
 * @param[in] vin_m Inverting input
 * @param[in] vout  opamp output
 * @return The status of the init request
 */
cy_rslt_t cyhal_opamp_init(cyhal_opamp_t *obj, cyhal_gpio_t vin_p, cyhal_gpio_t vin_m, cyhal_gpio_t vout);

/** Deinitialize the opamp peripheral and free associated resources.
 *
 * This will disconnect all inputs and outputs, including internal feedback.
 *
 * @param[in] obj The opamp object
 */
void cyhal_opamp_free(cyhal_opamp_t *obj);

/** Changes the current operating power level of the opamp.
 *
 * If the power level is set to @ref CYHAL_POWER_LEVEL_OFF, the opamp will be powered-off
 * but it will retain its configuration, so it is not necessary to reconfigure it when changing
 * the power level from @ref CYHAL_POWER_LEVEL_OFF to any other value.
 *
 * @param[in] obj   Opamp object
 * @param[in] power The power level to set
 * @return The status of the set power request
 */
cy_rslt_t cyhal_opamp_set_power(cyhal_opamp_t *obj, cyhal_power_level_t power);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_OPAMP_IMPL_HEADER
#include CYHAL_OPAMP_IMPL_HEADER
#endif /* CYHAL_OPAMP_IMPL_HEADER */

/** \} group_hal_opamp */
