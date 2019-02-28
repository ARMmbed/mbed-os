/*
 *  PSA crypto layer on top of Mbed TLS crypto
 */
/*  Copyright (C) 2018, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#if defined(MBEDTLS_PSA_CRYPTO_C)

#include <stdlib.h>
#include <string.h>

#include "psa_crypto_srv_ifs.h"
#include "psa/client.h"
#include "crypto.h"
#include "crypto_platform_spe.h"

#include "mbed_assert.h"

#define MINOR_VER 1
#define CLIENT_PSA_KEY_ID_SIZE_IN_BYTES 4

MBED_STATIC_ASSERT(sizeof(psa_key_id_t) == CLIENT_PSA_KEY_ID_SIZE_IN_BYTES, "Unexpected psa_key_id_t size");

psa_status_t psa_crypto_init(void)
{
    psa_status_t status;
    psa_handle_t handle = PSA_NULL_HANDLE;

    handle = psa_connect(PSA_CRYPTO_INIT_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, NULL, 0, NULL, 0);
    psa_close(handle);

    return (status);
}

/****************************************************************/
/* PSA_MAC */
/****************************************************************/

psa_status_t psa_mac_abort(psa_mac_operation_t *operation)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    if (operation->handle <= 0) {
        return (PSA_SUCCESS);
    }

    psa_crypto_ipc.func = PSA_MAC_ABORT;

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    status = psa_call(operation->handle, &in_vec, 1, NULL, 0);
    psa_close(operation->handle);
    operation->handle = PSA_NULL_HANDLE;

    return (status);
}

static psa_status_t psa_mac_setup(psa_mac_operation_t *operation,
                                  psa_key_handle_t key_handle,
                                  psa_algorithm_t alg,
                                  psa_sec_function_t func)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = func;
    psa_crypto_ipc.handle = key_handle;
    psa_crypto_ipc.alg = alg;

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    operation->handle = psa_connect(PSA_MAC_ID, MINOR_VER);

    if (operation->handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(operation->handle, &in_vec, 1, NULL, 0);
    return (status);
}

psa_status_t psa_mac_sign_setup(psa_mac_operation_t *operation,
                                psa_key_handle_t key_handle,
                                psa_algorithm_t alg)
{
    psa_status_t status = PSA_SUCCESS;

    status = psa_mac_setup(operation, key_handle, alg, PSA_MAC_SIGN_SETUP);
    return status;
}

psa_status_t psa_mac_verify_setup(psa_mac_operation_t *operation,
                                  psa_key_handle_t key_handle,
                                  psa_algorithm_t alg)
{
    psa_status_t status = PSA_SUCCESS;

    status = psa_mac_setup(operation, key_handle, alg, PSA_MAC_VERIFY_SETUP);
    return status;
}

psa_status_t psa_mac_update(psa_mac_operation_t *operation,
                            const uint8_t *input,
                            size_t input_length)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_MAC_UPDATE;

    psa_invec in_vec[2] = { { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length }
    };

    if (operation->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(operation->handle, in_vec, 2, NULL, 0);
    return (status);
}

psa_status_t psa_mac_sign_finish(psa_mac_operation_t *operation,
                                 uint8_t *mac,
                                 size_t mac_size,
                                 size_t *mac_length)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_MAC_SIGN_FINISH;

    psa_invec in_vec[2] = { { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &mac_size, sizeof(mac_size) }
    };
    psa_outvec out_vec[2] = { { mac, mac_size }, { mac_length, sizeof(*mac_length) } };

    if (operation->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(operation->handle, in_vec, 2, out_vec, 2);
    psa_close(operation->handle);
    operation->handle = PSA_NULL_HANDLE;

    return (status);
}

psa_status_t psa_mac_verify_finish(psa_mac_operation_t *operation,
                                   const uint8_t *mac,
                                   size_t mac_length)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_MAC_VERIFY_FINISH;

    psa_invec in_vec[3] = { { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &mac_length, sizeof(mac_length) },
        { mac, mac_length }
    };

    if (operation->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(operation->handle, in_vec, 3, NULL, 0);
    psa_close(operation->handle);
    operation->handle = PSA_NULL_HANDLE;

    return (status);
}


/****************************************************************/
/* PSA_HASH */
/****************************************************************/

