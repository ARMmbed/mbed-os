/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "eventOS_event.h"
#include "eventOS_callback_timer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "platform/arm_hal_interrupt.h"

#define TRACE_GROUP "ctim"

#define PROTOCOL_TIMER_INTERVAL 1000    // 50us units, so we use 50ms

NS_LARGE protocol_timer_t protocol_timer[PROTOCOL_TIMER_MAX];
int8_t protocol_timer_id = -1;
bool protocol_tick_handle_busy = false;
static uint16_t  protocol_tick_update = 0;
int protocol_timer_init(void)
{
    uint8_t i;
    protocol_timer_id = eventOS_callback_timer_register(protocol_timer_interrupt);
    for (i = 0; i < PROTOCOL_TIMER_MAX; i++) {
        protocol_timer[i].ticks = 0;
        protocol_timer[i].time_drifts =0;
    }
    if (protocol_timer_id >= 0) {
        eventOS_callback_timer_start(protocol_timer_id, PROTOCOL_TIMER_INTERVAL);
    }
    return protocol_timer_id;
}

// time is in milliseconds
void protocol_timer_start(protocol_timer_id_t id, void (*passed_fptr)(uint16_t), uint32_t time)
{
    //Check Overflow
    if (passed_fptr) {
        if (time > 0x12FFED) {
            time = 0xffff;
        }
        if (time >= 100) {
            time /= (1000 / 20);
            //time++;
        } else {
            time = 1;
        }
        platform_enter_critical();
        protocol_timer[id].ticks = (uint16_t) time;

        protocol_timer[id].orderedTime = (uint16_t) time;
        if (time > 1 && protocol_timer[id].time_drifts >= 50) {
            protocol_timer[id].ticks--;
            protocol_timer[id].time_drifts -= 50;
        }
        protocol_timer[id].fptr = passed_fptr;
        platform_exit_critical();
    } else {
        tr_debug("Do Not use Null pointer for fptr!!!\n");
    }
}

void protocol_timer_stop(protocol_timer_id_t id)
{
    platform_enter_critical();
    protocol_timer[id].ticks = 0;
    protocol_timer[id].orderedTime = 0;
    platform_exit_critical();
}



void protocol_timer_sleep_balance(uint32_t time_in_ms)
{
    uint8_t i;
    uint16_t ticks_module;
    uint16_t time_in50ms_ticks;
    uint16_t tick_update, tempTimer;

    ticks_module = (time_in_ms % 50);
    time_in50ms_ticks = (time_in_ms / 50);
    for (i = 0; i < PROTOCOL_TIMER_MAX; i++) {
        if (protocol_timer[i].ticks) {

            tick_update = time_in50ms_ticks;
            protocol_timer[i].time_drifts += ticks_module;

            if (protocol_timer[i].time_drifts >= 50) {
                protocol_timer[i].time_drifts -= 50;
            }

            if (protocol_timer[i].ticks <= tick_update) {
                tempTimer = (tick_update - protocol_timer[i].ticks);
                tick_update = 1;
                if (tempTimer >= protocol_timer[i].orderedTime) {
                    tick_update += (tempTimer / protocol_timer[i].orderedTime);
                    //time drift
                    protocol_timer[i].time_drifts += ((tempTimer % protocol_timer[i].orderedTime) *50);
                }

                protocol_timer[i].ticks = 0;
                protocol_timer[i].orderedTime = 0;
                protocol_timer[i].fptr(tick_update);

            } else {
                protocol_timer[i].ticks -= tick_update;
            }
        }
    }

}

void protocol_timer_event_lock_free(void)
{
    platform_enter_critical();
    protocol_tick_handle_busy = false;
    platform_exit_critical();
}


void protocol_timer_cb(uint16_t ticks)
{
    uint8_t i;
    uint16_t tick_update, tempTimer;
    for (i = 0; i < PROTOCOL_TIMER_MAX; i++) {
        if (protocol_timer[i].ticks) {
            if (protocol_timer[i].ticks <= ticks) {
                tempTimer = (ticks - protocol_timer[i].ticks);

                tick_update = 1;
                if (protocol_timer[i].time_drifts >= 50) {
                    tempTimer++;
                    protocol_timer[i].time_drifts -= 50;
                }

                if (tempTimer >= protocol_timer[i].orderedTime) {
                    tick_update += (tempTimer / protocol_timer[i].orderedTime);
                    protocol_timer[i].time_drifts += ((tempTimer % protocol_timer[i].orderedTime) *50);
                }

                protocol_timer[i].ticks = 0;
                protocol_timer[i].orderedTime = 0;
                protocol_timer[i].fptr(tick_update);

            } else {
                protocol_timer[i].ticks -= ticks;
            }
        }
    }
}

void protocol_timer_interrupt(int8_t timer_id, uint16_t slots)
{
    (void)timer_id;
    (void)slots;
    eventOS_callback_timer_start(protocol_timer_id, PROTOCOL_TIMER_INTERVAL);
    protocol_tick_update++;

    if (!protocol_tick_handle_busy) {
        /* This static stuff gets initialised once */
        static arm_event_storage_t event = {
            .data = {
                .data_ptr = NULL,
                .event_type = ARM_IN_PROTOCOL_TIMER_EVENT,
                .event_id = 0,
                .priority = ARM_LIB_HIGH_PRIORITY_EVENT
            }
        };

        /* Dynamic stuff */
        event.data.receiver = event.data.sender = protocol_read_tasklet_id();
        event.data.event_data = protocol_tick_update;
        protocol_tick_update = 0;

        /* Use user-allocated variant to avoid memory allocation failure */
        eventOS_event_send_user_allocated(&event);
        protocol_tick_handle_busy = true;
    }
}
