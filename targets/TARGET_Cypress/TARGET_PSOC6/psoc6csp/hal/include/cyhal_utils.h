/***************************************************************************//**
* \file cyhal_utils.h
*
* \brief
* Provides utility functions for working with the PSoC 6 HAL implementation.
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

/** \cond INTERNAL */
/**
* \addtogroup group_hal_psoc6 PSoC 6 Implementation Specific
* \{
* Common utility macros & functions used by multiple HAL drivers.
*/

#pragma once

#include "cy_result.h"
#include "cyhal_hw_types.h"
#include "cy_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CYHAL_IRQN_OFFSET           (16U) /**< Offset for implementation-defined ISR type numbers (IRQ0 = 16) */
#define CYHAL_GET_CURRENT_IRQN()    ((IRQn_Type) (__get_IPSR() - CYHAL_IRQN_OFFSET)) /**< Macro to get the IRQn of the current ISR */

#define CYHAL_MAP_COLUMNS           (2U) /**< Number of columns in the flag map */

/**
* \addtogroup group_hal_psoc6_pin_package
* \{
*/

/** Looks up the resource block that connects to the specified pins from the provided resource pin mapping table.
 * This is a convenience utility for cyhal_utils_get_resource() if the mappings is an array of known size.
 *
 * @param[in] pin        The pin to lookup the hardware block for
 * @param[in] mappings   The mappings of pin to hardware block
 * @return The entry for the specified pin if it exists, or null if it doesn't.
 */
#define CY_UTILS_GET_RESOURCE(pin, mappings) cyhal_utils_get_resource(pin, mappings, sizeof(mappings)/sizeof(cyhal_resource_pin_mapping_t))

/** Converts the provided gpio pin to a resource instance object
 *
 * @param[in] pin  The pin to get a resource object for
 * @return The equivalent resource instance object for the provided pin.
 */
static inline cyhal_resource_inst_t cyhal_utils_get_gpio_resource(cyhal_gpio_t pin)
{
    cyhal_resource_inst_t rsc = { CYHAL_RSC_GPIO, CYHAL_GET_PORT(pin), CYHAL_GET_PIN(pin) };
    return rsc;
}

/** Looks up the resource block that connects to the specified pins from the provided resource pin mapping table.
 *
 * @param[in] pin        The pin to lookup the hardware block for
 * @param[in] mappings   The mappings of pin to hardware block
 * @param[in] count      The number of items in the mappings table
 * @return The entry for the specified pin if it exists, or null if it doesn't.
 */
const cyhal_resource_pin_mapping_t *cyhal_utils_get_resource(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t* mappings, size_t count);

/** Attempts to reserve the specified pin and then initialize it to connect to the item defined by the provided mapping object.
 * @param[in] pin        The pin to reserve and connect
 * @param[in] mapping    The pin/hardware block connection mapping information
 * @return CY_RSLT_SUCCESS if everything was ok, else an error.
 */
cy_rslt_t cyhal_utils_reserve_and_connect(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t *mapping);

/** Disconnects any routing for the pin from the interconnect driver and then free's the pin from the hwmgr.
 *
 * @param[in] pin       The pin to disconnect and free
 */
void cyhal_utils_disconnect_and_free(cyhal_gpio_t pin);

/** Checks to see if the provided pin is a no-connect (CYHAL_NC_PIN_VALUE). If not, calls
 * cyhal_utils_disconnect_and_free().
 *
 * @param[in] pin       The pin to disconnect and free
 */
void cyhal_utils_release_if_used(cyhal_gpio_t *pin);

/** \} group_hal_psoc6_pin_package */

/** Calculate the peri clock divider value that need to be set to reach frequency closest to the input frequency
 *
 * @param[in] frequency The desired frequency
 * @param[in] frac_bits The number of fractional bits that the divider has
 * @return The calculate divider value to set, NOTE a divider value of x divide the frequency by (x+1)
 */
static inline uint32_t cyhal_divider_value(uint32_t frequency, uint32_t frac_bits)
{
    return ((Cy_SysClk_ClkPeriGetFrequency() * (1 << frac_bits)) + (frequency / 2)) / frequency - 1;
}

/** Determine if two resources are the same. If more than two instances need to be
 * compared, it is better to call \ref cyhal_utils_resources_equal_all().
 *
 * @param[in] resource1 First resource to compare
 * @param[in] resource2 Second resource to compare
 * @return Boolean indicating whether two resources are the same
 */
bool cyhal_utils_resources_equal(const cyhal_resource_inst_t *resource1, const cyhal_resource_inst_t *resource2);