psa_status_t psa_hash_abort(psa_hash_operation_t *operation)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    if (operation->handle <= 0) {
        return (PSA_SUCCESS);
    }

    psa_crypto_ipc.func = PSA_HASH_ABORT;

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    status = psa_call(operation->handle, &in_vec, 1, NULL, 0);
    psa_close(operation->handle);
    operation->handle = PSA_NULL_HANDLE;

    return (status);
}

psa_status_t psa_hash_setup(psa_hash_operation_t *operation,
                            psa_algorithm_t alg)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_HASH_SETUP;
    psa_crypto_ipc.alg = alg;

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    operation->handle = psa_connect(PSA_HASH_ID, MINOR_VER);
    if (operation->handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(operation->handle, &in_vec, 1, NULL, 0);
    return (status);
}

psa_status_t psa_hash_update(psa_hash_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_HASH_UPDATE;

    psa_invec in_vec[2] = { {&psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length }
    };

    if (operation->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(operation->handle, in_vec, 2, NULL, 0);
    return (status);
}

psa_status_t psa_hash_finish(psa_hash_operation_t *operation,
                             uint8_t *hash,
                             size_t hash_size,
                             size_t *hash_length)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_HASH_FINISH;

    psa_invec in_vec[2] = { { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &hash_size, sizeof(hash_size) }
    };
    psa_outvec out_vec[2] = { { hash, hash_size },
        { hash_length, sizeof(*hash_length) }
    };

    if (operation->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(operation->handle, in_vec, 2, out_vec, 2);
    psa_close(operation->handle);
    operation->handle = PSA_NULL_HANDLE;

    return (status);
}

psa_status_t psa_hash_verify(psa_hash_operation_t *operation,
                             const uint8_t *hash,
                             size_t hash_length)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_HASH_VERIFY;

    psa_invec in_vec[3] = { { &psa_crypto_ipc, sizeof(psa_crypto_ipc_t) },
        { &hash_length, sizeof(hash_length) },
        { hash, hash_length }
    };

    if (operation->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(operation->handle, in_vec, 3, NULL, 0);
    psa_close(operation->handle);
    operation->handle = PSA_NULL_HANDLE;

    return (status);
}

psa_status_t psa_hash_clone(const psa_hash_operation_t *source_operation,
                            psa_hash_operation_t *target_operation)
{
    psa_status_t status = 0;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    size_t index;

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &index, sizeof(index) }
    };
    psa_outvec out_vec = { &index, sizeof(index) };

    if (source_operation->handle <= PSA_NULL_HANDLE || target_operation->handle != PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    target_operation->handle = psa_connect(PSA_HASH_ID, MINOR_VER);
    if (target_operation->handle <= PSA_NULL_HANDLE) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    psa_crypto_ipc.func = PSA_HASH_CLONE_BEGIN;
    status = psa_call(source_operation->handle, in_vec, 1, &out_vec, 1);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    psa_crypto_ipc.func = PSA_HASH_CLONE_END;
    status = psa_call(target_operation->handle, in_vec, 2, NULL, 0);

exit:
    if (status != PSA_SUCCESS) {
        psa_close(target_operation->handle);
        target_operation->handle = PSA_NULL_HANDLE;
    }
    return (status);
}

