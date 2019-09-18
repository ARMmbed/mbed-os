/***************************************************************************//**
* \file cyhal_timer.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Timer/Counter.
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
* \addtogroup group_hal_timer TIMER (Timer/Counter)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress Timer.
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

/** Timer directions */
typedef enum
{
    CYHAL_TIMER_DIR_UP, //!< Counts up
    CYHAL_TIMER_DIR_DOWN, //!< Counts down
    CYHAL_TIMER_DIR_UP_DOWN, //!< Counts up and down, terminal count occurs on both overflow and underflow.
} cyhal_timer_direction_t;

/** Timer/counter interrupt triggers */
typedef enum {
    CYHAL_TIMER_IRQ_NONE            =  0,
    CYHAL_TIMER_IRQ_TERMINAL_COUNT  =  1 << 0,
    CYHAL_TIMER_IRQ_CAPTURE_COMPARE =  1 << 1,
    CYHAL_TIMER_IRQ_ALL             = (1 << 2) - 1,
} cyhal_timer_event_t;

/** @brief Describes the current configuration of a timer/counter */
typedef struct
{
    /**
      * Whether the timer is set to continously run.
      * If true, the timer will run forever.
      * Otherwise, the timer will run once and stop (one shot).
      */
    bool is_continuous; //!< Whether the timer/counter operates continuous (true) or one shot (false)
    cyhal_timer_direction_t direction; //!< Direction the timer/counter is running
    bool is_compare; //!< Is it in compare (true) or capture (false) mode
    uint32_t period; //!< Timer/counter period
    uint32_t compare_value; //!< Timer/counter comparison value
    uint32_t value; //!< Current value of the timer/counter
} cyhal_timer_cfg_t;

/** Handler for timer events */
typedef void(*cyhal_timer_event_callback_t)(void *callback_arg, cyhal_timer_event_t event);

/** Bad argument. eg: null pointer */
#define CYHAL_TIMER_RSLT_ERR_BAD_ARGUMENT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_TIMER, 0))
/** Failed to initialize Timer clock */
#define CYHAL_TIMER_RSLT_ERR_CLOCK_INIT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_TIMER, 1))
/** Failed to initialize Timer */
#define CYHAL_TIMER_RSLT_ERR_INIT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_TIMER, 2))
/** Cannot change the timer frequency when a shared clock divider is in use */
#define CYHAL_TIMER_RSLT_ERR_SHARED_CLOCK (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_TIMER, 3))

/** Default timer frequency, used when an existing clock divider is not provided to init */
#define CYHAL_TIMER_DEFAULT_FREQ (1000000u)

/** Initialize the timer/counter peripheral and configure the pin.
 *
 * @param[out] obj The timer/counter object to initialize
 * @param[in]  pin optional - The timer/counter compare/capture pin to initialize
 * @param[in]  clk optional - The shared clock to use, if not provided a new clock will be allocated
 *                  and the timer frequency will be set to CYHAL_TIMER_DEFAULT_FREQ
 * @return The status of the init request
 */
cy_rslt_t cyhal_timer_init(cyhal_timer_t *obj, cyhal_gpio_t pin, const cyhal_clock_divider_t *clk);

/** Deinitialize the timer/counter object
 *
 * @param[in,out] obj The timer/counter object
 */
void cyhal_timer_free(cyhal_timer_t *obj);

/** Updates the configuration of the timer/counter object
 *
 * @param[in] obj  The timer/counter object
 * @param[in] cfg  The configuration of the timer/counter
 * @return The status of the configure request
 */
cy_rslt_t cyhal_timer_configure(cyhal_timer_t *obj, const cyhal_timer_cfg_t *cfg);

/** Configures the timer frequency. This is not valid to call if a non-null clock divider 
 *  was provided to cyhal_timer_init
 *
 * @param[in] obj  The timer/counter object
 * @param[in] hz   The frequency rate in Hz
 * @return The status of the set_frequency request
 */
cy_rslt_t cyhal_timer_set_frequency(cyhal_timer_t *obj, uint32_t hz);

/** Starts the timer/counter with the pre-set configuration.
 *
 * @param[in] obj     The timer/counter object
 * @return The status of the start request
 */
cy_rslt_t cyhal_timer_start(cyhal_timer_t *obj);

/** Stops the timer/counter.
 *
 * @param[in] obj     The timer/counter object
 * @return The status of the stop request
 */
cy_rslt_t cyhal_timer_stop(cyhal_timer_t *obj);

/** The timer/counter callback handler registration
 *
 * @param[in] obj          The timer/counter object
 * @param[in] callback     The callback handler which will be invoked when the event occurs
 * @param[in] callback_arg Generic argument that will be provided to the callback when called
 */
void cyhal_timer_register_callback(cyhal_timer_t *obj, cyhal_timer_event_callback_t callback, void *callback_arg);

/** Configure timer/counter event enablement.
 *
 * @param[in] obj           The timer/counter object
 * @param[in] event         The timer/counter event type
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on interrupts, False to turn off
 */
void cyhal_timer_enable_event(cyhal_timer_t *obj, cyhal_timer_event_t event, uint8_t intrPriority, bool enable);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_TIMER_IMPL_HEADER
#include CYHAL_TIMER_IMPL_HEADER
#endif /* CYHAL_TIMER_IMPL_HEADER */

/** \} group_hal_timer */
