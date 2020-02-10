/*
 * Copyright (c) 2018, Arm Limited and affiliates
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

#include "psa/crypto.h"

#if ((!defined(TARGET_PSA)) || (!defined(MBEDTLS_PSA_CRYPTO_C)))
#error [NOT_SUPPORTED] Mbed Crypto is OFF - skipping.
#else

#include <stdio.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "entropy.h"
#include "entropy_poll.h"

using namespace utest::v1;

#if defined(MBEDTLS_ENTROPY_NV_SEED) || defined(COMPONENT_PSA_SRV_IPC)

#if !defined(MAX)
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#define MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE \
            MAX(MBEDTLS_ENTROPY_MIN_PLATFORM, MBEDTLS_ENTROPY_BLOCK_SIZE)

void inject_entropy()
{
    uint8_t seed[MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE] = { 0 };
    for (int i = 0; i < MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE; ++i) {
        seed[i] = i;
    }
    mbedtls_psa_inject_entropy(seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE);
}
#endif // defined(MBEDTLS_ENTROPY_NV_SEED) || defined(COMPONENT_PSA_SRV_IPC)

void test_crypto_random(void)
{
    static const unsigned char trail[] = "don't overwrite me";
    unsigned char changed[256] = { 0 };
    unsigned char output[sizeof(changed) + sizeof(trail)];
    size_t i, bytes = sizeof(changed);
    unsigned int run;

    memcpy(output + bytes, trail, sizeof(trail));
    /* Run several times, to ensure that every output byte will be
     * nonzero at least once with overwhelming probability
     * (2^(-8*number_of_runs)). */
    for (run = 0; run < 10; run++) {
        memset(output, 0, bytes);
        TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_generate_random(output, bytes));
        /* Check that no more than 'bytes' have been overwritten */
        TEST_ASSERT_EQUAL_UINT8_ARRAY(trail, output + bytes, sizeof(trail));

        for (i = 0; i < bytes; i++) {
            if (0 != output[i]) {
                ++changed[i];
            }
        }
    }

    /* Check that every byte was changed to nonzero at least once. This
     * validates that psa_generate_random is overwriting every byte of
     * the output buffer. */
    for (i = 0; i < bytes; i++) {
        TEST_ASSERT_NOT_EQUAL(0, changed[i]);
    }
}

void test_crypto_asymmetric_encrypt_decrypt(void)
{
    psa_status_t status = PSA_SUCCESS;
    psa_key_handle_t key_handle;
    psa_key_type_t key_type = PSA_KEY_TYPE_RSA_KEY_PAIR;
    psa_algorithm_t alg = PSA_ALG_RSA_PKCS1V15_CRYPT;
    size_t key_bits = 512, output_length;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    static const unsigned char input[] = "encrypt me!";
    unsigned char encrypted[64];
    unsigned char decrypted[sizeof(input)];

    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_ENCRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_bits(&attributes, key_bits);
    status = psa_generate_key(&attributes, &key_handle);
    TEST_SKIP_UNLESS_MESSAGE(status != PSA_ERROR_NOT_SUPPORTED, "RSA key generation is not supported");
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_get_key_attributes(key_handle, &attributes));
    TEST_ASSERT_EQUAL(key_bits, psa_get_key_bits(&attributes));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_asymmetric_encrypt(key_handle, alg, input, sizeof(input), NULL, 0,
                                                          encrypted, sizeof(encrypted), &output_length));
    TEST_ASSERT_EQUAL(sizeof(encrypted), output_length);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_asymmetric_decrypt(key_handle, alg, encrypted, sizeof(encrypted), NULL, 0,
                                                          decrypted, sizeof(decrypted), &output_length));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_destroy_key(key_handle));
    TEST_ASSERT_EQUAL(sizeof(input), output_length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(input, decrypted, output_length);
}

void test_crypto_hash_verify(void)
{
    psa_algorithm_t alg = PSA_ALG_SHA_256;
    psa_hash_operation_t operation;
    /* SHA-256 hash of an empty string */
    static const unsigned char hash[] = {
        0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8,
        0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
        0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55
    };

    operation = psa_hash_operation_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_setup(&operation, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_verify(&operation, hash, sizeof(hash)));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&operation));
}

