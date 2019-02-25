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

#ifndef __MBEDTLS_CONFIG_READER_H__
#define __MBEDTLS_CONFIG_READER_H__

#include "platform/inc/platform_mbed.h"

#if defined(MBEDTLS_CONFIG_FILE)
#warning "MBEDTLS_CONFIG_FILE has been set. Options specified in mbed_app.json will be ignored"
#endif

#if !defined(MBEDTLS_ENTROPY_HARDWARE_ALT) && !defined(MBEDTLS_TEST_NULL_ENTROPY) && \
    !defined(MBEDTLS_ENTROPY_NV_SEED)
    // for platforms that don't have entropy,none of the ciphersuites will
    // work, so don't bother

#if MBED_CONF_MBEDTLS_PELION_CLIENT
#warning "mbedtls.pelion-client enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_PELION_CLIENT
#endif

#if MBED_CONF_MBEDTLS_MBED_PELION_MINI_CLIENT
#warning "mbedtls.pelion-client-mini enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_MBED_PELION_MINI_CLIENT
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
#warning "mbedtls.ecdhe-ecdsa-with-aes-128-gcm-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
#warning "mbedtls.ecdche-ecdsa-with-aes-128-gcb-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384
#warning "mbedtls.ecdche-ecdsa-with-aes-256-cbc-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
#warning "mbedtls.ecdche-ecdsa-with-aes-256-gcm-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_CCM
#warning "mbedtls.ecdche-ecdsa-with-aes-ccm enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_CCM
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_CCM
#warning "mbedtls.psk-with-aes-ccm enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_PSK_WITH_AES_CCM
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_128_CBC_SHA256
#warning "mbedtls.psk-with-aes-128-cbc-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_PSK_WITH_AES_128_CBC_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
#warning "mbedtls.ecdh-ecdsa-with-aes-128-cbc-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
#warning "mbedtls.ecdh-ecdsa-with-aes-128-gcm-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384
#warning "mbedtls.ecdh-ecdsa-with-aes-256-cbc-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384
#endif

#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384
#warning "mbedtls.ecdh-ecdsa-with-aes-256-gcm-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_128_GCM_SHA256
#warning "mbedtls.psk-with-aes-128-gcm-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_PSK_WITH_AES_128_GCM_SHA256
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_256_GCM_SHA384
#warning "mbedtls.psk-with-aes-256-gcm-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_PSK_WITH_AES_256_GCM_SHA384
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_256_CBC_SHA384
#warning "mbedtls.psk-with-aes-256-cbc-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_PSK_WITH_AES_256_CBC_SHA384
#endif

#if MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256
#warning "mbedtls.ecdhe-rsa-with-aes-128-cbc-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
#warning "mbedtls.ecdhe-rsa-with-aes-128-gcm-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384
#warning "mbedtls.ecdhe-rsa-with-aes-256-cbc-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384
#endif

#if MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384
#warning "mbedtls.ecdhe-rsa-with-aes-256-gcm-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384
#endif

#if MBED_CONF_MBEDTLS_RSA_WITH_AES_256_GCM_SHA384
#warning "mbedtls.rsa-with-aes-256-gcm-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_RSA_WITH_AES_256_GCM_SHA384
#endif

#if MBED_CONF_MBEDTLS_RSA_WITH_AES_128_GCM_SHA256
#warning "mbedtls.rsa-with-aes-128-gcm-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_RSA_WITH_AES_128_GCM_SHA256
#endif

#if MBED_CONF_MBEDTLS_RSA_WITH_AES_128_CBC_SHA256
#warning "mbedtls.rsa-with-aes-128-cbc-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_RSA_WITH_AES_128_CBC_SHA256
#endif

