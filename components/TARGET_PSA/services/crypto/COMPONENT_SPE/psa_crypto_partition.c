/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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
// ---------------------------------- Includes ---------------------------------
#include <stdint.h>
#include <string.h>
#include "psa/client.h"
#include "psa/service.h"


#define PSA_CRYPTO_SECURE 1
#include "crypto_spe.h"
#include "crypto_platform_spe.h"
#include "mbed_spm_partitions.h"
#include "mbedtls/entropy.h"
#include "psa_crypto_access_control.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#define mbedtls_calloc calloc
#define mbedtls_free   free
#endif

#include "mbed_assert.h"

// ---------------------------------- Macros -----------------------------------
#if !defined(MIN)
#define MIN( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif

// -------------------------------- Structures ---------------------------------
typedef struct psa_spm_hash_clone_s {
    int32_t partition_id;
    void *source_operation;
    uint8_t ref_count;
} psa_spm_hash_clone_t;

// ---------------------------------- Globals ----------------------------------
static int psa_spm_init_count = 0;

/* maximal memory allocation for reading large hash or mac input buffers.
the data will be read in chunks of size */
#if !defined (MAX_DATA_CHUNK_SIZE_IN_BYTES)
#define MAX_DATA_CHUNK_SIZE_IN_BYTES 400
#endif

#ifndef MAX_CONCURRENT_HASH_CLONES
#define MAX_CONCURRENT_HASH_CLONES 2
#endif
static psa_spm_hash_clone_t psa_spm_hash_clones[MAX_CONCURRENT_HASH_CLONES];

#define CLIENT_PSA_KEY_ID_SIZE_IN_BYTES 4
MBED_STATIC_ASSERT(sizeof(psa_key_id_t) != CLIENT_PSA_KEY_ID_SIZE_IN_BYTES, "Unexpected psa_key_id_t size");

// ------------------------- Internal Helper Functions -------------------------
static inline psa_status_t reserve_hash_clone(int32_t partition_id, void *source_operation, size_t *index)
{
    /* check if the the clone request source operation is already part of another active clone operation,
     * for the same source, if so then reuse it and increment its ref_count by 1. A scenario as such may happen
     * in case there was a context switch between calls of PSA_HASH_CLONE_BEGIN and PSA_HASH_CLONE_END (on the
     * client side) leading to PSA_HASH_CLONE_BEGIN being executed more than one time without a call to
     * PSA_HASH_CLONE_END */
    for (*index = 0; *index < MAX_CONCURRENT_HASH_CLONES; (*index)++) {
        if (psa_spm_hash_clones[*index].partition_id == partition_id &&
                psa_spm_hash_clones[*index].source_operation == source_operation) {
            psa_spm_hash_clones[*index].ref_count++;
            return PSA_SUCCESS;
        }
    }

    /* find an available empty entry in the array */
    for (*index = 0; *index < MAX_CONCURRENT_HASH_CLONES; (*index)++) {
        if (psa_spm_hash_clones[*index].partition_id == 0 &&
                psa_spm_hash_clones[*index].source_operation == NULL) {
            psa_spm_hash_clones[*index].partition_id = partition_id;
            psa_spm_hash_clones[*index].source_operation = source_operation;
            psa_spm_hash_clones[*index].ref_count++;
            return PSA_SUCCESS;
        }
    }

    return PSA_ERROR_BAD_STATE;
}

static inline void release_hash_clone(psa_spm_hash_clone_t *hash_clone)
{
    hash_clone->ref_count--;
    if (hash_clone->ref_count == 0) {
        hash_clone->partition_id = 0;
        hash_clone->source_operation = NULL;
    }
}

static void clear_hash_clone(void *source_operation)
{
    for (size_t i = 0; i < MAX_CONCURRENT_HASH_CLONES; i++) {
        if (psa_spm_hash_clones[i].source_operation == source_operation) {
            psa_spm_hash_clones[i].partition_id = 0;
            psa_spm_hash_clones[i].source_operation = NULL;
            psa_spm_hash_clones[i].ref_count = 0;
            break;
        }
    }
}

static inline psa_status_t get_hash_clone(size_t index, int32_t partition_id,
                                          psa_spm_hash_clone_t **hash_clone)
{
    if (index >= MAX_CONCURRENT_HASH_CLONES ||
            psa_spm_hash_clones[index].partition_id != partition_id ||
            psa_spm_hash_clones[index].source_operation == NULL) {
        return PSA_ERROR_BAD_STATE;
    }

    *hash_clone = &psa_spm_hash_clones[index];
    return PSA_SUCCESS;
}

static void free_message_context(psa_msg_t *msg)
{
    mbedtls_free(msg->rhandle);
    psa_set_rhandle(msg->handle, NULL);
}

static void read_attributes(psa_handle_t handle,
                            psa_key_owner_id_t owner,
                            psa_key_attributes_t *attributes)
{
    uint32_t bytes_read;
    psa_client_key_attributes_t client;

    bytes_read = psa_read(handle, 1, &client, sizeof(client));
    if (bytes_read != sizeof(client)) {
        SPM_PANIC("SPM read length mismatch");
    }
    /* We currently don't support domain parameters */
    attributes->domain_parameters = NULL;
    attributes->domain_parameters_size = 0;
    psa_core_attributes_to_server(&client.core, owner, &attributes->core);
}

