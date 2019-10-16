/***************************************************************************//**
* \file cyhal_rtc.h
*
* \brief
* Provides a high level interface for interacting with the Real Time Clock on 
* Cypress devices.  This interface abstracts out the chip specific details. 
* If any chip specific functionality is necessary, or performance is critical 
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
* \addtogroup group_hal_rtc RTC (Real-Time Clock)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress RTC.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"
#include "cyhal_modules.h"

/** RTC not initialized */
#define CY_RSLT_RTC_NOT_INITIALIZED CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_RTC, 0)

#if defined(__cplusplus)
extern "C" {
#endif

/** RTC interrupt triggers */
typedef enum {
    CYHAL_RTC_ALARM,
} cyhal_rtc_event_t;

/** @brief Defines which fields should be active for the alarm. */
typedef struct
{
    uint8_t en_sec : 1; /** !< Enable match of seconds */
    uint8_t en_min : 1; /** !< Enable match of minutes */
    uint8_t en_hour : 1; /** !< Enable match of hours */
    uint8_t en_day : 1; /** !< Enable match of day of week */
    uint8_t en_date : 1; /** !< Enable match of date in month */
    uint8_t en_month : 1; /** !< Enable match of month */
} cyhal_alarm_active_t;

/** Handler for RTC events */
typedef void (*cyhal_rtc_event_callback_t)(void *callback_arg, cyhal_rtc_event_t event);

/** Initialize the RTC peripheral
 *
 * Powerup the RTC in preparation for access. This function must be called
 * before any other RTC functions are called. This does not change the state
 * of the RTC. It just enables access to it.
 * NOTE: Before calling this, make sure all necessary System Clocks are setup
 * correctly. Generally this means making sure the RTC has access to a Crystal
 * for optimal accuracy and operation in low power.
 *
 * @param[out] obj RTC object
 * @return The status of the init request
 */
cy_rslt_t cyhal_rtc_init(cyhal_rtc_t *obj);

/** Deinitialize RTC
 *
 * Frees resources associated with the RTC and disables CPU access. This 
 * only affects the CPU domain and not the time keeping logic.
 * After this function is called no other RTC functions should be called
 * except for rtc_init.
 *
 * @param[in,out] obj RTC object
 */
void cyhal_rtc_free(cyhal_rtc_t *obj);

/** Check if the RTC has the time set and is counting
 *
 * @param[in] obj RTC object
 * @return Whether the RTC is enabled or not
 */
bool cyhal_rtc_is_enabled(cyhal_rtc_t *obj);

/** Get the current time from the RTC peripheral
 *
 * @param[in]  obj RTC object
 * @param[out] time The current time (see: https://en.cppreference.com/w/cpp/chrono/c/tm)
 * @return The status of the read request
 */
cy_rslt_t cyhal_rtc_read(cyhal_rtc_t *obj, struct tm *time);

/** Write the current time in seconds to the RTC peripheral
 *
 * @param[in] obj  RTC object
 * @param[in] time The time to be set (see: https://en.cppreference.com/w/cpp/chrono/c/tm)
 * @return The status of the write request
 */
cy_rslt_t cyhal_rtc_write(cyhal_rtc_t *obj, const struct tm *time);

/** Set an alarm for the specified time in seconds to the RTC peripheral
 *
 * @param[in] obj    RTC object
 * @param[in] time   The alarm time to be set (see: https://en.cppreference.com/w/cpp/chrono/c/tm)
 * @param[in] active The set of fields that are checked to trigger the alarm
 * @return The status of the set_alarm request
 */
cy_rslt_t cyhal_rtc_set_alarm(cyhal_rtc_t *obj, const struct tm *time, cyhal_alarm_active_t active);

/** The RTC event callback handler registration
 *
 * @param[in] obj          The RTC object
 * @param[in] callback     The callback handler which will be invoked when the alarm fires
 * @param[in] callback_arg Generic argument that will be provided to the callback when called
 */
void cyhal_rtc_register_callback(cyhal_rtc_t *obj, cyhal_rtc_event_callback_t callback, void *callback_arg);

/** Configure RTC event enablement.
 *
 * @param[in] obj           The RTC object
 * @param[in] event         The RTC event type
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on interrupts, False to turn off
 */
void cyhal_rtc_enable_event(cyhal_rtc_t *obj, cyhal_rtc_event_t event, uint8_t intrPriority, bool enable);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_RTC_IMPL_HEADER
#include CYHAL_RTC_IMPL_HEADER
#endif /* CYHAL_RTC_IMPL_HEADER */

/** \} group_hal_rtc */
