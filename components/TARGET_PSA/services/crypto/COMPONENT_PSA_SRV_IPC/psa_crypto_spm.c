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


/*
 * PSA Crypto API (crypto.h)
 */

psa_status_t psa_crypto_init(void)
{
    psa_status_t status = ipc_oneshot(PSA_CRYPTO_INIT_ID, NULL, 0, NULL, 0);
    return (status);
}

psa_status_t psa_get_key_attributes(psa_key_handle_t handle,
                                    psa_key_attributes_t *attributes)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_GET_KEY_ATTRIBUTES,
        .handle     = handle
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_outvec out_vec = { attributes, sizeof(*attributes) };

    return ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, &out_vec, 1);
}

void psa_reset_key_attributes(psa_key_attributes_t *attributes)
{
    /* The reset of key attributes can happen entirely without going to the
     * core. In fact, it can't go to the core without causing issues with
     * memory ownership. Given that psa_set_key_domain_parameters(), which we
     * currently don't allow in the client/server architecture, allocates
     * memory that would be freed by psa_reset_key_attributes(), we must do
     * this in the NSPE due to lack of memory ownership information in the
     * core; the SPE can't currently know if any given allocation is valid to
     * free for a given client. */

    /* Note attributes->domain_parameters are currently ignored, as we don't
     * currently support them in client/server architecture. */
    memset(attributes, 0, sizeof(*attributes));
}

psa_status_t psa_set_key_domain_parameters(psa_key_attributes_t *attributes,
                                           psa_key_type_t type,
                                           const uint8_t *data,
                                           size_t data_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_get_key_domain_parameters(const psa_key_attributes_t *attributes,
                                           uint8_t *data,
                                           size_t data_size,
                                           size_t *data_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_open_key(psa_key_id_t id,
                          psa_key_handle_t *handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_OPEN_KEY,
        .handle     = *handle,
    };

    psa_invec in_vec[2] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) },
        { &id, sizeof(id) }
    };

    psa_outvec out_vec = { handle, sizeof(*handle) };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, in_vec, 2, &out_vec, 1);
    return (status);
}

psa_status_t psa_close_key(psa_key_handle_t handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_CLOSE_KEY,
        .handle     = handle,
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, NULL, 0);
    return (status);
}

psa_status_t psa_import_key(const psa_key_attributes_t *attributes,
                            const uint8_t *data,
                            size_t data_length,
                            psa_key_handle_t *handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_IMPORT_KEY,
        .handle     = 0,
    };

    psa_invec in_vec[3] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) },
        { attributes, sizeof(*attributes) },
        { data, data_length },
    };

    psa_outvec out_vec = { handle, sizeof(*handle) };

    return ipc_oneshot(PSA_KEY_MNG_ID, in_vec, 3, &out_vec, 1);
}

psa_status_t psa_destroy_key(psa_key_handle_t handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func       = PSA_DESTROY_KEY,
        .handle     = handle,
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, NULL, 0);
    return (status);
}

static psa_status_t psa_export_key_common(psa_key_handle_t handle,
                                          uint8_t *data,
                                          size_t data_size,
                                          size_t *data_length,
                                          psa_sec_function_t func)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func   = func,
        .handle = handle,
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_outvec out_vec[2] = {
        { data, data_size },
        { data_length, sizeof(*data_length) }
    };

    psa_status_t status = ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, out_vec, 2);
    return (status);
}

psa_status_t psa_export_key(psa_key_handle_t handle,
                            uint8_t *data,
                            size_t data_size,
                            size_t *data_length)
{
    psa_status_t status = psa_export_key_common(handle, data, data_size,
                                                data_length, PSA_EXPORT_KEY);
    return (status);
}

psa_status_t psa_export_public_key(psa_key_handle_t handle,
                                   uint8_t *data,
                                   size_t data_size,
                                   size_t *data_length)
{
    psa_status_t status = psa_export_key_common(handle, data, data_size,
                                                data_length,
                                                PSA_EXPORT_PUBLIC_KEY);
    return (status);
}

