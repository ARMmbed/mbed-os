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
* \section subsection_dac_features Features
* The DAC driver provides APIs to:
* * Configure and work with the DAC hardware
* * Update the DAC output value
* * Read the DAC output voltage setting
*
* \note The \ref cyhal_dac_write and \ref cyhal_dac_read APIs are defined as 16-bit, which may not match the underlying hardware.
* The functions will linearly scale the 16-bit value to and from a resolution that hardware is capable of outputting.
* For instance, if the device supports 12-bit resolution, only the 12 MSBs of the 16-bit value will be used.
* Refer to BSP Settings section in the kit's BSP API Reference Manual for details on the DAC's output range.
*
* \section subsection_dac_quickstart Quick Start
* Call \ref cyhal_dac_init to initialize a DAC instance by providing the DAC
* object ( <b> obj </b> ) and an output pin (<b> pin </b>).
*
* See \ref subsection_dac_use_case_1.
*
* \section subsection_dac_snippets Code Snippets
*
* \subsection subsection_dac_use_case_1 Use case 1: Simple DAC initialization
* The following snippet initializes a DAC resource and assigns the output to the specified <b>pin</b> using \ref cyhal_dac_init.
* \ref cyhal_dac_write is used to set the DAC output value. \ref cyhal_dac_read is used to read back DAC register.
* \snippet dac.c snippet_cyhal_dac_simple_init
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** \addtogroup group_hal_results
 *  \{ *//**
 *  \{ @name DAC Results
 */

/** Bad argument */
#define CYHAL_DAC_RSLT_BAD_ARGUMENT                     \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_DAC, 0))
/** Failed to initialize DAC */
#define CYHAL_DAC_RSLT_FAILED_INIT                      \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_DAC, 1))

/**
 * \} \}
 */

/** Initialize the DAC peripheral
 *
 * Configures the pin used by dac.
 *
 * @param[out] obj  Pointer to a DAC object. The caller must allocate the memory
 *  for this object but the init function will initialize its contents.
 * @param[in] pin The dac pin name
 * @return The status of the init request. On success returns \ref CY_RSLT_SUCCESS.<br>
 * On failure, a problem specific error code will be returned.
 * This error could be from the HAL or lower level driver. <br>
 * For all other return codes, please refer to device driver documentation available in the BSP landing page
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
