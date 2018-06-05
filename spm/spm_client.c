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
#include "rtx_os.h"
#include "spm_client.h"
#include "spm_internal.h"
#include "spm_panic.h"
#include "handles_manager.h"

extern spm_db_t g_spm;

static inline spm_secure_func_t *sec_func_in_partition_get_by_sfid(spm_partition_t *partition, uint32_t sfid)
{
    for (uint32_t i = 0; i < partition->sec_funcs_count; ++i) {
        spm_secure_func_t *sec_func = &(partition->sec_funcs[i]);
        if (sec_func->sfid == sfid) {
            return sec_func;
        }
    }

    return NULL;
}

static inline spm_secure_func_t *sec_func_get(uint32_t sfid)
{
    for (uint32_t i = 0; i < g_spm.partition_count; ++i) {
        spm_secure_func_t *sec_func = sec_func_in_partition_get_by_sfid(&(g_spm.partitions[i]), sfid);
        if (NULL != sec_func) {
            return sec_func;
        }
    }

    return NULL;
}

static inline void spm_validate_connection_allowed(spm_secure_func_t *target, spm_partition_t *source)
{
    if ((NULL == source) && (false == target->allow_nspe)) {
        SPM_PANIC("SFID 0x%x is not allowed to be called from NSPE\n", target->sfid);
    }

    if (NULL != source) {
        if (NULL == source->extern_sfids) {
            SPM_PANIC("Partition %d did not declare extern functions\n", source->partition_id);
        }

        for (uint32_t i = 0; i < source->extern_sfids_count; i++) {
            if (source->extern_sfids[i] == target->sfid) {
                return;
            }
        }

        SPM_PANIC("SFID 0x%x is not in partition %d extern functions list\n", target->sfid, source->partition_id);
    }
}

static inline error_t create_channel_handle(void *handle_mem, int32_t friend_pid, psa_handle_t *handle)
{
    return psa_hndl_mgr_handle_create(&(g_spm.channels_handle_mgr), handle_mem, friend_pid, handle);
}

static inline spm_ipc_channel_t *get_channel_from_handle(psa_handle_t handle)
{
    spm_ipc_channel_t *handle_mem = NULL;
    psa_hndl_mgr_handle_get_mem(&(g_spm.channels_handle_mgr), handle, (void **)&handle_mem);
    return handle_mem;
}

static void spm_secure_func_queue_enqueue(spm_secure_func_t *sec_func, spm_ipc_channel_t *item)
{
    osStatus_t os_status = osMutexAcquire(sec_func->partition->mutex, osWaitForever);
    SPM_ASSERT(osOK == os_status);
    PSA_UNUSED(os_status);

    if (sec_func->queue.tail == NULL) {
        sec_func->queue.head = item;
    } else {
        sec_func->queue.tail->next = item;
    }

    sec_func->queue.tail = item;

    int32_t flags = (int32_t)osThreadFlagsSet(sec_func->partition->thread_id, sec_func->mask);
    SPM_ASSERT(flags >= 0);
    PSA_UNUSED(flags);

    os_status = osMutexRelease(sec_func->partition->mutex);
    SPM_ASSERT(osOK == os_status);
}

void psa_connect_async(spm_pending_connect_msg_t *msg)
{
    SPM_ASSERT(msg != NULL);

    spm_secure_func_t *dst_sec_func = sec_func_get(msg->sfid);
    if (NULL == dst_sec_func) {
        SPM_PANIC("SFID 0x%x is invalid!\n", msg->sfid);
    }

    if (((dst_sec_func->min_version_policy == PSA_MINOR_VERSION_POLICY_RELAXED) && (msg->min_version > dst_sec_func->min_version)) ||
        ((dst_sec_func->min_version_policy == PSA_MINOR_VERSION_POLICY_STRICT) && (msg->min_version != dst_sec_func->min_version))) {
            SPM_PANIC("minor version %d does not comply with sfid %d minor version %d and minor policy %d",
                    msg->min_version, dst_sec_func->sfid, dst_sec_func->min_version, dst_sec_func->min_version_policy);
    }

    spm_partition_t *origin_partition = get_active_partition();
    spm_validate_connection_allowed(dst_sec_func, origin_partition);

    // Allocating from SPM-Core internal memory
    spm_ipc_channel_t *channel = (spm_ipc_channel_t *)osMemoryPoolAlloc(g_spm.channel_mem_pool, PSA_WAIT_POLL);
    if (NULL == channel) {
        msg->rc = PSA_CONNECTION_REFUSED_BUSY;
        if (origin_partition != NULL) {
            osStatus_t os_status = osSemaphoreRelease(msg->completion_sem_id);
            SPM_ASSERT(osOK == os_status);
            PSA_UNUSED(os_status);
        } else {
            nspe_done(msg->completion_sem_id);
        }

        return;
    }

    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_error_t status = create_channel_handle(channel, dst_sec_func->partition->partition_id, &handle);
    // handle_create() is expected to pass since channel allocation has already passed,
    // and the channels handler manager has the same storage size as the channels pool storage size
    SPM_ASSERT(PSA_SUCCESS == status);
    PSA_UNUSED(status);
    // Create the handle in the user message so we could destroy it in case of failure.
    msg->rc = handle;

    // NOTE: all struct fields must be initialized as the allocated memory is not zeroed.
    channel->state = CHANNEL_STATE_CONNECTING;
    channel->src_partition = origin_partition;
    channel->dst_sec_func = dst_sec_func;
    channel->msg_ptr = msg;
    channel->msg_type = PSA_IPC_MSG_TYPE_CONNECT;
    channel->rhandle = NULL;
    channel->next = NULL;

    spm_secure_func_queue_enqueue(dst_sec_func, channel);
}