psa_status_t psa_copy_key(psa_key_handle_t source_handle,
                          const psa_key_attributes_t *attributes,
                          psa_key_handle_t *target_handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func   = PSA_COPY_KEY,
        .handle = source_handle,
    };

    psa_invec in_vec = { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) };

    psa_outvec out_vec = { target_handle, sizeof(*target_handle) };

    return ipc_oneshot(PSA_KEY_MNG_ID, &in_vec, 1, &out_vec, 1);
}

psa_status_t psa_hash_compute(psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              uint8_t *hash,
                              size_t hash_size,
                              size_t *hash_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_HASH_COMPUTE,
        .handle = 0,
        .alg    = alg,
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length }
    };

    psa_outvec out_vec[2] = {
        { hash, hash_size },
        { hash_length, sizeof(*hash_length) }
    };

    return ipc_oneshot(PSA_HASH_ID, in_vec, 2, out_vec, 2);
}

psa_status_t psa_hash_compare(psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              const uint8_t *hash,
                              const size_t hash_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_HASH_COMPARE,
        .handle = 0,
        .alg    = alg,
    };

    psa_invec in_vec[3] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length },
        { hash, hash_length },
    };

    return ipc_oneshot(PSA_HASH_ID, in_vec, 3, NULL, 0);
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

psa_status_t psa_mac_compute(psa_key_handle_t handle,
                             psa_algorithm_t alg,
                             const uint8_t *input,
                             size_t input_length,
                             uint8_t *mac,
                             size_t mac_size,
                             size_t *mac_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_MAC_COMPUTE,
        .handle = handle,
        .alg    = alg,
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length },
    };

    psa_outvec out_vec[2] = {
        { mac, mac_size },
        { mac_length, sizeof(*mac_length) },
    };

    psa_status_t status = ipc_oneshot(PSA_MAC_ID, in_vec, 2, out_vec, 2);
    return (status);
}

psa_status_t psa_mac_verify(psa_key_handle_t handle,
                            psa_algorithm_t alg,
                            const uint8_t *input,
                            size_t input_length,
                            const uint8_t *mac,
                            const size_t mac_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_MAC_VERIFY,
        .handle = handle,
        .alg    = alg,
    };

    psa_invec in_vec[3] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length },
        { mac, mac_length },
    };

    psa_status_t status = ipc_oneshot(PSA_MAC_ID, in_vec, 2, NULL, 0);
    return (status);
}

static psa_status_t psa_mac_setup(psa_mac_operation_t *operation,
                                  psa_key_handle_t handle,
                                  psa_algorithm_t alg,
                                  psa_sec_function_t func)
{
    if (operation->handle != PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = func,
        .handle = handle,
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
                                psa_key_handle_t handle,
                                psa_algorithm_t alg)
{
    psa_status_t status = psa_mac_setup(operation, handle, alg, PSA_MAC_SIGN_SETUP);
    return (status);
}

psa_status_t psa_mac_verify_setup(psa_mac_operation_t *operation,
                                  psa_key_handle_t handle,
                                  psa_algorithm_t alg)
{
    psa_status_t status = psa_mac_setup(operation, handle, alg, PSA_MAC_VERIFY_SETUP);
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

psa_status_t psa_cipher_encrypt(psa_key_handle_t handle,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_CIPHER_ENCRYPT,
        .handle = handle,
        .alg    = alg,
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length },
    };

    psa_outvec out_vec[2] = {
        { output, output_size },
        { output_length, sizeof(*output_length) },
    };

    psa_status_t status = ipc_oneshot(PSA_SYMMETRIC_ID, in_vec, 2, out_vec, 2);
    return (status);
}

