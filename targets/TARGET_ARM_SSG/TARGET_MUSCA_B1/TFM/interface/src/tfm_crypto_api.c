/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
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

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_destroy_key(psa_key_slot_t key)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_destroy_key,
                               (uint32_t)key,
                               0,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
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

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
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

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_export_public_key(psa_key_slot_t key,
                                   uint8_t *data,
                                   size_t data_size,
                                   size_t *data_length)
{
    (void)key;
    (void)data;
    (void)data_size;
    (void)data_length;

    /* TODO: This API is not supported yet */
    return PSA_ERROR_NOT_SUPPORTED;
}

void psa_key_policy_init(psa_key_policy_t *policy)
{
    /* PSA API returns void so just ignore error value returned */
    (void)tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_key_policy_init,
                               (uint32_t)policy,
                               0,
                               0,
                               0);
}

void psa_key_policy_set_usage(psa_key_policy_t *policy,
                              psa_key_usage_t usage,
                              psa_algorithm_t alg)
{
    /* PSA API returns void so just ignore error value returned */
    (void)tfm_ns_lock_dispatch(
                              (veneer_fn)tfm_crypto_veneer_key_policy_set_usage,
                              (uint32_t)policy,
                              (uint32_t)usage,
                              (uint32_t)alg,
                              0);
}

psa_key_usage_t psa_key_policy_get_usage(const psa_key_policy_t *policy)
{
    psa_key_usage_t usage;

    /* Initialise to a sensible default to avoid returning an uninitialised
     * value in case the secure function fails.
     */
    usage = 0;

    /* The PSA API does not return an error, so ignore any error from TF-M */
    (void)tfm_ns_lock_dispatch(
                              (veneer_fn)tfm_crypto_veneer_key_policy_get_usage,
                              (uint32_t)policy,
                              (uint32_t)&usage,
                              0,
                              0);

    return usage;
}

psa_algorithm_t psa_key_policy_get_algorithm(const psa_key_policy_t *policy)
{
    psa_algorithm_t alg;

    /* Initialise to a sensible default to avoid returning an uninitialised
     * value in case the secure function fails.
     */
    alg = 0;

    /* The PSA API does not return an error, so ignore any error from TF-M */
    (void)tfm_ns_lock_dispatch(
                          (veneer_fn)tfm_crypto_veneer_key_policy_get_algorithm,
                          (uint32_t)policy,
                          (uint32_t)&alg,
                          0,
                          0);

    return alg;
}

