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
#include "ns_timer.h"
#include "nsdynmemLIB.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "minar/minar.h"
#include "mbed.h"
#include "core-util/FunctionPointer.h"
#include "core-util/Event.h"

using minar::Scheduler;
using minar::milliseconds;
using minar::callback_handle_t;
using namespace mbed::util;

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

#define TIMER_SYS_TICK_PERIOD       10 // milliseconds

static uint32_t run_time_tick_ticks = 0;
static NS_LIST_DEFINE(system_timer_free, sys_timer_struct_s, link);
static NS_LIST_DEFINE(system_timer_list, sys_timer_struct_s, link);
static callback_handle_t sys_timer_handle;


static sys_timer_struct_s *sys_timer_dynamically_allocate(void);



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

    Event e = FunctionPointer1<void, uint32_t>(system_timer_tick_update).bind(1);
    sys_timer_handle = Scheduler::postCallback(e).period(milliseconds(TIMER_SYS_TICK_PERIOD)).getHandle();
}



/*-------------------SYSTEM TIMER FUNCTIONS--------------------------*/
void timer_sys_disable(void)
{
    if (sys_timer_handle != NULL) {
        Scheduler::cancelCallback(sys_timer_handle);
        sys_timer_handle = NULL;
    }
}

/*
 * Starts ticking system timer interrupts every 10ms
 */
int8_t timer_sys_wakeup(void)
{
    // postCallback should never fail in MINAR
    // TODO: check if that's true
    if (NULL == sys_timer_handle) {
        Event e = FunctionPointer1<void, uint32_t>(system_timer_tick_update).bind(1);
        sys_timer_handle = Scheduler::postCallback(e).tolerance(minar::milliseconds(TIMER_SYS_TICK_PERIOD/10)).period(milliseconds(TIMER_SYS_TICK_PERIOD)).getHandle();
    }
    return 0;
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
            arm_event_s event;
            event.receiver = cur->timer_sys_launch_receiver;
            event.sender = 0; /**< Event sender Tasklet ID */
            event.data_ptr = NULL;
            event.event_type = cur->timer_event_type;
            event.event_id = cur->timer_sys_launch_message;
            event.event_data = 0;
            event.priority = ARM_LIB_MED_PRIORITY_EVENT;
            eventOS_event_send(&event);
            ns_list_remove(&system_timer_list, cur);
            ns_list_add_to_start(&system_timer_free, cur);
        } else {
            cur->timer_sys_launch_time -= ticks;
        }
    }

    platform_exit_critical();
}

