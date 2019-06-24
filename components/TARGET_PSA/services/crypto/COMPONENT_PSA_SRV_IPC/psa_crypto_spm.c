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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PSA_CRYPTO_C)

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "psa_manifest/sid.h"
#include "psa/client.h"
#include "psa/crypto.h"
#include "crypto_platform_spe.h"
#include "mbed_assert.h"

#define MINOR_VER 1
#define CLIENT_PSA_KEY_ID_SIZE_IN_BYTES 4

MBED_STATIC_ASSERT(sizeof(psa_key_id_t) == CLIENT_PSA_KEY_ID_SIZE_IN_BYTES, "Unexpected psa_key_id_t size");

/****************************************************************/
/* INTERNAL HELPER FUNCTIONS */
/****************************************************************/
static psa_status_t ipc_connect(uint32_t sid, psa_handle_t *handle)
{
    *handle = psa_connect(sid, MINOR_VER);
    if (*handle <= PSA_NULL_HANDLE) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }
    return (PSA_SUCCESS);
}

static inline void ipc_close(psa_handle_t *handle)
{
    psa_close(*handle);
    *handle = PSA_NULL_HANDLE;
}

static psa_status_t ipc_call(psa_handle_t *handle, psa_invec *in_vec, size_t in_vec_size,
                             psa_outvec *out_vec, size_t out_vec_size, bool close)
{
    if (*handle <= PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    psa_status_t status = psa_call(*handle, in_vec, in_vec_size, out_vec, out_vec_size);
    if (close) {
        ipc_close(handle);
    }
    return (status);
}

static psa_status_t ipc_oneshot(uint32_t sid, psa_invec *in_vec, size_t in_vec_size,
                                psa_outvec *out_vec, size_t out_vec_size)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t status = ipc_connect(sid, &handle);
    if (status != PSA_SUCCESS) {
        return status;
    }
    status = ipc_call(&handle, in_vec, in_vec_size, out_vec, out_vec_size, true);
    return (status);
}

/****************************************************************/
/* MODULE SETUP/TEARDOWN */
/****************************************************************/
psa_status_t psa_crypto_init(void)
{
    psa_status_t status = ipc_oneshot(PSA_CRYPTO_INIT_ID, NULL, 0, NULL, 0);
    return (status);
}

void mbedtls_psa_crypto_free(void)
{
    ipc_oneshot(PSA_CRYPTO_FREE_ID, NULL, 0, NULL, 0);
}

/****************************************************************/
/* MAC */
/****************************************************************/
psa_status_t psa_mac_abort(psa_mac_operation_t *operation)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return (PSA_SUCCESS);
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_MAC_ABORT,
        .handle = 0,
        .alg    = 0
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_status_t status = ipc_call(&operation->handle, &in_vec, 1, NULL, 0, true);
    return (status);
}

static psa_status_t psa_mac_setup(psa_mac_operation_t *operation,
                                  psa_key_handle_t key_handle,
                                  psa_algorithm_t alg,
                                  psa_sec_function_t func)
{
    if (operation->handle != PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = func,
        .handle = key_handle,
        .alg    = alg
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_status_t status = ipc_connect(PSA_MAC_ID, &operation->handle);
    if (status != PSA_SUCCESS) {
        return (status);
    }
    status = ipc_call(&operation->handle, &in_vec, 1, NULL, 0, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&operation->handle);
    }
    return (status);
}

psa_status_t psa_mac_sign_setup(psa_mac_operation_t *operation,
                                psa_key_handle_t key_handle,
                                psa_algorithm_t alg)
{
    psa_status_t status = psa_mac_setup(operation, key_handle, alg, PSA_MAC_SIGN_SETUP);
    return (status);
}

psa_status_t psa_mac_verify_setup(psa_mac_operation_t *operation,
                                  psa_key_handle_t key_handle,
                                  psa_algorithm_t alg)
{
    psa_status_t status = psa_mac_setup(operation, key_handle, alg, PSA_MAC_VERIFY_SETUP);
    return (status);
}

psa_status_t psa_mac_update(psa_mac_operation_t *operation,
                            const uint8_t *input,
                            size_t input_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_MAC_UPDATE,
        .handle = 0,
        .alg    = 0
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length }
    };

    psa_status_t status = ipc_call(&operation->handle, in_vec, 2, NULL, 0, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&operation->handle);
    }
    return (status);
}

