/***************************************************************************//**
* \file cyhal_opamp.h
*
* \brief
* Provides a high level interface for interacting with the Cypress OpAmp.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
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
* \addtogroup group_hal_opamp OPAMP (Operational Amplifier)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress OpAmp.
*
* \defgroup group_hal_opamp_macros Macros
* \defgroup group_hal_opamp_functions Functions
* \defgroup group_hal_opamp_data_structures Data Structures
* \defgroup group_hal_opamp_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_hal_opamp_functions
* \{
*/

/** Initialize the opamp peripheral.
 *  If vinp is NULL, it will initialize in follower mode.
 *
 * @param[out] obj  The opamp object
 * @param[in]  vinp The vplus pin
 * @param[in]  vinm The vminus pin
 * @param[in]  vout The vout pin
 * @return The status of the init request
 */
cy_rslt_t cyhal_opamp_init(cyhal_opamp_t *obj, cyhal_gpio_t vinp, cyhal_gpio_t vinm, cyhal_gpio_t vout);

/** Release the opamp peripheral.
 *
 * @param[in,out] obj The opamp object
 */
void cyhal_opamp_free(cyhal_opamp_t *obj);

/** Reconfigure the opamp object
 *
 * @param[in,out] obj       The opamp object
 * @param[in]     power     Power mode to operate in (0=off, 1=low, 2=medium, 3=high)
 * @param[in]     deepsleep Does this need to operate in deepsleep
 * @return The status of the power request
 */
cy_rslt_t cyhal_opamp_power(cyhal_opamp_t *obj, uint8_t power, bool deepsleep);

/** \} group_hal_opamp_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_opamp */
