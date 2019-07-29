/*
 *  cmac_alt.h
 *
 *  Copyright (C) 2019, ARM Limited, All Rights Reserved
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

#ifndef __CMAC_ALT__
#define __CMAC_ALT__

#if defined(MBEDTLS_CMAC_ALT)
#include "ssi_aes.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct mbedtls_cmac_context_t
{
    SaSiAesUserContext_t CC_Context;
    uint8_t CC_Key[SASI_AES_KEY_MAX_SIZE_IN_BYTES];
    size_t CC_keySizeInBytes;
    /** Unprocessed data - either data that was not block aligned and is still
     *  pending processing, or the final block */
    unsigned char       unprocessed_block[SASI_AES_BLOCK_SIZE_IN_BYTES];

    /** The length of data pending processing. */
    size_t              unprocessed_len;
    
    int                 is_cc_initiated;
} mbedtls_cmac_context_t;

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_CMAC_ALT */
#endif /* __CMAC_ALT__ */
