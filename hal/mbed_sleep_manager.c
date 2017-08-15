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

#include "mbed_sleep.h"
#include "mbed_critical.h"
#include "sleep_api.h"
#include "mbed_error.h"
#include <limits.h>

#if DEVICE_SLEEP

// deep sleep locking counter. A target is allowed to deep sleep if counter == 0
static uint16_t deep_sleep_lock = 0U;

void sleep_manager_lock_deep_sleep(void)
{
    core_util_critical_section_enter();
    if (deep_sleep_lock == USHRT_MAX) {
        core_util_critical_section_exit();
        error("Deep sleep lock would overflow (> USHRT_MAX)");
    }
    core_util_atomic_incr_u16(&deep_sleep_lock, 1);
    core_util_critical_section_exit();
}

void sleep_manager_unlock_deep_sleep(void)
{
    core_util_critical_section_enter();
    if (deep_sleep_lock == 0) {
        core_util_critical_section_exit();
        error("Deep sleep lock would underflow (< 0)");
    }
    core_util_atomic_decr_u16(&deep_sleep_lock, 1);
    core_util_critical_section_exit();
}

bool sleep_manager_can_deep_sleep(void)
{
    return deep_sleep_lock == 0 ? true : false;
}

void sleep_manager_sleep_auto(void)
{
    core_util_critical_section_enter();
// debug profile should keep debuggers attached, no deep sleep allowed
#ifdef MBED_DEBUG
    hal_sleep();
#else
    if (sleep_manager_can_deep_sleep()) {
        hal_deepsleep();
    } else {
        hal_sleep();
    }
#endif
    core_util_critical_section_exit();
}

#else

// locking is valid only if DEVICE_SLEEP is defined
// we provide empty implementation

void sleep_manager_lock_deep_sleep(void)
{

}

void sleep_manager_unlock_deep_sleep(void)
{

}

bool sleep_manager_can_deep_sleep(void)
{
    // no sleep implemented
    return false;
}

#endif