psa_handle_t psa_connect(uint32_t sfid, uint32_t minor_version)
{
    osRtxSemaphore_t msg_sem_storage = {0};
    const osSemaphoreAttr_t msg_sem_attr = {
        .name = NULL,
        .attr_bits = 0,
        .cb_mem = &msg_sem_storage,
        .cb_size = sizeof(msg_sem_storage),
    };

    spm_pending_connect_msg_t msg = {
        .sfid = sfid,
        .min_version = minor_version,
        .rc = PSA_NULL_HANDLE,
        .completion_sem_id = osSemaphoreNew(
            PSA_SEC_FUNC_SEM_MAX_COUNT,
            PSA_SEC_FUNC_SEM_INITIAL_COUNT,
            &msg_sem_attr)
    };

    if (NULL == msg.completion_sem_id) {
        SPM_PANIC("could not create a semaphore for connect message");
    }

    psa_connect_async(&msg);

    osStatus_t os_status = osSemaphoreAcquire(msg.completion_sem_id, osWaitForever);
    SPM_ASSERT(osOK == os_status);

    os_status = osSemaphoreDelete(msg.completion_sem_id);
    SPM_ASSERT(osOK == os_status);

    PSA_UNUSED(os_status);

    return (psa_handle_t)msg.rc;
}


void psa_call_async(spm_pending_call_msg_t *msg)
{
    SPM_ASSERT(msg != NULL);

    spm_ipc_channel_t *channel = get_channel_from_handle(msg->channel);

    if (msg->in_vec_size != 0) {
        if (msg->in_vec_size > PSA_MAX_INVEC_LEN) {
            SPM_PANIC("in_len (%d) is bigger than allowed (%d)\n", msg->in_vec_size, PSA_MAX_INVEC_LEN);
        }

        if (msg->in_vec == NULL) {
            SPM_PANIC("in_vec NULL but len is not 0 (%d) \n", msg->in_vec_size);
        }
    }

    if (msg->out_vec_size != 0) {
        if (msg->out_vec_size > PSA_MAX_OUTVEC_LEN) {
            SPM_PANIC("out_len (%d) is bigger than allowed (%d)\n", msg->out_vec_size, PSA_MAX_OUTVEC_LEN);
        }

        if (msg->out_vec == NULL) {
            SPM_PANIC("out_vec NULL but len is not 0 (%d) \n", msg->out_vec_size);
        }
    }

    SPM_ASSERT(PSA_IPC_MSG_TYPE_INVALID == channel->msg_type);
    CHANNEL_STATE_ASSERT(channel->state, CHANNEL_STATE_IDLE);

    spm_secure_func_t *dst_sec_func = channel->dst_sec_func;

    channel->state = CHANNEL_STATE_PENDING;
    channel->msg_ptr = msg;
    channel->msg_type = PSA_IPC_MSG_TYPE_CALL;

    spm_secure_func_queue_enqueue(dst_sec_func, channel);
}

psa_error_t psa_call(
    psa_handle_t handle,
    const psa_invec_t *in_vec,
    size_t in_len,
    const psa_outvec_t *out_vec,
    size_t out_len
    )
{
    osRtxSemaphore_t msg_sem_storage = {0};
    const osSemaphoreAttr_t msg_sem_attr = {
        .name = NULL,
        .attr_bits = 0,
        .cb_mem = &msg_sem_storage,
        .cb_size = sizeof(msg_sem_storage),
    };

    spm_pending_call_msg_t msg = {
        .channel = handle,
        .in_vec = in_vec,
        .in_vec_size = in_len,
        .out_vec = out_vec,
        .out_vec_size = out_len,
        .rc = PSA_SUCCESS,
        .completion_sem_id = osSemaphoreNew(
            PSA_SEC_FUNC_SEM_MAX_COUNT,
            PSA_SEC_FUNC_SEM_INITIAL_COUNT,
            &msg_sem_attr)
    };

    if (NULL == msg.completion_sem_id) {
        SPM_PANIC("could not create a semaphore for connect message");
    }

    psa_call_async(&msg);

    osStatus_t os_status = osSemaphoreAcquire(msg.completion_sem_id, osWaitForever);
    SPM_ASSERT(osOK == os_status);

    os_status = osSemaphoreDelete(msg.completion_sem_id);
    SPM_ASSERT(osOK == os_status);

    PSA_UNUSED(os_status);

    return (psa_error_t)msg.rc;
}

void psa_close(psa_handle_t handle)
{
    if (handle == PSA_NULL_HANDLE) {
        // Negative handles will fail in handle manager.
        return;
    }

    spm_ipc_channel_t *channel = get_channel_from_handle(handle);
    CHANNEL_STATE_ASSERT(channel->state, CHANNEL_STATE_IDLE);
    channel->msg_type = PSA_IPC_MSG_TYPE_DISCONNECT;
    channel->state = CHANNEL_STATE_INVALID;
    // Forward the handle as we return instantly.
    channel->msg_ptr = (void *)handle;

    spm_secure_func_queue_enqueue(channel->dst_sec_func, channel);
}
