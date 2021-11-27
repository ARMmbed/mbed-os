/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_crypto_defs.h"
#include "psa/crypto.h"
#include "tfm_ns_interface.h"
#include "psa_manifest/sid.h"
#include "psa/client.h"

#define API_DISPATCH(sfn_name, sfn_id)                          \
    psa_call(TFM_CRYPTO_HANDLE, PSA_IPC_CALL,                   \
        in_vec, IOVEC_LEN(in_vec),                              \
        out_vec, IOVEC_LEN(out_vec))

#define API_DISPATCH_NO_OUTVEC(sfn_name, sfn_id)                \
    psa_call(TFM_CRYPTO_HANDLE, PSA_IPC_CALL,                   \
        in_vec, IOVEC_LEN(in_vec),                              \
        (psa_outvec *)NULL, 0)

psa_status_t psa_crypto_init(void)
{
    /* Service init is performed during TFM boot up,
     * so application level initialisation is empty
     */
    return PSA_SUCCESS;
}

psa_status_t psa_open_key(psa_key_id_t id,
                          psa_key_id_t *key)
{
    psa_status_t status;
    const struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_OPEN_KEY_SID,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = &id, .len = sizeof(psa_key_id_t)},
    };
    psa_outvec out_vec[] = {
        {.base = key, .len = sizeof(psa_key_id_t)},
    };

    status = API_DISPATCH(tfm_crypto_open_key,
                          TFM_CRYPTO_OPEN_KEY);

    return status;
}

psa_status_t psa_close_key(psa_key_id_t key)
{
    psa_status_t status;
    const struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CLOSE_KEY_SID,
        .key_id = key,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_close_key,
                                    TFM_CRYPTO_CLOSE_KEY);;

    return status;
}

psa_status_t psa_import_key(const psa_key_attributes_t *attributes,
                            const uint8_t *data,
                            size_t data_length,
                            psa_key_id_t *key)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_IMPORT_KEY_SID,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = attributes, .len = sizeof(psa_key_attributes_t)},
        {.base = data, .len = data_length}
    };
    psa_outvec out_vec[] = {
        {.base = key, .len = sizeof(psa_key_id_t)}
    };

    status = API_DISPATCH(tfm_crypto_import_key,
                          TFM_CRYPTO_IMPORT_KEY);

    return status;
}

psa_status_t psa_destroy_key(psa_key_id_t key)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_DESTROY_KEY_SID,
        .key_id = key,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_destroy_key,
                                    TFM_CRYPTO_DESTROY_KEY);

    return status;
}

psa_status_t psa_get_key_attributes(psa_key_id_t key,
                                    psa_key_attributes_t *attributes)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GET_KEY_ATTRIBUTES_SID,
        .key_id = key,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = attributes, .len = sizeof(psa_key_attributes_t)},
    };

    status = API_DISPATCH(tfm_crypto_get_key_attributes,
                          TFM_CRYPTO_GET_KEY_ATTRIBUTES);

    return status;
}

void psa_reset_key_attributes(psa_key_attributes_t *attributes)
{
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_RESET_KEY_ATTRIBUTES_SID,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = attributes, .len = sizeof(psa_key_attributes_t)},
    };

    (void)API_DISPATCH(tfm_crypto_reset_key_attributes,
                       TFM_CRYPTO_RESET_KEY_ATTRIBUTES);

    return;
}

psa_status_t psa_export_key(psa_key_id_t key,
                            uint8_t *data,
                            size_t data_size,
                            size_t *data_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_EXPORT_KEY_SID,
        .key_id = key,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = data, .len = data_size}
    };

    status = API_DISPATCH(tfm_crypto_export_key,
                          TFM_CRYPTO_EXPORT_KEY);

    *data_length = out_vec[0].len;

    return status;
}

psa_status_t psa_export_public_key(psa_key_id_t key,
                                   uint8_t *data,
                                   size_t data_size,
                                   size_t *data_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_EXPORT_PUBLIC_KEY_SID,
        .key_id = key,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = data, .len = data_size}
    };

    status = API_DISPATCH(tfm_crypto_export_public_key,
                          TFM_CRYPTO_EXPORT_PUBLIC_KEY);

    *data_length = out_vec[0].len;

    return status;
}

