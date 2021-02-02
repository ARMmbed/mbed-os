/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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

#include "EventQueue.h"

namespace events {

handle_t EventQueue::call_handler(function_t handler)
{
    return call_handler_in(0, handler);
}

handle_t EventQueue::call_handler_in(tick_t ms, function_t handler)
{
    auto real_handler = new function_t(handler);
    handlers.emplace(now + ms, real_handler );
    return reinterpret_cast<handle_t >(real_handler);
}

bool EventQueue::cancel_handler(handle_t handle)
{
    if (!handle) {
        return false;
    }

    auto finder = [handle](decltype(handlers)::const_reference e) {
        if (e.second.get() == reinterpret_cast<function_t*>(handle)) {
            return true;
        }
        return false;
    };

    auto to_cancel = std::find_if(handlers.begin(), handlers.end(), finder);
    handlers.erase(to_cancel);
    return true;
}

void EventQueue::process_events(tick_t duration_ms)
{
    // execute all events during the duration
    for (uint64_t i = 0; i < duration_ms; ++i) {
        process_events();
        ++now;
    }

    // last round to execute immediate events
    process_events();
}

void EventQueue::process_events() {
    while (true) {
        if (handlers.begin() == handlers.end()) {
            return;
        }

        if (handlers.begin()->first > now) {
            return;
        }

        (*handlers.begin()->second)();
        handlers.erase(handlers.begin());
    }
}

}
