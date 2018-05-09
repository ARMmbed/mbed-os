/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
 * \file protocol_core_sleep.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "eventOS_callback_timer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "common_functions.h"
#include "platform/arm_hal_interrupt.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "sw_mac.h"

#define TRACE_GROUP "pCor"


static void protocol_timer_balance(uint32_t time_in_ms)
{
    protocol_timer_sleep_balance(time_in_ms);
}

static int protocol_stack_interface_disable_poll(protocol_interface_info_entry_t *cur)
{
    int ret_val = -1;
    platform_enter_critical();
    if (cur->if_stack_buffer_handler && cur->rfd_poll_info) {
        if (!cur->rfd_poll_info->pollActive) {
            ret_val = 0;
        }

    }
    platform_exit_critical();
    return ret_val;
}

uint32_t arm_net_check_enter_deep_sleep_possibility(void)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_sleep_possibility();

    if (!cur) {
        return 0;
    }

    //Calculate sleeping time
    uint32_t current_sleep_time = mac_data_poll_get_max_sleep_period(cur);
    if (current_sleep_time) {
        uint32_t alternative_sleep = 0;
        //core_poll_timer_disable();
        current_sleep_time *= 10;
        //Check ND time
        alternative_sleep = nd_object_time_to_next_nd_reg();
        if (alternative_sleep) {
            if (alternative_sleep < current_sleep_time) {
                current_sleep_time = alternative_sleep;
            }
        }
        return current_sleep_time;
    }

    return 0;
}


int arm_net_enter_sleep(void)
{
    int ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_sleep_possibility();
    if (cur) {
        if (protocol_stack_interface_disable_poll(cur) == 0) {
            platform_enter_critical();
            clear_power_state(SLEEP_MODE_REQ);
            platform_exit_critical();
            ret_val = 0;
        }
    }
    return ret_val;
}

int arm_net_wakeup_and_timer_synch(uint32_t sleeped_time_in_ms)
{
    int ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_sleep_possibility();

    if (cur) {
        if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
            //Update MS to 10ms ticks
            //uint32_t sleep_time_in_10ms = (sleeped_time_in_ms + 9) / 10 ;
            //Enable Data Polling after sleep
            //protocol_stack_interface_info_wake_for_polling_interfaces(sleep_time_in_10ms, cur);
            protocol_timer_balance(sleeped_time_in_ms);

        }
    }
    return ret_val;
}
