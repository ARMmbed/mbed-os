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
#include "ticker_api.h"
#include "critical.h"

namespace mbed {

void Ticker::detach() {
    core_util_critical_section_enter();
    remove();
    _function.attach(0);
    core_util_critical_section_exit();
}

void Ticker::setup(timestamp_t t) {
    core_util_critical_section_enter();
    remove();
    _delay = t;
    insert(_delay + ticker_read(_ticker_data));
    core_util_critical_section_exit();
}

void Ticker::handler() {
    insert(event.timestamp + _delay);
    _function.call();
}

} // namespace mbed
