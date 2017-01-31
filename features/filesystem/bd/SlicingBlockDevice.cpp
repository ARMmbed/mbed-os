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

#include "SlicingBlockDevice.h"


SlicingBlockDevice::SlicingBlockDevice(BlockDevice *bd, bd_block_t start, bd_block_t stop)
    : _bd(bd), _start(start), _stop(stop)
{
}

bd_error_t SlicingBlockDevice::init()
{
    bd_error_t err = _bd->init();
    if (err) {
        return err;
    }

    // Check that block indices are in range
    bd_count_t count = _bd->get_block_count();
    if (_start >= count || _stop > count) {
        return BD_ERROR_PARAMETER;
    }

    return 0;
}

bd_error_t SlicingBlockDevice::deinit()
{
    return _bd->deinit();
}

bd_count_or_error_t SlicingBlockDevice::read(void *p, bd_block_t block, bd_count_t count)
{
    if (block + count > _stop - _start) {
        return BD_ERROR_PARAMETER;
    }

    return _bd->read(p, block - _start, count);
}

bd_count_or_error_t SlicingBlockDevice::write(const void *p, bd_block_t block, bd_count_t count)
{
    if (block + count > _stop - _start) {
        return BD_ERROR_PARAMETER;
    }

    return _bd->write(p, block - _start, count);
}

bd_error_t SlicingBlockDevice::sync()
{
    return _bd->sync();
}

bd_error_t SlicingBlockDevice::status()
{
    return _bd->status();
}

bd_size_t SlicingBlockDevice::get_block_size()
{
    return _bd->get_block_size();
}

bd_count_t SlicingBlockDevice::get_block_count()
{
    return _stop - _start;
}
