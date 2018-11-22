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

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "psa_prot_internal_storage.h"
#include "test_pits.h"
#include "entropy.h"
#include "entropy_poll.h"
#include "crypto.h"

#if ((!defined(TARGET_PSA) || (!defined(COMPONENT_PSA_SRV_IPC)) && !defined(MBEDTLS_ENTROPY_NV_SEED)))
#error [NOT_SUPPORTED] PSA entropy injection tests can run only on PSA-enabled targets.
#endif // TARGET_PSA

/* MAX value support macro */
#if !defined(MAX)
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

/* Calculating the minimum allowed entropy size in bytes */
#define MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE \
            MAX(MBEDTLS_ENTROPY_MIN_PLATFORM, MBEDTLS_ENTROPY_BLOCK_SIZE)

using namespace utest::v1;

uint8_t seed[MBEDTLS_ENTROPY_MAX_SEED_SIZE+2] = {0};

void validate_entropy_seed_injection( int seed_length_a,
                                      int expected_status_a,
                                      int seed_length_b,
                                      int expected_status_b )
{
    psa_status_t status;
    uint8_t output[32] = { 0 };
    uint8_t zeros[32] = { 0 };
    status = mbedtls_psa_inject_entropy( seed, seed_length_a );
    TEST_ASSERT( status == expected_status_a );
    status = mbedtls_psa_inject_entropy( seed, seed_length_b );
    TEST_ASSERT( status == expected_status_b );
    TEST_ASSERT( psa_crypto_init( ) == PSA_SUCCESS );
    TEST_ASSERT( psa_generate_random( output, sizeof( output ) ) == PSA_SUCCESS );
    TEST_ASSERT( memcmp( output , zeros, sizeof( output ) ) != 0 );
}

void run_entropy_inject_with_crypto_init( )
{
    psa_its_status_t its_status;
    psa_status_t status;
    status = psa_crypto_init();
    TEST_ASSERT( status == PSA_ERROR_INSUFFICIENT_ENTROPY );
    status = mbedtls_psa_inject_entropy( seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE );
    TEST_ASSERT( status == PSA_SUCCESS );
    status = psa_crypto_init();
    TEST_ASSERT( status == PSA_SUCCESS );
    status = mbedtls_psa_inject_entropy( seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE );
    TEST_ASSERT( status == PSA_ERROR_NOT_PERMITTED );
    mbedtls_psa_crypto_free( );
    /* The seed is written by nv_seed callback functions therefore the injection will fail */
    status = mbedtls_psa_inject_entropy( seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE );
    TEST_ASSERT( status == PSA_ERROR_NOT_PERMITTED );
}


utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
#ifndef NO_GREENTEA
    GREENTEA_SETUP(60, "default_auto");
#endif
    return greentea_test_setup_handler(number_of_cases);
}

static void injection_small_good()
{
    validate_entropy_seed_injection( MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE, PSA_SUCCESS, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE, PSA_ERROR_NOT_PERMITTED);
}

static void injection_big_good()
{
    validate_entropy_seed_injection(MBEDTLS_ENTROPY_MAX_SEED_SIZE, PSA_SUCCESS, MBEDTLS_ENTROPY_MAX_SEED_SIZE, PSA_ERROR_NOT_PERMITTED);
}

static void injection_too_small()
{
    validate_entropy_seed_injection((MBEDTLS_ENTROPY_MIN_PLATFORM - 1), PSA_ERROR_INVALID_ARGUMENT, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE, PSA_SUCCESS);
}

static void injection_too_big()
{
    validate_entropy_seed_injection((MBEDTLS_ENTROPY_MAX_SEED_SIZE + 1), PSA_ERROR_INVALID_ARGUMENT, MBEDTLS_ENTROPY_MAX_SEED_SIZE, PSA_SUCCESS);
}

static void injection_and_init_deinit()
{
    run_entropy_inject_with_crypto_init();
}



/***************************************************************************************/ 

utest::v1::status_t case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    psa_status_t status;
    status = test_psa_its_reset();
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);
    mbedtls_psa_crypto_free( );
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

utest::v1::status_t case_setup_handler(const Case *const source, const size_t index_of_case)
{
    psa_status_t status;
    status = test_psa_its_reset();
    TEST_ASSERT_EQUAL(PSA_ITS_SUCCESS, status);
    /* fill seed in some data */
    for( size_t i = 0; i < MBEDTLS_ENTROPY_MAX_SEED_SIZE+2; ++i)
    {
        seed[i] = i;
    }
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
