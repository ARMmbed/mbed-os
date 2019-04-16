/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CRYPTO_PSA_WRAPPERS_H__
#define __CRYPTO_PSA_WRAPPERS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct psa_cipher_update_input
 *
 * \brief Input structure for the tfm_crypto_cipher_update_wrapper function
 *
 */
struct psa_cipher_update_input {
    const uint8_t *input; /*!< Input data to the cipher */
    size_t input_length;  /*!< Size of the input data */
};

/*!
 * \struct psa_cipher_update_output
 *
 * \brief Output structure for the tfm_crypto_cipher_update_wrapper function
 *
 */
struct psa_cipher_update_output {
    unsigned char *output; /*!< Buffer to hold the output data from the cipher*/
    size_t output_size;    /*!< Size of the output buffer */
    size_t *output_length; /*!< Size of the output data from the cipher */
};

/*!
 * \brief This function is a TF-M compatible wrapper for the
 *        \ref tfm_crypto_cipher_update implemented in the Crypto service
 *
 * \param[out] operation Pointer to a cipher operation context in the backend
 * \param[in]  input_s   Pointer to the structure containing input parameters
 *                       associated with \ref psa_cipher_update_input
 * \param[out] output_s  Pointer to the structure containing output parameters
 *                       associated with \ref psa_cipher_update_output
 *
 */
enum tfm_crypto_err_t tfm_crypto_cipher_update_wrapper(
                               psa_cipher_operation_t *operation,
                               const struct psa_cipher_update_input *input_s,
                               const struct psa_cipher_update_output *output_s);

/*!
 * \struct psa_aead_encrypt_input
 *
 * \brief Input structure for the tfm_crypto_aead_encrypt_wrapper function
 *
 */
struct psa_aead_encrypt_input {
    psa_key_slot_t key;             /*!< Key slot */
    psa_algorithm_t alg;            /*!< Algorithm type */
    const uint8_t *nonce;           /*!< Nonce or IV to be used */
    size_t nonce_length;            /*!< Size in bytes of the nonce buffer */
    const uint8_t *additional_data; /*!< Additional data to be authenticated */
    size_t additional_data_length;  /*!< Size in bytes of additional_data */
    const uint8_t *plaintext;       /*!< Buffer holding data for encryption */
    size_t plaintext_length;        /*!< Size in bytes of plaintext */
};

/*!
 * \struct psa_aead_encrypt_output
 *
 * \brief Output structure for the tfm_crypto_aead_encrypt_wrapper function
 *
 */
struct psa_aead_encrypt_output {
    uint8_t *ciphertext;       /*!< Pointer for the buffer to hold ciphertext */
    size_t ciphertext_size;    /*!< Size in bytes of the ciphertext buffer */
    size_t *ciphertext_length; /*!< Actual size in bytes of ciphertext */
};

/*!
 * \struct psa_aead_decrypt_input
 *
 * \brief Input structure for the tfm_crypto_aead_decrypt_wrapper function
 *
 */
struct psa_aead_decrypt_input {
    psa_key_slot_t key;             /*!< Key slot */
    psa_algorithm_t alg;            /*!< Algorithm type */
    const uint8_t *nonce;           /*!< Nonce or IV to be used */
    size_t nonce_length;            /*!< Size in bytes of the nonce buffer */
    const uint8_t *additional_data; /*!< Original data that was authenticated */
    size_t additional_data_length;  /*!< Size in bytes of additional_data */
    const uint8_t *ciphertext;      /*!< Buffer holding data for decryption */
    size_t ciphertext_length;       /*!< Size in bytes of ciphertext */
};

/*!
 * \struct psa_aead_decrypt_output
 *
 * \brief Output structure for the tfm_crypto_aead_decrypt_wrapper function
 *
 */
struct psa_aead_decrypt_output {
    uint8_t *plaintext;       /*!< Pointer for the buffer to hold plaintext */
    size_t plaintext_size;    /*!< Size in bytes of the plaintext buffer */
    size_t *plaintext_length; /*!< Actual size in bytes of plaintext */
};

/*!
 * \brief This function is a TF-M compatible wrapper for the
 *        \ref tfm_crypto_aead_encrypt implemented in the Crypto service
 *
 * \param[in]  input_s  Pointer to the structure containing input parameters
 *                      associated with \ref psa_aead_encrypt_input
 * \param[out] output_s Pointer to the structure containing output parameters
 *                      associated with \ref psa_aead_encrypt_output
 *
 */
enum tfm_crypto_err_t tfm_crypto_aead_encrypt_wrapper(
                                const struct psa_aead_encrypt_input *input_s,
                                const struct psa_aead_encrypt_output *output_s);
/*!
 * \brief This function is a TF-M compatible wrapper for the
 *        \ref tfm_crypto_aead_decrypt implemented in the Crypto service
 *
 * \param[in]  input_s  Pointer to the structure containing input parameters
 *                      associated with \ref psa_aead_decrypt_input
 * \param[out] output_s Pointer to the structure containing output parameters
 *                      associated with \ref psa_aead_decrypt_output
 *
 */
enum tfm_crypto_err_t tfm_crypto_aead_decrypt_wrapper(
                                const struct psa_aead_decrypt_input *input_s,
                                const struct psa_aead_decrypt_output *output_s);
#ifdef __cplusplus
}
#endif

#endif /* __CRYPTO_PSA_WRAPPERS_H__ */
