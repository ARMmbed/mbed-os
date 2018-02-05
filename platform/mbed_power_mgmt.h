/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_power_mgmt Power management functions
 * @{
 */

/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#ifndef MBED_POWER_MGMT_H
#define MBED_POWER_MGMT_H

#include "sleep_api.h"
#include "mbed_toolchain.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Sleep manager API
 * The sleep manager provides API to automatically select sleep mode.
 *
 * There are two sleep modes:
 * - sleep
 * - deepsleep
 *
 * Use locking/unlocking deepsleep for drivers that depend on features that
 * are not allowed (=disabled) during the deepsleep. For instance, high frequency
 * clocks.
 *
 * Example:
 * @code
 *
 * void driver::handler()
 * {
 *     if (_sensor.get_event()) {
 *         // any event - we are finished, unlock the deepsleep
 *         sleep_manager_unlock_deep_sleep();
 *         _callback();
 *     }
 * }
 *
 * int driver::measure(event_t event, callback_t& callback)
 * {
 *      _callback = callback;
 *      sleep_manager_lock_deep_sleep();
 *      // start async transaction, we are waiting for an event
 *      return _sensor.start(event, callback);
 * }
 * @endcode
 */
#ifdef SLEEP_PROFILING_ENABLED

#define sleep_manager_lock_deep_sleep()           \
    do                                            \
    {                                             \
        sleep_manager_lock_deep_sleep_internal(); \
        sleep_tracker_lock(__FILE__, __LINE__);   \
    } while (0);

#define sleep_manager_unlock_deep_sleep()           \
    do                                              \
    {                                               \
        sleep_manager_unlock_deep_sleep_internal(); \
        sleep_tracker_unlock(__FILE__, __LINE__);   \
    } while (0);

void sleep_tracker_lock(const char *const filename, int line);
void sleep_tracker_unlock(const char *const filename, int line);

#else

#define sleep_manager_lock_deep_sleep() \
    sleep_manager_lock_deep_sleep_internal()

#define sleep_manager_unlock_deep_sleep() \
    sleep_manager_lock_deep_sleep_internal()

#endif // SLEEP_PROFILING_ENABLED

/** Lock the deep sleep mode
 *
 * This locks the automatic deep mode selection.
 * sleep_manager_sleep_auto() will ignore deepsleep mode if
 * this function is invoked at least once (the internal counter is non-zero)
 *
 * Use this locking mechanism for interrupt driven API that are
 * running in the background and deepsleep could affect their functionality
 *
 * The lock is a counter, can be locked up to USHRT_MAX
 * This function is IRQ and thread safe
 */
void sleep_manager_lock_deep_sleep_internal(void);

/** Unlock the deep sleep mode
 *
 * Use unlocking in pair with sleep_manager_lock_deep_sleep().
 *
 * The lock is a counter, should be equally unlocked as locked
 * This function is IRQ and thread safe
 */
void sleep_manager_unlock_deep_sleep_internal(void);

/** Get the status of deep sleep allowance for a target
 *
 * @return true if a target can go to deepsleep, false otherwise
 */
bool sleep_manager_can_deep_sleep(void);
#ifdef __cplusplus
}
#endif

#endif

/** @}*/
/** @}*/
