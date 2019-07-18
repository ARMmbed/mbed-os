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

#include "drivers/internal/TaskBase.h"
#include "drivers/internal/TaskQueue.h"
#include "events/mbed_events.h"
#include "rtos/Semaphore.h"
#include "platform/mbed_critical.h"

TaskBase::TaskBase(TaskQueue *q)
    : _queue(q), _posted(false), _start_count(0), _flush_sem(NULL)
{

}

TaskBase::~TaskBase()
{
    cancel();
    wait();
}

void TaskBase::set(TaskQueue *q)
{
    core_util_critical_section_enter();

    // Cannot set the queue when it has been posted but has not been finished
    MBED_ASSERT(!_posted);
    _queue = q;

    core_util_critical_section_exit();
}

void TaskBase::cancel()
{
    core_util_critical_section_enter();

    if (_posted) {
        _queue->cancel(this);
        _posted = false;
        _wake_check();
    }

    core_util_critical_section_exit();
}

void TaskBase::wait()
{
    // Fast path check for finished
    core_util_critical_section_enter();
    if (finished()) {
        core_util_critical_section_exit();
        return;
    }
    core_util_critical_section_exit();

    rtos::Semaphore sem;

    // If the event is in-flight then wait for it to complete
    core_util_critical_section_enter();
    if (finished()) {
        // This element has been flushed from the queue
        core_util_critical_section_exit();
        return;
    }
    _flush_sem = &sem;
    core_util_critical_section_exit();

    sem.acquire();
}

bool TaskBase::ready()
{
    core_util_critical_section_enter();

    bool is_ready = !_posted;

    core_util_critical_section_exit();
    return is_ready;
}

bool TaskBase::finished()
{
    core_util_critical_section_enter();

    bool is_finished = !_posted && (_start_count == 0);

    core_util_critical_section_exit();
    return is_finished;
}

void TaskBase::finish()
{
    // Nothing to do
}

void TaskBase::post()
{
    core_util_critical_section_enter();

    MBED_ASSERT(_queue);
    if (_queue) {
        MBED_ASSERT(!_posted);
        _queue->post(this);
        _posted = true;
    }

    core_util_critical_section_exit();
}

TaskBase::run_callback_t TaskBase::_start(void *buffer, uint32_t size)
{
    // Each call to _start must result in a call to _finish
    MBED_ASSERT(_start_count < 0xFFFF);
    _start_count++;
    _posted = false;

    return start(buffer, size);
}

void TaskBase::_finish()
{
    // Each call to _finish must be preceded by a call to _start
    MBED_ASSERT(_start_count > 0);
    _start_count--;
    _wake_check();
    finish();
}

void TaskBase::_wake_check()
{
    if (!finished()) {
        return;
    }
    if (_flush_sem) {
        _flush_sem->release();
        _flush_sem = NULL;
    }
}
