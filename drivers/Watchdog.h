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
 *  malfunctions. There is only one instance in the system.
 *
 * Example:
 * @code
 *
 * Watchdog &watchdog = Watchdog::get_instance();
 * watchdog.start();
 *
 * while (true) {
 *    // Application code
 * }
 * @endcode
 *
 * @note Synchronization level: Interrupt safe
 * @ingroup drivers
 */
class Watchdog : private NonCopyable<Watchdog>  {
public:
    static const uint32_t watchdog_timeout = MBED_CONF_TARGET_WATCHDOG_TIMEOUT / 2;

    /** As Watchdog might not stop ever, there is just one instance - we use single instance.
      * This ensures we keep Watchdog alive. To operate watchdog, use start/stop methods.
      */
    static Watchdog &get_instance()
    {
        // Use this implementation of singleton (Meyer's) rather than the one that allocates
        // the instance on the heap because it ensures destruction at program end (preventing warnings
        // from memory checking tools, such as valgrind).
        static Watchdog instance;
        return instance;
    }

    /** Start the watchdog timer
     *
     * If watchdog is already running, only callback is being updated (timeout can't be set to watchdog neither ticker).
     *
     *  @param func Callback to be invoked after timeout
     *  @param timeout Ticker timeout to be kicking the watchdog
     *
     *  @return status true if the watchdog timer was started
     *                 successfully. assert if one of the input parameters is out of range for the current platform.
     *                 false if watchdog timer was not started
     */
    bool start(Callback<void(uint32_t)> func = NULL, uint32_t timeout = watchdog_timeout);

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

    /** Kick watchdog
     *
     * This method is useful to control kicking by application
     */
    void kick();

private:
    Watchdog();
    ~Watchdog();

    /** Ticker invokes this handler when it timeouts - kicking watchdog periodically 
     */
    void timeout_handler();

    bool _running;
    Callback<void(uint32_t)> _callback;
    us_timestamp_t _ticker_timeout;

#if DEVICE_LPTICKER
    /** Create singleton instance of LowPowerTicker for watchdog periodic call back of kick.
     */
    SingletonPtr<LowPowerTicker> _ticker;
#else
    /** Create singleton instance of Ticker for watchdog periodic call back of kick.
     */
    SingletonPtr<Ticker> _ticker;
#endif
};

} // namespace mbed

#endif // DEVICE_WATCHDOG
#endif // MBED_WATCHDOG_H
