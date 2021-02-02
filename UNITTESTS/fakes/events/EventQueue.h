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

#ifndef EVENTQUEUE_FAKE_H
#define EVENTQUEUE_FAKE_H

#include <map>
#include <memory>
#include <functional>
#include <algorithm>
#include "events/EventQueue.h"
#include <chrono>

namespace events {

typedef intptr_t handle_t;
typedef std::function<void()> function_t;
typedef unsigned long tick_t;

class EventQueue {
    using duration = std::chrono::duration<int, std::milli>;

public:
    EventQueue(unsigned size = 0, unsigned char *buffer = NULL) { delete buffer; };

    ~EventQueue() { };

    void dispatch(int ms = -1) {
        if (ms > 0) {
            process_events(ms);
        } else {
            dispatch_forever();
        }
    };

    void dispatch_forever() {
        now = (tick_t)-1;
        process_events();
        now = 0;
    };

    void break_dispatch() { };

    tick_t tick() { return now; };

    bool cancel(handle_t id) { return cancel_handler(id); };

    handle_t call(function_t f)
    {
        return call_handler(f);
    }

    handle_t call_in(duration ms, function_t f)
    {
        return call_handler_in(ms.count(), f);
    }

private:
    handle_t call_handler(function_t handler);

    handle_t call_handler_in(tick_t ms, function_t handler);

    bool cancel_handler(handle_t handle);

    void process_events(tick_t duration_ms);

    void process_events();

private:
    std::multimap<uint64_t, std::unique_ptr<function_t> > handlers;
    tick_t now = 0;
};

}

#endif //EVENTQUEUE_FAKE_H
