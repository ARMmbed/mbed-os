/** @file
 * Copyright (c) 2018, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#define PSA_CRYPTO_IMPLEMENTED 1
#include "pal_crypto_intf.h"

#define  PAL_KEY_SLOT_COUNT  32

/**
    @brief    - This API will call the requested crypto function
    @param    - type    : function code
                valist  : variable argument list
    @return   - error status
**/
int32_t pal_crypto_function(int type, va_list valist)
{
#if PSA_CRYPTO_IMPLEMENTED
    int                     i;
    size_t                  size, *length, salt_length, label_length, ciphertext_size;
    uint8_t                 *buffer, *ciphertext;
    const uint8_t           *salt, *label, *nonce, *additional_data;
    uint8_t                 *plaintext;
    uint32_t                status;
    const void              *extra;
    size_t                  extra_size, capacity, *gen_cap, nonce_length, additional_data_length;
    psa_key_handle_t        handle, *key_handle;
    psa_key_type_t          key_type, *key_type_out;
    psa_key_policy_t        *policy;
    psa_key_usage_t         usage, *usage_out;
    psa_key_lifetime_t      *lifetime_out;
    psa_algorithm_t         alg, *alg_out;
    psa_hash_operation_t    *hash_operation;
    psa_mac_operation_t     *mac_operation;
    psa_cipher_operation_t  *cipher_operation;
    psa_crypto_generator_t  *generator;

    switch (type)
    {
        case PAL_CRYPTO_INIT:
            return psa_crypto_init();
        case PAL_CRYPTO_GENERATE_RANDOM:
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, int);
            return psa_generate_random(buffer, size);
        case PAL_CRYPTO_IMPORT_KEY:
            handle = (psa_key_handle_t)va_arg(valist, int);
            key_type = va_arg(valist, psa_key_type_t);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, int);
            status = psa_import_key(handle, key_type, buffer, size);
            return status;
        case PAL_CRYPTO_EXPORT_KEY:
            handle = (psa_key_handle_t)va_arg(valist, int);
            buffer = (uint8_t *)(va_arg(valist, uint8_t*));
            size = va_arg(valist, int);
            length = (size_t *)va_arg(valist, size_t*);
            status = psa_export_key(handle, buffer, size, length);
            return status;
        case PAL_CRYPTO_EXPORT_PUBLIC_KEY:
            handle = (psa_key_handle_t)va_arg(valist, int);
            buffer = (uint8_t *)(va_arg(valist, uint8_t*));
            size = va_arg(valist, int);
            length = (size_t *)va_arg(valist, size_t*);
            status = psa_export_public_key(handle, buffer, size, length);
            return status;
        case PAL_CRYPTO_KEY_POLICY_INIT:
            policy = va_arg(valist, psa_key_policy_t*);
            memset(policy, 0, sizeof(psa_key_policy_t));
            return 0;
        case PAL_CRYPTO_KEY_POLICY_SET_USAGE:
            policy = va_arg(valist, psa_key_policy_t*);
            usage = va_arg(valist, psa_key_usage_t);
            alg = va_arg(valist, psa_algorithm_t);
            psa_key_policy_set_usage(policy, usage, alg);
            return 0;
        case PAL_CRYPTO_SET_KEY_POLICY:
            handle = (psa_key_handle_t)va_arg(valist, int);
            policy = va_arg(valist, psa_key_policy_t*);
            return psa_set_key_policy(handle, policy);
        case PAL_CRYPTO_DESTROY_KEY:
            handle = (psa_key_handle_t)va_arg(valist, int);
            status = psa_destroy_key(handle);
            return status;
        case PAL_CRYPTO_GET_KEY_INFORMATION:
            handle = (psa_key_handle_t)va_arg(valist, int);
            key_type_out = va_arg(valist, psa_key_type_t*);
            length = (size_t *)va_arg(valist, size_t*);
            status = psa_get_key_information(handle, key_type_out, length);
            return status;
        case PAL_CRYPTO_GET_KEY_POLICY:
            handle = (psa_key_handle_t)va_arg(valist, int);
            policy = va_arg(valist, psa_key_policy_t*);
            return psa_get_key_policy(handle, policy);
        case PAL_CRYPTO_KEY_POLICY_GET_USAGE:
            policy = va_arg(valist, psa_key_policy_t*);
            usage_out = va_arg(valist, psa_key_usage_t*);
            *usage_out = psa_key_policy_get_usage(policy);
            return 0;
        case PAL_CRYPTO_KEY_POLICY_GET_ALGORITHM:
            policy = va_arg(valist, psa_key_policy_t*);
            alg_out = va_arg(valist, psa_algorithm_t*);
            *alg_out = psa_key_policy_get_algorithm(policy);
            return 0;
        case PAL_CRYPTO_GET_KEY_LIFETIME:
            handle = (psa_key_handle_t)va_arg(valist, int);
            lifetime_out = va_arg(valist, psa_key_lifetime_t*);
            return psa_get_key_lifetime(handle, lifetime_out);
        case PAL_CRYPTO_HASH_SETUP:
            hash_operation = va_arg(valist, psa_hash_operation_t*);
            alg = va_arg(valist, psa_algorithm_t);
            return psa_hash_setup(hash_operation, alg);
        case PAL_CRYPTO_HASH_UPDATE:
            hash_operation = va_arg(valist, psa_hash_operation_t*);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            return psa_hash_update(hash_operation, buffer, size);
        case PAL_CRYPTO_HASH_VERIFY:
            hash_operation = va_arg(valist, psa_hash_operation_t*);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            return psa_hash_verify(hash_operation, buffer, size);
        case PAL_CRYPTO_HASH_FINISH:
            hash_operation = va_arg(valist, psa_hash_operation_t*);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            length = va_arg(valist, size_t*);
            return psa_hash_finish(hash_operation, buffer, size, length);
        case PAL_CRYPTO_HASH_ABORT:
            hash_operation = va_arg(valist, psa_hash_operation_t*);
            return psa_hash_abort(hash_operation);
        case PAL_CRYPTO_GENERATE_KEY:
            handle = (psa_key_handle_t)va_arg(valist, int);
            key_type = va_arg(valist, psa_key_type_t);
            size     = va_arg(valist, size_t);
            extra    = va_arg(valist, const void*);
            extra_size  = va_arg(valist, size_t);
            return psa_generate_key(handle, key_type, size, extra, extra_size);
        case PAL_CRYPTO_GENERATOR_READ:
            generator = va_arg(valist, psa_crypto_generator_t*);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, int);
            return psa_generator_read(generator, buffer, size);
        case PAL_CRYPTO_KEY_DERIVATION:
            generator = va_arg(valist, psa_crypto_generator_t*);
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            salt = va_arg(valist, const uint8_t *);
            salt_length = va_arg(valist, size_t);
            label = va_arg(valist, const uint8_t *);
            label_length = va_arg(valist, size_t);
            capacity = va_arg(valist, size_t);
            return psa_key_derivation(generator, handle, alg, salt, salt_length, label,
                                                                  label_length, capacity);
        case PAL_CRYPTO_GET_GENERATOR_CAPACITY:
            generator = va_arg(valist, psa_crypto_generator_t*);
            gen_cap   = va_arg(valist, size_t*);
            return psa_get_generator_capacity(generator, gen_cap);
        case PAL_CRYPTO_GENERATOR_IMPORT_KEY:
            handle = (psa_key_handle_t)va_arg(valist, int);
            key_type = va_arg(valist, psa_key_type_t);
            size     = va_arg(valist, size_t);
            generator = va_arg(valist, psa_crypto_generator_t*);
            return psa_generator_import_key(handle, key_type, size, generator);
        case PAL_CRYPTO_GENERATOR_ABORT:
            generator = va_arg(valist, psa_crypto_generator_t*);
            return psa_generator_abort(generator);
        case PAL_CRYPTO_AEAD_ENCRYPT:
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            nonce = va_arg(valist, const uint8_t *);
            nonce_length = va_arg(valist, size_t);
            additional_data = va_arg(valist, const uint8_t *);
            additional_data_length = va_arg(valist, size_t);
            plaintext = va_arg(valist, uint8_t *);
            size = va_arg(valist, size_t);
            ciphertext = va_arg(valist, uint8_t *);
            ciphertext_size = va_arg(valist, size_t);
            length = va_arg(valist, size_t*);
            return psa_aead_encrypt(handle, alg, nonce, nonce_length, additional_data,
                    additional_data_length, plaintext, size, ciphertext, ciphertext_size, length);
        case PAL_CRYPTO_AEAD_DECRYPT:
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            nonce = va_arg(valist, const uint8_t *);
            nonce_length = va_arg(valist, size_t);
            additional_data = va_arg(valist, const uint8_t *);
            additional_data_length = va_arg(valist, size_t);
            ciphertext = va_arg(valist, uint8_t *);
            ciphertext_size = va_arg(valist, size_t);
            plaintext = va_arg(valist, uint8_t *);
            size = va_arg(valist, size_t);
            length = va_arg(valist, size_t*);
            return psa_aead_decrypt(handle, alg, nonce, nonce_length, additional_data,
                    additional_data_length, ciphertext, ciphertext_size, plaintext, size, length);
        case PAL_CRYPTO_MAC_SIGN_SETUP:
            mac_operation = va_arg(valist, psa_mac_operation_t*);
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            return psa_mac_sign_setup(mac_operation, handle, alg);
        case PAL_CRYPTO_MAC_UPDATE:
            mac_operation = va_arg(valist, psa_mac_operation_t*);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            return psa_mac_update(mac_operation, buffer, size);
        case PAL_CRYPTO_MAC_SIGN_FINISH:
            mac_operation = va_arg(valist, psa_mac_operation_t*);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            length = (size_t *)va_arg(valist, size_t*);
            return psa_mac_sign_finish(mac_operation, buffer, size, length);
        case PAL_CRYPTO_MAC_VERIFY_SETUP:
            mac_operation = va_arg(valist, psa_mac_operation_t*);
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            return psa_mac_verify_setup(mac_operation, handle, alg);
        case PAL_CRYPTO_MAC_VERIFY_FINISH:
            mac_operation = va_arg(valist, psa_mac_operation_t*);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            return psa_mac_verify_finish(mac_operation, buffer, size);
        case PAL_CRYPTO_MAC_ABORT:
            mac_operation = va_arg(valist, psa_mac_operation_t*);
            return psa_mac_abort(mac_operation);
        case PAL_CRYPTO_ASYMMTERIC_ENCRYPT:
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            plaintext = va_arg(valist, uint8_t *);
            size = va_arg(valist, size_t);
            salt = va_arg(valist, const uint8_t *);
            salt_length = va_arg(valist, size_t);
            ciphertext = va_arg(valist, uint8_t *);
            ciphertext_size = va_arg(valist, size_t);
            length = va_arg(valist, size_t*);
            return psa_asymmetric_encrypt(handle, alg, plaintext, size, salt, salt_length,
                    ciphertext, ciphertext_size, length);
        case PAL_CRYPTO_ASYMMTERIC_DECRYPT:
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            plaintext = va_arg(valist, uint8_t *);
            size = va_arg(valist, size_t);
            salt = va_arg(valist, const uint8_t *);
            salt_length = va_arg(valist, size_t);
            ciphertext = va_arg(valist, uint8_t *);
            ciphertext_size = va_arg(valist, size_t);
            length = va_arg(valist, size_t*);
            return psa_asymmetric_decrypt(handle, alg, plaintext, size, salt, salt_length,
                    ciphertext, ciphertext_size, length);
        case PAL_CRYPTO_CIPHER_ENCRYPT_SETUP:
            cipher_operation =  va_arg(valist, psa_cipher_operation_t *);
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            return psa_cipher_encrypt_setup(cipher_operation, handle, alg);
        case PAL_CRYPTO_CIPHER_DECRYPT_SETUP:
            cipher_operation =  va_arg(valist, psa_cipher_operation_t *);
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            return psa_cipher_decrypt_setup(cipher_operation, handle, alg);
        case PAL_CRYPTO_CIPHER_GENERATE_IV:
            cipher_operation =  va_arg(valist, psa_cipher_operation_t *);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            length = va_arg(valist, size_t*);
            return psa_cipher_generate_iv(cipher_operation, buffer, size, length);
        case PAL_CRYPTO_CIPHER_SET_IV:
            cipher_operation =  va_arg(valist, psa_cipher_operation_t *);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            return psa_cipher_set_iv(cipher_operation, buffer, size);
        case PAL_CRYPTO_CIPHER_UPDATE:
            cipher_operation =  va_arg(valist, psa_cipher_operation_t *);
            plaintext = va_arg(valist, uint8_t *);
            size = va_arg(valist, size_t);
            ciphertext = va_arg(valist, uint8_t *);
            ciphertext_size = va_arg(valist, size_t);
            length = va_arg(valist, size_t*);
            return psa_cipher_update(cipher_operation, plaintext, size, ciphertext, ciphertext_size,
                                     length);
        case PAL_CRYPTO_CIPHER_FINISH:
            cipher_operation =  va_arg(valist, psa_cipher_operation_t *);
            ciphertext = va_arg(valist, uint8_t *);
            ciphertext_size = va_arg(valist, size_t);
            length = va_arg(valist, size_t*);
            return psa_cipher_finish(cipher_operation, ciphertext, ciphertext_size, length);
        case PAL_CRYPTO_CIPHER_ABORT:
            cipher_operation =  va_arg(valist, psa_cipher_operation_t *);
            return psa_cipher_abort(cipher_operation);
        case PAL_CRYPTO_ASYMMTERIC_SIGN:
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            ciphertext = va_arg(valist, uint8_t *);
            ciphertext_size = va_arg(valist, size_t);
            length = va_arg(valist, size_t*);
            return psa_asymmetric_sign(handle, alg, buffer, size, ciphertext, ciphertext_size,
                                       length);
        case PAL_CRYPTO_ASYMMTERIC_VERIFY:
            handle = (psa_key_handle_t)va_arg(valist, int);
            alg = va_arg(valist, psa_algorithm_t);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            ciphertext = va_arg(valist, uint8_t *);
            ciphertext_size = va_arg(valist, size_t);
            return psa_asymmetric_verify(handle, alg, buffer, size, ciphertext, ciphertext_size);
        case PAL_CRYPTO_KEY_AGREEMENT:
            generator = va_arg(valist, psa_crypto_generator_t*);
            handle = (psa_key_handle_t)va_arg(valist, int);
            buffer = va_arg(valist, uint8_t*);
            size = va_arg(valist, size_t);
            alg = va_arg(valist, psa_algorithm_t);
            return psa_key_agreement(generator, handle, buffer, size, alg);
        case PAL_CRYPTO_ALLOCATE_KEY:
            key_handle = (psa_key_handle_t *)va_arg(valist, int*);
            return psa_allocate_key(key_handle);
        case PAL_CRYPTO_FREE:
            for (i = 0; i < PAL_KEY_SLOT_COUNT; i++)
                psa_destroy_key(i);
            return 0;
        default:
            return PAL_STATUS_UNSUPPORTED_FUNC;
    }
#else
    return PAL_STATUS_ERROR;
#endif
}
