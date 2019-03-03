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
    uint8_t                 key_data[64];
    uint32_t                key_length;
    psa_key_usage_t         usage;
    psa_algorithm_t         key_alg;
    psa_status_t            expected_status;
} test_data;

static test_data check1[] = {
#ifdef ARCH_TEST_HMAC
#ifdef ARCH_TEST_SHA224
{"Test psa_mac_abort HMAC SHA 224\n", 1, PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
 PSA_KEY_USAGE_SIGN, PSA_ALG_HMAC(PSA_ALG_SHA_224),
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_SHA256
{"Test psa_mac_abort HMAC SHA 256\n", 2, PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
 PSA_KEY_USAGE_SIGN, PSA_ALG_HMAC(PSA_ALG_SHA_256),
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_SHA512
{"Test psa_mac_abort HMAC SHA 512\n", 3, PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
 PSA_KEY_USAGE_SIGN, PSA_ALG_HMAC(PSA_ALG_SHA_512),
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_SHA224
{"Test psa_mac_abort HMAC SHA 224 (truncated to 8 Byte)\n", 4, PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
 PSA_KEY_USAGE_SIGN, PSA_ALG_TRUNCATED_MAC(PSA_ALG_HMAC(PSA_ALG_SHA_224), 8),
 PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_AES_128
#ifdef ARCH_TEST_CMAC
{"Test psa_mac_abort CMAC AES 128\n", 5, PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, 16,
 PSA_KEY_USAGE_SIGN, PSA_ALG_CMAC,
 PSA_SUCCESS
},
#endif
#endif
};
