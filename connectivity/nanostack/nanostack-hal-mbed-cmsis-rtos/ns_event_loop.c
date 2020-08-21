/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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

#ifdef MBED_CONF_RTOS_PRESENT

#include "mbed_assert.h"
#include "cmsis.h"
#include "cmsis_os2.h"
#include "mbed_rtos_storage.h"
#include "ns_trace.h"

#include "eventOS_scheduler.h"

#include "ns_event_loop_mutex.h"
#include "ns_event_loop.h"

#define TRACE_GROUP "evlp"

#if !MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_USE_MBED_EVENTS

#if MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_DISPATCH_FROM_APPLICATION

static mbed_rtos_storage_event_flags_t event_flag_cb;
static const osEventFlagsAttr_t event_flags_attr = {
    .name = "nanostack_event_flags",
    .cb_mem = &event_flag_cb,
    .cb_size = sizeof event_flag_cb
};
static osEventFlagsId_t event_flag_id;

#else

#ifndef MBED_TZ_DEFAULT_ACCESS
#define MBED_TZ_DEFAULT_ACCESS   0
#endif

static void event_loop_thread(void *arg);

static uint64_t event_thread_stk[MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_THREAD_STACK_SIZE / 8];
static mbed_rtos_storage_thread_t event_thread_tcb;
static const osThreadAttr_t event_thread_attr = {
    .name = "nanostack_event_thread",
    .priority = osPriorityNormal,
    .stack_mem = &event_thread_stk[0],
    .stack_size = sizeof event_thread_stk,
    .cb_mem = &event_thread_tcb,
    .cb_size = sizeof event_thread_tcb,
    .tz_module = MBED_TZ_DEFAULT_ACCESS,
};
#endif

#if !MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_DISPATCH_FROM_APPLICATION
static osThreadId_t event_thread_id;
#endif

void eventOS_scheduler_signal(void)
{
    // XXX why does signal set lock if called with irqs disabled?
    //__enable_irq();
    //tr_debug("signal %p", (void*)event_thread_id);
#if MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_DISPATCH_FROM_APPLICATION
    osEventFlagsSet(event_flag_id, 1);
#else
    osThreadFlagsSet(event_thread_id, 1);
#endif
    //tr_debug("signalled %p", (void*)event_thread_id);
}

void eventOS_scheduler_idle(void)
{
    //tr_debug("idle");
    eventOS_scheduler_mutex_release();

#if MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_DISPATCH_FROM_APPLICATION
    osEventFlagsWait(event_flag_id, 1, osFlagsWaitAny, osWaitForever);
#else
    osThreadFlagsWait(1, 0, osWaitForever);
#endif

    eventOS_scheduler_mutex_wait();
}

#if !MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_DISPATCH_FROM_APPLICATION
static void event_loop_thread(void *arg)
{
    (void)arg;
    eventOS_scheduler_mutex_wait();
    eventOS_scheduler_run(); //Does not return
}
#endif

// This is used to initialize the lock used by event loop even
// if it is not ran in a separate thread.
void ns_event_loop_init(void)
{
    ns_event_loop_mutex_init();

    // If a separate event loop thread is not used, the signaling
    // happens via event flags instead of thread flags. This allows one to
    // perform the initialization from any thread and removes need to know the id
    // of event loop dispatch thread.
#if MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_DISPATCH_FROM_APPLICATION
    event_flag_id  = osEventFlagsNew(&event_flags_attr);
    MBED_ASSERT(event_flag_id != NULL);
#endif
}

#if !MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_DISPATCH_FROM_APPLICATION
void ns_event_loop_thread_create(void)
{
    event_thread_id = osThreadNew(event_loop_thread, NULL, &event_thread_attr);
    MBED_ASSERT(event_thread_id != NULL);
}

void ns_event_loop_thread_start(void)
{
}
#endif

#endif // !MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_USE_MBED_EVENTS

#endif //MBED_CONF_RTOS_PRESENT
