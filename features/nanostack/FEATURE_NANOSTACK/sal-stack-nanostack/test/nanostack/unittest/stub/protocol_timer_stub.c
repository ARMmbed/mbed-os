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


int protocol_timer_init(void)
{
    return 0;
}

void protocol_timer_start(protocol_timer_id_t id, void (*passed_fptr)(uint16_t), uint32_t time)
{
}

void protocol_timer_stop(protocol_timer_id_t id)
{
}



void protocol_timer_sleep_balance(uint32_t time_in_ms)
{
}

void protocol_timer_event_lock_free(void)
{
}


void protocol_timer_cb(uint16_t ticks)
{
}

void protocol_timer_interrupt(int8_t timer_id, uint16_t slots)
{
}
