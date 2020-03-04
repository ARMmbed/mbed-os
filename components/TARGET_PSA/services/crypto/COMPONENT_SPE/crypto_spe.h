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
/**
 * \file psa/crypto_spe.h
 * \brief Platform Security Architecture cryptography module
 */

#ifndef PSA_CRYPTO_SPE_H
#define PSA_CRYPTO_SPE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define psa_crypto_init  psa_sec_crypto_init
#define psa_get_key_attributes  psa_sec_get_key_attributes
#define psa_reset_key_attributes psa_sec_reset_key_attributes
#define psa_open_key  psa_sec_open_key
#define psa_close_key  psa_sec_close_key
#define psa_import_key  psa_sec_import_key
#define psa_destroy_key  psa_sec_destroy_key
#define psa_export_key  psa_sec_export_key
#define psa_export_public_key  psa_sec_export_public_key
#define psa_copy_key psa_sec_copy_key
#define psa_hash_compute  psa_sec_hash_compute
#define psa_hash_compare  psa_sec_hash_compare
#define psa_hash_setup  psa_sec_hash_setup
#define psa_hash_update  psa_sec_hash_update
#define psa_hash_finish  psa_sec_hash_finish
#define psa_hash_verify  psa_sec_hash_verify
#define psa_hash_abort  psa_sec_hash_abort
#define psa_hash_clone  psa_sec_hash_clone
#define psa_mac_compute  psa_sec_mac_compute
#define psa_mac_verify  psa_sec_mac_verify
#define psa_mac_sign_setup  psa_sec_mac_sign_setup
#define psa_mac_verify_setup  psa_sec_mac_verify_setup
#define psa_mac_update  psa_sec_mac_update
#define psa_mac_sign_finish  psa_sec_mac_sign_finish
#define psa_mac_verify_finish  psa_sec_mac_verify_finish
#define psa_mac_abort  psa_sec_mac_abort
#define psa_cipher_encrypt  psa_sec_cipher_encrypt
#define psa_cipher_decrypt  psa_sec_cipher_decrypt
#define psa_cipher_encrypt_setup  psa_sec_cipher_encrypt_setup
#define psa_cipher_decrypt_setup  psa_sec_cipher_decrypt_setup
#define psa_cipher_generate_iv  psa_sec_cipher_generate_iv
#define psa_cipher_set_iv  psa_sec_cipher_set_iv
#define psa_cipher_update  psa_sec_cipher_update
#define psa_cipher_finish  psa_sec_cipher_finish
#define psa_cipher_abort  psa_sec_cipher_abort
#define psa_aead_encrypt  psa_sec_aead_encrypt
#define psa_aead_decrypt  psa_sec_aead_decrypt
#define psa_aead_encrypt_setup  psa_sec_aead_encrypt_setup
#define psa_aead_decrypt_setup  psa_sec_aead_decrypt_setup
#define psa_aead_generate_nonce  psa_sec_aead_generate_nonce
#define psa_aead_set_nonce  psa_sec_aead_set_nonce
#define psa_aead_set_lengths  psa_sec_aead_set_lengths
#define psa_aead_update_ad  psa_sec_aead_update_ad
#define psa_aead_update  psa_sec_aead_update
#define psa_aead_finish  psa_sec_aead_finish
#define psa_aead_verify  psa_sec_aead_verify
#define psa_aead_abort  psa_sec_aead_abort
#define psa_sign_hash  psa_sec_sign_hash
#define psa_verify_hash  psa_sec_verify_hash
#define psa_asymmetric_encrypt  psa_sec_asymmetric_encrypt
#define psa_asymmetric_decrypt  psa_sec_asymmetric_decrypt
#define psa_key_derivation_setup  psa_sec_key_derivation_setup
#define psa_key_derivation_get_capacity  psa_sec_key_derivation_get_capacity
#define psa_key_derivation_set_capacity  psa_sec_key_derivation_set_capacity
#define psa_key_derivation_input_bytes  psa_sec_key_derivation_input_bytes
#define psa_key_derivation_input_key  psa_sec_key_derivation_input_key
#define psa_key_derivation_key_agreement  psa_sec_key_derivation_key_agreement
#define psa_key_derivation_output_bytes  psa_sec_key_derivation_output_bytes
#define psa_key_derivation_output_key  psa_sec_key_derivation_output_key
#define psa_key_derivation_abort  psa_sec_key_derivation_abort
#define psa_raw_key_agreement  psa_sec_raw_key_agreement
#define psa_generate_random  psa_sec_generate_random
#define psa_generate_key  psa_sec_generate_key

#define mbedtls_psa_crypto_free  mbedtls_psa_sec_crypto_free
#define mbedtls_psa_inject_entropy  mbedtls_psa_sec_inject_entropy
#define psa_set_key_domain_parameters psa_sec_set_key_domain_parameters
#define psa_get_key_domain_parameters psa_sec_get_key_domain_parameters

#define MBEDTLS_PSA_CRYPTO_KEY_FILE_ID_ENCODES_OWNER 1

#include "crypto.h"

#ifdef __cplusplus
}
#endif

/* The file "crypto_extra.h" contains vendor-specific definitions. This
 * can include vendor-defined algorithms, extra functions, etc. */
#include "crypto_extra.h"

#endif /* PSA_CRYPTO_SPE_H */