psa_status_t psa_purge_key(psa_key_id_t key)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_PURGE_KEY_SID,
        .key_id = key,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_purge_key,
                                    TFM_CRYPTO_PURGE_KEY);

    return status;
}

psa_status_t psa_copy_key(psa_key_id_t source_key,
                          const psa_key_attributes_t *attributes,
                          psa_key_id_t *target_key)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_COPY_KEY_SID,
        .key_id = source_key,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = attributes, .len = sizeof(psa_key_attributes_t)},

    };

    psa_outvec out_vec[] = {
        {.base = target_key, .len = sizeof(psa_key_id_t)},
    };

    status = API_DISPATCH(tfm_crypto_copy_key,
                          TFM_CRYPTO_COPY_KEY);

    return status;
}

psa_status_t psa_cipher_generate_iv(psa_cipher_operation_t *operation,
                                    unsigned char *iv,
                                    size_t iv_size,
                                    size_t *iv_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_GENERATE_IV_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
        {.base = iv, .len = iv_size},
    };

    status = API_DISPATCH(tfm_crypto_cipher_generate_iv,
                          TFM_CRYPTO_CIPHER_GENERATE_IV);

    *iv_length = out_vec[1].len;

    return status;
}

psa_status_t psa_cipher_set_iv(psa_cipher_operation_t *operation,
                               const unsigned char *iv,
                               size_t iv_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_SET_IV_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = iv, .len = iv_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_cipher_set_iv,
                          TFM_CRYPTO_CIPHER_SET_IV);

    return status;
}

psa_status_t psa_cipher_encrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_id_t key,
                                      psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_ENCRYPT_SETUP_SID,
        .key_id = key,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_cipher_encrypt_setup,
                          TFM_CRYPTO_CIPHER_ENCRYPT_SETUP);

    return status;
}

psa_status_t psa_cipher_decrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_id_t key,
                                      psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_DECRYPT_SETUP_SID,
        .key_id = key,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_cipher_decrypt_setup,
                          TFM_CRYPTO_CIPHER_DECRYPT_SETUP);

    return status;
}

psa_status_t psa_cipher_update(psa_cipher_operation_t *operation,
                               const uint8_t *input,
                               size_t input_length,
                               unsigned char *output,
                               size_t output_size,
                               size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_UPDATE_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
        {.base = output, .len = output_size}
    };

    status = API_DISPATCH(tfm_crypto_cipher_update,
                          TFM_CRYPTO_CIPHER_UPDATE);

    *output_length = out_vec[1].len;

    return status;
}

psa_status_t psa_cipher_abort(psa_cipher_operation_t *operation)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_ABORT_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_cipher_abort,
                          TFM_CRYPTO_CIPHER_ABORT);

    return status;
}

psa_status_t psa_cipher_finish(psa_cipher_operation_t *operation,
                               uint8_t *output,
                               size_t output_size,
                               size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_FINISH_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
        {.base = output, .len = output_size},
    };

    status = API_DISPATCH(tfm_crypto_cipher_finish,
                          TFM_CRYPTO_CIPHER_FINISH);

    *output_length = out_vec[1].len;

    return status;
}

psa_status_t psa_hash_setup(psa_hash_operation_t *operation,
                            psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_SETUP_SID,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_hash_setup,
                          TFM_CRYPTO_HASH_SETUP);

    return status;
}

psa_status_t psa_hash_update(psa_hash_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_UPDATE_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_hash_update,
                          TFM_CRYPTO_HASH_UPDATE);

    return status;
}

psa_status_t psa_hash_finish(psa_hash_operation_t *operation,
                             uint8_t *hash,
                             size_t hash_size,
                             size_t *hash_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_FINISH_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
        {.base = hash, .len = hash_size},
    };

    status = API_DISPATCH(tfm_crypto_hash_finish,
                          TFM_CRYPTO_HASH_FINISH);

    *hash_length = out_vec[1].len;

    return status;
}

