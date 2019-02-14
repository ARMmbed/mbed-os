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

void test_open_other_partition_key(void)
{
    static const psa_key_id_t key_id = 999;
    static const psa_key_type_t key_type = PSA_KEY_TYPE_AES;
    static const psa_key_usage_t key_usage = PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT;
    static const psa_algorithm_t key_alg = PSA_ALG_CBC_NO_PADDING;
    static const size_t key_bits = 128;
    psa_key_handle_t key_handle = 0;

    /* via test partition - create a key, set key policy, generate key material and close */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_create_persistent_key(key_id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_set_key_policy(key_handle, key_usage, key_alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_generate_key(key_handle, key_type, key_bits));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(key_handle));

    /* via test partition - reopen the key created by the test partition */
    key_handle = 0;
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_open_persistent_key(key_id, &key_handle));
    TEST_ASSERT_NOT_EQUAL(0, key_handle);

    /* via test partition - close the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_SUCCESS, test_partition_crypto_close_key(key_handle));

    /* try to open the key created by the test partition */
    TEST_ASSERT_EQUAL(PSA_ERROR_DOES_NOT_EXIST, psa_open_key(PSA_KEY_LIFETIME_PERSISTENT, key_id, &key_handle));
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
};

Specification specification(test_setup, cases);

int main(void)
{
    return !Harness::run(specification);
}
