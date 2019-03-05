/**
 * \file psa/crypto_platform_spe.h
 *
 * \brief PSA cryptography module: Mbed TLS platfom definitions
 */
/*
 *  Copyright (C) 2018, ARM Limited, All Rights Reserved
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

#ifndef PSA_CRYPTO_SPE_PLATFORM_H
#define PSA_CRYPTO_SPE_PLATFORM_H

/* Include the Mbed TLS configuration file, the way Mbed TLS does it
 * in each of its header files. */
#if !defined(MBEDTLS_CONFIG_FILE)
#include "../mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/** \defgroup PSA Crypto APIs
* @{
*/

/** \brief psa_s_function_t enum defines for all the available functions in PSA Crypto. */
typedef enum psa_sec_function_s {
    PSA_CRYPTO_INVALID,
    PSA_CRYPTO_INIT,
    PSA_IMPORT_KEY,
    PSA_ALLOCATE_KEY,
    PSA_CREATE_KEY,
    PSA_OPEN_KEY,
    PSA_CLOSE_KEY,
    PSA_DESTROY_KEY,
    PSA_GET_KEY_INFORMATION,
    PSA_EXPORT_KEY,
    PSA_EXPORT_PUBLIC_KEY,
    PSA_SET_KEY_POLICY,
    PSA_GET_KEY_POLICY,
    PSA_GET_KEY_LIFETIME,
    PSA_HASH_SETUP,
    PSA_HASH_UPDATE,
    PSA_HASH_FINISH,
    PSA_HASH_VERIFY,
    PSA_HASH_ABORT,
    PSA_HASH_CLONE_BEGIN,
    PSA_HASH_CLONE_END,
    PSA_MAC_SIGN_SETUP,
    PSA_MAC_VERIFY_SETUP,
    PSA_MAC_UPDATE,
    PSA_MAC_SIGN_FINISH,
    PSA_MAC_VERIFY_FINISH,
    PSA_MAC_ABORT,
    PSA_CIPHER_ENCRYPT_SETUP,
    PSA_CIPHER_DECRYPT_SETUP,
    PSA_CIPHER_GENERATE_IV,
    PSA_CIPHER_SET_IV,
    PSA_CIPHER_UPDATE,
    PSA_CIPHER_FINISH,
    PSA_CIPHER_ABORT,
    PSA_AEAD_ENCRYPT,
    PSA_AEAD_DECRYPT,
    PSA_ASYMMETRIC_SIGN,
    PSA_ASYMMETRIC_VERIFY,
    PSA_ASYMMETRIC_ENCRYPT,
    PSA_ASYMMETRIC_DECRYPT,
    PSA_GENERATE_RANDOM,
    PSA_GENERATE_KEY,
    PSA_GET_GENERATOR_CAPACITY,
    PSA_GENERATOR_READ,
    PSA_GENERATOR_IMPORT_KEY,
    PSA_GENERATOR_ABORT,
    PSA_KEY_DERIVATION,
    PSA_KEY_AGREEMENT
} psa_sec_function_t;

/**@}*/

/** \defgroup PSA Crypto structures for IPC
* @{
*/

/** psa_crypto_ipc_s struct used for some of the
 * PSA Crypto APIs that need psa_key_handle_t and psa_algorithm_t arguments
 * and in order to use the existing infrastructure of the SPM-IPC we provide a struct to
 * pack them together.
 */
typedef struct psa_crypto_ipc_s {
    psa_sec_function_t func;
    psa_key_handle_t handle;
    psa_algorithm_t alg;
} psa_crypto_ipc_t;

/** psa_crypto_derivation_ipc_s struct used for some of the
 * PSA Crypto APIs that need psa_key_handle_t and psa_algorithm_t arguments
 * and in order to use the existing infrastructure of the SPM-IPC we provide a struct to
 * pack them together.
 */
typedef struct psa_crypto_derivation_ipc_s {
    psa_sec_function_t func;
    psa_key_handle_t handle;
    psa_algorithm_t alg;
    size_t capacity;
} psa_crypto_derivation_ipc_t;

/** psa_key_mng_ipc_s struct used for some of the
 * PSA Crypto APIs that need psa_key_handle_t and psa_algorithm_t arguments
 * and in order to use the existing infrastructure of the SPM-IPC we provide a struct to
 * pack them together.
 */
typedef struct psa_key_mng_ipc_s {
    psa_key_handle_t handle;
    psa_key_lifetime_t lifetime;
    psa_key_type_t type;
    psa_sec_function_t func;
} psa_key_mng_ipc_t;

/** psa_crypto_ipc_aead_s struct used for AEAD integrated
 * PSA Crypto APIs that need psa_key_handle_t and psa_algorithm_t and extra arguments
 * and in order to use the existing infrastructure of the SPM-IPC we provide a struct to
 * pack them together.
 */
// Max length supported for nonce is 16 bytes.
#define PSA_AEAD_MAX_NONCE_SIZE 16
typedef struct psa_crypto_ipc_aead_s {
    psa_sec_function_t func;
    psa_key_handle_t handle;
    psa_algorithm_t alg;
    uint16_t nonce_size;
    size_t additional_data_length;
    size_t input_length;
    uint8_t nonce[PSA_AEAD_MAX_NONCE_SIZE];
} psa_crypto_ipc_aead_t;

/** psa_crypto_ipc_asymmetric_s struct used for asymmetric
 * PSA Crypto APIs that need psa_key_handle_t and psa_algorithm_t arguments
 * and in order to use the existing infrastructure of the SPM-IPC we provide a struct to
 * pack them together.
 */
typedef struct psa_crypto_ipc_asymmetric_s {
    psa_sec_function_t func;
    psa_key_handle_t handle;
    psa_algorithm_t alg;
    size_t input_length;
    size_t salt_length;
} psa_crypto_ipc_asymmetric_t;

/**@}*/

#endif /* PSA_CRYPTO_SPE_PLATFORM_H */
