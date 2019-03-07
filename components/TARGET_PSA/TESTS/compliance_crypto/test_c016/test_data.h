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
    psa_key_usage_t         usage;
    psa_algorithm_t         key_alg;
    size_t                  bits;
    void                    *extra;
    size_t                  extra_size;
    uint32_t                expected_bit_length;
    uint32_t                expected_key_length;
    psa_status_t            expected_status;
} test_data;

#ifdef FUTURE_SUPPORT
static uint32_t rsa_extra = 3;
#endif

static test_data check1[] = {
#ifdef ARCH_TEST_CIPER_MODE_CTR
#ifdef ARCH_TEST_AES_128
{"Test psa_generate_key 16 Byte AES\n", 1, PSA_KEY_TYPE_AES,
 PSA_KEY_USAGE_EXPORT, PSA_ALG_CTR,
 BYTES_TO_BITS(AES_16B_KEY_SIZE),  NULL, 0,
 BYTES_TO_BITS(AES_16B_KEY_SIZE), AES_16B_KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_AES_192
{"Test psa_generate_key 24 Byte AES\n", 2, PSA_KEY_TYPE_AES,
 PSA_KEY_USAGE_EXPORT, PSA_ALG_CTR,
 BYTES_TO_BITS(AES_24B_KEY_SIZE),  NULL, 0,
 BYTES_TO_BITS(AES_24B_KEY_SIZE), AES_24B_KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_AES_256
{"Test psa_generate_key 32 Byte AES\n", 3, PSA_KEY_TYPE_AES,
 PSA_KEY_USAGE_EXPORT, PSA_ALG_CTR,
 BYTES_TO_BITS(AES_32B_KEY_SIZE),  NULL, 0,
 BYTES_TO_BITS(AES_32B_KEY_SIZE), AES_32B_KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_1KEY
{"Test psa_generate_key with DES 64 bit key\n", 4, PSA_KEY_TYPE_DES,
 PSA_KEY_USAGE_EXPORT, PSA_ALG_CTR,
 BYTES_TO_BITS(DES_8B_KEY_SIZE), NULL, 0,
 BYTES_TO_BITS(DES_8B_KEY_SIZE), DES_8B_KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_2KEY
{"Test psa_generate_key with Triple DES 2-Key\n", 5, PSA_KEY_TYPE_DES,
 PSA_KEY_USAGE_EXPORT, PSA_ALG_CTR,
 BYTES_TO_BITS(DES3_2KEY_SIZE), NULL, 0,
 BYTES_TO_BITS(DES3_2KEY_SIZE), DES3_2KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_3KEY
{"Test psa_generate_key with Triple DES 3-Key\n", 6, PSA_KEY_TYPE_DES,
 PSA_KEY_USAGE_EXPORT, PSA_ALG_CTR,
 BYTES_TO_BITS(DES3_3KEY_SIZE), NULL, 0,
 BYTES_TO_BITS(DES3_3KEY_SIZE), DES3_3KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_AES_256
{"Test psa_generate_key with Null extra and Non-Zero extra size\n", 7, PSA_KEY_TYPE_AES,
 PSA_KEY_USAGE_EXPORT, PSA_ALG_CTR,
 BYTES_TO_BITS(AES_32B_KEY_SIZE), NULL, sizeof(uint32_t),
 0, 0, PSA_ERROR_INVALID_ARGUMENT
},
#endif
#endif

#ifdef FUTURE_SUPPORT
{"Test psa_generate_key with RSA 2048 Keypair\n", 8, PSA_KEY_TYPE_RSA_KEYPAIR,
 PSA_KEY_USAGE_EXPORT | PSA_KEY_USAGE_SIGN | PSA_KEY_USAGE_VERIFY, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
 2048, NULL, 0,
 2048, 1193, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_ECC_CURVE_SECP224R1
#ifdef ARCH_TEST_ASYMMETRIC_ENCRYPTION
{"Test psa_generate_key with ECC KeyPair\n", 9,
 PSA_KEY_TYPE_ECC_KEYPAIR(PSA_ECC_CURVE_SECP224R1),
 PSA_KEY_USAGE_EXPORT, PSA_ALG_CATEGORY_ASYMMETRIC_ENCRYPTION,
 224, NULL, 0,
 224, 28, PSA_SUCCESS
},
#endif
#endif

#ifdef FUTURE_SUPPORT
{"Test psa_generate_key with Non-Null extra for 32 Byte AES key\n", 10, PSA_KEY_TYPE_AES,
 PSA_KEY_USAGE_EXPORT, PSA_ALG_CTR,
 BYTES_TO_BITS(AES_32B_KEY_SIZE), &rsa_extra, sizeof(uint32_t),
 0, 0, PSA_ERROR_INVALID_ARGUMENT
},
#endif

#ifdef ARCH_TEST_RSA_PKCS1V15_SIGN_RAW
#ifdef ARCH_TEST_RSA_2048
{"Test psa_generate_key with RSA 2048 Public key\n", 11, PSA_KEY_TYPE_RSA_PUBLIC_KEY,
 PSA_KEY_USAGE_EXPORT | PSA_KEY_USAGE_SIGN | PSA_KEY_USAGE_VERIFY, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
 2048, NULL, 0,
 2048, 1193, PSA_ERROR_NOT_SUPPORTED
},
#endif
#endif
};

static test_data check2[] = {
#ifdef ARCH_TEST_CIPER_MODE_CTR
#ifdef ARCH_TEST_AES_128
{"Test psa_generate_key negative cases\n", 12, PSA_KEY_TYPE_AES,
 PSA_KEY_USAGE_EXPORT, PSA_ALG_CTR,
 BYTES_TO_BITS(AES_16B_KEY_SIZE),  NULL, 0,
 BYTES_TO_BITS(AES_16B_KEY_SIZE), AES_16B_KEY_SIZE, PSA_SUCCESS
},
#endif
#endif
};