psa_status_t psa_hash_verify(psa_hash_operation_t *operation,
                             const uint8_t *hash,
                             size_t hash_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_VERIFY_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = hash, .len = hash_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_hash_verify,
                          TFM_CRYPTO_HASH_VERIFY);

    return status;
}

psa_status_t psa_hash_abort(psa_hash_operation_t *operation)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_ABORT_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_hash_abort,
                          TFM_CRYPTO_HASH_ABORT);

    return status;
}

psa_status_t psa_hash_clone(const psa_hash_operation_t *source_operation,
                            psa_hash_operation_t *target_operation)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_CLONE_SID,
        .op_handle = source_operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = target_operation, .len = sizeof(psa_hash_operation_t)},
    };

    if (target_operation && (target_operation->handle != 0)) {
        return PSA_ERROR_BAD_STATE;
    }

    status = API_DISPATCH(tfm_crypto_hash_clone,
                          TFM_CRYPTO_HASH_CLONE);

    return status;
}

psa_status_t psa_hash_compute(psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              uint8_t *hash,
                              size_t hash_size,
                              size_t *hash_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_COMPUTE_SID,
        .alg = alg,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };

    psa_outvec out_vec[] = {
        {.base = hash, .len = hash_size}
    };

    status = API_DISPATCH(tfm_crypto_hash_compute,
                          TFM_CRYPTO_HASH_COMPUTE);

    *hash_length = out_vec[0].len;

    return status;
}

psa_status_t psa_hash_compare(psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              const uint8_t *hash,
                              size_t hash_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_COMPARE_SID,
        .alg = alg,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
        {.base = hash, .len = hash_length},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_hash_compare,
                          TFM_CRYPTO_HASH_COMPARE);

    return status;
}

psa_status_t psa_mac_sign_setup(psa_mac_operation_t *operation,
                                psa_key_id_t key,
                                psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_SIGN_SETUP_SID,
        .key_id = key,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_mac_sign_setup,
                          TFM_CRYPTO_MAC_SIGN_SETUP);

    return status;
}

psa_status_t psa_mac_verify_setup(psa_mac_operation_t *operation,
                                  psa_key_id_t key,
                                  psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_VERIFY_SETUP_SID,
        .key_id = key,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_mac_verify_setup,
                          TFM_CRYPTO_MAC_VERIFY_SETUP);

    return status;
}

psa_status_t psa_mac_update(psa_mac_operation_t *operation,
                            const uint8_t *input,
                            size_t input_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_UPDATE_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_mac_update,
                          TFM_CRYPTO_MAC_UPDATE);

    return status;
}

psa_status_t psa_mac_sign_finish(psa_mac_operation_t *operation,
                                 uint8_t *mac,
                                 size_t mac_size,
                                 size_t *mac_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_SIGN_FINISH_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
        {.base = mac, .len = mac_size},
    };

    status = API_DISPATCH(tfm_crypto_mac_sign_finish,
                          TFM_CRYPTO_MAC_SIGN_FINISH);

    *mac_length = out_vec[1].len;

    return status;
}

psa_status_t psa_mac_verify_finish(psa_mac_operation_t *operation,
                                   const uint8_t *mac,
                                   size_t mac_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_VERIFY_FINISH_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = mac, .len = mac_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_mac_verify_finish,
                          TFM_CRYPTO_MAC_VERIFY_FINISH);

    return status;
}

psa_status_t psa_mac_abort(psa_mac_operation_t *operation)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_ABORT_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_mac_abort,
                          TFM_CRYPTO_MAC_ABORT);

    return status;
}