psa_status_t psa_cipher_decrypt(psa_key_handle_t handle,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length)
{
    psa_crypto_ipc_t psa_crypto_ipc = {
        .func   = PSA_CIPHER_DECRYPT,
        .handle = handle,
        .alg    = alg,
    };

    psa_invec in_vec[3] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length },
    };

    psa_outvec out_vec[2] = {
        { output, output_size },
        { output_length, sizeof(*output_length) },
    };

    psa_status_t status = ipc_oneshot(PSA_SYMMETRIC_ID, in_vec, 2, out_vec, 2);
    return (status);
}

static psa_status_t psa_cipher_setup(psa_cipher_operation_t *operation,
                                     psa_key_handle_t handle,
                                     psa_algorithm_t alg,
                                     psa_sec_function_t func)
{
    if (operation->handle != PSA_NULL_HANDLE) {
        return (PSA_ERROR_BAD_STATE);
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = func,
        .handle = handle,
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
                                      psa_key_handle_t handle,
                                      psa_algorithm_t alg)
{
    psa_status_t status = psa_cipher_setup(operation, handle, alg, PSA_CIPHER_ENCRYPT_SETUP);
    return (status);
}

psa_status_t psa_cipher_decrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_handle_t handle,
                                      psa_algorithm_t alg)
{
    psa_status_t status = psa_cipher_setup(operation, handle, alg, PSA_CIPHER_DECRYPT_SETUP);
    return (status);
}

psa_status_t psa_cipher_generate_iv(psa_cipher_operation_t *operation,
                                    uint8_t *iv,
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
                               const uint8_t *iv,
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
                               uint8_t *output,
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

psa_status_t psa_aead_encrypt(psa_key_handle_t handle,
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
        .handle                 = handle,
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

psa_status_t psa_aead_decrypt(psa_key_handle_t handle,
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
        .handle                 = handle,
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

static psa_status_t psa_aead_setup(psa_aead_operation_t *operation,
                                   psa_key_handle_t handle,
                                   psa_algorithm_t alg,
                                   psa_sec_function_t func)
{
    if (operation->handle != PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = func,
        .handle = handle,
        .alg = alg
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_status_t status = ipc_connect(PSA_AEAD_ID, &operation->handle);
    if (status != PSA_SUCCESS) {
        return status;
    }
    status = ipc_call(&operation->handle, &in_vec, 1, NULL, 0, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&operation->handle);
    }
    return status;
}

psa_status_t psa_aead_encrypt_setup(psa_aead_operation_t *operation,
                                    psa_key_handle_t handle,
                                    psa_algorithm_t alg)
{
    return psa_aead_setup(operation, handle, alg, PSA_AEAD_ENCRYPT_SETUP);
}

psa_status_t psa_aead_decrypt_setup(psa_aead_operation_t *operation,
                                    psa_key_handle_t handle,
                                    psa_algorithm_t alg)
{
    return psa_aead_setup(operation, handle, alg, PSA_AEAD_DECRYPT_SETUP);
}

psa_status_t psa_aead_generate_nonce(psa_aead_operation_t *operation,
                                     uint8_t *nonce,
                                     size_t nonce_size,
                                     size_t *nonce_length)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_ipc_aead_t psa_crypto_ipc = {
        .func   = PSA_AEAD_GENERATE_NONCE,
        .handle = 0,
    };

    psa_invec in_vec[1] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
    };

    psa_outvec out_vec[2] = {
        { nonce, nonce_size },
        { nonce_length, sizeof(*nonce_length) },
    };

    return ipc_call(&operation->handle, in_vec, 1, out_vec, 2, false);
}

psa_status_t psa_aead_set_nonce(psa_aead_operation_t *operation,
                                const uint8_t *nonce,
                                size_t nonce_length)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    if (nonce_length > PSA_AEAD_MAX_NONCE_SIZE) {
        return (PSA_ERROR_INVALID_ARGUMENT);
    }

    psa_crypto_ipc_aead_t psa_crypto_ipc = {
        .func   = PSA_AEAD_SET_NONCE,
        .handle = 0,
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { nonce, nonce_length }
    };

    return ipc_call(&operation->handle, in_vec, 2, NULL, 0, false);
}

