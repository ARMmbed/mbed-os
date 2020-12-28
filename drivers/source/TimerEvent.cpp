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
#include "drivers/TimerEvent.h"

#include <stddef.h>
#include "hal/us_ticker_api.h"

using namespace std::chrono;

namespace mbed {

TimerEvent::TimerEvent(const ticker_data_t *data) : event(), _ticker_data(data)
{
    _ticker_data.set_handler(irq);
}

void TimerEvent::irq(uint32_t id)
{
    TimerEvent *timer_event = reinterpret_cast<TimerEvent *>(id);
    timer_event->handler();
}

TimerEvent::~TimerEvent()
{
    remove();
}

// insert in to linked list
void TimerEvent::insert(timestamp_t timestamp)
{
    ticker_insert_event(_ticker_data, &event, timestamp, (uint32_t)this);
}

void TimerEvent::insert(microseconds rel_time)
{
    insert_absolute(_ticker_data.now() + rel_time);
}

void TimerEvent::insert_absolute(us_timestamp_t timestamp)
{
    ticker_insert_event_us(_ticker_data, &event, timestamp, (uint32_t)this);
}

void TimerEvent::insert_absolute(TickerDataClock::time_point timestamp)
{
    _ticker_data.insert_event(&event, timestamp, (uint32_t)this);
}

void TimerEvent::remove()
{
    _ticker_data.remove_event(&event);
}

} // namespace mbed
