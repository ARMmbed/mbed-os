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

static inline spm_rot_service_t *rot_service_in_partition_get_by_sid(spm_partition_t *partition, uint32_t sid)
{
    for (uint32_t i = 0; i < partition->rot_services_count; ++i) {
        spm_rot_service_t *rot_service = &(partition->rot_services[i]);
        if (rot_service->sid == sid) {
            return rot_service;
        }
    }

    return NULL;
}

static inline spm_rot_service_t *rot_service_get(uint32_t sid)
{
    for (uint32_t i = 0; i < g_spm.partition_count; ++i) {
        spm_rot_service_t *rot_service = rot_service_in_partition_get_by_sid(&(g_spm.partitions[i]), sid);
        if (NULL != rot_service) {
            return rot_service;
        }
    }

    return NULL;
}

static inline void spm_validate_connection_allowed(spm_rot_service_t *target, spm_partition_t *source)
{
    if ((NULL == source) && (false == target->allow_nspe)) {
        SPM_PANIC("SID 0x%x is not allowed to be called from NSPE\n", target->sid);
    }

    if (NULL != source) {
        if (NULL == source->extern_sids) {
            SPM_PANIC("Partition %d did not declare extern functions\n", source->partition_id);
        }

        for (uint32_t i = 0; i < source->extern_sids_count; i++) {
            if (source->extern_sids[i] == target->sid) {
                return;
            }
        }

        SPM_PANIC("SID 0x%x is not in partition %d extern functions list\n", target->sid, source->partition_id);
    }
}

static inline psa_handle_t create_channel_handle(void *handle_mem, int32_t friend_pid)
{
    return psa_hndl_mgr_handle_create(&(g_spm.channels_handle_mgr), handle_mem, friend_pid);
}

static inline spm_ipc_channel_t *get_channel_from_handle(psa_handle_t handle)
{
    return (spm_ipc_channel_t *)psa_hndl_mgr_handle_get_mem(&(g_spm.channels_handle_mgr), handle);
}

static void spm_rot_service_queue_enqueue(spm_rot_service_t *rot_service, spm_ipc_channel_t *item)
{
    osStatus_t os_status = osMutexAcquire(rot_service->partition->mutex, osWaitForever);
    SPM_ASSERT(osOK == os_status);
    PSA_UNUSED(os_status);

    if (rot_service->queue.tail == NULL) {
        rot_service->queue.head = item;
    } else {
        rot_service->queue.tail->next = item;
    }

    rot_service->queue.tail = item;

    int32_t flags = (int32_t)osThreadFlagsSet(rot_service->partition->thread_id, rot_service->mask);
    SPM_ASSERT(flags >= 0);
    PSA_UNUSED(flags);

    os_status = osMutexRelease(rot_service->partition->mutex);
    SPM_ASSERT(osOK == os_status);
}

void psa_connect_async(spm_pending_connect_msg_t *msg)
{
    SPM_ASSERT(msg != NULL);

    spm_rot_service_t *dst_rot_service = rot_service_get(msg->sid);
    if (NULL == dst_rot_service) {
        SPM_PANIC("SID 0x%x is invalid!\n", msg->sid);
    }

    if (((dst_rot_service->min_version_policy == PSA_MINOR_VERSION_POLICY_RELAXED) && (msg->min_version > dst_rot_service->min_version)) ||
        ((dst_rot_service->min_version_policy == PSA_MINOR_VERSION_POLICY_STRICT) && (msg->min_version != dst_rot_service->min_version))) {
            SPM_PANIC("minor version %d does not comply with sid %d minor version %d and minor policy %d",
                    msg->min_version, dst_rot_service->sid, dst_rot_service->min_version, dst_rot_service->min_version_policy);
    }

    spm_partition_t *origin_partition = get_active_partition();
    spm_validate_connection_allowed(dst_rot_service, origin_partition);

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

    // Create the handle in the user message so we could destroy it in case of failure.
    msg->rc = (psa_error_t)create_channel_handle(channel, dst_rot_service->partition->partition_id);

    // NOTE: all struct fields must be initialized as the allocated memory is not zeroed.
    channel->state = CHANNEL_STATE_CONNECTING;
    channel->src_partition = origin_partition;
    channel->dst_rot_service = dst_rot_service;
    channel->msg_ptr = msg;
    channel->msg_type = PSA_IPC_MSG_TYPE_CONNECT;
    channel->rhandle = NULL;
    channel->next = NULL;

    spm_rot_service_queue_enqueue(dst_rot_service, channel);
}

psa_handle_t psa_connect(uint32_t sid, uint32_t minor_version)
{
    osRtxSemaphore_t msg_sem_storage = {0};
    const osSemaphoreAttr_t msg_sem_attr = {
        .name = NULL,
        .attr_bits = 0,
        .cb_mem = &msg_sem_storage,
        .cb_size = sizeof(msg_sem_storage),
    };

    spm_pending_connect_msg_t msg = {
        .sid = sid,
        .min_version = minor_version,
        .rc = PSA_NULL_HANDLE,
        .completion_sem_id = osSemaphoreNew(
            SPM_COMPLETION_SEM_MAX_COUNT,
            SPM_COMPLETION_SEM_INITIAL_COUNT,
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

    spm_rot_service_t *dst_rot_service = channel->dst_rot_service;

    channel->state = CHANNEL_STATE_PENDING;
    channel->msg_ptr = msg;
    channel->msg_type = PSA_IPC_MSG_TYPE_CALL;

    spm_rot_service_queue_enqueue(dst_rot_service, channel);
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
            SPM_COMPLETION_SEM_MAX_COUNT,
            SPM_COMPLETION_SEM_INITIAL_COUNT,
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

    spm_rot_service_queue_enqueue(channel->dst_rot_service, channel);
}
