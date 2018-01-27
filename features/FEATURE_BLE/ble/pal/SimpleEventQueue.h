/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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
#include "EventQueue.h"
#include "ble/BLEInstanceBase.h"
#include "ble/BLE.h"

namespace ble {
namespace pal {

/**
 * Simple implementation of the pal::EventQueue.
 */
struct SimpleEventQueue : EventQueue {

    typedef mbed::Callback<void()> event_t;

    /**
     * Construct an empty event queue.
     *
     * @attention a call to initialize is mandatory before any other call.
     *
     * @param ble_instance_id The id of the ble instance associated with that
     * event queue.
     */
    SimpleEventQueue() :
        _ble_base(NULL), _ble_instance_id(0), _events(NULL) { }

    /**
     * Initialize the event queue with a BLEInstanceBase and a ble id.
     *
     * @param ble_base the instance which will be used to signal the presence
     * of new events.
     *
     * @param ble_id Id of the BLE instance using that event queue.
     */
    void initialize(BLEInstanceBase* ble_base, BLE::InstanceID_t ble_id)
    {
        _ble_base = ble_base;
        _ble_instance_id = ble_id;
    }

    /**
     * @see ble::pal::EventQueue
     */
    ~SimpleEventQueue()
    {
        clear();
    }

    /**
     * @see ble::pal::post
     */
    virtual bool post(const mbed::Callback<void()>& event)
    {
        if (_ble_base == NULL) {
            return false;
        }

        EventNode* next = new (std::nothrow) EventNode(event);
        if (next == NULL) {
            return false;
        }

        if (_events == NULL) {
            _events = next;
        } else {
            EventNode* previous = _events;
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
            EventNode* next = _events->next;
            delete _events;
            _events = next;
        }
    }

    /**
     * Process the event queue.
     */
    void process()
    {
        while (_events) {
            EventNode* next = _events->next;
            _events->event();
            delete _events;
            _events = next;
        }
    }

private:
    struct EventNode {
        EventNode(const event_t event) : event(event), next(NULL) { }
        event_t event;
        EventNode* next;
    };

    void signal_event()
    {
        _ble_base->signalEventsToProcess(_ble_instance_id);
    }

    BLEInstanceBase* _ble_base;
    BLE::InstanceID_t _ble_instance_id;
    EventNode* _events;
};

} // namespace pal
} // namespace ble

#endif /* BLE_PAL_SIMPLE_EVENT_QUEUE_H_ */