/****************************************************************/
/* AEAD */
/****************************************************************/
psa_status_t psa_aead_encrypt(psa_key_handle_t key_handle,
                              psa_algorithm_t alg,
                              const uint8_t *nonce,
                              size_t nonce_length,
                              const uint8_t *additional_data,
                              size_t additional_data_length,
                              const uint8_t *plaintext,
                              size_t plaintext_length,
                              uint8_t *ciphertext,
                              size_t ciphertext_size,
                              size_t *ciphertext_length)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status = PSA_SUCCESS;
    psa_crypto_ipc_aead_t psa_crypto_ipc = { 0, 0, 0, 0, 0, 0, { 0 } };
    psa_invec in_vec[2];
    psa_outvec out_vec[2];

    psa_crypto_ipc.func = PSA_AEAD_ENCRYPT;
    psa_crypto_ipc.handle = key_handle;
    psa_crypto_ipc.alg = alg;
    psa_crypto_ipc.additional_data_length = additional_data_length;
    psa_crypto_ipc.input_length = plaintext_length;

    if (nonce_length > PSA_AEAD_MAX_NONCE_SIZE) {
        return (PSA_ERROR_INVALID_ARGUMENT);
    }

    psa_crypto_ipc.nonce_size = nonce_length;
    memcpy(psa_crypto_ipc.nonce, nonce, nonce_length);

    uint8_t *buffer = calloc(1, additional_data_length +
                             plaintext_length);
    if (buffer == NULL) {
        return (PSA_ERROR_INSUFFICIENT_MEMORY);
    }

    memcpy(buffer, additional_data, additional_data_length);
    memcpy(buffer + additional_data_length, plaintext, plaintext_length);


    in_vec[0] = (psa_invec) {
        &psa_crypto_ipc,
        sizeof(psa_crypto_ipc_aead_t)
    };

    in_vec[1] = (psa_invec) {
        buffer, additional_data_length +
        plaintext_length
    };
    out_vec[0] = (psa_outvec) {
        ciphertext, ciphertext_size
    };
    out_vec[1] = (psa_outvec) {
        ciphertext_length, sizeof(*ciphertext_length)
    };

    handle = psa_connect(PSA_AEAD_ID, MINOR_VER);
    if (handle <= 0) {
        status = PSA_ERROR_COMMUNICATION_FAILURE;
    } else {
        status = psa_call(handle, in_vec, 2, out_vec, 2);
        psa_close(handle);
    }

    free(buffer);
    return (status);
}


psa_status_t psa_aead_decrypt(psa_key_handle_t key_handle,
                              psa_algorithm_t alg,
                              const uint8_t *nonce,
                              size_t nonce_length,
                              const uint8_t *additional_data,
                              size_t additional_data_length,
                              const uint8_t *ciphertext,
                              size_t ciphertext_length,
                              uint8_t *plaintext,
                              size_t plaintext_size,
                              size_t *plaintext_length)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status = PSA_SUCCESS;
    psa_crypto_ipc_aead_t psa_crypto_ipc = { 0, 0, 0, 0, 0, 0, { 0 } };
    psa_invec in_vec[2];
    psa_outvec out_vec[2];

    psa_crypto_ipc.func = PSA_AEAD_DECRYPT;
    psa_crypto_ipc.handle = key_handle;
    psa_crypto_ipc.alg = alg;
    psa_crypto_ipc.additional_data_length = additional_data_length;
    psa_crypto_ipc.input_length = ciphertext_length;

    if (nonce_length > PSA_AEAD_MAX_NONCE_SIZE) {
        return (PSA_ERROR_INVALID_ARGUMENT);
    }

    psa_crypto_ipc.nonce_size = nonce_length;
    memcpy(psa_crypto_ipc.nonce, nonce, nonce_length);

    uint8_t *buffer = calloc(1, additional_data_length +
                             ciphertext_length);
    if (buffer == NULL) {
        return (PSA_ERROR_INSUFFICIENT_MEMORY);
    }

    memcpy(buffer, additional_data, additional_data_length);
    memcpy(buffer + additional_data_length, ciphertext, ciphertext_length);

    in_vec[0] = (psa_invec) {
        &psa_crypto_ipc,
        sizeof(psa_crypto_ipc_aead_t)
    };
    in_vec[1] = (psa_invec) {
        buffer, additional_data_length +
        ciphertext_length
    };

    out_vec[0] = (psa_outvec) {
        plaintext, plaintext_size
    };
    out_vec[1] = (psa_outvec) {
        plaintext_length, sizeof(*plaintext_length)
    };

    handle = psa_connect(PSA_AEAD_ID, MINOR_VER);
    if (handle <= 0) {
        status = PSA_ERROR_COMMUNICATION_FAILURE;
    } else {
        status = psa_call(handle, in_vec, 2, out_vec, 2);
        psa_close(handle);
    }

    free(buffer);
    return (status);
}

/****************************************************************/
/* PSA_ASYMMETRIC */
/****************************************************************/

