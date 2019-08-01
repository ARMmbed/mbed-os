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

#ifndef TASK_BASE_H
#define TASK_BASE_H

#include "platform/Callback.h"
#include "platform/mbed_assert.h"
#include "LinkEntry.h"

namespace rtos {
class Semaphore;
}

namespace events {

class TaskQueue;

/**
 * \defgroup drivers_TaskBase TaskBase class
 * \ingroup drivers-internal-api-usb
 * @{
 */

/** TaskBase
 *
 *  Representation of a caller allocated task
 */
class TaskBase : public LinkEntry {
public:

    typedef void (*run_callback_t)(void *data);

    /**
     * Construct a new TaskBase object
     *
     * @param q Queue for posting to
     */
    TaskBase(TaskQueue *q);

    /**
     * Destroy this TaskBase
     */
    virtual ~TaskBase();

    /**
     * Set the queue of this task
     *
     * @param q TaskQueue to post to
     */
    void set(TaskQueue *q);

    /**
     * Cancel the execution of this task
     *
     * Once cancelled the task can be posted again. Previous
     * calls to post may still run. If you need to ensure the
     * callback has finished the function wait() can be used.
     *
     * @note This function is interrupt safe
     */
    void cancel();

    /**
     * Return true if this task is ready to be posted
     *
     * Check if this task is on a queue waiting to be run.
     *
     * @return true if it is safe to call post
     */
    bool ready();

    /**
     * Wait for this task to finish execution
     *
     * When this function returns then this task is in the finished state.
     */
    void wait();

    /**
     * Check if the callback has run to completion or been fully canceled
     *
     * When an task is finished the queue is completely done with it and the
     * callback is either fully complete or has been canceled and will not run.
     *
     * @return true if this task has been flushed from the queue, false otherwise
     */
    bool finished();

protected:

    /**
     * Size of buffer required for TaskBase::start
     *
     * @return requested buffer size
     */
    virtual uint32_t size() = 0;

    /**
     * Copy any callback data and return a callback to run
     *
     * @param data Buffer to copy data to. Do not copy more than TaskBase::size() data.
     * @param size Maximum size to copy
     */
    virtual run_callback_t start(void *data, uint32_t size) = 0;

    /**
     * Inform this task that execution has finished.
     *
     */
    virtual void finish();

    /**
     * Post this task to the set TaskQueue for execution
     */
    void post();

private:

    TaskQueue *_queue;
    bool _posted;
    uint16_t _start_count;
    rtos::Semaphore *_flush_sem;

    friend class TaskQueue;

    /*
     * Must be called in a critical section
     *
     * This function should not be called directly. Instead
     * TaskQueue::task_start should be used instead.
     */
    run_callback_t _start(void *buffer, uint32_t size);

    /*
     * Must be called in a critical section
     *
     * This function should not be called directly. Instead
     * TaskQueue::task_finish should be used instead.
     *
     */
    void _finish();

    /*
     * Unblock wait if this task is finished
     */
    void _wake_check();
};

/** @}*/

}

#endif
