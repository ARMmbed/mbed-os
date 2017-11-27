/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "event_stub.h"
#include "ns_types.h"
#include "ns_list.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "timer_sys.h"
#include "nsdynmemLIB.h"
#include "ns_timer.h"

event_def event_stub;

int8_t eventOS_event_handler_create(void (*handler_func_ptr)(arm_event_s *), uint8_t init_event_type)
{
    event_stub.func_ptr = handler_func_ptr;
    return event_stub.int8_value;
}

int8_t eventOS_event_send(const arm_event_s *event)
{
    if( event_stub.func_ptr  && !event_stub.ignore_callback ){
        // Preserve const semantics
        arm_event_s copy = *event;
        event_stub.func_ptr(&copy);
    }
    return event_stub.int8_value;
}

void eventOS_event_send_user_allocated(arm_event_storage_t *event)
{
    if( event_stub.func_ptr  && !event_stub.ignore_callback ){
        event_stub.func_ptr(&event->data);
    }
}

void eventOS_scheduler_init(void)
{
}


int8_t eventOS_scheduler_get_active_tasklet(void)
{
    return event_stub.int8_value;
}

void eventOS_scheduler_set_active_tasklet(int8_t tasklet)
{
}

int eventOS_scheduler_timer_stop(void)
{
    return event_stub.int_value;
}

int eventOS_scheduler_timer_synch_after_sleep(uint32_t sleep_ticks)
{
    return event_stub.int_value;
}

bool eventOS_scheduler_dispatch_event(void)
{
    return event_stub.bool_value;
}

void eventOS_scheduler_run_until_idle(void)
{
}

NS_NORETURN void eventOS_scheduler_run(void)
{
}

void eventOS_cancel(arm_event_storage_t *event)
{
}

arm_event_storage_t *eventOS_event_timer_request_every(const struct arm_event_s *event, int32_t period)
{
    return &event_stub.event_storage;
}


