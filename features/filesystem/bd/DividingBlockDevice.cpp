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

#include "DividingBlockDevice.h"


DividingBlockDevice::DividingBlockDevice(BlockDevice *bd, bd_size_t block_size)
    : _bd(bd), _block_size(block_size), _div(1), _buffer(0)
{
}

bd_error_t DividingBlockDevice::init()
{
    bd_error_t err = _bd->init();
    if (err) {
        return err;
    }

    bd_size_t raw_size = _bd->get_block_size();
    _div = raw_size / _block_size;

    // Check for divisible block size
    if (_div * _block_size != raw_size) {
        return BD_ERROR_PARAMETER;
    }

    _buffer = static_cast<uint8_t*>(malloc(raw_size));
    if (!_buffer) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return 0;
}

bd_error_t DividingBlockDevice::deinit()
{
    free(_buffer);
    return _bd->deinit();
}

bd_count_or_error_t DividingBlockDevice::read(void *p, bd_block_t block, bd_count_t count)
{
    uint8_t *rem_buffer = static_cast<uint8_t*>(p);
    bd_block_t rem_block = block;
    bd_count_t rem_count = count;

    while (rem_count > 0) {
        // Prefer direct reads if possible
        if (rem_count > _div*_block_size && rem_block % _div == 0) {
            int result = _bd->read(rem_buffer, rem_block/_div, rem_count/_div);
            if (result < 0) {
                return result;
            }

            rem_buffer += result*_div*_block_size;
            rem_block += result*_div;
            rem_count -= result*_div;
            continue;
        }

        // Otherwise buffer the underlying block
        int err = _bd->read(_buffer, rem_block/_div);
        if (err < 0) {
            return err;
        }

        bd_count_t read = rem_count;
        if (read > _div - rem_block%_div) {
            read = _div - rem_block%_div;
        }

        memcpy(rem_buffer, &_buffer[(rem_block%_div)*_block_size], read*_block_size);
        rem_buffer += read*_block_size;
        rem_block += read;
        rem_count -= read;
    }

    return count;
}

bd_count_or_error_t DividingBlockDevice::write(const void *p, bd_block_t block, bd_count_t count)
{
    const uint8_t *rem_buffer = static_cast<const uint8_t*>(p);
    bd_block_t rem_block = block;
    bd_count_t rem_count = count;

    while (rem_count > 0) {
        // Prefer direct reads if possible
        if (rem_count > _div*_block_size && rem_block % _div == 0) {
            int result = _bd->write(rem_buffer, rem_block/_div, rem_count/_div);
            if (result < 0) {
                return result;
            }

            rem_buffer += result*_div*_block_size;
            rem_block += result*_div;
            rem_count -= result*_div;
            continue;
        }

        // Otherwise buffer the underlying block
        int err = _bd->read(_buffer, rem_block/_div);
        if (err < 0) {
            return err;
        }

        bd_count_t write = rem_count;
        if (write > _div - rem_block%_div) {
            write = _div - rem_block%_div;
        }

        memcpy(&_buffer[(rem_block%_div)*_block_size], rem_buffer, write*_block_size);

        err = _bd->write(_buffer, rem_block/_div);
        if (err < 0) {
            return err;
        }

        rem_buffer += write*_block_size;
        rem_block += write;
        rem_count -= write;
    }

    return count;
}

bd_error_t DividingBlockDevice::sync()
{
    return _bd->sync();
}

bd_error_t DividingBlockDevice::status()
{
    return _bd->status();
}

bd_size_t DividingBlockDevice::get_block_size()
{
    return _bd->get_block_size() / _div;
}

bd_count_t DividingBlockDevice::get_block_count()
{
    return _bd->get_block_count() * _div;
}