void test_crypto_symmetric_cipher_encrypt_decrypt(void)
{
    psa_key_handle_t key_handle;
    psa_key_type_t key_type = PSA_KEY_TYPE_AES;
    psa_algorithm_t alg = PSA_ALG_CBC_NO_PADDING;
    psa_cipher_operation_t operation;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    size_t output_len;
    static const unsigned char key[] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    static const unsigned char input[] = {
        0xb0, 0x28, 0x9f, 0x04, 0xdc, 0x7f, 0xe2, 0x25,
        0xa2, 0xce, 0xe9, 0xd3, 0xb9, 0xbc, 0xc7, 0x2f
    };
    static const unsigned char expected_encryption[] = {
        0x28, 0x8d, 0x76, 0xc0, 0xa7, 0x09, 0x50, 0x3f,
        0x87, 0x96, 0x1e, 0x96, 0x05, 0xcb, 0xb9, 0x6d
    };
    unsigned char encrypted[sizeof(input)], decrypted[sizeof(input)], iv[16];

    memset(iv, 0x2a, sizeof(iv));
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_bits(&attributes, 128);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_import_key(&attributes, key, sizeof(key), &key_handle));

    operation = psa_cipher_operation_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_encrypt_setup(&operation, key_handle, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_set_iv(&operation, iv, sizeof(iv)));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_update(&operation, input, sizeof(input),
                                                     encrypted, sizeof(encrypted), &output_len));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_finish(&operation, encrypted + output_len,
                                                     sizeof(encrypted) - output_len, &output_len));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_abort(&operation));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_encryption, encrypted, sizeof(expected_encryption));

    operation = psa_cipher_operation_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_decrypt_setup(&operation, key_handle, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_set_iv(&operation, iv, sizeof(iv)));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_update(&operation, encrypted, sizeof(encrypted),
                                                     decrypted, sizeof(decrypted), &output_len));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_finish(&operation, decrypted + output_len,
                                                     sizeof(decrypted) - output_len, &output_len));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_abort(&operation));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_destroy_key(key_handle));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(input, decrypted, sizeof(input));
}

