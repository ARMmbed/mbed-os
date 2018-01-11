/* Copyright (c) 2017 ARM Limited
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

#include "cmsis_os2.h"
#include "psa_defs.h"
#include "spm_internal.h"
#include "spm_panic.h"

extern spm_t g_spm;

partition_t *active_partition_get(void)
{
    osThreadId_t active_thread_id = osThreadGetId();
    SPM_ASSERT(NULL != active_thread_id);

    for (uint32_t i = 0; i < g_spm.partition_count; ++i) {
        if (g_spm.partitions[i].thread_id == active_thread_id) {
            return &(g_spm.partitions[i]);
        }
    }

    return NULL; // Valid in case of NSPE
}
