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
#include "spm_messages.h"
#include "spm_internal.h"
#include "spm_panic.h"
#include "handles_manager.h"
#include "cmsis.h"

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

static inline psa_handle_t create_msg_handle(void *handle_mem, int32_t friend_pid)
{
    return psa_hndl_mgr_handle_create(&(g_spm.messages_handle_mgr), handle_mem, friend_pid);
}

static inline spm_active_msg_t *get_msg_from_handle(psa_handle_t handle)
{
    return (spm_active_msg_t *)psa_hndl_mgr_handle_get_mem(&(g_spm.messages_handle_mgr), handle);
}

static inline void destroy_msg_handle(psa_handle_t handle)
{
    psa_hndl_mgr_handle_destroy(&(g_spm.messages_handle_mgr), handle);
}

static inline void destroy_channel_handle(psa_handle_t handle)
{
    psa_hndl_mgr_handle_destroy(&(g_spm.channels_handle_mgr), handle);
}


static inline spm_rot_service_t *get_rot_service(spm_partition_t *prt, psa_signal_t signal)
{
    for (size_t i = 0; i < prt->rot_services_count; i++) {
        if (prt->rot_services[i].mask == signal) {
            return &prt->rot_services[i];
        }
    }

    return NULL;
}

/*
 * This function validates the parameters sent from the user and copies it to an active message
 *
 * Function assumptions:
 * * channel is allocated from SPM Core memory
 * * channel->msg_ptr potentially allocated from nonsecure memory
 * * user_msg allocated from secure partition memory - not trusted by SPM Core
*/
static void copy_message_to_spm(spm_ipc_channel_t *channel, psa_msg_t *user_msg)
{
    // Memory allocated from MemoryPool isn't zeroed - thus a temporary variable will make sure we will start from a clear state.
    spm_active_msg_t temp_active_message = {
        .channel = channel,
        .iovecs = {{.in = {0}}},
        .out_index = 0,
    };

    if (channel->msg_type == PSA_IPC_CALL) {
        if (!is_buffer_accessible(channel->msg_ptr, sizeof(spm_pending_call_msg_t), channel->src_partition)) {
            SPM_PANIC("message data is inaccessible\n");
        }

        spm_pending_call_msg_t *call_msg_data = (spm_pending_call_msg_t *)channel->msg_ptr;

        // Copy pointers and sizes to secure memory to prevent TOCTOU
        const psa_invec_t *temp_invec = call_msg_data->in_vec;
        const uint32_t temp_invec_size = call_msg_data->in_vec_size;
        const psa_outvec_t *temp_outvec = call_msg_data->out_vec;
        const uint32_t temp_outvec_size = call_msg_data->out_vec_size;

        validate_iovec(temp_invec, temp_invec_size, temp_outvec, temp_outvec_size);
        temp_active_message.out_index = (uint8_t)temp_invec_size;

        if (temp_invec_size > 0) {
            if (!is_buffer_accessible(temp_invec, temp_invec_size * sizeof(*temp_invec), channel->src_partition)) {
                SPM_PANIC("in_vec is inaccessible\n");
            }

            for (uint32_t i = 0; i < temp_invec_size; ++i) {
                if (temp_invec[i].len == 0) {
                    continue;
                }

                // Copy struct
                temp_active_message.iovecs[i].in = temp_invec[i];
                user_msg->in_size[i] = temp_invec[i].len;

                // Copy then check to prevent TOCTOU
                if (!is_buffer_accessible(
                            temp_active_message.iovecs[i].in.base,
                            temp_active_message.iovecs[i].in.len,
                            channel->src_partition)) {
                    SPM_PANIC("in_vec[%d] is inaccessible\n", i);
                }
            }
        }

        if (temp_outvec_size > 0) {
            if (!is_buffer_accessible(temp_outvec, temp_outvec_size * sizeof(*temp_outvec), channel->src_partition)) {
                SPM_PANIC("out_vec is inaccessible\n");
            }

            for (uint32_t i = 0; i < temp_outvec_size; ++i) {
                if (temp_outvec[i].len == 0) {
                    continue;
                }

                // Copy struct
                temp_active_message.iovecs[temp_invec_size + i].out = temp_outvec[i];
                user_msg->out_size[i] = temp_outvec[i].len;

                // Copy then check to prevent TOCTOU
                if (!is_buffer_accessible(
                            temp_active_message.iovecs[temp_invec_size + i].out.base,
                            temp_active_message.iovecs[temp_invec_size + i].out.len,
                            channel->src_partition)) {
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

    psa_handle_t handle = create_msg_handle(active_msg, PSA_HANDLE_MGR_INVALID_FRIEND_OWNER);

    user_msg->type = channel->msg_type;
    user_msg->rhandle = channel->rhandle;
    user_msg->handle = handle;
}

static spm_ipc_channel_t *spm_rot_service_queue_dequeue(spm_rot_service_t *rot_service)
{
    osStatus_t os_status = osMutexAcquire(rot_service->partition->mutex, osWaitForever);
    SPM_ASSERT(osOK == os_status);
    PSA_UNUSED(os_status);

    spm_ipc_channel_t *ret = rot_service->queue.head;

    if (ret == NULL) {
        SPM_PANIC("Dequeue from empty queue");
    }

    rot_service->queue.head = ret->next;
    ret->next = NULL;

    if (rot_service->queue.head == NULL) {
        rot_service->queue.tail = NULL;

        uint32_t flags = osThreadFlagsClear(rot_service->mask);

        // osThreadFlagsClear() sets the msb on failure
        SPM_ASSERT((flags & SPM_CMSIS_RTOS_ERROR_BIT_MSK) == 0);
        SPM_ASSERT(flags & rot_service->mask);
        PSA_UNUSED(flags);
    }

    os_status = osMutexRelease(rot_service->partition->mutex);
    SPM_ASSERT(osOK == os_status);

    return ret;
}


static uint32_t psa_wait(bool wait_any, uint32_t bitmask, uint32_t timeout)
{
    spm_partition_t *curr_partition = get_active_partition();
    SPM_ASSERT(NULL != curr_partition); // active thread in SPM must be in partition DB

    uint32_t flags_interrupts = curr_partition->flags_interrupts | PSA_DOORBELL;
    uint32_t flags_all = curr_partition->flags_rot_srv | flags_interrupts;

    // In case we're waiting for any signal the bitmask must contain all the flags, otherwise
    // we should be waiting for a subset of interrupt signals.
    if (wait_any) {
        bitmask = flags_all;
    } else {
        // Make sure the interrupt mask contains only a subset of interrupt signal mask.
        if (bitmask != (flags_interrupts & bitmask)) {
            SPM_PANIC("interrupt mask 0x%x must have only bits from 0x%x!\n",
                      bitmask, flags_interrupts);
        }
    }

    uint32_t asserted_signals = osThreadFlagsWait(
                                    bitmask,
                                    osFlagsWaitAny | osFlagsNoClear,
                                    (PSA_BLOCK == timeout) ? osWaitForever : timeout
                                );

    // Asserted_signals must be a subset of the supported ROT_SRV and interrupt signals.
    SPM_ASSERT((asserted_signals == (asserted_signals & flags_all)) ||
               ((PSA_BLOCK != timeout) && (osFlagsErrorTimeout == asserted_signals)));

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

    // signum must be ONLY ONE of the bits of curr_partition->flags_rot_srv
    bool is_one_bit = ((signum != 0) && !(signum & (signum - 1)));
    if (!is_one_bit || !(signum & curr_partition->flags_rot_srv)) {
        SPM_PANIC(
            "signum 0x%x must have only 1 bit ON and must be a subset of 0x%x!\n",
            signum,
            curr_partition->flags_rot_srv
        );
    }

    uint32_t active_flags = osThreadFlagsGet();
    if (0 == (signum & active_flags)) {
        SPM_PANIC("flag is not active!\n");
    }

    spm_rot_service_t *curr_rot_service = get_rot_service(curr_partition, signum);
    if (curr_rot_service == NULL) {
        SPM_PANIC("Received signal (0x%08x) that does not match any root of trust service", signum);
    }
    spm_ipc_channel_t *curr_channel = spm_rot_service_queue_dequeue(curr_rot_service);

    switch (curr_channel->msg_type) {
        case PSA_IPC_CONNECT:
            channel_state_assert(
                &curr_channel->state,
                SPM_CHANNEL_STATE_CONNECTING
            );
            break;

        case PSA_IPC_CALL:
            channel_state_switch(
                &curr_channel->state,
                SPM_CHANNEL_STATE_PENDING,
                SPM_CHANNEL_STATE_ACTIVE
            );
            break;

        case PSA_IPC_DISCONNECT: {
            channel_state_assert(
                &curr_channel->state,
                SPM_CHANNEL_STATE_IDLE
            );
            break;
        }
        default:
            SPM_PANIC("psa_get - unexpected message type=0x%08X", curr_channel->msg_type);
            break;
    }

    copy_message_to_spm(curr_channel, msg);
}

static size_t read_or_skip(psa_handle_t msg_handle, uint32_t invec_idx, void *buf, size_t num_bytes)
{
    spm_active_msg_t *active_msg = get_msg_from_handle(msg_handle);

    channel_state_assert(&active_msg->channel->state, SPM_CHANNEL_STATE_ACTIVE);

    if (invec_idx >= PSA_MAX_IOVEC) {
        SPM_PANIC("Invalid invec_idx\n");
    }

    if (invec_idx >= active_msg->out_index) {
        return 0;
    }

    psa_invec_t *active_iovec = &active_msg->iovecs[invec_idx].in;

    if (num_bytes > active_iovec->len) {
        num_bytes = active_iovec->len;
    }

    if (num_bytes > 0) {
        if (buf) {
            memcpy(buf, active_iovec->base, num_bytes);
        }
        active_iovec->base = (void *)((uint8_t *)active_iovec->base + num_bytes);
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

    spm_active_msg_t *active_msg = get_msg_from_handle(msg_handle);

    channel_state_assert(&active_msg->channel->state, SPM_CHANNEL_STATE_ACTIVE);

    outvec_idx += active_msg->out_index;
    if (outvec_idx >= PSA_MAX_IOVEC) {
        SPM_PANIC("Invalid outvec_idx\n");
    }

    psa_outvec_t *active_iovec = &active_msg->iovecs[outvec_idx].out;
    if (num_bytes > active_iovec->len) {
        SPM_PANIC("Invalid write operation (Requested %d, Avialable %d)\n", num_bytes, active_iovec->len);
    }

    memcpy((uint8_t *)(active_iovec->base), buffer, num_bytes);
    active_iovec->base = (void *)((uint8_t *)active_iovec->base + num_bytes);
    active_iovec->len -= num_bytes;

    return;
}

void psa_reply(psa_handle_t msg_handle, psa_error_t status)
{
    spm_active_msg_t *active_msg = get_msg_from_handle(msg_handle);
    spm_ipc_channel_t *active_channel = active_msg->channel;
    SPM_ASSERT(active_channel != NULL);
    SPM_ASSERT(active_channel->msg_ptr != NULL);

    // !!!!!NOTE!!!!! handles must be destroyed before osMemoryPoolFree().
    // Message creation blocked on resource exhaustion and handle will be created
    // only after a successful memory allocation and is not expected to fail.
    {
        destroy_msg_handle(msg_handle);

        memset(active_msg, 0, sizeof(*active_msg));
        osStatus_t os_status = osMemoryPoolFree(g_spm.active_messages_mem_pool, active_msg);
        SPM_ASSERT(osOK == os_status);
        PSA_UNUSED(os_status);
    }

    osSemaphoreId_t completion_sem_id = NULL;
    bool nspe_call = (active_channel->src_partition == NULL);
    switch (active_channel->msg_type) {
        case PSA_IPC_CONNECT: {
            if ((status != PSA_CONNECTION_ACCEPTED) && (status != PSA_CONNECTION_REFUSED)) {
                SPM_PANIC("status (0X%08x) is not allowed for PSA_IPC_CONNECT", status);
            }

            spm_pending_connect_msg_t *connect_msg_data  = (spm_pending_connect_msg_t *)(active_channel->msg_ptr);
            completion_sem_id = connect_msg_data->completion_sem_id;
            if (status == PSA_CONNECTION_REFUSED) {
                channel_state_assert(&active_channel->state, SPM_CHANNEL_STATE_CONNECTING);
                // !!!!!NOTE!!!!! handles must be destroyed before osMemoryPoolFree().
                // Channel creation fails on resource exhaustion and handle will be created
                // only after a successful memory allocation and is not expected to fail.
                {
                    // In psa_connect the handle was written to user's memory before
                    // the connection was established.
                    // Channel state machine and ACL will prevent attacker from
                    // doing bad stuff before we overwrite it with a negative return code.
                    destroy_channel_handle((psa_handle_t)connect_msg_data->rc);

                    memset(active_channel, 0, sizeof(*active_channel));
                    osStatus_t os_status = osMemoryPoolFree(g_spm.channel_mem_pool, active_channel);
                    SPM_ASSERT(osOK == os_status);
                    PSA_UNUSED(os_status);
                }
                // Replace the handle we created in the user's memory with the error code
                connect_msg_data->rc = status;
                active_channel = NULL;
            } else {
                channel_state_switch(&active_channel->state,
                                     SPM_CHANNEL_STATE_CONNECTING, SPM_CHANNEL_STATE_IDLE);
            }
            break;
        }
        case PSA_IPC_CALL: {
            if ((status >= PSA_RESERVED_ERROR_MIN) && (status <= PSA_RESERVED_ERROR_MAX)) {
                SPM_PANIC("status (0X%08x) is not allowed for PSA_IPC_CALL", status);
            }

            channel_state_switch(&active_channel->state,
                                 SPM_CHANNEL_STATE_ACTIVE, SPM_CHANNEL_STATE_IDLE);

            if (status == PSA_DROP_CONNECTION) {
                active_channel->is_dropped = TRUE;
            }

            spm_pending_call_msg_t *call_msg_data = (spm_pending_call_msg_t *)(active_channel->msg_ptr);
            call_msg_data->rc = status;
            completion_sem_id = call_msg_data->completion_sem_id;
            break;
        }
        case PSA_IPC_DISCONNECT: {
            spm_pending_close_msg_t *close_msg_data = (spm_pending_close_msg_t *)(active_channel->msg_ptr);
            completion_sem_id = close_msg_data->completion_sem_id;

            channel_state_switch(&(active_channel->state),
                                 SPM_CHANNEL_STATE_IDLE,
                                 SPM_CHANNEL_STATE_INVALID
                                );

            // !!!!!NOTE!!!!! handles must be destroyed before osMemoryPoolFree().
            // Channel creation fails on resource exhaustion and handle will be created
            // only after a successful memory allocation and is not expected to fail.
            {
                destroy_channel_handle(close_msg_data->handle);

                memset(active_channel, 0, sizeof(*active_channel));
                osStatus_t os_status = osMemoryPoolFree(g_spm.channel_mem_pool, active_channel);
                active_channel = NULL;
                SPM_ASSERT(osOK == os_status);
                PSA_UNUSED(os_status);
            }
            break;

            // Note: The status code is ignored for PSA_IPC_DISCONNECT message type
        }
        default:
            SPM_PANIC("psa_reply() - Unexpected message type=0x%08X", active_channel->msg_type);
            break;
    }

    if (active_channel != NULL) {
        active_channel->msg_ptr = NULL;
        active_channel->msg_type = 0;  // uninitialized
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

    uint32_t flags = osThreadFlagsSet(target_partition->thread_id, PSA_DOORBELL);
    // osThreadFlagsSet() sets the msb on failure
    // flags is allowed to be 0 since by the time osThreadFlagsSet() returns
    // the flag could have been cleared by another thread
    SPM_ASSERT((flags & SPM_CMSIS_RTOS_ERROR_BIT_MSK) == 0);
    PSA_UNUSED(flags);
}

void psa_clear(void)
{
    uint32_t flags = osThreadFlagsClear(PSA_DOORBELL);

    // osThreadFlagsClear() sets the msb on failure
    SPM_ASSERT((flags & SPM_CMSIS_RTOS_ERROR_BIT_MSK) == 0);

    // psa_clear() must not be called when doorbell signal is not currently asserted
    if ((flags & PSA_DOORBELL) != PSA_DOORBELL) {
        SPM_PANIC("psa_call() called without signaled doorbell\n");
    }
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

void psa_eoi(uint32_t irq_signal)
{
    spm_partition_t *curr_partition = get_active_partition();
    SPM_ASSERT(NULL != curr_partition);
    if (curr_partition->irq_mapper == NULL) {
        SPM_PANIC("Try to clear an interrupt flag without declaring IRQ");
    }

    if (0 == (curr_partition->flags_interrupts & irq_signal)) {
        SPM_PANIC("Signal %d not in irq range\n", irq_signal);
    }

    bool is_one_bit = ((irq_signal != 0) && !(irq_signal & (irq_signal - 1)));
    if (!is_one_bit) {
        SPM_PANIC("signal 0x%x must have only 1 bit ON!\n", irq_signal);
    }

    IRQn_Type irq_line = curr_partition->irq_mapper(irq_signal);
    uint32_t flags = osThreadFlagsClear(irq_signal);
    // osThreadFlagsClear() sets the msb on failure
    SPM_ASSERT((flags & SPM_CMSIS_RTOS_ERROR_BIT_MSK) == 0);

    // psa_eoi() must not be called with an unasserted flag.
    if ((flags & irq_signal) != irq_signal) {
        SPM_PANIC("psa_eoi() called without signaled IRQ\n");
    }

    NVIC_EnableIRQ(irq_line);
}
