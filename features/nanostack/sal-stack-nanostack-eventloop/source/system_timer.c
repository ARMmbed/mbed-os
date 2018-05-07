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
#include "ns_types.h"
#include "ns_list.h"
#include "timer_sys.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/arm_hal_timer.h"
#include "nsdynmemLIB.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "event.h"
#include "eventOS_callback_timer.h"

#include "ns_timer.h"

#ifndef ST_MAX
#define ST_MAX 6
#endif

static sys_timer_struct_s startup_sys_timer_pool[ST_MAX];

#define TIMER_SLOTS_PER_MS          20
NS_STATIC_ASSERT(1000 % EVENTOS_EVENT_TIMER_HZ == 0, "Need whole number of ms per tick")
#define TIMER_SYS_TICK_PERIOD       (1000 / EVENTOS_EVENT_TIMER_HZ) // milliseconds

// timer_sys_ticks must be read in critical section to guarantee
// atomicity on 16-bit platforms
static volatile uint32_t timer_sys_ticks;

static NS_LIST_DEFINE(system_timer_free, sys_timer_struct_s, event.link);
static NS_LIST_DEFINE(system_timer_list, sys_timer_struct_s, event.link);


static sys_timer_struct_s *sys_timer_dynamically_allocate(void);
static void timer_sys_interrupt(void);
static void timer_sys_add(sys_timer_struct_s *timer);

#ifndef NS_EVENTLOOP_USE_TICK_TIMER
static int8_t platform_tick_timer_start(uint32_t period_ms);
/* Implement platform tick timer using eventOS timer */
// platform tick timer callback function
static void (*tick_timer_callback)(void);
static int8_t tick_timer_id = -1;	// eventOS timer id for tick timer

// EventOS timer callback function
static void tick_timer_eventOS_callback(int8_t timer_id, uint16_t slots)
{
    // Not interested in timer id or slots
    (void)slots;
    // Call the tick timer callback
    if (tick_timer_callback != NULL && timer_id == tick_timer_id) {
        platform_tick_timer_start(TIMER_SYS_TICK_PERIOD);
        tick_timer_callback();
    }
}

static int8_t platform_tick_timer_register(void (*tick_timer_cb)(void))
{
    tick_timer_callback = tick_timer_cb;
    tick_timer_id = eventOS_callback_timer_register(tick_timer_eventOS_callback);
    return tick_timer_id;
}

static int8_t platform_tick_timer_start(uint32_t period_ms)
{
    return eventOS_callback_timer_start(tick_timer_id, TIMER_SLOTS_PER_MS * period_ms);
}

static int8_t platform_tick_timer_stop(void)
{
    return eventOS_callback_timer_stop(tick_timer_id);
}
#endif // !NS_EVENTLOOP_USE_TICK_TIMER

/*
 * Initializes timers and starts system timer
 */
void timer_sys_init(void)
{
    for (uint8_t i = 0; i < ST_MAX; i++) {
        ns_list_add_to_start(&system_timer_free, &startup_sys_timer_pool[i]);
    }

    platform_tick_timer_register(timer_sys_interrupt);
    platform_tick_timer_start(TIMER_SYS_TICK_PERIOD);
}



/*-------------------SYSTEM TIMER FUNCTIONS--------------------------*/
void timer_sys_disable(void)
{
    platform_tick_timer_stop();
}

/*
 * Starts ticking system timer interrupts every 10ms
 */
int8_t timer_sys_wakeup(void)
{
    return platform_tick_timer_start(TIMER_SYS_TICK_PERIOD);
}


static void timer_sys_interrupt(void)
{
    system_timer_tick_update(1);
}



/* * * * * * * * * */

static sys_timer_struct_s *sys_timer_dynamically_allocate(void)
{
    return ns_dyn_mem_alloc(sizeof(sys_timer_struct_s));
}

