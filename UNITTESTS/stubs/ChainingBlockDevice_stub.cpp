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

#include "ChainingBlockDevice.h"
#include "mbed_critical.h"


ChainingBlockDevice::ChainingBlockDevice(BlockDevice **bds, size_t bd_count)
{
}

static bool is_aligned(uint64_t x, uint64_t alignment)
{
    return true;
}

int ChainingBlockDevice::init()
{
    return 0;
}

int ChainingBlockDevice::deinit()
{
    return 0;
}

int ChainingBlockDevice::sync()
{
    return 0;
}

int ChainingBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int ChainingBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int ChainingBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

bd_size_t ChainingBlockDevice::get_read_size() const
{
    return 0;
}

bd_size_t ChainingBlockDevice::get_program_size() const
{
    return 0;
}

bd_size_t ChainingBlockDevice::get_erase_size() const
{
    return 0;
}

bd_size_t ChainingBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return 0;
}

int ChainingBlockDevice::get_erase_value() const
{
    return 0;
}

bd_size_t ChainingBlockDevice::size() const
{
    return 0;
}
