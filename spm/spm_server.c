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
#include "mbed_toolchain.h"
#include "spm_server.h"
#include "spm_internal.h"
#include "spm_panic.h"
#include "handles_manager.h"

#define PSA_SIG_DOORBELL_POS    3                           // Bit position for PSA_DOORBELL signal
#define PSA_SIG_DOORBELL_MSK    (1 << PSA_SIG_DOORBELL_POS) // Mask for PSA_DOORBELL signal


extern spm_db_t g_spm;

static inline spm_partition_t *get_partition_by_pid(int32_t partition_id)
{
    for (uint32_t i = 0; i < g_spm.partition_count; ++i) {
        if (g_spm.partitions[i].partition_id == partition_id) {
            return &(g_spm.partitions[i]);
        }
    }

    return NULL;
}

static inline error_t create_msg_handle(void *handle_mem, int32_t friend_pid, psa_handle_t *handle)
{
    return psa_hndl_mgr_handle_create(&(g_spm.messages_handle_mgr), handle_mem, friend_pid, handle);
}

static inline spm_active_msg_t *get_msg_from_handle(psa_handle_t handle)
{
    spm_active_msg_t *handle_mem = NULL;
    psa_hndl_mgr_handle_get_mem(&(g_spm.messages_handle_mgr), handle, (void **)&handle_mem);
    return handle_mem;
}

static inline void destroy_msg_handle(psa_handle_t handle)
{
    psa_hndl_mgr_handle_destroy(&(g_spm.messages_handle_mgr), handle);
}

static inline void destroy_channel_handle(psa_handle_t handle)
{
    psa_hndl_mgr_handle_destroy(&(g_spm.channels_handle_mgr), handle);
}


static inline spm_secure_func_t *get_secure_function(spm_partition_t *prt, psa_signal_t signal)
{
    for (size_t i = 0; i < prt->sec_funcs_count; i++) {
        if (prt->sec_funcs[i].mask == signal) {
            return &prt->sec_funcs[i];
        }
    }

    return NULL;
}

static inline void validate_iovec(const void *vec, const uint32_t len)
{
    if (
        !(
            ((vec == NULL) && (len == 0)) ||
            ((vec != NULL) && (len > 0) && (len <= PSA_MAX_INVEC_LEN))
        )
    ) {
        SPM_PANIC("Failed iovec Validation iovec=(0X%p) len=(%d)\n", vec, len);
    }
}

