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

#include "FlashSimBlockDevice.h"
#include "mbed_assert.h"
#include "mbed_critical.h"
#include <algorithm>
#include <stdlib.h>
#include <string.h>

static const bd_size_t min_blank_buf_size = 32;

static inline uint32_t align_up(bd_size_t val, bd_size_t size)
{
    return (((val - 1) / size) + 1) * size;
}

FlashSimBlockDevice::FlashSimBlockDevice(BlockDevice *bd, uint8_t erase_value)
{
}

FlashSimBlockDevice::~FlashSimBlockDevice()
{
}

int FlashSimBlockDevice::init()
{
    return 0;
}

int FlashSimBlockDevice::deinit()
{
    return 0;
}

int FlashSimBlockDevice::sync()
{
    return 0;
}

bd_size_t FlashSimBlockDevice::get_read_size() const
{
    return 0;
}

bd_size_t FlashSimBlockDevice::get_program_size() const
{
    return 0;
}

bd_size_t FlashSimBlockDevice::get_erase_size() const
{
    return 0;
}

bd_size_t FlashSimBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return 0;
}

bd_size_t FlashSimBlockDevice::size() const
{
    return 0;
}

int FlashSimBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int FlashSimBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int FlashSimBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int FlashSimBlockDevice::get_erase_value() const
{
    return 0;
}
