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
#include "drivers/internal/TimerEvent.h"

#include <stddef.h>
#include "hal/us_ticker_api.h"

namespace mbed {

TimerEvent::TimerEvent() : event(), _ticker_data(get_us_ticker_data())
{
    ticker_set_handler(_ticker_data, (&TimerEvent::irq));
}

void TimerEvent::irq(uint32_t id)
{
    TimerEvent *timer_event = (TimerEvent *)id;
    timer_event->handler();
}

} // namespace mbed
