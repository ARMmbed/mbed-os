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
 
#include <string.h>
#include "spm_internal.h"

extern spm_t g_spm;
extern psa_handle_item_t g_channels_handle_storage[];
extern psa_handle_item_t g_messages_handle_storage[];
extern ipc_channel_t g_channel_data[];

void psa_spm_init(void);

void spm_reboot(void)
{
    osStatus_t status;

    for (uint32_t i = 0; i < g_spm.partition_count; ++i) {
        status = osThreadTerminate(g_spm.partitions[i].thread_id);
        MBED_ASSERT(status == osOK);
        status = osMutexDelete(g_spm.partitions[i].mutex);
        MBED_ASSERT(status == osOK);
        status = osSemaphoreDelete(g_spm.partitions[i].semaphore);
        MBED_ASSERT(status == osOK);

        g_spm.partitions[i].partition_state = PARTITION_STATE_INVALID;
        g_spm.partitions[i].thread_id = NULL;
        for (uint32_t j = 0; j < g_spm.partitions[i].sec_funcs_count; ++j) {
            g_spm.partitions[i].sec_funcs[j].partition = NULL;
        }

        g_spm.partitions[i].sec_funcs = NULL;
        g_spm.partitions[i].mutex = NULL;
        g_spm.partitions[i].semaphore = NULL;
        memset(&(g_spm.partitions[i].active_msg), 0, sizeof(active_msg_t));
        g_spm.partitions[i].msg_handle = PSA_NULL_HANDLE;
    }

    status = osMemoryPoolDelete(g_spm.channel_mem_pool);
    MBED_ASSERT(status == osOK);
    memset(g_channels_handle_storage, 0, (sizeof(psa_handle_item_t) * MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS));
    memset(g_messages_handle_storage, 0, (sizeof(psa_handle_item_t) * g_spm.partition_count));
    memset(g_channel_data, 0, (sizeof(ipc_channel_t) * MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS));
    memset(&g_spm, 0, sizeof(g_spm));

    psa_spm_init();
    PSA_UNUSED(status);
}