void test_crypto_asymmetric_sign_verify(void)
{
    psa_key_handle_t key_handle;
    psa_key_type_t key_type = PSA_KEY_TYPE_RSA_KEY_PAIR;
    psa_algorithm_t alg = PSA_ALG_RSA_PKCS1V15_SIGN_RAW;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    static const unsigned char key[] = {
        0x30, 0x82, 0x02, 0x5e, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xaf,
        0x05, 0x7d, 0x39, 0x6e, 0xe8, 0x4f, 0xb7, 0x5f, 0xdb, 0xb5, 0xc2, 0xb1,
        0x3c, 0x7f, 0xe5, 0xa6, 0x54, 0xaa, 0x8a, 0xa2, 0x47, 0x0b, 0x54, 0x1e,
        0xe1, 0xfe, 0xb0, 0xb1, 0x2d, 0x25, 0xc7, 0x97, 0x11, 0x53, 0x12, 0x49,
        0xe1, 0x12, 0x96, 0x28, 0x04, 0x2d, 0xbb, 0xb6, 0xc1, 0x20, 0xd1, 0x44,
        0x35, 0x24, 0xef, 0x4c, 0x0e, 0x6e, 0x1d, 0x89, 0x56, 0xee, 0xb2, 0x07,
        0x7a, 0xf1, 0x23, 0x49, 0xdd, 0xee, 0xe5, 0x44, 0x83, 0xbc, 0x06, 0xc2,
        0xc6, 0x19, 0x48, 0xcd, 0x02, 0xb2, 0x02, 0xe7, 0x96, 0xae, 0xbd, 0x94,
        0xd3, 0xa7, 0xcb, 0xf8, 0x59, 0xc2, 0xc1, 0x81, 0x9c, 0x32, 0x4c, 0xb8,
        0x2b, 0x9c, 0xd3, 0x4e, 0xde, 0x26, 0x3a, 0x2a, 0xbf, 0xfe, 0x47, 0x33,
        0xf0, 0x77, 0x86, 0x9e, 0x86, 0x60, 0xf7, 0xd6, 0x83, 0x4d, 0xa5, 0x3d,
        0x69, 0x0e, 0xf7, 0x98, 0x5f, 0x6b, 0xc3, 0x02, 0x03, 0x01, 0x00, 0x01,
        0x02, 0x81, 0x81, 0x00, 0x87, 0x4b, 0xf0, 0xff, 0xc2, 0xf2, 0xa7, 0x1d,
        0x14, 0x67, 0x1d, 0xdd, 0x01, 0x71, 0xc9, 0x54, 0xd7, 0xfd, 0xbf, 0x50,
        0x28, 0x1e, 0x4f, 0x6d, 0x99, 0xea, 0x0e, 0x1e, 0xbc, 0xf8, 0x2f, 0xaa,
        0x58, 0xe7, 0xb5, 0x95, 0xff, 0xb2, 0x93, 0xd1, 0xab, 0xe1, 0x7f, 0x11,
        0x0b, 0x37, 0xc4, 0x8c, 0xc0, 0xf3, 0x6c, 0x37, 0xe8, 0x4d, 0x87, 0x66,
        0x21, 0xd3, 0x27, 0xf6, 0x4b, 0xbe, 0x08, 0x45, 0x7d, 0x3e, 0xc4, 0x09,
        0x8b, 0xa2, 0xfa, 0x0a, 0x31, 0x9f, 0xba, 0x41, 0x1c, 0x28, 0x41, 0xed,
        0x7b, 0xe8, 0x31, 0x96, 0xa8, 0xcd, 0xf9, 0xda, 0xa5, 0xd0, 0x06, 0x94,
        0xbc, 0x33, 0x5f, 0xc4, 0xc3, 0x22, 0x17, 0xfe, 0x04, 0x88, 0xbc, 0xe9,
        0xcb, 0x72, 0x02, 0xe5, 0x94, 0x68, 0xb1, 0xea, 0xd1, 0x19, 0x00, 0x04,
        0x77, 0xdb, 0x2c, 0xa7, 0x97, 0xfa, 0xc1, 0x9e, 0xda, 0x3f, 0x58, 0xc1,
        0x02, 0x41, 0x00, 0xe2, 0xab, 0x76, 0x08, 0x41, 0xbb, 0x9d, 0x30, 0xa8,
        0x1d, 0x22, 0x2d, 0xe1, 0xeb, 0x73, 0x81, 0xd8, 0x22, 0x14, 0x40, 0x7f,
        0x1b, 0x97, 0x5c, 0xbb, 0xfe, 0x4e, 0x1a, 0x94, 0x67, 0xfd, 0x98, 0xad,
        0xbd, 0x78, 0xf6, 0x07, 0x83, 0x6c, 0xa5, 0xbe, 0x19, 0x28, 0xb9, 0xd1,
        0x60, 0xd9, 0x7f, 0xd4, 0x5c, 0x12, 0xd6, 0xb5, 0x2e, 0x2c, 0x98, 0x71,
        0xa1, 0x74, 0xc6, 0x6b, 0x48, 0x81, 0x13, 0x02, 0x41, 0x00, 0xc5, 0xab,
        0x27, 0x60, 0x21, 0x59, 0xae, 0x7d, 0x6f, 0x20, 0xc3, 0xc2, 0xee, 0x85,
        0x1e, 0x46, 0xdc, 0x11, 0x2e, 0x68, 0x9e, 0x28, 0xd5, 0xfc, 0xbb, 0xf9,
        0x90, 0xa9, 0x9e, 0xf8, 0xa9, 0x0b, 0x8b, 0xb4, 0x4f, 0xd3, 0x64, 0x67,
        0xe7, 0xfc, 0x17, 0x89, 0xce, 0xb6, 0x63, 0xab, 0xda, 0x33, 0x86, 0x52,
        0xc3, 0xc7, 0x3f, 0x11, 0x17, 0x74, 0x90, 0x2e, 0x84, 0x05, 0x65, 0x92,
        0x70, 0x91, 0x02, 0x41, 0x00, 0xb6, 0xcd, 0xbd, 0x35, 0x4f, 0x7d, 0xf5,
        0x79, 0xa6, 0x3b, 0x48, 0xb3, 0x64, 0x3e, 0x35, 0x3b, 0x84, 0x89, 0x87,
        0x77, 0xb4, 0x8b, 0x15, 0xf9, 0x4e, 0x0b, 0xfc, 0x05, 0x67, 0xa6, 0xae,
        0x59, 0x11, 0xd5, 0x7a, 0xd6, 0x40, 0x9c, 0xf7, 0x64, 0x7b, 0xf9, 0x62,
        0x64, 0xe9, 0xbd, 0x87, 0xeb, 0x95, 0xe2, 0x63, 0xb7, 0x11, 0x0b, 0x9a,
        0x1f, 0x9f, 0x94, 0xac, 0xce, 0xd0, 0xfa, 0xfa, 0x4d, 0x02, 0x40, 0x71,
        0x19, 0x5e, 0xec, 0x37, 0xe8, 0xd2, 0x57, 0xde, 0xcf, 0xc6, 0x72, 0xb0,
        0x7a, 0xe6, 0x39, 0xf1, 0x0c, 0xbb, 0x9b, 0x0c, 0x73, 0x9d, 0x0c, 0x80,
        0x99, 0x68, 0xd6, 0x44, 0xa9, 0x4e, 0x3f, 0xd6, 0xed, 0x92, 0x87, 0x07,
        0x7a, 0x14, 0x58, 0x3f, 0x37, 0x90, 0x58, 0xf7, 0x6a, 0x8a, 0xec, 0xd4,
        0x3c, 0x62, 0xdc, 0x8c, 0x0f, 0x41, 0x76, 0x66, 0x50, 0xd7, 0x25, 0x27,
        0x5a, 0xc4, 0xa1, 0x02, 0x41, 0x00, 0xbb, 0x32, 0xd1, 0x33, 0xed, 0xc2,
        0xe0, 0x48, 0xd4, 0x63, 0x38, 0x8b, 0x7b, 0xe9, 0xcb, 0x4b, 0xe2, 0x9f,
        0x4b, 0x62, 0x50, 0xbe, 0x60, 0x3e, 0x70, 0xe3, 0x64, 0x75, 0x01, 0xc9,
        0x7d, 0xdd, 0xe2, 0x0a, 0x4e, 0x71, 0xbe, 0x95, 0xfd, 0x5e, 0x71, 0x78,
        0x4e, 0x25, 0xac, 0xa4, 0xba, 0xf2, 0x5b, 0xe5, 0x73, 0x8a, 0xae, 0x59,
        0xbb, 0xfe, 0x1c, 0x99, 0x77, 0x81, 0x44, 0x7a, 0x2b, 0x24
    };
    static const unsigned char input[] = { 0x61, 0x62, 0x63 };
    static const unsigned char expected_signature[] = {
        0x2c, 0x77, 0x44, 0x98, 0x3f, 0x02, 0x3a, 0xc7, 0xbb, 0x1c, 0x55, 0x52,
        0x9d, 0x83, 0xed, 0x11, 0xa7, 0x6a, 0x78, 0x98, 0xa1, 0xbb, 0x5c, 0xe1,
        0x91, 0x37, 0x5a, 0x4a, 0xa7, 0x49, 0x5a, 0x63, 0x3d, 0x27, 0x87, 0x9f,
        0xf5, 0x8e, 0xba, 0x5a, 0x57, 0x37, 0x1c, 0x34, 0xfe, 0xb1, 0x18, 0x0e,
        0x8b, 0x85, 0x0d, 0x55, 0x24, 0x76, 0xeb, 0xb5, 0x63, 0x4d, 0xf6, 0x20,
        0x26, 0x19, 0x92, 0xf1, 0x2e, 0xbe, 0xe9, 0x09, 0x70, 0x41, 0xdb, 0xbe,
        0xa8, 0x5a, 0x42, 0xd4, 0x5b, 0x34, 0x4b, 0xe5, 0x07, 0x3c, 0xeb, 0x77,
        0x2f, 0xfc, 0x60, 0x49, 0x54, 0xb9, 0x15, 0x8b, 0xa8, 0x1e, 0xc3, 0xdc,
        0x4d, 0x9d, 0x65, 0xe3, 0xab, 0x7a, 0xa3, 0x18, 0x16, 0x5f, 0x38, 0xc3,
        0x6f, 0x84, 0x1f, 0x1c, 0x69, 0xcb, 0x1c, 0xfa, 0x49, 0x4a, 0xa5, 0xcb,
        0xb4, 0xd6, 0xc0, 0xef, 0xba, 0xfb, 0x04, 0x3a
    };
    unsigned char signature[sizeof(expected_signature)];
    size_t signature_len;

    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN | PSA_KEY_USAGE_VERIFY);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, key_type);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_import_key(&attributes, key, sizeof(key), &key_handle));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_sign_hash(key_handle, alg, input, sizeof(input),
                                                 signature, sizeof(signature), &signature_len));
    TEST_ASSERT_EQUAL(sizeof(signature), signature_len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_signature, signature, signature_len);

    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_verify_hash(key_handle, alg, input, sizeof(input),
                                                   signature, signature_len));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_destroy_key(key_handle));
}

