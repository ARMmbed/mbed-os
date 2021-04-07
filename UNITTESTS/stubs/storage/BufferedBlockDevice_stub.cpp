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

#include "BufferedBlockDevice.h"
#include "mbed_assert.h"
#include "mbed_critical.h"
#include <algorithm>
#include <string.h>

static inline uint32_t align_down(bd_size_t val, bd_size_t size)
{
    return 0;
}

BufferedBlockDevice::BufferedBlockDevice(BlockDevice *bd)
{
}

BufferedBlockDevice::~BufferedBlockDevice()
{
}

int BufferedBlockDevice::init()
{
    return 0;
}

int BufferedBlockDevice::deinit()
{
    return 0;
}

int BufferedBlockDevice::flush()
{
    return 0;
}

int BufferedBlockDevice::sync()
{
    return 0;
}

int BufferedBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int BufferedBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int BufferedBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int BufferedBlockDevice::trim(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

bd_size_t BufferedBlockDevice::get_read_size() const
{
    return 1;
}

bd_size_t BufferedBlockDevice::get_program_size() const
{
    return 1;
}

bd_size_t BufferedBlockDevice::get_erase_size() const
{
    return 0;
}

bd_size_t BufferedBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return 0;
}

int BufferedBlockDevice::get_erase_value() const
{
    return 0;
}

bd_size_t BufferedBlockDevice::size() const
{
    return 0;
}
