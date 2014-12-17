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
#include "Ticker.h"

#include "TimerEvent.h"
#include "FunctionPointer.h"

namespace mbed {

void Ticker::detach() {
    remove();
    _function.attach(0);
}

void Ticker::setup(timestamp_t t) {
    remove();
    _delay = t;
    insert(_delay + us_ticker_read());
}

void Ticker::handler() {

    _function.call();

    /* extend 32 bit timestamps to 64 bit for easy math */
    uint64_t currentTime = us_ticker_read(); 
    uint64_t scheduledTime = event.timestamp;

    /*  Check if the timer has wrapped around since the scheduled event and now.
        If it has, increment the current time with (1 << 32)
    */
    if (currentTime < scheduledTime) {
        currentTime += (1ULL << 32);
    }

    /*  Check if we are too late to schedule an event with the right interval.
        If we are, fast forward the event.timestamp to match the current time.
    */
    if (scheduledTime + _delay < currentTime) {
        uint32_t missedTicks = (currentTime - scheduledTime) / _delay;

        event.timestamp = scheduledTime + missedTicks * _delay;
    }

    insert(event.timestamp + _delay);
}

} // namespace mbed