psa_status_t psa_asymmetric_sign(psa_key_handle_t key_handle,
                                 psa_algorithm_t alg,
                                 const uint8_t *hash,
                                 size_t hash_length,
                                 uint8_t *signature,
                                 size_t signature_size,
                                 size_t *signature_length)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status = PSA_SUCCESS;
    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = { 0, 0, 0, 0, 0 };
    psa_invec in_vec[2];
    psa_outvec out_vec[2];

    psa_crypto_ipc.func = PSA_ASYMMETRIC_SIGN;
    psa_crypto_ipc.handle = key_handle;
    psa_crypto_ipc.alg = alg;

    in_vec[0] = (psa_invec) {
        &psa_crypto_ipc,
        sizeof(psa_crypto_ipc_asymmetric_t)
    };
    in_vec[1] = (psa_invec) {
        hash, hash_length
    };
    out_vec[0] = (psa_outvec) {
        signature, signature_size
    };
    out_vec[1] = (psa_outvec) {
        signature_length, sizeof(*signature_length)
    };

    handle = psa_connect(PSA_ASYMMETRIC_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 2, out_vec, 2);
    psa_close(handle);

    return (status);
}

psa_status_t psa_asymmetric_verify(psa_key_handle_t key_handle,
                                   psa_algorithm_t alg,
                                   const uint8_t *hash,
                                   size_t hash_length,
                                   const uint8_t *signature,
                                   size_t signature_size)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status = PSA_SUCCESS;
    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = { 0, 0, 0, 0, 0 };
    psa_invec in_vec[3];

    psa_crypto_ipc.func = PSA_ASYMMETRIC_VERIFY;
    psa_crypto_ipc.handle = key_handle;
    psa_crypto_ipc.alg = alg;

    in_vec[0] = (psa_invec) {
        &psa_crypto_ipc,
        sizeof(psa_crypto_ipc_asymmetric_t)
    };
    in_vec[1] = (psa_invec) {
        signature, signature_size
    };
    in_vec[2] = (psa_invec) {
        hash, hash_length
    };
    handle = psa_connect(PSA_ASYMMETRIC_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }
    status = psa_call(handle, in_vec, 3, NULL, 0);
    psa_close(handle);

    return (status);
}

static psa_status_t psa_asymmetric_operation(psa_sec_function_t func,
                                             psa_key_handle_t key_handle,
                                             psa_algorithm_t alg,
                                             const uint8_t *input,
                                             size_t input_length,
                                             const uint8_t *salt,
                                             size_t salt_length,
                                             uint8_t *output,
                                             size_t output_size,
                                             size_t *output_length)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status = PSA_SUCCESS;
    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = { 0, 0, 0, 0, 0 };
    psa_invec in_vec[2];
    psa_outvec out_vec[2];

    psa_crypto_ipc.func = func;
    psa_crypto_ipc.handle = key_handle;
    psa_crypto_ipc.alg = alg;
    psa_crypto_ipc.input_length = input_length;
    psa_crypto_ipc.salt_length = salt_length;

    uint8_t *buffer = calloc(1, input_length + salt_length);
    if (buffer == NULL) {
        return (PSA_ERROR_INSUFFICIENT_MEMORY);
    }

    memcpy(buffer, input, input_length);
    memcpy(buffer + input_length, salt, salt_length);

    in_vec[0] = (psa_invec) {
        &psa_crypto_ipc,
        sizeof(psa_crypto_ipc_asymmetric_t)
    };
    in_vec[1] = (psa_invec) {
        buffer, input_length + salt_length
    };

    out_vec[0] = (psa_outvec) {
        output, output_size
    };
    out_vec[1] = (psa_outvec) {
        output_length, sizeof(*output_length)
    };

    handle = psa_connect(PSA_ASYMMETRIC_ID, MINOR_VER);
    if (handle <= 0) {
        status = PSA_ERROR_COMMUNICATION_FAILURE;
    } else {
        status = psa_call(handle, in_vec, 2, out_vec, 2);
        psa_close(handle);
    }

    free(buffer);
    return (status);
}

psa_status_t psa_asymmetric_encrypt(psa_key_handle_t key_handle,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length)
{
    psa_status_t status = psa_asymmetric_operation(PSA_ASYMMETRIC_ENCRYPT,
                                                   key_handle,
                                                   alg, input, input_length,
                                                   salt, salt_length, output,
                                                   output_size, output_length);
    return status;
}

psa_status_t psa_asymmetric_decrypt(psa_key_handle_t key_handle,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length)
{
    psa_status_t status = psa_asymmetric_operation(PSA_ASYMMETRIC_DECRYPT,
                                                   key_handle,
                                                   alg, input, input_length,
                                                   salt, salt_length, output,
                                                   output_size, output_length);
    return status;
}

