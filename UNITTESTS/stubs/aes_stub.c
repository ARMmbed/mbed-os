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

#include <string.h>

#include "mbedtls/aes.h"

#include "aes_stub.h"
aes_stub_def aes_stub;


void mbedtls_aes_init(mbedtls_aes_context *ctx)
{
}

void mbedtls_aes_free(mbedtls_aes_context *ctx)
{
}

#if defined(MBEDTLS_CIPHER_MODE_XTS)
void mbedtls_aes_xts_init(mbedtls_aes_xts_context *ctx)
{
}

void mbedtls_aes_xts_free(mbedtls_aes_xts_context *ctx)
{
}
#endif

#if !defined(MBEDTLS_AES_SETKEY_ENC_ALT)
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}
#endif

#if !defined(MBEDTLS_AES_SETKEY_DEC_ALT)
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}
#endif

#if defined(MBEDTLS_CIPHER_MODE_XTS)
int mbedtls_aes_xts_setkey_enc(mbedtls_aes_xts_context *ctx,
                               const unsigned char *key,
                               unsigned int keybits)
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}

int mbedtls_aes_xts_setkey_dec(mbedtls_aes_xts_context *ctx,
                               const unsigned char *key,
                               unsigned int keybits)
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}
#endif

int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}

void mbedtls_aes_encrypt(mbedtls_aes_context *ctx,
                         const unsigned char input[16],
                         unsigned char output[16])
{
}

int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}

void mbedtls_aes_decrypt(mbedtls_aes_context *ctx,
                         const unsigned char input[16],
                         unsigned char output[16])
{
}

int mbedtls_aes_crypt_ecb(mbedtls_aes_context *ctx,
                          int mode,
                          const unsigned char input[16],
                          unsigned char output[16])
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}

int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}


#if defined(MBEDTLS_CIPHER_MODE_XTS)
int mbedtls_aes_crypt_xts(mbedtls_aes_xts_context *ctx,
                          int mode,
                          size_t length,
                          const unsigned char data_unit[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}
#endif

int mbedtls_aes_crypt_cfb128(mbedtls_aes_context *ctx,
                             int mode,
                             size_t length,
                             size_t *iv_off,
                             unsigned char iv[16],
                             const unsigned char *input,
                             unsigned char *output)
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}

int mbedtls_aes_crypt_cfb8(mbedtls_aes_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output)
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}

int mbedtls_aes_crypt_ofb(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *iv_off,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}

int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *nc_off,
                          unsigned char nonce_counter[16],
                          unsigned char stream_block[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    if (aes_stub.int_zero_counter) {
        aes_stub.int_zero_counter--;
        return 0;
    }
    return aes_stub.int_value;
}
