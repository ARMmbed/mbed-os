/*
* Copyright (c) 2019 ARM Limited. All rights reserved.
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

#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] PSA attestation test cases require RTOS to run.
#else

#include "psa/crypto.h"

#if ((!defined(TARGET_PSA)) || (!defined(MBEDTLS_PSA_CRYPTO_C)))
#error [NOT_SUPPORTED] Mbed Crypto is OFF - skipping.
#else

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "psa/lifecycle.h"
#include "psa_initial_attestation_api.h"
#include "psa_attest_inject_key.h"
#include <string.h>
#include <stdlib.h>

#include "entropy.h"
#include "entropy_poll.h"

/* MAX value support macro */
#if !defined(MAX)
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#define MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE \
MAX(MBEDTLS_ENTROPY_MIN_PLATFORM, MBEDTLS_ENTROPY_BLOCK_SIZE)

using namespace utest::v1;

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

#define PSA_ATTESTATION_PRIVATE_KEY_ID 17

static const uint8_t private_key_data[] = {
    0x49, 0xc9, 0xa8, 0xc1, 0x8c, 0x4b, 0x88, 0x56,
    0x38, 0xc4, 0x31, 0xcf, 0x1d, 0xf1, 0xc9, 0x94,
    0x13, 0x16, 0x09, 0xb5, 0x80, 0xd4, 0xfd, 0x43,
    0xa0, 0xca, 0xb1, 0x7d, 0xb2, 0xf1, 0x3e, 0xee
};

static const uint8_t public_key_data[] = {
    0x04, 0x77, 0x72, 0x65, 0x6f, 0x81, 0x4b, 0x39,
    0x92, 0x79, 0xd5, 0xe1, 0xf1, 0x78, 0x1f, 0xac,
    0x6f, 0x09, 0x9a, 0x3c, 0x5c, 0xa1, 0xb0, 0xe3,
    0x53, 0x51, 0x83, 0x4b, 0x08, 0xb6, 0x5e, 0x0b,
    0x57, 0x25, 0x90, 0xcd, 0xaf, 0x8f, 0x76, 0x93,
    0x61, 0xbc, 0xf3, 0x4a, 0xcf, 0xc1, 0x1e, 0x5e,
    0x07, 0x4e, 0x84, 0x26, 0xbd, 0xde, 0x04, 0xbe,
    0x6e, 0x65, 0x39, 0x45, 0x44, 0x96, 0x17, 0xde,
    0x45
};

#define TEST_TOKEN_SIZE (0x200)
#define TEST_CHALLENGE_OBJ_SIZE (32u)

#define CHALLENGE_FOR_TEST  0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, \
                            0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, \
                            0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, \
                            0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF

static uint8_t token_buffer[TEST_TOKEN_SIZE];
static uint8_t challenge_buffer[TEST_CHALLENGE_OBJ_SIZE] = {CHALLENGE_FOR_TEST};

static void check_initial_attestation_get_token()
{
    psa_status_t status = PSA_SUCCESS;
    size_t exported_length;
    uint8_t exported[sizeof(public_key_data)];
    enum psa_attest_err_t attest_err = PSA_ATTEST_ERR_SUCCESS;
    uint32_t token_size;

    status = psa_crypto_init();
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS);
    status = psa_attestation_inject_key(private_key_data,
                                        sizeof(private_key_data),
                                        PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_CURVE_SECP256R1),
                                        exported,
                                        sizeof(exported),
                                        &exported_length);

    TEST_ASSERT_EQUAL(status, PSA_SUCCESS);
    TEST_ASSERT_EQUAL(sizeof(public_key_data), exported_length);
    TEST_ASSERT_EQUAL(memcmp(public_key_data, exported, exported_length), 0);

    attest_err = psa_initial_attest_get_token_size(TEST_CHALLENGE_OBJ_SIZE,
                                                   &token_size);

    TEST_ASSERT_EQUAL(attest_err, PSA_ATTEST_ERR_SUCCESS);

    attest_err = psa_initial_attest_get_token(challenge_buffer,
                                              TEST_CHALLENGE_OBJ_SIZE,
                                              token_buffer,
                                              &token_size);

    TEST_ASSERT_EQUAL(attest_err, PSA_ATTEST_ERR_SUCCESS);
}
/***************************************************************************************/

utest::v1::status_t case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    psa_key_handle_t handle;
    psa_open_key(PSA_ATTESTATION_PRIVATE_KEY_ID, &handle);
    psa_destroy_key(handle);
    mbedtls_psa_crypto_free();
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

utest::v1::status_t case_setup_handler(const Case *const source, const size_t index_of_case)
{
    psa_status_t status;
    status = mbed_psa_reboot_and_request_new_security_state(PSA_LIFECYCLE_ASSEMBLY_AND_TEST);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
#if (defined(COMPONENT_PSA_SRV_IPC) || defined(MBEDTLS_ENTROPY_NV_SEED))
    uint8_t seed[MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE] = {0};
    /* inject some seed for test*/
    for (int i = 0; i < MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE; ++i) {
        seed[i] = i;
    }

    /* don't really care if this succeeds this is just to make crypto init pass*/
    mbedtls_psa_inject_entropy(seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE);
#endif
    return greentea_case_setup_handler(source, index_of_case);
}


Case cases[] = {
    Case("PSA attestation get token", case_setup_handler, check_initial_attestation_get_token, case_teardown_handler),
};

Specification specification(greentea_test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // ((!defined(TARGET_PSA)) || (!defined(MBEDTLS_PSA_CRYPTO_C)))
#endif // !defined(MBED_CONF_RTOS_PRESENT)
