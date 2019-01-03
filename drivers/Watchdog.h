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

#include "watchdog_api.h"

#include <cstdio>

#include "mbed_error.h"

#include "rtos/Mutex.h"

#include "rtos/Thread.h"

#define MAX_THREAD_WATCHDOG_SUPPORT 32

namespace mbed {



/** \addtogroup drivers */
/** A system timer that will reset the system in the case of system failures or
 *  malfunctions.
 *
 * Example:
 * @code
 *
 * Watchdog watchdog = Watchdog();
 * watchdog.register(tid, 2000);
 *
 * while (true) {
 *    watchdog.kick(tid);
 *
 *    // Application code
 * }
 * @endcode
 * @ingroup drivers
 */
class Watchdog {
public:
    Watchdog() {}

public:



        /** Register to watchdog timer with specified parameters,
         * This API is only to register to watchdog and Watchdog automatically starts
         * if atleast one user registered.
         *
         *  @param timeout      Timeout of the watchdog in milliseconds
         *
         *  @param tid          Thread id
         *
         *  @return status MBED_SUCCESS if register to watchdog timer was succeeded
         *                 successfully. MBED_ERROR_INVALID_ARGUMENT if one of the input
         *                 parameters is out of range for the current platform.
         *                 MBED_ERROR_UNSUPPORTED if one of the enabled input
         *                 parameters is not supported by the current platform.
         */
    mbed_error_status_t wd_register(const osThreadId_t tid, const uint32_t timeout);

    /** Unregister from watchdog with specified parameters
     *
     *  @param tid          Thread id
     *
     *  @return status MBED_SUCCESS if register to watchdog timer was succeeded
     *                 successfully. MBED_ERROR_INVALID_ARGUMENT if tid is not registered
     *                 and try to do unregister
     */
    mbed_error_status_t wd_unregister(const osThreadId_t tid);



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
     * @return Returns WATCHDOG_STATUS_OK if the watchdog timer was successfully
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
protected :
    /** Start an independent watchdog timer with specified parameters
     *
     *  @param timeout      Timeout of the watchdog in milliseconds
     *
     *  @return status WATCHDOG_STATUS_OK if the watchdog timer was started
     *                 successfully. WATCHDOG_INVALID_ARGUMENT if one of the input
     *                 parameters is out of range for the current platform.
     *                 WATCHDOG_NOT_SUPPORTED if one of the enabled input
     *                 parameters is not supported by the current platform.
     */
    watchdog_status_t start(const uint32_t timeout);

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
    mbed_error_status_t stop();

private:
    static rtos::Mutex _ThreadSafeLockMutex;//Thread safe mutex for concurrent access protection

    typedef struct
    {
       osThreadId_t tid;
       uint32_t bit_idx;
    }watchdog_usr_info ;

    static watchdog_usr_info _usr_info[MAX_THREAD_WATCHDOG_SUPPORT];//Dats structure used to store the tid and their respective bit position of "_bitmask"

    static uint32_t _bitmask; //Every bit is used to denote the registered threads/user
    static uint32_t _kick_bitmask;//Every bit is used to denote the registered threads/user called kick(tid)
    static uint32_t _is_initialized;//represents the watchdog initialized/un-initialized
    static uint32_t _re_initialize;//represents the watchdog reinitizliation

};

} // namespace mbed

#endif // DEVICE_WATCHDOG
#endif // MBED_WATCHDOG_H