psa_status_t psa_aead_set_lengths(psa_aead_operation_t *operation,
                                  size_t ad_length,
                                  size_t plaintext_length)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_ipc_aead_t psa_crypto_ipc = {
        .func   = PSA_AEAD_SET_LENGTHS,
        .handle = 0,
        .alg    = 0,
        .additional_data_length = ad_length,
        .input_length = plaintext_length,
    };

    psa_invec in_vec[1] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
    };

    return ipc_call(&operation->handle, in_vec, 1, NULL, 0, false);
}

psa_status_t psa_aead_update_ad(psa_aead_operation_t *operation,
                                const uint8_t *input,
                                size_t input_length)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_ipc_aead_t psa_crypto_ipc = {
        .func                   = PSA_AEAD_UPDATE_AD,
        .handle                 = 0,
        .alg                    = 0,
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length },
    };

    return ipc_call(&operation->handle, in_vec, 2, NULL, 0, false);
}

psa_status_t psa_aead_update(psa_aead_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length,
                             uint8_t *output,
                             size_t output_size,
                             size_t *output_length)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_ipc_aead_t psa_crypto_ipc = {
        .func                   = PSA_AEAD_UPDATE,
        .handle                 = 0,
        .alg                    = 0,
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { input, input_length },
    };

    psa_outvec out_vec[2] = {
        { output, output_size },
        { output_length, sizeof(*output_length) },
    };

    return ipc_call(&operation->handle, in_vec, 2, out_vec, 2, false);
}

psa_status_t psa_aead_finish(psa_aead_operation_t *operation,
                             uint8_t *ciphertext,
                             size_t ciphertext_size,
                             size_t *ciphertext_length,
                             uint8_t *tag,
                             size_t tag_size,
                             size_t *tag_length)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = PSA_AEAD_FINISH,
        .handle = 0,
        .alg = 0
    };

    psa_invec in_vec[1] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
    };

    psa_outvec out_vec[4] = {
        { ciphertext, ciphertext_size },
        { ciphertext_length, (ciphertext_length == NULL ? 0 : sizeof(*ciphertext_length)) },
        { tag, tag_size },
        { tag_length, (tag_length == NULL ? 0 : sizeof(*tag_length)) },
    };

    return ipc_call(&operation->handle, in_vec, 1, out_vec, 4, true);
}

psa_status_t psa_aead_verify(psa_aead_operation_t *operation,
                             uint8_t *plaintext,
                             size_t plaintext_size,
                             size_t *plaintext_length,
                             const uint8_t *tag,
                             size_t tag_length)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_ipc_t psa_crypto_ipc = {
        .func = PSA_AEAD_VERIFY,
        .handle = 0,
        .alg = 0
    };

    psa_invec in_vec[3] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { tag, tag_length },
    };

    psa_outvec out_vec[2] = {
        { plaintext, plaintext_size },
        { plaintext_length, (plaintext_length == NULL ? 0 : sizeof(*plaintext_length)) },
    };

    return ipc_call(&operation->handle, in_vec, 3, out_vec, 2, true);
}

psa_status_t psa_aead_abort(psa_aead_operation_t *operation)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_SUCCESS;
    }

    psa_crypto_ipc_aead_t psa_crypto_ipc = {
        .func = PSA_AEAD_ABORT,
        .handle = 0,
        .alg = 0
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    return ipc_call(&operation->handle, &in_vec, 1, NULL, 0, true);
}

