/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CRYPTO_DEFS_H__
#define __TFM_CRYPTO_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <limits.h>
#include "tfm_api.h"
#include "psa/crypto.h"

/**
 * \brief This type is used to overcome a limitation in the number of maximum
 *        IOVECs that can be used especially in psa_aead_encrypt and
 *        psa_aead_decrypt. To be removed in case the AEAD APIs number of
 *        parameters passed gets restructured
 */
#define TFM_CRYPTO_MAX_NONCE_LENGTH (16u)
struct tfm_crypto_aead_pack_input {
    uint8_t nonce[TFM_CRYPTO_MAX_NONCE_LENGTH];
    uint32_t nonce_length;
};

/**
 * \brief Structure used to pack non-pointer types in a call
 *
 */
struct tfm_crypto_pack_iovec {
    uint32_t sfn_id;             /*!< Secure function ID used to dispatch the
                                  *   request
                                  */
    uint16_t step;               /*!< Key derivation step */
    psa_key_handle_t key_handle; /*!< Key handle */
    psa_algorithm_t alg;         /*!< Algorithm */
    uint32_t op_handle;          /*!< Frontend context handle associated to a
                                  *   multipart operation
                                  */
    size_t capacity;             /*!< Key derivation capacity */

    struct tfm_crypto_aead_pack_input aead_in; /*!< FixMe: Temporarily used for
                                                *   AEAD until the API is
                                                *   restructured
                                                */
};

/**
 * \brief Define a progressive numerical value for each SID which can be used
 *        when dispatching the requests to the service
 */
enum {
    TFM_CRYPTO_GET_KEY_ATTRIBUTES_SID = (0u),
    TFM_CRYPTO_RESET_KEY_ATTRIBUTES_SID,
    TFM_CRYPTO_OPEN_KEY_SID,
    TFM_CRYPTO_CLOSE_KEY_SID,
    TFM_CRYPTO_IMPORT_KEY_SID,
    TFM_CRYPTO_DESTROY_KEY_SID,
    TFM_CRYPTO_EXPORT_KEY_SID,
    TFM_CRYPTO_EXPORT_PUBLIC_KEY_SID,
    TFM_CRYPTO_COPY_KEY_SID,
    TFM_CRYPTO_HASH_COMPUTE_SID,
    TFM_CRYPTO_HASH_COMPARE_SID,
    TFM_CRYPTO_HASH_SETUP_SID,
    TFM_CRYPTO_HASH_UPDATE_SID,
    TFM_CRYPTO_HASH_FINISH_SID,
    TFM_CRYPTO_HASH_VERIFY_SID,
    TFM_CRYPTO_HASH_ABORT_SID,
    TFM_CRYPTO_HASH_CLONE_SID,
    TFM_CRYPTO_MAC_COMPUTE_SID,
    TFM_CRYPTO_MAC_VERIFY_SID,
    TFM_CRYPTO_MAC_SIGN_SETUP_SID,
    TFM_CRYPTO_MAC_VERIFY_SETUP_SID,
    TFM_CRYPTO_MAC_UPDATE_SID,
    TFM_CRYPTO_MAC_SIGN_FINISH_SID,
    TFM_CRYPTO_MAC_VERIFY_FINISH_SID,
    TFM_CRYPTO_MAC_ABORT_SID,
    TFM_CRYPTO_CIPHER_ENCRYPT_SID,
    TFM_CRYPTO_CIPHER_DECRYPT_SID,
    TFM_CRYPTO_CIPHER_ENCRYPT_SETUP_SID,
    TFM_CRYPTO_CIPHER_DECRYPT_SETUP_SID,
    TFM_CRYPTO_CIPHER_GENERATE_IV_SID,
    TFM_CRYPTO_CIPHER_SET_IV_SID,
    TFM_CRYPTO_CIPHER_UPDATE_SID,
    TFM_CRYPTO_CIPHER_FINISH_SID,
    TFM_CRYPTO_CIPHER_ABORT_SID,
    TFM_CRYPTO_AEAD_ENCRYPT_SID,
    TFM_CRYPTO_AEAD_DECRYPT_SID,
    TFM_CRYPTO_AEAD_ENCRYPT_SETUP_SID,
    TFM_CRYPTO_AEAD_DECRYPT_SETUP_SID,
    TFM_CRYPTO_AEAD_GENERATE_NONCE_SID,
    TFM_CRYPTO_AEAD_SET_NONCE_SID,
    TFM_CRYPTO_AEAD_SET_LENGTHS_SID,
    TFM_CRYPTO_AEAD_UPDATE_AD_SID,
    TFM_CRYPTO_AEAD_UPDATE_SID,
    TFM_CRYPTO_AEAD_FINISH_SID,
    TFM_CRYPTO_AEAD_VERIFY_SID,
    TFM_CRYPTO_AEAD_ABORT_SID,
    TFM_CRYPTO_SIGN_HASH_SID,
    TFM_CRYPTO_VERIFY_HASH_SID,
    TFM_CRYPTO_ASYMMETRIC_ENCRYPT_SID,
    TFM_CRYPTO_ASYMMETRIC_DECRYPT_SID,
    TFM_CRYPTO_KEY_DERIVATION_SETUP_SID,
    TFM_CRYPTO_KEY_DERIVATION_GET_CAPACITY_SID,
    TFM_CRYPTO_KEY_DERIVATION_SET_CAPACITY_SID,
    TFM_CRYPTO_KEY_DERIVATION_INPUT_BYTES_SID,
    TFM_CRYPTO_KEY_DERIVATION_INPUT_KEY_SID,
    TFM_CRYPTO_KEY_DERIVATION_KEY_AGREEMENT_SID,
    TFM_CRYPTO_KEY_DERIVATION_OUTPUT_BYTES_SID,
    TFM_CRYPTO_KEY_DERIVATION_OUTPUT_KEY_SID,
    TFM_CRYPTO_KEY_DERIVATION_ABORT_SID,
    TFM_CRYPTO_RAW_KEY_AGREEMENT_SID,
    TFM_CRYPTO_GENERATE_RANDOM_SID,
    TFM_CRYPTO_GENERATE_KEY_SID,
    TFM_CRYPTO_SET_KEY_DOMAIN_PARAMETERS_SID,
    TFM_CRYPTO_GET_KEY_DOMAIN_PARAMETERS_SID,
    TFM_CRYPTO_SID_MAX,
};

/**
 * \brief Define an invalid value for an SID
 *
 */
#define TFM_CRYPTO_SID_INVALID (~0x0u)

/**
 * \brief This value is used to mark an handle as invalid.
 *
 */
#define TFM_CRYPTO_INVALID_HANDLE (0x0u)

/**
 * \brief The persistent key identifier that refers to the hardware unique key.
 *
 */
#define TFM_CRYPTO_KEY_ID_HUK (0xFFFF815Bu)

/**
 * \brief The algorithm identifier that refers to key derivation from the
 *        hardware unique key.
 *
 */
#define TFM_CRYPTO_ALG_HUK_DERIVATION ((psa_algorithm_t)0xB0000F00)

/**
 * \brief Define miscellaneous literal constants that are used in the service
 *
 */
enum {
    TFM_CRYPTO_NOT_IN_USE = 0,
    TFM_CRYPTO_IN_USE = 1
};

#ifdef __cplusplus
}
#endif

#endif /* __TFM_CRYPTO_DEFS_H__ */