// -------------------- Unimplemented PSA Crypto functions --------------------
psa_status_t psa_cipher_encrypt(psa_key_handle_t handle,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_cipher_decrypt(psa_key_handle_t handle,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_hash_compute(psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              uint8_t *hash,
                              size_t hash_size,
                              size_t *hash_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_hash_compare(psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              const uint8_t *hash,
                              const size_t hash_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_mac_compute(psa_key_handle_t handle,
                             psa_algorithm_t alg,
                             const uint8_t *input,
                             size_t input_length,
                             uint8_t *mac,
                             size_t mac_size,
                             size_t *mac_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_mac_verify(psa_key_handle_t handle,
                            psa_algorithm_t alg,
                            const uint8_t *input,
                            size_t input_length,
                            const uint8_t *mac,
                            const size_t mac_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_aead_encrypt_setup(psa_aead_operation_t *operation,
                                    psa_key_handle_t handle,
                                    psa_algorithm_t alg)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_aead_decrypt_setup(psa_aead_operation_t *operation,
                                    psa_key_handle_t handle,
                                    psa_algorithm_t alg)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_aead_generate_nonce(psa_aead_operation_t *operation,
                                     uint8_t *nonce,
                                     size_t nonce_size,
                                     size_t *nonce_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_aead_set_nonce(psa_aead_operation_t *operation,
                                const uint8_t *nonce,
                                size_t nonce_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_aead_set_lengths(psa_aead_operation_t *operation,
                                  size_t ad_length,
                                  size_t plaintext_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_aead_update_ad(psa_aead_operation_t *operation,
                                const uint8_t *input,
                                size_t input_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_aead_update(psa_aead_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length,
                             uint8_t *output,
                             size_t output_size,
                             size_t *output_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_aead_finish(psa_aead_operation_t *operation,
                             uint8_t *ciphertext,
                             size_t ciphertext_size,
                             size_t *ciphertext_length,
                             uint8_t *tag,
                             size_t tag_size,
                             size_t *tag_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_aead_verify(psa_aead_operation_t *operation,
                             uint8_t *plaintext,
                             size_t plaintext_size,
                             size_t *plaintext_length,
                             const uint8_t *tag,
                             size_t tag_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_aead_abort(psa_aead_operation_t *operation)
{
    return PSA_ERROR_NOT_SUPPORTED;
}


// ------------------------- Partition's Main Thread ---------------------------
static void psa_crypto_init_operation(void)
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_CRYPTO_INIT, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
            break;
        }

        case PSA_IPC_CALL: {
            status = psa_crypto_init();
            if (status == PSA_SUCCESS) {
                ++psa_spm_init_count;
                if (psa_spm_init_count == 1) {
                    memset(psa_spm_hash_clones, 0, sizeof(psa_spm_hash_clones));
                    psa_crypto_access_control_init();
                }
            }

            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}

static void psa_crypto_free_operation(void)
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_CRYPTO_FREE, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
            break;
        }

        case PSA_IPC_CALL: {
            /** perform crypto_free iff the number of init-s
             * is equal to the number of free-s
             */
            if (psa_spm_init_count > 0) {
                --psa_spm_init_count;
            }

            if (psa_spm_init_count == 0) {
                memset(psa_spm_hash_clones, 0, sizeof(psa_spm_hash_clones));
                psa_crypto_access_control_destroy();
                mbedtls_psa_crypto_free();
            }

            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}

static void psa_mac_operation(void)
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_MAC, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT: {
            psa_mac_operation_t *psa_operation = mbedtls_calloc(1, sizeof(psa_mac_operation_t));
            if (psa_operation == NULL) {
                status = PSA_CONNECTION_REFUSED;
                break;
            }

            psa_set_rhandle(msg.handle, psa_operation);
            break;
        }

        case PSA_IPC_CALL: {
            uint32_t bytes_read;
            psa_crypto_ipc_t psa_crypto = { 0 };

            if (msg.in_size[0] != sizeof(psa_crypto_ipc_t)) {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            bytes_read = psa_read(msg.handle, 0, &psa_crypto, msg.in_size[0]);
            if (bytes_read != msg.in_size[0]) {
                SPM_PANIC("SPM read length mismatch");
            }

            switch (psa_crypto.func) {
                case PSA_MAC_COMPUTE: {
                    uint8_t *input = NULL;
                    size_t input_length = msg.in_size[1];
                    uint8_t *mac = NULL;
                    size_t mac_size = msg.out_size[0];
                    size_t mac_length;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto.handle, msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    /* Read in input. */
                    if (input_length > 0) {
                        input = mbedtls_calloc(1, input_length);
                        if (input == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 1, input, input_length);
                    if (bytes_read != input_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    /* Allocate the mac output buffer. */
                    if (mac_size > 0) {
                        mac = mbedtls_calloc(1, mac_size);
                        if (mac == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            free(input);
                            break;
                        }
                    }

                    status = psa_mac_compute(psa_crypto.handle,
                                             psa_crypto.alg,
                                             input, input_length,
                                             mac, mac_size, &mac_length);
                    if (status == PSA_SUCCESS) {
                        /* Write out the mac. */
                        psa_write(msg.handle, 0, mac, mac_length);
                        psa_write(msg.handle, 1,
                                  &mac_length, sizeof(mac_length));
                    }

                    free(mac);
                    free(input);
                    break;
                }

                case PSA_MAC_VERIFY: {
                    uint8_t *input = NULL;
                    size_t input_length = msg.in_size[1];
                    uint8_t *mac = NULL;
                    size_t mac_length = msg.in_size[2];

                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto.handle, msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    /* Read in input. */
                    if (input_length > 0) {
                        input = mbedtls_calloc(1, input_length);
                        if (input == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 1, input, input_length);
                    if (bytes_read != input_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    /* Read in mac. */
                    if (mac_length > 0) {
                        mac = mbedtls_calloc(1, mac_length);
                        if (mac == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            free(input);
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 2, mac, mac_length);
                    if (bytes_read != mac_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_mac_verify(psa_crypto.handle,
                                            psa_crypto.alg,
                                            input, input_length,
                                            mac, mac_length);
                    free(mac);
                    free(input);
                    break;
                }

                case PSA_MAC_SIGN_SETUP: {
                    if (psa_crypto_access_control_is_handle_permitted(psa_crypto.handle, msg.client_id)) {
                        status = psa_mac_sign_setup(msg.rhandle, psa_crypto.handle, psa_crypto.alg);
                    } else {
                        status = PSA_ERROR_INVALID_HANDLE;
                    }

                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_MAC_VERIFY_SETUP: {
                    if (psa_crypto_access_control_is_handle_permitted(psa_crypto.handle, msg.client_id)) {
                        status = psa_mac_verify_setup(msg.rhandle, psa_crypto.handle, psa_crypto.alg);
                    } else {
                        status = PSA_ERROR_INVALID_HANDLE;
                    }

                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_MAC_UPDATE: {
                    uint8_t *input_buffer = NULL;
                    size_t data_remaining = msg.in_size[1];
                    size_t allocation_size = MIN(data_remaining, MAX_DATA_CHUNK_SIZE_IN_BYTES);
                    size_t size_to_read = 0;

                    if (allocation_size > 0) {
                        input_buffer = mbedtls_calloc(1, allocation_size);
                        if (input_buffer == NULL) {
                            psa_mac_abort(msg.rhandle);
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            while (data_remaining > 0) {
                                size_to_read = MIN(data_remaining, MAX_DATA_CHUNK_SIZE_IN_BYTES);

                                bytes_read = psa_read(msg.handle, 1, input_buffer, size_to_read);
                                if (bytes_read != size_to_read) {
                                    SPM_PANIC("SPM read length mismatch");
                                }

                                status = psa_mac_update(msg.rhandle, input_buffer, bytes_read);
                                // stop on error
                                if (status != PSA_SUCCESS) {
                                    break;
                                }
                                data_remaining = data_remaining - bytes_read;
                            }

                            mbedtls_free(input_buffer);
                        }
                    } else {
                        status = psa_mac_update(msg.rhandle, input_buffer, allocation_size);
                    }

                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_MAC_SIGN_FINISH: {
                    uint8_t *mac = NULL;
                    size_t mac_size = 0, mac_length = 0;

                    bytes_read = psa_read(msg.handle, 1, &mac_size, msg.in_size[1]);
                    if (bytes_read != msg.in_size[1]) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    if (mac_size > 0) {
                        mac = mbedtls_calloc(1, mac_size);
                        if (mac == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        status = psa_mac_sign_finish(msg.rhandle, mac, mac_size, &mac_length);
                        if (status == PSA_SUCCESS) {
                            psa_write(msg.handle, 0, mac, mac_length);
                            psa_write(msg.handle, 1, &mac_length, sizeof(mac_length));
                        }
                        mbedtls_free(mac);
                    } else {
                        psa_mac_abort(msg.rhandle);
                    }

                    free_message_context(&msg);
                    break;
                }

                case PSA_MAC_VERIFY_FINISH: {
                    uint8_t *mac = NULL;
                    size_t mac_length = 0;

                    bytes_read = psa_read(msg.handle, 1, &mac_length, msg.in_size[1]);
                    if (bytes_read != msg.in_size[1] || mac_length != msg.in_size[2]) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    if (mac_length > 0) {
                        mac = mbedtls_calloc(1, mac_length);
                        if (mac == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 2, mac, mac_length);
                            if (bytes_read != mac_length) {
                                SPM_PANIC("SPM read length mismatch");
                            }
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        status = psa_mac_verify_finish(msg.rhandle, mac, mac_length);
                        mbedtls_free(mac);
                    } else {
                        psa_mac_abort(msg.rhandle);
                    }

                    free_message_context(&msg);
                    break;
                }

                case PSA_MAC_ABORT: {
                    status = psa_mac_abort(msg.rhandle);
                    free_message_context(&msg);
                    break;
                }

                default: {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }

                break;
            }

            break;
        }

        case PSA_IPC_DISCONNECT: {
            if (msg.rhandle != NULL) {
                psa_mac_abort(msg.rhandle);
                free_message_context(&msg);
            }

            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}

static void psa_hash_operation(void)
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_HASH, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT: {
            psa_hash_operation_t *psa_operation = mbedtls_calloc(1, sizeof(psa_hash_operation_t));
            if (psa_operation == NULL) {
                status = PSA_CONNECTION_REFUSED;
                break;
            }

            psa_set_rhandle(msg.handle, psa_operation);
            break;
        }

        case PSA_IPC_CALL: {
            uint32_t bytes_read = 0;
            psa_crypto_ipc_t psa_crypto = {0};

            if (msg.in_size[0] != sizeof(psa_crypto_ipc_t)) {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            bytes_read = psa_read(msg.handle, 0, &psa_crypto, msg.in_size[0]);
            if (bytes_read != msg.in_size[0]) {
                SPM_PANIC("SPM read length mismatch");
            }

            switch (psa_crypto.func) {
                case PSA_HASH_COMPUTE: {
                    uint8_t *input = NULL;
                    size_t input_length = msg.in_size[1];
                    uint8_t *hash = NULL;
                    size_t hash_size = msg.out_size[0];
                    size_t hash_length;

                    /* Read in input. */
                    if (input_length > 0) {
                        input = mbedtls_calloc(1, input_length);
                        if (input == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 1, input, input_length);
                    if (bytes_read != input_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    /* Allocate the hash output buffer. */
                    if (hash_size > 0) {
                        hash = mbedtls_calloc(1, hash_size);
                        if (hash == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            free(input);
                            break;
                        }
                    }

                    status = psa_hash_compute(psa_crypto.alg,
                                              input, input_length,
                                              hash, hash_size, &hash_length);
                    if (status == PSA_SUCCESS) {
                        /* Write out the hash. */
                        psa_write(msg.handle, 0, hash, hash_length);
                        psa_write(msg.handle, 1,
                                  &hash_length, sizeof(hash_length));
                    }

                    free(hash);
                    free(input);
                    break;
                }

                case PSA_HASH_COMPARE: {
                    uint8_t *input = NULL;
                    size_t input_length = msg.in_size[1];
                    uint8_t *hash = NULL;
                    size_t hash_length = msg.in_size[2];

                    /* Read in input. */
                    if (input_length > 0) {
                        input = mbedtls_calloc(1, input_length);
                        if (input == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 1, input, input_length);
                    if (bytes_read != input_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    /* Read in hash. */
                    if (hash_length > 0) {
                        hash = mbedtls_calloc(1, hash_length);
                        if (hash == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            free(input);
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 2, hash, hash_length);
                    if (bytes_read != hash_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_hash_compare(psa_crypto.alg,
                                              input, input_length,
                                              hash, hash_length);
                    free(hash);
                    free(input);
                    break;
                }

                case PSA_HASH_SETUP: {
                    status = psa_hash_setup(msg.rhandle,
                                            psa_crypto.alg);
                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_HASH_UPDATE: {
                    uint8_t *input_buffer = NULL;
                    size_t data_remaining = msg.in_size[1];
                    size_t size_to_read = 0;
                    size_t allocation_size = MIN(data_remaining, MAX_DATA_CHUNK_SIZE_IN_BYTES);

                    if (allocation_size > 0) {
                        input_buffer = mbedtls_calloc(1, allocation_size);
                        if (input_buffer == NULL) {
                            psa_hash_abort(msg.rhandle);
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            while (data_remaining > 0) {
                                size_to_read = MIN(data_remaining, MAX_DATA_CHUNK_SIZE_IN_BYTES);

                                bytes_read = psa_read(msg.handle, 1, input_buffer, size_to_read);
                                if (bytes_read != size_to_read) {
                                    SPM_PANIC("SPM read length mismatch");
                                }

                                status = psa_hash_update(msg.rhandle, input_buffer, bytes_read);
                                // stop on error
                                if (status != PSA_SUCCESS) {
                                    break;
                                }
                                data_remaining = data_remaining - bytes_read;
                            }

                            mbedtls_free(input_buffer);
                        }
                    } else {
                        status = psa_hash_update(msg.rhandle, input_buffer, allocation_size);
                    }

                    if (status != PSA_SUCCESS) {
                        clear_hash_clone(msg.rhandle);
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_HASH_FINISH: {
                    uint8_t *hash = NULL;
                    size_t hash_size = 0, hash_length = 0;

                    bytes_read = psa_read(msg.handle, 1, &hash_size, msg.in_size[1]);
                    if (bytes_read != msg.in_size[1]) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    if (hash_size > 0) {
                        hash = mbedtls_calloc(1, hash_size);
                        if (hash == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        status = psa_hash_finish(msg.rhandle, hash, hash_size, &hash_length);
                        if (status == PSA_SUCCESS) {
                            psa_write(msg.handle, 0, hash, hash_length);
                            psa_write(msg.handle, 1, &hash_length, sizeof(hash_length));
                        }
                        mbedtls_free(hash);
                    } else {
                        psa_hash_abort(msg.rhandle);
                    }

                    clear_hash_clone(msg.rhandle);
                    free_message_context(&msg);
                    break;
                }

                case PSA_HASH_VERIFY: {
                    uint8_t *hash = NULL;
                    size_t hash_length = 0;

                    bytes_read = psa_read(msg.handle, 1, &hash_length, msg.in_size[1]);
                    if (bytes_read != msg.in_size[1] || hash_length != msg.in_size[2]) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    if (hash_length > 0) {
                        hash = mbedtls_calloc(1, hash_length);
                        if (hash == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 2, hash, hash_length);
                            if (bytes_read != hash_length) {
                                SPM_PANIC("SPM read length mismatch");
                            }
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        status = psa_hash_verify(msg.rhandle, hash, hash_length);
                        mbedtls_free(hash);
                    } else {
                        psa_hash_abort(msg.rhandle);
                    }

                    clear_hash_clone(msg.rhandle);
                    free_message_context(&msg);
                    break;
                }

                case PSA_HASH_ABORT: {
                    status = psa_hash_abort(msg.rhandle);
                    clear_hash_clone(msg.rhandle);
                    free_message_context(&msg);
                    break;
                }

                case PSA_HASH_CLONE_BEGIN: {
                    size_t index = 0;
                    status = reserve_hash_clone(msg.client_id, msg.rhandle, &index);
                    if (status == PSA_SUCCESS) {
                        psa_write(msg.handle, 0, &index, sizeof(index));
                    }
                    break;
                }

                case PSA_HASH_CLONE_END: {
                    psa_spm_hash_clone_t *hash_clone = NULL;
                    size_t index;

                    bytes_read = psa_read(msg.handle, 1, &index, msg.in_size[1]);
                    if (bytes_read != msg.in_size[1]) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = get_hash_clone(index, msg.client_id, &hash_clone);
                    if (status == PSA_SUCCESS) {
                        status = psa_hash_clone(hash_clone->source_operation, msg.rhandle);
                        release_hash_clone(hash_clone);
                    }
                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                default: {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }
            }

            break;
        }

        case PSA_IPC_DISCONNECT: {
            if (msg.rhandle != NULL) {
                psa_hash_abort(msg.rhandle);
                clear_hash_clone(msg.rhandle);
                free_message_context(&msg);
            }

            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}

static void psa_asymmetric_operation(void)
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_ASYMMETRIC, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
            break;
        }

        case PSA_IPC_CALL: {
            if (msg.in_size[0] != sizeof(psa_crypto_ipc_asymmetric_t)) {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            uint32_t bytes_read = 0;
            psa_crypto_ipc_asymmetric_t psa_crypto = {0};

            bytes_read = psa_read(msg.handle, 0, &psa_crypto, msg.in_size[0]);
            if (bytes_read != msg.in_size[0]) {
                SPM_PANIC("SPM read length mismatch");
            }

            if (!psa_crypto_access_control_is_handle_permitted(psa_crypto.handle,
                                                               msg.client_id)) {
                status = PSA_ERROR_INVALID_HANDLE;
                break;
            }

            switch (psa_crypto.func) {
                case PSA_SIGN_HASH: {
                    uint8_t *signature = NULL;
                    uint8_t *hash = NULL;
                    size_t signature_length = 0,
                           signature_size = msg.out_size[0],
                           hash_size = msg.in_size[1];

                    if (signature_size > 0) {
                        signature = mbedtls_calloc(1, signature_size);
                        if (signature == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                    }
                    if (status == PSA_SUCCESS && hash_size > 0) {
                        hash = mbedtls_calloc(1, hash_size);
                        if (hash == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 1, hash, hash_size);
                            if (bytes_read != hash_size) {
                                SPM_PANIC("SPM read length mismatch");
                            }
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        status = psa_sign_hash(psa_crypto.handle, psa_crypto.alg,
                                               hash, hash_size,
                                               signature, signature_size, &signature_length);

                        if (status == PSA_SUCCESS) {
                            psa_write(msg.handle, 0, signature, signature_length);
                        }
                        psa_write(msg.handle, 1, &signature_length, sizeof(signature_length));
                    }

                    mbedtls_free(hash);
                    mbedtls_free(signature);
                    break;
                }

                case PSA_VERIFY_HASH: {
                    uint8_t *signature = NULL;
                    uint8_t *hash = NULL;
                    size_t signature_size = msg.in_size[1],
                           hash_size = msg.in_size[2];

                    if (signature_size > 0) {
                        signature = mbedtls_calloc(1, signature_size);
                        if (signature == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 1, signature, signature_size);
                            if (bytes_read != signature_size) {
                                SPM_PANIC("SPM read length mismatch");
                            }
                        }
                    }
                    if (status == PSA_SUCCESS && hash_size > 0) {
                        hash = mbedtls_calloc(1, hash_size);
                        if (hash == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 2, hash, hash_size);
                            if (bytes_read != hash_size) {
                                SPM_PANIC("SPM read length mismatch");
                            }
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        status = psa_verify_hash(psa_crypto.handle, psa_crypto.alg,
                                                 hash, hash_size,
                                                 signature, signature_size);
                    }

                    mbedtls_free(signature);
                    mbedtls_free(hash);
                    break;
                }

                case PSA_ASYMMETRIC_ENCRYPT:
                case PSA_ASYMMETRIC_DECRYPT: {
                    uint8_t *input = NULL, *salt = NULL, *output = NULL, *buffer = NULL;
                    size_t output_length = 0,
                           buffer_size = msg.in_size[1],
                           output_size = msg.out_size[0];

                    if (buffer_size > 0) {
                        buffer = mbedtls_calloc(1, buffer_size);
                        if (buffer == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 1, buffer, buffer_size);
                            if (bytes_read != buffer_size) {
                                SPM_PANIC("SPM read length mismatch");
                            }

                            input = buffer;
                            salt = buffer + psa_crypto.input_length;
                        }
                    }
                    if (status == PSA_SUCCESS && output_size > 0) {
                        output = mbedtls_calloc(1, output_size);
                        if (output == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        if (psa_crypto.func == PSA_ASYMMETRIC_ENCRYPT) {
                            status = psa_asymmetric_encrypt(psa_crypto.handle, psa_crypto.alg,
                                                            input, psa_crypto.input_length,
                                                            salt, psa_crypto.salt_length,
                                                            output, output_size, &output_length);
                        } else {
                            status = psa_asymmetric_decrypt(psa_crypto.handle, psa_crypto.alg,
                                                            input, psa_crypto.input_length,
                                                            salt, psa_crypto.salt_length,
                                                            output, output_size, &output_length);
                        }

                        if (status == PSA_SUCCESS) {
                            psa_write(msg.handle, 0, output, output_length);
                        }
                        psa_write(msg.handle, 1, &output_length, sizeof(output_length));
                    }

                    mbedtls_free(output);
                    mbedtls_free(buffer);
                    break;
                }

                default: {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }
            }

            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}

static void psa_aead_operation()
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_AEAD, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT: {
            psa_aead_operation_t *psa_operation =
                mbedtls_calloc(1, sizeof(*psa_operation));
            if (psa_operation == NULL) {
                status = PSA_CONNECTION_REFUSED;
                break;
            }

            psa_set_rhandle(msg.handle, psa_operation);
            break;
        }

        case PSA_IPC_CALL: {
            if (msg.in_size[0] != sizeof(psa_crypto_ipc_aead_t)) {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            uint32_t bytes_read = 0;
            psa_crypto_ipc_aead_t psa_crypto = {0};

            bytes_read = psa_read(msg.handle, 0, &psa_crypto, msg.in_size[0]);
            if (bytes_read != msg.in_size[0]) {
                SPM_PANIC("SPM read length mismatch");
            }

            switch (psa_crypto.func) {
                case PSA_AEAD_ENCRYPT:
                case PSA_AEAD_DECRYPT: {
                    uint8_t *input = NULL, *additional_data = NULL, *output = NULL, *buffer = NULL;
                    size_t output_length = 0,
                           buffer_size = msg.in_size[1],
                           output_size = msg.out_size[0];

                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto.handle,
                                                                       msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    if (buffer_size > 0) {
                        buffer = mbedtls_calloc(1, buffer_size);
                        if (buffer == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 1, buffer, buffer_size);
                            if (bytes_read != buffer_size) {
                                SPM_PANIC("SPM read length mismatch");
                            }

                            additional_data = buffer;
                            input = buffer + psa_crypto.additional_data_length;
                        }
                    }
                    if (status == PSA_SUCCESS && output_size > 0) {
                        output = mbedtls_calloc(1, output_size);
                        if (output == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        if (psa_crypto.func == PSA_AEAD_ENCRYPT) {
                            status = psa_aead_encrypt(psa_crypto.handle, psa_crypto.alg,
                                                      psa_crypto.nonce, (size_t)psa_crypto.nonce_size,
                                                      additional_data, psa_crypto.additional_data_length,
                                                      input, psa_crypto.input_length,
                                                      output, output_size, &output_length);
                        } else {
                            status = psa_aead_decrypt(psa_crypto.handle, psa_crypto.alg,
                                                      psa_crypto.nonce, (size_t)psa_crypto.nonce_size,
                                                      additional_data, psa_crypto.additional_data_length,
                                                      input, psa_crypto.input_length,
                                                      output, output_size, &output_length);
                        }

                        if (status == PSA_SUCCESS) {
                            psa_write(msg.handle, 0, output, output_length);
                            psa_write(msg.handle, 1, &output_length, sizeof(output_length));
                        }
                    }

                    mbedtls_free(buffer);
                    mbedtls_free(output);
                    break;
                }

                case PSA_AEAD_ENCRYPT_SETUP: {
                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto.handle,
                                                                       msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    status = psa_aead_encrypt_setup(msg.rhandle,
                                                    psa_crypto.handle,
                                                    psa_crypto.alg);
                    break;
                }

                case PSA_AEAD_DECRYPT_SETUP: {
                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto.handle,
                                                                       msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }
                    status = psa_aead_decrypt_setup(msg.rhandle,
                                                    psa_crypto.handle,
                                                    psa_crypto.alg);

                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }

                case PSA_AEAD_GENERATE_NONCE: {
                    uint8_t *nonce;
                    size_t nonce_size = msg.out_size[0];
                    size_t nonce_length;

                    /* Allocate the nonce buffer. */
                    if (nonce_size > 0) {
                        nonce = mbedtls_calloc(1, nonce_size);
                        if (nonce == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }

                    status = psa_aead_generate_nonce(
                                 msg.rhandle,
                                 nonce, nonce_size, &nonce_length);
                    if (status == PSA_SUCCESS) {
                        /* Write out the nonce. */
                        psa_write(msg.handle, 0, nonce, nonce_length);
                        psa_write(msg.handle, 1, &nonce_length, sizeof(nonce_length));
                    }
                    free(nonce);
                    break;
                }

                case PSA_AEAD_SET_NONCE: {
                    uint8_t *nonce = NULL;
                    size_t nonce_length = msg.in_size[1];

                    /* Read in the nonce. */
                    if (nonce_length > 0) {
                        nonce = mbedtls_calloc(1, nonce_length);
                        if (nonce == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 1, nonce, nonce_length);
                    if (bytes_read != nonce_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_aead_set_nonce(
                                 msg.rhandle, nonce, nonce_length);
                    free(nonce);
                    break;
                }

                case PSA_AEAD_SET_LENGTHS: {
                    status = psa_aead_set_lengths(
                                 msg.rhandle,
                                 psa_crypto.additional_data_length,
                                 psa_crypto.input_length);
                    break;
                }

                case PSA_AEAD_UPDATE_AD: {
                    uint8_t *input = NULL;
                    size_t input_length = msg.in_size[1];

                    /* Read in input. */
                    if (input_length > 0) {
                        input = mbedtls_calloc(1, input_length);
                        if (input == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 1, input, input_length);
                    if (bytes_read != input_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_aead_update_ad(
                                 msg.rhandle, input, input_length);

                    free(input);
                    break;
                }

                case PSA_AEAD_UPDATE: {
                    uint8_t *input;
                    size_t input_length = msg.in_size[1];
                    uint8_t *output;
                    size_t output_size = msg.out_size[0];
                    size_t output_length;

                    /* Read in input. */
                    if (input_length > 0) {
                        input = mbedtls_calloc(1, input_length);
                        if (input == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 1, input, input_length);
                    if (bytes_read != input_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    /* Allocate the output buffer. */
                    if (output_size > 0) {
                        output = mbedtls_calloc(1, output_size);
                        if (output == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            free(input);
                            break;
                        }
                    }

                    status = psa_aead_update(
                                 msg.rhandle, input, input_length,
                                 output, output_size, &output_length);
                    if (status == PSA_SUCCESS) {
                        /* Write out the output. */
                        psa_write(msg.handle, 0, output, output_length);
                        psa_write(msg.handle, 1, &output_length, sizeof(output_length));
                    }
                    free(output);
                    free(input);
                    break;
                }

                case PSA_AEAD_FINISH: {
                    uint8_t *ciphertext = NULL;
                    size_t ciphertext_size = msg.out_size[0];
                    size_t ciphertext_length;
                    uint8_t *tag = NULL;
                    size_t tag_size = msg.out_size[2];
                    size_t tag_length;

                    /* Allocate ciphertext. */
                    if (ciphertext_size > 0) {
                        ciphertext = mbedtls_calloc(1, ciphertext_size);
                        if (ciphertext == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }

                    /* Allocate tag. */
                    if (tag_size > 0) {
                        tag = mbedtls_calloc(1, tag_size);
                        if (tag == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            free(ciphertext);
                            break;
                        }
                    }

                    status = psa_aead_finish(msg.rhandle,
                                             ciphertext,
                                             ciphertext_size,
                                             &ciphertext_length,
                                             tag,
                                             tag_size,
                                             &tag_length);
                    if (status == PSA_SUCCESS) {
                        /* Write out ciphertext. */
                        if (ciphertext_size > 0) {
                            psa_write(msg.handle, 0, ciphertext,
                                      ciphertext_length);
                            psa_write(msg.handle, 1, &ciphertext_length,
                                      sizeof(ciphertext_length));
                        }

                        /* Write out tag. */
                        if (tag_size > 0) {
                            psa_write(msg.handle, 2, tag, tag_length);
                            psa_write(msg.handle, 3, &tag_length,
                                      sizeof(tag_length));
                        }
                    }
                    free(tag);
                    free(ciphertext);
                    break;
                }

                case PSA_AEAD_VERIFY: {
                    uint8_t *plaintext = NULL;
                    size_t plaintext_size = msg.out_size[0];
                    size_t plaintext_length;
                    uint8_t *tag = NULL;
                    size_t tag_length = msg.in_size[1];

                    /* Allocate plaintext. */
                    if (plaintext_size > 0) {
                        plaintext = mbedtls_calloc(1, plaintext_size);
                        if (plaintext == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }

                    /* Read in tag. */
                    if (tag_length > 0) {
                        tag = mbedtls_calloc(1, tag_length);
                        if (tag == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            free(plaintext);
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 1, tag, tag_length);
                    if (bytes_read != tag_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_aead_verify(msg.rhandle,
                                             plaintext,
                                             plaintext_size,
                                             &plaintext_length,
                                             tag,
                                             tag_length);
                    if (status == PSA_SUCCESS) {
                        /* Write out the plaintext. */
                        psa_write(msg.handle, 0, plaintext, plaintext_length);
                        psa_write(msg.handle, 1, &plaintext_length,
                                  sizeof(plaintext_length));
                    }

                    free(tag);
                    free(plaintext);
                    break;
                }

                case PSA_AEAD_ABORT: {
                    status = psa_aead_abort(msg.rhandle);
                    free_message_context(&msg);
                    break;
                }

                default: {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }
            }

            break;
        }

        case PSA_IPC_DISCONNECT: {
            if (msg.rhandle != NULL) {
                psa_aead_abort(msg.rhandle);
                free_message_context(&msg);
            }
            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}

static void psa_symmetric_operation(void)
{
    psa_status_t status = PSA_SUCCESS;
    psa_msg_t msg = { 0 };

    if (PSA_SUCCESS != psa_get(PSA_SYMMETRIC, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT: {
            psa_cipher_operation_t *psa_operation =
                mbedtls_calloc(1, sizeof(psa_cipher_operation_t));
            if (psa_operation == NULL) {
                status = PSA_CONNECTION_REFUSED;
                break;
            }

            psa_set_rhandle(msg.handle, psa_operation);
            break;
        }

        case PSA_IPC_CALL: {
            uint32_t bytes_read;
            psa_crypto_ipc_t psa_crypto_ipc = { 0 };

            if (msg.in_size[0] != sizeof(psa_crypto_ipc_t)) {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            bytes_read = psa_read(msg.handle, 0, &psa_crypto_ipc,
                                  msg.in_size[0]);
            if (bytes_read != msg.in_size[0]) {
                SPM_PANIC("SPM read length mismatch");
            }

            switch (psa_crypto_ipc.func) {
                case PSA_CIPHER_ENCRYPT:
                case PSA_CIPHER_DECRYPT: {
                    uint8_t *input = NULL;
                    size_t input_length = msg.in_size[1];
                    uint8_t *output = NULL;
                    size_t output_size = msg.out_size[0];
                    size_t output_length;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto_ipc.handle, msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                    }

                    /* Read in input. */
                    if (input_length > 0) {
                        input = mbedtls_calloc(1, input_length);
                        if (input == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 1, input, input_length);
                    if (bytes_read != input_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    /* Allocate the output buffer. */
                    if (output_size > 0) {
                        output = mbedtls_calloc(1, output_size);
                        if (output == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            free(input);
                            break;
                        }
                    }

                    /* Perform encrypt or decrypt. */
                    switch (psa_crypto_ipc.func) {
                        case PSA_CIPHER_ENCRYPT:
                            status = psa_cipher_encrypt(
                                         psa_crypto_ipc.handle,
                                         psa_crypto_ipc.alg,
                                         input, input_length,
                                         output, output_size, &output_length);
                            break;
                        case PSA_CIPHER_DECRYPT:
                            status = psa_cipher_decrypt(
                                         psa_crypto_ipc.handle,
                                         psa_crypto_ipc.alg,
                                         input, input_length,
                                         output, output_size, &output_length);
                            break;
                        default:
                            SPM_PANIC("Unexpected func");
                    }

                    if (status == PSA_SUCCESS) {
                        /* Write out the output. */
                        psa_write(msg.handle, 0, output, output_length);
                        psa_write(msg.handle, 1, &output_length, sizeof(output_length));
                    }
                    free(input);
                    free(output);
                    break;
                }

                case PSA_CIPHER_ENCRYPT_SETUP: {
                    if (psa_crypto_access_control_is_handle_permitted(psa_crypto_ipc.handle, msg.client_id)) {
                        status = psa_cipher_encrypt_setup(msg.rhandle, psa_crypto_ipc.handle, psa_crypto_ipc.alg);
                    } else {
                        status = PSA_ERROR_INVALID_HANDLE;
                    }

                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_CIPHER_DECRYPT_SETUP: {
                    if (psa_crypto_access_control_is_handle_permitted(psa_crypto_ipc.handle, msg.client_id)) {
                        status = psa_cipher_decrypt_setup(msg.rhandle, psa_crypto_ipc.handle, psa_crypto_ipc.alg);
                    } else {
                        status = PSA_ERROR_INVALID_HANDLE;
                    }

                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_CIPHER_GENERATE_IV: {
                    size_t iv_length = 0;
                    size_t iv_size = msg.out_size[0];
                    unsigned char iv[PSA_AEAD_MAX_NONCE_SIZE] = { 0 };

                    status = psa_cipher_generate_iv(msg.rhandle, iv,
                                                    iv_size, &iv_length);
                    if (status == PSA_SUCCESS) {
                        psa_write(msg.handle, 0, iv, iv_length);
                        psa_write(msg.handle, 1, &iv_length,
                                  sizeof(iv_length));
                    } else {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_CIPHER_SET_IV: {
                    size_t iv_length = msg.in_size[1];
                    unsigned char iv[PSA_AEAD_MAX_NONCE_SIZE] = { 0 };

                    bytes_read = psa_read(msg.handle, 1, iv, iv_length);
                    if (bytes_read != iv_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }
                    status = psa_cipher_set_iv(msg.rhandle, iv, iv_length);
                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_CIPHER_UPDATE: {
                    size_t input_length = msg.in_size[1],
                           output_size = msg.out_size[0],
                           output_length = 0;
                    uint8_t *input = NULL;
                    unsigned char *output = NULL;

                    if (input_length > 0) {
                        input = mbedtls_calloc(1, input_length);
                        if (input == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 1, input, input_length);
                            if (bytes_read != input_length) {
                                SPM_PANIC("SPM read length mismatch");
                            }
                        }
                    }
                    if (status == PSA_SUCCESS && output_size > 0) {
                        output = mbedtls_calloc(1, output_size);
                        if (output == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        status = psa_cipher_update(msg.rhandle, input, input_length, output, output_size,
                                                   &output_length);
                        if (status == PSA_SUCCESS) {
                            psa_write(msg.handle, 0, output, output_length);
                            psa_write(msg.handle, 1, &output_length, sizeof(output_length));
                        }
                    } else {
                        psa_cipher_abort(msg.rhandle);
                    }

                    mbedtls_free(input);
                    mbedtls_free(output);
                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_CIPHER_FINISH: {
                    uint8_t *output = NULL;
                    size_t output_size = msg.out_size[0],
                           output_length = 0;

                    if (output_size > 0) {
                        output = mbedtls_calloc(1, output_size);
                        if (output == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        status = psa_cipher_finish(msg.rhandle, output, output_size, &output_length);
                        if (status == PSA_SUCCESS) {
                            psa_write(msg.handle, 0, output, output_length);
                            psa_write(msg.handle, 1, &output_length, sizeof(output_length));
                        }
                        mbedtls_free(output);
                    } else {
                        psa_cipher_abort(msg.rhandle);
                    }

                    free_message_context(&msg);
                    break;
                }

                case PSA_CIPHER_ABORT: {
                    status = psa_cipher_abort(msg.rhandle);
                    free_message_context(&msg);
                    break;
                }

                default: {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }
            }

            break;
        }

        case PSA_IPC_DISCONNECT: {
            if (msg.rhandle != NULL) {
                psa_cipher_abort(msg.rhandle);
                free_message_context(&msg);
            }

            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}


static void psa_key_management_operation(void)
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;
    int32_t partition_id = 0;

    if (PSA_SUCCESS != psa_get(PSA_KEY_MNG, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
            break;
        }

        case PSA_IPC_CALL: {
            if (msg.in_size[0] != sizeof(psa_key_mng_ipc_t)) {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            uint32_t bytes_read = 0;
            psa_key_mng_ipc_t psa_key_mng = {0};

            bytes_read = psa_read(msg.handle, 0,
                                  &psa_key_mng, msg.in_size[0]);

            if (bytes_read != msg.in_size[0]) {
                SPM_PANIC("SPM read length mismatch");
            }

            partition_id = msg.client_id;

            switch (psa_key_mng.func) {
                case PSA_GET_KEY_ATTRIBUTES: {
                    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
                    psa_client_key_attributes_t client;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle,
                                                                       partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    status = psa_get_key_attributes(psa_key_mng.handle, &attributes);
                    if (status == PSA_SUCCESS) {
                        /* We currently don't support domain parameters */
                        attributes.domain_parameters = NULL;
                        attributes.domain_parameters_size = 0;

                        psa_core_attributes_to_client(&attributes.core, &client.core);
                        psa_write(msg.handle, 0, &client, sizeof(client));
                    }

                    break;
                }

                case PSA_OPEN_KEY: {
                    psa_key_id_t id;
                    id.owner = msg.client_id;

                    bytes_read = psa_read(msg.handle, 1, &(id.key_id), msg.in_size[1]);
                    if (bytes_read != msg.in_size[1]) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    if (msg.in_size[1] != CLIENT_PSA_KEY_ID_SIZE_IN_BYTES) {
                        SPM_PANIC("Unexpected psa_key_id_t size received from client");
                    }

                    status = psa_open_key(id, &psa_key_mng.handle);
                    if (status == PSA_SUCCESS) {
                        psa_crypto_access_control_register_handle(psa_key_mng.handle, partition_id);
                        psa_write(msg.handle, 0, &psa_key_mng.handle, sizeof(psa_key_mng.handle));
                    }
                    break;
                }

                case PSA_CLOSE_KEY: {
                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle,
                                                                       partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    status = psa_close_key(psa_key_mng.handle);
                    if (status == PSA_SUCCESS) {
                        psa_crypto_access_control_unregister_handle(psa_key_mng.handle);
                    }

                    break;
                }

                case PSA_IMPORT_KEY: {
                    size_t attributes_length = msg.in_size[1];
                    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
                    size_t data_length = msg.in_size[2];
                    uint8_t *data = NULL;
                    psa_key_handle_t handle;

                    /* Read in attributes. */
                    read_attributes(msg.handle, msg.client_id, &attributes);

                    /* Read in data. */
                    if (data_length > 0) {
                        data = mbedtls_calloc(1, data_length);
                        if (data == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }
                    bytes_read = psa_read(msg.handle, 2, data, data_length);
                    if (bytes_read != data_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    /* Import the data as a key. */
                    status = psa_import_key(&attributes, data, data_length, &handle);

                    if (status == PSA_SUCCESS) {
                        /* Write out the allocated handle. */
                        psa_crypto_access_control_register_handle(handle, partition_id);
                        psa_write(msg.handle, 0, &handle, sizeof(handle));
                    }
                    mbedtls_free(data);
                    break;
                }

                case PSA_DESTROY_KEY: {
                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle,
                                                                       partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    status = psa_destroy_key(psa_key_mng.handle);
                    if (status == PSA_SUCCESS) {
                        psa_crypto_access_control_unregister_handle(psa_key_mng.handle);
                    }

                    break;
                }

                case PSA_EXPORT_KEY: {
                    uint8_t *data = NULL;
                    size_t data_size = msg.out_size[0];
                    size_t data_length;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle, partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    if (data_size > 0) {
                        data = mbedtls_calloc(1, data_size);
                        if (data == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }

                    status = psa_export_key(psa_key_mng.handle, data, data_size, &data_length);
                    if (status == PSA_SUCCESS) {
                        psa_write(msg.handle, 0, data, data_length);
                    }
                    psa_write(msg.handle, 1, &data_length, sizeof(data_length));

                    mbedtls_free(data);
                    break;
                }

                case PSA_EXPORT_PUBLIC_KEY: {
                    size_t data_size = msg.out_size[0];
                    size_t data_length;
                    uint8_t *data = NULL;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle, partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    if (data_size > 0) {
                        data = mbedtls_calloc(1, data_size);
                        if (data == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }

                    status = psa_export_public_key(psa_key_mng.handle, data, data_size, &data_length);
                    if (status == PSA_SUCCESS) {
                        psa_write(msg.handle, 0, data, data_length);
                    }
                    psa_write(msg.handle, 1, &data_length, sizeof(data_length));

                    mbedtls_free(data);
                    break;
                }

                case PSA_COPY_KEY: {
                    psa_key_handle_t target_handle;
                    psa_key_attributes_t attributes;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle, partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    /* Read in attributes. */
                    read_attributes(msg.handle, msg.client_id, &attributes);

                    status = psa_copy_key(psa_key_mng.handle, &attributes, &target_handle);
                    if (status == PSA_SUCCESS) {
                        psa_crypto_access_control_register_handle(target_handle, partition_id);
                        psa_write(msg.handle, 0, &target_handle, sizeof(target_handle));
                    }
                    break;
                }

                case PSA_GENERATE_KEY: {
                    psa_key_attributes_t attributes;
                    psa_key_handle_t handle;

                    /* Read in attributes. */
                    read_attributes(msg.handle, msg.client_id, &attributes);

                    status = psa_generate_key(&attributes, &handle);
                    if (status == PSA_SUCCESS) {
                        /* Write out the allocated handle. */
                        psa_crypto_access_control_register_handle(handle, partition_id);
                        psa_write(msg.handle, 0, &handle, sizeof(handle));
                    }
                    break;
                }

                default: {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }
            }

            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}

static void psa_entropy_operation(void)
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_ENTROPY_INJECT, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
            status = PSA_SUCCESS;
            break;
        }

        case PSA_IPC_CALL: {
#if defined(MBEDTLS_PSA_INJECT_ENTROPY)
            unsigned char *seed = NULL;
            uint32_t bytes_read;
            size_t seed_size = msg.in_size[0];
            if (MBEDTLS_ENTROPY_MAX_SEED_SIZE < seed_size) {
                status = PSA_ERROR_INVALID_ARGUMENT;
                break;
            }

            if (seed_size > 0) {
                seed = mbedtls_calloc(1, seed_size);
                if (seed == NULL) {
                    status = PSA_ERROR_INSUFFICIENT_MEMORY;
                    break;
                }
            }

            bytes_read = psa_read(msg.handle, 0, seed, seed_size);
            if (bytes_read != seed_size) {
                SPM_PANIC("SPM read length mismatch");
            }

            status = mbedtls_psa_inject_entropy(seed, seed_size);
            mbedtls_free(seed);
#else
            status = PSA_ERROR_NOT_SUPPORTED;
#endif /* MBEDTLS_PSA_INJECT_ENTROPY */
            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}


static void psa_rng_operation(void)
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_RNG, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
            break;
        }

        case PSA_IPC_CALL: {
            size_t random_size = msg.out_size[0];
            unsigned char *random = NULL;

            if (random_size > 0) {
                random = mbedtls_calloc(1, random_size);
                if (random == NULL) {
                    status = PSA_ERROR_INSUFFICIENT_MEMORY;
                    break;
                }
            }

            status = psa_generate_random(random, random_size);
            if (status == PSA_SUCCESS) {
                psa_write(msg.handle, 0, random, random_size);
            }

            mbedtls_free(random);
            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}


void psa_crypto_key_derivation_operations(void)
{
    psa_status_t status = PSA_SUCCESS;
    psa_msg_t msg = { 0 };

    if (PSA_SUCCESS != psa_get(PSA_KEY_DERIVATION, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT: {
            psa_key_derivation_operation_t *psa_operation =
                mbedtls_calloc(1, sizeof(*psa_operation));
            if (psa_operation == NULL) {
                status = PSA_ERROR_INSUFFICIENT_MEMORY;
                break;
            }

            psa_set_rhandle(msg.handle, psa_operation);
            break;
        }

        case PSA_IPC_CALL: {
            uint32_t bytes_read;
            psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0 };
            if (msg.in_size[0] != sizeof(psa_crypto_derivation_ipc_t)) {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            bytes_read = psa_read(msg.handle, 0, &psa_crypto_ipc,
                                  msg.in_size[0]);
            if (bytes_read != msg.in_size[0]) {
                SPM_PANIC("SPM read length mismatch");
            }

            switch (psa_crypto_ipc.func) {
                case PSA_KEY_DERIVATION_SETUP: {
                    status = psa_key_derivation_setup(msg.rhandle,
                                                      psa_crypto_ipc.alg);
                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_KEY_DERIVATION_GET_CAPACITY: {
                    size_t capacity = 0;

                    status = psa_key_derivation_get_capacity(msg.rhandle,
                                                             &capacity);
                    if (status == PSA_SUCCESS) {
                        psa_write(msg.handle, 0, &capacity, sizeof(capacity));
                    }
                    break;
                }

                case PSA_KEY_DERIVATION_SET_CAPACITY: {
                    size_t capacity = 0;

                    /* Read capacity */
                    bytes_read = psa_read(msg.handle, 1, &capacity,
                                          msg.in_size[1]);
                    if (bytes_read != sizeof(capacity)) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_key_derivation_set_capacity(msg.rhandle,
                                                             capacity);
                    break;
                }

                case PSA_KEY_DERIVATION_INPUT_BYTES: {
                    psa_key_derivation_step_t step;
                    uint8_t *data;
                    size_t data_length = msg.in_size[2];

                    /* Read step. */
                    bytes_read = psa_read(msg.handle, 1, &step,
                                          msg.in_size[1]);
                    if (bytes_read != sizeof(step)) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    /* Read data. */
                    data = mbedtls_calloc(1, data_length);
                    if (data == NULL) {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read(msg.handle, 2, data, data_length);
                    if (bytes_read != data_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_key_derivation_input_bytes(msg.rhandle, step,
                                                            data, data_length);

                    free(data);
                    break;
                }

                case PSA_KEY_DERIVATION_INPUT_KEY: {
                    psa_key_derivation_step_t step;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto_ipc.handle, msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    /* Read step. */
                    bytes_read = psa_read(msg.handle, 1, &step,
                                          msg.in_size[1]);
                    if (bytes_read != sizeof(step)) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_key_derivation_input_key(
                                 msg.rhandle, step, psa_crypto_ipc.handle);
                    break;
                }

                case PSA_KEY_DERIVATION_KEY_AGREEMENT: {
                    psa_key_derivation_step_t step;
                    uint8_t *peer_key;
                    size_t peer_key_length = msg.in_size[2];

                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto_ipc.handle, msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    /* Read step. */
                    bytes_read = psa_read(msg.handle, 1, &step,
                                          msg.in_size[1]);
                    if (bytes_read != sizeof(step)) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    /* Read peer_key. */
                    peer_key = mbedtls_calloc(1, peer_key_length);
                    if (peer_key == NULL) {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read(msg.handle, 2,
                                          peer_key, peer_key_length);
                    if (bytes_read != peer_key_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_key_derivation_key_agreement(
                                 msg.rhandle, step, psa_crypto_ipc.handle,
                                 peer_key, peer_key_length);

                    free(peer_key);
                    break;
                }

                case PSA_KEY_DERIVATION_OUTPUT_BYTES: {
                    uint8_t *output = NULL;
                    size_t output_length = msg.out_size[0];

                    /* Allocate the output buffer. */
                    if (output_length > 0) {
                        output = mbedtls_calloc(1, output_length);
                        if (output == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }

                    status = psa_key_derivation_output_bytes(
                                 msg.rhandle, output, output_length);
                    if (status == PSA_SUCCESS) {
                        /* Write the output. */
                        psa_write(msg.handle, 0, output, output_length);
                    }
                    free(output);
                    break;
                }

                case PSA_KEY_DERIVATION_OUTPUT_KEY: {
                    psa_key_attributes_t attributes;
                    psa_key_handle_t handle;

                    /* Read in attributes. */
                    read_attributes(msg.handle, msg.client_id, &attributes);

                    status = psa_key_derivation_output_key(
                                 &attributes, msg.rhandle, &handle);
                    if (status == PSA_SUCCESS) {
                        /* Write out the allocated handle. */
                        psa_crypto_access_control_register_handle(handle, msg.client_id);
                        psa_write(msg.handle, 0, &handle, sizeof(handle));
                    }
                    break;
                }

                case PSA_KEY_DERIVATION_ABORT: {
                    status = psa_key_derivation_abort(msg.rhandle);
                    free_message_context(&msg);
                    break;
                }

                case PSA_RAW_KEY_AGREEMENT: {
                    uint8_t *peer_key;
                    size_t peer_key_length = msg.in_size[1];
                    uint8_t *output;
                    size_t output_size = msg.out_size[0];
                    size_t output_length;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto_ipc.handle, msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    /* Read peer_key. */
                    peer_key = mbedtls_calloc(1, peer_key_length);
                    if (peer_key == NULL) {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read(msg.handle, 1,
                                          peer_key, peer_key_length);
                    if (bytes_read != peer_key_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    /* Allocate the output buffer. */
                    if (output_size > 0) {
                        output = mbedtls_calloc(1, output_size);
                        if (output == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            free(peer_key);
                            break;
                        }
                    }

                    status = psa_raw_key_agreement(psa_crypto_ipc.alg,
                                                   psa_crypto_ipc.handle,
                                                   peer_key,
                                                   peer_key_length,
                                                   output,
                                                   output_size,
                                                   &output_length);
                    if (status == PSA_SUCCESS) {
                        /* Write the output. */
                        psa_write(msg.handle, 0, output, output_length);
                        psa_write(msg.handle, 1,
                                  &output_length, sizeof(output_length));
                    }

                    free(output);
                    free(peer_key);
                    break;
                }

                default: {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }
            }

            break;
        }
        case PSA_IPC_DISCONNECT: {
            if (msg.rhandle != NULL) {
                psa_key_derivation_abort(msg.rhandle);
                free_message_context(&msg);
            }

            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}


void crypto_main(void *ptr)
{
    while (1) {
        psa_signal_t signals = 0;
        signals = psa_wait(CRYPTO_SRV_WAIT_ANY_SID_MSK, PSA_BLOCK);
        if (signals & PSA_CRYPTO_INIT) {
            psa_crypto_init_operation();
        }
        if (signals & PSA_MAC) {
            psa_mac_operation();
        }
        if (signals & PSA_HASH) {
            psa_hash_operation();
        }
        if (signals & PSA_SYMMETRIC) {
            psa_symmetric_operation();
        }
        if (signals & PSA_ASYMMETRIC) {
            psa_asymmetric_operation();
        }
        if (signals & PSA_AEAD) {
            psa_aead_operation();
        }
        if (signals & PSA_KEY_MNG) {
            psa_key_management_operation();
        }
        if (signals & PSA_RNG) {
            psa_rng_operation();
        }
        if (signals & PSA_CRYPTO_FREE) {
            psa_crypto_free_operation();
        }
        if (signals & PSA_KEY_DERIVATION) {
            psa_crypto_key_derivation_operations();
        }
        if (signals & PSA_ENTROPY_INJECT) {
            psa_entropy_operation();
        }
    }
}