void test_crypto_key_derivation(void)
{
    psa_key_handle_t key_handle, derived_key_handle;
    psa_algorithm_t alg = PSA_ALG_HKDF(PSA_ALG_SHA_256), derived_alg = PSA_ALG_CTR;
    psa_key_type_t key_type = PSA_KEY_TYPE_DERIVE, derived_key_type = PSA_KEY_TYPE_AES;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_derivation_operation_t operation =
        PSA_KEY_DERIVATION_OPERATION_INIT;
    size_t key_bits = 512, derived_key_bits = 256;

    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_bits(&attributes, key_bits);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_generate_key(&attributes, &key_handle));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_key_derivation_setup(&operation, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS,
                      psa_key_derivation_set_capacity(&operation,
                                                      PSA_BITS_TO_BYTES(derived_key_bits)));
    TEST_ASSERT_EQUAL(PSA_SUCCESS,
                      psa_key_derivation_input_bytes(&operation,
                                                     PSA_KEY_DERIVATION_INPUT_SALT, NULL, 0));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_key_derivation_input_key(&operation,
                                                                PSA_KEY_DERIVATION_INPUT_SECRET,
                                                                key_handle));
    TEST_ASSERT_EQUAL(PSA_SUCCESS,
                      psa_key_derivation_input_bytes(&operation,
                                                     PSA_KEY_DERIVATION_INPUT_INFO, NULL, 0));

    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT);
    psa_set_key_algorithm(&attributes, derived_alg);
    psa_set_key_type(&attributes, derived_key_type);
    psa_set_key_bits(&attributes, derived_key_bits);
    TEST_ASSERT_EQUAL(PSA_SUCCESS,
                      psa_key_derivation_output_key(&attributes, &operation, &derived_key_handle));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_get_key_attributes(derived_key_handle, &attributes));
    TEST_ASSERT_EQUAL(derived_key_type, psa_get_key_type(&attributes));
    TEST_ASSERT_EQUAL(derived_key_bits, psa_get_key_bits(&attributes));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_key_derivation_abort(&operation));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_destroy_key(key_handle));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_destroy_key(derived_key_handle));
}

