/*
* Copyright (c) 2018 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "psa/crypto.h"

#if ((!defined(TARGET_PSA)) || (!defined(MBEDTLS_PSA_CRYPTO_C)))
#error [NOT_SUPPORTED] Mbed Crypto is OFF - skipping.
#else

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "entropy.h"
#include "entropy_poll.h"

#define TEST_RANDOM_SIZE 64

#if !defined(MAX)
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

/* Calculating the minimum allowed entropy size in bytes */
#define MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE \
            MAX(MBEDTLS_ENTROPY_MIN_PLATFORM, MBEDTLS_ENTROPY_BLOCK_SIZE)

using namespace utest::v1;

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

static void check_multi_crypto_init_deinit()
{
#if !defined(COMPONENT_PSA_SRV_IPC)
    TEST_SKIP();
#endif
    uint8_t output[TEST_RANDOM_SIZE] = {0};

    psa_status_t status = psa_crypto_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    status = psa_crypto_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    status = psa_generate_random(output, sizeof(output));
    TEST_ASSERT_NOT_EQUAL(PSA_ERROR_BAD_STATE, status);

    mbedtls_psa_crypto_free();
    status = psa_generate_random(output, sizeof(output));
    TEST_ASSERT_NOT_EQUAL(PSA_ERROR_BAD_STATE, status);

    mbedtls_psa_crypto_free();
    status = psa_generate_random(output, sizeof(output));
    TEST_ASSERT_EQUAL(PSA_ERROR_BAD_STATE, status);
}

static void check_crypto_init_deinit()
{
    psa_status_t status;
    uint8_t output[TEST_RANDOM_SIZE] = {0};

    // Should fail as init is required first
    status = psa_generate_random(output, sizeof(output));
    TEST_ASSERT_EQUAL(PSA_ERROR_BAD_STATE, status);

    status = psa_crypto_init();
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    status = psa_generate_random(output, sizeof(output));
    TEST_ASSERT_NOT_EQUAL(PSA_ERROR_BAD_STATE, status);

    mbedtls_psa_crypto_free();
    status = psa_generate_random(output, sizeof(output));
    TEST_ASSERT_EQUAL(PSA_ERROR_BAD_STATE, status);
}

Case cases[] = {
    Case("PSA crypto-init De-init", check_crypto_init_deinit),
    Case("PSA crypto- multiple init De-init", check_multi_crypto_init_deinit),
};

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
#if (defined(COMPONENT_PSA_SRV_IPC) || defined(MBEDTLS_ENTROPY_NV_SEED))
    uint8_t seed[MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE] = {0};
    /* inject some a seed for test*/
    for (int i = 0; i < MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE; ++i) {
        seed[i] = i;
    }

    /* don't really care if this succeed this is just to make crypto init pass*/
    mbedtls_psa_inject_entropy(seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE);
#endif
    return !Harness::run(specification);
}

#endif // ((!defined(TARGET_PSA)) || (!defined(MBEDTLS_PSA_CRYPTO_C)))
