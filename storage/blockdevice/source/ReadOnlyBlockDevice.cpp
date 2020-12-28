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

/** \addtogroup storage */
/** @{*/

#include "blockdevice/ReadOnlyBlockDevice.h"
#include "platform/mbed_error.h"

namespace mbed {

ReadOnlyBlockDevice::ReadOnlyBlockDevice(BlockDevice *bd)
    : _bd(bd)
{
    // Does nothing
}

ReadOnlyBlockDevice::~ReadOnlyBlockDevice()
{
    // Does nothing
}

int ReadOnlyBlockDevice::init()
{
    return _bd->init();
}

int ReadOnlyBlockDevice::deinit()
{
    return _bd->deinit();
}

int ReadOnlyBlockDevice::sync()
{
    return _bd->sync();
}

int ReadOnlyBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    return _bd->read(buffer, addr, size);
}

int ReadOnlyBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    return MBED_ERROR_WRITE_PROTECTED;
}

int ReadOnlyBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return MBED_ERROR_WRITE_PROTECTED;
}

bd_size_t ReadOnlyBlockDevice::get_read_size() const
{
    return _bd->get_read_size();
}

bd_size_t ReadOnlyBlockDevice::get_program_size() const
{
    return _bd->get_program_size();
}

bd_size_t ReadOnlyBlockDevice::get_erase_size() const
{
    return _bd->get_erase_size();
}

bd_size_t ReadOnlyBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return _bd->get_erase_size(addr);
}

int ReadOnlyBlockDevice::get_erase_value() const
{
    return _bd->get_erase_value();
}

bd_size_t ReadOnlyBlockDevice::size() const
{
    return _bd->size();
}

const char *ReadOnlyBlockDevice::get_type() const
{
    if (_bd != NULL) {
        return _bd->get_type();
    }

    return NULL;
}

} // namespace mbed

/** @}*/
