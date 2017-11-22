/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#if !(DEVICE_RESET_REASON)
  #error "Watchdog feature depends on reset reason API also being implemented"
#endif

#include <stdbool.h>
#include <stdint.h>

/** \file watchdog_api.h
 *
 * This module provides platform independent access to the system watchdog timer
 * which is an embedded peripheral that will reset the system in the case of
 * system failures or malfunctions.
 *
 * The watchdog timer initialises a system timer with a time period specified in
 * the configuration. This timer counts down and triggers a system reset when it
 * wraps. To prevent the system reset the timer must be continually
 * kicked/refreshed by calling hal_watchdog_kick which will reset the countdown
 * to the user specified reset value.
 *
 * The watchdog timer supports a second mode of operation called windowed mode.
 * When configured in this mode by setting enable_window to true, the watchdog
 * will enable a restriction on the kick. If the watchdog timer is kicked too
 * soon after it has last been refreshed a system reset occurs. The earliest
 * time in milliseconds the timer can be kicked without triggering a reset is
 * specified by window_ms.
 */

typedef struct
{
  /**
  * Refresh value for the watchdog in milliseconds. The maximum value of this
  * setting is platform dependent, to find the maximum value for the current
  * platform call hal_watchdog_get_max_timeout(void)
  */
  uint32_t timeout_ms;
  /**
   * Configures the watchdog for windowed mode of operation instead of running
   * the independent watchdog. In this mode a restriction is placed on the
   * time period in which the watchdog can be refreshed/kicked. If the
   * watchdog is kicked too soon after it has last been refreshed the system
   * will be reset. The period of time in which the reset will be triggered is
   * defined by window_ms. This value is false by default.
   */
  bool enable_window;
  /**
   * Specifies the time window for the watchdog window in milliseconds. If the
   * watchdog is configured to run in windowed mode kicking the watchdog less
   * than this many milliseconds after it has last been kicked will trigger a
   * system reset. This value must be less than timeout_ms.
   */
  uint32_t window_ms;
  /**
   * Configures the watchdog behaviour while the system is in sleep mode. When
   * this flag is enabled the watchdog timer runs normally while the system is
   * in sleep mode, when disabled the watchdog is paused during this time.
   */
  bool enable_sleep;
} watchdog_config_t;


typedef enum {
  WATCHDOG_STATUS_OK,
  WATCHDOG_STATUS_NOT_SUPPORTED,
  WATCHDOG_STATUS_INVALID_ARGUMENT
} watchdog_status_t;

#ifdef __cplusplus
    extern "C" {
#endif

/** Initialise and start a watchdog timer with the given configuration.
 *
 * If the watchdog timer is configured and started successfully this
 * function will return WATCHDOG_STATUS_OK.
 *
 * If the enable_window is set but windowed mode is not supported by the
 * platform the function will return WATCHDOG_STATUS_NOT_SUPPORTED.
 *
 * If the timeout specified is outside the range supported by the platform,
 * or if the window period is greater than the timeout period it will return
 * WATCHDOG_STATUS_INVALID_ARGUMENT.
 *
 * @param[in]  config   Configuration settings for the watchdog timer
 *
 * @return WATCHDOG_STATUS_OK if the watchdog is configured correctly and
 *         has started. Otherwise a status indicating the fault.
 */
watchdog_status_t hal_watchdog_init(const watchdog_config_t *config);

/** Refreshes the watchdog timer.
 *
 * This function should be called periodically before the watchdog times out.
 * Otherwise, the system is reset.
 *
 * If using the windowed operation mode this function must be called within the
 * configured timeout window. If the function is called before the window is
 * reached the system is reset.
 *
 * If a watchdog is not currently running this function does nothing
 */
void hal_watchdog_kick(void);

/** Stops the watchdog timer
 *
 * Calling this function will attempt to disable any currently running watchdog
 * timers if supported by the current platform
 *
 * @return Returns WATCHDOG_STATUS_OK if the watchdog timer was succesfully
 *         stopped, or if the timer was never started. Returns
 *         WATCHDOG_STATUS_NOT_SUPPORTED if the watchdog cannot be disabled on
 *         the current platform.
 */
watchdog_status_t hal_watchdog_stop(void);

/** Get the watchdog timer refresh value
 *
 * This function returns the configured refresh timeout of the watchdog timer.
 *
 * @return Reload value for the watchdog timer in milliseconds.
 */
uint32_t hal_watchdog_get_reload_value(void);

/** Checks if the last system reset was caused by a watchdog timer.
 *
 * @return True if last reset was triggered by a watchdog, False if not.
 */
bool hal_watchdog_caused_last_reset(void);

/** Get the maximum refresh value for the current platform in milliseconds
 *
 * @return Maximum refresh value supported by the watchdog for the current
 *         platform in milliseconds
 */
uint32_t hal_watchdog_get_max_timeout(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // DEVICE_WATCHDOG

#endif // MBED_WATCHDOG_API_H

/** @}*/
