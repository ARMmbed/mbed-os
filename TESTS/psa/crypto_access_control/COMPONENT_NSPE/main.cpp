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

int main(void)
{
    return (1);
}
