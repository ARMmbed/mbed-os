/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CRYPTO_VENEERS_H__
#define __TFM_CRYPTO_VENEERS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tfm_crypto_defs.h"

#include "psa_crypto.h"

#include "crypto_psa_wrappers.h"

/**
 * \brief Import the key data in the provided key slot (veneer function)
 *
 * \param[in] key         Key slot
 * \param[in] type        Key type
 * \param[in] data        Key data to import
 * \param[in] data_length Length in bytes of the data field
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_import_key(psa_key_slot_t key,
                                                   psa_key_type_t type,
                                                   const uint8_t *data,
                                                   size_t data_length);
/**
 * \brief Destroy the key in the provided key slot (veneer function)
 *
 * \param[in] key         Key slot
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_destroy_key(psa_key_slot_t key);

/**
 * \brief Retrieve key information for the provided key slot (veneer function)
 *
 * \param[in]  key  Key slot
 * \param[out] type Key type associated to the key slot requested
 * \param[out] bits Length in bits of the key in the requested slot
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_get_key_information(
                                                           psa_key_slot_t key,
                                                           psa_key_type_t *type,
                                                           size_t *bits);
/**
 * \brief Export the key contained in the provided key slot (veneer function)
 *
 * \param[in]  key         Key slot
 * \param[out] data        Buffer to hold the exported key
 * \param[in]  data_size   Length of the buffer pointed to by data
 * \param[out] data_length Length of the exported key
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_export_key(psa_key_slot_t key,
                                                   uint8_t *data,
                                                   size_t data_size,
                                                   size_t *data_length);

/**
 * \brief Initialise the key policy to a default that forbids any use of the
 *        key (veneer function)
 *
 * \param[out] policy  Key policy to initialise
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_key_policy_init(
                                                      psa_key_policy_t *policy);

/**
 * \brief Set the permitted usage and algorithm for the provided key policy
 *        (veneer function)
 *
 * \param[out] policy  Key policy to modify
 * \param[in]  usage   Permitted usage
 * \param[in]  alg     Permitted algorithm
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_key_policy_set_usage(
                                                       psa_key_policy_t *policy,
                                                       psa_key_usage_t usage,
                                                       psa_algorithm_t alg);

/**
 * \brief Get the permitted usage for the provided key policy (veneer function)
 *
 * \param[in]  policy  Key policy
 * \param[out] usage   Permitted usage for this key policy
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_key_policy_get_usage(
                                                 const psa_key_policy_t *policy,
                                                 psa_key_usage_t *usage);

/**
 * \brief Get the permitted algorithm for the provided key policy
 *        (veneer function)
 *
 * \param[in]  policy  Key policy
 * \param[out] alg     Permitted algorithm for this key policy
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_key_policy_get_algorithm(
                                                 const psa_key_policy_t *policy,
                                                 psa_algorithm_t *alg);

/**
 * \brief Set the key policy for the provided key slot (veneer function)
 *
 * \param[in] key     Key slot
 * \param[in] policy  Key policy
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_set_key_policy(
                                                psa_key_slot_t key,
                                                const psa_key_policy_t *policy);

/**
 * \brief Get the key policy for the provided key slot (veneer function)
 *
 * \param[in]  key     Key slot
 * \param[out] policy  Key policy
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_get_key_policy(
                                                      psa_key_slot_t key,
                                                      psa_key_policy_t *policy);

/**
 * \brief Set the lifetime for the provided key slot (veneer function)
 *
 * \param[in] key       Key slot
 * \param[in] lifetime  Lifetime value
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_set_key_lifetime(
                                                   psa_key_slot_t key,
                                                   psa_key_lifetime_t lifetime);

/**
 * \brief Get the lifetime for the provided key slot (veneer function)
 *
 * \param[in]  key       Key slot
 * \param[out] lifetime  Lifetime value
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_get_key_lifetime(
                                                  psa_key_slot_t key,
                                                  psa_key_lifetime_t *lifetime);

/**
 * \brief Set the initialisation vector on the provided cipher operation (veneer
 *        function)
 *
 * \param[in] operation  Cipher operation context
 * \param[in] iv         Buffer that contains the IV
 * \param[in] iv_length  Length of the provided IV
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_cipher_set_iv(
                                              psa_cipher_operation_t *operation,
                                              const unsigned char *iv,
                                              size_t iv_length);
/**
 * \brief Set the cipher operation using the provided algorithm and key slot,
 *        for encryption context (veneer function)
 *
 * \note A successful call to this function initialises a cipher operation
 *       context which will be referred using the operation parameter
 *
 * \param[out] operation Cipher operation context
 * \param[in]  key       Key slot to bind to the cipher context
 * \param[in]  alg       Algorithm to use for the cipher operation
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_cipher_encrypt_setup(
                                              psa_cipher_operation_t *operation,
                                              psa_key_slot_t key,
                                              psa_algorithm_t alg);
/**
 * \brief Set the cipher operation using the provided algorithm and key slot,
 *        for decryption context (veneer function)
 *
 * \note A successful call to this function initialises a cipher operation
 *       context which will be referred using the operation parameter
 *
 * \param[out] operation Cipher operation context
 * \param[in]  key       Key slot to bind to the cipher context
 * \param[in]  alg       Algorithm to use for the cipher operation
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_cipher_decrypt_setup(
                                              psa_cipher_operation_t *operation,
                                              psa_key_slot_t key,
                                              psa_algorithm_t alg);
/**
 * \brief Update the cipher context with a chunk of input data to create a
 *        chunk of encrypted output data (for encryption contexts), or to
 *        decrypt a chunk of encrypted input data to obtain decrypted data
 *        (for decryption contexts) (veneer function)
 *
 * \param[in]  operation Cipher operation context
 * \param[in]  input_s   Pointer to the struct containing input parameters
 * \param[out] output_s  Pointer to the struct containing output parameters
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_cipher_update(
                                     psa_cipher_operation_t *operation,
                                     struct psa_cipher_update_input *input_s,
                                     struct psa_cipher_update_output *output_s);
/**
 * \brief Finalise a cipher context flushing out any remaining block of
 *        output data (veneer function)
 *
 * \note A successful call to this function releases the cipher operation
 *       context provided as parameter
 *
 * \param[in,out] operation     Cipher operation context
 * \param[out]    output        Buffer containing output data
 * \param[in]     output_size   Size of the output buffer
 * \param[out]    output_length Size of the produced output
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_cipher_finish(
                                              psa_cipher_operation_t *operation,
                                              uint8_t *output,
                                              size_t output_size,
                                              size_t *output_length);
/**
 * \brief Abort a cipher operation, clear the operation context provided
 *        (veneer function)
 *
 * \note A successful call to this function releases the cipher operation
 *       context provided as parameter
 *
 * \param[in,out] operation Cipher operation context
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_cipher_abort(
                                             psa_cipher_operation_t *operation);
/**
 * \brief Setup a hash operation with the provided algorithm (veneer function)
 *
 * \note A successful call to this function initialises a hash operation
 *       context which will be referred using the operation parameter
 *
 * \param[out] operation Hash operation context
 * \param[in]  alg       Algorithm chosen as hash
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_hash_setup(
                                                psa_hash_operation_t *operation,
                                                psa_algorithm_t alg);
/**
 * \brief Add a new input chunk to the data for which the final hash value
 *        will be computed (veneer function)
 *
 * \param[in,out] operation    Hash operation context
 * \param[in]     input        Buffer containing the input data
 * \param[in]     input_length Size of the provided input data
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_hash_update(
                                                psa_hash_operation_t *operation,
                                                const uint8_t *input,
                                                size_t input_length);
/**
 * \brief Finalise a hash context operation producing the final hash value
 *        (veneer function)
 *
 * \note A successful call to this function releases the hash operation
 *       context provided as parameter
 *
 * \param[in,out] operation   Hash operation context
 * \param[out]    hash        Buffer containing hash data
 * \param[in]     hash_size   Size of the hash buffer
 * \param[out]    hash_length Size of the produced hash
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_hash_finish(
                                                psa_hash_operation_t *operation,
                                                uint8_t *hash,
                                                size_t hash_size,
                                                size_t *hash_length);
/**
 * \brief Finalise a hash context operation, verifying that the final hash
 *        value matches the one provided as input (veneer function)
 *
 * \note A successful call to this function releases the hash operation
 *       context provided as parameter. The hash operation is released
 *       also in case TFM_CRYPTO_ERR_PSA_ERROR_INVALID_SIGNATURE is returned
 *
 * \param[in,out] operation   Hash operation context
 * \param[in]     hash        Buffer containing the provided hash value
 * \param[in]     hash_length Size of the provided hash value
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_hash_verify(
                                                psa_hash_operation_t *operation,
                                                const uint8_t *hash,
                                                size_t hash_length);
/**
 * \brief Abort a hash operation, clears the operation context provided
 *        (veneer function)
 *
 * \note A successful call to this function releases the hash operation
 *       context provided as parameter
 *
 * \param[in,out] operation Hash operation context
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_hash_abort(
                                               psa_hash_operation_t *operation);
/**
 * \brief Start a MAC operation with the provided algorithm (for signing)
 *        (veneer function)
 *
 * \note A successful call to this function initialises a MAC operation
 *       context which will be referred using the operation parameter
 *
 * \param[out] operation MAC operation context
 * \param[in]  key       Key slot to bind to the MAC context
 * \param[in]  alg       Algorithm chosen as MAC
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_mac_sign_setup(
                                                 psa_mac_operation_t *operation,
                                                 psa_key_slot_t key,
                                                 psa_algorithm_t alg);
/**
 * \brief Start a MAC operation with the provided algorithm (for verifying)
 *        (veneer function)
 *
 * \note A successful call to this function initialises a MAC operation
 *       context which will be referred using the operation parameter
 *
 * \param[out] operation MAC operation context
 * \param[in]  key       Key slot to bind to the MAC context
 * \param[in]  alg       Algorithm chosen as MAC
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_mac_verify_setup(
                                                 psa_mac_operation_t *operation,
                                                 psa_key_slot_t key,
                                                 psa_algorithm_t alg);
/**
 * \brief Adds a new input chunk to the data for which the final MAC value
 *        will be computed (veneer function)
 *
 * \param[in,out] operation    MAC operation context
 * \param[in]     input        Buffer containing the input data
 * \param[in]     input_length Size of the provided input data
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_mac_update(
                                                 psa_mac_operation_t *operation,
                                                 const uint8_t *input,
                                                 size_t input_length);

/**
 * \brief Finalises a MAC context operation producing the final MAC value
 *        (veneer function)
 *
 * \note A successful call to this function releases the MAC operation
 *       context provided as parameter
 *
 * \param[in,out] operation   MAC operation context
 * \param[out]    mac         Buffer containing MAC data
 * \param[in]     mac_size    Size of the MAC buffer
 * \param[out]    mac_length  Size of the produced MAC
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_mac_sign_finish(
                                                 psa_mac_operation_t *operation,
                                                 uint8_t *mac,
                                                 size_t mac_size,
                                                 size_t *mac_length);
/**
 * \brief Finalise a MAC context operation, verifying that the final MAC value
 *        matches the one provided as input (veneer function)
 *
 * \note A successful call to this function releases the MAC operation
 *       context provided as parameter. The MAC operation is released
 *       also in case TFM_CRYPTO_ERR_PSA_ERROR_INVALID_SIGNATURE is returned
 *
 * \param[in,out] operation   MAC operation context
 * \param[in]     mac         Buffer containing the provided MAC value
 * \param[in]     mac_length  Size of the provided MAC value
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_mac_verify_finish(
                                                 psa_mac_operation_t *operation,
                                                 const uint8_t *mac,
                                                 size_t mac_length);
/**
 * \brief Abort a MAC operation, clear the operation context provided
 *        (veneer function)
 *
 * \note A successful call to this function releases the MAC operation
 *       context provided as parameter
 *
 * \param[in,out] operation MAC operation context
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_mac_abort(
                                                psa_mac_operation_t *operation);
/**
 * \brief Perform an AEAD encryption operation on input data with additional
 *        data to be authenticated, producing ciphertext in output with an
 *        appended authentication tag (veneer function)
 *
 * \param[in]  input_s   Pointer to the struct containing input parameters
 * \param[out] output_s  Pointer to the struct containing output parameters
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_aead_encrypt(
                                     struct psa_aead_encrypt_input *input_s,
                                     struct psa_aead_encrypt_output *output_s);
/**
 * \brief Perform an AEAD decryption operation on input data with additional
 *        data to be verified, producing back the original plain text in case
 *        the verification of the authentication tag is successful (veneer
 *        function)
 *
 * \param[in]  input_s   Pointer to the struct containing input parameters
 * \param[out] output_s  Pointer to the struct containing output parameters
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_aead_decrypt(
                                     struct psa_aead_decrypt_input *input_s,
                                     struct psa_aead_decrypt_output *output_s);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_CRYPTO_VENEERS_H__ */