#if MBED_CONF_MBEDTLS_RSA_WITH_AES_256_CBC_SHA256
#warning "mbedtls.rsa-with-aes-256-cbc-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_RSA_WITH_AES_256_CBC_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_128_CBC_SHA256
#warning "mbedtls.ecdh-rsa-with-aes-128-cbc-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_128_CBC_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_128_GCM_SHA256
#warning "mbedtls.ecdh-rsa-with-aes-128-gcm-sha256 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_128_GCM_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_256_CBC_SHA384
#warning "mbedtls.ecdh-rsa-with-aes-256-cbc-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_256_CBC_SHA384
#endif

#if MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_256_GCM_SHA384
#warning "mbedtls.ecdh-rsa-with-aes-256-gcm-sha384 enabled, but the platform does not have entropy so TLS will be non-functional"
#undef MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_256_GCM_SHA384
#endif
    
#endif // !defined(MBEDTLS_ENTROPY_HARDWARE_ALT) && !defined(MBEDTLS_TEST_NULL_ENTROPY) && !defined(MBEDTLS_ENTROPY_NV_SEED)

// This first section is checking the values from mbed_app.json and
// mbedtls/mbed_lib.json and setting the appropriate defines for them

#if MBED_CONF_MBEDTLS_PELION_CLIENT
    #undef MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
    #define MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 1
    #undef MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
    #define MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 1
#endif

#if MBED_CONF_MBEDTLS_MBED_PELION_MINI_CLIENT
    #undef MBED_CONF_MBEDTLS_PSK_WITH_AES_CCM
    #define MBED_CONF_MBEDTLS_PSK_WITH_AES_CCM 1
    #undef MBED_CONF_MBEDTLS_PSK_WITH_AES_128_CBC_SHA256
    #define MBED_CONF_MBEDTLS_PSK_WITH_AES_128_CBC_SHA256 1
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
    #define MBEDTLS_CONFIG_ECDHE
    #define MBEDTLS_CONFIG_ECDSA
    #define MBEDTLS_CONFIG_ECDHE_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
    #define MBEDTLS_CONFIG_ECDHE
    #define MBEDTLS_CONFIG_ECDSA
    #define MBEDTLS_CONFIG_ECDHE_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384
    #define MBEDTLS_CONFIG_ECDHE
    #define MBEDTLS_CONFIG_ECDSA
    #define MBEDTLS_CONFIG_ECDHE_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA512
#endif

#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
    #define MBEDTLS_CONFIG_ECDHE
    #define MBEDTLS_CONFIG_ECDSA
    #define MBEDTLS_CONFIG_ECDHE_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA512
#endif


#if MBED_CONF_MBEDTLS_ECDHE_ECDSA_WITH_AES_CCM
    #define MBEDTLS_CONFIG_ECDHE
    #define MBEDTLS_CONFIG_ECDSA
    #define MBEDTLS_CONFIG_ECDHE_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CCM
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_CCM
    #define MBEDTLS_CONFIG_PSK
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CCM
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_128_CBC_SHA256
    #define MBEDTLS_CONFIG_PSK
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
    #define MBEDTLS_CONFIG_ECDH
    #define MBEDTLS_CONFIG_ECDSA
    #define MBEDTLS_CONFIG_ECDH_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
    #define MBEDTLS_CONFIG_ECDH
    #define MBEDTLS_CONFIG_ECDSA
    #define MBEDTLS_CONFIG_ECDH_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA256
#endif

#if MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384
    #define MBEDTLS_CONFIG_ECDH
    #define MBEDTLS_CONFIG_ECDSA
    #define MBEDTLS_CONFIG_ECDH_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA512
#endif

#if (MBED_CONF_MBEDTLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384)
    #define MBEDTLS_CONFIG_ECDH
    #define MBEDTLS_CONFIG_ECDSA
    #define MBEDTLS_CONFIG_ECDH_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA512
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_128_GCM_SHA256
    #define MBEDTLS_CONFIG_PSK
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA256
#endif

#if MBED_CONF_MBEDTLS_PSK_WITH_AES_256_GCM_SHA384
    #define MBEDTLS_CONFIG_PSK
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA512
#endif

