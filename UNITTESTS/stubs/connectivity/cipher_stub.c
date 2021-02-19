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

#include "mbedtls/cipher.h"
#include "mbedtls/cipher_internal.h"
#include "mbedtls/platform_util.h"

#include <stdlib.h>
#include <string.h>

#if defined(MBEDTLS_GCM_C)
#include "mbedtls/gcm.h"
#endif

#if defined(MBEDTLS_CCM_C)
#include "mbedtls/ccm.h"
#endif

#if defined(MBEDTLS_CMAC_C)
#include "mbedtls/cmac.h"
#endif

#include "cipher_stub.h"

cipher_stub_def cipher_stub;

const int *mbedtls_cipher_list(void)
{
    return cipher_stub.int_ptr;
}

const mbedtls_cipher_info_t *mbedtls_cipher_info_from_type(const mbedtls_cipher_type_t cipher_type)
{
    return cipher_stub.info_value;
}

const mbedtls_cipher_info_t *mbedtls_cipher_info_from_string(const char *cipher_name)
{
    return cipher_stub.info_value;
}

const mbedtls_cipher_info_t *mbedtls_cipher_info_from_values(const mbedtls_cipher_id_t cipher_id,
                                                             int key_bitlen,
                                                             const mbedtls_cipher_mode_t mode)
{
    return cipher_stub.info_value;
}

void mbedtls_cipher_init(mbedtls_cipher_context_t *ctx)
{
}

void mbedtls_cipher_free(mbedtls_cipher_context_t *ctx)
{
}

int mbedtls_cipher_setup(mbedtls_cipher_context_t *ctx, const mbedtls_cipher_info_t *cipher_info)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_setkey(mbedtls_cipher_context_t *ctx, const unsigned char *key,
                          int key_bitlen, const mbedtls_operation_t operation)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_set_iv(mbedtls_cipher_context_t *ctx,
                          const unsigned char *iv, size_t iv_len)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_reset(mbedtls_cipher_context_t *ctx)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_update_ad(mbedtls_cipher_context_t *ctx,
                             const unsigned char *ad, size_t ad_len)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_update(mbedtls_cipher_context_t *ctx, const unsigned char *input,
                          size_t ilen, unsigned char *output, size_t *olen)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_finish(mbedtls_cipher_context_t *ctx,
                          unsigned char *output, size_t *olen)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_set_padding_mode(mbedtls_cipher_context_t *ctx, mbedtls_cipher_padding_t mode)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_write_tag(mbedtls_cipher_context_t *ctx,
                             unsigned char *tag, size_t tag_len)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_check_tag(mbedtls_cipher_context_t *ctx,
                             const unsigned char *tag, size_t tag_len)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_crypt(mbedtls_cipher_context_t *ctx,
                         const unsigned char *iv, size_t iv_len,
                         const unsigned char *input, size_t ilen,
                         unsigned char *output, size_t *olen)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_auth_encrypt(mbedtls_cipher_context_t *ctx,
                                const unsigned char *iv, size_t iv_len,
                                const unsigned char *ad, size_t ad_len,
                                const unsigned char *input, size_t ilen,
                                unsigned char *output, size_t *olen,
                                unsigned char *tag, size_t tag_len)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

int mbedtls_cipher_auth_decrypt(mbedtls_cipher_context_t *ctx,
                                const unsigned char *iv, size_t iv_len,
                                const unsigned char *ad, size_t ad_len,
                                const unsigned char *input, size_t ilen,
                                unsigned char *output, size_t *olen,
                                const unsigned char *tag, size_t tag_len)
{
    if (cipher_stub.int_zero_counter) {
        cipher_stub.int_zero_counter--;
        return 0;
    }
    return cipher_stub.int_value;
}

