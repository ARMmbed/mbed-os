/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
    : _bd(bd)
    , _read_count(0)
    , _program_count(0)
    , _erase_count(0)
{
}

int ProfilingBlockDevice::init()
{
    return _bd->init();
}

int ProfilingBlockDevice::deinit()
{
    return _bd->deinit();
}

int ProfilingBlockDevice::sync()
{
    return _bd->sync();
}

int ProfilingBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    int err = _bd->read(b, addr, size);
    if (!err) {
        _read_count += size;
    }
    return err;
}

int ProfilingBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    int err = _bd->program(b, addr, size);
    if (!err) {
        _program_count += size;
    }
    return err;
}

int ProfilingBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    int err = _bd->erase(addr, size);
    if (!err) {
        _erase_count += size;
    }
    return err;
}

bd_size_t ProfilingBlockDevice::get_read_size() const
{
    return _bd->get_read_size();
}

bd_size_t ProfilingBlockDevice::get_program_size() const
{
    return _bd->get_program_size();
}

bd_size_t ProfilingBlockDevice::get_erase_size() const
{
    return _bd->get_erase_size();
}

bd_size_t ProfilingBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return _bd->get_erase_size(addr);
}

int ProfilingBlockDevice::get_erase_value() const
{
    return _bd->get_erase_value();
}

bd_size_t ProfilingBlockDevice::size() const
{
    return _bd->size();
}

void ProfilingBlockDevice::reset()
{
    _read_count = 0;
    _program_count = 0;
    _erase_count = 0;
}

bd_size_t ProfilingBlockDevice::get_read_count() const
{
    return _read_count;
}

bd_size_t ProfilingBlockDevice::get_program_count() const
{
    return _program_count;
}

bd_size_t ProfilingBlockDevice::get_erase_count() const
{
    return _erase_count;
}