psa_status_t psa_mac_sign_finish(psa_mac_operation_t *operation,
                                 uint8_t *mac,
                                 size_t mac_size,
                                 size_t *mac_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_MAC_SIGN_FINISH,
        .handle = 0,
        .alg    = 0
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &mac_size, sizeof(mac_size) }
    };

    psa_outvec out_vec[2] = {
        { mac, mac_size },
        { mac_length, sizeof(*mac_length) }
    };

    psa_status_t status = ipc_call(&operation->handle, in_vec, 2, out_vec, 2, true);
    return (status);
}

psa_status_t psa_mac_verify_finish(psa_mac_operation_t *operation,
                                   const uint8_t *mac,
                                   size_t mac_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_MAC_VERIFY_FINISH,
        .handle = 0,
        .alg    = 0
    };

    psa_invec in_vec[3] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &mac_length, sizeof(mac_length) },
        { mac, mac_length }
    };

    psa_status_t status = ipc_call(&operation->handle, in_vec, 3, NULL, 0, true);
    return (status);
}

/****************************************************************/
/* HASH */
/****************************************************************/
psa_status_t psa_hash_abort(psa_hash_operation_t *operation)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return (PSA_SUCCESS);
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_HASH_ABORT,
        .handle = 0,
        .alg    = 0
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_status_t status = ipc_call(&operation->handle, &in_vec, 1, NULL, 0, true);
    return (status);
}

psa_status_t psa_hash_setup(psa_hash_operation_t *operation,
                            psa_algorithm_t alg)
{
    if (operation->handle != PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_HASH_SETUP,
        .handle = 0,
        .alg    = alg
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_status_t status = ipc_connect(PSA_HASH_ID, &operation->handle);
    if (status != PSA_SUCCESS) {
        return (status);
    }
    status = ipc_call(&operation->handle, &in_vec, 1, NULL, 0, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&operation->handle);
    }
    return (status);
}

psa_status_t psa_hash_update(psa_hash_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_HASH_UPDATE,
        .handle = 0,
        .alg    = 0
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length }
    };

    psa_status_t status = ipc_call(&operation->handle, in_vec, 2, NULL, 0, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&operation->handle);
    }
    return (status);
}

psa_status_t psa_hash_finish(psa_hash_operation_t *operation,
                             uint8_t *hash,
                             size_t hash_size,
                             size_t *hash_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_HASH_FINISH,
        .handle = 0,
        .alg    = 0
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &hash_size, sizeof(hash_size) }
    };

    psa_outvec out_vec[2] = {
        { hash, hash_size },
        { hash_length, sizeof(*hash_length) }
    };

    psa_status_t status = ipc_call(&operation->handle, in_vec, 2, out_vec, 2, true);
    return (status);
}

psa_status_t psa_hash_verify(psa_hash_operation_t *operation,
                             const uint8_t *hash,
                             size_t hash_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_HASH_VERIFY,
        .handle = 0,
        .alg    = 0
    };

    psa_invec in_vec[3] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &hash_length, sizeof(hash_length) },
        { hash, hash_length }
    };

    psa_status_t status = ipc_call(&operation->handle, in_vec, 3, NULL, 0, true);
    return (status);
}

psa_status_t psa_hash_clone(const psa_hash_operation_t *source_operation,
                            psa_hash_operation_t *target_operation)
{
    if (source_operation->handle <= PSA_NULL_HANDLE || target_operation->handle != PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = 0,
        .handle = 0,
        .alg = 0
    };

    size_t index = 0;

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &index, sizeof(index) }
    };

    psa_outvec out_vec = { &index, sizeof(index) };

    psa_status_t status = ipc_connect(PSA_HASH_ID, &target_operation->handle);
    if (status != PSA_SUCCESS) {
        return (status);
    }

    psa_crypto_ipc.func = PSA_HASH_CLONE_BEGIN;
    status = ipc_call((psa_handle_t *)&source_operation->handle, in_vec, 1, &out_vec, 1, false);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    psa_crypto_ipc.func = PSA_HASH_CLONE_END;
    status = ipc_call(&target_operation->handle, in_vec, 2, NULL, 0, false);

