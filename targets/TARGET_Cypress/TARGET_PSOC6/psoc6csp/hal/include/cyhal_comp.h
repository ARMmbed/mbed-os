/***************************************************************************//**
* \file cyhal_comp.h
*
* Provides a high level interface for interacting with the Cypress Comparator.
* This interface abstracts out the chip specific details.
* If any chip specific functionality is necessary, or performance is critical,
* the low level functions can be used directly.
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
* \addtogroup group_hal_comp COMP (Comparator)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress Comparator.
*
* \defgroup group_hal_comp_macros Macros
* \defgroup group_hal_comp_functions Functions
* \defgroup group_hal_comp_data_structures Data Structures
* \defgroup group_hal_comp_enums Enumerated Types
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
* \addtogroup group_hal_comp_enums
* \{
*/

/** Comparator interrupt triggers */
typedef enum {
    CYHAL_COMP_IRQ_NONE, //!< Interrupts disabled
    CYHAL_COMP_IRQ_RISE, //!< Rising edge
    CYHAL_COMP_IRQ_FALL,  //!< Falling edge
} cyhal_comp_irq_event_t;

/** Comparator power options */
typedef enum {
    CYHAL_COMP_POWER_OFF,    //!< Block is turned off
    CYHAL_COMP_POWER_LOW,    //!< Block runs in low power/speed mode
    CYHAL_COMP_POWER_MEDIUM, //!< Block runs in medium power/speed mode
    CYHAL_COMP_POWER_HIGH,   //!< Block runs in high power/speed mode
} cyhal_comp_power_t;

/** Comparator output modes */
typedef enum {
    CYHAL_COMP_OUTPUT_PULSE,  //!< Pulse output
    CYHAL_COMP_OUTPUT_DIRECT, //!< Level output directly
    CYHAL_COMP_OUTPUT_SYNC,   //!< Level output after synchronous edge detection
} cyhal_comp_output_t;

/** \} group_hal_comp_enums */


/**
* \addtogroup group_hal_comp_data_structures
* \{
*/

/** Configuration options for the comparator */
typedef struct
{
    cyhal_comp_power_t power;   //!< Power mode to operate in (0=off, 1=low, 2=medium, 3=high)
    cyhal_comp_output_t output; //!< Output configuration (0=pulse, 1=direct, 2=sync)
    bool hysteresis;         //!< Should this use hysteresis
    bool deepsleep;          //!< Does this need to operate in deepsleep
} cyhal_comp_config_t;

/** Handler for comparator interrupts */
typedef void (*cyhal_comp_irq_handler_t)(void *handler_arg, cyhal_comp_irq_event_t event);

/** \} group_hal_comp_data_structures */


/**
* \addtogroup group_hal_comp_functions
* \{
*/

/** Initialize the comparator peripheral.
 *
 * @param[out] obj  The comparator object
 * @param[in]  vinp The vplus pin
 * @param[in]  vinm The vminus pin
 * @param[in]  vout The vout pin
 * @return The status of the init request
 */
cy_rslt_t cyhal_comp_init(cyhal_comp_t *obj, cyhal_gpio_t vinp, cyhal_gpio_t vinm, cyhal_gpio_t vout);

/** Release the comparator peripheral.
 *
 * @param[in,out] obj The comparator object
 */
void cyhal_comp_free(cyhal_comp_t *obj);

/** Reconfigure the comparator object
 *
 * @param[in,out] obj  The comparator object
 * @param[in]     cfg  Configuration to apply to the comparator
 * @return The status of the power request
 */
cy_rslt_t cyhal_comp_configure(cyhal_comp_t *obj, const cyhal_comp_config_t cfg);

/** Gets the result of the comparator object
 *
 * @param[in]  obj    The comparator object
 * @return The comparator output state
 */
bool cyhal_comp_output(cyhal_comp_t *obj);

/** Register/clear an interrupt handler for the comparator toggle IRQ event
 *
 * @param[in] obj         The comparator object
 * @param[in] handler     The function to call when the specified event happens
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 */
void cyhal_comp_register_irq(cyhal_comp_t *obj, cyhal_comp_irq_handler_t handler, void *handler_arg);

/** Enable or Disable the comparator IRQ
 *
 * @param[in] obj    The comparator object
 * @param[in] event  The comparator IRQ event
 * @param[in] enable True to turn on interrupts, False to turn off
 */
void cyhal_cyhal_comp_irq_enable(cyhal_comp_t *obj, cyhal_comp_irq_event_t event, bool enable);

/** \} group_hal_comp_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_comp */