psa_status_t psa_sign_hash(psa_key_handle_t handle,
                           psa_algorithm_t alg,
                           const uint8_t *hash,
                           size_t hash_length,
                           uint8_t *signature,
                           size_t signature_size,
                           size_t *signature_length)
{
    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = {
        .func           = PSA_SIGN_HASH,
        .handle         = handle,
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

psa_status_t psa_verify_hash(psa_key_handle_t handle,
                             psa_algorithm_t alg,
                             const uint8_t *hash,
                             size_t hash_length,
                             const uint8_t *signature,
                             size_t signature_size)
{
    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = {
        .func           = PSA_VERIFY_HASH,
        .handle         = handle,
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
                                             psa_key_handle_t handle,
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
        .handle         = handle,
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

psa_status_t psa_asymmetric_encrypt(psa_key_handle_t handle,
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
                                                   handle,
                                                   alg, input, input_length,
                                                   salt, salt_length, output,
                                                   output_size, output_length);
    return (status);
}

psa_status_t psa_asymmetric_decrypt(psa_key_handle_t handle,
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
                                                   handle,
                                                   alg, input, input_length,
                                                   salt, salt_length, output,
                                                   output_size, output_length);
    return (status);
}

psa_status_t psa_key_derivation_setup(
    psa_key_derivation_operation_t *operation,
    psa_algorithm_t alg)
{
    if (operation->handle != PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func   = PSA_KEY_DERIVATION_SETUP,
        .handle = 0,
        .alg    = alg
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_status_t status = ipc_connect(PSA_KEY_DERIVATION_ID, &operation->handle);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = ipc_call(&operation->handle, &in_vec, 1, NULL, 0, false);
    if (status != PSA_SUCCESS) {
        ipc_close(&operation->handle);
    }

    return status;
}

psa_status_t psa_key_derivation_get_capacity(
    const psa_key_derivation_operation_t *op,
    size_t *capacity)
{
    psa_key_derivation_operation_t *operation = (psa_key_derivation_operation_t *) op;

    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_KEY_DERIVATION_GET_CAPACITY,
        .handle     = 0,
        .alg        = 0,
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_outvec out_vec = { capacity, sizeof(*capacity) };

    return ipc_call(&operation->handle, &in_vec, 1, &out_vec, 1, false);
}

psa_status_t psa_key_derivation_set_capacity(
    psa_key_derivation_operation_t *operation,
    size_t capacity)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_KEY_DERIVATION_SET_CAPACITY,
        .handle     = 0,
        .alg        = 0,
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &capacity, sizeof(capacity) },
    };

    return ipc_call(&operation->handle, in_vec, 2, NULL, 0, false);
}

psa_status_t psa_key_derivation_input_bytes(
    psa_key_derivation_operation_t *operation,
    psa_key_derivation_step_t step,
    const uint8_t *data,
    size_t data_length)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_KEY_DERIVATION_INPUT_BYTES,
        .handle     = 0,
        .alg        = 0,
    };

    psa_invec in_vec[3] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &step, sizeof(step) },
        { data, data_length },
    };

    return ipc_call(&operation->handle, in_vec, 3, NULL, 0, false);
}

psa_status_t psa_key_derivation_input_key(
    psa_key_derivation_operation_t *operation,
    psa_key_derivation_step_t step,
    psa_key_handle_t handle)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_KEY_DERIVATION_INPUT_KEY,
        .handle     = handle,
        .alg        = 0
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &step, sizeof(step) },
    };

    psa_status_t status = ipc_call(&operation->handle, in_vec, 2, NULL, 0, false);
    return (status);
}

psa_status_t psa_key_derivation_key_agreement(
    psa_key_derivation_operation_t *operation,
    psa_key_derivation_step_t step,
    psa_key_handle_t private_key,
    const uint8_t *peer_key,
    size_t peer_key_length)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_KEY_DERIVATION_KEY_AGREEMENT,
        .handle     = private_key,
        .alg        = 0,
    };

    psa_invec in_vec[3] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { &step, sizeof(step) },
        { peer_key, peer_key_length },
    };

    return ipc_call(&operation->handle, in_vec, 3, NULL, 0, false);
}

