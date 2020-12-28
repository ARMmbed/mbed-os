/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#include "eventOS_scheduler.h"

#include "ns_event_loop_mutex.h"

#ifdef MBED_CONF_RTOS_PRESENT
#include "mbed_assert.h"
#include "cmsis.h"
#include "cmsis_os2.h"
#include "mbed_rtos_storage.h"
#include "ns_trace.h"


#define TRACE_GROUP "evlm"

static mbed_rtos_storage_mutex_t event_mutex;
static const osMutexAttr_t event_mutex_attr = {
    .name = "nanostack_event_mutex",
    .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
    .cb_mem = &event_mutex,
    .cb_size = sizeof event_mutex,
};
static osMutexId_t event_mutex_id;
static osThreadId_t event_mutex_owner_id = NULL;
static uint32_t owner_count = 0;

void eventOS_scheduler_mutex_wait(void)
{
    osMutexAcquire(event_mutex_id, osWaitForever);
    if (0 == owner_count) {
        event_mutex_owner_id = osThreadGetId();
    }
    owner_count++;
}

void eventOS_scheduler_mutex_release(void)
{
    owner_count--;
    if (0 == owner_count) {
        event_mutex_owner_id = NULL;
    }
    osMutexRelease(event_mutex_id);
}

uint8_t eventOS_scheduler_mutex_is_owner(void)
{
    return osThreadGetId() == event_mutex_owner_id ? 1 : 0;
}

void ns_event_loop_mutex_init(void)
{
    event_mutex_id = osMutexNew(&event_mutex_attr);
    MBED_ASSERT(event_mutex_id != NULL);
}

#else

void eventOS_scheduler_mutex_wait(void)
{
}

void eventOS_scheduler_mutex_release(void)
{
}

uint8_t eventOS_scheduler_mutex_is_owner(void)
{
    return 1;
}

void ns_event_loop_mutex_init(void)
{
}

#endif // MBED_CONF_RTOS_PRESENT
