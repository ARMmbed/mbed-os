/*
 * Copyright (c) 2013-2018, ARM Limited, All Rights Reserved
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
#include "mbedtls/ssl.h"
#include "aes_test.h"
#include "ecdsa_test.h"
#include "ecdh_test.h"
#include "rsa_test.h"
#include "mbedtls/gcm.h"
#include "mbedtls/sha512.h"
#include "mbedtls/sha256.h"
#include "mbedtls/ccm.h"

using namespace utest::v1;

#if (MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256)
/* Tests for the presence of the TLS-ECDHE-ECDSA-WITH-AES-128-GCM ciphersuite */
void test_case_ecdhe_ecdsa_with_aes_128_gcm_sha256_ciphersuite() {

    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_ecdhe = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256) {
            found_ecdhe = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);

    int sha256_result = mbedtls_sha256_self_test(0);
    int ecdsa_result = mbedtls_ecdsa_test_mbedos(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_ecdhe, "ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdsa_result, "ECDSA test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384)
/* Tests for the presence of the TLS-ECDHE-ECDSA-WITH-AES-256-GCM-SHA384 ciphersuite */
void test_case_ecdhe_ecdsa_with_aes_256_gcm_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_256 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {

        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384) {
            found_256 = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha512_result = mbedtls_sha512_self_test(0);
    int ecdsa_result = mbedtls_ecdsa_test_mbedos(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_256, "ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha512_result, "SHA512 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdsa_result, "ECDSA test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
}
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
/* Tests for the presence of the TLS-ECDHE-ECDSA-WITH-AES-128-cbc-SHA256 ciphersuite */
void test_case_ecdhe_ecdsa_with_aes_128_cbc_sha256_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_128 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256) {
            found_128 = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha256_result = mbedtls_sha256_self_test(0);
    int ecdsa_result = mbedtls_ecdsa_test_mbedos(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_128, "ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdsa_result, "ECDSA test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
}
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384
/* Tests for the presence of the TLS-ECDHE-ECDSA-WITH-AES-256-cbc-SHA384 ciphersuite */
void test_case_ecdhe_ecdsa_with_aes_256_cbc_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_256 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384) {
            found_256 = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha512_result = mbedtls_sha512_self_test(0);
    int ecdsa_result = mbedtls_ecdsa_test_mbedos(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_256, "ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha512_result, "SHA512 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdsa_result, "ECDSA test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
}
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_CCM
/* Tests for the presence of the TLS-PSK-WITH-AES-*-CCM ciphersuites */
void test_case_psk_with_aes_ccm_ciphersuites() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_ccm_8_128 = 0;
    int found_ccm_8_256 = 0;
    int found_ccm_128 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {

        if (ciphersuites[i] == MBEDTLS_TLS_PSK_WITH_AES_128_CCM_8) {
            found_ccm_8_128 = 1;
        }else if (ciphersuites[i] == MBEDTLS_TLS_PSK_WITH_AES_256_CCM_8) {
            found_ccm_8_256 = 1;
        }else if (ciphersuites[i] == MBEDTLS_TLS_PSK_WITH_AES_128_CCM) {
            found_ccm_128 = 1;
        }
    }

    int aes_ccm_result = mbedtls_ccm_self_test(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_ccm_8_128, "PSK_WITH_AES_128_CCM_8 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_ccm_8_256, "PSK_WITH_AES_256_CCM_8 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_ccm_128, "PSK_WITH_AES_128_CCM not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_ccm_result, "AES-CCM test failed");
}
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_128_CBC_SHA256
/* Tests for the presence of the TLS-PSK-WITH-AES-*-CBC-SHA256 ciphersuites */
void test_case_psk_with_aes_128_cbc_sha256_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_128 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_PSK_WITH_AES_128_CBC_SHA256) {
            found_128 = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha256_result = mbedtls_sha256_self_test(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_128, "PSK_WITH_AES_128_CBC_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
}
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_CCM
/* Tests for the presence of the TLS-ECDHE-ECDSA-WITH-AES-*-CCM ciphersuites */
void test_case_ecdhe_ecdsa_with_aes_ccm_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;

    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_norm = 0;
    int found_8 = 0;
    int found_128 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CCM) {
            found_norm = 1;
        }
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CCM_8) {
            found_8 = 1;
        }
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM) {
            found_128 = 1;
        }
    }

    int aes_ccm_result = mbedtls_ccm_self_test(0);
    int ecdsa_result = mbedtls_ecdsa_test_mbedos(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);
    

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_norm, "ECDHE_ECDSA_WITH_AES_256_CCM not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_8, "ECDHE_ECDSA_WITH_AES_256_CCM_8 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_128, "ECDHE_ECDSA_WITH_AES_128_CCM not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_ccm_result, "AES-CCM test failed\n");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdsa_result, "ECDSA test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
}
#endif
#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
/* Tests for the presence of the TLS-ECDH-ECDSA-WITH-AES-128-CBC-SHA256 ciphersuite */
void test_case_ecdh_ecdsa_with_aes_128_cbc_sha256_ciphersuite() {

    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_128 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256) {
            found_128 = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);

    int sha256_result = mbedtls_sha256_self_test(0);
    int ecdsa_result = mbedtls_ecdsa_test_mbedos(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_128, "ECDH_ECDSA_WITH_AES_128_CBC_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdsa_result, "ECDSA test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
}
#endif // MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256

