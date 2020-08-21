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
#include "mbedtls/md.h"
#include "Service_Libs/ieee_802_11/ieee_802_11.h"
#include "Service_Libs/hmac/hmac_sha1.h"

#define TRACE_GROUP "iprf"

uint16_t ieee_802_11_prf_setup(ieee_802_11_prf_t *prf, uint16_t bits, uint16_t a_len, uint16_t b_len)
{
    prf->bits = bits;
    prf->a_len = a_len;
    prf->b_len = b_len;
    return a_len + 1 + b_len + 1; // A string + Y + B string + X (index)
}

uint8_t *ieee_802_11_prf_get_a_string(ieee_802_11_prf_t *prf, uint8_t *string)
{
    (void) prf;
    return string;
}

uint8_t *ieee_802_11_prf_get_b_string(ieee_802_11_prf_t *prf, uint8_t *string)
{
    return string + prf->a_len + 1;
}

uint16_t ieee_802_11_prf_starts(ieee_802_11_prf_t *prf, const uint8_t *key, uint16_t key_len)
{
    prf->key = key;
    prf->key_len = key_len;

    uint8_t iterations = (prf->bits + 159) / 160;
    uint16_t result_len = 160 / 8 * iterations;
    return result_len;
}

void ieee_802_11_prf_update(ieee_802_11_prf_t *prf, uint8_t *string)
{
    prf->string = string;
    prf->string[prf->a_len] = 0x00; /* Y (0) */
}

int8_t ieee_802_11_prf_finish(ieee_802_11_prf_t *prf, uint8_t *result)
{
    uint16_t string_len = prf->a_len + 1 + prf->b_len + 1;

    for (uint8_t i = 0; i < (prf->bits + 159) / 160; i++) {
        prf->string[prf->a_len + 1 + prf->b_len] = i; /* X (index) */
        if (hmac_sha1_calc(prf->key, prf->key_len, prf->string, string_len, result, 20) < 0) {
            return -1;
        }
        result += 160 / 8;
    }

    return 0;
}
