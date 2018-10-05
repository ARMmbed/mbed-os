/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "ObservingBlockDevice.h"
#include "ReadOnlyBlockDevice.h"
#include "mbed.h"


ObservingBlockDevice::ObservingBlockDevice(BlockDevice *bd)
{
    // Does nothing
}

ObservingBlockDevice::~ObservingBlockDevice()
{
    // Does nothing
}

void ObservingBlockDevice::attach(Callback<void(BlockDevice *)> cb)
{
}

int ObservingBlockDevice::init()
{
    return 0;
}

int ObservingBlockDevice::deinit()
{
    return 0;
}

int ObservingBlockDevice::sync()
{
    return 0;
}

int ObservingBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int ObservingBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int ObservingBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

bd_size_t ObservingBlockDevice::get_read_size() const
{
    return 0;
}

bd_size_t ObservingBlockDevice::get_program_size() const
{
    return 0;
}

bd_size_t ObservingBlockDevice::get_erase_size() const
{
    return 0;
}

bd_size_t ObservingBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return 0;
}

int ObservingBlockDevice::get_erase_value() const
{
    return 0;
}

bd_size_t ObservingBlockDevice::size() const
{
    return 0;
}
