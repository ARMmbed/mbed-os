/** @file
 * Copyright (c) 2018, Arm Limited or its affiliates. All rights reserved.
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
    size_t                  key_length;
    psa_status_t            expected_status;
} test_data;

static test_data check1[] = {
#ifdef ARCH_TEST_AES_128
{"Test psa_allocate_key 16 Byte AES\n", 1, PSA_KEY_TYPE_AES,
 AES_16B_KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_AES_192
{"Test psa_allocate_key 24 Byte AES\n", 2, PSA_KEY_TYPE_AES,
 AES_24B_KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_AES_256
{"Test psa_allocate_key 32 Byte AES\n", 3, PSA_KEY_TYPE_AES,
 AES_32B_KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_RSA_2048
{"Test psa_allocate_key 2048 RSA public key\n", 4, PSA_KEY_TYPE_RSA_PUBLIC_KEY,
 294, PSA_SUCCESS
},

{"Test psa_allocate_key with RSA 2048 keypair\n", 5, PSA_KEY_TYPE_RSA_KEYPAIR,
 1193, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_1KEY
{"Test psa_allocate_key with DES 64 bit key\n", 6, PSA_KEY_TYPE_DES,
 DES_8B_KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_2KEY
{"Test psa_allocate_key with Triple DES 2-Key\n", 7, PSA_KEY_TYPE_DES,
 DES3_2KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_3KEY
{"Test psa_allocate_key with Triple DES 3-Key\n", 8, PSA_KEY_TYPE_DES,
 DES3_3KEY_SIZE, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_ECC_CURVE_SECP192R1
{"Test psa_allocate_key with EC Public key\n", 9,
 PSA_KEY_TYPE_ECC_PUBLIC_KEY_BASE | PSA_ECC_CURVE_SECP192R1,
 75, PSA_SUCCESS
},

{"Test psa_allocate_key with EC keypair\n", 10,
 PSA_KEY_TYPE_ECC_KEYPAIR_BASE | PSA_ECC_CURVE_SECP192R1,
 97, PSA_SUCCESS
},
#endif
};