psa_status_t psa_set_key_policy(psa_key_slot_t key,
                                const psa_key_policy_t *policy)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_set_key_policy,
                               (uint32_t)key,
                               (uint32_t)policy,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_get_key_policy(psa_key_slot_t key,
                                psa_key_policy_t *policy)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_get_key_policy,
                               (uint32_t)key,
                               (uint32_t)policy,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_set_key_lifetime(psa_key_slot_t key,
                                  psa_key_lifetime_t lifetime)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_set_key_lifetime,
                               (uint32_t)key,
                               (uint32_t)lifetime,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_get_key_lifetime(psa_key_slot_t key,
                                  psa_key_lifetime_t *lifetime)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_get_key_lifetime,
                               (uint32_t)key,
                               (uint32_t)lifetime,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_cipher_set_iv(psa_cipher_operation_t *operation,
                               const unsigned char *iv,
                               size_t iv_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_cipher_set_iv,
                               (uint32_t)operation,
                               (uint32_t)iv,
                               (uint32_t)iv_length,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_cipher_encrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_slot_t key,
                                      psa_algorithm_t alg)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch(
                              (veneer_fn)tfm_crypto_veneer_cipher_encrypt_setup,
                              (uint32_t)operation,
                              (uint32_t)key,
                              (uint32_t)alg,
                              0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_cipher_decrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_slot_t key,
                                      psa_algorithm_t alg)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch(
                              (veneer_fn)tfm_crypto_veneer_cipher_decrypt_setup,
                              (uint32_t)operation,
                              (uint32_t)key,
                              (uint32_t)alg,
                              0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
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

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_cipher_abort(psa_cipher_operation_t *operation)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_cipher_abort,
                               (uint32_t)operation,
                               0,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
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

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_hash_setup(psa_hash_operation_t *operation,
                            psa_algorithm_t alg)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_hash_setup,
                               (uint32_t)operation,
                               (uint32_t)alg,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
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

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
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

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
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

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_hash_abort(psa_hash_operation_t *operation)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_hash_abort,
                               (uint32_t)operation,
                               0,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_mac_sign_setup(psa_mac_operation_t *operation,
                                psa_key_slot_t key,
                                psa_algorithm_t alg)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_mac_sign_setup,
                               (uint32_t)operation,
                               (uint32_t)key,
                               (uint32_t)alg,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_mac_verify_setup(psa_mac_operation_t *operation,
                                  psa_key_slot_t key,
                                  psa_algorithm_t alg)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_mac_verify_setup,
                               (uint32_t)operation,
                               (uint32_t)key,
                               (uint32_t)alg,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_mac_update(psa_mac_operation_t *operation,
                            const uint8_t *input,
                            size_t input_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_mac_update,
                               (uint32_t)operation,
                               (uint32_t)input,
                               (uint32_t)input_length,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_mac_sign_finish(psa_mac_operation_t *operation,
                                 uint8_t *mac,
                                 size_t mac_size,
                                 size_t *mac_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_mac_sign_finish,
                               (uint32_t)operation,
                               (uint32_t)mac,
                               (uint32_t)mac_size,
                               (uint32_t)mac_length);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_mac_verify_finish(psa_mac_operation_t *operation,
                                   const uint8_t *mac,
                                   size_t mac_length)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_mac_verify_finish,
                               (uint32_t)operation,
                               (uint32_t)mac,
                               (uint32_t)mac_length,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_mac_abort(psa_mac_operation_t *operation)
{
    enum tfm_crypto_err_t err;

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_mac_abort,
                               (uint32_t)operation,
                               0,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_aead_encrypt(psa_key_slot_t key,
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
    enum tfm_crypto_err_t err;

    /* Packing in structures is needed to overcome the 4 parameters
     * per call limit
     */
    struct psa_aead_encrypt_input input_s = {.key = key,
                                             .alg = alg,
                                             .nonce = nonce,
                                             .nonce_length = nonce_length,
                                             .additional_data = additional_data,
                                             .additional_data_length =
                                                         additional_data_length,
                                             .plaintext = plaintext,
                                             .plaintext_length =
                                                              plaintext_length};
    struct psa_aead_encrypt_output output_s = {.ciphertext = ciphertext,
                                               .ciphertext_size =
                                                                ciphertext_size,
                                               .ciphertext_length =
                                                             ciphertext_length};

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_aead_encrypt,
                               (uint32_t)&input_s,
                               (uint32_t)&output_s,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}

psa_status_t psa_aead_decrypt(psa_key_slot_t key,
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
    enum tfm_crypto_err_t err;

    /* Packing in structures is needed to overcome the 4 parameters
     * per call limit
     */
    struct psa_aead_decrypt_input input_s = {.key = key,
                                             .alg = alg,
                                             .nonce = nonce,
                                             .nonce_length = nonce_length,
                                             .additional_data = additional_data,
                                             .additional_data_length =
                                                         additional_data_length,
                                             .ciphertext = ciphertext,
                                             .ciphertext_length =
                                                             ciphertext_length};
    struct psa_aead_decrypt_output output_s = {.plaintext = plaintext,
                                               .plaintext_size = plaintext_size,
                                               .plaintext_length =
                                                              plaintext_length};

    err = tfm_ns_lock_dispatch((veneer_fn)tfm_crypto_veneer_aead_decrypt,
                               (uint32_t)&input_s,
                               (uint32_t)&output_s,
                               0,
                               0);

    return TFM_CRYPTO_ERR_TO_PSA_STATUS(err);
}