static sys_timer_struct_s *timer_struct_get(void)
{
    sys_timer_struct_s *timer;
    platform_enter_critical();
    timer = ns_list_get_first(&system_timer_free);
    if (timer) {
        ns_list_remove(&system_timer_free, timer);
    } else {
        timer = sys_timer_dynamically_allocate();
    }
    platform_exit_critical();
    return timer;
}

void timer_sys_event_free(arm_event_storage_t *event)
{
    platform_enter_critical();
    sys_timer_struct_s *timer = NS_CONTAINER_OF(event, sys_timer_struct_s, event);
    if (timer->period == 0) {
        // Non-periodic - return to free list
        ns_list_add_to_start(&system_timer_free, timer);
    } else {
        // Periodic - check due time of next launch
        timer->launch_time += timer->period;
        if (TICKS_BEFORE_OR_AT(timer->launch_time, timer_sys_ticks)) {
            // next event is overdue - queue event now
            eventOS_event_send_timer_allocated(&timer->event);
        } else {
            // add back to timer queue for the future
            timer_sys_add(timer);
        }
    }
    platform_exit_critical();
}

void timer_sys_event_cancel_critical(struct arm_event_storage *event)
{
    sys_timer_struct_s *timer = NS_CONTAINER_OF(event, sys_timer_struct_s, event);
    timer->period = 0;
    // If its unqueued it is on my timer list, otherwise it is in event-loop.
    if (event->state == ARM_LIB_EVENT_UNQUEUED) {
        ns_list_remove(&system_timer_list, timer);
    }
}

uint32_t eventOS_event_timer_ticks(void)
{
    uint32_t ret_val;
    // Enter/exit critical is a bit clunky, but necessary on 16-bit platforms,
    // which won't be able to do an atomic 32-bit read.
    platform_enter_critical();
    ret_val = timer_sys_ticks;
    platform_exit_critical();
    return ret_val;
}

/* Called internally with lock held */
static void timer_sys_add(sys_timer_struct_s *timer)
{
    uint32_t at = timer->launch_time;

    // Find first timer scheduled to run after us, and insert before it.
    // (This means timers scheduled for same time run in order of request)
    ns_list_foreach(sys_timer_struct_s, t, &system_timer_list) {
        if (TICKS_BEFORE(at, t->launch_time)) {
            ns_list_add_before(&system_timer_list, t, timer);
            return;
        }
    }

    // Didn't insert before another timer, so must be last.
    ns_list_add_to_end(&system_timer_list, timer);
}

/* Called internally with lock held */
static arm_event_storage_t *eventOS_event_timer_request_at_(const arm_event_t *event, uint32_t at, uint32_t period)
{
    // Because we use user-allocated events, they must get delivered to avoid
    // a leak. Previously this call queued timers for invalid tasks, then they
    // would go undelivered. Now it returns an error.
    if (!event_tasklet_handler_id_valid(event->receiver)) {
        return NULL;
    }

    sys_timer_struct_s *timer = timer_struct_get();
    if (!timer) {
        return NULL;
    }

    timer->event.data = *event;
    timer->event.allocator = ARM_LIB_EVENT_TIMER;
    timer->event.state = ARM_LIB_EVENT_UNQUEUED;
    timer->launch_time = at;
    timer->period = period;

    if (TICKS_BEFORE_OR_AT(at, timer_sys_ticks)) {
        eventOS_event_send_timer_allocated(&timer->event);
    } else {
        timer_sys_add(timer);
    }

    return &timer->event;
}

arm_event_storage_t *eventOS_event_timer_request_at(const arm_event_t *event, uint32_t at)
{
    platform_enter_critical();

    arm_event_storage_t *ret = eventOS_event_timer_request_at_(event, at, 0);

    platform_exit_critical();

    return ret;
}

