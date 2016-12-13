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

#define STARTUP_EVENT 0
#define TIMER_EVENT 1

// Timeout structure, already typedefed to timeout_t
struct timeout_entry_t {
    void (*callback)(void *);
    void *arg;
    uint8_t event_id;
    ns_list_link_t link;
};

static NS_LIST_HEAD(timeout_t, link) timeout_list = NS_LIST_INIT(timeout_list);
static int8_t timeout_tasklet_id = -1;

static void timeout_tasklet(arm_event_s *event)
{
    if (TIMER_EVENT != event->event_type) {
        return;
    }

    timeout_t *found = NULL;
    ns_list_foreach_safe(timeout_t, cur, &timeout_list) {
        if (cur->event_id == event->event_id) {
            found = cur;
            ns_list_remove(&timeout_list, cur);
            break;
        }
    }

    if (found) {
        found->callback(found->arg);
        ns_dyn_mem_free(found);
    }
}

timeout_t *eventOS_timeout_ms(void (*callback)(void *), uint32_t ms, void *arg)
{
    uint16_t count;
    uint8_t index;
    timeout_t *e = ns_dyn_mem_alloc(sizeof(timeout_t));
    if (!e) {
        return NULL;
    }
    e->callback = callback;
    e->arg = arg;

    // Start timeout taskled if it is not running
    if (-1 == timeout_tasklet_id) {
        timeout_tasklet_id = eventOS_event_handler_create(timeout_tasklet, STARTUP_EVENT);
        if (timeout_tasklet_id < 0) {
            timeout_tasklet_id = -1;
            goto FAIL;
        }
    }

    // Check that we still have indexes left. We have only 8bit timer id.
    count = ns_list_count(&timeout_list);
    if (count >= UINT8_MAX) { // Too big list, timer_id is uint8_t
        goto FAIL;
    }

    // Find next free index
    index = 0;
AGAIN:
    ns_list_foreach(timeout_t, cur, &timeout_list) {
        if (cur->event_id == index) { // This index was used
            index++; // Check next one.
            goto AGAIN; // Start checking from begining of the list, indexes are not in order
        }
    }
    e->event_id = index;
    ns_list_add_to_end(&timeout_list, e);
    eventOS_event_timer_request(index, TIMER_EVENT, timeout_tasklet_id, ms);
    return e;
FAIL:
    ns_dyn_mem_free(e);
    return NULL;
}

void eventOS_timeout_cancel(timeout_t *t)
{
    ns_list_foreach_safe(timeout_t, cur, &timeout_list) {
        if (t == cur) {
            ns_list_remove(&timeout_list, cur);
            eventOS_event_timer_cancel(cur->event_id, timeout_tasklet_id);
            ns_dyn_mem_free(cur);
        }
    }
}
