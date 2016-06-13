/*
 * Copyright (c) 2015-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "uvisor-lib/uvisor-lib.h"
#include "rt_OsEventObserver.h"

#if !(defined(UVISOR_PRESENT) && (UVISOR_PRESENT == 1))

/* Note: This file is not included in the uVisor release library. Instead, the
 *       host OS needs to compile it separately if a platform does not support
 *       uVisor (but uVisor API header files are still used). */

/* uVisor hook for unsupported platforms */
UVISOR_EXTERN void uvisor_init(void)
{
    return;
}

extern RtxBoxIndex * __uvisor_ps;

static void thread_switch(void *context)
{
    if (context == NULL) return;

    /* If the active_heap is NULL, then the process heap needs to be
     * initialized. The initializer sets the active heap itself. */
    if (__uvisor_ps->index.active_heap) {
        __uvisor_ps->index.active_heap = context;
    }
}

static OsEventObserver os_event_observer = {
    .version = 0,
    .pre_start = 0,
    .thread_create = 0,
    .thread_destroy = 0,
    .thread_switch = thread_switch,
};

int uvisor_lib_init(void)
{
    osRegisterForOsEvents(&os_event_observer);
    return 0;
}

#endif
