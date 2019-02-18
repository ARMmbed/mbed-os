/**
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#include "lorawan_data_structures.h"

uint8_t *write_four_bytes(uint32_t value, uint8_t *ptr)
{
    if (!ptr) {
        return NULL;
    }
    *ptr++ = value;
    *ptr++ = value >> 8;
    *ptr++ = value >> 16;
    *ptr++ = value >> 24;
    return ptr;
}

uint32_t read_four_bytes(const uint8_t *data_buf)
{
    if (!data_buf) {
        return 0;
    }
    uint32_t temp_32;
    temp_32 =  *data_buf++;
    temp_32 += (uint32_t)(*data_buf++) << 8;
    temp_32 += (uint32_t)(*data_buf++) << 16;
    temp_32 += (uint32_t)(*data_buf++) << 24;
    return temp_32;
}

void count_set_bits(uint8_t mask, uint8_t &output)
{
    output = 0;

    while (mask) {
        mask &= (mask - 1);
        output++;
    }
}

