/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "timer_sys.h"

#define STARTUP_EVENT 0
#define TIMER_EVENT 1

// Timeout structure, already typedefed to timeout_t
struct timeout_entry_t {
    void (*callback)(void *);
    void *arg;
    arm_event_storage_t *event;
};

static int8_t timeout_tasklet_id = -1;

static void timeout_tasklet(arm_event_s *event)
{
    if (TIMER_EVENT != event->event_type) {
        return;
    }

    timeout_t *t = event->data_ptr;
    arm_event_storage_t *storage = t->event;
    sys_timer_struct_s *timer = NS_CONTAINER_OF(storage, sys_timer_struct_s, event);

    t->callback(t->arg);


    // Check if this was periodic timer
    if (timer->period == 0) {
        ns_dyn_mem_free(event->data_ptr);
    }
}

static timeout_t *eventOS_timeout_at_(void (*callback)(void *), void *arg, uint32_t at, uint32_t period)
{
    arm_event_storage_t *storage;

    timeout_t *timeout = ns_dyn_mem_alloc(sizeof(timeout_t));
    if (!timeout) {
        return NULL;
    }
    timeout->callback = callback;
    timeout->arg = arg;

    // Start timeout taskled if it is not running
    if (-1 == timeout_tasklet_id) {
        timeout_tasklet_id = eventOS_event_handler_create(timeout_tasklet, STARTUP_EVENT);
        if (timeout_tasklet_id < 0) {
            timeout_tasklet_id = -1;
            goto FAIL;
        }
    }

    arm_event_t event = {
        .receiver   = timeout_tasklet_id,
        .sender     = timeout_tasklet_id,
        .event_type = TIMER_EVENT,
        .event_id   = TIMER_EVENT,
        .data_ptr   = timeout
    };

    if (period) {
        storage = eventOS_event_timer_request_every(&event, period);
    } else {
        storage = eventOS_event_timer_request_at(&event, at);
    }

    timeout->event = storage;
    if (storage) {
        return timeout;
    }
FAIL:
    ns_dyn_mem_free(timeout);
    return NULL;
}

timeout_t *eventOS_timeout_ms(void (*callback)(void *), uint32_t ms, void *arg)
{
    return eventOS_timeout_at_(callback, arg, eventOS_event_timer_ms_to_ticks(ms) + eventOS_event_timer_ticks(), 0);
}

timeout_t *eventOS_timeout_every_ms(void (*callback)(void *), uint32_t every, void *arg)
{
    return eventOS_timeout_at_(callback, arg, 0, eventOS_event_timer_ms_to_ticks(every));
}

void eventOS_timeout_cancel(timeout_t *t)
{
    if (!t) {
        return;
    }

    eventOS_cancel(t->event);

    // Defer the freeing until returning from the callback
    if (t->event->state != ARM_LIB_EVENT_RUNNING) {
        ns_dyn_mem_free(t);
    }
}
