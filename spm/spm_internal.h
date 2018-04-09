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

#ifndef SPM_INTERNAL_H
#define SPM_INTERNAL_H

#include <stdbool.h>
#include "cmsis_os2.h"
#include "psa_defs.h"
#include "spm_panic.h"
#include "handles_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PARTITION_STATE_ASSERT(partition, partition_state_expected)                                       \
do {                                                                                                      \
    if (partition->partition_state != partition_state_expected) {                                         \
        SPM_PANIC("partition with ID 0x%x is in incorrect processing state: %d while %d is expected!\n",  \
            partition->partition_id, partition->partition_state, partition_state_expected);               \
    }                                                                                                     \
} while (0)

typedef struct partition partition_t;
typedef struct ipc_channel ipc_channel_t;

/*
 * Enumeration for the possible Partition processing states.
 */
typedef enum partition_state {
    PARTITION_STATE_INVALID = 0,
    PARTITION_STATE_IDLE = 1,
    PARTITION_STATE_ACTIVE = 2,
    PARTITION_STATE_COMPLETED = 3
} PartitionState;

/*
 * Structure containing the SPM internal data.
 */
typedef struct spm {
    partition_t *partitions; /* Array of all the Secure Partitions in the system.*/
    uint32_t partition_count; /* Number of Secure Partitions in the system.*/
    psa_handle_manager_t channels_handle_mgr;
    psa_handle_manager_t messages_handle_mgr;
    osMemoryPoolId_t channel_mem_pool; /* Channel memory pool identifier.*/
} spm_t;

/*
 * Structure containing the currently active message for a Secure Function.
 */
typedef struct active_msg {
    ipc_channel_t *channel; /* Pointer to the channel delivering this message.*/
    psa_invec_t in_vec[PSA_MAX_INVEC_LEN]; /* IOvecs sent.*/
    psa_outvec_t out_vec[PSA_MAX_OUTVEC_LEN]; /* IOvecs for response.*/
    psa_error_t rc; /* Return code to be filled by the Secure Function.*/
    SpmMsgType type; /* The message type, one of ::spm_msg_type.*/
} active_msg_t;

/*
 * Structure containing resources and attributes of a Secure Function.
 */
typedef struct secure_func {
    const uint32_t sfid; /* The Secure Function ID.*/
    const uint32_t mask; /* The signal for this Secure function*/
    partition_t *partition; /* Pointer to the Partition which the Secure function belongs to.*/
    const uint32_t min_version; /* Minor version of the Secure Function interface.*/
    const uint32_t min_version_policy; /* Minor version policy of the Secure function.*/
    const bool allow_nspe; /* Whether to allow non-secure clients to connect to the Secure Functions.*/
} secure_func_t;

/*
 * Structure containing resources and attributes of a Secure Partition.
 */
typedef struct partition {
    const int32_t partition_id; /* The Partition ID.*/
    PartitionState partition_state; /* The current processing state of the Partition.*/
    osThreadId_t thread_id; /* Thread ID of the Partition thread.*/
    const uint32_t flags_sf; /* Mask of all the SF signals the partition supports.*/
    const uint32_t flags_interrupts; /* Mask of all the IRQs & doorbell which the partition supports.*/
    secure_func_t *sec_funcs; /* Array of the Partition's Secure Functions.*/
    const uint32_t sec_funcs_count; /* Number of the Partition's Secure Functions.*/
    const uint32_t *extern_sfids; /* Array of Secure Function IDs which the partition can connect to.*/
    const uint32_t extern_sfids_count; /* Number of Secure Functions which the partition can connect to.*/
    osMutexId_t mutex; /* Mutex to serialize client access to the Partition.*/
    osSemaphoreId_t semaphore; /* Semaphore to synchronize client and server.*/
    active_msg_t active_msg; /* The currently active message for the Partition's Secure Function being executed.*/
    psa_handle_t msg_handle; /* Handle for the active_msg, returned by psa_get().*/
} partition_t;

/*
 * Structure containing Partition->Secure-Function channel information.
 */
typedef struct ipc_channel {
    partition_t *src_partition; /* Pointer to the Partition which connects to the Secure function.*/
    secure_func_t *dst_sec_func; /* Pointer to the connected Secure Function.*/
    void *rhandle; /* Reverse handle to be used for this channel.*/
} ipc_channel_t;


/**
 * Returns a pointer to the secure partition struct of the active thread, or NULL for NSPE threads
 */
partition_t *active_partition_get(void);

#ifdef __cplusplus
}
#endif

#endif // SPM_INTERNAL_H
