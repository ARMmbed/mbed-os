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

#include "SlicingBlockDevice.h"
#include "platform/mbed_assert.h"
#include "stddef.h"

namespace mbed {


SlicingBlockDevice::SlicingBlockDevice(BlockDevice *bd, bd_addr_t start, bd_addr_t stop)
    : _bd(bd)
    , _start_from_end(false), _start(start)
    , _stop_from_end(false), _stop(stop)
{
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
    MBED_ASSERT(is_valid_erase(_start, _stop - _start));

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
    MBED_ASSERT(is_valid_read(addr + _start, size));
    return _bd->read(b, addr + _start, size);
}

int SlicingBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_program(addr + _start, size));
    return _bd->program(b, addr + _start, size);
}

int SlicingBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_erase(addr + _start, size));
    return _bd->erase(addr + _start, size);
}

bool SlicingBlockDevice::is_valid_read(bd_addr_t addr, bd_size_t size) const
{
    return (
               addr % get_read_size() == 0 &&
               size % get_read_size() == 0 &&
               addr + size <= (this->size() + _start));
}

bool SlicingBlockDevice::is_valid_program(bd_addr_t addr, bd_size_t size) const
{
    return (
               addr % get_program_size() == 0 &&
               size % get_program_size() == 0 &&
               addr + size <= (this->size() + _start));
}

bool SlicingBlockDevice::is_valid_erase(bd_addr_t addr, bd_size_t size) const
{
    return (
               addr % get_erase_size(addr) == 0 &&
               (addr + size) % get_erase_size(addr + size - 1) == 0 &&
               addr + size <= (this->size() + _start));
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
    return _bd->get_erase_size();
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
    if (_bd != NULL) {
        return _bd->get_type();
    }

    return NULL;
}

} // namespace mbed

