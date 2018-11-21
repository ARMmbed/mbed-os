/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_crypto_veneers.h"
#include "psa_crypto.h"
#include "tfm_ns_lock.h"
#include "crypto_psa_wrappers.h"

psa_status_t psa_crypto_init(void)
{
    /* Service init is performed during TFM boot up,
     * so application level initialisation is empty
     */
    return PSA_SUCCESS;
}

psa_status_t psa_import_key(psa_key_slot_t key,
                            psa_key_type_t type,
                            const uint8_t *data,
                            size_t data_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_import_key,
                               (uint32_t)key,
                               (uint32_t)type,
                               (uint32_t)data,
                               (uint32_t)data_length);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_destroy_key(psa_key_slot_t key)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_destroy_key,
                               (uint32_t)key,
                               0,
                               0,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_get_key_information(psa_key_slot_t key,
                                     psa_key_type_t *type,
                                     size_t *bits)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_get_key_information,
                               (uint32_t)key,
                               (uint32_t)type,
                               (uint32_t)bits,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_export_key(psa_key_slot_t key,
                            uint8_t *data,
                            size_t data_size,
                            size_t *data_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_export_key,
                               (uint32_t)key,
                               (uint32_t)data,
                               (uint32_t)data_size,
                               (uint32_t)data_length);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_export_public_key(psa_key_slot_t key,
                                   uint8_t *data,
                                   size_t data_size,
                                   size_t *data_length)
{
    /* TODO: This API is not supported yet */
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_encrypt_set_iv(psa_cipher_operation_t *operation,
                                const unsigned char *iv,
                                size_t iv_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_encrypt_set_iv,
                               (uint32_t)operation,
                               (uint32_t)iv,
                               (uint32_t)iv_length,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_encrypt_setup(psa_cipher_operation_t *operation,
                               psa_key_slot_t key,
                               psa_algorithm_t alg)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_encrypt_setup,
                               (uint32_t)operation,
                               (uint32_t)key,
                               (uint32_t)alg,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_decrypt_setup(psa_cipher_operation_t *operation,
                               psa_key_slot_t key,
                               psa_algorithm_t alg)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_decrypt_setup,
                               (uint32_t)operation,
                               (uint32_t)key,
                               (uint32_t)alg,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_cipher_update(psa_cipher_operation_t *operation,
                               const uint8_t *input,
                               size_t input_length,
                               unsigned char *output,
                               size_t output_size,
                               size_t *output_length)
{
    enum tfm_crypto_err_t err;

    /* Packing in structures is needed to overcome the 4 parameters
     * per call limit
     */
    struct psa_cipher_update_input input_s = {.input = input,
                                              .input_length = input_length};
    struct psa_cipher_update_output output_s = {.output = output,
                                                .output_size = output_size,
                                                .output_length =
                                                               output_length};

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_cipher_update,
                               (uint32_t)operation,
                               (uint32_t)&input_s,
                               (uint32_t)&output_s,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_cipher_abort(psa_cipher_operation_t *operation)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_cipher_abort,
                               (uint32_t)operation,
                               0,
                               0,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_cipher_finish(psa_cipher_operation_t *operation,
                               uint8_t *output,
                               size_t output_size,
                               size_t *output_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_cipher_finish,
                               (uint32_t)operation,
                               (uint32_t)output,
                               (uint32_t)output_size,
                               (uint32_t)output_length);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_hash_start(psa_hash_operation_t *operation,
                            psa_algorithm_t alg)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_hash_start,
                               (uint32_t)operation,
                               (uint32_t)alg,
                               0,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_hash_update(psa_hash_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_hash_update,
                               (uint32_t)operation,
                               (uint32_t)input,
                               (uint32_t)input_length,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_hash_finish(psa_hash_operation_t *operation,
                             uint8_t *hash,
                             size_t hash_size,
                             size_t *hash_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_hash_finish,
                               (uint32_t)operation,
                               (uint32_t)hash,
                               (uint32_t)hash_size,
                               (uint32_t)hash_length);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_hash_verify(psa_hash_operation_t *operation,
                             const uint8_t *hash,
                             size_t hash_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_hash_verify,
                               (uint32_t)operation,
                               (uint32_t)hash,
                               (uint32_t)hash_length,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}

psa_status_t psa_hash_abort(psa_hash_operation_t *operation)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_hash_abort,
                               (uint32_t)operation,
                               0,
                               0,
                               0);

    return TFM_CRYPTO_PSA_RETURN(err);
}