/*
 * This function validates the parameters sent from the user and copies it to an active message
 *
 * Function assumptions:
 * * channel is allocated from SPM Core memory
 * * channel->msg_ptr potentially allocated from nonsecure memory
 * * user_msg allocated from secure partition memory - not trusted by SPM Core
*/
static psa_handle_t copy_message_to_spm(spm_ipc_channel_t *channel, int32_t current_partition_id, psa_msg_t *user_msg)
{
    psa_handle_t handle = PSA_NULL_HANDLE;

    // Memory allocated from MemoryPool isn't zeroed - thus a temporary variable will make sure we will start from a clear state.
    spm_active_msg_t temp_active_message = {
        .channel = channel,
        .in_vec = {{0}},
        .out_vec = {{0}},
        .rc = 0,
        .type = channel->msg_type
    };

    if (channel->msg_type == PSA_IPC_MSG_TYPE_CALL) {
        if (!is_buffer_accessible(channel->msg_ptr, sizeof(spm_pending_call_msg_t), channel->src_partition)) {
            SPM_PANIC("message data is inaccessible\n");
        }

        spm_pending_call_msg_t *call_msg_data = (spm_pending_call_msg_t *)channel->msg_ptr;

        // Copy pointers and sizes to secure memory to prevent TOCTOU
        const psa_invec_t *temp_invec = call_msg_data->in_vec;
        const uint32_t temp_invec_size = call_msg_data->in_vec_size;
        const psa_outvec_t *temp_outvec = call_msg_data->out_vec;
        const uint32_t temp_outvec_size = call_msg_data->out_vec_size;

        validate_iovec(temp_invec, temp_invec_size);
        validate_iovec(temp_outvec, temp_outvec_size);

        if (temp_invec != NULL) {
            if (!is_buffer_accessible(temp_invec, temp_invec_size * sizeof(*temp_invec), channel->src_partition)) {
                SPM_PANIC("in_vec is inaccessible\n");
            }

            for (uint32_t i = 0; i < temp_invec_size; ++i) {
                if (temp_invec[i].len == 0) {
                    continue;
                }

                // Copy struct
                temp_active_message.in_vec[i] = temp_invec[i];
                user_msg->in_size[i] = temp_invec[i].len;

                // Copy then check to prevent TOCTOU
                if (!is_buffer_accessible(temp_active_message.in_vec[i].base, temp_active_message.in_vec[i].len, channel->src_partition)) {
                    SPM_PANIC("in_vec[%d] is inaccessible\n", i);
                }

            }
        }

        if (temp_outvec != NULL) {
            if (!is_buffer_accessible(temp_outvec, temp_outvec_size * sizeof(*temp_outvec), channel->src_partition)) {
                SPM_PANIC("out_vec is inaccessible\n");
            }

            for (uint32_t i = 0; i < temp_outvec_size; ++i) {
                if (temp_outvec[i].len == 0) {
                    continue;
                }

                // Copy struct
                temp_active_message.out_vec[i] = temp_outvec[i];
                user_msg->out_size[i] = temp_outvec[i].len;

                // Copy then check to prevent TOCTOU
                if (!is_buffer_accessible(temp_active_message.out_vec[i].base, temp_active_message.out_vec[i].len, channel->src_partition)) {
                    SPM_PANIC("out_vec[%d] is inaccessible\n", i);
                }
            }
        }
    }

    // Allocating from SPM-Core internal memory
    spm_active_msg_t *active_msg = (spm_active_msg_t *)osMemoryPoolAlloc(g_spm.active_messages_mem_pool, osWaitForever);
    if (NULL == active_msg) {
        SPM_PANIC("Could not allocate active message");
    }

    // Copy struct
    *active_msg = temp_active_message;

    psa_error_t status = create_msg_handle(active_msg, current_partition_id, &handle);
    SPM_ASSERT(PSA_SUCCESS == status);
    PSA_UNUSED(status);

    user_msg->type = channel->msg_type;
    user_msg->rhandle = channel->rhandle;
    user_msg->handle = handle;
    return handle;
}

static spm_ipc_channel_t * spm_secure_func_queue_dequeue(spm_secure_func_t *sec_func)
{
    osStatus_t os_status = osMutexAcquire(sec_func->partition->mutex, osWaitForever);
    SPM_ASSERT(osOK == os_status);
    PSA_UNUSED(os_status);

    spm_ipc_channel_t *ret = sec_func->queue.head;

    if (ret == NULL) {
        SPM_PANIC("Dequeue from empty queue");
    }

    sec_func->queue.head = ret->next;
    ret->next = NULL;

    if (sec_func->queue.head == NULL) {
        sec_func->queue.tail = NULL;
        int32_t flags = (int32_t)osThreadFlagsClear(sec_func->mask);
        SPM_ASSERT(flags >= 0);
        PSA_UNUSED(flags);
    }

    os_status = osMutexRelease(sec_func->partition->mutex);
    SPM_ASSERT(osOK == os_status);

    return ret;
}


static uint32_t psa_wait(bool wait_any, uint32_t bitmask, uint32_t timeout)
{
    spm_partition_t *curr_partition = get_active_partition();
    SPM_ASSERT(NULL != curr_partition); // active thread in SPM must be in partition DB

    uint32_t flags_all = curr_partition->flags_sf | curr_partition->flags_interrupts;

    // In case we're waiting for any signal the bitmask must contain all the flags, otherwise
    // we should be waiting for a subset of interrupt signals.
    if (wait_any) {
        bitmask = flags_all;
    } else {
        // Make sure the interrupt mask contains only a subset of interrupt signal mask.
        if (bitmask != (curr_partition->flags_interrupts & bitmask)) {
            SPM_PANIC("interrupt mask 0x%x must have only bits from 0x%x!\n",
                bitmask, curr_partition->flags_interrupts);
        }
    }

    uint32_t asserted_signals = osThreadFlagsWait(
        bitmask,
        osFlagsWaitAny | osFlagsNoClear,
        (PSA_WAIT_BLOCK == timeout) ? osWaitForever : timeout
        );

    // Asserted_signals must be a subset of the supported SF and interrupt signals.
    SPM_ASSERT((asserted_signals == (asserted_signals & flags_all)) ||
               ((PSA_WAIT_BLOCK != timeout) && (osFlagsErrorTimeout == asserted_signals)));

    return (osFlagsErrorTimeout == asserted_signals) ? 0 : asserted_signals;
}

