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

#ifndef MBED_CHAINABLEEVENTHANDLER_H_
#define MBED_CHAINABLEEVENTHANDLER_H_

#include <new>

/**
 * Base class for chainable EventHandlers. Acts as a collection
 * of discrete EventHandlers that can be linked together and
 * notified when relevant events happen
 */
template<typename T>
class ChainableEventHandler
{

public:

    ChainableEventHandler() { }

    virtual ~ChainableEventHandler() {
        // Clean up all nodes
        auto it = head;
        while(it) {
            node_t* temp = it;
            it = it->next;
            delete temp;
        }
    }

    /**
     * Add an EventHandler to be notified of events sent to
     * this ChainableGattServerEventHandler
     *
     * @param[in] event_handler Handler to add to chain
     */
    void addEventHandler(T* event_handler) {
        auto eh = new (std::nothrow) node_t { event_handler, nullptr };
        if(!eh) { return; }
        if(!head) {
            head = eh;
        } else {
            auto *it = head;
            while(it->next) {
                it = it->next;
            }
            it->next = eh;
        }
    }

    /**
     * Remove an EventHandler previously added with addEventHandler.
     *
     * @param[in] event_handler Pointer to event handler to remove
     */
    void removeEventHandler(T* target) {
        node_t* to_remove = head;
        if(head->eh == target) {
            head = head->next;
        } else {
            auto* it = head;
            while(it->next) {
                if(it->next->eh == target) {
                    to_remove = it->next;
                    break;
                }
                it = it->next;
            }
            if(it->next) {
                it->next = to_remove->next;
            } else {
                to_remove = nullptr;
            }
        }

        delete to_remove;
    }

protected:

    template<typename... Args>
    void execute_on_all(void (T::*fn)(Args...), Args&... args) {
        auto it = head;
        while(it) {
            (it->eh->*fn)(args...);
            it = it->next;
        }
    }

private:

    struct node_t {
        T* eh;
        node_t* next = nullptr;
    };

    node_t *head = nullptr;

};

#endif /* MBED_CHAINABLEEVENTHANDLER_H_ */
