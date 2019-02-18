/*
 * Copyright (c) 2019, Arm Limited and affiliates
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

#if ((!defined(TARGET_PSA)) || (!defined(MBEDTLS_PSA_CRYPTO_C)) || (!defined(COMPONENT_PSA_SRV_IPC)))
#error [NOT_SUPPORTED] These tests can run only on SPM-enabled targets and where Mbed Crypto is ON - skipping.
#endif

#include <stdio.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "psa/crypto.h"
#include "entropy.h"
#include "entropy_poll.h"
#include "test_partition_proxy.h"
#include "psa/lifecycle.h"

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

static psa_status_t create_and_generate_key_via_test_partition(psa_key_id_t key_id, psa_key_type_t key_type,
                                                               psa_algorithm_t key_alg, psa_key_usage_t key_usage,
                                                               size_t key_bits, psa_key_handle_t *key_handle,
                                                               unsigned char close_key)
{
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_create_persistent_key(key_id, key_handle));
    TEST_ASSERT_NOT_EQUAL(0, *key_handle);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_set_key_policy(*key_handle, key_usage, key_alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_generate_key(*key_handle, key_type, key_bits));
    if (close_key) {
        TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(*key_handle));
    }
    return (PSA_SUCCESS);
}

void test_open_other_partition_key(void)
{
    static const psa_key_id_t key_id = 999;
    static const psa_key_type_t key_type = PSA_KEY_TYPE_AES;
    static const psa_key_usage_t key_usage = PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT;
    static const psa_algorithm_t key_alg = PSA_ALG_CBC_NO_PADDING;
    static const size_t key_bits = 128;
    psa_key_handle_t key_handle = 0;

    /* via test partition - create a key, set key policy, generate key material and close */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, create_and_generate_key_via_test_partition(key_id, key_type, key_alg, key_usage,
                                                                              key_bits, &key_handle, 1));

    /* via test partition - reopen the key created by the test partition */
    key_handle = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_open_persistent_key(key_id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);

    /* via test partition - close the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(key_handle));

    /* try to open the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_ERROR_DOES_NOT_EXIST, psa_open_key(PSA_KEY_LIFETIME_PERSISTENT, key_id, &key_handle));
}

void test_create_key_same_id_different_partitions(void)
{
    static const psa_key_id_t key_id = 999;
    static const psa_key_type_t key_type = PSA_KEY_TYPE_AES;
    static const psa_key_usage_t key_usage_remote = PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT,
                                 key_usage_local = PSA_KEY_USAGE_SIGN | PSA_KEY_USAGE_VERIFY;
    static const psa_algorithm_t key_alg = PSA_ALG_CBC_NO_PADDING;
    static const size_t key_bits_remote = 128,
                        key_bits_local = 256;
    psa_key_handle_t key_handle_remote = 0,
                     key_handle_local = 0;
    psa_key_type_t got_key_type_remote = 0,
                   got_key_type_local = 0;
    size_t got_key_bits_remote = 0,
           got_key_bits_local = 0;
    psa_key_usage_t got_key_usage_remote = 0;
    psa_algorithm_t got_key_alg_remote = 0;
    psa_key_policy_t policy = PSA_KEY_POLICY_INIT;

    /* via test partition - create a key, set key policy, generate key material and close */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, create_and_generate_key_via_test_partition(key_id, key_type, key_alg,
                                                                              key_usage_remote, key_bits_remote,
                                                                              &key_handle_remote, 1));

    /* create a key, set key policy, generate key material and close from current partition (i.e. NSPE) */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_create_key(PSA_KEY_LIFETIME_PERSISTENT, key_id, &key_handle_local));
    TEST_ASSERT_NOT_EQUAL(0, key_handle_local);
    psa_key_policy_set_usage(&policy, key_usage_local, key_alg);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_set_key_policy(key_handle_local, &policy));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_generate_key(key_handle_local, key_type, key_bits_local, NULL, 0));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_close_key(key_handle_local));

    /* via test partition - reopen the key created by the test partition */
    key_handle_remote = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_open_persistent_key(key_id, &key_handle_remote));
    TEST_ASSERT_NOT_EQUAL(0, key_handle_remote);

    /* reopen the key created from the current partition (NSPE) */
    key_handle_local = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_open_key(PSA_KEY_LIFETIME_PERSISTENT, key_id, &key_handle_local));
    TEST_ASSERT_NOT_EQUAL(0, key_handle_local);

    /* via test partition - get key info for the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_get_key_information(key_handle_remote,
                                                                             &got_key_type_remote,
                                                                             &got_key_bits_remote));
    TEST_ASSERT_EQUAL(key_type, got_key_type_remote);
    TEST_ASSERT_EQUAL(key_bits_remote, got_key_bits_remote);

    /* via test partition - get key policy for key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_get_key_policy(key_handle_remote,
                                                                        &got_key_usage_remote,
                                                                        &got_key_alg_remote));
    TEST_ASSERT_EQUAL(key_usage_remote, got_key_usage_remote);
    TEST_ASSERT_EQUAL(key_alg, got_key_alg_remote);

    /* get key info for key created by the current partition (NSPE) */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_get_key_information(key_handle_local, &got_key_type_local, &got_key_bits_local));
    TEST_ASSERT_EQUAL(key_type, got_key_type_local);
    TEST_ASSERT_EQUAL(key_bits_local, got_key_bits_local);

    /* get key policy for key created by the current partition (NSPE) */
    policy = psa_key_policy_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_get_key_policy(key_handle_local, &policy));
    TEST_ASSERT_EQUAL(key_usage_local, policy.usage);
    TEST_ASSERT_EQUAL(key_alg, policy.alg);

    /* via test partition - close the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(key_handle_remote));

    /* close the key created by the current partition (NSPE) */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_close_key(key_handle_local));
}

