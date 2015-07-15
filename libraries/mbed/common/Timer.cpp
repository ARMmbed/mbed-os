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
#include "Timer.h"
#include "ticker_api.h"
#include "us_ticker_api.h"

namespace mbed {

Timer::Timer() : _running(), _start(), _time(), _ticker_data(get_us_ticker_data()) {
    reset();
}

Timer::Timer(const ticker_data_t *data) : _running(), _start(), _time(), _ticker_data(data) {
    reset();
}

void Timer::start() {
    if (!_running) {
        _start = ticker_read(_ticker_data);
        _running = 1;
    }
}

void Timer::stop() {
    _time += slicetime();
    _running = 0;
}

int Timer::read_us() {
    return _time + slicetime();
}

float Timer::read() {
    return (float)read_us() / 1000000.0f;
}

int Timer::read_ms() {
    return read_us() / 1000;
}

int Timer::slicetime() {
    if (_running) {
        return ticker_read(_ticker_data) - _start;
    } else {
        return 0;
    }
}

void Timer::reset() {
    _start = ticker_read(_ticker_data);
    _time = 0;
}

#ifdef MBED_OPERATORS
Timer::operator float() {
    return read();
}
#endif

} // namespace mbed
