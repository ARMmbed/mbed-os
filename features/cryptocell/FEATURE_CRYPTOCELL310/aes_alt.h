/*
 *  aes_alt.h
 *
 *  Copyright (C) 2019, Arm Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef __AES_ALT__
#define __AES_ALT__

#if defined(MBEDTLS_AES_ALT)
#include "ssi_aes.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    SaSiAesUserContext_t CC_Context;
    SaSiAesEncryptMode_t CC_cipherFlag;
    uint8_t CC_Key[SASI_AES_KEY_MAX_SIZE_IN_BYTES];
    size_t CC_keySizeInBytes;
}
mbedtls_aes_context;

#if defined(MBEDTLS_CIPHER_MODE_XTS)
/**
 * \brief The AES XTS context-type definition.
 */
typedef struct mbedtls_aes_xts_context
{
    int unsupported;
}
mbedtls_aes_xts_context;
#endif /* MBEDTLS_CIPHER_MODE_XTS */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_AES_ALT */
#endif /* __AES_ALT__ */

