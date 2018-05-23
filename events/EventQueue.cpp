/* events
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
#include "events/EventQueue.h"

#include "events/mbed_events.h"
#include "mbed.h"


EventQueue::EventQueue(unsigned event_size, unsigned char *event_pointer) {
    if (!event_pointer) {
        equeue_create(&_equeue, event_size);
    } else {
        equeue_create_inplace(&_equeue, event_size, event_pointer);
    }
}

EventQueue::~EventQueue() {
    equeue_destroy(&_equeue);
}

void EventQueue::dispatch(int ms) {
    return equeue_dispatch(&_equeue, ms);
}

void EventQueue::break_dispatch() {
    return equeue_break(&_equeue);
}

unsigned EventQueue::tick() {
    return equeue_tick();
}

void EventQueue::cancel(int id) {
    return equeue_cancel(&_equeue, id);
}

int EventQueue::time_left(int id) {
    return equeue_timeleft(&_equeue, id);
}

void EventQueue::background(Callback<void(int)> update) {
    _update = update;

    if (_update) {
        equeue_background(&_equeue, &Callback<void(int)>::thunk, &_update);
    } else {
        equeue_background(&_equeue, 0, 0);
    }
}

void EventQueue::chain(EventQueue *target) {
    if (target) {
        equeue_chain(&_equeue, &target->_equeue);
    } else {
        equeue_chain(&_equeue, 0);
    }
}
