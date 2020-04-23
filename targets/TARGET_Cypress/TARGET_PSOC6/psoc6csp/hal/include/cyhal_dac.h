/***************************************************************************//**
* \file cyhal_dac.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Digital to
* Analog Converter. This interface abstracts out the chip specific details. If
* any chip specific functionality is necessary, or performance is critical the
* low level functions can be used directly.
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
* \addtogroup group_hal_dac DAC (Digital to Analog Converter)
* \ingroup group_hal
* \{
* High level interface for interacting with the digital to analog converter (DAC).
*
* This block drives a pin with a firmware configurable voltage. See the device datasheet
* for details on which pins support DAC output.
*
* The cyhal_dac_write and cyhal_dac_read APIs are defined relative to the DAC's output
* voltage range, which is device dependent.
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

/** Bad argument */
#define CYHAL_DAC_RSLT_BAD_ARGUMENT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_DAC, 0))
/** Failed to initialize DAC */
#define CYHAL_DAC_RSLT_FAILED_INIT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_DAC, 1))

/** Initialize the DAC peripheral
 *
 * Configures the pin used by dac.
 * @param[in] obj The dac object to initialize
 * @param[in] pin The dac pin name
 * @return The status of the init request
 */
cy_rslt_t cyhal_dac_init(cyhal_dac_t *obj, cyhal_gpio_t pin);

/** Release the dac object
 *
 * @param[in,out] obj The dac object
 */
void cyhal_dac_free(cyhal_dac_t *obj);

/** Set the output voltage, as a normalized unsigned 16-bit value
 * (where 0 is the lowest value the DAC can output and 0xFFFF
 * is the highest)
 *
 * @param[in] obj        The dac object
 * @param[in] value The 16-bit output value to set
 */
void cyhal_dac_write(const cyhal_dac_t *obj, uint16_t value);

/** Read the current DAC output voltage setting, as a normalized unsigned
 * 16-bit value (where 0 is the lowest value the DAC can output and 0xFFFF
 * is the highest).
 * Note: Depending on the precision of the underlying hardware, this may not
 * precisely match the most recent value passed in to cyhal_dac_write.
 *
 * @param[in]  obj        The dac object
 * @return The 16-bit output value
 */
uint16_t cyhal_dac_read(cyhal_dac_t *obj);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_DAC_IMPL_HEADER
#include CYHAL_DAC_IMPL_HEADER
#endif /* CYHAL_DAC_IMPL_HEADER */

/** \} group_hal_dac */
