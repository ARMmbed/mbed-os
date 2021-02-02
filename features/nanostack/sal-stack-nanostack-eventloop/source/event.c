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
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "timer_sys.h"
#include "nsdynmemLIB.h"
#include "ns_timer.h"
#include "event.h"
#include "platform/arm_hal_interrupt.h"


typedef struct arm_core_tasklet {
    int8_t id; /**< Event handler Tasklet ID */
    void (*func_ptr)(arm_event_s *);
    ns_list_link_t link;
} arm_core_tasklet_t;

static NS_LIST_DEFINE(arm_core_tasklet_list, arm_core_tasklet_t, link);
static NS_LIST_DEFINE(event_queue_active, arm_event_storage_t, link);
static NS_LIST_DEFINE(free_event_entry, arm_event_storage_t, link);

// Statically allocate initial pool of events.
#define STARTUP_EVENT_POOL_SIZE 10
static arm_event_storage_t startup_event_pool[STARTUP_EVENT_POOL_SIZE];

/** Curr_tasklet tell to core and platform which task_let is active, Core Update this automatic when switch Tasklet. */
int8_t curr_tasklet = 0;


static arm_core_tasklet_t *tasklet_dynamically_allocate(void);
static arm_event_storage_t *event_dynamically_allocate(void);
static arm_event_storage_t *event_core_get(void);
static void event_core_write(arm_event_storage_t *event);

static arm_core_tasklet_t *event_tasklet_handler_get(uint8_t tasklet_id)
{
    ns_list_foreach(arm_core_tasklet_t, cur, &arm_core_tasklet_list) {
        if (cur->id == tasklet_id) {
            return cur;
        }
    }
    return NULL;
}

bool event_tasklet_handler_id_valid(uint8_t tasklet_id)
{
    return event_tasklet_handler_get(tasklet_id);
}

// XXX this can return 0, but 0 seems to mean "none" elsewhere? Or at least
// curr_tasklet is reset to 0 in various places.
static int8_t tasklet_get_free_id(void)
{
    /*(Note use of uint8_t to avoid overflow if we reach 0x7F)*/
    for (uint8_t i = 0; i <= INT8_MAX; i++) {
        if (!event_tasklet_handler_get(i)) {
            return i;
        }
    }
    return -1;
}


int8_t eventOS_event_handler_create(void (*handler_func_ptr)(arm_event_s *), uint8_t init_event_type)
{
    arm_event_storage_t *event_tmp;

    // XXX Do we really want to prevent multiple tasklets with same function?
    ns_list_foreach(arm_core_tasklet_t, cur, &arm_core_tasklet_list) {
        if (cur->func_ptr == handler_func_ptr) {
            return -1;
        }
    }

    //Allocate new
    arm_core_tasklet_t *new = tasklet_dynamically_allocate();
    if (!new) {
        return -2;
    }

    event_tmp = event_core_get();
    if (!event_tmp) {
        ns_dyn_mem_free(new);
        return -2;
    }

    //Fill in tasklet; add to list
    new->id = tasklet_get_free_id();
    new->func_ptr = handler_func_ptr;
    ns_list_add_to_end(&arm_core_tasklet_list, new);

    //Queue "init" event for the new task
    event_tmp->data.receiver = new->id;
    event_tmp->data.sender = 0;
    event_tmp->data.event_type = init_event_type;
    event_tmp->data.event_id = 0;
    event_tmp->data.event_data = 0;
    event_core_write(event_tmp);

    return new->id;
}

int8_t eventOS_event_send(const arm_event_t *event)
{
    if (event_tasklet_handler_get(event->receiver)) {
        arm_event_storage_t *event_tmp = event_core_get();
        if (event_tmp) {
            event_tmp->data = *event;
            event_core_write(event_tmp);
            return 0;
        }
    }
    return -1;
}

void eventOS_event_send_user_allocated(arm_event_storage_t *event)
{
    event->allocator = ARM_LIB_EVENT_USER;
    event_core_write(event);
}

