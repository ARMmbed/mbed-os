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
#include "ns_types.h"
#include "ns_list.h"
#include "ns_timer.h"
#include "ns_timer_stub.h"
#include "eventOS_callback_timer.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/arm_hal_timer.h"
#include "nsdynmemLIB.h"

ns_timer_stub_def ns_timer_stub;

int8_t ns_timer_init(void)
{
    return ns_timer_stub.int8_value;
}

int8_t eventOS_callback_timer_register(void (*timer_interrupt_handler)(int8_t, uint16_t))
{
    ns_timer_stub.cb = timer_interrupt_handler;
    return ns_timer_stub.int8_value;
}

int8_t eventOS_callback_timer_unregister(int8_t ns_timer_id)
{
    return ns_timer_stub.int8_value;
}


int8_t ns_timer_sleep(void)
{
    return ns_timer_stub.int8_value;
}

int8_t eventOS_callback_timer_start(int8_t ns_timer_id, uint16_t slots)
{
    return ns_timer_stub.int8_value;
}

int8_t eventOS_callback_timer_stop(int8_t ns_timer_id)
{
    return ns_timer_stub.int8_value;
}
