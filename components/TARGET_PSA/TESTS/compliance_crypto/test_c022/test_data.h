/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "val_crypto.h"

typedef struct {
    char                    test_desc[75];
    psa_key_handle_t        key_handle;
    psa_key_type_t          key_type;
    uint8_t                 key_data[32];
    uint32_t                key_length;
    psa_key_usage_t         usage;
    psa_algorithm_t         key_alg;
    const uint8_t           salt[16];
    size_t                  salt_length;
    const uint8_t           label[16];
    size_t                  label_length;
    size_t                  capacity;
    psa_status_t            expected_status;
} test_data;

static test_data check1[] = {
#ifdef ARCH_TEST_HKDF
#ifdef ARCH_TEST_SHA256
{"Test psa_key_derivation to get 16 Byte data with SHA-256\n", 1, PSA_KEY_TYPE_DERIVE,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_DERIVE, PSA_ALG_HKDF(PSA_ALG_SHA_256),
 {0}, 0, {0}, 0, 32,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_SHA512
{"Test psa_key_derivation to get 32 Byte data with SHA-512\n", 2, PSA_KEY_TYPE_DERIVE,
{0xEA, 0xD5, 0xE6, 0xC8, 0x51, 0xF9, 0xEC, 0xBB, 0x9B, 0x57, 0x7C, 0xED, 0xD2,
 0x4B, 0x82, 0x84, 0x9F, 0x9F, 0xE6, 0x73, 0x21, 0x3D, 0x1A, 0x05, 0xC9, 0xED,
 0xDF, 0x25, 0x17, 0x68, 0x86, 0xAE},
 AES_32B_KEY_SIZE, PSA_KEY_USAGE_DERIVE, PSA_ALG_HKDF(PSA_ALG_SHA_512),
 {0}, 0, {0}, 0, 64,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_MD5
{"Test psa_key_derivation to get 32 Byte data with MD-5\n", 3, PSA_KEY_TYPE_DERIVE,
{0xEA, 0xD5, 0xE6, 0xC8, 0x51, 0xF9, 0xEC, 0xBB, 0x9B, 0x57, 0x7C, 0xED, 0xD2,
 0x4B, 0x82, 0x84, 0x9F, 0x9F, 0xE6, 0x73, 0x21, 0x3D, 0x1A, 0x05, 0xC9, 0xED,
 0xDF, 0x25, 0x17, 0x68, 0x86, 0xAE},
 AES_32B_KEY_SIZE, PSA_KEY_USAGE_DERIVE, PSA_ALG_HKDF(PSA_ALG_MD5),
 {0}, 0, {0}, 0, 64,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_SHA256
{"Test psa_key_derivation to get 16 Byte data with salt and label\n", 4, PSA_KEY_TYPE_DERIVE,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_DERIVE, PSA_ALG_HKDF(PSA_ALG_SHA_256),
 "abcdefghijklmnop", 16, "This is a label", 15, 32,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_SHA1
{"Test psa_key_derivation with too large capacity for alg and key\n", 6, PSA_KEY_TYPE_DERIVE,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_DERIVE, PSA_ALG_HKDF(PSA_ALG_SHA_1),
 {0}, 0, {0}, 0, 5200,
 PSA_ERROR_INVALID_ARGUMENT
},
#endif

#ifdef ARCH_TEST_SHA256
{"Test psa_key_derivation with unsupported key type\n", 11, PSA_KEY_TYPE_AES,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_DERIVE, PSA_ALG_HKDF(PSA_ALG_SHA_256),
 {0}, 0, {0}, 0, 32,
 PSA_ERROR_INVALID_ARGUMENT
},

{"Test psa_key_derivation with incorrect usage\n", 12, PSA_KEY_TYPE_DERIVE,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_HKDF(PSA_ALG_SHA_256),
 {0}, 0, {0}, 0, 32,
 PSA_ERROR_NOT_PERMITTED
},
#endif
#endif

#ifdef ARCH_TEST_RSA_PKCS1V15_CRYPT
{"Test psa_key_derivation with invalid algorithm\n", 5, PSA_KEY_TYPE_DERIVE,
{0xEA, 0xD5, 0xE6, 0xC8, 0x51, 0xF9, 0xEC, 0xBB, 0x9B, 0x57, 0x7C, 0xED, 0xD2,
 0x4B, 0x82, 0x84, 0x9F, 0x9F, 0xE6, 0x73, 0x21, 0x3D, 0x1A, 0x05, 0xC9, 0xED,
 0xDF, 0x25, 0x17, 0x68, 0x86, 0xAE},
 AES_32B_KEY_SIZE, PSA_KEY_USAGE_DERIVE, PSA_ALG_RSA_PKCS1V15_CRYPT,
 {0}, 0, {0}, 0, 64,
 PSA_ERROR_INVALID_ARGUMENT
},
#endif

};

static test_data check2[] = {
#ifdef ARCH_TEST_HKDF
#ifdef ARCH_TEST_SHA512
{"Test psa_key_derivation to get 32 Byte data with SHA-512\n", 13, PSA_KEY_TYPE_DERIVE,
{0xEA, 0xD5, 0xE6, 0xC8, 0x51, 0xF9, 0xEC, 0xBB, 0x9B, 0x57, 0x7C, 0xED, 0xD2,
 0x4B, 0x82, 0x84, 0x9F, 0x9F, 0xE6, 0x73, 0x21, 0x3D, 0x1A, 0x05, 0xC9, 0xED,
 0xDF, 0x25, 0x17, 0x68, 0x86, 0xAE},
 AES_32B_KEY_SIZE, PSA_KEY_USAGE_DERIVE, PSA_ALG_HKDF(PSA_ALG_SHA_512),
 {0}, 0, {0}, 0, 64,
 PSA_SUCCESS
},
#endif
#endif

};
