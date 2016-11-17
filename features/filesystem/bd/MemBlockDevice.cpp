/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include "MemBlockDevice.h"


MemBlockDevice::MemBlockDevice(bd_size_t size, bd_count_t count)
    : _size(size)
    , _count(count)
    , _blocks(0)
{
}

MemBlockDevice::~MemBlockDevice()
{
    // nop if uninitialized
    deinit();
}

bd_error_t MemBlockDevice::init()
{
    _blocks = (uint8_t**)malloc(_count*sizeof(uint8_t*));
    if (!_blocks) {
        return BD_ERROR_DEVICE_ERROR;
    }

    memset(_blocks, 0, _count*sizeof(uint8_t*));
    return BD_ERROR_OK;
}

bd_error_t MemBlockDevice::deinit()
{
    if (_blocks) {
        for (bd_count_t i = 0; i < _count; i++) {
            free(_blocks[i]);
        }

        free(_blocks);
        _blocks = 0;
    }

    return BD_ERROR_OK;
}

bd_error_t MemBlockDevice::status()
{
    return _blocks ? BD_ERROR_OK : BD_ERROR_NO_INIT;
}

bd_size_t MemBlockDevice::get_block_size()
{
    return _size;
}

bd_block_t MemBlockDevice::get_block_count()
{
    return _count;
}

bd_count_or_error_t MemBlockDevice::read(void *b, bd_block_t block, bd_count_t count)
{
    uint8_t *buffer = (uint8_t*)b;
    if (!_blocks || block + count > _count) {
        return BD_ERROR_PARAMETER;
    }
    
    for (bd_count_t i = 0; i < count; i++) {
        if (!_blocks[block + i]) {
            memset(&buffer[i*_size], 0, _size);
        } else {
            memcpy(&buffer[i*_size], _blocks[block+i], _size);
        }
    }

    return count;
}

bd_count_or_error_t MemBlockDevice::write(const void *b, bd_block_t block, bd_count_t count)
{
    const uint8_t *buffer = (const uint8_t*)b;
    if (!_blocks || block + count > _count) {
        return BD_ERROR_PARAMETER;
    }

    for (bd_count_t i = 0; i < count; i++) {
        if (!_blocks[block + i]) {
            _blocks[block + i] = (uint8_t*)malloc(_size);
            if (!_blocks[block + i]) {
                return BD_ERROR_DEVICE_ERROR;
            }
        }

        memcpy(_blocks[block+i], &buffer[i*_size], _size);
    }

    return count;
}
