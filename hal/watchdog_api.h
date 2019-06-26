/** \addtogroup hal */
/** @{*/

/*
 * Copyright (c) 2018-2019 Arm Limited and affiliates.
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
 */

#ifndef MBED_WATCHDOG_API_H
#define MBED_WATCHDOG_API_H

#if DEVICE_WATCHDOG

#include <stdbool.h>
#include <stdint.h>

/**
 * \defgroup hal_watchdog Watchdog HAL API
 * Low-level interface to the Independent Watchdog Timer of a target.
 *
 * This module provides platform independent access to the system watchdog timer
 * which is an embedded peripheral that will reset the system in the case of
 * system failures or malfunctions.
 *
 * The watchdog timer initializes a system timer with a time period specified in
 * the configuration. This timer counts down and triggers a system reset when it
 * wraps. To prevent the system reset the timer must be continually
 * kicked/refreshed by calling ::hal_watchdog_kick which will reset the countdown
 * to the user specified reset value.
 *
 * # Defined behavior
 * * Sleep and debug modes don't stop the watchdog timer from counting down.
 * * The function ::hal_watchdog_init is safe to call repeatedly. The
 * function's implementation must not do anything if ::hal_watchdog_init has
 * already initialized the hardware watchdog timer.
 * * Maximum supported timeout is `UINT32_MAX` milliseconds; minimum timeout
 * is 1 millisecond.
 * * The watchdog should trigger at or after the timeout value.
 * * The watchdog should trigger before twice the timeout value.
 *
 * # Undefined behavior
 * * Calling any function other than ::hal_watchdog_init or
 * ::hal_watchdog_get_platform_features before you have initialized the watchdog.
 *
 * # Notes
 * * A software reset may not stop the watchdog timer; the behavior is platform specific.
 *
 * @{
 */

/**
 * \defgroup hal_watchdog_tests Watchdog HAL tests
 * Greentea tests for the Watchdog HAL.
 *
 * To run the Watchdog HAL tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-watchdog*
 *
 */

/** Watchdog configuration.
 */
typedef struct {
    /**
    * Refresh value for the watchdog in milliseconds. The maximum value of this
    * setting is platform dependent, to find the maximum value for the current
    * platform call hal_watchdog_get_features() and check the timeout value
    * member. The minimum valid value for this setting is 1. Attempting to
    * initialize the watchdog with a timeout of 0 ms returns
    * WATCHDOG_STATUS_INVALID_ARGUMENT.
    */
    uint32_t timeout_ms;
} watchdog_config_t;

/** Watchdog features.
 */
typedef struct {
    /**
     * Maximum timeout value for the watchdog in milliseconds.
     */
    uint32_t max_timeout;
    /**
     * You can update the watchdog configuration after the watchdog has started.
     */
    bool update_config;
    /**
     * You can stop the watchdog after it starts without a reset.
     */
    bool disable_watchdog;
} watchdog_features_t;


/** Status of a watchdog operation.
*/
typedef enum {
    WATCHDOG_STATUS_OK,                 /**< Operation successful. **/
    WATCHDOG_STATUS_NOT_SUPPORTED,      /**< Operation not supported. **/
    WATCHDOG_STATUS_INVALID_ARGUMENT    /**< Invalid argument. **/
} watchdog_status_t;

#ifdef __cplusplus
extern "C" {
#endif

/** Initialize and start a watchdog timer with the given configuration.
 *
 * If the watchdog timer is configured and starts successfully, this
 * function returns ::WATCHDOG_STATUS_OK.
 *
 * If the timeout specified is outside the range supported by the platform,
 * it returns ::WATCHDOG_STATUS_INVALID_ARGUMENT.
 *
 * @param[in]  config   Configuration settings for the watchdog timer
 *
 * @return ::WATCHDOG_STATUS_OK if the watchdog is configured correctly and
 *         has started. Otherwise a status indicating the fault.
 */
watchdog_status_t hal_watchdog_init(const watchdog_config_t *config);

/** Refreshes the watchdog timer.
 *
 * Call this function periodically before the watchdog times out.
 * Otherwise, the system resets.
 *
 * If a watchdog is not running, this function does nothing.
 */
void hal_watchdog_kick(void);

/** Stops the watchdog timer.
 *
 * Calling this function disables any running watchdog
 * timers if the current platform supports them.
 *
 * @return Returns ::WATCHDOG_STATUS_OK if the watchdog timer was succesfully
 *         stopped, or if the timer was never started. Returns
 *         ::WATCHDOG_STATUS_NOT_SUPPORTED if the watchdog cannot be disabled on
 *         the current platform.
 */
watchdog_status_t hal_watchdog_stop(void);

/** Get the watchdog timer refresh value.
 *
 * This function returns the configured refresh timeout of the watchdog timer.
 *
 * @return Reload value for the watchdog timer in milliseconds.
 */
uint32_t hal_watchdog_get_reload_value(void);

/** Get information on the current platforms supported watchdog functionality.
 *
 * @return watchdog_feature_t indicating supported watchdog features on the
 *         current platform
 */
watchdog_features_t hal_watchdog_get_platform_features(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // DEVICE_WATCHDOG

#endif // MBED_WATCHDOG_API_H

/** @}*/
