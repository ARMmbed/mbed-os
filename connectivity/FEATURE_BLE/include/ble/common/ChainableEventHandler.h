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

    ~ChainableEventHandler() {
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
     * this ChainableEventHandler
     *
     * @param[in] event_handler Handler to add to chain
     *
     * @retval true if adding EventHandler was successful, false otherwise
     */
    bool addEventHandler(T* event_handler) {
        auto eh = new (std::nothrow) node_t { event_handler, nullptr };
        if(!eh) { return false; }
        if(!head) {
            head = eh;
        } else {
            auto *it = head;
            while(it->next) {
                it = it->next;
            }
            it->next = eh;
        }
        return true;
    }

    /**
     * Remove an EventHandler previously added with addEventHandler.
     *
     * @param[in] event_handler Pointer to event handler to remove
     */
    void removeEventHandler(T* event_handler) {
        node_t* to_remove = head;
        if(head->eh == event_handler) {
            head = head->next;
        } else {
            auto* it = head;
            while(it->next) {
                if(it->next->eh == event_handler) {
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

    /**
     * Test if an event handler is present in the chain or not.
     *
     * @param[in] event_handler Pointer to event handler to check
     */
    bool isEventHandlerPresent(T* event_handler) {
        auto* it = head;
        while (it) {
            if (it == event_handler) {
                return true;
            }
            it = it->next;
        }
        return false;
    }

protected:

    template<typename... FnArgs, typename... Args>
    void execute_on_all(void (T::*fn)(FnArgs...), Args&&... args) {
        auto it = head;
        while(it) {
            // we do not use std::forward, args have to remain lvalues
            // as they are passed to multiple handlers
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
