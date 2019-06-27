/*
 * Copyright (c) 2018 Arm Limited and affiliates.
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

#ifndef MBED_WATCHDOG_H
#define MBED_WATCHDOG_H

#ifdef DEVICE_WATCHDOG

#include "mbed_error.h"
#include "mbed_assert.h"
#include "platform/mbed_critical.h"
#include "watchdog_api.h"
#include "platform/NonCopyable.h"
#include "platform/SingletonPtr.h"
#include "drivers/LowPowerTicker.h"
#include <cstdio>

namespace mbed {

/** \addtogroup drivers */
/** Hardware system timer that will reset the system in the case of system failures or
 *  malfunctions.
 *
 * Example:
 * @code
 *
 * Watchdog watchdog();
 * watchdog.start();
 *
 * while (true) {
 *    // Application code
 * }
 * @endcode
 * @ingroup drivers
 */
class Watchdog : private NonCopyable<Watchdog>  {
public:

    /** Constructor configured with timeout
     *
     */
    Watchdog();
    ~Watchdog();
public:

    /** Start the watchdog timer
     *
     *
     *  @return status true if the watchdog timer was started
     *                 successfully. assert if one of the input parameters is out of range for the current platform.
     *                 false if watchdog timer was not started
     */
    bool start();

    /** Stops the watchdog timer
     *
     * Calling this function will attempt to disable any currently running
     * watchdog timers if supported by the current platform.
     *
     * @return Returns true if the watchdog timer was successfully
     *         stopped, Returns false if the watchdog cannot be disabled
     *         on the current platform or if the timer was never started.
     */
    bool stop();

    /** Get the watchdog timer refresh value
     *
     * This function returns the refresh timeout of the watchdog timer.
     *
     * @return Reload value for the watchdog timer in milliseconds.
     */
    uint32_t get_timeout() const;

    /** Get the maximum refresh value for the current platform in milliseconds
     *
     * @return Maximum refresh value supported by the watchdog for the current
     *         platform in milliseconds
     */
    uint32_t get_max_timeout() const;

    /** Check if watchdog is already running
     *
     * @return Maximum refresh value supported by the watchdog for the current
     *         platform in milliseconds
     */
    bool is_running() const;

private:
    static void kick();
    static uint32_t _elapsed_ms;
    static bool _running;
#if DEVICE_LPTICKER
    /** Create singleton instance of LowPowerTicker for watchdog periodic call back of kick.
     */
    static SingletonPtr<LowPowerTicker> _ticker;
#else
    /** Create singleton instance of Ticker for watchdog periodic call back of kick.
     */
    static SingletonPtr<Ticker> _ticker;
#endif
};

} // namespace mbed

#endif // DEVICE_WATCHDOG
#endif // MBED_WATCHDOG_H