exit:
    if (status != PSA_SUCCESS) {
        ipc_close(&target_operation->handle);
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
    if (nonce_length > PSA_AEAD_MAX_NONCE_SIZE) {
        return (PSA_ERROR_INVALID_ARGUMENT);
    }

    uint8_t *buffer = calloc(1, (additional_data_length + plaintext_length));
    if (buffer == NULL) {
        return (PSA_ERROR_INSUFFICIENT_MEMORY);
    }

    psa_crypto_ipc_aead_t psa_crypto_ipc = {
        .func                   = PSA_AEAD_ENCRYPT,
        .handle                 = key_handle,
        .alg                    = alg,
        .nonce_size             = nonce_length,
        .additional_data_length = additional_data_length,
        .input_length           = plaintext_length,
        .nonce                  = { 0 }
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { buffer, (additional_data_length + plaintext_length) }
    };

    psa_outvec out_vec[2] = {
        { ciphertext, ciphertext_size },
        { ciphertext_length, sizeof(*ciphertext_length) }
    };

    psa_status_t status;
    memcpy(buffer, additional_data, additional_data_length);
    memcpy(buffer + additional_data_length, plaintext, plaintext_length);
    memcpy(psa_crypto_ipc.nonce, nonce, nonce_length);

    status = ipc_oneshot(PSA_AEAD_ID, in_vec, 2, out_vec, 2);
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
    if (nonce_length > PSA_AEAD_MAX_NONCE_SIZE) {
        return (PSA_ERROR_INVALID_ARGUMENT);
    }

    uint8_t *buffer = calloc(1, (additional_data_length + ciphertext_length));
    if (buffer == NULL) {
        return (PSA_ERROR_INSUFFICIENT_MEMORY);
    }

    psa_crypto_ipc_aead_t psa_crypto_ipc = {
        .func                   = PSA_AEAD_DECRYPT,
        .handle                 = key_handle,
        .alg                    = alg,
        .nonce_size             = nonce_length,
        .additional_data_length = additional_data_length,
        .input_length           = ciphertext_length,
        .nonce                  = { 0 }
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { buffer, (additional_data_length + ciphertext_length) }
    };

    psa_outvec out_vec[2] = {
        { plaintext, plaintext_size },
        { plaintext_length, sizeof(*plaintext_length) }
    };

    psa_status_t status;
    memcpy(buffer, additional_data, additional_data_length);
    memcpy(buffer + additional_data_length, ciphertext, ciphertext_length);
    memcpy(psa_crypto_ipc.nonce, nonce, nonce_length);

    status = ipc_oneshot(PSA_AEAD_ID, in_vec, 2, out_vec, 2);
    free(buffer);
    return (status);
}

/****************************************************************/
/* ASYMMETRIC */
/****************************************************************/
psa_status_t psa_asymmetric_sign(psa_key_handle_t key_handle,
                                 psa_algorithm_t alg,
                                 const uint8_t *hash,
                                 size_t hash_length,
                                 uint8_t *signature,
                                 size_t signature_size,
                                 size_t *signature_length)
{
    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = {
        .func           = PSA_ASYMMETRIC_SIGN,
        .handle         = key_handle,
        .alg            = alg,
        .input_length   = 0,
        .salt_length    = 0
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { hash, hash_length }
    };

    psa_outvec out_vec[2] = {
        { signature, signature_size },
        { signature_length, sizeof(*signature_length) }
    };

    psa_status_t status = ipc_oneshot(PSA_ASYMMETRIC_ID, in_vec, 2, out_vec, 2);
    return (status);
}

psa_status_t psa_asymmetric_verify(psa_key_handle_t key_handle,
                                   psa_algorithm_t alg,
                                   const uint8_t *hash,
                                   size_t hash_length,
                                   const uint8_t *signature,
                                   size_t signature_size)
{
    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = {
        .func           = PSA_ASYMMETRIC_VERIFY,
        .handle         = key_handle,
        .alg            = alg,
        .input_length   = 0,
        .salt_length    = 0
    };

    psa_invec in_vec[3] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { signature, signature_size },
        { hash, hash_length }
    };

    psa_status_t status = ipc_oneshot(PSA_ASYMMETRIC_ID, in_vec, 3, NULL, 0);
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
    uint8_t *buffer = calloc(1, (input_length + salt_length));
    if (buffer == NULL) {
        return (PSA_ERROR_INSUFFICIENT_MEMORY);
    }

    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = {
        .func           = func,
        .handle         = key_handle,
        .alg            = alg,
        .input_length   = input_length,
        .salt_length    = salt_length
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { buffer, (input_length + salt_length) }
    };

    psa_outvec out_vec[2] = {
        { output, output_size },
        { output_length, sizeof(*output_length) }
    };

    psa_status_t status;
    memcpy(buffer, input, input_length);
    memcpy(buffer + input_length, salt, salt_length);

    status = ipc_oneshot(PSA_ASYMMETRIC_ID, in_vec, 2, out_vec, 2);
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
    return (status);
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
    return (status);
}

