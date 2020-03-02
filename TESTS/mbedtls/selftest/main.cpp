/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#if defined(MBED_CONF_RTOS_PRESENT)
#include "rtos.h"
#endif

using namespace utest::v1;

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"
#include "mbedtls/entropy.h"
#include "mbedtls/entropy_poll.h"

#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf     printf
#endif

#define MBEDTLS_SELF_TEST_TEST_CASE(self_test_function) \
    void self_test_function ## _test_case() {           \
        int ret = self_test_function(0);                \
        TEST_ASSERT_EQUAL(ret, 0);                      \
    }

#if defined(MBEDTLS_SELF_TEST)

#if defined(MBEDTLS_SHA256_C)
MBEDTLS_SELF_TEST_TEST_CASE(mbedtls_sha256_self_test)
#endif

#if defined(MBEDTLS_SHA512_C)
MBEDTLS_SELF_TEST_TEST_CASE(mbedtls_sha512_self_test)
#endif

#if defined(MBEDTLS_ENTROPY_C)
MBEDTLS_SELF_TEST_TEST_CASE(mbedtls_entropy_self_test)
#endif

#else
#warning "MBEDTLS_SELF_TEST not enabled"
#endif /* MBEDTLS_SELF_TEST */

Case cases[] = {
#if defined(MBEDTLS_SELF_TEST)

#if defined(MBEDTLS_SHA256_C)
    Case("mbedtls_sha256_self_test", mbedtls_sha256_self_test_test_case),
#endif

#if defined(MBEDTLS_SHA512_C)
    Case("mbedtls_sha512_self_test", mbedtls_sha512_self_test_test_case),
#endif

#if defined(MBEDTLS_ENTROPY_C)
    Case("mbedtls_entropy_self_test", mbedtls_entropy_self_test_test_case),
#endif

#endif /* MBEDTLS_SELF_TEST */
};

#if (defined(MBEDTLS_ENTROPY_C) && defined(TARGET_PSA) && defined(COMPONENT_PSA_SRV_IPC) && defined(MBEDTLS_PSA_CRYPTO_C))
#include "crypto.h"
#if !defined(MAX)
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

/* Calculating the minimum allowed entropy size in bytes */
#define MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE \
            MAX(MBEDTLS_ENTROPY_MIN_PLATFORM, MBEDTLS_ENTROPY_BLOCK_SIZE)

void inject_entropy_for_psa()
{
    if (psa_crypto_init() == PSA_ERROR_INSUFFICIENT_ENTROPY) {
        uint8_t seed[MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE] = {0};
        /* inject some a seed for test*/
        for (int i = 0; i < MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE; ++i) {
            seed[i] = i;
        }

        /* don't really care if this succeed this is just to make crypto init pass*/
        mbedtls_psa_inject_entropy(seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE);
    }
}
#endif // (defined(MBEDTLS_ENTROPY_C) && defined(TARGET_PSA) && defined(COMPONENT_PSA_SRV_IPC) && defined(MBEDTLS_PSA_CRYPTO_C))


utest::v1::status_t test_setup(const size_t num_cases)
{
    GREENTEA_SETUP(120, "default_auto");
    return verbose_test_setup_handler(num_cases);
}

Specification specification(test_setup, cases);

int main()
{
    int ret = 0;
#if defined(MBEDTLS_PLATFORM_C)
    if ((ret = mbedtls_platform_setup(NULL)) != 0) {
        mbedtls_printf("Mbed TLS selftest failed! mbedtls_platform_setup returned %d\n", ret);
        return 1;
    }
#endif

#if (defined(MBEDTLS_ENTROPY_C) && defined(TARGET_PSA) && defined(COMPONENT_PSA_SRV_IPC) && defined(MBEDTLS_PSA_CRYPTO_C))
    inject_entropy_for_psa();
#endif

    ret = (Harness::run(specification) ? 0 : 1);
#if defined(MBEDTLS_PLATFORM_C)
    mbedtls_platform_teardown(NULL);
#endif
    return ret;
}