/** Determine if the set of resources are the same. This expects at least two resource
 * instances to be provided. NOTE: If only two instances are provided it is better
 * to use \ref cyhal_utils_resources_equal().
 *
 * @param[in] count The number of resources being provided to compare
 * @param[in] ...   Variable List of const cyhal_resource_inst_t* items to compare
 * @return Boolean indicating whether resource instances are the same
 */
bool cyhal_utils_resources_equal_all(uint32_t count, ...);

/** Converts a set of flags from one representation to a equivalent set of flags
 * in another representation.
 * For example usage see cyhal_syspm.c or cyhal_sdhc.c
 *
 * @param[in] map           Map of the flags from one representation to the other.
 *                          The keys represent the source flags and the values
 *                          represent the equivalent flags to be converted to.
 * @param[in] from_column   Column index of the flags to convert from in the map.
 * @param[in] to_column     Column index of the equivalent flags to be converted to in the map.
 * @param[in] count         Count of the total flags in the list.
 * @param[in] source_flags  Source flags to be converted.
 * @return Converted flags.
 */
uint32_t cyhal_utils_convert_flags(
    const uint32_t map[][CYHAL_MAP_COLUMNS],
    uint8_t from_index,
    uint8_t to_index,
    uint32_t count,
    uint32_t source_flags);

/** Converts a hal pm mode to a pdl mode
 *
 * @param[in] mode          hal power management callback mode.
 * @return Equivalent pdl syspm mode.
 */
cy_en_syspm_callback_mode_t cyhal_utils_convert_haltopdl_pm_mode(cyhal_syspm_callback_mode_t mode);

/** Converts a pdl pm mode to a hal mode
 *
 * @param[in] mode          pdl syspm power management callback mode.
 * @return Equivalent hal pm callback mode.
 */
cyhal_syspm_callback_mode_t cyhal_utils_convert_pdltohal_pm_mode(cy_en_syspm_callback_mode_t mode);

/**
 * Utility method to check if the clock is using the new data format (true) or the old
 * format (false).
 * @param[out] clock    The clock instance to check which format it is using.
 * @return Indication of whether the provided clock is using the new format (true) or old (false)
 */
static inline bool cyhal_utils_is_new_clock_format(const cyhal_clock_t *clock)
{
    return (((cyhal_clock_block_t)clock->div_type == clock->block) && (clock->div_num == clock->channel));
}

/** Gets the peripheral divider information from a provided clock instance. The clock can be using either
 * the new or the old format for clocks.
 *
 * @param[in]   clock       The clock to get peripheral divider information from
 * @param[out]  div_type    The divider type the clock instance represents
 * @param[out]  div_num     The divider number the clock instance represents
 */
void cyhal_utils_get_peri_clock_details(const cyhal_clock_t *clock, cy_en_divider_types_t *div_type, uint32_t *div_num);

/**
 * Allocates a clock that can drive the specified instance.
 * 
 * @param[out]  clock           The clock object to initialize
 * @param[in]   clocked_item    The destination that the allocated clock must be able to drive
 * @param[in]   div             The divider width that is required. This is ignored if the block is hard-wired to an HFCLK output
 * @param[in]   accept_larger   If no dividers of the specified width are available, can a wider divider be substituted.
 */
cy_rslt_t cyhal_utils_allocate_clock(cyhal_clock_t *clock, const cyhal_resource_inst_t *clocked_item, cyhal_clock_divider_types_t div, bool accept_larger);

/** 
 * Calculates clock tolerance in the specified units given a desired and actual frequency
 * 
 * @param[in] type          tolerance type
 * @param[in] desired_hz    desired clock frequency in hertz
 * @param[in] actual_hz     actual clock frequency in hertz
 * @return the computed tolerance
 */
int32_t cyhal_utils_calculate_tolerance(cyhal_clock_tolerance_unit_t type, uint32_t desired_hz, uint32_t actual_hz);

/**
 * Attempts to set the clock to the specified frequency. This is similar to cyhal_clock_set_frequency, but it will also make
 * an attempt to set the frequency for HFCLK outputs, which are not supported by the public API due to their limited range
 * of supported dividers (1, 2, 4, 8)
 * 
 * @param[in] clock     The clock instance to set the frequency for.
 * @param[in] hz        The frequency, in hertz, to set the clock to.
 * @param[in] tolerance The allowed tolerance from the desired hz that is acceptable, use NULL if no
 *                      tolerance check is required.
 */
cy_rslt_t cyhal_utils_set_clock_frequency(cyhal_clock_t* clock, uint32_t hz, const cyhal_clock_tolerance_t *tolerance);

#if defined(__cplusplus)
}
#endif

/** \} group_hal_psoc6_utils */
/** \} group_hal_psoc6 */
/** \endcond */
