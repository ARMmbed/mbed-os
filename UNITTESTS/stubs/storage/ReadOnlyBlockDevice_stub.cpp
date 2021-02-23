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

#include "ReadOnlyBlockDevice.h"
#include "mbed_error.h"


ReadOnlyBlockDevice::ReadOnlyBlockDevice(BlockDevice *bd)
{
    // Does nothing
}

ReadOnlyBlockDevice::~ReadOnlyBlockDevice()
{
    // Does nothing
}

int ReadOnlyBlockDevice::init()
{
    return 0;
}

int ReadOnlyBlockDevice::deinit()
{
    return 0;
}

int ReadOnlyBlockDevice::sync()
{
    return 0;
}

int ReadOnlyBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int ReadOnlyBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int ReadOnlyBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

bd_size_t ReadOnlyBlockDevice::get_read_size() const
{
    return 0;
}

bd_size_t ReadOnlyBlockDevice::get_program_size() const
{
    return 0;
}

bd_size_t ReadOnlyBlockDevice::get_erase_size() const
{
    return 0;
}

bd_size_t ReadOnlyBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return 0;
}

int ReadOnlyBlockDevice::get_erase_value() const
{
    return 0;
}

bd_size_t ReadOnlyBlockDevice::size() const
{
    return 0;
}