/****************************************************************/
/* KEY MANAGMENT */
/****************************************************************/
psa_status_t psa_allocate_key(psa_key_handle_t *key_handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_ALLOCATE_KEY,
        .handle     = *key_handle,
        .lifetime   = 0,
        .type       = 0
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_outvec out_vec = { key_handle, sizeof(*key_handle) };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, &out_vec, 1);
    return (status);
}

psa_status_t psa_create_key(psa_key_lifetime_t lifetime,
                            psa_key_id_t id,
                            psa_key_handle_t *key_handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_CREATE_KEY,
        .handle     = *key_handle,
        .lifetime   = lifetime,
        .type       = 0
    };

    psa_invec in_vec[2] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) },
        { &id, sizeof(id) }
    };

    psa_outvec out_vec = { key_handle, sizeof(*key_handle) };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, in_vec, 2, &out_vec, 1);
    return (status);
}

psa_status_t psa_open_key(psa_key_lifetime_t lifetime,
                          psa_key_id_t id,
                          psa_key_handle_t *key_handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_OPEN_KEY,
        .handle     = *key_handle,
        .lifetime   = lifetime,
        .type       = 0
    };

    psa_invec in_vec[2] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) },
        { &id, sizeof(id) }
    };

    psa_outvec out_vec = { key_handle, sizeof(*key_handle) };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, in_vec, 2, &out_vec, 1);
    return (status);
}

psa_status_t psa_close_key(psa_key_handle_t key_handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_CLOSE_KEY,
        .handle     = key_handle,
        .lifetime   = 0,
        .type       = 0
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, NULL, 0);
    return (status);
}

psa_status_t psa_get_key_lifetime(psa_key_handle_t key_handle,
                                  psa_key_lifetime_t *lifetime)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_GET_KEY_LIFETIME,
        .handle     = key_handle,
        .lifetime   = 0,
        .type       = 0
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_outvec out_vec = { lifetime, (lifetime == NULL ? 0 : sizeof(*lifetime)) };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, &out_vec, 1);
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
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_SET_KEY_POLICY,
        .handle     = key_handle,
        .lifetime   = 0,
        .type       = 0
    };

    psa_invec in_vec[2] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) },
        { policy, (policy == NULL ? 0 : sizeof(*policy)) }
    };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, in_vec, 2, NULL, 0);
    return (status);
}

psa_status_t psa_get_key_policy(psa_key_handle_t key_handle,
                                psa_key_policy_t *policy)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_GET_KEY_POLICY,
        .handle     = key_handle,
        .lifetime   = 0,
        .type       = 0
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_outvec out_vec = { policy, (policy == NULL ? 0 : sizeof(*policy)) };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, &out_vec, 1);
    return (status);
}

psa_status_t psa_import_key(psa_key_handle_t key_handle,
                            psa_key_type_t type,
                            const uint8_t *data,
                            size_t data_length)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_IMPORT_KEY,
        .handle     = key_handle,
        .lifetime   = 0,
        .type       = type
    };

    psa_invec in_vec[2] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) },
        { data, data_length  }
    };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, in_vec, 2, NULL, 0);
    return (status);
}

psa_status_t psa_destroy_key(psa_key_handle_t key_handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_DESTROY_KEY,
        .handle     = key_handle,
        .lifetime   = 0,
        .type       = 0
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, NULL, 0);
    return (status);
}

psa_status_t psa_get_key_information(psa_key_handle_t key_handle,
                                     psa_key_type_t *type,
                                     size_t *bits)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_GET_KEY_INFORMATION,
        .handle     = key_handle,
        .lifetime   = 0,
        .type       = 0
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_outvec out_vec[2] = {
        { type, (type == NULL ? 0 : sizeof(*type)) },
        { bits, (bits == NULL ? 0 : sizeof(*bits)) }
    };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, out_vec, 2);
    return (status);
}

static psa_status_t psa_export_key_common(psa_key_handle_t key_handle,
                                          uint8_t *data,
                                          size_t data_size,
                                          size_t *data_length,
                                          psa_sec_function_t func)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = func,
        .handle     = key_handle,
        .lifetime   = 0,
        .type       = 0
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_outvec out_vec[2] = {
        { data, data_size },
        { data_length, sizeof(*data_length) }
    };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, out_vec, 2);
    return (status);
}

