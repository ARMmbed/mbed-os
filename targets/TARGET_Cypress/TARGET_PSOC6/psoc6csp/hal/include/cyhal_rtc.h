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
* \addtogroup group_hal_rtc RTC (Real-Time Clock)
* \ingroup group_hal
* \{
* High level interface for interacting with the real-time clock (RTC).
*
* The real time clock provides tracking of the current time and date, as
* well as the ability to trigger a callback at a specific time in the future.
*
* If a suitable clock source is available, the RTC can continue timekeeping
* operations even when the device is in a low power operating mode. See the
* device datasheet for more details.
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
/** Bad argument */
#define CY_RSLT_RTC_BAD_ARGUMENT CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_RTC, 1)

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

/** Enumeration used to configure the DST format */
typedef enum
{
    CYHAL_RTC_DST_RELATIVE,        /**< Relative DST format */
    CYHAL_RTC_DST_FIXED            /**< Fixed DST format */
} cyhal_rtc_dst_format_t;

/**
* Day Light Savings Time (DST) structure for setting when to apply. It allows to
* set the DST time and date using a fixed or relative time format.
*/
typedef struct
{
    cyhal_rtc_dst_format_t format;   /**< DST format. See /ref cyhal_rtc_dst_format_t.
                                          Based on this value other structure elements
                                          should be filled or could be ignored */
    uint32_t hour;                   /**< Hour in 24hour format, range[0-23] */
    union
    {
        uint32_t dayOfMonth;         /**< Day of Month, range[1-31]. */
        struct /* format = CYHAL_RTC_DST_FIXED */
        {
            uint32_t dayOfWeek;      /**< Day of the week, starting on Sunday, range[0-6] */
            uint32_t weekOfMonth;    /**< Week of month, range[0-5]. Where 5 => Last week of month */
        };
    };
    uint32_t month;                  /**< Month value, range[1-12]. */
} cyhal_rtc_dst_t;

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

/** Set the start and end time for Day Light Savings
 *
 * @param[in] obj  RTC object
 * @param[in] start When Day Light Savings time should start
 * @param[in] stop When Day Light Savings time should end
 * @return The status of the set_dst request
 */
cy_rslt_t cyhal_rtc_set_dst(cyhal_rtc_t *obj, const cyhal_rtc_dst_t *start, const cyhal_rtc_dst_t *stop);

/** Checks to see if Day Light Savings Time is currently active. This should only be called after
 * \ref cyhal_rtc_set_dst().
 *
 * @param[in] obj  RTC object
 * @return Boolean indicating whether the current date/time is within the specified DST start/stop window.
 */
bool cyhal_rtc_is_dst(cyhal_rtc_t *obj);

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
