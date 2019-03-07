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
    uint8_t                 salt[16];
    size_t                  salt_length;
    uint8_t                 label[16];
    size_t                  label_length;
    size_t                  capacity;
    size_t                  size;
    psa_status_t            expected_status;
} test_data;

static test_data check1[] = {
#ifdef ARCH_TEST_HKDF
#ifdef ARCH_TEST_CIPER_MODE_CTR
#ifdef ARCH_TEST_SHA256
{"Test psa_generator_abort\n", 1, PSA_KEY_TYPE_DERIVE,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_DERIVE, PSA_ALG_HKDF(PSA_ALG_SHA_256),
 {0}, 0, {0}, 0, 32,
 16, PSA_SUCCESS
},
#endif
#endif
#endif
};
