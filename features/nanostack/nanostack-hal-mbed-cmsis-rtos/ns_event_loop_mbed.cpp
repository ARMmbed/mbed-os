/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#include "mbed_assert.h"
#include "platform/arm_hal_interrupt.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "cmsis.h"
#include "cmsis_os2.h"
#include "mbed_rtos_storage.h"
#endif
#include "ns_trace.h"

#include "eventOS_scheduler.h"

#include "mbed_error.h"
#include "mbed_shared_queues.h"
#include "events/Event.h"
#include "ns_event_loop_mutex.h"
#include "ns_event_loop.h"

#define TRACE_GROUP "evlp"

#if MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_USE_MBED_EVENTS

using events::EventQueue;
using events::Event;

static Event<void()> *event;
static volatile int event_pending;
static volatile bool started;

void eventOS_scheduler_signal(void)
{
    platform_enter_critical();
    if (started && event_pending == 0) {
        event_pending = event->post();
        MBED_ASSERT(event_pending != 0);
    }
    platform_exit_critical();
}

void eventOS_scheduler_idle(void)
{
    error("Shouldn't be called");
}

static void do_dispatch_with_mutex_held()
{
    platform_enter_critical();
    event_pending = 0;
    platform_exit_critical();

    /* Process only 1 Nanostack event at a time, to try to be nice to
     * others on the global queue.
     */
    eventOS_scheduler_mutex_wait();
    bool dispatched = eventOS_scheduler_dispatch_event();
    eventOS_scheduler_mutex_release();

    /* Go round again if (potentially) more */
    if (dispatched) {
        eventOS_scheduler_signal();
    }
}

void ns_event_loop_init(void)
{
    ns_event_loop_mutex_init();
}

void ns_event_loop_thread_create(void)
{
    EventQueue *equeue = mbed::mbed_event_queue();
    MBED_ASSERT(equeue != NULL);

    event = new Event<void()>(equeue, do_dispatch_with_mutex_held);
    MBED_ASSERT(event != NULL);
}

void ns_event_loop_thread_start(void)
{
    started = true;
    eventOS_scheduler_signal();
}

#endif // MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_USE_MBED_EVENTS
