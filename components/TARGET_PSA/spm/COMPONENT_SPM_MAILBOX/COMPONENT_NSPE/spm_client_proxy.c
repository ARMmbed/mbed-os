/* Copyright (c) 2018 ARM Limited
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


// Includes
// --------

#include <stdio.h>
#include "ipc_defs.h"
#include "ipc_queue.h"
#include "spm_client.h"
#include "spm_messages.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include "cmsis_os2.h"
#include "rtx_lib.h"


// Globals
// -------

extern ipc_producer_queue_t *prod_queue;

// API Functions Implementation
// ----------------------------
uint32_t psa_framework_version(void)
{
    return (uint32_t)PSA_FRAMEWORK_VERSION;
}

uint32_t psa_version(uint32_t sid)
{
    osRtxSemaphore_t res_sem_storage = {0, 0, 0, 0, 0, 0, 0, 0};

    const osSemaphoreAttr_t res_sem_attr = {
        .name      = "VER_RES_SEM",
        .attr_bits = 0,
        .cb_mem    = &res_sem_storage,
        .cb_size   = sizeof(res_sem_storage),
    };

    const spm_pending_version_msg_t msg = {
        .rc                = PSA_VERSION_NONE,
        .completion_sem_id = osSemaphoreNew(IPC_RES_SEM_MAX_COUNT,
                                            IPC_RES_SEM_INITIAL_COUNT,
                                            &res_sem_attr
                                           )
    };

    MBED_ASSERT(msg.completion_sem_id != NULL);

    ipc_queue_item_t queue_item = {
        .a = PSA_IPC_VERSION,
        .b = (uint32_t)(&msg),
        .c = sid
    };

    ipc_queue_enqueue(prod_queue, queue_item);

    osStatus_t os_status = osSemaphoreAcquire(msg.completion_sem_id, osWaitForever);
    MBED_ASSERT(osOK == os_status);

    os_status = osSemaphoreDelete(msg.completion_sem_id);
    MBED_ASSERT(osOK == os_status);

    PSA_UNUSED(os_status);

    return msg.rc;
}

psa_handle_t psa_connect(uint32_t sid, uint32_t minor_version)
{
    osRtxSemaphore_t res_sem_storage = {0, 0, 0, 0, 0, 0, 0, 0};

    const osSemaphoreAttr_t res_sem_attr = {
        .name      = "CONN_RES_SEM",
        .attr_bits = 0,
        .cb_mem    = &res_sem_storage,
        .cb_size   = sizeof(res_sem_storage),
    };

    const spm_pending_connect_msg_t msg = {
        .min_version       = minor_version,
        .rc                = PSA_SUCCESS,
        .completion_sem_id = osSemaphoreNew(IPC_RES_SEM_MAX_COUNT,
                                            IPC_RES_SEM_INITIAL_COUNT,
                                            &res_sem_attr
                                           )
    };

    MBED_ASSERT(msg.completion_sem_id != NULL);

    ipc_queue_item_t queue_item = {
        .a = PSA_IPC_CONNECT,
        .b = (uint32_t)(&msg),
        .c = sid
    };

    ipc_queue_enqueue(prod_queue, queue_item);

    osStatus_t os_status = osSemaphoreAcquire(msg.completion_sem_id, osWaitForever);
    MBED_ASSERT(osOK == os_status);

    os_status = osSemaphoreDelete(msg.completion_sem_id);
    MBED_ASSERT(osOK == os_status);

    PSA_UNUSED(os_status);

    return (psa_handle_t)(msg.rc);
}

psa_error_t psa_call(psa_handle_t        handle,
                     const psa_invec_t  *in_vec,
                     size_t              in_len,
                     const psa_outvec_t *out_vec,
                     size_t              out_len
                    )
{
    // - Immediate errors are checked here.
    // - Other errors are checked on the SPM core code

    // TODO: Panic instead
    MBED_ASSERT(handle > 0);
    // TODO: Panic instead
    MBED_ASSERT((in_vec != NULL) || (in_len == 0));
    // TODO: Panic instead
    MBED_ASSERT((out_vec != NULL) || (out_len == 0));
    // TODO: Panic instead
    MBED_ASSERT(in_len + out_len <= PSA_MAX_IOVEC);

    osRtxSemaphore_t res_sem_storage = {0, 0, 0, 0, 0, 0, 0, 0};

    const osSemaphoreAttr_t res_sem_attr = {
        .name      = "CALL_RES_SEM",
        .attr_bits = 0,
        .cb_mem    = &res_sem_storage,
        .cb_size   = sizeof(res_sem_storage),
    };

    const spm_pending_call_msg_t msg = {
        .in_vec            = in_vec,
        .in_vec_size       = in_len,
        .out_vec           = out_vec,
        .out_vec_size      = out_len,
        .rc                = PSA_SUCCESS,
        .completion_sem_id = osSemaphoreNew(IPC_RES_SEM_MAX_COUNT,
                                            IPC_RES_SEM_INITIAL_COUNT,
                                            &res_sem_attr
                                           )
    };

    MBED_ASSERT(msg.completion_sem_id != NULL);

    ipc_queue_item_t queue_item = {
        .a = PSA_IPC_CALL,
        .b = (uint32_t)(&msg),
        .c = (uint32_t)handle
    };

    ipc_queue_enqueue(prod_queue, queue_item);

    osStatus_t os_status = osSemaphoreAcquire(msg.completion_sem_id, osWaitForever);
    MBED_ASSERT(osOK == os_status);

    os_status = osSemaphoreDelete(msg.completion_sem_id);
    MBED_ASSERT(osOK == os_status);

    PSA_UNUSED(os_status);

    return msg.rc;
}


void psa_close(psa_handle_t handle)
{
    if (handle == PSA_NULL_HANDLE) {
        return;
    }

    // - Immediate errors are checked here.
    // - Other errors are checked on the SPM core code
    // TODO: Panic instead
    MBED_ASSERT(handle >= 0);

    osRtxSemaphore_t res_sem_storage = {0, 0, 0, 0, 0, 0, 0, 0};
    const osSemaphoreAttr_t res_sem_attr = {
        .name      = "CLOSE_RES_SEM",
        .attr_bits = 0,
        .cb_mem    = &res_sem_storage,
        .cb_size   = sizeof(res_sem_storage),
    };

    spm_pending_close_msg_t msg = {
        .handle = handle,
        .completion_sem_id = osSemaphoreNew(IPC_RES_SEM_MAX_COUNT,
                                            IPC_RES_SEM_INITIAL_COUNT,
                                            &res_sem_attr
                                           )
    };

    MBED_ASSERT(msg.completion_sem_id != NULL);
    ipc_queue_item_t queue_item = {
        .a = PSA_IPC_DISCONNECT,
        .b = (uint32_t)(&msg),
        .c = (uint32_t)handle
    };

    ipc_queue_enqueue(prod_queue, queue_item);

    osStatus_t os_status = osSemaphoreAcquire(msg.completion_sem_id, osWaitForever);
    MBED_ASSERT(osOK == os_status);

    os_status = osSemaphoreDelete(msg.completion_sem_id);
    MBED_ASSERT(osOK == os_status);

    PSA_UNUSED(os_status);
}
