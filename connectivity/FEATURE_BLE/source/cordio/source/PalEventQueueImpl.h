/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
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

#ifndef BLE_PAL_SIMPLE_EVENT_QUEUE_H_
#define BLE_PAL_SIMPLE_EVENT_QUEUE_H_

#include <new>
#include "source/pal/PalEventQueue.h"

#include "wsf_buf.h"

namespace ble {

class BLEInstanceBase;

namespace impl {

/**
 * Simple implementation of the EventQueue.
 */
class PalEventQueue final : public ble::PalEventQueue {
public:
    typedef mbed::Callback<void()> event_t;

    /**
     * Construct an empty event queue.
     *
     * @attention a call to initialize is mandatory before any other call.
     */
    PalEventQueue() :
        _ble_base(nullptr), _events(nullptr)
    {
    }

    /**
     * Initialize the event queue with a BLEInstanceBase and a ble id.
     *
     * @param ble_base the instance which will be used to signal the presence
     * of new events.
     */
    void initialize(ble::BLEInstanceBase *ble_base)
    {
        _ble_base = ble_base;
    }

    /**
     * @see ble::EventQueue
     */
    ~PalEventQueue()
    {
        clear();
    }

    /**
     * @see ble::post
     */
    bool post(const mbed::Callback<void()> &event) final
    {
        if (_ble_base == nullptr) {
            return false;
        }
        void *event_buf = WsfBufAlloc(sizeof(EventNode));
        MBED_ASSERT(event_buf != nullptr);
        if (event_buf == nullptr) {
            return false;
        }
        auto *next = new(event_buf) EventNode(event);

        if (_events == nullptr) {
            _events = next;
        } else {
            EventNode *previous = _events;
            while (previous->next) {
                previous = previous->next;
            }

            previous->next = next;
        }

        signal_event();

        return true;
    }

    /**
     * Clear the event queue from all its events.
     */
    void clear()
    {
        while (_events) {
            EventNode *next = _events->next;
            _events->~EventNode();
            WsfBufFree(_events);
            _events = next;
        }
    }

    /**
     * Process the event queue.
     */
    void process()
    {
        while (_events) {
            // pop
            auto *event = _events;
            _events = event->next;
            // execute and delete
            event->event();
            event->~EventNode();
            WsfBufFree(event);
        }
    }

private:
    struct EventNode {
        EventNode(const event_t event) : event(event), next(nullptr)
        {
        }

        event_t event;
        EventNode *next;
    };

    void signal_event();

    ble::BLEInstanceBase *_ble_base;
    EventNode *_events;
};

} // namespace impl
} // namespace ble

#endif /* BLE_PAL_SIMPLE_EVENT_QUEUE_H_ */
