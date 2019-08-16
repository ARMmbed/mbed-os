/*
 * Copyright (c) 2019, Arm Limited and affiliates
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
#include "mbed_spm_partitions.h"
#include "psa/client.h"
#include "psa/service.h"
#include "psa/crypto.h"

typedef psa_status_t (*SignalHandler)(psa_msg_t *);

static void read_input_param_from_message(psa_msg_t *msg, uint8_t param_index, void *param_ptr)
{
    size_t bytes_read = psa_read(msg->handle, param_index, param_ptr, msg->in_size[param_index]);
    if (bytes_read != msg->in_size[param_index]) {
        SPM_PANIC("SPM read length mismatch");
    }
}

static psa_status_t crypto_generate_key(psa_msg_t *msg)
{
    psa_status_t status;
    psa_key_handle_t key_handle = 0;
    psa_key_type_t key_type = 0;
    size_t key_bits = 0;
    psa_key_attributes_t attributes;

    read_input_param_from_message(msg, 0, &attributes);

    status = psa_generate_key(&attributes, &key_handle);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &key_handle, sizeof(key_handle));
    }
    return (status);
}

static psa_status_t crypto_open_key(psa_msg_t *msg)
{
    psa_status_t status;
    psa_key_id_t key_id;
    psa_key_handle_t key_handle;

    read_input_param_from_message(msg, 0, &key_id);

    status = psa_open_key(key_id, &key_handle);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &key_handle, sizeof(key_handle));
    }
    return (status);
}

static psa_status_t crypto_close_key(psa_msg_t *msg)
{
    psa_status_t status;
    psa_key_handle_t key_handle;

    read_input_param_from_message(msg, 0, &key_handle);

    status = psa_close_key(key_handle);
    return (status);
}

static psa_status_t crypto_destroy_key(psa_msg_t *msg)
{
    psa_status_t status;
    psa_key_handle_t key_handle;

    read_input_param_from_message(msg, 0, &key_handle);

    status = psa_destroy_key(key_handle);
    return (status);
}

static psa_status_t crypto_get_key_attributes(psa_msg_t *msg)
{
    psa_status_t status;
    psa_key_handle_t key_handle;
    psa_key_attributes_t attributes;

    read_input_param_from_message(msg, 0, &key_handle);

    status = psa_get_key_attributes(key_handle, &attributes);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &attributes, sizeof(attributes));
    }
    return (status);
}

static psa_status_t crypto_import_key(psa_msg_t *msg)
{
    psa_status_t status;
    psa_key_handle_t key_handle;
    psa_key_attributes_t attributes;
    uint8_t *key_data;
    size_t key_data_size = msg->in_size[1];

    read_input_param_from_message(msg, 0, &attributes);

    key_data = calloc(1, key_data_size);
    if (key_data == NULL) {
        return (PSA_ERROR_INSUFFICIENT_MEMORY);
    }
    read_input_param_from_message(msg, 1, key_data);

    status = psa_import_key(&attributes, key_data, key_data_size, &key_handle);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &key_handle, sizeof(key_handle));
    }
    free(key_data);

    return (status);
}

static void message_handler(psa_msg_t *msg, SignalHandler handler)
{
    psa_status_t status = 0;

    switch (msg->type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
            break;
        }
        case PSA_IPC_CALL: {
            status = handler(msg);
            break;
        }
        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg->type));
            break;
        }
    }

    psa_reply(msg->handle, status);
}

void test_partition_main(void)
{
    psa_signal_t signal;
    psa_msg_t msg = {0};
    while (1) {
        signal = psa_wait(CRYPTO_ACL_TEST_WAIT_ANY_SID_MSK, PSA_BLOCK);
        if (signal & CRYPTO_GENERATE_KEY_MSK) {
            if (PSA_SUCCESS != psa_get(CRYPTO_GENERATE_KEY_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, crypto_generate_key);
        }
        if (signal & CRYPTO_OPEN_KEY_MSK) {
            if (PSA_SUCCESS != psa_get(CRYPTO_OPEN_KEY_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, crypto_open_key);
        }
        if (signal & CRYPTO_CLOSE_KEY_MSK) {
            if (PSA_SUCCESS != psa_get(CRYPTO_CLOSE_KEY_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, crypto_close_key);
        }
        if (signal & CRYPTO_DESTROY_KEY_MSK) {
            if (PSA_SUCCESS != psa_get(CRYPTO_DESTROY_KEY_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, crypto_destroy_key);
        }
        if (signal & CRYPTO_GET_KEY_ATTRIBUTES_MSK) {
            if (PSA_SUCCESS != psa_get(CRYPTO_GET_KEY_ATTRIBUTES_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, crypto_get_key_attributes);
        }
        if (signal & CRYPTO_IMPORT_KEY_MSK) {
            if (PSA_SUCCESS != psa_get(CRYPTO_IMPORT_KEY_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, crypto_import_key);
        }
    }
}
