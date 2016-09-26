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
#include "ns_timer.h"
#include "nsdynmemLIB.h"
#include "eventOS_event.h"
#include "eventOS_callback_timer.h"

#ifndef ST_MAX
#define ST_MAX 6
#endif

typedef struct sys_timer_struct_s {
    uint32_t timer_sys_launch_time;
    int8_t timer_sys_launch_receiver;
    uint8_t timer_sys_launch_message;
    uint8_t timer_event_type;

    ns_list_link_t link;
} sys_timer_struct_s;

#define TIMER_SLOTS_PER_MS          20
#define TIMER_SYS_TICK_PERIOD       10 // milliseconds

static uint32_t run_time_tick_ticks = 0;
static NS_LIST_DEFINE(system_timer_free, sys_timer_struct_s, link);
static NS_LIST_DEFINE(system_timer_list, sys_timer_struct_s, link);


static sys_timer_struct_s *sys_timer_dynamically_allocate(void);
static void timer_sys_interrupt(void);

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
    run_time_tick_ticks = 0;

    // Clear old timers
    ns_list_foreach_safe(sys_timer_struct_s, temp, &system_timer_list) {
        ns_list_remove(&system_timer_list, temp);
        ns_dyn_mem_free(temp);
    }
    // Clear old free timer entrys
    ns_list_foreach_safe(sys_timer_struct_s, temp, &system_timer_free) {
        ns_list_remove(&system_timer_free, temp);
        ns_dyn_mem_free(temp);
    }

    for (uint8_t i = 0; i < ST_MAX; i++) {
        sys_timer_struct_s *temp = sys_timer_dynamically_allocate();
        if (temp) {
            ns_list_add_to_start(&system_timer_free, temp);
        }
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
    return (sys_timer_struct_s*)ns_dyn_mem_alloc(sizeof(sys_timer_struct_s));
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

uint32_t timer_get_runtime_ticks(void)  // only used in dev_stats_internal.c
{
    uint32_t ret_val;
    platform_enter_critical();
    ret_val = run_time_tick_ticks;
    platform_exit_critical();
    return ret_val;
}



int8_t eventOS_event_timer_request(uint8_t snmessage, uint8_t event_type, int8_t tasklet_id, uint32_t time)
{
    int8_t res = -1;
    sys_timer_struct_s *timer = NULL;

    platform_enter_critical();
    // Note that someone wanting 20ms gets 2 ticks, thanks to this test. 30ms would be 4 ticks.
    // And why shouldn't they be able to get a 1-tick callback?
    if (time > 2 * TIMER_SYS_TICK_PERIOD) {
        time /= TIMER_SYS_TICK_PERIOD;
        // XXX Why this? Someone wanting 50ms shouldn't get 6 ticks. Round to nearest, maybe?
        time++;
    } else {
        time = 2;
    }
    timer = timer_struct_get();
    if (timer) {
        timer->timer_sys_launch_message = snmessage;
        timer->timer_sys_launch_receiver = tasklet_id;
        timer->timer_event_type = event_type;
        timer->timer_sys_launch_time = time;
        ns_list_add_to_start(&system_timer_list, timer);
        res = 0;
    }
    platform_exit_critical();
    return res;
}

int8_t eventOS_event_timer_cancel(uint8_t snmessage, int8_t tasklet_id)
{
    int8_t res = -1;
    platform_enter_critical();
    ns_list_foreach(sys_timer_struct_s, cur, &system_timer_list) {
        if (cur->timer_sys_launch_receiver == tasklet_id && cur->timer_sys_launch_message == snmessage) {
            ns_list_remove(&system_timer_list, cur);
            ns_list_add_to_start(&system_timer_free, cur);
            res = 0;
            break;
        }
    }

    platform_exit_critical();
    return res;
}


uint32_t eventOS_event_timer_shortest_active_timer(void)
{
    uint32_t ret_val = 0;

    platform_enter_critical();
    ns_list_foreach(sys_timer_struct_s, cur, &system_timer_list) {
        if (ret_val == 0 || cur->timer_sys_launch_time < ret_val) {
            ret_val = cur->timer_sys_launch_time;
        }
    }

    platform_exit_critical();
    //Convert ticks to ms
    ret_val *= TIMER_SYS_TICK_PERIOD;
    return ret_val;
}

void system_timer_tick_update(uint32_t ticks)
{
    platform_enter_critical();
    //Keep runtime time
    run_time_tick_ticks += ticks;
    ns_list_foreach_safe(sys_timer_struct_s, cur, &system_timer_list) {
        if (cur->timer_sys_launch_time <= ticks) {
            arm_event_s event = {
                .receiver = cur->timer_sys_launch_receiver,
                .sender = 0, /**< Event sender Tasklet ID */
                .data_ptr = NULL,
                .event_type = cur->timer_event_type,
                .event_id = cur->timer_sys_launch_message,
                .event_data = 0,
                .priority = ARM_LIB_MED_PRIORITY_EVENT,
            };
            eventOS_event_send(&event);
            ns_list_remove(&system_timer_list, cur);
            ns_list_add_to_start(&system_timer_free, cur);
        } else {
            cur->timer_sys_launch_time -= ticks;
        }
    }

    platform_exit_critical();
}

