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
static int psa_spm_init_refence_counter = 0;

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
                ++psa_spm_init_refence_counter;
                if (psa_spm_init_refence_counter == 1) {
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
            if (psa_spm_init_refence_counter > 0) {
                --psa_spm_init_refence_counter;
            }

            if (psa_spm_init_refence_counter == 0) {
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
                case PSA_ASYMMETRIC_SIGN: {
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
                        status = psa_asymmetric_sign(psa_crypto.handle, psa_crypto.alg,
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

                case PSA_ASYMMETRIC_VERIFY: {
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
                        status = psa_asymmetric_verify(psa_crypto.handle, psa_crypto.alg,
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
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
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

            if (!psa_crypto_access_control_is_handle_permitted(psa_crypto.handle,
                                                               msg.client_id)) {
                status = PSA_ERROR_INVALID_HANDLE;
                break;
            }

            switch (psa_crypto.func) {
                case PSA_AEAD_ENCRYPT:
                case PSA_AEAD_DECRYPT: {
                    uint8_t *input = NULL, *additional_data = NULL, *output = NULL, *buffer = NULL;
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
                case PSA_GET_KEY_LIFETIME: {
                    size_t lifetime_length = msg.out_size[0];
                    psa_key_lifetime_t lifetime;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle,
                                                                       partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    status = psa_get_key_lifetime(psa_key_mng.handle,
                                                  &lifetime);
                    if (status == PSA_SUCCESS) {
                        psa_write(msg.handle, 0,
                                  &lifetime, lifetime_length);
                    }

                    break;
                }

                case PSA_SET_KEY_POLICY: {
                    size_t policy_length = msg.in_size[1];
                    psa_key_policy_t policy;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle,
                                                                       partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    bytes_read = psa_read(msg.handle, 1,
                                          &policy, policy_length);
                    if (bytes_read != policy_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_set_key_policy(psa_key_mng.handle, &policy);
                    break;
                }

                case PSA_GET_KEY_POLICY: {
                    size_t policy_size = msg.out_size[0];
                    psa_key_policy_t policy;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle,
                                                                       partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    status = psa_get_key_policy(psa_key_mng.handle, &policy);
                    if (status == PSA_SUCCESS) {
                        psa_write(msg.handle, 0, &policy, policy_size);
                    }

                    break;
                }

                case PSA_IMPORT_KEY: {
                    size_t key_length = msg.in_size[1];
                    uint8_t *key = NULL;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle, partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    if (key_length > 0) {
                        key = mbedtls_calloc(1, key_length);
                        if (key == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }

                    bytes_read = psa_read(msg.handle, 1, key, key_length);
                    if (bytes_read != key_length) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_import_key(psa_key_mng.handle, psa_key_mng.type, key, key_length);
                    mbedtls_free(key);
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

                case PSA_GET_KEY_INFORMATION: {
                    psa_key_type_t type = 0;
                    size_t bits = 0;

                    if (psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle,
                                                                      partition_id)) {
                        status = psa_get_key_information(psa_key_mng.handle, &type, &bits);
                    } else {
                        status = PSA_ERROR_INVALID_HANDLE;
                    }

                    if (msg.out_size[0] >= sizeof(psa_key_type_t)) {
                        psa_write(msg.handle, 0, &type, sizeof(psa_key_type_t));
                    }
                    if (msg.out_size[1] >= sizeof(size_t)) {
                        psa_write(msg.handle, 1, &bits, sizeof(size_t));
                    }

                    break;
                }

                case PSA_EXPORT_KEY: {
                    size_t key_length = msg.out_size[0];
                    size_t data_length;
                    uint8_t *key = NULL;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle, partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    if (key_length > 0) {
                        key = mbedtls_calloc(1, key_length);
                        if (key == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }

                    status = psa_export_key(psa_key_mng.handle, key, key_length, &data_length);
                    if (status == PSA_SUCCESS) {
                        psa_write(msg.handle, 0, key, data_length);
                    }
                    psa_write(msg.handle, 1, &data_length, sizeof(size_t));

                    mbedtls_free(key);
                    break;
                }

                case PSA_EXPORT_PUBLIC_KEY: {
                    size_t key_length = msg.out_size[0];
                    size_t data_length;
                    uint8_t *key = NULL;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle, partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    if (key_length > 0) {
                        key = mbedtls_calloc(1, key_length);
                        if (key == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }

                    status = psa_export_public_key(psa_key_mng.handle, key, key_length, &data_length);
                    if (status == PSA_SUCCESS) {
                        psa_write(msg.handle, 0, key, data_length);
                    }
                    psa_write(msg.handle, 1, &data_length, sizeof(size_t));

                    mbedtls_free(key);
                    break;
                }

                case PSA_GENERATE_KEY: {
                    size_t bits;
                    size_t bits_size = msg.in_size[1];
                    size_t parameter_size = msg.in_size[2];
                    uint8_t *parameter = NULL;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_key_mng.handle,
                                                                       partition_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    bytes_read = psa_read(msg.handle, 1, &bits, bits_size);
                    if (bytes_read != bits_size) {
                        SPM_PANIC("SPM read length mismatch");
                    }
                    if (PSA_KEY_TYPE_RSA_KEYPAIR == psa_key_mng.type &&
                            parameter_size != 0) {
                        parameter = mbedtls_calloc(1, parameter_size);
                        if (parameter == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }

                        bytes_read = psa_read(msg.handle, 2,
                                              parameter, parameter_size);
                        if (bytes_read != parameter_size) {
                            SPM_PANIC("SPM read length mismatch");
                        }
                    }

                    status = psa_generate_key(psa_key_mng.handle,
                                              psa_key_mng.type,
                                              bits,
                                              parameter, parameter_size);
                    mbedtls_free(parameter);
                    break;
                }

                case PSA_ALLOCATE_KEY: {
                    status = psa_allocate_key(&psa_key_mng.handle);
                    if (status == PSA_SUCCESS) {
                        psa_crypto_access_control_register_handle(psa_key_mng.handle, partition_id);
                        psa_write(msg.handle, 0, &psa_key_mng.handle, sizeof(psa_key_mng.handle));
                    }
                    break;
                }

                case PSA_CREATE_KEY: {
                    psa_key_id_t id;
                    id.owner = msg.client_id;

                    bytes_read = psa_read(msg.handle, 1, &(id.key_id), msg.in_size[1]);
                    if (bytes_read != msg.in_size[1]) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    if (msg.in_size[1] != CLIENT_PSA_KEY_ID_SIZE_IN_BYTES) {
                        SPM_PANIC("Unexpected psa_key_id_t size received from client");
                    }

                    status = psa_create_key(psa_key_mng.lifetime, id, &psa_key_mng.handle);
                    if (status == PSA_SUCCESS) {
                        psa_crypto_access_control_register_handle(psa_key_mng.handle, partition_id);
                        psa_write(msg.handle, 0, &psa_key_mng.handle, sizeof(psa_key_mng.handle));
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

                    status = psa_open_key(psa_key_mng.lifetime, id, &psa_key_mng.handle);
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


void psa_crypto_generator_operations(void)
{
    psa_status_t status = PSA_SUCCESS;
    psa_msg_t msg = { 0 };

    if (PSA_SUCCESS != psa_get(PSA_GENERATOR, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT: {
            psa_crypto_generator_t *psa_operation =
                mbedtls_calloc(1, sizeof(psa_crypto_generator_t));
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
                case PSA_GET_GENERATOR_CAPACITY: {
                    size_t capacity = 0;

                    status = psa_get_generator_capacity(msg.rhandle,
                                                        &capacity);
                    if (status == PSA_SUCCESS)
                        psa_write(msg.handle, 0, &capacity,
                                  sizeof(capacity));

                    break;
                }

                case PSA_GENERATOR_READ: {
                    uint8_t *output = NULL;
                    size_t output_length = msg.out_size[0];

                    if (output_length > 0) {
                        output = mbedtls_calloc(1, output_length);
                        if (output == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }
                    }

                    status = psa_generator_read(msg.rhandle, output, output_length);
                    if (status == PSA_SUCCESS) {
                        psa_write(msg.handle, 0, output, output_length);
                    }

                    mbedtls_free(output);
                    break;
                }

                case PSA_GENERATOR_IMPORT_KEY: {
                    psa_key_type_t type;
                    size_t bits;

                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto_ipc.handle,
                                                                       msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    bytes_read = psa_read(msg.handle, 1,
                                          &type, msg.in_size[1]);
                    if (bytes_read != sizeof(type)) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    bytes_read = psa_read(msg.handle, 2,
                                          &bits, msg.in_size[2]);
                    if (bytes_read != sizeof(bits)) {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_generator_import_key(psa_crypto_ipc.handle, type,
                                                      bits, msg.rhandle);
                    break;
                }

                case PSA_GENERATOR_ABORT: {
                    status = psa_generator_abort(msg.rhandle);
                    free_message_context(&msg);
                    break;
                }

                case PSA_KEY_DERIVATION: {
                    uint8_t *salt = NULL, *label = NULL;
                    size_t salt_size = msg.in_size[1],
                           label_size = msg.in_size[2];

                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto_ipc.handle, msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    if (salt_size > 0) {
                        salt = mbedtls_calloc(1, salt_size);
                        if (salt == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 1, salt, salt_size);
                            if (bytes_read != salt_size) {
                                SPM_PANIC("SPM read length mismatch");
                            }
                        }
                    }
                    if (status == PSA_SUCCESS && label_size > 0) {
                        label = mbedtls_calloc(1, label_size);
                        if (label == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 2, label, label_size);
                            if (bytes_read != label_size) {
                                SPM_PANIC("SPM read length mismatch");
                            }
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        status = psa_key_derivation(msg.rhandle, psa_crypto_ipc.handle, psa_crypto_ipc.alg,
                                                    salt, salt_size, label, label_size, psa_crypto_ipc.capacity);
                    }

                    mbedtls_free(salt);
                    mbedtls_free(label);
                    if (status != PSA_SUCCESS) {
                        free_message_context(&msg);
                    }
                    break;
                }

                case PSA_KEY_AGREEMENT: {
                    uint8_t *private_key = NULL;
                    size_t private_key_size = msg.in_size[1];

                    if (!psa_crypto_access_control_is_handle_permitted(psa_crypto_ipc.handle, msg.client_id)) {
                        status = PSA_ERROR_INVALID_HANDLE;
                        break;
                    }

                    if (private_key_size > 0) {
                        private_key = mbedtls_calloc(1, private_key_size);
                        if (private_key == NULL) {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        } else {
                            bytes_read = psa_read(msg.handle, 1, private_key, private_key_size);
                            if (bytes_read != private_key_size) {
                                SPM_PANIC("SPM read length mismatch");
                            }
                        }
                    }

                    if (status == PSA_SUCCESS) {
                        status = psa_key_agreement(msg.rhandle, psa_crypto_ipc.handle,
                                                   private_key, private_key_size, psa_crypto_ipc.alg);
                        mbedtls_free(private_key);
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
                psa_generator_abort(msg.rhandle);
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
        if (signals & PSA_GENERATOR) {
            psa_crypto_generator_operations();
        }
        if (signals & PSA_ENTROPY_INJECT) {
            psa_entropy_operation();
        }
    }
}
