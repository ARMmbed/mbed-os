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

#include "psa/client.h"
#include "psa_manifest/sid.h"
#include "test_partition_proxy.h"

#define MINOR_VER 1

static psa_status_t invoke_ipc_call(uint32_t sid, psa_invec *in_vec, size_t in_vec_size,
                                    psa_outvec *out_vec, size_t out_vec_size)
{
    psa_status_t status;

    psa_handle_t handle = psa_connect(sid, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, in_vec_size, out_vec, out_vec_size);
    psa_close(handle);

    return (status);
}

psa_status_t test_partition_crypto_create_persistent_key(psa_key_id_t key_id, psa_key_handle_t *key_handle)
{
    psa_invec in_vec = { &key_id, sizeof(key_id) };
    psa_outvec out_vec = { key_handle, sizeof(*key_handle) };
    psa_status_t status = invoke_ipc_call(CRYPTO_CREATE_PERSISTENT_KEY, &in_vec, 1, &out_vec, 1);
    return (status);
}

psa_status_t test_partition_crypto_set_key_policy(psa_key_handle_t key_handle, psa_key_usage_t key_usage,
                                                  psa_algorithm_t key_alg)
{
    psa_invec in_vec[3] = {
        { &key_handle, sizeof(key_handle) },
        { &key_usage, sizeof(key_usage) },
        { &key_alg, sizeof(key_alg) }
    };
    psa_status_t status = invoke_ipc_call(CRYPTO_SET_KEY_POLICY, in_vec, 3, NULL, 0);
    return (status);
}

psa_status_t test_partition_crypto_get_key_policy(psa_key_handle_t key_handle, psa_key_usage_t *key_usage,
                                                  psa_algorithm_t *key_alg)
{
    psa_invec in_vec = { &key_handle, sizeof(key_handle) };
    psa_outvec out_vec[2] = {
        { key_usage, sizeof(*key_usage) },
        { key_alg, sizeof(*key_alg) }
    };
    psa_status_t status = invoke_ipc_call(CRYPTO_GET_KEY_POLICY, &in_vec, 1, out_vec, 2);
    return (status);
}

psa_status_t test_partition_crypto_get_key_information(psa_key_handle_t key_handle, psa_key_type_t *key_type,
                                                       size_t *key_bits)
{
    psa_invec in_vec = { &key_handle, sizeof(key_handle) };
    psa_outvec out_vec[2] = {
        { key_type, sizeof(*key_type) },
        { key_bits, sizeof(*key_bits) }
    };
    psa_status_t status = invoke_ipc_call(CRYPTO_GET_KEY_INFO, &in_vec, 1, out_vec, 2);
    return (status);
}

psa_status_t test_partition_crypto_generate_key(psa_key_handle_t key_handle, psa_key_type_t key_type, size_t key_bits)
{
    psa_invec in_vec[3] = {
        { &key_handle, sizeof(key_handle) },
        { &key_type, sizeof(key_type) },
        { &key_bits, sizeof(key_bits) }
    };
    psa_status_t status = invoke_ipc_call(CRYPTO_GENERATE_KEY, in_vec, 3, NULL, 0);
    return (status);
}

psa_status_t test_partition_crypto_open_persistent_key(psa_key_id_t key_id, psa_key_handle_t *key_handle)
{
    psa_invec in_vec = { &key_id, sizeof(key_id) };
    psa_outvec out_vec = { key_handle, sizeof(*key_handle) };
    psa_status_t status = invoke_ipc_call(CRYPTO_OPEN_PERSISTENT_KEY, &in_vec, 1, &out_vec, 1);
    return (status);
}

psa_status_t test_partition_crypto_close_key(psa_key_handle_t key_handle)
{
    psa_invec in_vec = { &key_handle, sizeof(key_handle) };
    psa_status_t status = invoke_ipc_call(CRYPTO_CLOSE_KEY, &in_vec, 1, NULL, 0);
    return (status);
}

psa_status_t test_partition_crypto_destroy_key(psa_key_handle_t key_handle)
{
    psa_invec in_vec = { &key_handle, sizeof(key_handle) };
    psa_status_t status = invoke_ipc_call(CRYPTO_DESTROY_KEY, &in_vec, 1, NULL, 0);
    return (status);
}

psa_status_t test_partition_crypto_import_key(psa_key_handle_t key_handle, psa_key_type_t key_type,
                                              const unsigned char *key_data, size_t key_data_size)
{
    psa_invec in_vec[4] = {
        { &key_handle, sizeof(key_handle) },
        { &key_type, sizeof(key_type) },
        { &key_data_size, sizeof(key_data_size) },
        { key_data, key_data_size }
    };
    psa_status_t status = invoke_ipc_call(CRYPTO_IMPORT_KEY, in_vec, 4, NULL, 0);
    return (status);
}
