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

#ifndef MBED_WATCHDOG_H
#define MBED_WATCHDOG_H

#ifdef DEVICE_WATCHDOG

#include <cstdio>
#include "mbed_error.h"
#include "rtos/ThisThread.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_power_mgmt.h"
#include <string>
namespace mbed
{

/** \addtogroup drivers */
/** A system timer that will reset the system in the case of system failures or
 *  malfunctions.
 *
 * Example:
 * @code
 *
 * Watchdog watchdog = Watchdog(300,"Software Watchdog");
 * watchdog.start();
 *
 * while (true) {
 *    watchdog.kick();
 *
 *    // Application code
 * }
 * @endcode
 * @ingroup drivers
 */
class Watchdog
{
public:
    Watchdog() {}

    Watchdog(uint32_t timeout = 0,const char *str = NULL):_max_timeout(timeout),name(str),_current_count(0),_is_initialized(false),next(NULL) {}

    ~Watchdog();
public:

    /** Start an independent watchdog timer with specified parameters
     *
     * Assert for multiple calls of start
     */
    void start();

    /** Stops the watchdog timer
     *
     * Calling this function will attempt to disable any currently running
     * watchdog timers if supported by the current platform.
     *
     * Assert with out called start
     */
    void stop();

    /** Refreshes the watchdog timer.
     *
     * This function should be called periodically before the watchdog times out.
     * Otherwise, the system is reset.
     *
     * If the watchdog timer is not currently running this function does nothing
     */
    void kick();

    /** Get the watchdog timer refresh value
     *
     * This function should be called from mbed_watchdog_manager_kick to monitor all the
     * user/threads alive state.
     *
     * Otherwise, the system is reset.
     */
    void is_alive();
protected :

    /** add_to_list is used to store the registered user into List.
      * This API is only used to call from start.
      */
    void add_to_list();

    /** Remove from list is uesed to remove the entry from the list.
      * This API is only used to call from stop.
      *
      */
    void remove_from_list();
private:
    uint32_t _max_timeout; //_max_timeout initialized via constructor while creating instance of this class
    const char *name;//To store the details of user
    uint32_t _current_count;//this parameter is used to reset everytime threads/user calls kick
    bool _is_initialized;//To control start and stop functionality
    static Watchdog *first;//List to store the user/threads who called start
    Watchdog *next;
};

} // namespace mbed

#endif // DEVICE_WATCHDOG
#endif // MBED_WATCHDOG_H
