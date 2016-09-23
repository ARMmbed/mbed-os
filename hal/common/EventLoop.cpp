/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#ifdef MBED_CONF_RTOS_PRESENT

#include "EventLoop.h"
#include "Event.h"
#include "Callback.h"

namespace mbed {

EventLoop::EventLoop(uint32_t size, osPriority priority, uint32_t stack_size, unsigned char *stack_pointer):
    _queue(size),_thread(priority, stack_size, stack_pointer) {
}

void EventLoop::start() {
    _thread.start(callback(this, &EventLoop::dispatch));
}

void EventLoop::stop() {
    _thread.terminate();
}

bool EventLoop::post(const queue_event_t& evt) {
    queue_event_t *_evt = _queue.alloc();
    if (_evt == NULL) {
        return false;
    }
    _evt->attach(evt);
    _queue.put(_evt);
    return true;
}

void EventLoop::dispatch(void) {
    osEvent e;

    while (true) {
        e = _queue.get();
        if (e.status == osEventMail) {
            queue_event_t *cb = (queue_event_t*)e.value.p;
            cb->call();
            _queue.free(cb);
        }
    }
}

} // namespace mbed

#endif // #ifdef MBED_CONF_RTOS_PRESENT