psa_status_t psa_aead_encrypt(psa_key_id_t key,
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
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_AEAD_ENCRYPT_SID,
        .key_id = key,
        .alg = alg,
        .aead_in = {.nonce = {0}, .nonce_length = nonce_length}
    };

    /* Sanitize the optional input */
    if ((additional_data == NULL) && (additional_data_length != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    size_t idx = 0;
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = plaintext, .len = plaintext_length},
        {.base = additional_data, .len = additional_data_length},
    };
    psa_outvec out_vec[] = {
        {.base = ciphertext, .len = ciphertext_size},
    };

    if (nonce_length > TFM_CRYPTO_MAX_NONCE_LENGTH) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (nonce != NULL) {
        for (idx = 0; idx < nonce_length; idx++) {
            iov.aead_in.nonce[idx] = nonce[idx];
        }
    }

    size_t in_len = IOVEC_LEN(in_vec);
    if (additional_data == NULL) {
        in_len--;
    }
    status = psa_call(TFM_CRYPTO_HANDLE, PSA_IPC_CALL, in_vec, in_len,
                      out_vec, IOVEC_LEN(out_vec));

    *ciphertext_length = out_vec[0].len;

    return status;
}

psa_status_t psa_aead_decrypt(psa_key_id_t key,
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
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_AEAD_DECRYPT_SID,
        .key_id = key,
        .alg = alg,
        .aead_in = {.nonce = {0}, .nonce_length = nonce_length}
    };

    /* Sanitize the optional input */
    if ((additional_data == NULL) && (additional_data_length != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    size_t idx = 0;
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = ciphertext, .len = ciphertext_length},
        {.base = additional_data, .len = additional_data_length},
    };
    psa_outvec out_vec[] = {
        {.base = plaintext, .len = plaintext_size},
    };

    if (nonce_length > TFM_CRYPTO_MAX_NONCE_LENGTH) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (nonce != NULL) {
        for (idx = 0; idx < nonce_length; idx++) {
            iov.aead_in.nonce[idx] = nonce[idx];
        }
    }

    size_t in_len = IOVEC_LEN(in_vec);
    if (additional_data == NULL) {
        in_len--;
    }
    status = psa_call(TFM_CRYPTO_HANDLE, PSA_IPC_CALL, in_vec, in_len,
                      out_vec, IOVEC_LEN(out_vec));

    *plaintext_length = out_vec[0].len;

    return status;
}

psa_status_t psa_sign_message(psa_key_id_t key,
                              psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              uint8_t *signature,
                              size_t signature_size,
                              size_t *signature_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_SIGN_MESSAGE_SID,
        .key_id = key,
        .alg = alg,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };
    psa_outvec out_vec[] = {
        {.base = signature, .len = signature_size},
    };

    status = API_DISPATCH(tfm_crypto_sign_message,
                          TFM_CRYPTO_SIGN_MESSAGE);

    if (status == PSA_SUCCESS) {
        *signature_length = out_vec[0].len;
    }

    return status;
}

psa_status_t psa_verify_message(psa_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                const uint8_t *signature,
                                size_t signature_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_VERIFY_MESSAGE_SID,
        .key_id = key,
        .alg = alg
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
        {.base = signature, .len = signature_length}
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_verify_message,
                                    TFM_CRYPTO_VERIFY_MESSAGE);

    return status;
}

psa_status_t psa_sign_hash(psa_key_id_t key,
                           psa_algorithm_t alg,
                           const uint8_t *hash,
                           size_t hash_length,
                           uint8_t *signature,
                           size_t signature_size,
                           size_t *signature_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_SIGN_HASH_SID,
        .key_id = key,
        .alg = alg,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = hash, .len = hash_length},
    };
    psa_outvec out_vec[] = {
        {.base = signature, .len = signature_size},
    };

    status = API_DISPATCH(tfm_crypto_sign_hash,
                          TFM_CRYPTO_SIGN_HASH);

    *signature_length = out_vec[0].len;

    return status;
}

psa_status_t psa_verify_hash(psa_key_id_t key,
                             psa_algorithm_t alg,
                             const uint8_t *hash,
                             size_t hash_length,
                             const uint8_t *signature,
                             size_t signature_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_VERIFY_HASH_SID,
        .key_id = key,
        .alg = alg
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = hash, .len = hash_length},
        {.base = signature, .len = signature_length}
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_verify_hash,
                                    TFM_CRYPTO_VERIFY_HASH);

    return status;
}