void test_crypto_key_handles(void)
{
    psa_key_id_t id = 999;
    psa_key_type_t type = PSA_KEY_TYPE_AES;
    size_t bits = 256;
    psa_key_usage_t usage = PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT;
    psa_algorithm_t alg = PSA_ALG_CBC_NO_PADDING;
    psa_key_handle_t key_handle;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;

    psa_set_key_usage_flags(&attributes, usage);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, type);
    psa_set_key_bits(&attributes, bits);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_generate_key(&attributes, &key_handle));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_close_key(key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);

    key_handle = 0;
    attributes = psa_key_attributes_init();
    psa_set_key_usage_flags(&attributes, usage);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, type);
    psa_set_key_bits(&attributes, bits);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_generate_key(&attributes, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_destroy_key(key_handle));

    key_handle = 0;
    attributes = psa_key_attributes_init();
    psa_set_key_usage_flags(&attributes, usage);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, type);
    psa_set_key_bits(&attributes, bits);
    psa_set_key_lifetime(&attributes, PSA_KEY_LIFETIME_PERSISTENT);
    psa_set_key_id(&attributes, id);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_generate_key(&attributes, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_close_key(key_handle));

    key_handle = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_open_key(id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_destroy_key(key_handle));

    key_handle = 0;
    TEST_ASSERT_EQUAL(PSA_ERROR_DOES_NOT_EXIST, psa_open_key(id, &key_handle));
}