psa_status_t psa_key_derivation_output_bytes(
    psa_key_derivation_operation_t *operation,
    uint8_t *output,
    size_t output_length)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_KEY_DERIVATION_OUTPUT_BYTES,
        .handle     = 0,
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    psa_outvec out_vec = { output, output_length };

    return ipc_call(&operation->handle, &in_vec, 1, &out_vec, 1, false);
}

psa_status_t psa_key_derivation_output_key(
    const psa_key_attributes_t *attributes,
    psa_key_derivation_operation_t *operation,
    psa_key_handle_t *handle)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_KEY_DERIVATION_OUTPUT_KEY,
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { attributes, sizeof(*attributes) },
    };

    psa_outvec out_vec = { handle, sizeof(*handle) };

    return ipc_call(&operation->handle, in_vec, 2, &out_vec, 1, false);
}

psa_status_t psa_key_derivation_abort(
    psa_key_derivation_operation_t *operation)
{
    if (operation->handle <= PSA_NULL_HANDLE) {
        return PSA_SUCCESS;
    }

    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func       = PSA_KEY_DERIVATION_ABORT,
        .handle     = 0,
        .alg        = 0,
    };

    psa_invec in_vec = { &psa_crypto_ipc, sizeof(psa_crypto_ipc) };

    return ipc_call(&operation->handle, &in_vec, 1, NULL, 0, true);
}

psa_status_t psa_raw_key_agreement(psa_algorithm_t alg,
                                   psa_key_handle_t private_key,
                                   const uint8_t *peer_key,
                                   size_t peer_key_length,
                                   uint8_t *output,
                                   size_t output_size,
                                   size_t *output_length)
{
    psa_crypto_derivation_ipc_t psa_crypto_ipc = {
        .func           = PSA_RAW_KEY_AGREEMENT,
        .handle         = private_key,
        .alg            = alg,
    };

    psa_invec in_vec[2] = {
        { &psa_crypto_ipc, sizeof(psa_crypto_ipc) },
        { peer_key, peer_key_length },
    };

    psa_outvec out_vec[2] = {
        { output, output_size },
        { output_length, sizeof(*output_length) }
    };

    return ipc_oneshot(PSA_KEY_DERIVATION_ID, in_vec, 2, out_vec, 2);
}

psa_status_t psa_generate_random(uint8_t *output,
                                 size_t output_size)
{
    psa_outvec out_vec = { output, output_size };

    psa_status_t status = ipc_oneshot(PSA_RNG_ID, NULL, 0, &out_vec, 1);
    return (status);
}

psa_status_t psa_generate_key(const psa_key_attributes_t *attributes,
                              psa_key_handle_t *handle)
{
    psa_key_mng_ipc_t psa_key_mng_ipc = {
        .func   = PSA_GENERATE_KEY,
    };

    psa_invec in_vec[2] = {
        { &psa_key_mng_ipc, sizeof(psa_key_mng_ipc) },
        { attributes, sizeof(*attributes) },
    };

    psa_outvec out_vec = { handle, sizeof(*handle) };

    return ipc_oneshot(PSA_KEY_MNG_ID, in_vec, 2, &out_vec, 1);
}


/*
 * PSA Crypto API extensions (crypto_extra.h)
 */

void mbedtls_psa_crypto_free(void)
{
    ipc_oneshot(PSA_CRYPTO_FREE_ID, NULL, 0, NULL, 0);
}

psa_status_t mbedtls_psa_inject_entropy(const uint8_t *seed,
                                        size_t seed_size)
{
    psa_invec in_vec = { seed, seed_size };

    psa_status_t status = ipc_oneshot(PSA_ENTROPY_ID, &in_vec, 1, NULL, 0);
    return (status);
}

#endif /* MBEDTLS_PSA_CRYPTO_C */
