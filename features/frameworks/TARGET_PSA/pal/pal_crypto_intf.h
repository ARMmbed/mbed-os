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

#ifndef _PAL_CRYPTO_H_
#define _PAL_CRYPTO_H_

#include <stdarg.h>
#include "pal_common.h"

#if PSA_IPC_IMPLEMENTED
#include "psa/client.h"
#endif

#if PSA_CRYPTO_IMPLEMENTED
#include "psa/crypto.h"
#endif

enum crypto_function_code {
    PAL_CRYPTO_INIT                     = 0x1,
    PAL_CRYPTO_GENERATE_RANDOM          = 0x2,
    PAL_CRYPTO_IMPORT_KEY               = 0x3,
    PAL_CRYPTO_EXPORT_KEY               = 0x4,
    PAL_CRYPTO_EXPORT_PUBLIC_KEY        = 0x5,
    PAL_CRYPTO_DESTROY_KEY              = 0x6,
    PAL_CRYPTO_GET_KEY_INFO             = 0x7,
    PAL_CRYPTO_KEY_POLICY_INIT          = 0x8,
    PAL_CRYPTO_KEY_POLICY_SET_USAGE     = 0x9,
    PAL_CRYPTO_KEY_POLICY_GET_USAGE     = 0xA,
    PAL_CRYPTO_KEY_POLICY_GET_ALGORITHM = 0xB,
    PAL_CRYPTO_SET_KEY_POLICY           = 0xC,
    PAL_CRYPTO_GET_KEY_POLICY           = 0xD,
    PAL_CRYPTO_GET_KEY_INFORMATION      = 0xE,
    PAL_CRYPTO_GET_KEY_LIFETIME         = 0xF,
    PAL_CRYPTO_HASH_SETUP               = 0x11,
    PAL_CRYPTO_HASH_UPDATE              = 0x12,
    PAL_CRYPTO_HASH_VERIFY              = 0x13,
    PAL_CRYPTO_HASH_FINISH              = 0x14,
    PAL_CRYPTO_HASH_ABORT               = 0x15,
    PAL_CRYPTO_GENERATE_KEY             = 0x16,
    PAL_CRYPTO_GENERATOR_READ           = 0x17,
    PAL_CRYPTO_KEY_DERIVATION           = 0x18,
    PAL_CRYPTO_GET_GENERATOR_CAPACITY   = 0x19,
    PAL_CRYPTO_GENERATOR_IMPORT_KEY     = 0x1A,
    PAL_CRYPTO_GENERATOR_ABORT          = 0x1B,
    PAL_CRYPTO_AEAD_ENCRYPT             = 0x1C,
    PAL_CRYPTO_AEAD_DECRYPT             = 0x1D,
    PAL_CRYPTO_MAC_SIGN_SETUP           = 0x1E,
    PAL_CRYPTO_MAC_UPDATE               = 0x1F,
    PAL_CRYPTO_MAC_SIGN_FINISH          = 0x20,
    PAL_CRYPTO_MAC_VERIFY_SETUP         = 0x21,
    PAL_CRYPTO_MAC_VERIFY_FINISH        = 0x22,
    PAL_CRYPTO_MAC_ABORT                = 0x23,
    PAL_CRYPTO_ASYMMTERIC_ENCRYPT       = 0x24,
    PAL_CRYPTO_ASYMMTERIC_DECRYPT       = 0x25,
    PAL_CRYPTO_CIPHER_ENCRYPT_SETUP     = 0x26,
    PAL_CRYPTO_CIPHER_DECRYPT_SETUP     = 0x2A,
    PAL_CRYPTO_CIPHER_GENERATE_IV       = 0x2B,
    PAL_CRYPTO_CIPHER_SET_IV            = 0x2C,
    PAL_CRYPTO_CIPHER_UPDATE            = 0x2D,
    PAL_CRYPTO_CIPHER_FINISH            = 0x2E,
    PAL_CRYPTO_CIPHER_ABORT             = 0x2F,
    PAL_CRYPTO_ASYMMTERIC_SIGN          = 0x30,
    PAL_CRYPTO_ASYMMTERIC_VERIFY        = 0x31,
    PAL_CRYPTO_KEY_AGREEMENT            = 0x32,
    PAL_CRYPTO_ALLOCATE_KEY             = 0x33,
    PAL_CRYPTO_FREE                     = 0xFE,
};

int32_t pal_crypto_function(int type, va_list valist);
#endif /* _PAL_CRYPTO_H_ */
