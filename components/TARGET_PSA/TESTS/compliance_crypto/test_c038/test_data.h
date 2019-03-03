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
    psa_status_t            expected_status;
} test_data;

static test_data check1[] = {
#ifdef ARCH_TEST_AES_128
#ifdef ARCH_TEST_CBC_NO_PADDING
{"Test psa_cipher_abort - Encrypt - AES CBC_NO_PADDING\n", 1, PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_CBC_PKCS7
{"Test psa_cipher_abort - Encrypt - AES CBC_PKCS7\n", 2, PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_PKCS7,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_CIPER_MODE_CTR
{"Test psa_cipher_abort - Encrypt - AES CTR\n", 3, PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CTR,
 PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_CBC_NO_PADDING
#ifdef ARCH_TEST_DES_1KEY
{"Test psa_cipher_abort - Encrypt - DES CBC (nopad)\n", 4, PSA_KEY_TYPE_DES,
{0x01, 0x02, 0x04, 0x07, 0x08, 0x0b, 0x0d, 0x0e}, DES_8B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_2KEY
{"Test psa_cipher_abort - Encrypt - 2-key 3DE -CBC (nopad)\n", 5, PSA_KEY_TYPE_DES,
{0x01, 0x02, 0x04, 0x07, 0x08, 0x0b, 0x0d, 0x0e, 0xc1, 0xc2, 0xc4, 0xc7, 0xc8,
 0xcb, 0xcd, 0xce}, DES3_2KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_3KEY
{"Test psa_cipher_abort - Encrypt - 3-key 3DE -CBC (nopad)\n", 6, PSA_KEY_TYPE_DES,
{0x01, 0x02, 0x04, 0x07, 0x08, 0x0b, 0x0d, 0x0e, 0xc1, 0xc2, 0xc4, 0xc7, 0xc8,
 0xcb, 0xcd, 0xce, 0x31, 0x32, 0x34, 0x37, 0x38, 0x3b, 0x3d, 0x3e}, DES3_3KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
 PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_AES_128
#ifdef ARCH_TEST_CBC_NO_PADDING
{"Test psa_cipher_abort - Decrypt - AES CBC_NO_PADDING\n", 7, PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_NO_PADDING,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_CBC_PKCS7
{"Test psa_cipher_abort - Decrypt - AES CBC_PKCS7\n", 8, PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_PKCS7,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_CIPER_MODE_CTR
{"Test psa_cipher_abort - Decrypt - AES CTR\n", 9, PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_DECRYPT, PSA_ALG_CTR,
 PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_CBC_NO_PADDING
#ifdef ARCH_TEST_DES_1KEY
{"Test psa_cipher_abort - Decrypt - DES CBC (nopad)\n", 10, PSA_KEY_TYPE_DES,
{0x01, 0x02, 0x04, 0x07, 0x08, 0x0b, 0x0d, 0x0e}, DES_8B_KEY_SIZE,
 PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_NO_PADDING,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_2KEY
{"Test psa_cipher_abort - Decrypt - 2-key 3DE -CBC (nopad)\n", 11, PSA_KEY_TYPE_DES,
{0x01, 0x02, 0x04, 0x07, 0x08, 0x0b, 0x0d, 0x0e, 0xc1, 0xc2, 0xc4, 0xc7, 0xc8,
 0xcb, 0xcd, 0xce}, DES3_2KEY_SIZE,
 PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_NO_PADDING,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_3KEY
{"Test psa_cipher_abort - Decrypt - 3-key 3DE -CBC (nopad)\n", 12, PSA_KEY_TYPE_DES,
{0x01, 0x02, 0x04, 0x07, 0x08, 0x0b, 0x0d, 0x0e, 0xc1, 0xc2, 0xc4, 0xc7, 0xc8,
 0xcb, 0xcd, 0xce, 0x31, 0x32, 0x34, 0x37, 0x38, 0x3b, 0x3d, 0x3e}, DES3_3KEY_SIZE,
 PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_NO_PADDING,
 PSA_SUCCESS
},
#endif
#endif
};