void test_crypto_hash_clone(void)
{
    psa_algorithm_t alg = PSA_ALG_SHA_256;
    unsigned char hash[PSA_HASH_MAX_SIZE];
    size_t hash_len;
    psa_hash_operation_t source;
    psa_hash_operation_t target;
    /* SHA-256 hash of an empty string */
    static const unsigned char expected_hash[] = {
        0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8,
        0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
        0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55
    };

    source = psa_hash_operation_init();
    target = psa_hash_operation_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_setup(&source, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_finish(&source, hash, sizeof(hash), &hash_len));
    /* should fail because psa_hash_finish has been called on source */
    TEST_ASSERT_EQUAL(PSA_ERROR_BAD_STATE, psa_hash_clone(&source, &target));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&source));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&target));

    source = psa_hash_operation_init();
    target = psa_hash_operation_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_setup(&source, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_verify(&source, expected_hash, sizeof(expected_hash)));
    /* should fail because psa_hash_verify has been called on source */
    TEST_ASSERT_EQUAL(PSA_ERROR_BAD_STATE, psa_hash_clone(&source, &target));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&source));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&target));

    source = psa_hash_operation_init();
    target = psa_hash_operation_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_setup(&source, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&source));
    /* should fail because psa_hash_abort has been called on source */
    TEST_ASSERT_EQUAL(PSA_ERROR_BAD_STATE, psa_hash_clone(&source, &target));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&target));

    source = psa_hash_operation_init();
    target = psa_hash_operation_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_setup(&source, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_setup(&target, alg));
    /* should fail because psa_hash_setup has been called on target */
    TEST_ASSERT_EQUAL(PSA_ERROR_BAD_STATE, psa_hash_clone(&source, &target));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&source));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&target));

    source = psa_hash_operation_init();
    target = psa_hash_operation_init();
    /* should fail because psa_hash_setup has not been called on source */
    TEST_ASSERT_EQUAL(PSA_ERROR_BAD_STATE, psa_hash_clone(&source, &target));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&source));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&target));

    source = psa_hash_operation_init();
    target = psa_hash_operation_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_setup(&source, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_clone(&source, &target));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&source));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&target));
}

utest::v1::status_t case_setup_handler(const Case *const source, const size_t index_of_case)
{
    psa_status_t status = psa_crypto_init();
#if defined(MBEDTLS_ENTROPY_NV_SEED) || defined(COMPONENT_PSA_SRV_IPC)
    if (status == PSA_ERROR_INSUFFICIENT_ENTROPY) {
        inject_entropy();
        status = psa_crypto_init();
    }
#endif /* defined(MBEDTLS_ENTROPY_NV_SEED) || defined(COMPONENT_PSA_SRV_IPC) */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    mbedtls_psa_crypto_free();
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("mbed-crypto random", case_setup_handler, test_crypto_random, case_teardown_handler),
    Case("mbed-crypto asymmetric encrypt/decrypt", case_setup_handler, test_crypto_asymmetric_encrypt_decrypt, case_teardown_handler),
    Case("mbed-crypto hash verify", case_setup_handler, test_crypto_hash_verify, case_teardown_handler),
    Case("mbed-crypto symmetric cipher encrypt/decrypt", case_setup_handler, test_crypto_symmetric_cipher_encrypt_decrypt, case_teardown_handler),
    Case("mbed-crypto asymmetric sign/verify", case_setup_handler, test_crypto_asymmetric_sign_verify, case_teardown_handler),
    Case("mbed-crypto key derivation", case_setup_handler, test_crypto_key_derivation, case_teardown_handler),
    Case("mbed-crypto key handles", case_setup_handler, test_crypto_key_handles, case_teardown_handler),
    Case("mbed-crypto hash clone", case_setup_handler, test_crypto_hash_clone, case_teardown_handler),
};

Specification specification(test_setup, cases);

int main(void)
{
    return !Harness::run(specification);
}
#endif // ((!defined(TARGET_PSA)) || (!defined(MBEDTLS_PSA_CRYPTO_C)))
