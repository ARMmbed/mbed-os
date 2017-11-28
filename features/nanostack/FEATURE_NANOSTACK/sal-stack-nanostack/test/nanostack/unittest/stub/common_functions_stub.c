/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "ns_types.h"
//#include "common_functions.h"
#define COMMON_FUNCTIONS_FN

#include <string.h>

#include "common_functions_stub.h"

common_functions_stub_def common_functions_stub;

bool bitsequal(const uint8_t *a, const uint8_t *b, uint_fast8_t bits)
{
    return common_functions_stub.bool_value;
}

uint8_t *bitcopy(uint8_t *restrict dst, const uint8_t *restrict src, uint_fast8_t bits)
{
    return common_functions_stub.uint8_ptr;
}

uint8_t *bitcopy0(uint8_t *restrict dst, const uint8_t *restrict src, uint_fast8_t bits)
{
    return common_functions_stub.uint8_ptr;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_64_bit(uint64_t value, uint8_t ptr[__static 8])
{
    return ptr + 8;
}

COMMON_FUNCTIONS_FN uint64_t common_read_64_bit(const uint8_t data_buf[__static 8])
{
    return common_functions_stub.uint64_value;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_32_bit(uint32_t value, uint8_t ptr[__static 4])
{
    return ptr + 4;
}

COMMON_FUNCTIONS_FN uint32_t common_read_32_bit(const uint8_t data_buf[__static 4])
{
    return common_functions_stub.uint32_value;
}

COMMON_FUNCTIONS_FN uint32_t common_read_16_bit_inverse(const uint8_t data_buf[__static 4])
{
    return common_functions_stub.uint16_value;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_32_bit_inverse(uint32_t value, uint8_t ptr[__static 4])
{
    return ptr + 4;
}

COMMON_FUNCTIONS_FN uint32_t common_read_32_bit_inverse(const uint8_t data_buf[__static 4])
{
    return common_functions_stub.uint32_value;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_24_bit(uint_fast24_t value, uint8_t ptr[__static 3])
{
    return ptr + 3;
}

COMMON_FUNCTIONS_FN uint_fast24_t common_read_24_bit(const uint8_t data_buf[__static 3])
{
    return common_functions_stub.uint24_value;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_16_bit(uint16_t value, uint8_t ptr[__static 2])
{
    return ptr + 2;
}

COMMON_FUNCTIONS_FN uint16_t common_read_16_bit(const uint8_t data_buf[__static 2])
{
    if (common_functions_stub.readuint16_from_queue) {
        common_functions_stub.readuint16_from_queue--;
        return common_functions_stub.uint16_value_queue[common_functions_stub.readuint16_from_queue];
    }
    return common_functions_stub.uint16_value;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_16_bit_inverse(uint16_t value, uint8_t ptr[__static 2])
{
    return ptr + 2;
}

COMMON_FUNCTIONS_FN uint_fast8_t common_count_bits(uint8_t byte)
{
    return common_functions_stub.uint8_value;
}

COMMON_FUNCTIONS_FN uint_fast8_t common_count_leading_zeros_8(uint8_t byte)
{
    return common_functions_stub.uint8_value;
}

COMMON_FUNCTIONS_FN uint_fast8_t common_count_leading_zeros_16(uint16_t value)
{
    return common_functions_stub.uint8_value;
}

COMMON_FUNCTIONS_FN uint_fast8_t common_count_leading_zeros_32(uint32_t value)
{
    return common_functions_stub.uint8_value;
}

COMMON_FUNCTIONS_FN bool common_serial_number_greater_8(uint8_t s1, uint8_t s2)
{
    return common_functions_stub.bool_value;
}

COMMON_FUNCTIONS_FN bool common_serial_number_greater_16(uint16_t s1, uint16_t s2)
{
    return common_functions_stub.bool_value;
}

COMMON_FUNCTIONS_FN bool common_serial_number_greater_32(uint32_t s1, uint32_t s2)
{
    return common_functions_stub.bool_value;
}

COMMON_FUNCTIONS_FN bool bit_test(const uint8_t *bitset, uint_fast8_t bit)
{
    return common_functions_stub.bool_value;
}

COMMON_FUNCTIONS_FN void bit_set(uint8_t *bitset, uint_fast8_t bit)
{
}

COMMON_FUNCTIONS_FN void bit_clear(uint8_t *bitset, uint_fast8_t bit)
{
}
