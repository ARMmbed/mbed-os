/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#ifndef POLLED_QUEUE_H
#define POLLED_QUEUE_H

#include "usb/internal/TaskQueue.h"
#include "platform/Callback.h"
#include "LinkedList.h"

namespace events {
/**
 * \defgroup drivers_PolledQueue PolledQueue class
 * \ingroup drivers-internal-api-usb
 * @{
 */

/** PolledQueue
 *
 * This class is an implementation of TaskQueue which is
 * processed synchronously by calls to dispatch.
 */
class PolledQueue: public TaskQueue {
public:

    /** Create a PolledQueue
     *
     *  Create an event queue.
     *
     *  @param cb Callback called when dispatch needs to be called
     */
    PolledQueue(mbed::Callback<void()> cb = nullptr);

    virtual ~PolledQueue();

    virtual void post(TaskBase *event);

    virtual void cancel(TaskBase *event);

    /**
     * Process all the events in this queue
     */
    void dispatch();

    /**
     * Attach a callback indicating that this queue needs to be processed
     *
     * @param cb Callback called when dispatch needs to be called
     */
    void attach(mbed::Callback<void()> cb);

protected:

    mbed::Callback<void()> _cb;
    LinkedList<TaskBase> _list;

};

/** @}*/

}
#endif