psa_status_t psa_asymmetric_encrypt(psa_key_id_t key,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_ASYMMETRIC_ENCRYPT_SID,
        .key_id = key,
        .alg = alg
    };

    /* Sanitize the optional input */
    if ((salt == NULL) && (salt_length != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
        {.base = salt, .len = salt_length}
    };

    psa_outvec out_vec[] = {
        {.base = output, .len = output_size},
    };

    size_t in_len = IOVEC_LEN(in_vec);
    if (salt == NULL) {
        in_len--;
    }
    status = psa_call(TFM_CRYPTO_HANDLE, PSA_IPC_CALL, in_vec, in_len,
                      out_vec, IOVEC_LEN(out_vec));

    *output_length = out_vec[0].len;

    return status;
}

psa_status_t psa_asymmetric_decrypt(psa_key_id_t key,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_ASYMMETRIC_DECRYPT_SID,
        .key_id = key,
        .alg = alg
    };

    /* Sanitize the optional input */
    if ((salt == NULL) && (salt_length != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
        {.base = salt, .len = salt_length}
    };

    psa_outvec out_vec[] = {
        {.base = output, .len = output_size},
    };

    size_t in_len = IOVEC_LEN(in_vec);
    if (salt == NULL) {
        in_len--;
    }
    status = psa_call(TFM_CRYPTO_HANDLE, PSA_IPC_CALL, in_vec, in_len,
                      out_vec, IOVEC_LEN(out_vec));

    *output_length = out_vec[0].len;

    return status;
}

psa_status_t psa_key_derivation_get_capacity(
                                const psa_key_derivation_operation_t *operation,
                                size_t *capacity)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_DERIVATION_GET_CAPACITY_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    psa_outvec out_vec[] = {
        {.base = capacity, .len = sizeof(size_t)},
    };

    status = API_DISPATCH(tfm_crypto_key_derivation_get_capacity,
                          TFM_CRYPTO_KEY_DERIVATION_GET_CAPACITY);

    return status;
}

psa_status_t psa_key_derivation_output_bytes(
                                      psa_key_derivation_operation_t *operation,
                                      uint8_t *output,
                                      size_t output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_DERIVATION_OUTPUT_BYTES_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    psa_outvec out_vec[] = {
        {.base = output, .len = output_length},
    };

    status = API_DISPATCH(tfm_crypto_key_derivation_output_bytes,
                          TFM_CRYPTO_KEY_DERIVATION_OUTPUT_BYTES);

    return status;
}

psa_status_t psa_key_derivation_input_key(
                                      psa_key_derivation_operation_t *operation,
                                      psa_key_derivation_step_t step,
                                      psa_key_id_t key)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_DERIVATION_INPUT_KEY_SID,
        .key_id = key,
        .step = step,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_key_derivation_input_key,
                                    TFM_CRYPTO_KEY_DERIVATION_INPUT_KEY);

    return status;
}

psa_status_t psa_key_derivation_abort(
                                      psa_key_derivation_operation_t *operation)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_DERIVATION_ABORT_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_key_derivation_abort,
                          TFM_CRYPTO_KEY_DERIVATION_ABORT);

    return status;
}

psa_status_t psa_key_derivation_key_agreement(
                                      psa_key_derivation_operation_t *operation,
                                      psa_key_derivation_step_t step,
                                      psa_key_id_t private_key,
                                      const uint8_t *peer_key,
                                      size_t peer_key_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_DERIVATION_KEY_AGREEMENT_SID,
        .key_id = private_key,
        .step = step,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = peer_key, .len = peer_key_length},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_key_derivation_key_agreement,
                                    TFM_CRYPTO_KEY_DERIVATION_KEY_AGREEMENT);

    return status;
}

