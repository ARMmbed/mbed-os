/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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
#include <stdlib.h>
#include <assert.h>

#include "eventOS_scheduler.h"
#include "ns_event_loop.h"
#include "randLIB.h"
#include "platform/arm_hal_timer.h"
#include "ns_trace.h"

#include "arm_hal_interrupt_private.h"
#include "ns_hal_init.h"

void ns_hal_init(void *heap, size_t h_size, void (*passed_fptr)(heap_fail_t), mem_stat_t *info_ptr)
{
    static bool initted;
    if (initted) {
        return;
    }
    if (!heap) {
        heap = malloc(h_size);
        assert(heap);
        if (!heap) {
            return;
        }
    }
    platform_critical_init();
    ns_dyn_mem_init(heap, h_size, passed_fptr, info_ptr);
#ifndef NS_EXCLUDE_HIGHRES_TIMER
    platform_timer_enable();
#endif
    eventOS_scheduler_init();

    // We do not initialise randlib, as it should be done after
    // RF driver has started, to get MAC address and RF noise as seed.
    // We do not initialise trace - left to application.

    // Prepare the event loop lock which is used even if the loop
    // is not ran in a separate thread.
    ns_event_loop_init();

#if !MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_DISPATCH_FROM_APPLICATION
    ns_event_loop_thread_create();
    ns_event_loop_thread_start();
#endif

    initted = true;
}
