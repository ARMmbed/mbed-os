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

extern spm_db_t g_spm;
extern psa_handle_item_t g_channels_handle_storage[];
extern psa_handle_item_t g_messages_handle_storage[];
extern spm_ipc_channel_t g_channel_data[];
extern spm_active_msg_t g_active_messages_data[];

void psa_spm_init(void);

void spm_reboot(void)
{
    osStatus_t status;

    for (uint32_t i = 0; i < g_spm.partition_count; ++i) {
        status = osThreadTerminate(g_spm.partitions[i].thread_id);
        MBED_ASSERT(status == osOK);
        status = osMutexDelete(g_spm.partitions[i].mutex);
        MBED_ASSERT(status == osOK);

        for (uint32_t j = 0; j < g_spm.partitions[i].rot_services_count; ++j) {
            g_spm.partitions[i].rot_services[j].partition = NULL;
            g_spm.partitions[i].rot_services[j].queue.head = NULL;
            g_spm.partitions[i].rot_services[j].queue.tail = NULL;
        }

        g_spm.partitions[i].rot_services = NULL;
        g_spm.partitions[i].mutex = NULL;
        g_spm.partitions[i].thread_id = NULL;
    }

    status = osMemoryPoolDelete(g_spm.channel_mem_pool);
    MBED_ASSERT(status == osOK);
    status = osMemoryPoolDelete(g_spm.active_messages_mem_pool);
    MBED_ASSERT(status == osOK);
    memset(g_channels_handle_storage, 0, (sizeof(psa_handle_item_t) * MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS));
    memset(g_messages_handle_storage, 0, (sizeof(psa_handle_item_t) * MBED_CONF_SPM_IPC_MAX_NUM_OF_MESSAGES));
    memset(g_channel_data, 0, (sizeof(spm_ipc_channel_t) * MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS));
    memset(g_active_messages_data, 0, (sizeof(spm_active_msg_t) * MBED_CONF_SPM_IPC_MAX_NUM_OF_MESSAGES));
    memset(&g_spm, 0, sizeof(g_spm));

    g_spm.channels_handle_mgr.handle_generator = PSA_HANDLE_MGR_INVALID_HANDLE;
    g_spm.channels_handle_mgr.pool_size        = MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS;
    g_spm.channels_handle_mgr.handles_pool     = g_channels_handle_storage;
    g_spm.messages_handle_mgr.handle_generator = PSA_HANDLE_MGR_INVALID_HANDLE;
    g_spm.messages_handle_mgr.pool_size        = MBED_CONF_SPM_IPC_MAX_NUM_OF_MESSAGES;
    g_spm.messages_handle_mgr.handles_pool     = g_messages_handle_storage;

    psa_spm_init();
    PSA_UNUSED(status);
}
