/*
 * Copyright (c) 2015-2019 ARM Limited. All rights reserved.
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

#include <stdlib.h>
#include "eventOS_scheduler.h"
#include "eventOS_event.h"
#include "net_interface.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "platform/arm_hal_timer.h"
#include "ns_hal_init.h"
#include "include/mesh_system.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "ns_file_system.h"
#include "ns_time_api.h"
// For tracing we need to define flag, have include and define group
#define HAVE_DEBUG 1
#include "ns_trace.h"
#define TRACE_GROUP  "m6-mesh-system"

/* Heap for NanoStack */
#if !MBED_CONF_MBED_MESH_API_USE_MALLOC_FOR_HEAP
static uint8_t app_stack_heap[MBED_CONF_MBED_MESH_API_HEAP_SIZE + 1];
#else
static uint8_t *app_stack_heap;
#endif
static bool mesh_initialized = false;
#ifdef MBED_CONF_MBED_MESH_API_HEAP_STAT_INFO_DEFINITION
MBED_CONF_MBED_MESH_API_HEAP_STAT_INFO_DEFINITION;
#endif

/*
 * Heap error handler, called when heap problem is detected.
 * Function is for-ever loop.
 */
static void mesh_system_heap_error_handler(heap_fail_t event)
{
    MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STACK, MBED_ERROR_CODE_OUT_OF_MEMORY), "Heap error, mesh_system_heap_error_handler()", event);
}

void mesh_system_init(void)
{
    if (mesh_initialized == false) {
#if MBED_CONF_MBED_MESH_API_USE_MALLOC_FOR_HEAP
        app_stack_heap = malloc(MBED_CONF_MBED_MESH_API_HEAP_SIZE + 1);
        MBED_ASSERT(app_stack_heap);
#endif
        ns_hal_init(app_stack_heap, MBED_CONF_MBED_MESH_API_HEAP_SIZE,
                    mesh_system_heap_error_handler, MBED_CONF_MBED_MESH_API_HEAP_STAT_INFO);
        eventOS_scheduler_mutex_wait();
        net_init_core();
        eventOS_scheduler_mutex_release();
        mesh_initialized = true;
    }
}

void mesh_system_send_connect_event(uint8_t receiver)
{
    arm_event_s event = {
        .sender =  receiver,
        .event_id = APPL_EVENT_CONNECT,
        .receiver = receiver,
        .event_type = APPLICATION_EVENT,
        .priority = ARM_LIB_LOW_PRIORITY_EVENT,
    };
    eventOS_event_send(&event);
}

int mesh_system_set_file_system_root_path(const char *root_path)
{
    return ns_file_system_set_root_path(root_path);
}

void mesh_system_time_callback_set(ns_time_read_cb read_cb, ns_time_write_cb write_cb)
{
    ns_time_api_system_time_callback_set(read_cb);
    ns_time_api_system_time_write_callback_set(write_cb);
}