void test_use_other_partition_key_manage_key(void)
{
    static const psa_key_id_t key_id = 999;
    static const psa_key_type_t key_type = PSA_KEY_TYPE_AES;
    static const psa_algorithm_t key_alg = PSA_ALG_CBC_NO_PADDING;
    static const psa_key_usage_t key_usage = PSA_KEY_USAGE_EXPORT;
    static const size_t key_bits = 128;
    static const unsigned char key_data[] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    psa_key_handle_t key_handle = 0;
    psa_key_policy_t policy = PSA_KEY_POLICY_INIT;
    unsigned char output[sizeof(key_data)] = { 0 };
    size_t len, got_key_bits;
    psa_key_type_t got_key_type;
    psa_key_lifetime_t got_lifetime;

    /* via test partition - create a key without generating any key material */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_create_persistent_key(key_id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);

    /* try to set the key policy for the key that was created by the test partition */
    psa_key_policy_set_usage(&policy, key_usage, key_alg);
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_set_key_policy(key_handle, &policy));

    /* via test partition - set key policy */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_set_key_policy(key_handle, key_usage, key_alg));

    /* try to generate key data for the key that was created by the test partition */
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_generate_key(key_handle, key_type, key_bits, NULL, 0));

    /* via test partition - generate key material and close the key */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_generate_key(key_handle, key_type, key_bits));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(key_handle));

    /* via test partition - reopen the key created by the test partition and keep it open */
    key_handle = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_open_persistent_key(key_id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);

    /* try to work with the handle created for a key created by the test partition */
    got_key_type = 0;
    got_key_bits = 0;
    got_lifetime = 0;
    policy = psa_key_policy_init();
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_get_key_policy(key_handle, &policy));
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_get_key_lifetime(key_handle, &got_lifetime));
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_close_key(key_handle));
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_destroy_key(key_handle));
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_get_key_information(key_handle, &got_key_type, &got_key_bits));
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_export_key(key_handle, output, sizeof(output), &len));
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_export_public_key(key_handle, output, sizeof(output), &len));

    /* via test partition - destroy the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_destroy_key(key_handle));

    /* via test partition - create a key, set key policy but no key material */
    key_handle = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_create_persistent_key(key_id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_set_key_policy(key_handle, key_usage, key_alg));

    /* try to import key data into the key that was created by the test partition */
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_import_key(key_handle, key_type,
                                                               key_data, sizeof(key_data)));

    /* via test partition - import key data for the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_import_key(key_handle, key_type, key_data, sizeof(key_data)));

    /* via test partition - close the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(key_handle));
}

void test_use_other_partition_key_mac(void)
{
    static const psa_key_id_t key_id = 999;
    static const psa_key_type_t key_type = PSA_KEY_TYPE_AES;
    static const psa_algorithm_t key_alg = PSA_ALG_CBC_NO_PADDING;
    static const psa_key_usage_t key_usage = PSA_KEY_USAGE_SIGN | PSA_KEY_USAGE_VERIFY;
    static const size_t key_bits = 128;
    psa_key_handle_t key_handle = 0;
    psa_mac_operation_t operation;

    /* via test partition - create a key, set key policy, generate key material and close */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, create_and_generate_key_via_test_partition(key_id, key_type, key_alg, key_usage,
                                                                              key_bits, &key_handle, 1));

    /* via test partition - reopen the key created by the test partition */
    key_handle = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_open_persistent_key(key_id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);

    /* try to setup mac sign operation using the key that was created by the test partition */
    operation = psa_mac_operation_init();
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_mac_sign_setup(&operation, key_handle, key_alg));

    /* try to setup mac verify operation using the key that was created by the test partition */
    operation = psa_mac_operation_init();
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_mac_verify_setup(&operation, key_handle, key_alg));

    /* via test partition - close the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(key_handle));
}

void test_use_other_partition_key_cipher(void)
{
    static const psa_key_id_t key_id = 999;
    static const psa_key_type_t key_type = PSA_KEY_TYPE_AES;
    static const psa_algorithm_t key_alg = PSA_ALG_CBC_NO_PADDING;
    static const psa_key_usage_t key_usage = PSA_KEY_USAGE_SIGN | PSA_KEY_USAGE_VERIFY;
    static const size_t key_bits = 128;
    psa_key_handle_t key_handle = 0;
    psa_cipher_operation_t operation;

    /* via test partition - create a key, set key policy, generate key material and close */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, create_and_generate_key_via_test_partition(key_id, key_type, key_alg, key_usage,
                                                                              key_bits, &key_handle, 1));

    /* via test partition - reopen the key created by the test partition */
    key_handle = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_open_persistent_key(key_id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);

    /* try to setup cipher encrypt sign operation using the key that was created by the test partition */
    operation = psa_cipher_operation_init();
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_cipher_encrypt_setup(&operation, key_handle, key_alg));

    /* try to setup cipher decrypt operation using the key that was created by the test partition */
    operation = psa_cipher_operation_init();
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_cipher_decrypt_setup(&operation, key_handle, key_alg));

    /* via test partition - close the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(key_handle));
}

void test_use_other_partition_key_aead(void)
{
    static const psa_key_id_t key_id = 999;
    static const psa_key_type_t key_type = PSA_KEY_TYPE_AES;
    static const psa_algorithm_t key_alg = PSA_ALG_GCM;
    static const psa_key_usage_t key_usage = PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT;
    static const size_t key_bits = 128;
    static const unsigned char nonce[16] = { 0 };
    unsigned char plain_text[] = "encrypt me!";
    unsigned char cipher_text[PSA_AEAD_ENCRYPT_OUTPUT_SIZE(key_alg, sizeof(plain_text))] = { 0 };
    psa_key_handle_t key_handle = 0;
    size_t len;

    /* via test partition - create a key, set key policy, generate key material and close */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, create_and_generate_key_via_test_partition(key_id, key_type, key_alg, key_usage,
                                                                              key_bits, &key_handle, 1));

    /* via test partition - reopen the key created by the test partition */
    key_handle = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_open_persistent_key(key_id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);

    /* try to aead encrypt using the key that was created by the test partition */
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_aead_encrypt(key_handle, key_alg, nonce, sizeof(nonce), NULL, 0,
                                                                 plain_text, sizeof(plain_text),
                                                                 cipher_text, sizeof(cipher_text), &len));

    /* try to aead decrypt using the key that was created by the test partition */
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_aead_decrypt(key_handle, key_alg, nonce, sizeof(nonce), NULL, 0,
                                                                 cipher_text, sizeof(cipher_text),
                                                                 plain_text, sizeof(plain_text), &len));

    /* via test partition - close the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(key_handle));
}

void test_use_other_partition_key_asymmetric_sign_verify(void)
{
    static const psa_key_id_t key_id = 999;
    static const psa_key_type_t key_type = PSA_KEY_TYPE_ECC_KEYPAIR(PSA_ECC_CURVE_SECP256R1);
    static const psa_algorithm_t key_alg = PSA_ALG_ECDSA(PSA_ALG_SHA_256);
    static const psa_key_usage_t key_usage = PSA_KEY_USAGE_SIGN | PSA_KEY_USAGE_VERIFY;
    static const size_t key_bits = 256;
    static const unsigned char input[] = "hello world!";
    unsigned char signature[PSA_ECDSA_SIGNATURE_SIZE(key_bits)] = { 0 };
    psa_key_handle_t key_handle = 0;
    size_t len;

    /* via test partition - create a key, set key policy, generate key material and close */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, create_and_generate_key_via_test_partition(key_id, key_type, key_alg, key_usage,
                                                                              key_bits, &key_handle, 1));

    /* via test partition - reopen the key created by the test partition */
    key_handle = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_open_persistent_key(key_id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);

    /* try to asymmetric sign using the key that was created by the test partition */
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_asymmetric_sign(key_handle, key_alg, input, sizeof(input),
                                                                    signature, sizeof(signature), &len));

    /* try to asymmetric verify using the key that was created by the test partition */
    TEST_ASSERT_EQUAL(PSA_ERROR_INVALID_HANDLE, psa_asymmetric_verify(key_handle, key_alg, input, sizeof(input),
                                                                      signature, len));

    /* via test partition - close the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(key_handle));
}

utest::v1::status_t case_setup_handler(const Case *const source, const size_t index_of_case)
{
    psa_status_t status = mbed_psa_reboot_and_request_new_security_state(PSA_LIFECYCLE_ASSEMBLY_AND_TEST);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    status = psa_crypto_init();
#if defined(MBEDTLS_ENTROPY_NV_SEED) || defined(COMPONENT_PSA_SRV_IPC)
    if (status == PSA_ERROR_INSUFFICIENT_ENTROPY) {
        inject_entropy();
        status = psa_crypto_init();
    }
#endif
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t case_teardown_handler(const Case *const source, const size_t passed,
                                          const size_t failed, const failure_t failure)
{
    psa_status_t status = mbed_psa_reboot_and_request_new_security_state(PSA_LIFECYCLE_ASSEMBLY_AND_TEST);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    mbedtls_psa_crypto_free();
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
#ifndef NO_GREENTEA
    GREENTEA_SETUP(120, "default_auto");
#endif
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("open other partitions' key",
         case_setup_handler, test_open_other_partition_key, case_teardown_handler),
    Case("create key with same id different partitions",
         case_setup_handler, test_create_key_same_id_different_partitions, case_teardown_handler),
    Case("use other partitions' key - key manage",
         case_setup_handler, test_use_other_partition_key_manage_key, case_teardown_handler),
    Case("use other partitions' key - mac",
         case_setup_handler, test_use_other_partition_key_mac, case_teardown_handler),
    Case("use other partitions' key - cipher",
         case_setup_handler, test_use_other_partition_key_cipher, case_teardown_handler),
    Case("use other partitions' key - aead",
         case_setup_handler, test_use_other_partition_key_aead, case_teardown_handler),
    Case("use other partitions' key - asymmetric sign verify",
         case_setup_handler, test_use_other_partition_key_asymmetric_sign_verify, case_teardown_handler),
};

Specification specification(test_setup, cases);

int main(void)
{
    return !Harness::run(specification);
}
