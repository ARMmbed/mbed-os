/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
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

#include "randLIB_stub.h"


uint8_t randLIB_stub::uint8_value;
uint16_t randLIB_stub::uint16_value;
uint32_t randLIB_stub::uint32_value;
uint64_t randLIB_stub::uint64_value;
void* randLIB_stub::void_value;

extern "C"{

void randLIB_seed_random(void){}

void randLIB_add_seed(uint64_t seed){}

uint8_t randLIB_get_8bit(void)
{
    return randLIB_stub::uint8_value;
}

uint16_t randLIB_get_16bit(void)
{
    return randLIB_stub::uint16_value;
}

uint32_t randLIB_get_32bit(void)
{
    return randLIB_stub::uint32_value;
}

uint64_t randLIB_get_64bit(void)
{
    return randLIB_stub::uint64_value;
}

void *randLIB_get_n_bytes_random(void *data_ptr, uint8_t count)
{
    return randLIB_stub::void_value;
}

uint16_t randLIB_get_random_in_range(uint16_t min, uint16_t max)
{
    return randLIB_stub::uint16_value;
}

uint32_t randLIB_randomise_base(uint32_t base, uint16_t min_factor, uint16_t max_factor)
{
    return randLIB_stub::uint32_value;
}

}

