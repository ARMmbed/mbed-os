/***************************************************************************//**
* \file cyhal_utils.h
*
* \brief
* Provides utility functions for working with the CAT1/CAT2 HAL implementation.
*
********************************************************************************
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
*******************************************************************************/

/** \cond INTERNAL */
/**
* \addtogroup group_hal_impl
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

#define _CYHAL_UTILS_NS_PER_SECOND (1000000000)

#define _CYHAL_UTILS_IRQN_OFFSET           (16U) /**< Offset for implementation-defined ISR type numbers (IRQ0 = 16) */
#define _CYHAL_UTILS_GET_CURRENT_IRQN()    ((IRQn_Type) (__get_IPSR() - _CYHAL_UTILS_IRQN_OFFSET)) /**< Macro to get the IRQn of the current ISR */

/**
* \addtogroup group_hal_impl_pin_package
* \{
*/

/** Attempts to reserve a resource block that connects to the specified pin from the provided resource pin mapping table.
  * If the first matching block cannot be reserved, this will continue searching until either a match is successfully
  * reserved or the end of the table is reached.
 * This is a convenience utility for _cyhal_utils_get_resource() if the mappings is an array of known size.
  *
  * @param[in] pin      The pin to which the reserved hardware block must connect
  * @param[in] mappings The mapping of pin to hardware block
  * @return The entry for the specified pin corresponding to the reserved block, if it exists and was successfully reserved.
            Otherwise, NULL.
  */
#define _CYHAL_UTILS_TRY_ALLOC(pin, mappings) \
    _cyhal_utils_try_alloc(pin, mappings, sizeof(mappings)/sizeof(cyhal_resource_pin_mapping_t))

/** Looks up the resource block that connects to the specified pins from the provided resource pin mapping table.
 * This is a convenience utility for _cyhal_utils_get_resource() if the mappings is an array of known size.
 *
 * @param[in] pin        The pin to lookup the hardware block for
 * @param[in] mappings   The mappings of pin to hardware block
 * @return The entry for the specified pin if it exists, or null if it doesn't.
 */
#define _CYHAL_UTILS_GET_RESOURCE(pin, mappings) _cyhal_utils_get_resource(pin, mappings, sizeof(mappings)/sizeof(cyhal_resource_pin_mapping_t), NULL)

/** Converts the provided gpio pin to a resource instance object
 *
 * @param[in] pin  The pin to get a resource object for
 * @return The equivalent resource instance object for the provided pin.
 */
static inline cyhal_resource_inst_t _cyhal_utils_get_gpio_resource(cyhal_gpio_t pin)
{
    cyhal_resource_inst_t rsc = { CYHAL_RSC_GPIO, CYHAL_GET_PORT(pin), CYHAL_GET_PIN(pin) };
    return rsc;
}

/** Looks up the resource block that connects to the specified pins from the provided resource pin mapping table.
 *
 * @param[in] pin        The pin to lookup the hardware block for
 * @param[in] mappings   The mappings of pin to hardware block
 * @param[in] count      The number of items in the mappings table
 * @param[in] block_res  If not NULL, find pin mapping, that belongs to specified block
 * @return The entry for the specified pin if it exists, or null if it doesn't.
 */
const cyhal_resource_pin_mapping_t *_cyhal_utils_get_resource(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t* mappings, size_t count, const cyhal_resource_inst_t* block_res);

/** Attempts to reserve a resource block that connects to the specified pin from the provided resource pin mapping table.
  * If the first matching block cannot be reserved, this will continue searching until either a match is successfully
  * reserved or the end of the table is reached.
  *
  * @param[in] pin      The pin to which the reserved hardware block must connect
  * @param[in] mappings The mapping of pin to hardware block
  * @param[in] size     The number of items in the mappings table
  * @return The entry for the specified pin corresponding to the reserved block, if it exists and was successfully reserved.
            Otherwise, NULL.
  */
const cyhal_resource_pin_mapping_t* _cyhal_utils_try_alloc(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t *mappings, size_t count);

/** Attempts to reserve the specified pin and then initialize it to connect to the item defined by the provided mapping object.
 * @param[in] pin        The pin to reserve and connect
 * @param[in] mapping    The pin/hardware block connection mapping information
 * @return CY_RSLT_SUCCESS if everything was ok, else an error.
 */
cy_rslt_t _cyhal_utils_reserve_and_connect(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t *mapping);

/** Disconnects any routing for the pin from the interconnect driver and then free's the pin from the hwmgr.
 *
 * @param[in] pin       The pin to disconnect and free
 */
void _cyhal_utils_disconnect_and_free(cyhal_gpio_t pin);

/** Checks to see if the provided pin is a no-connect (CYHAL_NC_PIN_VALUE). If not, calls
 * _cyhal_utils_disconnect_and_free().
 *
 * @param[in] pin       The pin to disconnect and free
 */
void _cyhal_utils_release_if_used(cyhal_gpio_t *pin);

/** \} group_hal_impl_pin_package */

