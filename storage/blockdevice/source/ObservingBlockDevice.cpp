/* mbed Microcontroller Library
 * Copyright (c) 2017-2020 ARM Limited
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

#include "blockdevice/ObservingBlockDevice.h"
#include "blockdevice/ReadOnlyBlockDevice.h"

namespace mbed {

ObservingBlockDevice::ObservingBlockDevice(BlockDevice *bd)
    : _bd(bd)
{
    // Does nothing
}

ObservingBlockDevice::~ObservingBlockDevice()
{
    // Does nothing
}

void ObservingBlockDevice::attach(mbed::Callback<void(BlockDevice *)> cb)
{
    _change = cb;
}

int ObservingBlockDevice::init()
{
    return _bd->init();
}

int ObservingBlockDevice::deinit()
{
    return _bd->deinit();
}

int ObservingBlockDevice::sync()
{
    return _bd->sync();
}

int ObservingBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    return _bd->read(buffer, addr, size);
}

int ObservingBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    int res = _bd->program(buffer, addr, size);
    if (_change) {
        ReadOnlyBlockDevice dev(_bd);
        _change(&dev);
    }
    return res;
}

int ObservingBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    int res = _bd->erase(addr, size);
    if (_change) {
        ReadOnlyBlockDevice dev(_bd);
        _change(&dev);
    }
    return res;
}

bd_size_t ObservingBlockDevice::get_read_size() const
{
    return _bd->get_read_size();
}

bd_size_t ObservingBlockDevice::get_program_size() const
{
    return _bd->get_program_size();
}

bd_size_t ObservingBlockDevice::get_erase_size() const
{
    return _bd->get_erase_size();
}

bd_size_t ObservingBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return _bd->get_erase_size(addr);
}

int ObservingBlockDevice::get_erase_value() const
{
    return _bd->get_erase_value();
}

bd_size_t ObservingBlockDevice::size() const
{
    return _bd->size();
}

const char *ObservingBlockDevice::get_type() const
{
    if (_bd != NULL) {
        return _bd->get_type();
    }

    return NULL;
}

} // namespace mbed