#if (MBED_CONF_MBEDTLS_PSK_WITH_AES_256_CBC_SHA384)
    #define MBEDTLS_CONFIG_PSK
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA512
#endif

#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256)
    #define MBEDTLS_CONFIG_ECDHE
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_ECDHE_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA256
#endif

#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256)
    #define MBEDTLS_CONFIG_ECDHE
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_ECDHE_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA256
#endif

#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384)
    #define MBEDTLS_CONFIG_ECDHE
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_ECDHE_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA512
#endif

#if (MBED_CONF_MBEDTLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384)
    #define MBEDTLS_CONFIG_ECDHE
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_ECDHE_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA512
#endif

#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_256_GCM_SHA384)
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA512
#endif

#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_128_GCM_SHA256)
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA256
#endif

#if (MBED_CONF_MBEDTLS_RSA_WITH_AES_128_CBC_SHA256 || MBED_CONF_MBEDTLS_RSA_WITH_AES_256_CBC_SHA256)
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA256
#endif

#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_128_CBC_SHA256)
    #define MBEDTLS_CONFIG_ECDH
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_ECDH_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA256
#endif

#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_128_GCM_SHA256)
    #define MBEDTLS_CONFIG_ECDH
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_ECDH_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA256
#endif

#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_256_CBC_SHA384)
    #define MBEDTLS_CONFIG_ECDH
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_ECDH_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CONFIG_SHA512
#endif

#if (MBED_CONF_MBEDTLS_ECDH_RSA_WITH_AES_256_GCM_SHA384)
    #define MBEDTLS_CONFIG_ECDH
    #define MBEDTLS_CONFIG_RSA
    #define MBEDTLS_CONFIG_ECDH_RSA_KEY_EXCHANGE
    #define MBEDTLS_CONFIG_AES
    #define MBEDTLS_CONFIG_GCM
    #define MBEDTLS_CONFIG_SHA512
#endif

// The rest is just making sure approrpiate pre-requisites for the selections are defined
#ifdef MBEDTLS_CONFIG_AES
    #define MBEDTLS_AES_ROM_TABLES
    #define MBEDTLS_AES_C
    #define MBEDTLS_CIPHER_C
#endif

#ifdef MBEDTLS_CONFIG_ECDHE

    #define MBEDTLS_X509_USE_C
    #define MBEDTLS_PEM_PARSE_C
    #define MBEDTLS_BASE64_C
    #define MBEDTLS_ASN1_PARSE_C
    #define MBEDTLS_PK_PARSE_C
    #define MBEDTLS_PK_C
    #define MBEDTLS_ECDH_C
    #define MBEDTLS_ECP_C
    #define MBEDTLS_BIGNUM_C
    #define MBEDTLS_ECP_DP_SECP256R1_ENABLED
    #define MBEDTLS_ECP_DP_SECP384R1_ENABLED
    #define MBEDTLS_ECP_DP_CURVE25519_ENABLED
    #define MBEDTLS_ECP_NIST_OPTIM
#endif

#ifdef MBEDTLS_CONFIG_ECDSA
    #define MBEDTLS_ECDSA_C
    #define MBEDTLS_ECP_C
    #define MBEDTLS_X509_USE_C
    #define MBEDTLS_PEM_PARSE_C
    #define MBEDTLS_BASE64_C
    #define MBEDTLS_ASN1_PARSE_C
    #define MBEDTLS_BIGNUM_C
    #define MBEDTLS_OID_C
    #define MBEDTLS_PK_PARSE_C
    #define MBEDTLS_PK_C
    #define MBEDTLS_ASN1_WRITE_C
    #define MBEDTLS_ECP_DP_SECP256R1_ENABLED
    #define MBEDTLS_ECP_DP_SECP384R1_ENABLED
    #define MBEDTLS_ECP_DP_CURVE25519_ENABLED
    #define MBEDTLS_ECP_NIST_OPTIM
    #define MBEDTLS_ECDSA_DETERMINISTIC
#endif