uint32_t psa_wait_any(uint32_t timeout)
{
    return psa_wait(true, 0, timeout);
}

uint32_t psa_wait_interrupt(uint32_t interrupt_mask, uint32_t timeout)
{
    return psa_wait(false, interrupt_mask, timeout);
}

void psa_get(psa_signal_t signum, psa_msg_t *msg)
{
    spm_partition_t *curr_partition = get_active_partition();
    SPM_ASSERT(NULL != curr_partition); // active thread in SPM must be in partition DB

    if (!is_buffer_accessible(msg, sizeof(*msg), curr_partition)) {
        SPM_PANIC("msg is inaccessible\n");
    }

    memset(msg, 0, sizeof(*msg));

    // signum must be ONLY ONE of the bits of curr_partition->flags_sf
    bool is_one_bit = ((signum != 0) && !(signum & (signum - 1)));
    if (!is_one_bit || !(signum & (curr_partition->flags_sf | curr_partition->flags_interrupts))) {
        SPM_PANIC(
            "signum 0x%x must have only 1 bit ON and must be a subset of 0x%x!\n",
            signum,
            curr_partition->flags_sf | curr_partition->flags_interrupts
            );
    }

    uint32_t active_flags = osThreadFlagsGet();
    if (0 == (signum & active_flags)) {
        SPM_PANIC("flag is not active!\n");
    }

    spm_secure_func_t *curr_sec_func = get_secure_function(curr_partition, signum);
    if (curr_sec_func == NULL) {
        SPM_PANIC("Recieved signal (0x%08x) that does not match any ecure function", signum);
    }
    spm_ipc_channel_t *curr_channel = spm_secure_func_queue_dequeue(curr_sec_func);

    SPM_ASSERT((curr_channel->msg_type > PSA_IPC_MSG_TYPE_INVALID) &&
               (curr_channel->msg_type <= PSA_IPC_MSG_TYPE_MAX));

    switch (curr_channel->msg_type) {
        case PSA_IPC_MSG_TYPE_CONNECT:
            CHANNEL_STATE_ASSERT(curr_channel->state, CHANNEL_STATE_CONNECTING);
            curr_channel->state = CHANNEL_STATE_IDLE;
            break;

        case PSA_IPC_MSG_TYPE_CALL:
            CHANNEL_STATE_ASSERT(curr_channel->state, CHANNEL_STATE_PENDING);
            curr_channel->state = CHANNEL_STATE_ACTIVE;
            break;

        case PSA_IPC_MSG_TYPE_DISCONNECT:
        {
            CHANNEL_STATE_ASSERT(curr_channel->state, CHANNEL_STATE_INVALID);
            curr_channel->state = CHANNEL_STATE_INVALID;
            msg->handle = PSA_NULL_HANDLE;
            msg->type = PSA_IPC_MSG_TYPE_DISCONNECT;
            msg->rhandle = curr_channel->rhandle;

            // Handle resides in msg_ptr because psa_close is asynchronous
            psa_handle_t channel_handle = (psa_handle_t)curr_channel->msg_ptr;
            destroy_channel_handle(channel_handle);

            memset(curr_channel, 0, sizeof(*curr_channel));
            osStatus_t os_status = osMemoryPoolFree(g_spm.channel_mem_pool, curr_channel);
            SPM_ASSERT(osOK == os_status);
            PSA_UNUSED(os_status);
            return;
            break;
        }
        default:
            SPM_PANIC("Unexpected message type %d\n", curr_channel->msg_type);
    }

    copy_message_to_spm(curr_channel, curr_partition->partition_id, msg);
}

