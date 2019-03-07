/** @file
 * Copyright (c) 2018-2019, Arm Limited or its affiliates. All rights reserved.
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

#ifndef _VAL_CRYPTO_H_
#define _VAL_CRYPTO_H_

#include "val.h"
#include "psa/client.h"
#include "crypto_values.h"
#include "crypto.h"

#define BYTES_TO_BITS(byte)             (byte * 8)

/* Size */
#define AES_16B_KEY_SIZE                16
#define AES_24B_KEY_SIZE                24
#define AES_32B_KEY_SIZE                32
#define AES_18B_KEY_SIZE                18
#define AES_34B_KEY_SIZE                34
#define DES_8B_KEY_SIZE                 8
#define DES3_2KEY_SIZE                  16
#define DES3_3KEY_SIZE                  24
#define SIZE_32B                        32
#define SIZE_50B                        50
#define SIZE_128B                       128
#define SIZE_256B                       256
#define SIZE_512B                       512
#define BUFFER_SIZE                     1200
#define HASH_64B                        64
#define BUFFER_SIZE_HIGH                5130

/* Key Slot */
#define INVALID_KEY_SLOT                0xDEAD
#define ZERO_KEY_SLOT                   0
#define OCCUPIED_KEY_SLOT               1
#define MAX_KEY_SLOT                    32

#define PSA_KEY_LIFETIME_INVALID        0xFFFFFFFF
#define PSA_KEY_USAGE_INVALID           0xFFFFFFFF
#define PSA_ALG_INVALID                 0xFFFFFFFF

enum crypto_function_code {
    VAL_CRYPTO_INIT                     = 0x1,
    VAL_CRYPTO_GENERATE_RANDOM          = 0x2,
    VAL_CRYPTO_IMPORT_KEY               = 0x3,
    VAL_CRYPTO_EXPORT_KEY               = 0x4,
    VAL_CRYPTO_EXPORT_PUBLIC_KEY        = 0x5,
    VAL_CRYPTO_DESTROY_KEY              = 0x6,
    VAL_CRYPTO_GET_KEY_INFO             = 0x7,
    VAL_CRYPTO_KEY_POLICY_INIT          = 0x8,
    VAL_CRYPTO_KEY_POLICY_SET_USAGE     = 0x9,
    VAL_CRYPTO_KEY_POLICY_GET_USAGE     = 0xA,
    VAL_CRYPTO_KEY_POLICY_GET_ALGORITHM = 0xB,
    VAL_CRYPTO_SET_KEY_POLICY           = 0xC,
    VAL_CRYPTO_GET_KEY_POLICY           = 0xD,
    VAL_CRYPTO_GET_KEY_INFORMATION      = 0xE,
    VAL_CRYPTO_GET_KEY_LIFETIME         = 0xF,
    VAL_CRYPTO_HASH_SETUP               = 0x11,
    VAL_CRYPTO_HASH_UPDATE              = 0x12,
    VAL_CRYPTO_HASH_VERIFY              = 0x13,
    VAL_CRYPTO_HASH_FINISH              = 0x14,
    VAL_CRYPTO_HASH_ABORT               = 0x15,
    VAL_CRYPTO_GENERATE_KEY             = 0x16,
    VAL_CRYPTO_GENERATOR_READ           = 0x17,
    VAL_CRYPTO_KEY_DERIVATION           = 0x18,
    VAL_CRYPTO_GET_GENERATOR_CAPACITY   = 0x19,
    VAL_CRYPTO_GENERATOR_IMPORT_KEY     = 0x1A,
    VAL_CRYPTO_GENERATOR_ABORT          = 0x1B,
    VAL_CRYPTO_AEAD_ENCRYPT             = 0x1C,
    VAL_CRYPTO_AEAD_DECRYPT             = 0x1D,
    VAL_CRYPTO_MAC_SIGN_SETUP           = 0x1E,
    VAL_CRYPTO_MAC_UPDATE               = 0x1F,
    VAL_CRYPTO_MAC_SIGN_FINISH          = 0x20,
    VAL_CRYPTO_MAC_VERIFY_SETUP         = 0x21,
    VAL_CRYPTO_MAC_VERIFY_FINISH        = 0x22,
    VAL_CRYPTO_MAC_ABORT                = 0x23,
    VAL_CRYPTO_ASYMMTERIC_ENCRYPT       = 0x24,
    VAL_CRYPTO_ASYMMTERIC_DECRYPT       = 0x25,
    VAL_CRYPTO_CIPHER_ENCRYPT_SETUP     = 0x26,
    VAL_CRYPTO_CIPHER_DECRYPT_SETUP     = 0x2A,
    VAL_CRYPTO_CIPHER_GENERATE_IV       = 0x2B,
    VAL_CRYPTO_CIPHER_SET_IV            = 0x2C,
    VAL_CRYPTO_CIPHER_UPDATE            = 0x2D,
    VAL_CRYPTO_CIPHER_FINISH            = 0x2E,
    VAL_CRYPTO_CIPHER_ABORT             = 0x2F,
    VAL_CRYPTO_ASYMMTERIC_SIGN          = 0x30,
    VAL_CRYPTO_ASYMMTERIC_VERIFY        = 0x31,
    VAL_CRYPTO_KEY_AGREEMENT            = 0x32,
    VAL_CRYPTO_ALLOCATE_KEY             = 0x33,
    VAL_CRYPTO_FREE                     = 0xFE,
};

int32_t val_crypto_function(int type, ...);
#endif /* _VAL_CRYPTO_H_ */