psa_status_t psa_generate_random(uint8_t *output,
                                 size_t output_size)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GENERATE_RANDOM_SID,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    psa_outvec out_vec[] = {
        {.base = output, .len = output_size},
    };

    if (output_size == 0) {
        return PSA_SUCCESS;
    }

    status = API_DISPATCH(tfm_crypto_generate_random,
                          TFM_CRYPTO_GENERATE_RANDOM);

    return status;
}

psa_status_t psa_generate_key(const psa_key_attributes_t *attributes,
                              psa_key_id_t *key)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GENERATE_KEY_SID,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = attributes, .len = sizeof(psa_key_attributes_t)},
    };

    psa_outvec out_vec[] = {
        {.base = key, .len = sizeof(psa_key_id_t)},
    };

    status = API_DISPATCH(tfm_crypto_generate_key,
                          TFM_CRYPTO_GENERATE_KEY);

    return status;
}

psa_status_t psa_aead_update_ad(psa_aead_operation_t *operation,
                                const uint8_t *input,
                                size_t input_length)
{
    psa_status_t status;

    status = PSA_ERROR_NOT_SUPPORTED;

    return status;
}

psa_status_t psa_aead_finish(psa_aead_operation_t *operation,
                             uint8_t *ciphertext,
                             size_t ciphertext_size,
                             size_t *ciphertext_length,
                             uint8_t *tag,
                             size_t tag_size,
                             size_t *tag_length)
{
    psa_status_t status;

    status = PSA_ERROR_NOT_SUPPORTED;

    return status;
}

psa_status_t psa_aead_verify(psa_aead_operation_t *operation,
                             uint8_t *plaintext,
                             size_t plaintext_size,
                             size_t *plaintext_length,
                             const uint8_t *tag,
                             size_t tag_length)
{
    psa_status_t status;

    status = PSA_ERROR_NOT_SUPPORTED;

    return status;
}

psa_status_t psa_aead_abort(psa_aead_operation_t *operation)
{
    psa_status_t status;

    status = PSA_ERROR_NOT_SUPPORTED;

    return status;
}

psa_status_t psa_mac_compute(psa_key_id_t key,
                             psa_algorithm_t alg,
                             const uint8_t *input,
                             size_t input_length,
                             uint8_t *mac,
                             size_t mac_size,
                             size_t *mac_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_COMPUTE_SID,
        .key_id = key,
        .alg = alg,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };
    psa_outvec out_vec[] = {
        {.base = mac, .len = mac_size},
    };

    status = API_DISPATCH(tfm_crypto_mac_compute,
                          TFM_CRYPTO_MAC_COMPUTE);

    if (status == PSA_SUCCESS) {
        *mac_length = out_vec[0].len;
    }

    return status;
}

psa_status_t psa_mac_verify(psa_key_id_t key,
                            psa_algorithm_t alg,
                            const uint8_t *input,
                            size_t input_length,
                            const uint8_t *mac,
                            const size_t mac_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_VERIFY_SID,
        .key_id = key,
        .alg = alg,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
        {.base = mac, .len = mac_length},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_mac_verify,
                                    TFM_CRYPTO_MAC_VERIFY);

    return status;
}

psa_status_t psa_cipher_encrypt(psa_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_ENCRYPT_SID,
        .key_id = key,
        .alg = alg,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };
    psa_outvec out_vec[] = {
        {.base = output, .len = output_size}
    };

    status = API_DISPATCH(tfm_crypto_cipher_encrypt,
                          TFM_CRYPTO_CIPHER_ENCRYPT);

    if (status == PSA_SUCCESS) {
        *output_length = out_vec[0].len;
    }

    return status;
}

psa_status_t psa_cipher_decrypt(psa_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_DECRYPT_SID,
        .key_id = key,
        .alg = alg,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };
    psa_outvec out_vec[] = {
        {.base = output, .len = output_size}
    };

    status = API_DISPATCH(tfm_crypto_cipher_decrypt,
                          TFM_CRYPTO_CIPHER_DECRYPT);

    if (status == PSA_SUCCESS) {
        *output_length = out_vec[0].len;
    }

    return status;
}

