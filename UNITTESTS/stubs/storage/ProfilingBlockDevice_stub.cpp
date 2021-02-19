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

#include "ProfilingBlockDevice.h"


ProfilingBlockDevice::ProfilingBlockDevice(BlockDevice *bd)
{
}

int ProfilingBlockDevice::init()
{
    return 0;
}

int ProfilingBlockDevice::deinit()
{
    return 0;
}

int ProfilingBlockDevice::sync()
{
    return 0;
}

int ProfilingBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int ProfilingBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int ProfilingBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

bd_size_t ProfilingBlockDevice::get_read_size() const
{
    return 0;
}

bd_size_t ProfilingBlockDevice::get_program_size() const
{
    return 0;
}

bd_size_t ProfilingBlockDevice::get_erase_size() const
{
    return 0;
}

bd_size_t ProfilingBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return 0;
}

int ProfilingBlockDevice::get_erase_value() const
{
    return 0;
}

bd_size_t ProfilingBlockDevice::size() const
{
    return 0;
}

void ProfilingBlockDevice::reset()
{
}

bd_size_t ProfilingBlockDevice::get_read_count() const
{
    return 0;
}

bd_size_t ProfilingBlockDevice::get_program_count() const
{
    return 0;
}

bd_size_t ProfilingBlockDevice::get_erase_count() const
{
    return 0;
}