/****************************************************************/
/* PSA_KEY_MANAGMENT */
/****************************************************************/

psa_status_t psa_allocate_key(psa_key_handle_t *key_handle)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = *key_handle;
    psa_key_mng_ipc.func = PSA_ALLOCATE_KEY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[1] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) }
    };
    psa_outvec out_vec[1] = {
        { key_handle, sizeof(*key_handle) }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 1, out_vec, 1);
    psa_close(handle);

    return (status);
}

psa_status_t psa_create_key(psa_key_lifetime_t lifetime,
                            psa_key_id_t id,
                            psa_key_handle_t *key_handle)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = *key_handle;
    psa_key_mng_ipc.lifetime = lifetime;
    psa_key_mng_ipc.func = PSA_CREATE_KEY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[2] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) },
        { &id, sizeof(id) }
    };
    psa_outvec out_vec[1] = { {
            key_handle, sizeof(*key_handle)
        }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 2, out_vec, 1);
    psa_close(handle);

    return (status);
}

psa_status_t psa_open_key(psa_key_lifetime_t lifetime,
                          psa_key_id_t id,
                          psa_key_handle_t *key_handle)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = *key_handle;
    psa_key_mng_ipc.lifetime = lifetime;
    psa_key_mng_ipc.func = PSA_OPEN_KEY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[2] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) },
        { &id, sizeof(id) }
    };
    psa_outvec out_vec[1] = { {
            key_handle, sizeof(*key_handle)
        }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 2, out_vec, 1);
    psa_close(handle);

    return (status);
}

psa_status_t psa_close_key(psa_key_handle_t key_handle)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = key_handle;
    psa_key_mng_ipc.func = PSA_CLOSE_KEY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[1] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 1, NULL, 0);
    psa_close(handle);

    return (status);
}

psa_status_t psa_get_key_lifetime(psa_key_handle_t key_handle,
                                  psa_key_lifetime_t *lifetime)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = key_handle;
    psa_key_mng_ipc.func = PSA_GET_KEY_LIFETIME;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[1] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc_t) }
    };
    psa_outvec out_vec[1] = { {
            lifetime, (lifetime == NULL ? 0 :
                       sizeof(*lifetime))
        }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 1, out_vec, 1);
    psa_close(handle);

    return (status);
}

psa_key_usage_t psa_key_policy_get_usage(const psa_key_policy_t *policy)
{
    return (policy->usage);
}

psa_algorithm_t psa_key_policy_get_algorithm(const psa_key_policy_t *policy)
{
    return (policy->alg);
}

void psa_key_policy_set_usage(psa_key_policy_t *policy,
                              psa_key_usage_t usage,
                              psa_algorithm_t alg)
{
    policy->usage = usage;
    policy->alg = alg;
}

psa_status_t psa_set_key_policy(psa_key_handle_t key_handle,
                                const psa_key_policy_t *policy)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = key_handle;
    psa_key_mng_ipc.func = PSA_SET_KEY_POLICY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[2] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc_t) },
        { policy, (policy == NULL ? 0 : sizeof(*policy)) }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 2, NULL, 0);
    psa_close(handle);

    return (status);
}

psa_status_t psa_get_key_policy(psa_key_handle_t key_handle,
                                psa_key_policy_t *policy)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = key_handle;
    psa_key_mng_ipc.func = PSA_GET_KEY_POLICY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[1] = { {
            &psa_key_mng_ipc,
            sizeof(psa_key_mng_ipc_t)
        }
    };
    psa_outvec out_vec[1] = { {
            policy, (policy == NULL ? 0 :
                     sizeof(*policy))
        }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 1, out_vec, 1);
    psa_close(handle);

    return (status);
}

psa_status_t psa_import_key(psa_key_handle_t key_handle,
                            psa_key_type_t type,
                            const uint8_t *data,
                            size_t data_length)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = key_handle;
    psa_key_mng_ipc.type = type;
    psa_key_mng_ipc.func = PSA_IMPORT_KEY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[2] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc_t) },
        { data, data_length  }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 2, NULL, 0);
    psa_close(handle);

    return (status);
}

