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

#define CHANNEL_STATE_ASSERT(current_state, expected_state)                                \
do {                                                                                       \
    if (current_state != expected_state) {                                                 \
        SPM_PANIC("channel in incorrect processing state: %d while %d is expected!\n",     \
            current_state, expected_state);                                                \
    }                                                                                      \
} while (0)

typedef struct spm_partition spm_partition_t;
typedef struct spm_ipc_channel spm_ipc_channel_t;

/*
 * Enumeration for the possible channel processing states.
 */
typedef enum partition_state {
    CHANNEL_STATE_INVALID = 0,
    CHANNEL_STATE_CONNECTING = 1,
    CHANNEL_STATE_IDLE = 2,
    CHANNEL_STATE_PENDING = 3,
    CHANNEL_STATE_ACTIVE = 4
} ChannelState;

/*
 * Structure containing the SPM internal data.
 */
typedef struct spm_db {
    spm_partition_t *partitions; /* Array of all the Secure Partitions in the system.*/
    uint32_t partition_count; /* Number of Secure Partitions in the system.*/
    psa_handle_manager_t channels_handle_mgr;
    psa_handle_manager_t messages_handle_mgr;
    osMemoryPoolId_t channel_mem_pool; /* Channel memory pool identifier.*/
    osMemoryPoolId_t active_messages_mem_pool; /* Channel memory pool identifier.*/
} spm_db_t;

/*
 * Structure containing the currently active message for a Secure Function.
 */
typedef struct spm_active_msg {
    spm_ipc_channel_t *channel; /* Pointer to the channel delivering this message.*/
    psa_invec_t in_vec[PSA_MAX_INVEC_LEN]; /* IOvecs sent.*/
    psa_outvec_t out_vec[PSA_MAX_OUTVEC_LEN]; /* IOvecs for response.*/
    psa_error_t rc; /* Return code to be filled by the Secure Function.*/
    SpmMsgType type; /* The message type, one of ::spm_msg_type.*/
} spm_active_msg_t;

/*
 * Structure containing data sent from NSPE for SF call.
 */
typedef struct spm_pending_call_msg {
    psa_handle_t channel; /* Handle pointing to the channel */
    const psa_invec_t *in_vec; /* Invecs sent.*/
    uint32_t in_vec_size; /* Number of Invecs sent.*/
    const psa_outvec_t *out_vec; /* Outvecs for response.*/
    uint32_t out_vec_size; /* Number of Outvecs for response.*/
    psa_error_t rc; /* Return code to be filled by the Secure Function.*/
    osSemaphoreId_t completion_sem_id; /* Semaphore to be released at the end of execution */
} spm_pending_call_msg_t;

/*
 * Structure containing data sent from NSPE for connection.
 */
typedef struct spm_pending_connect_msg {
    uint32_t sfid; /* The Secure Function ID to be called*/
    uint32_t min_version; /* Minor version of the Secure Function interface.*/
    psa_error_t rc; /* Return code to be filled by the Secure Function.*/
    osSemaphoreId_t completion_sem_id; /* Semaphore to be released at the end of execution */
} spm_pending_connect_msg_t;

/*
 * Structure to aggregate channels queue in a Secure function.
 */
typedef struct spm_channel_linked_list {
    spm_ipc_channel_t *head; /* List's first object*/
    spm_ipc_channel_t *tail; /* List's last object*/
} spm_channel_linked_list_t;

/*
 * Structure containing resources and attributes of a Secure Function.
 */
typedef struct spm_secure_func {
    const uint32_t sfid; /* The Secure Function ID.*/
    const uint32_t mask; /* The signal for this Secure function*/
    spm_partition_t *partition; /* Pointer to the Partition which the Secure function belongs to.*/
    const uint32_t min_version; /* Minor version of the Secure Function interface.*/
    const uint32_t min_version_policy; /* Minor version policy of the Secure function.*/
    const bool allow_nspe; /* Whether to allow non-secure clients to connect to the Secure Functions.*/
    spm_channel_linked_list_t queue; /* Queue of channels holding SF operations waiting to be served. */
} spm_secure_func_t;

/*
 * Structure containing Partition->Secure-Function channel information.
 */
typedef struct spm_ipc_channel {
    spm_partition_t *src_partition; /* Pointer to the Partition which connects to the Secure function.*/
    spm_secure_func_t *dst_sec_func; /* Pointer to the connected Secure Function.*/
    void *rhandle; /* Reverse handle to be used for this channel.*/
    void *msg_ptr; /* message data sent from user */
    struct spm_ipc_channel *next; /* Next channel in the chain  */
    SpmMsgType msg_type; /* The message type, one of ::spm_msg_type.*/
    ChannelState state; /* The current processing state of the channel.*/
} spm_ipc_channel_t;

/*
 * Structure containing resources and attributes of a Secure Partition.
 */
typedef struct spm_partition {
    const int32_t partition_id; /* The Partition ID.*/
    osThreadId_t thread_id; /* Thread ID of the Partition thread.*/
    const uint32_t flags_sf; /* Mask of all the SF signals the partition supports.*/
    const uint32_t flags_interrupts; /* Mask of all the IRQs & doorbell which the partition supports.*/
    spm_secure_func_t *sec_funcs; /* Array of the Partition's Secure Functions.*/
    const uint32_t sec_funcs_count; /* Number of the Partition's Secure Functions.*/
    const uint32_t *extern_sfids; /* Array of Secure Function IDs which the partition can connect to.*/
    const uint32_t extern_sfids_count; /* Number of Secure Functions which the partition can connect to.*/
    osMutexId_t mutex; /* Mutex for all secure_function's queues operations. */
} spm_partition_t;


/**
 * Returns a pointer to the secure partition struct of the active thread, or NULL for NSPE threads
 */
spm_partition_t *get_active_partition(void);


// Platform dependent APIs

/**
 * Validates a memory block is accessable from a specific partition
 *
 * @param[in] ptr pointer to the beggining of the memory block.
 * @param[in] size size of the memory block in bytes.
 * @param[in] accessing_partition which partition is trying to access the memory.
 * @return true if the entire memory block is accessable from given partition.
 */
bool is_buffer_accessible(const void *ptr, size_t size, spm_partition_t * accessing_partition);

/**
 * Alerts NSPE that a proccess (connect or call) has ended.
 *
 * @param[in] completion_sem_id semaphore id in NSPE.
 */
void nspe_done(osSemaphoreId_t completion_sem_id);

/**
 * Validates parameters sent from caller and queues a connect message on the correct SF.
 *
 * @param[in] msg - pointer to connect message struct
 */
void psa_connect_async(spm_pending_connect_msg_t *msg);

/**
 * Validates parameters sent from caller and queues a call message on the correct SF.
 *
 * @param[in] msg - pointer to call message struct
 */
void psa_call_async(spm_pending_call_msg_t *msg);

#ifdef __cplusplus
}
#endif

#endif // SPM_INTERNAL_H
