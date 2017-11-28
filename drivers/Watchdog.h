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

#ifndef MBED_WATCHDOG_H
#define MBED_WATCHDOG_H

#ifdef DEVICE_WATCHDOG

#include "watchdog_api.h"

#include <cstdio>


namespace mbed {
/** \addtogroup drivers */
/** @{*/
/** A system timer that will reset the system in the case of system failures or
 *  malfunctions.
 *
 * Example:
 * @code
 *
 * Watchdog watchdog = Watchdog();
 * watchdog.set_timeout(2000);
 * watchdog.start();
 *
 * while (true) {
 *    watchdog.kick();
 *
 *    // Application code
 * }
 * @endcode
 *
 */
class Watchdog
{
public:
    Watchdog() {}

public:
    /** Start an independent watchdog timer with specified parameters
     *
     *  @param timeout      Timeout of the watchdog in milliseconds
     *  @param enable_sleep Sets sleep mode behaviour. When enabled the watchdog
     *                      will continue to run in sleep mode. When disable the
     *                      watchdog will be paused. This feature is not
     *                      supported on all platforms.
     *
     *  @return status WATCHDOG_STATUS_OK if the watchdog timer was started
     *                 successfully. WATCHDOG_INVALID_ARGUMENT if one of the input
     *                 parameters is out of range for the current platform.
     *                 WATCHDOG_NOT_SUPPORTED if one of the enabled input
     *                 parameters is not supported by the current platform.
     */
    watchdog_status_t start(const uint32_t timeout, const bool enable_sleep = true);


    /** Start a windowed watchdog timer with specified parameters
     *
     *  @param timeout      Timeout of the watchdog in milliseconds
     *  @param window       Time period of the window of the watchdog
     *  @param enable_sleep Sets sleep mode behaviour. When enabled the watchdog
     *                      will continue to run in sleep mode. When disable the
     *                      watchdog will be paused. This feature is not
     *                      supported on all platforms.
     *
     *  @return status WATCHDOG_STATUS_OK if the watchdog timer was started
     *                 successfully. WATCHDOG_INVALID_ARGUMENT if one of the input
     *                 parameters is out of range for the current platform.
     *                 WATCHDOG_NOT_SUPPORTED if one of the enabled input
     *                 parameters is not supported by the current platform.
     */
    watchdog_status_t start(const uint32_t timeout, const uint32_t window, const bool enable_sleep = true);


    /** Refreshes the watchdog timer.
     *
     * This function should be called periodically before the watchdog times out.
     * Otherwise, the system is reset.
     *
     * If the watchdog timer is not currently running this function does nothing
     */
    void kick();


    /** Stops the watchdog timer
     *
     * Calling this function will attempt to disable any currently running
     * watchdog timers if supported by the current platform.
     *
     * @return Returns WATCHDOG_STATUS_OK if the watchdog timer was succesfully
     *         stopped, or if the timer was never started. Returns
     *         WATCHDOG_STATUS_NOT_SUPPORTED if the watchdog cannot be disabled
     *         on the current platform.
     */
    watchdog_status_t stop();


    /** Get the watchdog timer refresh value
     *
     * This function returns the refresh timeout of the watchdog timer.
     *
     * @return Reload value for the watchdog timer in milliseconds.
     */
    uint32_t reload_value() const;


    /** Get the maximum refresh value for the current platform in milliseconds
     *
     * @return Maximum refresh value supported by the watchdog for the current
     *         platform in milliseconds
     */
    static uint32_t max_timeout();
};

} // namespace mbed

/**@}*/
/**@}*/

#endif // DEVICE_WATCHDOG
#endif // MBED_WATCHDOG_H
