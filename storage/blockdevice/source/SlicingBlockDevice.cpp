/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "blockdevice/SlicingBlockDevice.h"
#include "platform/mbed_assert.h"
#include "stddef.h"
#include <stdio.h>

namespace mbed {


SlicingBlockDevice::SlicingBlockDevice(BlockDevice *bd, bd_addr_t start, bd_addr_t stop)
    : _bd(bd)
    , _start_from_end(false), _start(start)
    , _stop_from_end(false), _stop(stop)
{
    MBED_ASSERT(bd);
    // SlicingBlockDevice(bd, 0,0) would use the full block, which does not make sense, it must be a programming eror.
    // SlicingBlockDevice(bd, 100,100) would have no size, which is also a programming error.
    MBED_ASSERT(start != stop);

    if ((int64_t)_start < 0) {
        _start_from_end = true;
        _start = -_start;
    }

    if ((int64_t)_stop <= 0) {
        _stop_from_end = true;
        _stop = -_stop;
    }
}

int SlicingBlockDevice::init()
{
    int err = _bd->init();
    if (err) {
        return err;
    }

    bd_size_t size = _bd->size();

    // Calculate from_end values
    if (_start_from_end) {
        _start_from_end = false;
        _start = size - _start;
    }

    if (_stop_from_end) {
        _stop_from_end = false;
        _stop = size - _stop;
    }

    // Check that block addresses are valid
    if (!is_valid_erase(0, _stop - _start)) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return 0;
}

int SlicingBlockDevice::deinit()
{
    return _bd->deinit();
}

int SlicingBlockDevice::sync()
{
    return _bd->sync();
}

int SlicingBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    if (!is_valid_read(addr, size)) {
        return BD_ERROR_DEVICE_ERROR;
    }
    return _bd->read(b, addr + _start, size);
}

int SlicingBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    if (!is_valid_program(addr, size)) {
        return BD_ERROR_DEVICE_ERROR;
    }
    return _bd->program(b, addr + _start, size);
}

int SlicingBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    if (!is_valid_erase(addr, size)) {
        return BD_ERROR_DEVICE_ERROR;
    }
    return _bd->erase(addr + _start, size);
}

bool SlicingBlockDevice::is_valid_read(bd_addr_t addr, bd_size_t size) const
{
    return _bd->is_valid_read(_start + addr, size) && _start + addr + size <= _stop;
}

bool SlicingBlockDevice::is_valid_program(bd_addr_t addr, bd_size_t size) const
{
    return _bd->is_valid_program(_start + addr, size) && _start + addr + size <= _stop;
}

bool SlicingBlockDevice::is_valid_erase(bd_addr_t addr, bd_size_t size) const
{
    return _bd->is_valid_erase(_start + addr, size) && _start + addr + size <= _stop;
}

bd_size_t SlicingBlockDevice::get_read_size() const
{
    return _bd->get_read_size();
}

bd_size_t SlicingBlockDevice::get_program_size() const
{
    return _bd->get_program_size();
}

bd_size_t SlicingBlockDevice::get_erase_size() const
{
    return _bd->get_erase_size(_start);
}

bd_size_t SlicingBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return _bd->get_erase_size(_start + addr);
}

int SlicingBlockDevice::get_erase_value() const
{
    return _bd->get_erase_value();
}

bd_size_t SlicingBlockDevice::size() const
{
    return _stop - _start;
}

const char *SlicingBlockDevice::get_type() const
{
    return _bd->get_type();
}

} // namespace mbed
