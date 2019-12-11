/***************************************************************************//**
* \file cyhal_lptimer.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Low-Power Timer.
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
* \addtogroup group_hal_lptimer LPTIMER (Low-Power Timer)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress LPTIMER.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** LPTIMER interrupt triggers */
typedef enum {
    CYHAL_LPTIMER_COMPARE_MATCH,
} cyhal_lptimer_event_t;

/** Handler for LPTIMER interrupts */
typedef void (*cyhal_lptimer_event_callback_t)(void *callback_arg, cyhal_lptimer_event_t event);

/** Initialize the LPTIMER
 *
 * Initialize or re-initialize the LPTIMER. This resets all the
 * clocking and prescaler registers, along with disabling
 * the compare interrupt. Refer to the BSP for the clock source
 * for the LPTIMER.
 *
 * @param[out] obj The LPTIMER object
 * @return The status of the init request
 */
cy_rslt_t cyhal_lptimer_init(cyhal_lptimer_t *obj);

/** Deinitialize the LPTIMER
 *
 * Powers down the LPTIMER.
 * After calling this function no other LPTIMER functions should be called except
 * cyhal_lptimer_init(). Calling any function other than init after freeing is
 * undefined.
 *
 * @param[inout] obj The LPTIMER object
 */
void cyhal_lptimer_free(cyhal_lptimer_t *obj);

/** Reload/Reset the Low-Power timer.
 *
 * @param[in] obj   The LPTIMER object
 * @return The status of the reload request
 */
cy_rslt_t cyhal_lptimer_reload(cyhal_lptimer_t *obj);

/** Set timeframe between interrupts
 * 
 * Configures the LPTIMER in free-running mode. Generates an interrupt on match.
 * This function is for initial configuration. For quick updates to the match
 * value, use cyhal_lptimer_set_time().
 * 
 * @param[in] obj   The LPTIMER object
 * @param[in] time  The time in ticks to be set
 *
 * @return The status of the set_time request
 */
cy_rslt_t cyhal_lptimer_set_time(cyhal_lptimer_t *obj, uint32_t time);

/** Update the match/compare value
 * 
 * Update the match value of an already configured LPTIMER set up
 * to generate an interrupt on match. Note that this function does not
 * reinitialize the counter or the associated peripheral initialization
 * sequence.
 * 
 * @param[in] obj   The LPTIMER object
 * @param[in] value The match value in ticks
 *
 * @return The status of the set_match request
 */
cy_rslt_t cyhal_lptimer_set_match(cyhal_lptimer_t *obj, uint32_t value);

/** Read the current tick
 *
 * If no rollover has occurred, the seconds passed since cyhal_lptimer_init() or cyhal_lptimer_set_time()
 * was called can be found by dividing the ticks returned by this function
 * by the frequency of the source clock (refer to the BSP for the clock source).
 *
 * @param[in] obj    The LPTIMER object
 * @return The timer's timer value in ticks
 */
uint32_t cyhal_lptimer_read(const cyhal_lptimer_t *obj);

/** The LPTIMER match event handler registration
 *
 * @param[in] obj          The LPTIMER object
 * @param[in] callback     The callback handler which will be invoked when the interrupt triggers
 * @param[in] callback_arg Generic argument that will be provided to the handler when called
 */
void cyhal_lptimer_register_callback(cyhal_lptimer_t *obj, cyhal_lptimer_event_callback_t callback, void *callback_arg);

/** Configure and Enable/Disable the LPTIMER events
 *
 * @param[in] obj           The LPTIMER object
 * @param[in] event         The LPTIMER event type
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on event, False to turn off
 */
void cyhal_lptimer_enable_event(cyhal_lptimer_t *obj, cyhal_lptimer_event_t event, uint8_t intrPriority, bool enable);

/** Manually trigger the LPTIMER interrupt.
 *
 * @param[in] obj      The LPTIMER object
 */
void cyhal_lptimer_irq_trigger(cyhal_lptimer_t *obj);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_LPTIMER_IMPL_HEADER
#include CYHAL_LPTIMER_IMPL_HEADER
#endif /* CYHAL_LPTIMER_IMPL_HEADER */

/** \} group_hal_lptimer */
