/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef MBED_DEEPSLEEPLOCK_H
#define MBED_DEEPSLEEPLOCK_H

#include <limits.h>
#include "platform/mbed_power_mgmt.h"
#include "platform/mbed_critical.h"

namespace mbed {

/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_DeepSleepLock DeepSleepLock functions
 * @{
 */

/** RAII object for disabling, then restoring the deep sleep mode
  * Usage:
  * @code
  *
  * void f() {
  *     // some code here
  *     {
  *         DeepSleepLock lock;
  *         // Code in this block will run with the deep sleep mode locked
  *     }
  *     // deep sleep mode will be restored to their previous state
  * }
  * @endcode
  */
class DeepSleepLock {
private:
    uint16_t _lock_count;

public:
    DeepSleepLock(): _lock_count(1)
    {
        sleep_manager_lock_deep_sleep();
    }

    ~DeepSleepLock()
    {
        if (_lock_count) {
            sleep_manager_unlock_deep_sleep();
        }
    }

    /** Mark the start of a locked deep sleep section
     */
    void lock()
    {
        uint16_t count = core_util_atomic_incr_u16(&_lock_count, 1);
        if (1 == count) {
            sleep_manager_lock_deep_sleep();
        }
        if (0 == count) {
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_OVERFLOW), "DeepSleepLock overflow (> USHRT_MAX)", count);
        }
    }

    /** Mark the end of a locked deep sleep section
     */
    void unlock()
    {
        uint16_t count = core_util_atomic_decr_u16(&_lock_count, 1);
        if (count == 0) {
            sleep_manager_unlock_deep_sleep();
        }
        if (count == USHRT_MAX) {
            core_util_critical_section_exit();
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_UNDERFLOW), "DeepSleepLock underflow (< 0)", count);
        }
    }
};

/**@}*/

/**@}*/


}

#endif