void eventOS_event_send_timer_allocated(arm_event_storage_t *event)
{
    event->allocator = ARM_LIB_EVENT_TIMER;
    event_core_write(event);
}

void eventOS_event_cancel_critical(arm_event_storage_t *event)
{
    ns_list_remove(&event_queue_active, event);
}

static arm_event_storage_t *event_dynamically_allocate(void)
{
    arm_event_storage_t *event = ns_dyn_mem_temporary_alloc(sizeof(arm_event_storage_t));
    if (event) {
        event->allocator = ARM_LIB_EVENT_DYNAMIC;
    }
    return event;
}

static arm_core_tasklet_t *tasklet_dynamically_allocate(void)
{
    return ns_dyn_mem_alloc(sizeof(arm_core_tasklet_t));
}

arm_event_storage_t *event_core_get(void)
{
    arm_event_storage_t *event;
    platform_enter_critical();
    event = ns_list_get_first(&free_event_entry);
    if (event) {
        ns_list_remove(&free_event_entry, event);
    } else {
        event = event_dynamically_allocate();
    }
    if (event) {
        event->data.data_ptr = NULL;
        event->data.priority = ARM_LIB_LOW_PRIORITY_EVENT;
    }
    platform_exit_critical();
    return event;
}

void event_core_free_push(arm_event_storage_t *free)
{
    switch (free->allocator) {
        case ARM_LIB_EVENT_STARTUP_POOL:
            free->state = ARM_LIB_EVENT_UNQUEUED;
            platform_enter_critical();
            ns_list_add_to_start(&free_event_entry, free);
            platform_exit_critical();
            break;
        case ARM_LIB_EVENT_DYNAMIC:
            // Free all dynamically allocated events.
            // No need to set state to UNQUEUED - it's being freed.
            ns_dyn_mem_free(free);
            break;
        case ARM_LIB_EVENT_TIMER:
            // Hand it back to the timer system
            free->state = ARM_LIB_EVENT_UNQUEUED;
            timer_sys_event_free(free);
            break;
        case ARM_LIB_EVENT_USER:
            // *INDENT-OFF*
            // No need set state to UNQUEUED - we forget about it.
            // *INDENT-ON*
        default:
            break;
    }
}


static arm_event_storage_t *event_core_read(void)
{
    platform_enter_critical();
    arm_event_storage_t *event = ns_list_get_first(&event_queue_active);
    if (event) {
        event->state = ARM_LIB_EVENT_RUNNING;
        ns_list_remove(&event_queue_active, event);
    }
    platform_exit_critical();
    return event;
}

void event_core_write(arm_event_storage_t *event)
{
    platform_enter_critical();
    bool added = false;
    ns_list_foreach(arm_event_storage_t, event_tmp, &event_queue_active) {
        // note enum ordering means we're checking if event_tmp is LOWER priority than event
        if (event_tmp->data.priority > event->data.priority) {
            ns_list_add_before(&event_queue_active, event_tmp, event);
            added = true;
            break;
        }
    }
    if (!added) {
        ns_list_add_to_end(&event_queue_active, event);
    }
    event->state = ARM_LIB_EVENT_QUEUED;

    /* Wake From Idle */
    platform_exit_critical();
    eventOS_scheduler_signal();
}

// Requires lock to be held
arm_event_storage_t *eventOS_event_find_by_id_critical(uint8_t tasklet_id, uint8_t event_id)
{
    ns_list_foreach(arm_event_storage_t, cur, &event_queue_active) {
        if (cur->data.receiver == tasklet_id && cur->data.event_id == event_id) {
            return cur;
        }
    }

    return NULL;
}

/**
 *
 * \brief Initialize Nanostack Core.
 *
 * Function Initialize Nanostack Core, Socket Interface,Buffer memory and Send Init event to all Tasklett which are Defined.
 *
 */
