/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include "Timer.h"
#include "Timer_stub.h"

namespace mbed {

Timer::Timer()
{
}

Timer::Timer(const ticker_data_t *data)
{
}

Timer::~Timer()
{
}

void Timer::start()
{
}

void Timer::stop()
{
    ;
}

int Timer::read_us()
{
    return 0;
}

float Timer::read()
{
    return 0;
}

int Timer::read_ms()
{
    timer_stub_value += timer_stub_step;
    return timer_stub_value;
}

us_timestamp_t Timer::read_high_resolution_us()
{
    return 0;
}

void Timer::reset()
{
}

Timer::operator float()
{
    return 0;
}

} // namespace mbed
