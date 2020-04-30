/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#include "drivers/LowPowerTicker.h"

#include "hal/us_ticker_api.h"
#include "hal/lp_ticker_api.h"
#include "platform/CriticalSectionLock.h"
#include "platform/mbed_power_mgmt.h"

using namespace std::chrono;

namespace mbed {

TickerBase::TickerBase(const ticker_data_t *data) : TickerBase(data, !data->interface->runs_in_deep_sleep)
{
}

// When low power ticker is in use, then do not disable deep sleep.
TickerBase::TickerBase(const ticker_data_t *data, bool lock_deepsleep) : TimerEvent(data),  _lock_deepsleep(lock_deepsleep)
{
}

void TickerBase::detach()
{
    CriticalSectionLock lock;
    remove();
    // unlocked only if we were attached (we locked it) and this is not low power ticker
    if (_function && _lock_deepsleep) {
        sleep_manager_unlock_deep_sleep();
    }

    _function = 0;
}

void TickerBase::setup(microseconds t)
{
    remove();
    _delay = t;
    insert_absolute(_ticker_data.now() + t);
}

void TickerBase::setup_absolute(TickerDataClock::time_point t)
{
    remove();
    insert_absolute(t);
}

void TickerBase::handler()
{
    insert_absolute(get_time_point(event) + _delay);
    if (_function) {
        _function();
    }
}

void TickerBase::attach(Callback<void()> func, microseconds t)
{
    CriticalSectionLock lock;
    // lock only for the initial callback setup and this is not low power ticker
    if (!_function && _lock_deepsleep) {
        sleep_manager_lock_deep_sleep();
    }
    _function = func;
    setup(t);
}

void TickerBase::attach_us(Callback<void()> func, us_timestamp_t t)
{
    CriticalSectionLock lock;
    // lock only for the initial callback setup and this is not low power ticker
    if (!_function && _lock_deepsleep) {
        sleep_manager_lock_deep_sleep();
    }
    _function = func;
    setup(microseconds{t});
}

void TickerBase::attach_absolute(Callback<void()> func, TickerDataClock::time_point abs_time)
{
    CriticalSectionLock lock;
    // lock only for the initial callback setup and this is not low power ticker
    if (!_function && _lock_deepsleep) {
        sleep_manager_lock_deep_sleep();
    }
    _function = func;
    setup_absolute(abs_time);
}

Ticker::Ticker() : TickerBase(get_us_ticker_data(), true)
{
}

#if DEVICE_LPTICKER
LowPowerTicker::LowPowerTicker() : TickerBase(get_lp_ticker_data(), false)
{
}
#endif

} // namespace mbed
