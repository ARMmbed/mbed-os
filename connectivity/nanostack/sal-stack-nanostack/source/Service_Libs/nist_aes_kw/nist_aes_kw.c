/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_NIST_KW_C) && defined(HAVE_WS) && (defined(HAVE_PAE_SUPP) || defined(HAVE_PAE_AUTH))
#include "mbedtls/nist_kw.h"
#endif

#include "Service_Libs/nist_aes_kw/nist_aes_kw.h"

#ifdef HAVE_WS

#define TRACE_GROUP "naes"

int8_t nist_aes_key_wrap(uint8_t is_wrap, const uint8_t *key, int16_t key_bits, const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len)
{
#if defined(MBEDTLS_NIST_KW_C) && (defined(HAVE_PAE_SUPP) || defined(HAVE_PAE_AUTH))

    int8_t ret_val = 0;
    mbedtls_nist_kw_context ctx;

#ifdef EXTRA_DEBUG_INFO
    const uint8_t *print_data = key;
    uint16_t print_data_len = key_bits / 8;
    while (true) {
        tr_debug("nist_aes_key_wrap key %s\n", trace_array(print_data, print_data_len > 32 ? 32 : print_data_len));
        if (print_data_len > 32) {
            print_data_len -= 32;
            print_data += 32;
        } else {
            break;
        }
    }

    print_data = input;
    print_data_len = input_len;
    while (true) {
        tr_debug("nist_aes_key_wrap in %s\n", trace_array(print_data, print_data_len > 32 ? 32 : print_data_len));
        if (print_data_len > 32) {
            print_data_len -= 32;
            print_data += 32;
        } else {
            break;
        }
    }
#endif

    mbedtls_nist_kw_init(&ctx);

    if (mbedtls_nist_kw_setkey(&ctx, MBEDTLS_CIPHER_ID_AES, key, key_bits, is_wrap) != 0) {
        ret_val = -1;
        goto error;
    }

    size_t out_size = *output_len;

    if (is_wrap) {
        if (mbedtls_nist_kw_wrap(&ctx, MBEDTLS_KW_MODE_KW, input, input_len, output, output_len, out_size) != 0) {
            ret_val = -1;
            goto error;
        }
    } else {
        if (mbedtls_nist_kw_unwrap(&ctx, MBEDTLS_KW_MODE_KW, input, input_len, output, output_len, out_size) != 0) {
            ret_val = -1;
            goto error;
        }
    }

#ifdef EXTRA_DEBUG_INFO
    print_data = output;
    print_data_len = *output_len;
    while (true) {
        tr_debug("nist_aes_key_wrap out %s\n", trace_array(print_data, print_data_len > 32 ? 32 : print_data_len));
        if (print_data_len > 32) {
            print_data_len -= 32;
            print_data += 32;
        } else {
            break;
        }
    }
#endif

error:
    mbedtls_nist_kw_free(&ctx);

    return ret_val;
#else
    (void) is_wrap;
    (void) key;
    (void) key_bits;
    (void) input;
    (void) input_len;
    (void) output;
    (void) output_len;
    return 0;
#endif
}

#endif
