/* Copyright (c) 2017-2018 ARM Limited
 *
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

#include "mbed_assert.h"
#include "rtx_os.h"
#include "spm_internal.h"
#include "handles_manager.h"
#include "spm_api.h"

MBED_STATIC_ASSERT(
    MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS <= PSA_HANDLE_MGR_MAX_HANDLES_NUM,
    "Number of channels exceeds maximum number of handles allowed in handles manager!"
);

MBED_STATIC_ASSERT(
    MBED_CONF_SPM_IPC_MAX_NUM_OF_MESSAGES <= PSA_HANDLE_MGR_MAX_HANDLES_NUM,
    "Number of active messages exceeds maximum number of handles allowed in handles manager!"
);


// Sanity check - memory ranges are well formed

MBED_STATIC_ASSERT(
    (PSA_SECURE_RAM_START + PSA_SECURE_RAM_SIZE - 1) > PSA_SECURE_RAM_START,
    "Illegal secure ram region!"
);

MBED_STATIC_ASSERT(
    (PSA_SECURE_ROM_START + PSA_SECURE_ROM_SIZE - 1) > PSA_SECURE_ROM_START,
    "Illegal secure flash region!"
);

MBED_STATIC_ASSERT(
    (PSA_NON_SECURE_RAM_START + PSA_NON_SECURE_RAM_SIZE - 1) > PSA_NON_SECURE_RAM_START,
    "Illegal non-secure ram region!"
);

MBED_STATIC_ASSERT(
    (PSA_NON_SECURE_ROM_START + PSA_NON_SECURE_ROM_SIZE - 1) > PSA_NON_SECURE_ROM_START,
    "Illegal non-secure flash region!"
);

MBED_STATIC_ASSERT(
    (PSA_SHARED_RAM_START + PSA_SHARED_RAM_SIZE - 1) > PSA_SHARED_RAM_START,
    "Illegal shared ram region!"
);


// Sanity check - memory ranges are not overlapping

#ifndef MIN
#define MIN( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif

#ifndef MAX
#define MAX( a, b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#endif

#define OVERLAP_CHECK( name1, start1, size1, name2, start2, size2) \
    MBED_STATIC_ASSERT( \
        MAX(start1, start2) >= MIN((start1 + size1), (start2 + size2)), \
        name1 " and " name2 " are overlapping!!!" \
    )

OVERLAP_CHECK(
    "PSA_SECURE_RAM",     PSA_SECURE_RAM_START,     PSA_SECURE_RAM_SIZE,
    "PSA_NON_SECURE_RAM", PSA_NON_SECURE_RAM_START, PSA_NON_SECURE_RAM_SIZE
);

OVERLAP_CHECK(
    "PSA_SECURE_RAM", PSA_SECURE_RAM_START, PSA_SECURE_RAM_SIZE,
    "PSA_SHARED_RAM", PSA_SHARED_RAM_START, PSA_SHARED_RAM_SIZE
);

OVERLAP_CHECK(
    "PSA_SHARED_RAM",     PSA_SHARED_RAM_START,     PSA_SHARED_RAM_SIZE,
    "PSA_NON_SECURE_RAM", PSA_NON_SECURE_RAM_START, PSA_NON_SECURE_RAM_SIZE
);

OVERLAP_CHECK(
    "PSA_SECURE_FLASH",     PSA_SECURE_ROM_START, PSA_SECURE_ROM_SIZE,
    "PSA_NON_SECURE_FLASH", PSA_NON_SECURE_ROM_START, PSA_NON_SECURE_ROM_SIZE
);


psa_handle_item_t g_channels_handle_storage[MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS] = {0};
spm_ipc_channel_t g_channel_data[MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS] = {0};
osRtxMemoryPool_t g_channel_mem_pool_storage = {0};
osMemoryPoolAttr_t g_channel_mem_pool_attr = {
    .name = "SPM_channel_pool",
    .attr_bits = 0,
    .cb_mem = &g_channel_mem_pool_storage,
    .cb_size = sizeof(g_channel_mem_pool_storage),
    .mp_mem = g_channel_data,
    .mp_size = sizeof(g_channel_data)
};

psa_handle_item_t g_messages_handle_storage[MBED_CONF_SPM_IPC_MAX_NUM_OF_MESSAGES] = {0};
spm_active_msg_t g_active_messages_data[MBED_CONF_SPM_IPC_MAX_NUM_OF_MESSAGES] = {0};
osRtxMemoryPool_t g_active_messages_mem_pool_storage = {0};
osMemoryPoolAttr_t g_active_messages_mem_pool_attr = {
    .name = "SPM_active_messages_pool",
    .attr_bits = 0,
    .cb_mem = &g_active_messages_mem_pool_storage,
    .cb_size = sizeof(g_active_messages_mem_pool_storage),
    .mp_mem = g_active_messages_data,
    .mp_size = sizeof(g_active_messages_data)
};

spm_db_t g_spm = {
    .partitions = NULL,
    .partition_count = 0,
    .channels_handle_mgr = {
        .handle_generator = PSA_HANDLE_MGR_INVALID_HANDLE,
        .pool_size        = MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS,
        .handles_pool     = g_channels_handle_storage
    },
    .messages_handle_mgr = {
        .handle_generator = PSA_HANDLE_MGR_INVALID_HANDLE,
        .pool_size        = MBED_CONF_SPM_IPC_MAX_NUM_OF_MESSAGES,
        .handles_pool     = g_messages_handle_storage
    },
    .channel_mem_pool = NULL,
    .active_messages_mem_pool = NULL
};

// forward declaration
uint32_t init_partitions(spm_partition_t **partitions);

void psa_spm_init(void)
{
    spm_hal_memory_protection_init();

    g_spm.channel_mem_pool = osMemoryPoolNew(
                                 MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS,
                                 sizeof(spm_ipc_channel_t),
                                 &g_channel_mem_pool_attr
                             );
    if (NULL == g_spm.channel_mem_pool) {
        error("%s - Failed to create channel memory pool!\n", __func__);
    }

    g_spm.active_messages_mem_pool = osMemoryPoolNew(
                                         MBED_CONF_SPM_IPC_MAX_NUM_OF_MESSAGES,
                                         sizeof(spm_active_msg_t),
                                         &g_active_messages_mem_pool_attr
                                     );
    if (NULL == g_spm.active_messages_mem_pool) {
        error("%s - Failed to create active messages memory pool!\n", __func__);
    }

    g_spm.partition_count = init_partitions(&(g_spm.partitions));
}