#ifdef MBEDTLS_CONFIG_ECDHE_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
    #define MBEDTLS_ECDH_C
    #define MBEDTLS_ECDSA_C
    #define MBEDTLS_X509_CRT_PARSE_C
    #define MBEDTLS_SSL_SERVER_NAME_INDICATION
    #define MBEDTLS_X509_USE_C
    #define MBEDTLS_PEM_PARSE_C
    #define MBEDTLS_BASE64_C
    #define MBEDTLS_ASN1_PARSE_C
    #define MBEDTLS_BIGNUM_C
    #define MBEDTLS_OID_C
    #define MBEDTLS_PK_PARSE_C
    #define MBEDTLS_PK_C
    #define MBEDTLS_ECP_DP_SECP256R1_ENABLED
    #define MBEDTLS_ECP_DP_SECP384R1_ENABLED
    #define MBEDTLS_ECP_DP_CURVE25519_ENABLED
    #define MBEDTLS_ECP_NIST_OPTIM
#endif

#ifdef MBEDTLS_CONFIG_GCM
    #define MBEDTLS_GCM_C
#endif

#ifdef MBEDTLS_CONFIG_SHA256
    #define MBEDTLS_SHA256_C
    #define MBEDTLS_MD_C
#endif

#ifdef MBEDTLS_CONFIG_SHA512
    #define MBEDTLS_SHA512_C
    #define MBEDTLS_MD_C
#endif

#ifdef MBEDTLS_CONFIG_CBC
    #define MBEDTLS_CIPHER_MODE_CBC
    #define MBEDTLS_CIPHER_PADDING_PKCS7
#endif
#ifdef MBEDTLS_CONFIG_CCM
    #define MBEDTLS_CCM_C
#endif

#ifdef MBEDTLS_CONFIG_PSK
    #define MBEDTLS_KEY_EXCHANGE_PSK_ENABLED
#endif

#ifdef MBEDTLS_CONFIG_ECDH
    #define MBEDTLS_X509_USE_C
    #define MBEDTLS_PEM_PARSE_C
    #define MBEDTLS_BASE64_C
    #define MBEDTLS_ASN1_PARSE_C
    #define MBEDTLS_PK_PARSE_C
    #define MBEDTLS_PK_C
    #define MBEDTLS_ECDH_C
    #define MBEDTLS_ECP_C
    #define MBEDTLS_BIGNUM_C
    #define MBEDTLS_ECP_DP_SECP256R1_ENABLED
    #define MBEDTLS_ECP_DP_SECP384R1_ENABLED
    #define MBEDTLS_ECP_DP_CURVE25519_ENABLED
    #define MBEDTLS_ECP_NIST_OPTIM
#endif

#ifdef MBEDTLS_CONFIG_ECDH_ECDSA_KEY_EXCHANGE
    #define MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED
    #define MBEDTLS_X509_CRT_PARSE_C
    #define MBEDTLS_SSL_SERVER_NAME_INDICATION
    #define MBEDTLS_X509_USE_C
    #define MBEDTLS_PEM_PARSE_C
    #define MBEDTLS_BASE64_C
#endif

#ifdef MBEDTLS_CONFIG_RSA
    #define MBEDTLS_RSA_C
    #define MBEDTLS_BIGNUM_C
    #define MBEDTLS_OID_C
#endif

#ifdef MBEDTLS_CONFIG_ECDHE_RSA_KEY_EXCHANGE
    #define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED
    #define MBEDTLS_ECDH_C
    #define MBEDTLS_RSA_C
    #define MBEDTLS_PKCS1_V15
    #define MBEDTLS_PKCS1_V21
    #define MBEDTLS_X509_CRT_PARSE_C
    #define MBEDTLS_SSL_SERVER_NAME_INDICATION
    #define MBEDTLS_X509_USE_C
    #define MBEDTLS_PEM_PARSE_C
    #define MBEDTLS_BASE64_C
#endif

