/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "drivers/Ticker.h"

#include "drivers/TimerEvent.h"
#include "platform/FunctionPointer.h"
#include "hal/ticker_api.h"
#include "platform/mbed_critical.h"

namespace mbed {

void Ticker::detach()
{
    core_util_critical_section_enter();
    remove();
    // unlocked only if we were attached (we locked it) and this is not low power ticker
    if (_function && _lock_deepsleep) {
        sleep_manager_unlock_deep_sleep();
    }

    _function = 0;
    core_util_critical_section_exit();
}

void Ticker::setup(us_timestamp_t t)
{
    core_util_critical_section_enter();
    remove();
    _delay = t;
    insert_absolute(_delay + ticker_read_us(_ticker_data));
    core_util_critical_section_exit();
}

void Ticker::handler()
{
    insert_absolute(event.timestamp + _delay);
    if (_function) {
        _function();
    }
}

void Ticker::attach_us(Callback<void()> func, us_timestamp_t t)
{
    core_util_critical_section_enter();
    // lock only for the initial callback setup and this is not low power ticker
    if (!_function && _lock_deepsleep) {
        sleep_manager_lock_deep_sleep();
    }
    _function = func;
    setup(t);
    core_util_critical_section_exit();
}

} // namespace mbed
