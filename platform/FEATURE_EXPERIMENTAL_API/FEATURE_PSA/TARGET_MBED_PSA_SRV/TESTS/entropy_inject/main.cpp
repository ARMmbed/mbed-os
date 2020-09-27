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

#if ((!defined(TARGET_PSA) || (!defined(COMPONENT_PSA_SRV_IPC)) && !defined(MBEDTLS_ENTROPY_NV_SEED)))
#error [NOT_SUPPORTED] PSA entropy injection tests can run only on PSA-enabled targets.
#else

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "psa/internal_trusted_storage.h"
#include "psa/lifecycle.h"
#include "entropy.h"
#include "entropy_poll.h"
#include "psa/crypto.h"
#include <string.h>

/* MAX value support macro */
#if !defined(MAX)
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

/* Calculating the minimum allowed entropy size in bytes */
#define MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE \
            MAX(MBEDTLS_ENTROPY_MIN_PLATFORM, MBEDTLS_ENTROPY_BLOCK_SIZE)

using namespace utest::v1;

uint8_t seed[MBEDTLS_ENTROPY_MAX_SEED_SIZE + 2] = {0};
bool skip_tests = false;

void validate_entropy_seed_injection(int seed_length_a,
                                     int expected_status_a,
                                     int seed_length_b,
                                     int expected_status_b)
{
    psa_status_t status;
    uint8_t output[32] = { 0 };
    uint8_t zeros[32] = { 0 };
    int memcmp_res = 0;
    status = mbedtls_psa_inject_entropy(seed, seed_length_a);
    TEST_ASSERT_EQUAL_INT(expected_status_a, status);

    status = mbedtls_psa_inject_entropy(seed, seed_length_b);
    TEST_ASSERT_EQUAL_INT(expected_status_b, status);

    status = psa_crypto_init();
    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);

    status = psa_generate_random(output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);

    memcmp_res = memcmp(output, zeros, sizeof(output));
    TEST_ASSERT_NOT_EQUAL(0, memcmp_res);
}

void run_entropy_inject_with_crypto_init()
{
    psa_status_t status;
    status = psa_crypto_init();
    TEST_ASSERT_EQUAL_INT(PSA_ERROR_INSUFFICIENT_ENTROPY, status);

    status = mbedtls_psa_inject_entropy(seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE);
    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);

    status = psa_crypto_init();
    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);

    status = mbedtls_psa_inject_entropy(seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE);
    TEST_ASSERT_EQUAL_INT(PSA_ERROR_NOT_PERMITTED, status);

    mbedtls_psa_crypto_free();
    /* The seed is written by nv_seed callback functions therefore the injection will fail */
    status = mbedtls_psa_inject_entropy(seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE);
    TEST_ASSERT_EQUAL_INT(PSA_ERROR_NOT_PERMITTED, status);
}


utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");

    /* fill seed in some data */
    for (size_t i = 0; i < sizeof(seed); ++i) {
        seed[i] = i;
    }

    psa_status_t status = mbed_psa_reboot_and_request_new_security_state(PSA_LIFECYCLE_ASSEMBLY_AND_TEST);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    if (mbedtls_psa_inject_entropy(seed, MBEDTLS_ENTROPY_MAX_SEED_SIZE) == PSA_ERROR_NOT_SUPPORTED) {
        skip_tests = true;
    }

    return greentea_test_setup_handler(number_of_cases);
}

static void injection_small_good()
{
    TEST_SKIP_UNLESS(!skip_tests);
    validate_entropy_seed_injection(
        MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE, PSA_SUCCESS,
        MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE, PSA_ERROR_NOT_PERMITTED);
}

static void injection_big_good()
{
    TEST_SKIP_UNLESS(!skip_tests);
    validate_entropy_seed_injection(
        MBEDTLS_ENTROPY_MAX_SEED_SIZE, PSA_SUCCESS,
        MBEDTLS_ENTROPY_MAX_SEED_SIZE, PSA_ERROR_NOT_PERMITTED);
}

static void injection_too_small()
{
    TEST_SKIP_UNLESS(!skip_tests);
    validate_entropy_seed_injection(
        (MBEDTLS_ENTROPY_MIN_PLATFORM - 1), PSA_ERROR_INVALID_ARGUMENT,
        MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE, PSA_SUCCESS);
}

static void injection_too_big()
{
    TEST_SKIP_UNLESS(!skip_tests);
    validate_entropy_seed_injection(
        (MBEDTLS_ENTROPY_MAX_SEED_SIZE + 1), PSA_ERROR_INVALID_ARGUMENT,
        MBEDTLS_ENTROPY_MAX_SEED_SIZE, PSA_SUCCESS);
}

static void injection_and_init_deinit()
{
    TEST_SKIP_UNLESS(!skip_tests);
    run_entropy_inject_with_crypto_init();
}



/***************************************************************************************/

utest::v1::status_t case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    psa_status_t status;
    status = mbed_psa_reboot_and_request_new_security_state(PSA_LIFECYCLE_ASSEMBLY_AND_TEST);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    mbedtls_psa_crypto_free();
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

utest::v1::status_t case_setup_handler(const Case *const source, const size_t index_of_case)
{
    psa_status_t status;
    status = mbed_psa_reboot_and_request_new_security_state(PSA_LIFECYCLE_ASSEMBLY_AND_TEST);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    return greentea_case_setup_handler(source, index_of_case);
}

Case cases[] = {
    Case("PSA entropy injection small good", case_setup_handler, injection_small_good, case_teardown_handler),
    Case("PSA entropy injection big good",   case_setup_handler, injection_big_good,   case_teardown_handler),
    Case("PSA entropy injection too big",    case_setup_handler, injection_too_big,    case_teardown_handler),
    Case("PSA entropy injection too small",  case_setup_handler, injection_too_small,  case_teardown_handler),
    Case("PSA entropy injection before and after init", case_setup_handler, injection_and_init_deinit, case_teardown_handler),
};

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}

#endif // ((!defined(TARGET_PSA) || (!defined(COMPONENT_PSA_SRV_IPC)) && !defined(MBEDTLS_ENTROPY_NV_SEED)))
