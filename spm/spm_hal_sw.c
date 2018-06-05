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


// These implementations are meant to be used only for SPM level 0.

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
