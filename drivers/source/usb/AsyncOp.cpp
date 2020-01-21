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

#include "AsyncOp.h"
#include "mbed_critical.h"
#include "mbed_assert.h"

using namespace rtos;

AsyncOp::AsyncOp():
    _list(NULL), _wait(NULL), _aborted(false), _timeout(false)
{

}

AsyncOp::AsyncOp(mbed::Callback<void()> &callback):
    _list(NULL), _wait(NULL), _aborted(false), _timeout(false)
{
    _callback = callback;
}

AsyncOp::~AsyncOp()
{
    MBED_ASSERT(_list == NULL);
}

void AsyncOp::wait(rtos::Mutex *host_mutex, uint32_t milliseconds)
{
    // Optimization so semaphore is only created if necessary
    core_util_critical_section_enter();
    bool done = _list == NULL;
    core_util_critical_section_exit();
    if (done) {
        return;
    }

    // Construct semaphore to wait on
    Semaphore sem(0);

    core_util_critical_section_enter();
    done = _list == NULL;
    // Wait is only allowed to be called from one thread
    MBED_ASSERT(_wait == NULL);
    _wait = &sem;
    core_util_critical_section_exit();

    if (done) {
        // Operation was signaled before semaphore was set
        return;
    }

    if (sem.try_acquire_for(milliseconds)) {
        // Operation completion signaled semaphore
        return;
    }

    _host_lock(host_mutex);
    _abort(true);
    _host_unlock(host_mutex);
}

void AsyncOp::abort()
{
    // Host lock must be held

    _abort(false);
}

void AsyncOp::complete()
{
    core_util_critical_section_enter();

    mbed::Callback<void()> cb = _callback;
    _callback = nullptr;
    _list = nullptr;
    if (_wait != nullptr) {
        _wait->release();
    }

    core_util_critical_section_exit();

    if (cb) {
        cb();
    }
}

bool AsyncOp::timeout()
{
    core_util_critical_section_enter();

    bool ret = _timeout;

    core_util_critical_section_exit();
    return ret;
}

void AsyncOp::_abort(bool timeout)
{
    // host lock must be held

    core_util_critical_section_enter();
    OperationListBase *list = _list;
    if (list) {
        _callback = nullptr;
        _aborted = true;
        _wait = nullptr;
        _timeout = timeout;
        _list = nullptr;
    }
    core_util_critical_section_exit();
    if (list) {
        list->remove(this);
    }
}

void AsyncOp::_host_lock(rtos::Mutex *host_mutex)
{
    if (host_mutex) {
        host_mutex->lock();
    } else {
        core_util_critical_section_enter();
    }
}

void AsyncOp::_host_unlock(rtos::Mutex *host_mutex)
{
    if (host_mutex) {
        host_mutex->unlock();
    } else {
        core_util_critical_section_exit();
    }
}
