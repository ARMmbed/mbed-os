/*
 * Copyright (c) , Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif


#include "mbedtls/cmac.h"
#include "mbedtls/platform_util.h"
#include <string.h>

#include "cmac_stub.h"

cmac_stub_def cmac_stub;

int mbedtls_cipher_cmac_starts(mbedtls_cipher_context_t *ctx,
                               const unsigned char *key, size_t keybits)
{
    if (cmac_stub.int_zero_counter) {
        cmac_stub.int_zero_counter--;
        return 0;
    }
    return cmac_stub.int_value;
}

int mbedtls_cipher_cmac_update(mbedtls_cipher_context_t *ctx,
                               const unsigned char *input, size_t ilen)
{
    if (cmac_stub.int_zero_counter) {
        cmac_stub.int_zero_counter--;
        return 0;
    }
    return cmac_stub.int_value;
}

int mbedtls_cipher_cmac_finish(mbedtls_cipher_context_t *ctx,
                               unsigned char *output)
{
    if (cmac_stub.int_zero_counter) {
        cmac_stub.int_zero_counter--;
        return 0;
    }
    return cmac_stub.int_value;
}

int mbedtls_cipher_cmac_reset(mbedtls_cipher_context_t *ctx)
{
    if (cmac_stub.int_zero_counter) {
        cmac_stub.int_zero_counter--;
        return 0;
    }
    return cmac_stub.int_value;
}

int mbedtls_cipher_cmac(const mbedtls_cipher_info_t *cipher_info,
                        const unsigned char *key, size_t keylen,
                        const unsigned char *input, size_t ilen,
                        unsigned char *output)
{
    if (cmac_stub.int_zero_counter) {
        cmac_stub.int_zero_counter--;
        return 0;
    }
    return cmac_stub.int_value;
}

int mbedtls_aes_cmac_prf_128(const unsigned char *key, size_t key_length,
                             const unsigned char *input, size_t in_len,
                             unsigned char *output)
{
    if (cmac_stub.int_zero_counter) {
        cmac_stub.int_zero_counter--;
        return 0;
    }
    return cmac_stub.int_value;
}

