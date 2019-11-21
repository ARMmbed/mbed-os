/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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

#include "utils.h"
#include "MbedCRC.h"

using namespace mbed;

uint32_t align_up(uint64_t val, uint64_t size)
{
    return (((val - 1) / size) + 1) * size;
}

uint32_t align_down(uint64_t val, uint64_t size)
{
    return (((val) / size)) * size;
}

uint32_t calc_crc(uint32_t init_crc, uint32_t data_size, const void *data_buf)
{
    uint32_t crc;
    MbedCRC<POLY_32BIT_ANSI, 32> ct(init_crc, 0x0, true, false);
    ct.compute(data_buf, data_size, &crc);
    return crc;
}