#if (MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256)
/* Tests for the presence of the TLS-ECDH-ECDSA-WITH-AES-128-GCM-SHA256 ciphersuite */
void test_case_ecdh_ecdsa_with_aes_128_gcm_sha256_ciphersuite() {

    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_ecdh = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if(ciphersuites[i] == MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256) {
            found_ecdh = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);

    int sha256_result = mbedtls_sha256_self_test(0);
    int ecdsa_result = mbedtls_ecdsa_test_mbedos(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_ecdh, "ECDH_ECDSA_WITH_AES_128_GCM_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdsa_result, "ECDSA test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
}
#endif

#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384
/* Tests for the presence of the TLS-ECDH-ECDSA-WITH-AES-256-CBC-SHA384 ciphersuite */
void test_case_ecdh_ecdsa_with_aes_256_cbc_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_256 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384) {
            found_256 = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha512_result = mbedtls_sha512_self_test(0);
    int ecdsa_result = mbedtls_ecdsa_test_mbedos(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_256, "ECDH_ECDSA_WITH_AES_256_CBC_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha512_result, "SHA512 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdsa_result, "ECDSA test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384)
/* Tests for the presence of the TLS-ECDH-ECDSA-WITH-AES-256-GCM-SHA384 ciphersuite */
void test_case_ecdh_ecdsa_with_aes_256_gcm_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_256 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {

        if (ciphersuites[i] == MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384) {
            found_256 = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha512_result = mbedtls_sha512_self_test(0);
    int ecdsa_result = mbedtls_ecdsa_test_mbedos(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_256, "ECDH_ECDSA_WITH_AES_256_GCM_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha512_result, "SHA512 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdsa_result, "ECDSA test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
}
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_128_GCM_SHA256
/* Tests for the presence of the TLS-PSK-WITH-AES-128-GCM-SHA256 ciphersuite */
void test_case_psk_with_aes_128_gcm_sha256_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_128 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_PSK_WITH_AES_128_GCM_SHA256) {
            found_128 = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha256_result = mbedtls_sha256_self_test(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_128, "PSK_WITH_AES_128_GCM_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
}
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_256_GCM_SHA384
/* Tests for the presence of the TLS-PSK-WITH-AES-256-GCM-SHA384 ciphersuite */
void test_case_psk_with_aes_256_gcm_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_128 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_PSK_WITH_AES_256_GCM_SHA384) {
            found_128 = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha256_result = mbedtls_sha512_self_test(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_128, "PSK_WITH_AES_256_GCM_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA512 test failed");
}
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_256_CBC_SHA384
/* Tests for the presence of the TLS-PSK-WITH-AES-256-CBC-SHA384 ciphersuite */
void test_case_psk_with_aes_256_cbc_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found_128 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_PSK_WITH_AES_256_CBC_SHA384) {
            found_128 = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha256_result = mbedtls_sha512_self_test(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found_128, "PSK_WITH_AES_256_CBC_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA512 test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256)
/* Tests for the presence of the TLS-ECDHE-RSA-WITH-AES-128-CBC-SHA256 ciphersuite */
void test_case_ecdhe_rsa_with_aes_128_cbc_sha256_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256) {
            found = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha256_result = mbedtls_sha256_self_test(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "ECDHE_RSA_WITH_AES_128_CBC_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256)
/* Tests for the presence of the TLS-ECDHE-RSA-WITH-AES-128-GCM-SHA256 ciphersuite */
void test_case_ecdhe_rsa_with_aes_128_gcm_sha256_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256) {
            found = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha256_result = mbedtls_sha256_self_test(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "ECDHE_RSA_WITH_AES_128_GCM_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384)
/* Tests for the presence of the TLS-ECDHE-RSA-WITH-AES-256-CBC-SHA384 ciphersuite */
void test_case_ecdhe_rsa_with_aes_256_cbc_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384) {
            found = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha512_result = mbedtls_sha512_self_test(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "ECDHE_RSA_WITH_AES_256_CBC_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha512_result, "SHA512 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384)
/* Tests for the presence of the TLS-ECDHE-RSA-WITH-AES-256-GCM-SHA384 ciphersuite */
void test_case_ecdhe_rsa_with_aes_256_gcm_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384) {
            found = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha512_result = mbedtls_sha512_self_test(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "ECDHE_RSA_WITH_AES_256_GCM_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha512_result, "SHA512 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");

}
#endif

#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_256_GCM_SHA384)
/* Tests for the presence of the TLS-RSA-WITH-AES-256-GCM-SHA384 ciphersuite */
void test_case_rsa_with_aes256_gcm_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_RSA_WITH_AES_256_GCM_SHA384) {
            found = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha512_result = mbedtls_sha512_self_test(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "RSA_WITH_AES_256_GCM_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha512_result, "SHA512 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_128_GCM_SHA256)
/* Tests for the presence of the TLS-RSA-WITH-AES-128-GCM-SHA256 ciphersuite */
void test_case_rsa_with_aes128_gcm_sha256_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256) {
            found = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha256_result = mbedtls_sha256_self_test(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "RSA_WITH_AES_128_GCM_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_128_CBC_SHA256)
/* Tests for the presence of the TLS-RSA-WITH-AES-128-CBC-SHA256 ciphersuite */
void test_case_rsa_with_aes128_cbc_sha256_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA256) {
            found = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha256_result = mbedtls_sha256_self_test(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "RSA_WITH_AES_128_GCM_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_256_CBC_SHA256)
/* Tests for the presence of the TLS-RSA-WITH-AES-256-CBC-SHA256 ciphersuite */
void test_case_rsa_with_aes256_cbc_sha256_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA256) {
            found = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha256_result = mbedtls_sha256_self_test(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "RSA_WITH_AES_256_GCM_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_128_CBC_SHA256)
/* Tests for the presence of the TLS-RSA-WITH-AES-128-CBC-SHA256 ciphersuite */
void test_case_ecdh_rsa_with_aes128_cbc_sha256_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256) {
            found = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha256_result = mbedtls_sha256_self_test(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "ECDH_RSA_WITH_AES_128_CBC_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_128_GCM_SHA256)
/* Tests for the presence of the TLS-ECDH-RSA-WITH-AES-128-GCM-SHA256 ciphersuite */
void test_case_ecdh_rsa_with_aes128_gcm_sha256_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256) {
            found = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha256_result = mbedtls_sha256_self_test(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "ECDH_RSA_WITH_AES_128_GCM_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_256_CBC_SHA384)
/* Tests for the presence of the TLS-ECDH-RSA-WITH-AES-256-CBC-SHA384 ciphersuite */
void test_case_ecdh_rsa_with_aes256_cbc_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384) {
            found = 1;
        }
    }

    int aes_cbc_result = mbedtls_aes_cbc_test_mbedos(0);
    int sha512_result = mbedtls_sha512_self_test(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "ECDH_RSA_WITH_AES_256_CBC_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_cbc_result, "AES-CBC test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha512_result, "SHA512 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_256_GCM_SHA384)
/* Tests for the presence of the TLS-ECDH-RSA-WITH-AES-256-GCM-SHA384 ciphersuite */
void test_case_ecdh_rsa_with_aes256_gcm_sha384_ciphersuite() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int found = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384) {
            found = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha512_result = mbedtls_sha512_self_test(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);
    int rsa_result = mbedtls_config_rsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found, "ECDH_RSA_WITH_AES_128_GCM_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha512_result, "SHA512 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, rsa_result, "RSA test failed");
}
#endif

#if (MBED_CONF_MBEDTLS_PELION_CLIENT)
/* Tests for the presence of the Pelion  Client ciphersuites */
void test_case_pelion_client_ciphersuites() {
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;


    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, 0);
    ssl.conf = &conf;

    const int *ciphersuites = ssl.conf->ciphersuite_list[ssl.minor_ver];

    int gcm_128 = 0;
    int gcm_256 = 0;
    for(int i = 0; ciphersuites[i] != 0; i++) {
        if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256) {
            gcm_128 = 1;
        } else if (ciphersuites[i] == MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384) {
            gcm_256 = 1;
        }
    }

    int aes_gcm_result = mbedtls_gcm_self_test(0);
    int sha256_result = mbedtls_sha256_self_test(0);
    int sha512_result = mbedtls_sha512_self_test(0);
    int ecdh_result = mbedtls_ecdh_test_mbedos(0);
    int ecdsa_result = mbedtls_ecdsa_test_mbedos(0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, gcm_128, "ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, gcm_256, "ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 not found in ciphersuites");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, aes_gcm_result, "AES-GCM test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha256_result, "SHA256 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, sha512_result, "SHA512 test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdh_result, "ECDH test failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ecdsa_result, "ECDSA test failed");
}
#endif

void test_case_mbed_https_connection() {
    int ret = mbedtls_connection_test_mbedos(1);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ret, "HTTPS Connection test failed");
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
    Case("MbedTLS Config: ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 ciphersuite", test_case_ecdhe_ecdsa_with_aes_128_gcm_sha256_ciphersuite, greentea_failure_handler),
#endif
#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
    Case("MbedTLS Config: ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 ciphersuite", test_case_ecdhe_ecdsa_with_aes_128_cbc_sha256_ciphersuite, greentea_failure_handler),
#endif
#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
    Case("MbedTLS Config: ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 ciphersuite", test_case_ecdhe_ecdsa_with_aes_256_gcm_sha384_ciphersuite, greentea_failure_handler),
#endif
#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384
    Case("MbedTLS Config: ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 ciphersuite", test_case_ecdhe_ecdsa_with_aes_256_cbc_sha384_ciphersuite, greentea_failure_handler),
#endif
#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_CCM_8
    Case("MbedTLS Config: ECDHE_ECDSA_WITH_AES_128_CCM_8 ciphersuite", test_case_ecdhe_ecdsa_with_aes_128_ccm_8_ciphersuite, greentea_failure_handler),
#endif
#if MBED_CONF_MBEDTLS_PSK_WITH_AES_CCM  
    Case("MbedTLS Config: PSK_WITH_AES_CCM* ciphersuites", test_case_psk_with_aes_ccm_ciphersuites, greentea_failure_handler),
#endif
#if MBED_CONF_MBEDTLS_PSK_WITH_AES_128_CBC_SHA256
    Case("MbedTLS Config: PSK_WITH_AES_128_CBC_SHA256 ciphersuite", test_case_psk_with_aes_128_cbc_sha256_ciphersuite, greentea_failure_handler),
#endif
#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_CCM
    Case("MbedTLS Config: ECDHE_ECDSA_WITH_AES_CCM ciphersuite", test_case_ecdhe_ecdsa_with_aes_ccm_ciphersuite, greentea_failure_handler),
#endif
#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
    Case("MBedTLS Config: ECDH_ECDSA_WITH_AES_128_CBC_SHA256 ciphersuite", test_case_ecdh_ecdsa_with_aes_128_cbc_sha256_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256)
    Case("MMbedTLS Config: ECDH_ECDSA_WITH_AES_128_GCM_SHA256 ciphersuite", test_case_ecdh_ecdsa_with_aes_128_gcm_sha256_ciphersuite, greentea_failure_handler),
#endif
#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384
    Case("MbedTLS Config: ECDH_ECDSA_WITH_AES_256_CBC_SHA384 ciphersuite", test_case_ecdh_ecdsa_with_aes_256_cbc_sha384_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384)
    Case("MbedTLS Config: ECDH_ECDSA_WITH_AES_256_GCM_SHA384 ciphersuite", test_case_ecdh_ecdsa_with_aes_256_gcm_sha384_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_PSK_WITH_AES_128_GCM_SHA256)
    Case("MbedTLS Config: PSK_WITH_AES_128_GCM_SHA256 ciphersuite", test_case_psk_with_aes_128_gcm_sha256_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_PSK_WITH_AES_256_GCM_SHA384)
    Case("MbedTLS Config: PSK_WITH_AES_256_GCM_SHA384 ciphersuite", test_case_psk_with_aes_256_gcm_sha384_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_PSK_WITH_AES_256_CBC_SHA384)
    Case("MbedTLS Config: PSK_WITH_AES_256_CBC_SHA384 ciphersuite", test_case_psk_with_aes_256_cbc_sha384_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256)
    Case("MbedTLS Config: ECDHE_RSA_WITH_AES_128_CBC_SHA256 ciphersuite", test_case_ecdhe_rsa_with_aes_128_cbc_sha256_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256)
    Case("MbedTLS Config: ECDHE_RSA_WITH_AES_128_GCM_SHA256 ciphersuite", test_case_ecdhe_rsa_with_aes_128_gcm_sha256_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384)
    Case("MbedTLS Config: ECDHE_RSA_WITH_AES_256_CBC_SHA384 ciphersuite", test_case_ecdhe_rsa_with_aes_256_cbc_sha384_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384)
    Case("MbedTLS config: ECDHE_RSA_WITH_AES_256_GCM_SHA384 ciphersuite", test_case_ecdhe_rsa_with_aes_256_gcm_sha384_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_256_GCM_SHA384)
    Case("MbedTLS Config: RSA_WITH_AES_256_GCM_SHA384 ciphersuite", test_case_rsa_with_aes256_gcm_sha384_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_128_GCM_SHA256)
    Case("MbedTLS Config: RSA_WITH_AES_128_GCM_SHA256 ciphersuite", test_case_rsa_with_aes128_gcm_sha256_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_128_CBC_SHA256)
    Case("MbedTLS Config: RSA_WITH_AES_128_CBC_SHA256 ciphersuite", test_case_rsa_with_aes128_cbc_sha256_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_256_CBC_SHA256)
    Case("MbedTLS Config: RSA_WITH_AES_256_CBC_SHA256 ciphersuite", test_case_rsa_with_aes256_cbc_sha256_ciphersuite, greentea_failure_handler),
#endif
#if(MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_128_CBC_SHA256)
    Case("MbedTLS Config: ECDH_RSA_WITH_AES_128_CBC_SHA256 ciphersuite", test_case_ecdh_rsa_with_aes128_cbc_sha256_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_128_GCM_SHA256)
    Case("MbedTLS Config: ECDH_RSA_WITH_AES_128_GCM_SHA256 ciphersuite", test_case_ecdh_rsa_with_aes128_gcm_sha256_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_256_CBC_SHA384)
    Case("MbedTLS Config: ECDH_RSA_WITH_AES_256_CBC_SHA384 ciphersuite", test_case_ecdh_rsa_with_aes256_cbc_sha384_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_256_GCM_SHA384)
    Case("MbedTLS Config: ECDH_RSA_WITH_AES_256_GCM_SHA384 ciphersuite", test_case_ecdh_rsa_with_aes256_gcm_sha384_ciphersuite, greentea_failure_handler),
#endif
#if (MBED_CONF_MBEDTLS_PELION_CLIENT)
    Case("MbedTLS Config: Pelion Client Ciphersuites", test_case_pelion_client_ciphersuites, greentea_failure_handler),
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(70, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
