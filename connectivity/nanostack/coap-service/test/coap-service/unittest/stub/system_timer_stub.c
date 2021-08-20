/*
 * Copyright (c) 2014-2017, Pelion and affiliates.
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
#include "ns_types.h"
#include "timer_sys.h"
#include "platform/arm_hal_interrupt.h"
#include "ns_timer.h"
#include "nsdynmemLIB.h"
#include "eventOS_event.h"
#include "eventOS_callback_timer.h"

#ifndef ST_MAX
#define ST_MAX 6
#endif

#define TIMER_SYS_TICK_PERIOD 100

void timer_sys_init(void)
{
}

void timer_sys_disable(void)
{

}

int8_t timer_sys_wakeup(void)
{
    return 0;
}

uint32_t timer_get_runtime_ticks(void)  // only used in dev_stats_internal.c
{
    return 0;
}



int8_t eventOS_event_timer_request(uint8_t snmessage, uint8_t event_type, int8_t tasklet_id, uint32_t time)
{
    return -1;
}

int8_t eventOS_event_timer_cancel(uint8_t snmessage, int8_t tasklet_id)
{
    return -1;
}


uint32_t eventOS_event_timer_shortest_active_timer(void)
{
    return 0;
}

void system_timer_tick_update(uint32_t ticks)
{
}