void eventOS_scheduler_init(void)
{
    /* Reset Event List variables */
    ns_list_init(&free_event_entry);
    ns_list_init(&event_queue_active);
    ns_list_init(&arm_core_tasklet_list);

    //Add first 10 entries to "free" list
    for (unsigned i = 0; i < (sizeof(startup_event_pool) / sizeof(startup_event_pool[0])); i++) {
        startup_event_pool[i].allocator = ARM_LIB_EVENT_STARTUP_POOL;
        ns_list_add_to_start(&free_event_entry, &startup_event_pool[i]);
    }

    /* Init Generic timer module */
    timer_sys_init();               //initialize timer
    /* Set Tasklett switcher to Idle */
    curr_tasklet = 0;

}

int8_t eventOS_scheduler_get_active_tasklet(void)
{
    return curr_tasklet;
}

void eventOS_scheduler_set_active_tasklet(int8_t tasklet)
{
    curr_tasklet = tasklet;
}

int eventOS_scheduler_timer_stop(void)
{
    timer_sys_disable();
    if (ns_timer_sleep() != 0) {
        return 1;
    }
    return 0;
}

int eventOS_scheduler_timer_synch_after_sleep(uint32_t sleep_ticks)
{
    //Update MS to 10ms ticks
    sleep_ticks /= 10;
    sleep_ticks++;
    system_timer_tick_update(sleep_ticks);
    if (timer_sys_wakeup() == 0) {
        return 0;
    }
    return -1;
}

/**
 *
 * \brief Infinite Event Read Loop.
 *
 * Function Read and handle Cores Event and switch/enable tasklet which are event receiver. WhenEvent queue is empty it goes to sleep
 *
 */
bool eventOS_scheduler_dispatch_event(void)
{
    curr_tasklet = 0;

    arm_event_storage_t *cur_event = event_core_read();
    if (!cur_event) {
        return false;
    }

    curr_tasklet = cur_event->data.receiver;

    arm_core_tasklet_t *tasklet = event_tasklet_handler_get(curr_tasklet);
    /* Do not bother with check for NULL - tasklets cannot be deleted,
     * and user-facing API eventOS_event_send() has already checked the tasklet
     * exists, so there is no possible issue there.
     *
     * For eventOS_event_send_user_allocated(), it would be a non-recoverable
     * error to not deliver the message - we have to have a receiver to pass
     * ownership to. If the lookup fails, let it crash. We want the send call
     * itself to return void to simplify logic.
     */

    /* Tasklet Scheduler Call */
    tasklet->func_ptr(&cur_event->data);
    event_core_free_push(cur_event);

    /* Set Current Tasklet to Idle state */
    curr_tasklet = 0;

    return true;
}

void eventOS_scheduler_run_until_idle(void)
{
    while (eventOS_scheduler_dispatch_event());
}

/**
 *
 * \brief Infinite Event Read Loop.
 *
 * Function Read and handle Cores Event and switch/enable tasklet which are event receiver. WhenEvent queue is empty it goes to sleep
 *
 */
NS_NORETURN void eventOS_scheduler_run(void)
{
    while (1) {
        if (!eventOS_scheduler_dispatch_event()) {
            eventOS_scheduler_idle();
        }
    }
}

void eventOS_cancel(arm_event_storage_t *event)
{
    if (!event) {
        return;
    }

    platform_enter_critical();

    /*
     * Notify timer of cancellation.
     */
    if (event->allocator == ARM_LIB_EVENT_TIMER) {
        timer_sys_event_cancel_critical(event);
    }

    /*
     * Remove event from the list,
     * Only queued can be removed, unqued are either timers or stale pointers
     * RUNNING cannot be removed, we are currenly "in" that event.
     */
    if (event->state == ARM_LIB_EVENT_QUEUED) {
        eventOS_event_cancel_critical(event);
    }

    /*
     * Push back to "free" state
     */
    if (event->state != ARM_LIB_EVENT_RUNNING) {
        event_core_free_push(event);
    }

    platform_exit_critical();
}