arm_event_storage_t *eventOS_event_timer_request_in(const arm_event_t *event, int32_t in)
{
    platform_enter_critical();

    arm_event_storage_t *ret = eventOS_event_timer_request_at_(event, timer_sys_ticks + in, 0);

    platform_exit_critical();

    return ret;

}

arm_event_storage_t *eventOS_event_timer_request_every(const arm_event_t *event, int32_t period)
{
    if (period <= 0) {
        return NULL;
    }

    platform_enter_critical();

    arm_event_storage_t *ret = eventOS_event_timer_request_at_(event, timer_sys_ticks + period, period);

    platform_exit_critical();

    return ret;

}

int8_t eventOS_event_timer_request(uint8_t event_id, uint8_t event_type, int8_t tasklet_id, uint32_t time)
{
    const arm_event_t event = {
        .event_id = event_id,
        .event_type = event_type,
        .receiver = tasklet_id,
        .sender = 0,
        .data_ptr = NULL,
        .event_data = 0,
        .priority = ARM_LIB_MED_PRIORITY_EVENT,
    };

    // Legacy time behaviour preserved

    // Note that someone wanting 20ms gets 2 ticks, thanks to this test. 30ms would be 4 ticks.
    // And why shouldn't they be able to get a 1-tick callback?
    if (time > 2 * TIMER_SYS_TICK_PERIOD) {
        time /= TIMER_SYS_TICK_PERIOD;
        // XXX Why this? Someone wanting 50ms shouldn't get 6 ticks. Round to nearest, maybe?
        time++;
    } else {
        time = 2;
    }

    platform_enter_critical();
    arm_event_storage_t *ret = eventOS_event_timer_request_at_(&event, timer_sys_ticks + time, 0);
    platform_exit_critical();
    return ret?0:-1;
}

int8_t eventOS_event_timer_cancel(uint8_t event_id, int8_t tasklet_id)
{
    platform_enter_critical();

    /* First check pending timers */
    ns_list_foreach(sys_timer_struct_s, cur, &system_timer_list) {
        if (cur->event.data.receiver == tasklet_id && cur->event.data.event_id == event_id) {
            eventOS_cancel(&cur->event);
            goto done;
        }
    }

    /* No pending timer, so check for already-pending event */
    arm_event_storage_t *event = eventOS_event_find_by_id_critical(tasklet_id, event_id);
    if (event && event->allocator == ARM_LIB_EVENT_TIMER) {
        eventOS_cancel(event);
        goto done;
    }

    /* No match found */
    platform_exit_critical();
    return -1;

done:
    platform_exit_critical();
    return 0;
}

uint32_t eventOS_event_timer_shortest_active_timer(void)
{
    uint32_t ret_val = 0;

    platform_enter_critical();
    sys_timer_struct_s *first = ns_list_get_first(&system_timer_list);
    if (first == NULL) {
        // Weird API has 0 for "no events"
        ret_val = 0;
    } else if (TICKS_BEFORE_OR_AT(first->launch_time, timer_sys_ticks)) {
        // Which means an immediate/overdue event has to be 1
        ret_val = 1;
    } else {
        ret_val = first->launch_time - timer_sys_ticks;
    }

    platform_exit_critical();
    return eventOS_event_timer_ticks_to_ms(ret_val);
}

void system_timer_tick_update(uint32_t ticks)
{
    platform_enter_critical();
    //Keep runtime time
    timer_sys_ticks += ticks;
    ns_list_foreach_safe(sys_timer_struct_s, cur, &system_timer_list) {
        if (TICKS_BEFORE_OR_AT(cur->launch_time, timer_sys_ticks)) {
            // Unthread from our list
            ns_list_remove(&system_timer_list, cur);
            // Make it an event (can't fail - no allocation)
            // event system will call our timer_sys_event_free on event delivery.
            eventOS_event_send_timer_allocated(&cur->event);
        } else {
            // List is ordered, so as soon as we see a later event, we're done.
            break;
        }
    }

    platform_exit_critical();
}

