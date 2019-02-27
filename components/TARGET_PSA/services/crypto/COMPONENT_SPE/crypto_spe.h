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
#define psa_import_key  psa_sec_import_key
#define psa_destroy_key  psa_sec_destroy_key
#define psa_get_key_information  psa_sec_get_key_information
#define psa_export_key  psa_sec_export_key
#define psa_export_public_key  psa_sec_export_public_key
#define psa_key_policy_init  psa_key_policy_init
#define psa_key_policy_get_usage  psa_key_policy_get_usage
#define psa_key_policy_get_algorithm  psa_key_policy_get_algorithm
#define psa_key_policy_set_usage  psa_key_policy_set_usage
#define psa_set_key_policy  psa_sec_set_key_policy
#define psa_get_key_policy  psa_sec_get_key_policy
#define psa_get_key_lifetime  psa_sec_get_key_lifetime
#define psa_hash_setup  psa_sec_hash_setup
#define psa_hash_update  psa_sec_hash_update
#define psa_hash_finish  psa_sec_hash_finish
#define psa_hash_verify  psa_sec_hash_verify
#define psa_hash_abort  psa_sec_hash_abort
#define psa_mac_sign_setup  psa_sec_mac_sign_setup
#define psa_mac_verify_setup  psa_sec_mac_verify_setup
#define psa_mac_update  psa_sec_mac_update
#define psa_mac_sign_finish  psa_sec_mac_sign_finish
#define psa_mac_verify_finish  psa_sec_mac_verify_finish
#define psa_mac_abort  psa_sec_mac_abort
#define psa_cipher_encrypt_setup  psa_sec_cipher_encrypt_setup
#define psa_cipher_decrypt_setup  psa_sec_cipher_decrypt_setup
#define psa_cipher_generate_iv  psa_sec_cipher_generate_iv
#define psa_cipher_set_iv  psa_sec_cipher_set_iv
#define psa_cipher_update  psa_sec_cipher_update
#define psa_cipher_finish  psa_sec_cipher_finish
#define psa_cipher_abort  psa_sec_cipher_abort
#define psa_aead_encrypt  psa_sec_aead_encrypt
#define psa_aead_decrypt  psa_sec_aead_decrypt
#define psa_asymmetric_sign  psa_sec_asymmetric_sign
#define psa_asymmetric_verify  psa_sec_asymmetric_verify
#define psa_asymmetric_encrypt  psa_sec_asymmetric_encrypt
#define psa_asymmetric_decrypt  psa_sec_asymmetric_decrypt
#define psa_generate_random  psa_sec_generate_random
#define psa_generate_key  psa_sec_generate_key
#define psa_get_generator_capacity  psa_sec_get_generator_capacity
#define psa_generator_read  psa_sec_generator_read
#define psa_generator_import_key  psa_sec_generator_import_key
#define mbedtls_psa_crypto_free  mbedtls_psa_sec_crypto_free
#define psa_key_derivation  psa_sec_key_derivation
#define psa_key_agreement  psa_sec_key_agreement
#define psa_generator_abort  psa_sec_generator_abort
#define mbedtls_psa_inject_entropy  mbedtls_psa_sec_inject_entropy
#define psa_allocate_key  psa_sec_allocate_key
#define psa_open_key  psa_sec_open_key
#define psa_create_key  psa_sec_create_key
#define psa_close_key  psa_sec_close_key
#define psa_hash_clone  psa_sec_hash_clone

#define MBEDTLS_PSA_CRYPTO_KEY_FILE_ID_ENCODES_OWNER 1

#include "crypto.h"

#ifdef __cplusplus
}
#endif

/* The file "crypto_extra.h" contains vendor-specific definitions. This
 * can include vendor-defined algorithms, extra functions, etc. */
#include "crypto_extra.h"

#endif /* PSA_CRYPTO_SPE_H */