psa_status_t psa_destroy_key(psa_key_handle_t key_handle)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = key_handle;
    psa_key_mng_ipc.func = PSA_DESTROY_KEY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[1] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc_t) }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 1, NULL, 0);
    psa_close(handle);

    return (status);
}

psa_status_t psa_get_key_information(psa_key_handle_t key_handle,
                                     psa_key_type_t *type,
                                     size_t *bits)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = key_handle;
    psa_key_mng_ipc.func = PSA_GET_KEY_INFORMATION;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[1] = { {
            &psa_key_mng_ipc,
            sizeof(psa_key_mng_ipc_t)
        }
    };
    psa_outvec out_vec[2] = { {
            type, (type == NULL ? 0 :
                   sizeof(psa_key_type_t))
        },
        {
            bits, (bits == NULL ? 0 :
                   sizeof(size_t))
        }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 1, out_vec, 2);
    psa_close(handle);

    return (status);
}

static psa_status_t psa_export_key_common(psa_key_handle_t key_handle,
                                          uint8_t *data,
                                          size_t data_size,
                                          size_t *data_length,
                                          psa_sec_function_t func)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = key_handle;
    psa_key_mng_ipc.func = func;

    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[1] = { { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc_t) } };
    psa_outvec out_vec[2] = { { data, data_size },
        { data_length, sizeof(*data_length) }
    };

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 1, out_vec, 2);
    psa_close(handle);

    return (status);
}

psa_status_t psa_export_key(psa_key_handle_t key_handle,
                            uint8_t *data,
                            size_t data_size,
                            size_t *data_length)
{
    psa_status_t status = psa_export_key_common(key_handle, data, data_size,
                                                data_length, PSA_EXPORT_KEY);
    return status;
}

psa_status_t psa_export_public_key(psa_key_handle_t key_handle,
                                   uint8_t *data,
                                   size_t data_size,
                                   size_t *data_length)
{
    psa_status_t status = psa_export_key_common(key_handle, data, data_size,
                                                data_length,
                                                PSA_EXPORT_PUBLIC_KEY);
    return status;
}

psa_status_t psa_generate_key(psa_key_handle_t key_handle,
                              psa_key_type_t type,
                              size_t bits,
                              const void *parameters,
                              size_t parameters_size)
{
    psa_status_t status;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0, 0 };
    psa_key_mng_ipc.handle = key_handle;
    psa_key_mng_ipc.type = type;
    psa_key_mng_ipc.func = PSA_GENERATE_KEY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec[3];
    in_vec[0].base = &psa_key_mng_ipc;
    in_vec[0].len = sizeof(psa_key_mng_ipc_t);
    in_vec[1].base = &bits;
    in_vec[1].len = sizeof(size_t);

    if (((parameters == NULL) && (parameters_size != 0)) || ((parameters != NULL) && (parameters_size == 0))) {
        return (PSA_ERROR_INVALID_ARGUMENT);
    }

    in_vec[2].base = parameters;
    in_vec[2].len = parameters_size;

    handle = psa_connect(PSA_KEY_MNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, in_vec, 3, NULL, 0);
    psa_close(handle);

    return (status);
}

/****************************************************************/
/* PSA_RNG */
/****************************************************************/

psa_status_t psa_generate_random(uint8_t *output,
                                 size_t output_size)
{
    psa_status_t status;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_outvec out_vec[1] = { { output, output_size } };

    handle = psa_connect(PSA_RNG_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, NULL, 0, out_vec, 1);
    psa_close(handle);

    return (status);
}

/****************************************************************/
/* PSA_ENTROPY_INJECT */
/****************************************************************/

