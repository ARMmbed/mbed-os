/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <string.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

#include "mbedtls/sha256.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf     printf
#endif

using namespace utest::v1;

#if defined(MBEDTLS_SHA256_C)
/* Tests several call to mbedtls_sha256_update_ret function that are not modulo 64 bytes */
void test_case_sha256_split()
{
    const unsigned char test_buf[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopqabcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopqabcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
    // sha256_output_values for 3*abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq
    const unsigned char test_sum[] = {
        0x50, 0xEA, 0x82, 0x5D, 0x96, 0x84, 0xF4, 0x22,
        0x9C, 0xA2, 0x9F, 0x1F, 0xEC, 0x51, 0x15, 0x93,
        0xE2, 0x81, 0xE4, 0x6A, 0x14, 0x0D, 0x81, 0xE0,
        0x00, 0x5F, 0x8F, 0x68, 0x86, 0x69, 0xA0, 0x6C
    };
    unsigned char outsum[32];
    int i;

    mbedtls_sha256_context ctx;
    printf("test sha256\n");
    mbedtls_sha256_init(&ctx);
    (void)mbedtls_sha256_starts_ret(&ctx, 0);
#if 0
    printf("test not splitted\n");
    (void)mbedtls_sha256_update_ret(&ctx, test_buf, 168);
#else
    printf("test splitted into 3 pieces\n");
    (void)mbedtls_sha256_update_ret(&ctx, test_buf, 2);
    (void)mbedtls_sha256_update_ret(&ctx, test_buf + 2, 66);
    (void)mbedtls_sha256_update_ret(&ctx, test_buf + 68, 100);
#endif

    (void)mbedtls_sha256_finish_ret(&ctx, outsum);
    mbedtls_sha256_free(&ctx);

    printf("\nreceived result : ");
    for (i = 0; i < 32; i++) {
        printf("%02X", outsum[i]);
    }
    printf("\nawaited result  : 50EA825D9684F4229CA29F1FEC511593E281E46A140D81E0005F8F688669A06C\n"); // for  abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq

    printf("\nend of test sha256\n");
    TEST_ASSERT_EQUAL_UINT8_ARRAY(outsum, test_sum, 32);
}

/* Tests that treating 2 sha256 objects in // does not impact the result */
void test_case_sha256_multi()
{
    const unsigned char test_buf[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopqabcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopqabcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
    const unsigned char test_buf2[] = {"abcdefghijklmnopqrstuvwxyz012345678901234567890123456789"};

    // sha256_output_values for abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq
    const unsigned char test_sum1[] = {
        0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8,
        0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
        0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67,
        0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1
    };
    // sha256_output_values for 3*abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq
    const unsigned char test_sum2[] = {
        0x50, 0xEA, 0x82, 0x5D, 0x96, 0x84, 0xF4, 0x22,
        0x9C, 0xA2, 0x9F, 0x1F, 0xEC, 0x51, 0x15, 0x93,
        0xE2, 0x81, 0xE4, 0x6A, 0x14, 0x0D, 0x81, 0xE0,
        0x00, 0x5F, 0x8F, 0x68, 0x86, 0x69, 0xA0, 0x6C
    };
    // sha256_output_values for abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopqabcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopqabcdefghijklmnopqrstuvwxyz012345678901234567890123456789
    const unsigned char test_sum3[] = {
        0x6D, 0x5D, 0xDB, 0x5F, 0x4A, 0x94, 0xAB, 0x7E,
        0x5C, 0xF7, 0x9A, 0xD8, 0x3F, 0x58, 0xD3, 0x97,
        0xFE, 0x79, 0xFB, 0x0D, 0x79, 0xB2, 0x0D, 0x22,
        0xFF, 0x95, 0x9F, 0x04, 0xA2, 0xE4, 0x6C, 0x68
    };
    unsigned char outsum1[32], outsum2[32], outsum3[32];
    int i;

    mbedtls_sha256_context ctx1;
    mbedtls_sha256_context ctx2;
    mbedtls_sha256_context ctx3;
    printf("test sha256_multi\n");
    //Init both contexts
    mbedtls_sha256_init(&ctx1);
    mbedtls_sha256_init(&ctx2);
    mbedtls_sha256_init(&ctx3);
    //Start both contexts
    (void)mbedtls_sha256_starts_ret(&ctx1, 0);
    (void)mbedtls_sha256_starts_ret(&ctx2, 0);

    printf("upd ctx1\n");
    (void)mbedtls_sha256_update_ret(&ctx1, test_buf, 56);
    printf("upd ctx2\n");
    (void)mbedtls_sha256_update_ret(&ctx2, test_buf, 66);
    printf("finish ctx1\n");
    (void)mbedtls_sha256_finish_ret(&ctx1, outsum1);
    printf("upd ctx2\n");
    (void)mbedtls_sha256_update_ret(&ctx2, test_buf + 66, 46);
    printf("clone ctx2 in ctx3\n");
    mbedtls_sha256_clone(&ctx3, (const mbedtls_sha256_context *)&ctx2);
    printf("free ctx1\n");
    mbedtls_sha256_free(&ctx1);
    printf("upd ctx2\n");
    (void)mbedtls_sha256_update_ret(&ctx2, test_buf + 112, 56);
    printf("upd ctx3 with different values than ctx2\n");
    (void)mbedtls_sha256_update_ret(&ctx3, test_buf2, 56);
    printf("finish ctx2\n");
    (void)mbedtls_sha256_finish_ret(&ctx2, outsum2);
    printf("finish ctx3\n");
    (void)mbedtls_sha256_finish_ret(&ctx3, outsum3);
    printf("free ctx2\n");
    mbedtls_sha256_free(&ctx2);
    printf("free ctx3\n");
    mbedtls_sha256_free(&ctx3);

    printf("\nreceived result ctx1 : ");
    for (i = 0; i < 32; i++) {
        printf("%02X", outsum1[i]);
    }
    printf("\nawaited result       : 248D6A61D20638B8E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1\n"); // for  abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq
    printf("\nreceived result ctx2 : ");
    for (i = 0; i < 32; i++) {
        printf("%02X", outsum2[i]);
    }
    printf("\nawaited result       : 50EA825D9684F4229CA29F1FEC511593E281E46A140D81E0005F8F688669A06C\n"); // for  3*abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq
    printf("\nreceived result ctx3 : ");
    for (i = 0; i < 32; i++) {
        printf("%02X", outsum3[i]);
    }
    printf("\nawaited result       : 6D5DDB5F4A94AB7E5CF79AD83F58D397FE79FB0D79B20D22FF959F04A2E46C68\n"); // for 2*abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq+3*0123456789
    printf("\nend of test sha256\n");
    TEST_ASSERT_EQUAL_UINT8_ARRAY(outsum1, test_sum1, 32);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(outsum2, test_sum2, 32);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(outsum3, test_sum3, 32);
}
#endif //MBEDTLS_SHA256_C

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
#if defined(MBEDTLS_SHA256_C)
    Case("Crypto: sha256_split", test_case_sha256_split, greentea_failure_handler),
    Case("Crypto: sha256_multi", test_case_sha256_multi, greentea_failure_handler),
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    int ret = 0;
#if defined(MBEDTLS_PLATFORM_C)
    if ((ret = mbedtls_platform_setup(NULL)) != 0) {
        mbedtls_printf("Mbed TLS multitest failed! mbedtls_platform_setup returned %d\n", ret);
        return 1;
    }
#endif
    ret = (Harness::run(specification) ? 0 : 1);
#if defined(MBEDTLS_PLATFORM_C)
    mbedtls_platform_teardown(NULL);
#endif
    return ret;
}
