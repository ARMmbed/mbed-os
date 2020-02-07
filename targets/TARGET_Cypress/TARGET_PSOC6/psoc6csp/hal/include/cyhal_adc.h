/***************************************************************************//**
* \file cyhal_adc.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Analog to
* Digital Converter. This interface abstracts out the chip specific details.
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
* \addtogroup group_hal_adc ADC (Analog to Digital Converter)
* \ingroup group_hal
* \{
* High level interface for interacting with the analog to digital converter (ADC).
*
* Each ADC instance supports one or more selectable channels, each
* of which can perform conversions on a different pin.
* See the device datasheet for details about which pins support ADC conversion.
*
* In order to use the ADC, first call cyhal_adc_init to initialize an ADC instance.
* Then call cyhal_adc_channel_init to initialize one or more channels associated with
* that instance.
*
* All channels are single-ended.
* The values returned by the read API are relative to the ADC's voltage range, which
* is device specific.
*
* \defgroup group_hal_adc_common Common
* \defgroup group_hal_adc_functions ADC Functions
* \defgroup group_hal_adc_channel_functions ADC Channel Functions
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

/** Maximum value that the ADC can return */
#define CYHAL_ADC_MAX_VALUE 0xFFFF

/** Bad argument */
#define CYHAL_ADC_RSLT_BAD_ARGUMENT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_ADC, 0))
/** Failed to initialize ADC clock */
#define CYHAL_ADC_RSLT_FAILED_CLOCK (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_ADC, 1))
/** Failed to initialize ADC */
#define CYHAL_ADC_RSLT_FAILED_INIT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_ADC, 2))
/** No channels available */
#define CYHAL_ADC_RSLT_NO_CHANNELS (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_ADC, 3))

/** Initialize ADC peripheral
 *
 * @param[out] obj The adc object to initialize
 * @param[in]  pin A pin corresponding to the ADC block to initialize
 *  Note: This pin is not reserved, it is just used to identify which ADC block to allocate.
 *  If multiple channels will be allocated for a single ADC instance, only one pin should be
 *  passed here; it does not matter which one. After calling this function once, call
 *  cyhal_adc_channel_init once for each pin whose value should be measured.
 * @param[in]  clk The clock to use can be shared, if not provided a new clock will be allocated
 * @return The status of the init request
 */
cy_rslt_t cyhal_adc_init(cyhal_adc_t *obj, cyhal_gpio_t pin, const cyhal_clock_divider_t *clk);

/** Uninitialize the ADC peripheral and cyhal_adc_t object
 *
 * @param[in,out] obj The ADC object
 */
void cyhal_adc_free(cyhal_adc_t *obj);

/** Initialize a single-ended ADC channel.
 *
 * Configures the pin used by ADC.
 * @param[out] obj The adc channel object to initialize
 * @param[in]  adc The adc for which the channel should be initialized
 * @param[in]  pin The adc pin name
 * @return The status of the init request
 */
cy_rslt_t cyhal_adc_channel_init(cyhal_adc_channel_t *obj, cyhal_adc_t* adc, cyhal_gpio_t pin);

/** Uninitialize the ADC channel and cyhal_adc_channel_t object
 *
 * @param[in,out] obj The ADC channel object
 */
void cyhal_adc_channel_free(cyhal_adc_channel_t *obj);

/** Read the value from ADC pin, represented as an unsigned 16bit value
 *  where 0x0000 represents the minimum value in the ADC's range, and 0xFFFF
 *  represents the maximum value in the ADC's range.
 *
 * @param[in] obj The ADC object
 * @return An unsigned 16bit value representing the current input voltage
 */
uint16_t cyhal_adc_read_u16(const cyhal_adc_channel_t *obj);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_ADC_IMPL_HEADER
#include CYHAL_ADC_IMPL_HEADER
#endif /* CYHAL_ADC_IMPL_HEADER */

/** \} group_hal_adc */
