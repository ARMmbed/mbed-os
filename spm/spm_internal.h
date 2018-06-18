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
#include "cmsis_compiler.h"

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

#define SPM_COMPLETION_SEM_MAX_COUNT (1UL) /* Maximum number of available tokens for a completion semaphore. */
#define SPM_COMPLETION_SEM_INITIAL_COUNT (0UL) /* Initial number of available tokens for a completion semaphore. */

#define PSA_MMIO_PERM_READ_ONLY   (0x000000001)
#define PSA_MMIO_PERM_READ_WRITE  (0x000000003)

#define PSA_RESERVED_ERROR_MIN (INT32_MIN + 1)
#define PSA_RESERVED_ERROR_MAX (INT32_MIN + 127)

typedef struct spm_partition spm_partition_t;
typedef struct spm_ipc_channel spm_ipc_channel_t;

/*
 * IRQ signal mapper definition.
 * The function will not return on invalid signal.
 */
typedef uint32_t (*spm_signal_to_irq_mapper_t)(uint32_t);

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
 * Structure containing the currently active message for a Root of Trust Service.
 */
typedef struct spm_active_msg {
    spm_ipc_channel_t *channel; /* Pointer to the channel delivering this message.*/
    psa_invec_t in_vec[PSA_MAX_INVEC_LEN]; /* IOvecs sent.*/
    psa_outvec_t out_vec[PSA_MAX_OUTVEC_LEN]; /* IOvecs for response.*/
} spm_active_msg_t;


// spm_pending_call_msg struct below is packed since in a dual processor solution
// it is used in both processors
/*
 * Structure containing data sent from NSPE for ROT_SRV call.
 */
typedef __PACKED_STRUCT spm_pending_call_msg {
    psa_handle_t channel; /* Handle pointing to the channel */
    const psa_invec_t *in_vec; /* Invecs sent.*/
    uint32_t in_vec_size; /* Number of Invecs sent.*/
    const psa_outvec_t *out_vec; /* Outvecs for response.*/
    uint32_t out_vec_size; /* Number of Outvecs for response.*/
    psa_error_t rc; /* Return code to be filled by the Root of Trust Service.*/
    osSemaphoreId_t completion_sem_id; /* Semaphore to be released at the end of execution */
} __ALIGNED(4) spm_pending_call_msg_t;

// spm_pending_connect_msg struct below is packed since in a dual processor solution
// it is used in both processors
/*
 * Structure containing data sent from NSPE for connection.
 */
typedef __PACKED_STRUCT spm_pending_connect_msg {
    uint32_t sid; /* The Root of Trust Service ID to be called*/
    uint32_t min_version; /* Minor version of the Root of Trust Service interface.*/
    psa_error_t rc; /* Return code to be filled by the Root of Trust Service.*/
    osSemaphoreId_t completion_sem_id; /* Semaphore to be released at the end of execution */
} __ALIGNED(4) spm_pending_connect_msg_t;

/*
 * Structure to aggregate channels queue in a Root of Trust Service.
 */
typedef struct spm_channel_linked_list {
    spm_ipc_channel_t *head; /* List's first object*/
    spm_ipc_channel_t *tail; /* List's last object*/
} spm_channel_linked_list_t;

/*
 * Structure containing resources and attributes of a Root of Trust Service.
 */
typedef struct spm_rot_service {
    const uint32_t sid; /* The Root of Trust Service ID.*/
    const uint32_t mask; /* The signal for this Root of Trust Service*/
    spm_partition_t *partition; /* Pointer to the Partition which the Root of Trust Service belongs to.*/
    const uint32_t min_version; /* Minor version of the Root of Trust Service interface.*/
    const uint32_t min_version_policy; /* Minor version policy of the Root of Trust Service.*/
    const bool allow_nspe; /* Whether to allow non-secure clients to connect to the Root of Trust Service.*/
    spm_channel_linked_list_t queue; /* Queue of channels holding ROT_SRV operations waiting to be served. */
} spm_rot_service_t;

/*
 * Structure containing Partition->RoT-Service channel information.
 */
typedef struct spm_ipc_channel {
    spm_partition_t *src_partition; /* Pointer to the Partition which connects to the Root of Trust Service.*/
    spm_rot_service_t *dst_rot_service; /* Pointer to the connected Root of Trust Service.*/
    void *rhandle; /* Reverse handle to be used for this channel.*/
    void *msg_ptr; /* message data sent from user */
    struct spm_ipc_channel *next; /* Next channel in the chain  */
    uint8_t msg_type; /* The message type.*/
    ChannelState state; /* The current processing state of the channel.*/
} spm_ipc_channel_t;

/*
 * Structure containing resources and attributes of a Secure Partition.
 */
typedef struct spm_partition {
    const int32_t partition_id; /* The Partition ID.*/
    osThreadId_t thread_id; /* Thread ID of the Partition thread.*/
    const uint32_t flags_rot_srv; /* Mask of all the ROT_SRV signals the partition supports.*/
    const uint32_t flags_interrupts; /* Mask of all the IRQs & doorbell which the partition supports.*/
    spm_rot_service_t *rot_services; /* Array of the Partition's Root of Trust Services.*/
    const uint32_t rot_services_count; /* Number of the Partition's Root of Trust Services.*/
    const uint32_t *extern_sids; /* Array of Root of Trust Service IDs which the partition can connect to.*/
    const uint32_t extern_sids_count; /* Number of Root of Trust Services which the partition can connect to.*/
    osMutexId_t mutex; /* Mutex for all rot_service's queues operations. */
    spm_signal_to_irq_mapper_t irq_mapper; /* a function which maps signal to irq number*/
} spm_partition_t;


/*
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

/*
 * Validates parameters sent from caller and queues a connect message on the correct ROT_SRV.
 *
 * @param[in] msg - pointer to connect message struct
 */
void psa_connect_async(spm_pending_connect_msg_t *msg);

/*
 * Validates parameters sent from caller and queues a call message on the correct ROT_SRV.
 *
 * @param[in] msg - pointer to call message struct
 */
void psa_call_async(spm_pending_call_msg_t *msg);

#ifdef __cplusplus
}
#endif

#endif // SPM_INTERNAL_H
