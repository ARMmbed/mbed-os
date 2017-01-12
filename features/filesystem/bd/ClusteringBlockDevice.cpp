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

#include "ClusteringBlockDevice.h"


ClusteringBlockDevice::ClusteringBlockDevice(BlockDevice *bd, bd_size_t block_size)
    : _bd(bd), _block_size(block_size), _cluster(1)
{
}

bd_error_t ClusteringBlockDevice::init()
{
    bd_error_t err = _bd->init();
    if (err) {
        return err;
    }

    bd_size_t raw_size = _bd->get_block_size();
    _cluster = _block_size / raw_size;

    // Check for divisible block size
    if (_cluster * raw_size != _block_size) {
        return BD_ERROR_PARAMETER;
    }

    return 0;
}

bd_error_t ClusteringBlockDevice::deinit()
{
    return _bd->deinit();
}

bd_count_or_error_t ClusteringBlockDevice::read(void *p, bd_block_t block, bd_count_t count)
{
    bd_count_or_error_t result = _bd->read(p, block*_cluster, count*_cluster);
    if (result > 0) {
        result /= _cluster;
    }
    return result;
}

bd_count_or_error_t ClusteringBlockDevice::write(const void *p, bd_block_t block, bd_count_t count)
{
    bd_count_or_error_t result = _bd->write(p, block*_cluster, count*_cluster);
    if (result > 0) {
        result /= _cluster;
    }
    return result;
}

bd_error_t ClusteringBlockDevice::sync()
{
    return _bd->sync();
}

bd_error_t ClusteringBlockDevice::status()
{
    return _bd->status();
}

bd_size_t ClusteringBlockDevice::get_block_size()
{
    return _bd->get_block_size() * _cluster;
}

bd_count_t ClusteringBlockDevice::get_block_count()
{
    return _bd->get_block_count() / _cluster;
}
