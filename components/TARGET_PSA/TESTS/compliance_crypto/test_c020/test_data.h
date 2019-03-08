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
    psa_key_handle_t        key_handle[4];
    psa_key_type_t          key_type[2];
    uint8_t                 key_data[32];
    uint32_t                key_length;
    psa_key_usage_t         usage[2];
    psa_algorithm_t         key_alg[2];
    size_t                  capacity;
    size_t                  size;
    psa_status_t            expected_status;
} test_data;

static test_data check1[] = {
#ifdef ARCH_TEST_HKDF
#ifdef ARCH_TEST_CIPER_MODE_CTR
#ifdef ARCH_TEST_AES_128
#ifdef ARCH_TEST_SHA256
{"Test psa_generator_import_key for 16 Byte AES Key\n", {1, 2, 3, 4},
{PSA_KEY_TYPE_DERIVE, PSA_KEY_TYPE_AES},
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, {PSA_KEY_USAGE_DERIVE, PSA_KEY_USAGE_EXPORT},
 {PSA_ALG_HKDF(PSA_ALG_SHA_256), PSA_ALG_CTR},
 32, 16, PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_SHA512
#ifdef ARCH_TEST_DES_3KEY
{"Test psa_generator_import_key for Triple DES 3-Key\n", {5, 6, 7, 8},
{PSA_KEY_TYPE_DERIVE, PSA_KEY_TYPE_DES},
{0xEA, 0xD5, 0xE6, 0xC8, 0x51, 0xF9, 0xEC, 0xBB, 0x9B, 0x57, 0x7C, 0xED, 0xD2,
 0x4B, 0x82, 0x84, 0x9F, 0x9F, 0xE6, 0x73, 0x21, 0x3D, 0x1A, 0x05, 0xC9, 0xED,
 0xDF, 0x25, 0x17, 0x68, 0x86, 0xAE},
 AES_32B_KEY_SIZE, {PSA_KEY_USAGE_DERIVE, PSA_KEY_USAGE_EXPORT},
{PSA_ALG_HKDF(PSA_ALG_SHA_512), PSA_ALG_CTR},
 DES3_3KEY_SIZE, DES3_3KEY_SIZE, PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_SHA1
#ifdef ARCH_TEST_AES
{"Test psa_generator_import_key output greater than capacity\n", {9, 10, 11, 12},
{PSA_KEY_TYPE_DERIVE, PSA_KEY_TYPE_AES},
{0x70, 0x24, 0x55, 0x0C, 0x14, 0x9D, 0xED, 0x29},
 DES_8B_KEY_SIZE, {PSA_KEY_USAGE_DERIVE, PSA_KEY_USAGE_EXPORT},
{PSA_ALG_HKDF(PSA_ALG_SHA_1), PSA_ALG_CTR},
 64, 80, PSA_ERROR_INSUFFICIENT_CAPACITY
},
#endif
#endif

#ifdef ARCH_TEST_SHA256
#ifdef ARCH_TEST_RSA
{"Test psa_generator_import_key for RSA Public Key - Invalid type\n", {13, 14, 15, 16},
{PSA_KEY_TYPE_DERIVE, PSA_KEY_TYPE_RSA_PUBLIC_KEY},
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, {PSA_KEY_USAGE_DERIVE, PSA_KEY_USAGE_EXPORT},
{PSA_ALG_HKDF(PSA_ALG_SHA_256), PSA_ALG_CTR},
 32, 16, PSA_ERROR_INVALID_ARGUMENT
},
#endif
#endif

#ifdef ARCH_TEST_SHA256
#ifdef ARCH_TEST_AES_128
{"Test psa_generator_import_key for invalid byte for generation\n", {20, 21, 22, 23},
{PSA_KEY_TYPE_DERIVE, PSA_KEY_TYPE_AES},
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, {PSA_KEY_USAGE_DERIVE, PSA_KEY_USAGE_EXPORT},
{PSA_ALG_HKDF(PSA_ALG_SHA_256), PSA_ALG_CTR},
 32, 10, PSA_ERROR_INVALID_ARGUMENT
},
#endif
#endif
#endif
#endif
};

static test_data check2[] = {
#ifdef ARCH_TEST_HKDF
#ifdef ARCH_TEST_CIPER_MODE_CTR
#ifdef ARCH_TEST_AES_128
#ifdef ARCH_TEST_SHA256
{"Test psa_generator_import_key negative cases\n",
{17, 18, 18, 18},
{PSA_KEY_TYPE_DERIVE, PSA_KEY_TYPE_AES},
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, {PSA_KEY_USAGE_DERIVE, PSA_KEY_USAGE_EXPORT},
{PSA_ALG_HKDF(PSA_ALG_SHA_256), PSA_ALG_CTR},
 32, BYTES_TO_BITS(8), PSA_ERROR_INVALID_HANDLE
},
#endif
#endif
#endif
#endif
};