static size_t read_or_skip(psa_handle_t msg_handle, uint32_t invec_idx, void *buf, size_t num_bytes)
{
    if (invec_idx >= PSA_MAX_INVEC_LEN) {
        SPM_PANIC("Invalid invec_idx\n");
    }

    spm_active_msg_t *active_msg = get_msg_from_handle(msg_handle);
    SPM_ASSERT((active_msg->type > PSA_IPC_MSG_TYPE_INVALID) &&
               (active_msg->type <= PSA_IPC_MSG_TYPE_MAX));

    CHANNEL_STATE_ASSERT(active_msg->channel->state, CHANNEL_STATE_ACTIVE);

    psa_invec_t *active_iovec = &active_msg->in_vec[invec_idx];

    if (num_bytes > active_iovec->len) {
        num_bytes = active_iovec->len;
    }

    if (num_bytes > 0) {
        if (buf) {
            memcpy(buf, active_iovec->base, num_bytes);
        }
        active_iovec->base = (void *)((uint8_t*)active_iovec->base + num_bytes);
        active_iovec->len -= num_bytes;
    }

    return num_bytes;
}

size_t psa_read(psa_handle_t msg_handle, uint32_t invec_idx, void *buf, size_t num_bytes)
{
    spm_partition_t *curr_partition = get_active_partition();
    SPM_ASSERT(NULL != curr_partition); // active thread in SPM must be in partition DB

    if (!is_buffer_accessible(buf, num_bytes, curr_partition)) {
        SPM_PANIC("buffer is inaccessible\n");
    }

    return read_or_skip(msg_handle, invec_idx, buf, num_bytes);
}

size_t psa_skip(psa_handle_t msg_handle, uint32_t invec_idx, size_t num_bytes)
{
    return read_or_skip(msg_handle, invec_idx, NULL, num_bytes);
}

void psa_write(psa_handle_t msg_handle, uint32_t outvec_idx, const void *buffer, size_t num_bytes)
{
    if (0 == num_bytes) {
        return;
    }

    spm_partition_t *curr_partition = get_active_partition();
    SPM_ASSERT(NULL != curr_partition); // active thread in S

    if (!is_buffer_accessible(buffer, num_bytes, curr_partition)) {
        SPM_PANIC("buffer is inaccessible\n");
    }

    if (outvec_idx >= PSA_MAX_OUTVEC_LEN) {
        SPM_PANIC("Invalid outvec_idx %d \n", outvec_idx);
    }

    spm_active_msg_t *active_msg = get_msg_from_handle(msg_handle);
    SPM_ASSERT((active_msg->type > PSA_IPC_MSG_TYPE_INVALID) &&
               (active_msg->type <= PSA_IPC_MSG_TYPE_MAX));

    CHANNEL_STATE_ASSERT(active_msg->channel->state, CHANNEL_STATE_ACTIVE);

    psa_outvec_t *active_iovec = &active_msg->out_vec[outvec_idx];

    if (num_bytes > active_iovec->len) {
        SPM_PANIC("Invalid write operation (Requested %d, Avialable %d)\n", num_bytes, active_iovec->len);
    }

    memcpy((uint8_t *)(active_iovec->base), buffer, num_bytes);
    active_iovec->base = (void *)((uint8_t *)active_iovec->base + num_bytes);
    active_iovec->len -= num_bytes;

    return;
}

