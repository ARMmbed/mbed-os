/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_LIB_HOOK_EXPORTS_H__
#define __UVISOR_API_LIB_HOOK_EXPORTS_H__

#include <stdint.h>

/* Predeclaration */
typedef struct uvisor_semaphore UvisorSemaphore;

/*
 * uVisor library hooks
 *
 * All functions that uVisor needs to call that are implemented in uvisor-lib.
 * These functions will be run by unprivileged code only. */
typedef struct {
    void (*box_init)(void * lib_config);
    int (*semaphore_init)(UvisorSemaphore * semaphore, uint32_t initial_count, uint32_t max_count);
    int (*semaphore_pend)(UvisorSemaphore * semaphore, uint32_t timeout_ms);
} UvisorLibHooks;

#endif
