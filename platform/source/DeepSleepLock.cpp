/* mbed Microcontroller Library
 * Copyright (c) 2017-2019 ARM Limited
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

#include "platform/DeepSleepLock.h"

#include "platform/mbed_atomic.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_error.h"
#include "platform/mbed_power_mgmt.h"

namespace mbed {

DeepSleepLock::DeepSleepLock(): _lock_count(1)
{
    sleep_manager_lock_deep_sleep();
}

DeepSleepLock::~DeepSleepLock()
{
    if (_lock_count) {
        sleep_manager_unlock_deep_sleep();
    }
}

void DeepSleepLock::lock()
{
    uint16_t count = core_util_atomic_incr_u16(&_lock_count, 1);
    if (1 == count) {
        sleep_manager_lock_deep_sleep();
    }
    if (0 == count) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_OVERFLOW), "DeepSleepLock overflow (> USHRT_MAX)", count);
    }
}

void DeepSleepLock::unlock()
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

} // namespace mbed