psa_status_t psa_raw_key_agreement(psa_algorithm_t alg,
                                   psa_key_id_t private_key,
                                   const uint8_t *peer_key,
                                   size_t peer_key_length,
                                   uint8_t *output,
                                   size_t output_size,
                                   size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_RAW_KEY_AGREEMENT_SID,
        .alg = alg,
        .key_id = private_key
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = peer_key, .len = peer_key_length},
    };

    psa_outvec out_vec[] = {
        {.base = output, .len = output_size},
    };

    status = API_DISPATCH(tfm_crypto_raw_key_agreement,
                          TFM_CRYPTO_RAW_KEY_AGREEMENT);

    *output_length = out_vec[0].len;

    return status;
}

psa_status_t psa_key_derivation_setup(psa_key_derivation_operation_t *operation,
                                      psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_DERIVATION_SETUP_SID,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_key_derivation_setup,
                          TFM_CRYPTO_KEY_DERIVATION_SETUP);

    return status;
}

psa_status_t psa_key_derivation_set_capacity(
                                      psa_key_derivation_operation_t *operation,
                                      size_t capacity)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_DERIVATION_SET_CAPACITY_SID,
        .capacity = capacity,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_key_derivation_set_capacity,
                                    TFM_CRYPTO_KEY_DERIVATION_SET_CAPACITY);

    return status;
}

psa_status_t psa_key_derivation_input_bytes(
                                      psa_key_derivation_operation_t *operation,
                                      psa_key_derivation_step_t step,
                                      const uint8_t *data,
                                      size_t data_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_DERIVATION_INPUT_BYTES_SID,
        .step = step,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = data, .len = data_length},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_key_derivation_input_bytes,
                                    TFM_CRYPTO_KEY_DERIVATION_INPUT_BYTES);

    return status;
}

psa_status_t psa_key_derivation_output_key(
                                      const psa_key_attributes_t *attributes,
                                      psa_key_derivation_operation_t *operation,
                                      psa_key_id_t *key)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_DERIVATION_OUTPUT_KEY_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = attributes, .len = sizeof(psa_key_attributes_t)},
    };

    psa_outvec out_vec[] = {
        {.base = key, .len = sizeof(psa_key_id_t)}
    };

    status = API_DISPATCH(tfm_crypto_key_derivation_output_key,
                          TFM_CRYPTO_KEY_DERIVATION_OUTPUT_KEY);

    return status;
}

psa_status_t psa_aead_encrypt_setup(psa_aead_operation_t *operation,
                                    psa_key_id_t key,
                                    psa_algorithm_t alg)
{
    psa_status_t status;

    status = PSA_ERROR_NOT_SUPPORTED;

    return status;
}

psa_status_t psa_aead_decrypt_setup(psa_aead_operation_t *operation,
                                    psa_key_id_t key,
                                    psa_algorithm_t alg)
{
    psa_status_t status;

    status = PSA_ERROR_NOT_SUPPORTED;

    return status;
}

psa_status_t psa_aead_generate_nonce(psa_aead_operation_t *operation,
                                     uint8_t *nonce,
                                     size_t nonce_size,
                                     size_t *nonce_length)
{
    psa_status_t status;

    status = PSA_ERROR_NOT_SUPPORTED;

    return status;
}

psa_status_t psa_aead_set_nonce(psa_aead_operation_t *operation,
                                const uint8_t *nonce,
                                size_t nonce_length)
{
    psa_status_t status;

    status = PSA_ERROR_NOT_SUPPORTED;

    return status;
}

psa_status_t psa_aead_set_lengths(psa_aead_operation_t *operation,
                                  size_t ad_length,
                                  size_t plaintext_length)
{
    psa_status_t status;

    status = PSA_ERROR_NOT_SUPPORTED;

    return status;
}

psa_status_t psa_aead_update(psa_aead_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length,
                             uint8_t *output,
                             size_t output_size,
                             size_t *output_length)
{
    psa_status_t status;

    status = PSA_ERROR_NOT_SUPPORTED;

    return status;
}
