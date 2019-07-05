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
#include "platform/mbed_atomic.h"
#include "platform/mbed_assert.h"

namespace mbed {

ExhaustibleBlockDevice::ExhaustibleBlockDevice(BlockDevice *bd, uint32_t erase_cycles)
    : _bd(bd), _erase_array(NULL), _erase_cycles(erase_cycles), _init_ref_count(0), _is_initialized(false)
{
}

ExhaustibleBlockDevice::~ExhaustibleBlockDevice()
{
    delete[] _erase_array;
}

int ExhaustibleBlockDevice::init()
{
    int err;
    uint32_t val = core_util_atomic_incr_u32(&_init_ref_count, 1);

    if (val != 1) {
        return BD_ERROR_OK;
    }

    err = _bd->init();
    if (err) {
        goto fail;
    }

    if (!_erase_array) {
        // can only be allocated after initialization
        _erase_array = new uint32_t[_bd->size() / _bd->get_erase_size()];
        for (size_t i = 0; i < _bd->size() / _bd->get_erase_size(); i++) {
            _erase_array[i] = _erase_cycles;
        }
    }

    _is_initialized = true;
    return BD_ERROR_OK;

fail:
    _is_initialized = false;
    _init_ref_count = 0;
    return err;
}

int ExhaustibleBlockDevice::deinit()
{
    if (!_is_initialized) {
        return BD_ERROR_OK;
    }

    core_util_atomic_decr_u32(&_init_ref_count, 1);

    if (_init_ref_count) {
        return BD_ERROR_OK;
    }

    // _erase_array is lazily cleaned up in destructor to allow
    // data to live across de/reinitialization
    _is_initialized = false;
    return _bd->deinit();
}

int ExhaustibleBlockDevice::sync()
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _bd->sync();
}

int ExhaustibleBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _bd->read(buffer, addr, size);
}

int ExhaustibleBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_program(addr, size));

    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    if (_erase_array[addr / get_erase_size()] == 0) {
        return 0;
    }

    return _bd->program(buffer, addr, size);
}

int ExhaustibleBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_erase(addr, size));
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

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
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_read_size();
}

bd_size_t ExhaustibleBlockDevice::get_program_size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_program_size();
}

bd_size_t ExhaustibleBlockDevice::get_erase_size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_erase_size();
}

bd_size_t ExhaustibleBlockDevice::get_erase_size(bd_addr_t addr) const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_erase_size(addr);
}

int ExhaustibleBlockDevice::get_erase_value() const
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _bd->get_erase_value();
}

bd_size_t ExhaustibleBlockDevice::size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->size();
}

const char *ExhaustibleBlockDevice::get_type() const
{
    if (_bd != NULL) {
        return _bd->get_type();
    }

    return NULL;
}

} // namespace mbed

