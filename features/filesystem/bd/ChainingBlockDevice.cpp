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

#include "ChainingBlockDevice.h"


ChainingBlockDevice::ChainingBlockDevice(BlockDevice **bds, size_t bd_count)
    : _bds(bds), _bd_count(bd_count), _block_size(0), _block_count(0) {
}

bd_error_t ChainingBlockDevice::init()
{
    _block_size = 0;
    _block_count = 0;

    // Initialize children block devices, find total block count
    // and assert that block sizes are equal. We can't do this in
    // the constructor since some block devices may need to be
    // initialized before they know their block size/count
    for (size_t i = 0; i < _bd_count; i++) {
        bd_error_t err = _bds[i]->init();
        if (err) {
            return err;
        }

        bd_size_t prev = _block_size;
        _block_size = _bds[i]->get_block_size();
        if (i != 0 && prev != _block_size) {
            return BD_ERROR_PARAMETER;
        }

        _block_count += _bds[i]->get_block_count();
    }

    return 0;
}

bd_error_t ChainingBlockDevice::deinit()
{
    for (size_t i = 0; i < _bd_count; i++) {
        bd_error_t err = _bds[i]->deinit();
        if (err) {
            return err;
        }
    }

    return 0;
}

bd_count_or_error_t ChainingBlockDevice::read(void *p, bd_block_t block, bd_count_t count)
{
    uint8_t *buffer = static_cast<uint8_t*>(p);
    bd_count_t read_count = 0;

    if (block + count > _block_count) {
        return BD_ERROR_PARAMETER;
    }

    // Find block devices containing blocks, may span multiple block devices
    for (size_t i = 0; i < _bd_count && count > 0; i++) {
        bd_size_t block_count = _bds[i]->get_block_count();

        if (block < block_count) {
            bd_count_t read = count;
            if (block + read > block_count) {
                read = block_count - block;
            }

            bd_count_or_error_t result = _bds[i]->read(buffer, block, read);
            if (result != (bd_count_or_error_t)read) {
                return read_count + (result < 0 ? 0 : result);
            }

            buffer += read*_block_size;
            block += read;
            count -= read;
            read_count += read;
        }

        block -= block_count;
    }

    return read_count;
}

bd_count_or_error_t ChainingBlockDevice::write(const void *p, bd_block_t block, bd_count_t count)
{
    const uint8_t *buffer = static_cast<const uint8_t*>(p);
    bd_count_t write_count = 0;

    if (block + count > _block_count) {
        return BD_ERROR_PARAMETER;
    }

    // Find block devices containing blocks, may span multiple block devices
    for (size_t i = 0; i < _bd_count && count > 0; i++) {
        bd_size_t block_count = _bds[i]->get_block_count();

        if (block < block_count) {
            bd_count_t write = count;
            if (block + write > block_count) {
                write = block_count - block;
            }

            bd_count_or_error_t result = _bds[i]->write(buffer, block, write);
            if (result != (bd_count_or_error_t)write) {
                return write_count + (result < 0 ? 0 : result);
            }

            buffer += write*_block_size;
            block += write;
            count -= write;
            write_count += write;
        }

        block -= block_count;
    }

    return write_count;
}

bd_error_t ChainingBlockDevice::sync()
{
    for (size_t i = 0; i < _bd_count; i++) {
        bd_error_t err = _bds[i]->sync();
        if (err) {
            return err;
        }
    }

    return 0;
}

bd_error_t ChainingBlockDevice::status()
{
    for (size_t i = 0; i < _bd_count; i++) {
        bd_error_t err = _bds[i]->status();
        if (err) {
            return err;
        }
    }

    return 0;
}

bd_size_t ChainingBlockDevice::get_block_size()
{
    return _block_size;
}

bd_count_t ChainingBlockDevice::get_block_count()
{
    return _block_count;
}
