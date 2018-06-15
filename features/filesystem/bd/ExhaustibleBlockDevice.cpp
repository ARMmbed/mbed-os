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

#include "ExhaustibleBlockDevice.h"
#include "mbed.h"


ExhaustibleBlockDevice::ExhaustibleBlockDevice(BlockDevice *bd, uint32_t erase_cycles)
    : _bd(bd), _erase_array(NULL), _erase_cycles(erase_cycles)
{
}

ExhaustibleBlockDevice::~ExhaustibleBlockDevice()
{
    delete[] _erase_array;
}

int ExhaustibleBlockDevice::init()
{
    int err = _bd->init();
    if (err) {
        return err;
    }

    if (!_erase_array) {
        // can only be allocated after initialization
        _erase_array = new uint32_t[_bd->size() / _bd->get_erase_size()];
        for (size_t i = 0; i < _bd->size() / _bd->get_erase_size(); i++) {
            _erase_array[i] = _erase_cycles;
        }
    }

    return 0;
}

int ExhaustibleBlockDevice::deinit()
{
    // _erase_array is lazily cleaned up in destructor to allow
    // data to live across de/reinitialization
    return _bd->deinit();
}

int ExhaustibleBlockDevice::sync()
{
    return _bd->sync();
}

int ExhaustibleBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    return _bd->read(buffer, addr, size);
}

int ExhaustibleBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_program(addr, size));

    if (_erase_array[addr / get_erase_size()] == 0) {
        return 0;
    }

    return _bd->program(buffer, addr, size);
}

int ExhaustibleBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_erase(addr, size));

    bd_size_t eu_size = get_erase_size();
    while (size) {
        // use an erase cycle
        if (_erase_array[addr / eu_size] > 0) {
            _erase_array[addr / eu_size] -= 1;
        }

        if (_erase_array[addr / eu_size] > 0) {
            int  err = _bd->erase(addr, eu_size);
            if (err) {
                return err;
            }
        }

        addr += eu_size;
        size -= eu_size;
    }

    return 0;
}

bd_size_t ExhaustibleBlockDevice::get_read_size() const
{
    return _bd->get_read_size();
}

bd_size_t ExhaustibleBlockDevice::get_program_size() const
{
    return _bd->get_program_size();
}

bd_size_t ExhaustibleBlockDevice::get_erase_size() const
{
    return _bd->get_erase_size();
}

bd_size_t ExhaustibleBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return _bd->get_erase_size(addr);
}

int ExhaustibleBlockDevice::get_erase_value() const
{
    return _bd->get_erase_value();
}

bd_size_t ExhaustibleBlockDevice::size() const
{
    return _bd->size();
}