/** Calculate a peripheral clock divider value that needs to be set to reach the frequency closest
 * to the one requested.
 * \note The caller may need to subtract one from the value returned in order to align with the
 * API/register requirements. This is necessary if the API/register expects a value of 0 to mean
 * divide by 1.
 *
 * @param[in] frequency The desired frequency
 * @param[in] frac_bits The number of fractional bits that the divider has
 * @return The calculated divider value
 */
static inline uint32_t _cyhal_utils_divider_value(uint32_t frequency, uint32_t frac_bits)
{
    #if defined(COMPONENT_CAT1A)
    return ((Cy_SysClk_ClkPeriGetFrequency() * (1 << frac_bits)) + (frequency / 2)) / frequency;
    #elif defined(COMPONENT_CAT2)
    return ((Cy_SysClk_ClkSysGetFrequency() * (1 << frac_bits)) + (frequency / 2)) / frequency;
    #endif
}

/** Determine if two resources are the same. If more than two instances need to be
 * compared, it is better to call \ref _cyhal_utils_resources_equal_all().
 *
 * @param[in] resource1 First resource to compare
 * @param[in] resource2 Second resource to compare
 * @return Boolean indicating whether two resources are the same
 */
bool _cyhal_utils_resources_equal(const cyhal_resource_inst_t *resource1, const cyhal_resource_inst_t *resource2);

/** Determine if the set of resources are the same. This expects at least two resource
 * instances to be provided. NOTE: If only two instances are provided it is better
 * to use \ref _cyhal_utils_resources_equal().
 *
 * @param[in] count The number of resources being provided to compare
 * @param[in] ...   Variable List of const cyhal_resource_inst_t* items to compare
 * @return Boolean indicating whether resource instances are the same
 */
bool _cyhal_utils_resources_equal_all(uint32_t count, ...);

/** Converts a set of flags from one representation to a equivalent set of flags
 * in another representation.
 * For example usage see cyhal_syspm.c or cyhal_sdhc.c
 *
 * @param[in] map           New flag values. Each index corresponds to a bit position
 *                          from the source_flags. Index 0 is a default value if nothing
 *                          matches.
 * @param[in] count         Count of the total flags in the list.
 * @param[in] source_flags  Source flags to be converted.
 * @return Converted flags.
 */
uint32_t _cyhal_utils_convert_flags(const uint32_t map[], uint32_t count, uint32_t source_flags);

/** Converts a hal pm mode to a pdl mode
 *
 * @param[in] mode          hal power management callback mode.
 * @return Equivalent pdl syspm mode.
 */
cy_en_syspm_callback_mode_t _cyhal_utils_convert_haltopdl_pm_mode(cyhal_syspm_callback_mode_t mode);

/** Converts a pdl pm mode to a hal mode
 *
 * @param[in] mode          pdl syspm power management callback mode.
 * @return Equivalent hal pm callback mode.
 */
cyhal_syspm_callback_mode_t _cyhal_utils_convert_pdltohal_pm_mode(cy_en_syspm_callback_mode_t mode);

/**
 * Utility method to check if the clock is using the new data format (true) or the old
 * format (false).
 * @param[out] clock    The clock instance to check which format it is using.
 * @return Indication of whether the provided clock is using the new format (true) or old (false)
 */
static inline bool _cyhal_utils_is_new_clock_format(const cyhal_clock_t *clock)
{
    #if defined(COMPONENT_CAT1A)
    return (((cyhal_clock_block_t)clock->div_type == clock->block) && (clock->div_num == clock->channel));
    #else
    CY_UNUSED_PARAMETER(clock);
    return true;
    #endif
}

/**
 * Utility method to update clock format to new format if the clock is using the old data format.
 *
 * @param[out] clock    The clock instance to check which format it is using.
 */
static inline void _cyhal_utils_update_clock_format(cyhal_clock_t *clock)
{
    #if defined(COMPONENT_CAT1A)
    if(((cyhal_clock_block_t)clock->div_type != clock->block) || (clock->div_num != clock->channel))
    {
        clock->block = (cyhal_clock_block_t)clock->div_type;
        clock->channel = clock->div_num;
    }
    #else
    CY_UNUSED_PARAMETER(clock);
    #endif
}

/** Gets the peripheral divider information from a provided clock instance. The clock can be using either
 * the new or the old format for clocks.
 *
 * @param[in]   clock               The clock to get peripheral divider information from
 * @param[out]  div_type            The divider type the clock instance represents
 * @param[out]  div_num             The divider number the clock instance represents
 */
void _cyhal_utils_get_peri_clock_details(const cyhal_clock_t *clock, cy_en_divider_types_t *div_type, uint32_t *div_num);

/**
 * Calculates clock tolerance in the specified units given a desired and actual frequency
 *
 * @param[in] type                  tolerance type
 * @param[in] desired_hz            desired clock frequency in hertz
 * @param[in] actual_hz             actual clock frequency in hertz
 * @return the computed tolerance
 */
int32_t _cyhal_utils_calculate_tolerance(cyhal_clock_tolerance_unit_t type, uint32_t desired_hz, uint32_t actual_hz);