psa_status_t mbedtls_psa_inject_entropy(const unsigned char *seed,
                                        size_t seed_size)
{
    psa_status_t status;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec in_vec = { seed, seed_size };

    handle = psa_connect(PSA_ENTROPY_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(handle, &in_vec, 1, NULL, 0);
    psa_close(handle);

    return (status);
}
/****************************************************************/
/* PSA Generator */
/****************************************************************/
psa_status_t psa_get_generator_capacity(const psa_crypto_generator_t *generator,
                                        size_t *capacity)
{
    psa_status_t status;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.func = PSA_GET_GENERATOR_CAPACITY;
    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };
    psa_outvec out_vec = { capacity, sizeof(*capacity) };

    if (generator->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(generator->handle, &in_vec, 1, &out_vec, 1);
    return (status);
}

psa_status_t psa_generator_read(psa_crypto_generator_t *generator,
                                uint8_t *output,
                                size_t output_length)
{
    psa_status_t status;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.func = PSA_GENERATOR_READ;
    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };
    psa_outvec out_vec = { output, output_length };

    if (generator->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(generator->handle, &in_vec, 1, &out_vec, 1);
    return (status);
}

psa_status_t psa_generator_import_key(psa_key_handle_t key_handle,
                                      psa_key_type_t type,
                                      size_t bits,
                                      psa_crypto_generator_t *generator)
{
    psa_status_t status;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.handle = key_handle;
    psa_crypto_ipc.func = PSA_GENERATOR_IMPORT_KEY;
    psa_invec in_vec[3] = { { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &type, sizeof(type) },
        { &bits, sizeof(bits) }
    };

    if (generator->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(generator->handle, in_vec, 3, NULL, 0);
    return (status);
}

psa_status_t psa_key_derivation(psa_crypto_generator_t *generator,
                                psa_key_handle_t key_handle,
                                psa_algorithm_t alg,
                                const uint8_t *salt,
                                size_t salt_length,
                                const uint8_t *label,
                                size_t label_length,
                                size_t capacity)
{
    psa_status_t status;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.handle = key_handle;
    psa_crypto_ipc.alg = alg;
    psa_crypto_ipc.func = PSA_KEY_DERIVATION;
    psa_crypto_ipc.capacity = capacity;

    psa_invec in_vec[3]  = { { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { salt, salt_length },
        { label, label_length }
    };

    generator->handle = psa_connect(PSA_GENERATOR_ID, MINOR_VER);
    if (generator->handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(generator->handle, in_vec, 3, NULL, 0);
    return (status);
}

psa_status_t psa_key_agreement(psa_crypto_generator_t *generator,
                               psa_key_handle_t private_key_handle,
                               const uint8_t *peer_key,
                               size_t peer_key_length,
                               psa_algorithm_t alg)
{
    psa_status_t status;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.handle = private_key_handle;
    psa_crypto_ipc.alg = alg;
    psa_crypto_ipc.func = PSA_KEY_AGREEMENT;

    psa_invec in_vec[2] = { { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { peer_key, peer_key_length }
    };

    generator->handle = psa_connect(PSA_GENERATOR_ID, MINOR_VER);
    if (generator->handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(generator->handle, in_vec, 2, NULL, 0);
    return (status);
}

psa_status_t psa_generator_abort(psa_crypto_generator_t *generator)
{
    psa_status_t status = PSA_SUCCESS;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.func = PSA_GENERATOR_ABORT;
    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };
    if (generator->handle != PSA_NULL_HANDLE) {
        status = psa_call(generator->handle, &in_vec, 1, NULL, 0);
        psa_close(generator->handle);
        generator->handle = PSA_NULL_HANDLE;
    }

    return (status);
}

/****************************************************************/
/* PSA_SYMMETRIC */
/****************************************************************/

psa_status_t psa_cipher_encrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_handle_t key_handle,
                                      psa_algorithm_t alg)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec in_vec;

    psa_crypto_ipc.func = PSA_CIPHER_ENCRYPT_SETUP;
    psa_crypto_ipc.handle = key_handle;
    psa_crypto_ipc.alg = alg;

    in_vec.base = &psa_crypto_ipc;
    in_vec.len = sizeof(psa_crypto_ipc);

    operation->handle = psa_connect(PSA_SYMMETRIC_ID, MINOR_VER);
    if (operation->handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(operation->handle, &in_vec, 1, NULL, 0);
    return (status);
}

psa_status_t psa_cipher_decrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_handle_t key_handle,
                                      psa_algorithm_t alg)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec in_vec;

    psa_crypto_ipc.func = PSA_CIPHER_DECRYPT_SETUP;
    psa_crypto_ipc.handle = key_handle;
    psa_crypto_ipc.alg = alg;

    in_vec = (psa_invec) {
        &psa_crypto_ipc, sizeof(psa_crypto_ipc)
    };

    operation->handle = psa_connect(PSA_SYMMETRIC_ID, MINOR_VER);

    if (operation->handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    status = psa_call(operation->handle, &in_vec, 1, NULL, 0);
    return (status);
}

psa_status_t psa_cipher_generate_iv(psa_cipher_operation_t *operation,
                                    unsigned char *iv,
                                    size_t iv_size,
                                    size_t *iv_length)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec in_vec;
    psa_outvec out_vec[2];

    psa_crypto_ipc.func = PSA_CIPHER_GENERATE_IV;

    in_vec = (psa_invec) {
        &psa_crypto_ipc, sizeof(psa_crypto_ipc)
    };

    out_vec[0] = (psa_outvec) {
        iv, iv_size
    };
    out_vec[1] = (psa_outvec) {
        iv_length, sizeof(*iv_length)
    };

    if (operation->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(operation->handle, &in_vec, 1, out_vec, 2);
    return (status);
}

psa_status_t psa_cipher_set_iv(psa_cipher_operation_t *operation,
                               const unsigned char *iv,
                               size_t iv_length)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec in_vec[2];

    psa_crypto_ipc.func = PSA_CIPHER_SET_IV;

    in_vec[0] = (psa_invec) {
        &psa_crypto_ipc, sizeof(psa_crypto_ipc)
    };
    in_vec[1] = (psa_invec) {
        iv, iv_length
    };

    if (operation->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(operation->handle, in_vec, 2, NULL, 0);
    return (status);
}

psa_status_t psa_cipher_update(psa_cipher_operation_t *operation,
                               const uint8_t *input,
                               size_t input_length,
                               unsigned char *output,
                               size_t output_size,
                               size_t *output_length)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec in_vec[2];
    psa_outvec out_vec[2];

    psa_crypto_ipc.func = PSA_CIPHER_UPDATE;

    in_vec[0] = (psa_invec) {
        &psa_crypto_ipc, sizeof(psa_crypto_ipc)
    };
    in_vec[1] = (psa_invec) {
        input, input_length
    };

    out_vec[0] = (psa_outvec) {
        output, output_size
    };
    out_vec[1] = (psa_outvec) {
        output_length, (output_length == NULL ? 0 :
                        sizeof(*output_length))
    };

    if (operation->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(operation->handle, in_vec, 2, out_vec, 2);
    return (status);
}

psa_status_t psa_cipher_finish(psa_cipher_operation_t *operation,
                               uint8_t *output,
                               size_t output_size,
                               size_t *output_length)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec in_vec;
    psa_outvec out_vec[2];

    psa_crypto_ipc.func = PSA_CIPHER_FINISH;

    in_vec = (psa_invec) {
        &psa_crypto_ipc, sizeof(psa_crypto_ipc)
    };

    out_vec[0] = (psa_outvec) {
        output, output_size
    };
    out_vec[1] = (psa_outvec) {
        output_length, (output_length == NULL ? 0 :
                        sizeof(*output_length))
    };

    if (operation->handle <= 0) {
        return (PSA_ERROR_BAD_STATE);
    }

    status = psa_call(operation->handle, &in_vec, 1, out_vec, 2);
    psa_close(operation->handle);
    operation->handle = PSA_NULL_HANDLE;

    return (status);
}

psa_status_t psa_cipher_abort(psa_cipher_operation_t *operation)
{
    psa_status_t status;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec in_vec;

    if (operation->handle <= 0) {
        return (PSA_SUCCESS);
    }

    psa_crypto_ipc.func = PSA_CIPHER_ABORT;

    in_vec = (psa_invec) {
        &psa_crypto_ipc, sizeof(psa_crypto_ipc)
    };

    status = psa_call(operation->handle, &in_vec, 1, NULL, 0);
    psa_close(operation->handle);
    operation->handle = PSA_NULL_HANDLE;

    return (status);
}

void mbedtls_psa_crypto_free(void)
{
    psa_handle_t handle = PSA_NULL_HANDLE;

    //TODO: add retry mechanism to make sure resourecs were deallocated.
    handle = psa_connect(PSA_CRYPTO_FREE_ID, MINOR_VER);
    if (handle <= 0) {
        return;
    }
    psa_call(handle, NULL, 0, NULL, 0);
    psa_close(handle);
}

#endif /* MBEDTLS_PSA_CRYPTO_C */
