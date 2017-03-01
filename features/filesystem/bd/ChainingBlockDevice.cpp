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

#include "ChainingBlockDevice.h"


ChainingBlockDevice::ChainingBlockDevice(BlockDevice **bds, size_t bd_count)
    : _bds(bds), _bd_count(bd_count)
    , _read_size(0), _program_size(0), _erase_size(0), _size(0)
{
}

static bool is_aligned(uint64_t x, uint64_t alignment)
{
    return (x / alignment) * alignment == x;
}

int ChainingBlockDevice::init()
{
    _read_size = 0;
    _program_size = 0;
    _erase_size = 0;
    _size = 0;

    // Initialize children block devices, find all sizes and
    // assert that block sizes are similar. We can't do this in
    // the constructor since some block devices may need to be
    // initialized before they know their block size/count
    for (size_t i = 0; i < _bd_count; i++) {
        int err = _bds[i]->init();
        if (err) {
            return err;
        }

        bd_size_t read = _bds[i]->get_read_size();
        if (i == 0 || (read >= _read_size && is_aligned(read, _read_size))) {
            _read_size = read;
        } else {
            MBED_ASSERT(_read_size > read && is_aligned(_read_size, read));
        }

        bd_size_t program = _bds[i]->get_program_size();
        if (i == 0 || (program >= _program_size && is_aligned(program, _program_size))) {
            _program_size = program;
        } else {
            MBED_ASSERT(_program_size > program && is_aligned(_program_size, program));
        }

        bd_size_t erase = _bds[i]->get_erase_size();
        if (i == 0 || (erase >= _erase_size && is_aligned(erase, _erase_size))) {
            _erase_size = erase;
        } else {
            MBED_ASSERT(_erase_size > erase && is_aligned(_erase_size, erase));
        }

        _size += _bds[i]->size();
    }

    return 0;
}

int ChainingBlockDevice::deinit()
{
    for (size_t i = 0; i < _bd_count; i++) {
        int err = _bds[i]->deinit();
        if (err) {
            return err;
        }
    }

    return 0;
}

int ChainingBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_read(addr, size));
    uint8_t *buffer = static_cast<uint8_t*>(b);

    // Find block devices containing blocks, may span multiple block devices
    for (size_t i = 0; i < _bd_count && size > 0; i++) {
        bd_size_t bdsize = _bds[i]->size();

        if (addr < bdsize) {
            bd_size_t read = size;
            if (addr + read > bdsize) {
                read = bdsize - addr;
            }

            int err = _bds[i]->read(buffer, addr, read);
            if (err) {
                return err;
            }

            buffer += read;
            addr += read;
            size -= read;
        }

        addr -= size;
    }

    return 0;
}

int ChainingBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_program(addr, size));
    const uint8_t *buffer = static_cast<const uint8_t*>(b);

    // Find block devices containing blocks, may span multiple block devices
    for (size_t i = 0; i < _bd_count && size > 0; i++) {
        bd_size_t bdsize = _bds[i]->size();

        if (addr < bdsize) {
            bd_size_t program = size;
            if (addr + program > bdsize) {
                program = bdsize - addr;
            }

            int err = _bds[i]->program(buffer, addr, program);
            if (err) {
                return err;
            }

            buffer += program;
            addr += program;
            size -= program;
        }

        addr -= size;
    }

    return 0;
}

int ChainingBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_erase(addr, size));

    // Find block devices containing blocks, may span multiple block devices
    for (size_t i = 0; i < _bd_count && size > 0; i++) {
        bd_size_t bdsize = _bds[i]->size();

        if (addr < bdsize) {
            bd_size_t erase = size;
            if (addr + erase > bdsize) {
                erase = bdsize - addr;
            }

            int err = _bds[i]->erase(addr, erase);
            if (err) {
                return err;
            }

            addr += erase;
            size -= erase;
        }

        addr -= size;
    }

    return 0;
}

bd_size_t ChainingBlockDevice::get_read_size() const
{
    return _read_size;
}

bd_size_t ChainingBlockDevice::get_program_size() const
{
    return _program_size;
}

bd_size_t ChainingBlockDevice::get_erase_size() const
{
    return _erase_size;
}

bd_size_t ChainingBlockDevice::size() const
{
    return _size;
}
