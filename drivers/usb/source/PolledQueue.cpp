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

#include "usb/internal/PolledQueue.h"

#include "events/mbed_events.h"
#include "platform/Callback.h"


events::PolledQueue::PolledQueue(mbed::Callback<void()> cb): _cb(cb)
{

}

events::PolledQueue::~PolledQueue()
{

}

void events::PolledQueue::dispatch()
{
    core_util_critical_section_enter();
    uint64_t buf[MBED_MAX_TASK_SIZE / sizeof(uint64_t)];

    while (true) {

        // Atomically dequeue the task and copy the callback
        TaskBase *task = _list.dequeue();
        if (!task) {
            break;
        }
        MBED_ASSERT(sizeof(buf) >= task_size(task));
        TaskBase::run_callback_t callback = task_start(task, (uint8_t *)buf, sizeof(buf));

        // Run the callback outside the critical section
        core_util_critical_section_exit();
        callback((uint8_t *)buf);
        core_util_critical_section_enter();

        // Finish
        task_finish(task);
        task = NULL;

    }

    core_util_critical_section_exit();
}

void events::PolledQueue::attach(mbed::Callback<void()> cb)
{
    core_util_critical_section_enter();

    _cb = cb;

    core_util_critical_section_exit();
}

void events::PolledQueue::post(TaskBase *task)
{
    core_util_critical_section_enter();

    bool empty = _list.head() == NULL;
    _list.remove(task);
    _list.enqueue(task);
    if (empty && _cb) {
        _cb();
    }

    core_util_critical_section_exit();
}

void events::PolledQueue::cancel(TaskBase *task)
{
    core_util_critical_section_enter();

    _list.remove(task);

    core_util_critical_section_exit();
}
