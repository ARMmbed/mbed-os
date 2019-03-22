/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef MBED_WATCHDOG_MGR_H
#define MBED_WATCHDOG_MGR_H

#ifdef DEVICE_WATCHDOG

#include "watchdog_api.h"
#include "mbed_error.h"
#include "platform/Callback.h"
#include "platform/mbed_critical.h"
#include "platform/SingletonPtr.h"
#include "LowPowerTicker.h"
#include "Watchdog.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup platform */
/** A system timer that will reset the system in the case of system failures or
 *  malfunctions.
 *
 * Example:
 * @code
 *
 * mbed_wdog_manager_start();
 *
 * while (true) {
 *    wait(0.3);
 *
 * }
 * @endcode
 * @ingroup platform
 */

/** Start an independent watchdog timer
 *
 *
 *  @return status true if the watchdog timer was started
 *                 successfully. assert if one of the input parameters is out of range for the current platform.
 *                 false if watchdog timer was not started
 */
bool mbed_wdog_manager_start();

/** Stops the watchdog timer
 *
 * Calling this function will attempt to disable any currently running
 * watchdog timers if supported by the current platform.
 *
 * @return Returns true if the watchdog timer was successfully
 *         stopped, Returns false if the watchdog cannot be disabled
 *         on the current platform or if the timer was never started.
 */
bool mbed_wdog_manager_stop();


/** Get the watchdog timer refresh value
 *
 * This function returns the refresh timeout of the watchdog timer.
 *
 * @return Reload value for the watchdog timer in milliseconds.
 */
uint32_t mbed_wdog_manager_get_timeout();


/** Get the maximum refresh value for the current platform in milliseconds
 *
 * @return Maximum refresh value supported by the watchdog for the current
 *         platform in milliseconds
 */
uint32_t mbed_wdog_manager_get_max_timeout();

#ifdef __cplusplus
}
#endif
#endif // DEVICE_WATCHDOG
#endif // MBED_WATCHDOG_H
