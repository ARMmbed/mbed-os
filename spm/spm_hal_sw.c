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

#if SPM_LEVEL == 0

bool is_buffer_accessible(const void *ptr, size_t size, spm_partition_t * accessing_partition)
{
    if (NULL == ptr) {
        return false;
    }

    if (((uintptr_t)ptr + size) < ((uintptr_t)ptr)) {
        return false;
    }

    PSA_UNUSED(accessing_partition);
    return true;
}


void nspe_done(osSemaphoreId_t completion_sem_id)
{
    osStatus_t os_status = osSemaphoreRelease(completion_sem_id);
    SPM_ASSERT(osOK == os_status);
    PSA_UNUSED(os_status);
}

void memory_protection_init(const mem_region_t *regions, uint32_t region_count)
{
    PSA_UNUSED(regions);
    PSA_UNUSED(region_count);
}

#endif // SPM_LEVEL == 0

#if (SPM_LEVEL == 0) || (SPM_LEVEL == 1)

extern spm_db_t g_spm;

spm_partition_t *get_active_partition(void)
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

#endif // (SPM_LEVEL == 0) || (SPM_LEVEL == 1)
