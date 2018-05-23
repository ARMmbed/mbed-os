/* events
 * Copyright (c) 2018 ARM Limited
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

#ifndef MBED_TASK_H
#define MBED_TASK_H

#include "events/EventQueue.h"
#include "events/TaskBase.h"
#include "platform/mbed_assert.h"
#include "platform/Callback.h"
#include "events/_AllArgs.h"

namespace events {
/** \addtogroup events */

template <typename F>
class Task;

/** Task
 *
 *  Representation of a postable task
 * @ingroup events
 */
template <typename R, typename A0, typename A1>
class Task<R(A0, A1)>: public TaskBase {
public:

    /**
     * Construct a new task
     *
     * @param q TaskQueue to post to
     * @param cb Callback to run
     */
    Task(TaskQueue *q=NULL, mbed::Callback<R(A0, A1)> cb=mbed::Callback<R(A0, A1)>())
        : TaskBase(q), _args(cb) {
    }

    /**
     * Set the callback of this task
     *
     * @param cb Callback to run
     */
    Task& operator=(mbed::Callback<R(A0, A1)> cb) {
        _args.b0 = cb;
        return *this;
    }

    /**
     * Post this task for execution
     *
     * The number of arguments to call should match
     * the type of the callback. For example Task<void(int, int)>
     * expects two integers as arguments to call, while Task<void()>
     * expects no arguments.
     *
     * @param a0 First callback parameter
     * @param a1 Second callback parameter
     */
    void call(A0 a0, A1 a1) {
        _args.b1 = a0;
        _args.b2 = a1;
        post();
    }

protected:

    virtual uint32_t size() {
        return sizeof(_args);
    }

    virtual run_callback_t start(void *data, uint32_t max_size) {
        All::ops::copy(data, (void*)&_args);
        return &All::ops::call;
    }

private:
    typedef AllArgs<mbed::Callback<R(A0, A1)>, A0, A1> All;
    All _args;
};

template <typename R, typename A0>
class Task<R(A0)>: public TaskBase {
public:

    Task(TaskQueue *q=NULL, mbed::Callback<R(A0)> cb=mbed::Callback<R(A0)>())
        : TaskBase(q), _args(cb) {
    }

    Task& operator=( mbed::Callback<R(A0)> cb) {
        _args.b0 = cb;
        return *this;
    }

    void call(A0 a0) {
        _args.b1 = a0;
        post();
    }

protected:

    virtual uint32_t size() {
        return sizeof(_args);
    }

    virtual run_callback_t start(void *data, uint32_t max_size) {
        All::ops::copy(data, (void*)&_args);
        return &All::ops::call;
    }

private:
    typedef AllArgs<mbed::Callback<R(A0)>, A0> All;
    All _args;
};

template <typename R>
class Task<R()>: public TaskBase {
public:

    Task(TaskQueue *q=NULL, mbed::Callback<R()> cb=mbed::Callback<R()>())
        : TaskBase(q), _args(cb) {
    }

    Task& operator=( mbed::Callback<R()> cb) {
        _args.b0 = cb;
        return *this;
    }

    void call() {
        post();
    }

protected:

    virtual uint32_t size() {
        return sizeof(_args);
    }

    virtual run_callback_t start(void *data, uint32_t max_size) {
        All::ops::copy(data, (void*)&_args);
        return &All::ops::call;
    }

private:
    typedef AllArgs<mbed::Callback<R()> > All;
    All _args;
};

}

/** @}*/

#endif