#ifdef MBEDTLS_CONFIG_RSA_KEY_EXCHANGE
    #define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
    #define MBEDTLS_RSA_C
    
    #define MBEDTLS_PKCS1_V15
    #define MBEDTLS_X509_CRT_PARSE_C
    #define MBEDTLS_SSL_SERVER_NAME_INDICATION
    #define MBEDTLS_X509_USE_C
    #define MBEDTLS_PEM_PARSE_C
    #define MBEDTLS_BASE64_C
    #define MBEDTLS_BIGNUM_C
    #define MBEDTLS_OID_C
    #define MBEDTLS_ASN1_PARSE_C
    #define MBEDTLS_PK_PARSE_C
    #define MBEDTLS_PK_C
#endif

#ifdef MBEDTLS_CONFIG_ECDH_RSA_KEY_EXCHANGE
    #define MBEDTLS_KEY_EXCHANGE_ECDH_RSA_ENABLED
    #define MBEDTLS_PKCS1_V15
    #define MBEDTLS_ECDH_C
    #define MBEDTLS_X509_CRT_PARSE_C
    #define MBEDTLS_SSL_SERVER_NAME_INDICATION
#endif

#define MBED_TLS_HAVE_ASM
#define MBED_TLS_HAVE_TIME
#define MBEDTLS_HAVE_TIME
#define MBEDTLS_REMOVE_ARC4_CIPHERSUITES
#define MBEDTLS_CIPHER_C
#define MBEDTLS_ERROR_STRERROR_DUMMY
#define MBEDTLS_X509_CHECK_KEY_USAGE
#define MBEDTLS_X509_CHECK_EXTENDED_KEY_USAGE
#define MBEDTLS_VERSION_C
#define MBEDTLS_SELF_TEST
#define MBEDTLS_PK_RSA_ALT_SUPPORT
#define MBEDTLS_SSL_SESSION_TICKETS
#define MBEDTLS_VERSION_FEATURES
#define MBEDTLS_DEBUG_C
#define MBEDTLS_ERROR_C
#define MBEDTLS_AES_C
#define MBEDTLS_SHA256_C

#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT) || defined(MBEDTLS_TEST_NULL_ENTROPY) || \
    defined(MBEDTLS_ENTROPY_NV_SEED)

#define MBEDTLS_HMAC_DRBG_C
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_SSL_ALL_ALERT_MESSAGES
#define MBEDTLS_SSL_ALPN
#define MBEDTLS_SSL_CACHE_C
#define MBEDTLS_SSL_COOKIE_C
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_DTLS_ANTI_REPLAY
#define MBEDTLS_SSL_DTLS_BADMAC_LIMIT
#define MBEDTLS_SSL_DTLS_CLIENT_PORT_REUSE
#define MBEDTLS_SSL_DTLS_HELLO_VERIFY
#define MBEDTLS_SSL_ENCRYPT_THEN_MAC
#define MBEDTLS_SSL_EXPORT_KEYS
#define MBEDTLS_SSL_EXTENDED_MASTER_SECRET
#define MBEDTLS_SSL_MAX_FRAGMENT_LENGTH
#define MBEDTLS_SSL_PROTO_DTLS
#define MBEDTLS_SSL_PROTO_TLS1_2
#define MBEDTLS_SSL_RENEGOTIATION
#define MBEDTLS_SSL_TLS_C

#endif // (MBEDTLS_ENTROPY_HARDWARE_ALT) || defined(MBEDTLS_TEST_NULL_ENTROPY) || defined(MBEDTLS_ENTROPY_NV_SEED)


// finally, let any file referred to with mbedtls.app-config-file override
// any of the other selections
#ifdef MBED_CONF_MBEDTLS_APP_CONFIG_FILE

#include <limits.h>
#include MBED_CONF_MBEDTLS_APP_CONFIG_FILE

#endif // else MBED_CONF_MBEDTLS_APP_CONFIG_FILE

#include "check_config.h"

#endif // __MBEDTLS_CONFIG_READER_H__