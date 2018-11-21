/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
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
 * \brief Import the key data on the provided key slot (veneer function)
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
 * \param[in] key          Key slot
 * \param[out] data        Buffer to hold the exported key
 * \param[in] data_size    Length of the buffer pointed to by data
 * \param[out] data_length Length of the exported key
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_export_key(psa_key_slot_t key,
                                                   uint8_t *data,
                                                   size_t data_size,
                                                   size_t *data_length);
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
enum tfm_crypto_err_t tfm_crypto_veneer_encrypt_set_iv(
                                              psa_cipher_operation_t *operation,
                                              const unsigned char *iv,
                                              size_t iv_length);
/**
 * \brief Set the cipher operation using the provided algorithm and key slot,
 *        for encryption context (veneer function)
 *
 * \param[in] operation Cipher operation context
 * \param[in] key       Key slot to bind to the cipher context
 * \param[in] alg       Algorithm to use for the cipher operation
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_encrypt_setup(
                                              psa_cipher_operation_t *operation,
                                              psa_key_slot_t key,
                                              psa_algorithm_t alg);
/**
 * \brief Set the cipher operation using the provided algorithm and key slot,
 *        for decryption context (veneer function)
 *
 * \param[in] operation Cipher operation context
 * \param[in] key       Key slot to bind to the cipher context
 * \param[in] alg       Algorithm to use for the cipher operation
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_decrypt_setup(
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
 * \brief Abort a cipher operation, clears the operation context provided
 *        (veneer function)
 *
 * \param[in]  operation     Cipher operation context
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_cipher_abort(
                                             psa_cipher_operation_t *operation);

/**
 * \brief Finalise a cipher context flushing out any remaining block of
 *        output data (veneer function)
 *
 * \param[in]  operation     Cipher operation context
 * \param[out] output        Buffer containing output data
 * \param[in]  output_size   Size of the output buffer
 * \param[out] output_length Size of the produced output
 *
 * \return Return values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_cipher_finish(
                                              psa_cipher_operation_t *operation,
                                              uint8_t *output,
                                              size_t output_size,
                                              size_t *output_length);
/**
 * \brief Starts a hash operation with the provided algorithm (veneer function)
 *
 * \param[in] operation Hash operation context
 * \param[in] alg       Algorithm chosen as hash
 *
 * \return Returns values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_hash_start(
                                                psa_hash_operation_t *operation,
                                                psa_algorithm_t alg);
/**
 * \brief Adds a new input chunk to the data for which the final hash value
 *        will be computed (veneer function)
 *
 * \param[in] operation    Hash operation context
 * \param[in] input        Buffer containing the input data
 * \param[in] input_length Size of the provided input data
 *
 * \return Returns values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_hash_update(
                                                psa_hash_operation_t *operation,
                                                const uint8_t *input,
                                                size_t input_length);
/**
 * \brief Finalises a hash context operation producing the final hash value
 *        (veneer function)
 *
 * \param[in]  operation   Hash operation context
 * \param[out] hash        Buffer containing hash data
 * \param[in]  hash_size   Size of the hash buffer
 * \param[out] hash_length Size of the produced hash
 *
 * \return Returns values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_hash_finish(
                                                psa_hash_operation_t *operation,
                                                uint8_t *hash,
                                                size_t hash_size,
                                                size_t *hash_length);
/**
 * \brief Finalises a hash context operation, verifying that the final hash
 *        value matches the one provided as input (veneer function)
 *
 * \param[in] operation   Hash operation context
 * \param[in] hash        Buffer containing the provided hash value
 * \param[in] hash_length Size of the provided hash value
 *
 * \return Returns values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_hash_verify(
                                                psa_hash_operation_t *operation,
                                                const uint8_t *hash,
                                                size_t hash_length);
/**
 * \brief Aborts a hash operation, clears the operation context provided
 *        (veneer function)
 *
 * \param[in] operation Hash operation context
 *
 * \return Returns values as described in \ref tfm_crypto_err_t
 */
enum tfm_crypto_err_t tfm_crypto_veneer_hash_abort(
                                               psa_hash_operation_t *operation);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_CRYPTO_VENEERS_H__ */
