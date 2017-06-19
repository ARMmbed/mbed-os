/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
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

#include "tz_context.h"
#include <stdint.h>

/* Provide do-nothing stubs for RTX's trustzone context saving hooks. uVisor
 * doesn't use these on ARMv8-M. */

uint32_t TZ_InitContextSystem_S(void) {
    return 1; /* Success */
}

TZ_MemoryId_t TZ_AllocModuleContext_S(TZ_ModuleId_t module) {
    return 1; /* Always hand out slot 1. */
}

uint32_t TZ_FreeModuleContext_S(TZ_MemoryId_t id) {
    return 1; /* Success */
}

uint32_t TZ_LoadContext_S(TZ_MemoryId_t id) {
    return 1; /* Success */
}

uint32_t TZ_StoreContext_S(TZ_MemoryId_t id) {
    return 1; /* Success */
}
