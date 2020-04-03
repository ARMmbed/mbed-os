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
#include "drivers/Timer.h"
#include "drivers/LowPowerTimer.h"
#include "hal/ticker_api.h"
#include "hal/us_ticker_api.h"
#include "hal/lp_ticker_api.h"
#include "platform/CriticalSectionLock.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_power_mgmt.h"

using std::milli;
using std::micro;
using namespace std::chrono;

namespace mbed {

TimerBase::TimerBase(const ticker_data_t *data) : TimerBase(data, !data->interface->runs_in_deep_sleep)
{
}

TimerBase::TimerBase(const ticker_data_t *data, bool lock_deepsleep) : _ticker_data(data), _lock_deepsleep(lock_deepsleep)
{
    reset();
}

TimerBase::~TimerBase()
{
    if (_running) {
        if (_lock_deepsleep) {
            sleep_manager_unlock_deep_sleep();
        }
    }
}

void TimerBase::start()
{
    CriticalSectionLock lock;
    if (!_running) {
        if (_lock_deepsleep) {
            sleep_manager_lock_deep_sleep();
        }
        _start = _ticker_data.now();
        _running = true;
    }
}

void TimerBase::stop()
{
    CriticalSectionLock lock;
    _time += slicetime();
    if (_running) {
        if (_lock_deepsleep) {
            sleep_manager_unlock_deep_sleep();
        }
    }
    _running = false;
}

int TimerBase::read_us() const
{
    return duration<int, micro>(elapsed_time()).count();
}

float TimerBase::read() const
{
    return duration<float>(elapsed_time()).count();
}

int TimerBase::read_ms() const
{
    return duration_cast<duration<int, milli>>(elapsed_time()).count();
}

us_timestamp_t TimerBase::read_high_resolution_us() const
{
    return duration_cast<duration<us_timestamp_t, micro>>(elapsed_time()).count();
}

microseconds TimerBase::elapsed_time() const
{
    CriticalSectionLock lock;
    return _time + slicetime();
}

microseconds TimerBase::slicetime() const
{
    CriticalSectionLock lock;
    microseconds ret;
    if (_running) {
        ret = _ticker_data.now() - _start;
    }
    return ret;
}

void TimerBase::reset()
{
    CriticalSectionLock lock;
    _start = _ticker_data.now();
    _time = 0s;
}

TimerBase::operator float() const
{
    return duration<float>(elapsed_time()).count();
}

Timer::Timer() : TimerBase(get_us_ticker_data(), true)
{
}

#if DEVICE_LPTICKER
LowPowerTimer::LowPowerTimer() : TimerBase(get_lp_ticker_data(), false)
{
}
#endif

} // namespace mbed
