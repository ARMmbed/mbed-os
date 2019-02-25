/*
 * Copyright (c) 2017-2018, Arm Limited and affiliates.
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

/*
 * Nanostack tasklet to handle FNET services.
 * Module will start tasklet that will drive FNET poll service
 *
 * */

#include "fnet.h"

#include "ns_types.h"
#include "ns_list.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "net_interface.h"

#define NS_FNET_TIMER               0x01
#define NS_FNET_TIMER_INTERVAL_MS   100

typedef struct ns_fnet_events {
    arm_event_storage_t *ns_fnet_timer_storage;
    uint32_t current_time_in_ms;
    int8_t ns_fnet_tasklet_id;
    int8_t receiver_id;
} ns_fnet_events_t;

static ns_fnet_events_t ns_fnet_events = {
    .ns_fnet_tasklet_id = -1,
    .ns_fnet_timer_storage = NULL,
    .receiver_id = -1,
    .current_time_in_ms = 0,
};

ns_fnet_events_t *ns_fnet_events_ptr = &ns_fnet_events;

static void ns_fnet_events_interval_timer_start(int8_t receiver)
{
    struct arm_event_s event = {
        .receiver = receiver,
        .sender = receiver,
        .event_type = ARM_LIB_SYSTEM_TIMER_EVENT,
        .event_id = NS_FNET_TIMER,
        .priority = ARM_LIB_LOW_PRIORITY_EVENT,
    };

    ns_fnet_events_ptr->ns_fnet_timer_storage = eventOS_event_timer_request_every(&event, eventOS_event_timer_ms_to_ticks(NS_FNET_TIMER_INTERVAL_MS));
}

static void ns_fnet_event_handler(arm_event_s *event)
{
    if (event->event_type == ARM_LIB_TASKLET_INIT_EVENT) {
        FNET_DEBUG("NS FNET tasklet initialised");
        ns_fnet_events_ptr->receiver_id = event->receiver;
        ns_fnet_events_interval_timer_start(ns_fnet_events_ptr->receiver_id);
    } else if (event->event_type == ARM_LIB_SYSTEM_TIMER_EVENT) {
        if (event->event_id == NS_FNET_TIMER) {
            ns_fnet_events_ptr->current_time_in_ms += NS_FNET_TIMER_INTERVAL_MS;
            fnet_poll_service();
        }
    }
}

void ns_fnet_events_start(void)
{
    if (ns_fnet_events_ptr->ns_fnet_tasklet_id < 0) {
        ns_fnet_events_ptr->ns_fnet_tasklet_id = eventOS_event_handler_create(&ns_fnet_event_handler, ARM_LIB_TASKLET_INIT_EVENT);
    } else {
        if (ns_fnet_events_ptr->ns_fnet_timer_storage == NULL) {
            ns_fnet_events_interval_timer_start(ns_fnet_events_ptr->receiver_id);
        }
    }
}

void ns_fnet_events_stop(void)
{
    if (ns_fnet_events_ptr->ns_fnet_tasklet_id < 0) {
        return;
    }

    if (ns_fnet_events_ptr->ns_fnet_timer_storage) {
        eventOS_cancel(ns_fnet_events_ptr->ns_fnet_timer_storage);
        ns_fnet_events_ptr->ns_fnet_timer_storage = NULL;
    }
}

void ns_fnet_events_fast_poll(void)
{
    if (ns_fnet_events_ptr->ns_fnet_tasklet_id < 0) {
        return;
    }

    if (ns_fnet_events_ptr->ns_fnet_timer_storage) {
        // scheduler is up and running, poll immediately
        fnet_poll_service();
    }
}


uint32_t ns_fnet_time_in_ms_get(void)
{
    return ns_fnet_events_ptr->current_time_in_ms;
}