psa_status_t psa_export_key(psa_key_handle_t key_handle,
                            uint8_t *data,
                            size_t data_size,
                            size_t *data_length)
{
    psa_status_t status = psa_export_key_common(key_handle, data, data_size,
                                                data_length, PSA_EXPORT_KEY);
    return (status);
}

psa_status_t psa_export_public_key(psa_key_handle_t key_handle,
                                   uint8_t *data,
                                   size_t data_size,
                                   size_t *data_length)
{
    psa_status_t status = psa_export_key_common(key_handle, data, data_size,
                                                data_length,
                                                PSA_EXPORT_PUBLIC_KEY);
    return (status);
}

psa_status_t psa_generate_key(psa_key_handle_t key_handle,
                              psa_key_type_t type,
                              size_t bits,
                              const void *parameters,
                              size_t parameters_size)
{
    if (((parameters == NULL) && (parameters_size != 0)) || ((parameters != NULL) && (parameters_size == 0))) {
        return (PSA_ERROR_INVALID_ARGUMENT);
    }

    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_GENERATE_KEY,
        .handle     = key_handle,
        .lifetime   = 0,
        .type       = type
    };

    psa_invec in_vec[3] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) },
        { &bits, sizeof(bits) },
        { parameters, parameters_size }
    };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, in_vec, 3, NULL, 0);
    return (status);
}

/****************************************************************/
/* RNG */
/****************************************************************/
psa_status_t psa_generate_random(uint8_t *output,
                                 size_t output_size)
{
    psa_outvec out_vec = { output, output_size };

    psa_status_t status = ipc_oneshot(PSA_RNG_ID, NULL, 0, &out_vec, 1);
    return (status);
}

/****************************************************************/
/* ENTROPY INJECT */
/****************************************************************/
psa_status_t mbedtls_psa_inject_entropy(const unsigned char *seed,
                                        size_t seed_size)
{
    psa_invec in_vec = { seed, seed_size };

    psa_status_t status = ipc_oneshot(PSA_ENTROPY_ID, &in_vec, 1, NULL, 0);
    return (status);
}

/****************************************************************/
/* GENERATOR */
/****************************************************************/
psa_status_t psa_get_generator_capacity(const psa_crypto_generator_t *generator,
                                        size_t *capacity)
{
    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_GET_GENERATOR_CAPACITY,
        .handle     = 0,
        .alg        = 0,
        .capacity   = 0
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_outvec out_vec = { capacity, sizeof(*capacity) };

    psa_status_t status = ipc_call((psa_handle_t *)&generator->handle, &in_vec, 1, &out_vec, 1, false);
    return (status);
}

psa_status_t psa_generator_read(psa_crypto_generator_t *generator,
                                uint8_t *output,
                                size_t output_length)
{
    if (generator->handle <= PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_GENERATOR_READ,
        .handle     = 0,
        .alg        = 0,
        .capacity   = 0
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_outvec out_vec = { output, output_length };

    psa_status_t status = ipc_call(&generator->handle, &in_vec, 1, &out_vec, 1, false);
    return (status);
}

psa_status_t psa_generator_import_key(psa_key_handle_t key_handle,
                                      psa_key_type_t type,
                                      size_t bits,
                                      psa_crypto_generator_t *generator)
{
    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_GENERATOR_IMPORT_KEY,
        .handle     = key_handle,
        .alg        = 0,
        .capacity   = 0
    };

    psa_invec in_vec[3] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &type, sizeof(type) },
        { &bits, sizeof(bits) }
    };

    psa_status_t status = ipc_call(&generator->handle, in_vec, 3, NULL, 0, false);
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
    if (generator->handle != PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_KEY_DERIVATION,
        .handle     = key_handle,
        .alg        = alg,
        .capacity   = capacity
    };

    psa_invec in_vec[3]  = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { salt, salt_length },
        { label, label_length }
    };

    psa_status_t status = ipc_connect(PSA_GENERATOR_ID, &generator->handle);
    if (status != PSA_SUCCESS) {
        return (status);
    }
    status = ipc_call(&generator->handle, in_vec, 3, NULL, 0, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&generator->handle);
    }
    return (status);
}

psa_status_t psa_key_agreement(psa_crypto_generator_t *generator,
                               psa_key_handle_t private_key_handle,
                               const uint8_t *peer_key,
                               size_t peer_key_length,
                               psa_algorithm_t alg)
{
    if (generator->handle != PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_KEY_AGREEMENT,
        .handle     = private_key_handle,
        .alg        = alg,
        .capacity   = 0
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { peer_key, peer_key_length }
    };

    psa_status_t status = ipc_connect(PSA_GENERATOR_ID, &generator->handle);
    if (status != PSA_SUCCESS) {
        return (status);
    }
    status = ipc_call(&generator->handle, in_vec, 2, NULL, 0, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&generator->handle);
    }
    return (status);
}