/**
 * Allocates a clock that can drive the specified instance.
 *
 * @param[out]  clock               The clock object to initialize
 * @param[in]   clocked_item        The destination that the allocated clock must be able to drive
 * @param[in]   div                 The divider width that is required. This is ignored if the block is hard-wired to
 *                                  an HFCLK output
 * @param[in]   accept_larger       If no dividers of the specified width are available, can a wider divider be
 *                                  substituted.
 */
cy_rslt_t _cyhal_utils_allocate_clock(cyhal_clock_t *clock, const cyhal_resource_inst_t *clocked_item,
                        cyhal_clock_block_t div, bool accept_larger);

#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
/** Function for finding most suitable divider for provided clock */
typedef cy_rslt_t (*_cyhal_utils_clk_div_func_t)(uint32_t hz_src, uint32_t desired_hz,
                        const cyhal_clock_tolerance_t *tolerance, bool only_below_desired, uint32_t *div);

/**
 * Finds best divider for HF clock according to source and desired frequency data
 *
 * @param[in] hz_src                clock source frequency in hertz
 * @param[in] desired_hz            desired clock frequency in hertz
 * @param[in] tolerance             desired clock tolerance to achieve. If NULL provided, all possible dividers will
 *                                  be checked and selected most suitable.
 * @param[in] only_below_desired    resulting clock frequencies, that are above desired_hz will be skipped
 * @param[out] div                  resulting divider
 * @return CYHAL_CLOCK_RSLT_ERR_FREQ if divider is not found, CY_RSLT_SUCCESS in other situations
 */
cy_rslt_t _cyhal_utils_find_hf_clk_div(uint32_t hz_src, uint32_t desired_hz, const cyhal_clock_tolerance_t *tolerance,
                        bool only_below_desired, uint32_t *div);

/**
 * Attempts to set the clock to the specified frequency. This is similar to cyhal_clock_set_frequency,
 * but it will also make an attempt to set the frequency for HFCLK outputs, which are not supported by the public
 * API due to their limited range of supported dividers (1, 2, 4, 8)
 *
 * @param[in] clock                 The clock instance to set the frequency for.
 * @param[in] hz                    The frequency, in hertz, to set the clock to.
 * @param[in] tolerance             The allowed tolerance from the desired hz that is acceptable, use NULL if no
 *                                  tolerance check is required.
 */
cy_rslt_t _cyhal_utils_set_clock_frequency(cyhal_clock_t* clock, uint32_t hz, const cyhal_clock_tolerance_t *tolerance);

/**
 * Finds for provided HF clock most suitable source to achieve target clock frequency and returns it with
 * corresponding divider value. No clock configuration changed in this function.
 *
 * @param[in] clock                 The HFCLK clock instance that needs clock configuration.
 * @param[in] hz                    The maximum frequency, in hertz, that needs to be achieved. The clock will not exceed
 *                                  this value.
 * @param[in] tolerance             The allowed tolerance below the desired hz that is acceptable, use NULL if no
 *                                  tolerance check is required.
 * @param[in] div_find_func         Pointer to _cyhal_utils_clk_div_func_t - type function, that will find most suitable
 *                                  divider for provided clock.
 * @param[out] hf_source            Resulting HF source clock, switching to which, in combination with resulting divider,
 *                                  will provide frequency closest to desired.
 * @param[out] div                  Resulting divider for resulting HF source clock.
 */
cy_rslt_t _cyhal_utils_find_hf_source_n_divider(cyhal_clock_t *clock, uint32_t hz,
                        const cyhal_clock_tolerance_t *tolerance, _cyhal_utils_clk_div_func_t div_find_func,
                        cyhal_clock_t *hf_source, uint32_t *div);

/**
 * Attempts to set the clock to the specified frequency. This is similar to cyhal_clock_set_frequency, but it will also
 * make an attempt to set the frequency for HFCLK outputs. This is an enhancement beyond _cyhal_utils_set_clock_frequency
 * as this will also attemt to adjust the source clock as well as change the divider.
 *
 * @param[in] clock                 The HFCLK clock instance to set the frequency for.
 * @param[in] hz                    The maximum frequency, in hertz, to set the clock to. The clock will not exceed this
 *                                  value.
 * @param[in] tolerance             The allowed tolerance below the desired hz that is acceptable, use NULL if no
 *                                  tolerance check is required.
 */
cy_rslt_t _cyhal_utils_set_clock_frequency2(cyhal_clock_t *clock, uint32_t hz, const cyhal_clock_tolerance_t *tolerance);

/* Compatibility macros until other environments are updated to not use this anymore. */
#define CY_UTILS_GET_RESOURCE(pin, mappings)            _CYHAL_UTILS_GET_RESOURCE(pin, mappings)
#define cyhal_utils_get_resource(pin, mappings, count)  _cyhal_utils_get_resource(pin, mappings, count, NULL)
#define cyhal_utils_get_gpio_resource(pin)              _cyhal_utils_get_gpio_resource(pin)
#define CYHAL_SCB_BASE_ADDRESSES                        _CYHAL_SCB_BASE_ADDRESSES
#define CYHAL_TCPWM_DATA                                _CYHAL_TCPWM_DATA

#endif /* defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B) */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_impl_utils */
/** \} group_hal_impl */
/** \endcond */
