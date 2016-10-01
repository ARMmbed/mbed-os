/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "hal/ticker_api.h"
#include "hal/us_ticker_api.h"
#include "platform/critical.h"

namespace mbed {

Timer::Timer() : _running(), _start(), _time(), _ticker_data(get_us_ticker_data()) {
    reset();
}

Timer::Timer(const ticker_data_t *data) : _running(), _start(), _time(), _ticker_data(data) {
    reset();
}

void Timer::start() {
    core_util_critical_section_enter();
    if (!_running) {
        _start = ticker_read(_ticker_data);
        _running = 1;
    }
    core_util_critical_section_exit();
}

void Timer::stop() {
    core_util_critical_section_enter();
    _time += slicetime();
    _running = 0;
    core_util_critical_section_exit();
}

int Timer::read_us() {
    core_util_critical_section_enter();
    int time = _time + slicetime();
    core_util_critical_section_exit();
    return time;
}

float Timer::read() {
    return (float)read_us() / 1000000.0f;
}

int Timer::read_ms() {
    return read_us() / 1000;
}

int Timer::slicetime() {
    core_util_critical_section_enter();
    int ret = 0;
    if (_running) {
        ret = ticker_read(_ticker_data) - _start;
    }
    core_util_critical_section_exit();
    return ret;
}

void Timer::reset() {
    core_util_critical_section_enter();
    _start = ticker_read(_ticker_data);
    _time = 0;
    core_util_critical_section_exit();
}

Timer::operator float() {
    return read();
}

} // namespace mbed
