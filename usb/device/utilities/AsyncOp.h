/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef MBED_ASYNC_OP_H
#define MBED_ASYNC_OP_H

#include "Mutex.h"
#include "Semaphore.h"

#include "LinkEntry.h"
#include "LinkedListBase.h"

class AsyncOp: public LinkEntry {
public:
    /**
     * Construct a new AsyncOp object
     *
     * @param lock Mutex used to serialize the object or code calling complete
     *  or NULL if a critical section is used
     */
    AsyncOp(rtos::Mutex *lock);

    /**
     * Add this operation to the linked list to start it
     */
    void start(LinkedListBase *list);

    /**
     * Wait for this asynchronous operation to complete
     */
    void wait();

    /**
     * Mark this asynchronous operation as complete
     *
     * This wake the thread calling wait()
     */
    void complete();

private:
    void _lock();
    void _unlock();

    LinkedListBase *_list;
    rtos::Semaphore *_signal;
    rtos::Mutex *const _signal_lock;
};

#endif
