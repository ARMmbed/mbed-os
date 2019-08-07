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

#include "platform/mbed_error.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_critical.h"
#include "hal/watchdog_api.h"
#include "platform/NonCopyable.h"
#include <cstdio>

namespace mbed {
/**
 * \defgroup drivers_Watchdog Watchdog class
 * \ingroup drivers-public-api
 * @{
 */

/** A hardware watchdog timer that resets the system in the case of system
 * failures or malfunctions. If you fail to refresh the Watchdog timer periodically,
 * it resets the system after a set period of time.
 *
 * There is only one instance of the Watchdog class in the system, which directly maps to the hardware.
 * Use Watchdog::get_instance to obtain a reference.
 *
 * Watchdog::start initializes a system timer with a time period specified in
 * @a timeout param. This timer counts down and triggers a system reset
 * when it wraps. To prevent the system reset, you must periodically kick or refresh
 * the timer by calling Watchdog::kick, which resets the countdown
 * to the initial value.
 *
 * Example:
 * @code
 * Watchdog &watchdog = Watchdog::get_instance();
 * watchdog.start(500);
 *
 * while (true) {
      // kick watchdog regularly within provided timeout
      watchdog.kick();
      // Application code
 * }
 * @endcode
 *
 * @note Synchronization level: Interrupt safe
 */
class Watchdog : private NonCopyable<Watchdog>  {
public:

    /** Get a reference to the single Watchdog instance in the system.
     *
     * @return A reference to the single Watchdog instance present in the system.
     */
    static Watchdog &get_instance()
    {
        // Use this implementation of singleton (Meyer's) rather than the one that allocates
        // the instance on the heap because it ensures destruction at program end (preventing warnings
        // from memory checking tools, such as valgrind).
        static Watchdog instance;
        return instance;
    }

    /** Start the Watchdog timer with the maximum timeout value available for
     * the target.
     *
     * @note The timeout is set to a value returned by Watchdog::get_max_timeout.
     *
     * If the Watchdog timer is already running, this function does nothing.
     *
     * @return true if the Watchdog timer was started successfully;
     *         false if the Watchdog timer was not started or if the Watchdog
     *         timer is already running.
     */
    bool start();

    /** Start the Watchdog timer.
     *
     * @note Asset that the timeout param is supported by the target
     * (0 < timeout <= Watchdog::get_max_timeout).
     *
     * @param timeout Watchdog timeout in milliseconds.
     *
     * @return true if the Watchdog timer was started successfully;
     *         false if Watchdog timer was not started or if the Watchdog
     *         timer is already running.
     */
    bool start(uint32_t timeout);

    /** Stop the Watchdog timer.
     *
     * Calling this function disables a running Watchdog
     * peripheral if the platform supports it.
     *
     * @return true if the Watchdog timer was successfully stopped;
     *         false if the Watchdog timer cannot be disabled on this platform
     *         or if the Watchdog timer has not been started.
     */
    bool stop();

    /** Get the Watchdog timer refresh value.
     *
     * This function returns the refresh timeout of the watchdog peripheral.
     *
     * @return Reload value for the Watchdog timer in milliseconds.
     */
    uint32_t get_timeout() const;

    /** Get the maximum Watchdog refresh value for this platform.
     *
     * @return Maximum reload value supported by the Watchdog timer for this
     *         platform in milliseconds.
     */
    uint32_t get_max_timeout() const;

    /** Check if the Watchdog timer is already running.
     *
     * @return true if the Watchdog timer is running and
     *         false otherwise.
     */
    bool is_running() const;

    /** Refresh the Watchdog timer.
     *
     * Call this function periodically before the Watchdog times out.
     * Otherwise, the system resets.
     *
     * If the Watchdog timer is not running, this function does nothing.
     */
    void kick();

private:
    Watchdog();
    ~Watchdog();

    bool _running;
};

/** @}*/

} // namespace mbed

#endif // DEVICE_WATCHDOG
#endif // MBED_WATCHDOG_H