psa_status_t psa_generator_abort(psa_crypto_generator_t *generator)
{
    if (generator->handle <= PSA_NULL_HANDLE) {
        return (PSA_SUCCESS);
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_GENERATOR_ABORT,
        .handle     = 0,
        .alg        = 0,
        .capacity   = 0
    };

    psa_invec in_vec =  { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_status_t status = ipc_call(&generator->handle, &in_vec, 1, NULL, 0, true);
    return (status);
}

/****************************************************************/
/* SYMMETRIC */
/****************************************************************/
static psa_status_t psa_cipher_setup(psa_cipher_operation_t *operation,
                                     psa_key_handle_t key_handle,
                                     psa_algorithm_t alg,
                                     psa_sec_function_t func)
{
    if (operation->handle != PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = func,
        .handle = key_handle,
        .alg = alg
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_status_t status = ipc_connect(PSA_SYMMETRIC_ID, &operation->handle);
    if (status != PSA_SUCCESS) {
        return (status);
    }
    status = ipc_call(&operation->handle, &in_vec, 1, NULL, 0, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&operation->handle);
    }
    return (status);
}

psa_status_t psa_cipher_encrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_handle_t key_handle,
                                      psa_algorithm_t alg)
{
    psa_status_t status = psa_cipher_setup(operation, key_handle, alg, PSA_CIPHER_ENCRYPT_SETUP);
    return (status);
}

psa_status_t psa_cipher_decrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_handle_t key_handle,
                                      psa_algorithm_t alg)
{
    psa_status_t status = psa_cipher_setup(operation, key_handle, alg, PSA_CIPHER_DECRYPT_SETUP);
    return (status);
}

psa_status_t psa_cipher_generate_iv(psa_cipher_operation_t *operation,
                                    unsigned char *iv,
                                    size_t iv_size,
                                    size_t *iv_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = PSA_CIPHER_GENERATE_IV,
        .handle = 0,
        .alg = 0
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_outvec out_vec[2] = {
        { iv, iv_size },
        { iv_length, sizeof(*iv_length) }
    };

    psa_status_t status = ipc_call(&operation->handle, &in_vec, 1, out_vec, 2, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&operation->handle);
    }
    return (status);
}

psa_status_t psa_cipher_set_iv(psa_cipher_operation_t *operation,
                               const unsigned char *iv,
                               size_t iv_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = PSA_CIPHER_SET_IV,
        .handle = 0,
        .alg = 0
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { iv, iv_length }
    };

    psa_status_t status = ipc_call(&operation->handle, in_vec, 2, NULL, 0, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&operation->handle);
    }
    return (status);
}

psa_status_t psa_cipher_update(psa_cipher_operation_t *operation,
                               const uint8_t *input,
                               size_t input_length,
                               unsigned char *output,
                               size_t output_size,
                               size_t *output_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = PSA_CIPHER_UPDATE,
        .handle = 0,
        .alg = 0
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length }
    };

    psa_outvec out_vec[2] = {
        { output, output_size },
        { output_length, (output_length == NULL ? 0 : sizeof(*output_length)) }
    };

    psa_status_t status = ipc_call(&operation->handle, in_vec, 2, out_vec, 2, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&operation->handle);
    }
    return (status);
}

psa_status_t psa_cipher_finish(psa_cipher_operation_t *operation,
                               uint8_t *output,
                               size_t output_size,
                               size_t *output_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = PSA_CIPHER_FINISH,
        .handle = 0,
        .alg = 0
    };

    psa_invec in_vec =  { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_outvec out_vec[2] = {
        { output, output_size },
        { output_length, (output_length == NULL ? 0 : sizeof(*output_length)) }
    };

    psa_status_t status = ipc_call(&operation->handle, &in_vec, 1, out_vec, 2, true);
    return (status);
}

psa_status_t psa_cipher_abort(psa_cipher_operation_t *operation)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return (PSA_SUCCESS);
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = PSA_CIPHER_ABORT,
        .handle = 0,
        .alg = 0
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_status_t status = ipc_call(&operation->handle, &in_vec, 1, NULL, 0, true);
    return (status);
}

#endif /* MBEDTLS_PSA_CRYPTO_C */
