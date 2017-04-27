/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Most functions can be inlined, and definitions are in common_functions.h.
 * Define COMMON_FUNCTIONS_FN before including it to generate external definitions.
 */
#define COMMON_FUNCTIONS_FN extern

#include "common_functions.h"

#include <string.h>

/* Returns mask for <split_value> (0-8) most-significant bits of a byte */
static inline uint8_t context_split_mask(uint_fast8_t split_value)
{
    return (uint8_t) - (0x100u >> split_value);
}

bool bitsequal(const uint8_t *a, const uint8_t *b, uint_fast8_t bits)
{
    uint_fast8_t bytes = bits / 8;
    bits %= 8;

    if (memcmp(a, b, bytes)) {
        return false;
    }

    if (bits) {
        uint_fast8_t split_bit = context_split_mask(bits);
        if ((a[bytes] & split_bit) != (b[bytes] & split_bit)) {
            return false;
        }
    }

    return true;
}

uint8_t *bitcopy(uint8_t *restrict dst, const uint8_t *restrict src, uint_fast8_t bits)
{
    uint_fast8_t bytes = bits / 8;
    bits %= 8;

    if (bytes) {
        dst = (uint8_t *) memcpy(dst, src, bytes) + bytes;
        src += bytes;
    }

    if (bits) {
        uint_fast8_t split_bit = context_split_mask(bits);
        *dst = (*src & split_bit) | (*dst & ~ split_bit);
    }

    return dst;
}

uint8_t *bitcopy0(uint8_t *restrict dst, const uint8_t *restrict src, uint_fast8_t bits)
{
    uint_fast8_t bytes = bits / 8;
    bits %= 8;

    if (bytes) {
        dst = (uint8_t *) memcpy(dst, src, bytes) + bytes;
        src += bytes;
    }

    if (bits) {
        uint_fast8_t split_bit = context_split_mask(bits);
        *dst = (*src & split_bit);
    }

    return dst;
}
