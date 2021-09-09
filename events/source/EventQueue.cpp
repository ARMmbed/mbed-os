/* events
 * Copyright (c) 2016-2019 ARM Limited
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
#include "events/EventQueue.h"
#include "events/mbed_events.h"

using mbed::Callback;

namespace events {

EventQueue::EventQueue(unsigned event_size, unsigned char *event_pointer)
{
    if (event_size == 0) {
        // As static queue (EventQueue(0)) won't perform any access to its data buffer
        // we can pass (0, NULL)
        equeue_create_inplace(&_equeue, 0, NULL);
    } else {
        if (!event_pointer) {
            equeue_create(&_equeue, event_size);
        } else {
            equeue_create_inplace(&_equeue, event_size, event_pointer);
        }
    }
}

EventQueue::~EventQueue()
{
    equeue_destroy(&_equeue);
}

void EventQueue::dispatch_for(duration ms)
{
    return equeue_dispatch(&_equeue, ms.count());
}

void EventQueue::dispatch(int ms)
{
    return equeue_dispatch(&_equeue, ms);
}

void EventQueue::dispatch_forever()
{
    return equeue_dispatch(&_equeue, -1);
}

void EventQueue::dispatch_once()
{
    return equeue_dispatch(&_equeue, 0);
}

void EventQueue::break_dispatch()
{
    return equeue_break(&_equeue);
}

unsigned EventQueue::tick()
{
    return equeue_tick();
}

bool EventQueue::cancel(int id)
{
    return equeue_cancel(&_equeue, id);
}

int EventQueue::time_left(int id)
{
    return equeue_timeleft(&_equeue, id);
}

void EventQueue::background(Callback<void(int)> update)
{
    // Start by setting the background callback to nullptr
    // as equeue_background calls the existing handler with a timeout
    // of -1 to indicate to the callback that the tineout process is
    // not further required.
    // Updating _update before would prevent calling into the
    // old callback as the callbacks share the same memory locations.
    equeue_background(&_equeue, 0, 0);

    _update = update;

    if (_update) {
        equeue_background(&_equeue, &Callback<void(int)>::thunk, &_update);
    }
}

int EventQueue::chain(EventQueue *target)
{
    if (target) {
        return equeue_chain(&_equeue, &target->_equeue);
    } else {
        return equeue_chain(&_equeue, 0);
    }
}
}
