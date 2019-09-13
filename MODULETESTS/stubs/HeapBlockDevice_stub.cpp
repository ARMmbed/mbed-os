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

#include "HeapBlockDevice.h"
#include "mbed_critical.h"


HeapBlockDevice::HeapBlockDevice(bd_size_t size, bd_size_t block)
{
}

HeapBlockDevice::HeapBlockDevice(bd_size_t size, bd_size_t read, bd_size_t program, bd_size_t erase)
{
}

HeapBlockDevice::~HeapBlockDevice()
{
}

int HeapBlockDevice::init()
{
    return 0;
}

int HeapBlockDevice::deinit()
{
    return 0;
}

bd_size_t HeapBlockDevice::get_read_size() const
{
    return 0;
}

bd_size_t HeapBlockDevice::get_program_size() const
{
    return 0;
}

bd_size_t HeapBlockDevice::get_erase_size() const
{
    return 0;
}

bd_size_t HeapBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return 0;
}

bd_size_t HeapBlockDevice::size() const
{
    return 0;
}

int HeapBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int HeapBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int HeapBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