void psa_end(psa_handle_t msg_handle, psa_error_t retval)
{
    // handle should be PSA_NULL_HANDLE when serving PSA_IPC_MSG_TYPE_DISCONNECT
    if (msg_handle == PSA_NULL_HANDLE) {
        return;
    }

    spm_active_msg_t *active_msg = get_msg_from_handle(msg_handle);
    SPM_ASSERT((active_msg->type > PSA_IPC_MSG_TYPE_INVALID) &&
               (active_msg->type <= PSA_IPC_MSG_TYPE_MAX));

    spm_ipc_channel_t *active_channel = active_msg->channel;
    SPM_ASSERT(active_channel != NULL);

    memset(active_msg, 0, sizeof(*active_msg));
    osStatus_t os_status = osMemoryPoolFree(g_spm.active_messages_mem_pool, active_msg);
    SPM_ASSERT(osOK == os_status);
    PSA_UNUSED(os_status);

    destroy_msg_handle(msg_handle);

    osSemaphoreId_t completion_sem_id = NULL;
    bool nspe_call = (active_channel->src_partition == NULL);
    switch(active_channel->msg_type) {
        case PSA_IPC_MSG_TYPE_CONNECT:
        {
            CHANNEL_STATE_ASSERT(active_channel->state, CHANNEL_STATE_IDLE);
            active_channel->state = (retval < 0 ? CHANNEL_STATE_INVALID : CHANNEL_STATE_IDLE);
            spm_pending_connect_msg_t *connect_msg_data  = (spm_pending_connect_msg_t *)(active_channel->msg_ptr);
            completion_sem_id = connect_msg_data ->completion_sem_id;

            if (retval != PSA_SUCCESS) {
                memset(active_channel, 0, sizeof(*active_channel));
                os_status = osMemoryPoolFree(g_spm.channel_mem_pool, active_channel);
                SPM_ASSERT(osOK == os_status);
                // In psa_connect handle was written to user's memory before the connection was established.
                // Channel state machine and ACL will prevent attacker from doing bad stuff before we overwrite it with negative return code
                destroy_channel_handle((psa_handle_t)connect_msg_data ->rc);
                // Replace the handle we created in the user's memory with the error code
                connect_msg_data ->rc = retval;
                active_channel = NULL;
            }
            break;
        }
        case PSA_IPC_MSG_TYPE_CALL:
        {
            CHANNEL_STATE_ASSERT(active_channel->state, CHANNEL_STATE_ACTIVE);
            active_channel->state = CHANNEL_STATE_IDLE;
            spm_pending_call_msg_t *call_msg_data = (spm_pending_call_msg_t *)(active_channel->msg_ptr);
            call_msg_data->rc = retval;
            completion_sem_id = call_msg_data->completion_sem_id;
            break;
        }
        default:
            SPM_PANIC("Invalid message type %d\n", active_channel->msg_type);
            break;
    }

    if (active_channel != NULL) {
        CHANNEL_STATE_ASSERT(active_channel->state, CHANNEL_STATE_IDLE);
        active_channel->msg_ptr = NULL;
        active_channel->msg_type = PSA_IPC_MSG_TYPE_INVALID;
    }

    if (nspe_call) {
        nspe_done(completion_sem_id);
    } else {
        osStatus_t os_status = osSemaphoreRelease(completion_sem_id);
        SPM_ASSERT(osOK == os_status);
        PSA_UNUSED(os_status);
    }

    return;
}

void psa_notify(int32_t partition_id)
{
    spm_partition_t *target_partition = get_partition_by_pid(partition_id);
    if (NULL == target_partition) {
        SPM_PANIC("Could not find partition (partition_id = %d)\n",
        partition_id
        );
    }

    int32_t flags = (int32_t)osThreadFlagsSet(target_partition->thread_id, PSA_SIG_DOORBELL_MSK);
    SPM_ASSERT(flags >= 0);
    PSA_UNUSED(flags);
}

void psa_clear(void)
{
    int32_t flags = (int32_t)osThreadFlagsClear(PSA_SIG_DOORBELL_MSK);
    SPM_ASSERT(flags >= 0);
    PSA_UNUSED(flags);
}

int32_t psa_identity(psa_handle_t msg_handle)
{
    spm_active_msg_t *active_msg = get_msg_from_handle(msg_handle);
    SPM_ASSERT(active_msg->channel != NULL);
    if (active_msg->channel->src_partition == NULL) {
        return PSA_NSPE_IDENTIFIER;
    }

    return active_msg->channel->src_partition->partition_id;
}

void psa_set_rhandle(psa_handle_t msg_handle, void *rhandle)
{
    spm_active_msg_t *active_msg = get_msg_from_handle(msg_handle);
    active_msg->channel->rhandle = rhandle;
}
