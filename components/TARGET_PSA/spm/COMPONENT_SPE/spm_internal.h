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

#ifndef SPM_INTERNAL_H
#define SPM_INTERNAL_H

#include <stdbool.h>
#include "cmsis_os2.h"
#include "cmsis.h"
#include "psa_defs.h"
#include "spm_messages.h"
#include "spm_panic.h"
#include "handles_manager.h"
#include "cmsis_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPM_COMPLETION_SEM_MAX_COUNT (1UL) /* Maximum number of available tokens for a completion semaphore. */
#define SPM_COMPLETION_SEM_INITIAL_COUNT (0UL) /* Initial number of available tokens for a completion semaphore. */

#define PSA_MMIO_PERM_READ_ONLY   (0x000000001)
#define PSA_MMIO_PERM_READ_WRITE  (0x000000003)

#define PSA_RESERVED_ERROR_MIN (INT32_MIN + 1)
#define PSA_RESERVED_ERROR_MAX (INT32_MIN + 127)

#define SPM_CHANNEL_STATE_INVALID          (0x01)
#define SPM_CHANNEL_STATE_CONNECTING       (0x02)
#define SPM_CHANNEL_STATE_IDLE             (0x03)
#define SPM_CHANNEL_STATE_PENDING          (0x04)
#define SPM_CHANNEL_STATE_ACTIVE           (0x05)

#define MEM_PARTITIONS_ALL (0) /* A constant to use to retrieve the memory regions for all the partitions at once. */

#define SPM_CMSIS_RTOS_ERROR_BIT_MSK (0x80000000)

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

struct spm_partition;
struct spm_ipc_channel;

/*
 * Structure to describe MMIO region along with owning partition.
 */
typedef struct mem_region {
    const uint32_t base;
    const uint32_t size;
    const uint32_t permission;
    const int32_t partition_id;
} mem_region_t;

typedef union spm_iovec {
    psa_invec_t in;
    psa_outvec_t out;
} spm_iovec_t;

/*
 * IRQ signal mapper definition.
 * The function will not return on invalid signal.
 */
typedef IRQn_Type(*spm_signal_to_irq_mapper_t)(uint32_t);

/*
 * Structure to aggregate channels queue in a Root of Trust Service.
 */
typedef struct spm_channel_linked_list {
    struct spm_ipc_channel *head; /* List's first object*/
    struct spm_ipc_channel *tail; /* List's last object*/
} spm_channel_linked_list_t;

/*
 * Structure containing resources and attributes of a Root of Trust Service.
 */
typedef struct spm_rot_service {
    const uint32_t sid; /* The Root of Trust Service ID.*/
    const uint32_t mask; /* The signal for this Root of Trust Service*/
    struct spm_partition *partition; /* Pointer to the Partition which the Root of Trust Service belongs to.*/
    const uint32_t min_version; /* Minor version of the Root of Trust Service interface.*/
    const uint32_t min_version_policy; /* Minor version policy of the Root of Trust Service.*/
    const bool allow_nspe; /* Whether to allow non-secure clients to connect to the Root of Trust Service.*/
    spm_channel_linked_list_t queue; /* Queue of channels holding ROT_SRV operations waiting to be served. */
} spm_rot_service_t;

/*
 * Structure containing Partition->RoT-Service channel information.
 */
typedef struct spm_ipc_channel {
    struct spm_partition *src_partition; /* Pointer to the Partition which connects to the Root of Trust Service.*/
    spm_rot_service_t *dst_rot_service; /* Pointer to the connected Root of Trust Service.*/
    void *rhandle; /* Reverse handle to be used for this channel.*/
    void *msg_ptr; /* message data sent from user */
    struct spm_ipc_channel *next; /* Next channel in the chain  */
    uint8_t msg_type; /* The message type.*/
    uint8_t state; /* The current processing state of the channel.*/
    uint8_t is_dropped;
} spm_ipc_channel_t;

/*
 * Structure containing the currently active message for a Root of Trust Service.
 */
typedef struct spm_active_msg {
    spm_ipc_channel_t *channel; /* Pointer to the channel delivering this message.*/
    spm_iovec_t iovecs[PSA_MAX_IOVEC]; /* IOvecs sent for message and response.*/
    uint8_t out_index; /* First index of outvecs in iovecs*/
} spm_active_msg_t;

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
 * Returns a pointer to the currently active secure partition or NULL in case called from NSPE.
 */
spm_partition_t *get_active_partition(void);

/*
 * Return an array of memory regions used by a given partition.
 *
 * @param[in] partition_id - a partition ID to find memory regions for, if MEM_PARTITIONS_ALL then
 *                           memory regions for all the partitions are returned
 * @param[out] region_count - will be set to the number of memory regions returned
 */
const mem_region_t *get_mem_regions(int32_t partition_id, uint32_t *region_count);

// Platform dependent APIs

/**
 * Validates a memory block is accessable from a specific partition
 *
 * @param[in] ptr pointer to the beggining of the memory block.
 * @param[in] size size of the memory block in bytes.
 * @param[in] accessing_partition which partition is trying to access the memory.
 * @return true if the entire memory block is accessable from given partition.
 */
bool is_buffer_accessible(const void *ptr, size_t size, spm_partition_t *accessing_partition);

/**
 * Alerts NSPE that a proccess (connect or call) has ended.
 *
 * @param[in] completion_sem_id semaphore id in NSPE.
 */
void nspe_done(osSemaphoreId_t completion_sem_id);

/*
 * Validates parameters sent from caller and queues a connect message on the correct ROT_SRV.
 *
 * @param[in] sid - desired RoT service ID
 * @param[in] msg - pointer to connect message struct
 */
void psa_connect_async(uint32_t sid, spm_pending_connect_msg_t *msg);

/*
 * Validates parameters sent from caller and queues a call message on the correct ROT_SRV.
 *
 * @param[in] handle - channel handle for the connection
 * @param[in] msg - pointer to call message struct
 */
void psa_call_async(psa_handle_t handle, spm_pending_call_msg_t *msg);

/*
 * Validates parameters sent from caller and queues a disconnect message on the correct ROT_SRV.
 *
 * @param[in] handle - handle of channel to close
 * @param[in] msg - pointer to close message struct
 */
void psa_close_async(psa_handle_t handle, spm_pending_close_msg_t *msg);


/*
 * Validates IOvecs.
 *
 * @param[in] in_vec - psa_invec_t array
 * @param[in] in_len - number of elements in in_vec
 * @param[in] out_vec - psa_outvec_t array
 * @param[in] out_len - number of elements in out_vec
*/
void validate_iovec(
    const void *in_vec,
    const uint32_t in_len,
    const void *out_vec,
    const uint32_t out_len
);

void channel_state_switch(uint8_t *current_state, uint8_t expected_state, uint8_t new_state);
void channel_state_assert(uint8_t *current_state, uint8_t expected_state);

#ifdef __cplusplus
}
#endif

#endif // SPM_INTERNAL_H
