/* Copyright (c) 2018 ARM Limited
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

#include <stdlib.h>
#include <string.h>
#include "psa/client.h"
#include "psa/service.h"
#include "mbed_spm_partitions.h"
#include "psa/internal_trusted_storage.h"
#include "pits_impl.h"
#include "mbed_error.h"

int kv_init_storage_config();

#ifdef   __cplusplus
extern "C"
{
#endif

typedef psa_status_t (*SignalHandler)(psa_msg_t *);

static psa_status_t storage_set(psa_msg_t *msg)
{
    psa_storage_uid_t key = 0;
    void *data = NULL;
    uint32_t alloc_size = msg->in_size[1];
    psa_storage_create_flags_t flags = 0;

    if ((msg->in_size[0] != sizeof(key)) || (msg->in_size[2] != sizeof(flags))) {
        return PSA_DROP_CONNECTION;
    }

    if (psa_read(msg->handle, 0, &key, sizeof(key)) != sizeof(key)) {
        return PSA_DROP_CONNECTION;
    }

    if (psa_read(msg->handle, 2, &flags, sizeof(flags)) != sizeof(flags)) {
        return PSA_DROP_CONNECTION;
    }

    data = malloc(alloc_size);
    if (data == NULL) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    if (psa_read(msg->handle, 1, data, msg->in_size[1]) != msg->in_size[1]) {
        free(data);
        return PSA_ERROR_STORAGE_FAILURE;
    }

    psa_status_t status = psa_its_set_impl(msg->client_id, key, alloc_size, data, flags);
    memset(data, 0, alloc_size);
    free(data);
    return status;
}

static psa_status_t storage_get(psa_msg_t *msg)
{
    psa_storage_uid_t key = 0;
    uint32_t offset = 0;
    size_t actual_size;

    if ((msg->in_size[0] != sizeof(key)) ||
            (msg->in_size[1] != sizeof(offset)) ||
            (msg->out_size[1] != sizeof(actual_size))) {
        return PSA_DROP_CONNECTION;
    }

    if (psa_read(msg->handle, 0, &key, sizeof(key)) != sizeof(key)) {
        return PSA_DROP_CONNECTION;
    }

    if (psa_read(msg->handle, 1, &offset, sizeof(offset)) != sizeof(offset)) {
        return PSA_DROP_CONNECTION;
    }

    uint8_t *data = (uint8_t *)malloc(msg->out_size[0]);
    if (data == NULL) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    psa_status_t status = psa_its_get_impl(msg->client_id, key, offset, msg->out_size[0], data, &actual_size);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, data, actual_size);
        psa_write(msg->handle, 1, &actual_size, sizeof(actual_size));
    }

    memset(data, 0, msg->out_size[0]);
    free(data);
    return status;
}

static psa_status_t storage_info(psa_msg_t *msg)
{
    struct psa_storage_info_t info = { 0 };
    psa_storage_uid_t key = 0;

    if ((msg->in_size[0] != sizeof(key)) || (msg->out_size[0] != sizeof(info))) {
        return PSA_DROP_CONNECTION;
    }

    if (psa_read(msg->handle, 0, &key, sizeof(key)) != sizeof(key)) {
        return PSA_DROP_CONNECTION;
    }

    psa_status_t status = psa_its_get_info_impl(msg->client_id, key, &info);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &info, msg->out_size[0]);
    }

    return status;
}

static psa_status_t storage_remove(psa_msg_t *msg)
{
    psa_storage_uid_t key = 0;

    if (msg->in_size[0] != sizeof(key)) {
        return PSA_DROP_CONNECTION;
    }

    if (psa_read(msg->handle, 0, &key, sizeof(key)) != sizeof(key)) {
        return PSA_DROP_CONNECTION;
    }

    return psa_its_remove_impl(msg->client_id, key);
}
static psa_status_t storage_reset(psa_msg_t *msg)
{
    (void)msg;
    return psa_its_reset_impl();
}



static void message_handler(psa_msg_t *msg, SignalHandler handler)
{
    psa_status_t status = PSA_SUCCESS;
    switch (msg->type) {
        case PSA_IPC_CONNECT: //fallthrough
        case PSA_IPC_DISCONNECT: {
            break;
        }
        case PSA_IPC_CALL: {
            status = handler(msg);
            break;
        }
        default: {
            SPM_PANIC("Unexpected message type %lu!", msg->type);
            break;
        }
    }
    psa_reply(msg->handle, status);
}

void its_entry(void *ptr)
{
    psa_signal_t signals = 0;
    psa_msg_t msg = {0};

    while (1) {
        signals = psa_wait(ITS_WAIT_ANY_SID_MSK, PSA_BLOCK);
        // KVStore initiation:
        // - Must be done after the psa_wait() call since only now we know OS initialization is done
        // - Repeating calls has no effect
        int kv_status = kv_init_storage_config();
        if (kv_status != MBED_SUCCESS) {
            SPM_PANIC("KVStore initiation failed with status %d!", kv_status);
        }

        if ((signals & PSA_ITS_SET_MSK) != 0) {
            if (PSA_SUCCESS != psa_get(PSA_ITS_SET_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, storage_set);
        }
        if ((signals & PSA_ITS_GET_MSK) != 0) {
            if (PSA_SUCCESS != psa_get(PSA_ITS_GET_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, storage_get);
        }
        if ((signals & PSA_ITS_INFO_MSK) != 0) {
            if (PSA_SUCCESS != psa_get(PSA_ITS_INFO_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, storage_info);
        }
        if ((signals & PSA_ITS_REMOVE_MSK) != 0) {
            if (PSA_SUCCESS != psa_get(PSA_ITS_REMOVE_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, storage_remove);
        }
        if ((signals & PSA_ITS_RESET_MSK) != 0) {
            if (PSA_SUCCESS != psa_get(PSA_ITS_RESET_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, storage_reset);
        }

    }
}

#ifdef   __cplusplus
}
#endif
