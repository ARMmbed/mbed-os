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

#include "HeapBlockDevice.h"


HeapBlockDevice::HeapBlockDevice(bd_size_t size, bd_size_t block)
    : _read_size(block), _program_size(block), _erase_size(block)
    , _count(size / block), _blocks(0)
{
    MBED_ASSERT(_count * _erase_size == size);
}

HeapBlockDevice::HeapBlockDevice(bd_size_t size, bd_size_t read, bd_size_t program, bd_size_t erase)
    : _read_size(read), _program_size(program), _erase_size(erase)
    , _count(size / erase), _blocks(0)
{
    MBED_ASSERT(_count * _erase_size == size);
}

HeapBlockDevice::~HeapBlockDevice()
{
    if (_blocks) {
        for (size_t i = 0; i < _count; i++) {
            free(_blocks[i]);
        }

        delete[] _blocks;
        _blocks = 0;
    }
}

int HeapBlockDevice::init()
{
    if (!_blocks) {
        _blocks = new uint8_t*[_count];
        for (size_t i = 0; i < _count; i++) {
            _blocks[i] = 0;
        }
    }

    return BD_ERROR_OK;
}

int HeapBlockDevice::deinit()
{
    MBED_ASSERT(_blocks != NULL);
    // Memory is lazily cleaned up in destructor to allow
    // data to live across de/reinitialization
    return BD_ERROR_OK;
}

bd_size_t HeapBlockDevice::get_read_size() const
{
    MBED_ASSERT(_blocks != NULL);
    return _read_size;
}

bd_size_t HeapBlockDevice::get_program_size() const
{
    MBED_ASSERT(_blocks != NULL);
    return _program_size;
}

bd_size_t HeapBlockDevice::get_erase_size() const
{
    MBED_ASSERT(_blocks != NULL);
    return _erase_size;
}

bd_size_t HeapBlockDevice::size() const
{
    MBED_ASSERT(_blocks != NULL);
    return _count * _erase_size;
}

int HeapBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(_blocks != NULL);
    MBED_ASSERT(is_valid_read(addr, size));
    uint8_t *buffer = static_cast<uint8_t*>(b);

    while (size > 0) {
        bd_addr_t hi = addr / _erase_size;
        bd_addr_t lo = addr % _erase_size;

        if (_blocks[hi]) {
            memcpy(buffer, &_blocks[hi][lo], _read_size);
        } else {
            memset(buffer, 0, _read_size);
        }

        buffer += _read_size;
        addr += _read_size;
        size -= _read_size;
    }

    return 0;
}

int HeapBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(_blocks != NULL);
    MBED_ASSERT(is_valid_program(addr, size));
    const uint8_t *buffer = static_cast<const uint8_t*>(b);

    while (size > 0) {
        bd_addr_t hi = addr / _erase_size;
        bd_addr_t lo = addr % _erase_size;

        if (!_blocks[hi]) {
            _blocks[hi] = (uint8_t*)malloc(_erase_size);
            if (!_blocks[hi]) {
                return BD_ERROR_DEVICE_ERROR;
            }
        }

        memcpy(&_blocks[hi][lo], buffer, _program_size);

        buffer += _program_size;
        addr += _program_size;
        size -= _program_size;
    }

    return 0;
}

int HeapBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(_blocks != NULL);
    MBED_ASSERT(is_valid_erase(addr, size));
    // TODO assert on programming unerased blocks

    return 0;
}

