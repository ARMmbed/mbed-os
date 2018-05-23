/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "BufferedBlockDevice.h"
#include "mbed_assert.h"
#include <algorithm>
#include <string.h>

static inline uint32_t align_down(bd_size_t val, bd_size_t size)
{
    return val / size * size;
}

BufferedBlockDevice::BufferedBlockDevice(BlockDevice *bd)
    : _bd(bd), _bd_program_size(0), _curr_aligned_addr(0), _flushed(true), _cache(0)
{
}

BufferedBlockDevice::~BufferedBlockDevice()
{
    deinit();
}

int BufferedBlockDevice::init()
{
    int err = _bd->init();
    if (err) {
        return err;
    }

    _bd_program_size = _bd->get_program_size();

    if (!_cache) {
        _cache = new uint8_t[_bd_program_size];
    }

    _curr_aligned_addr = _bd->size();
    _flushed = true;

    return 0;
}

int BufferedBlockDevice::deinit()
{
    delete[] _cache;
    _cache = 0;
    return _bd->deinit();
}

int BufferedBlockDevice::flush()
{
    if (!_flushed) {
        int ret = _bd->program(_cache, _curr_aligned_addr, _bd_program_size);
        if (ret) {
            return ret;
        }
        _flushed = true;
    }
    return 0;
}

int BufferedBlockDevice::sync()
{
    int ret = flush();
    if (ret) {
        return ret;
    }
    return _bd->sync();
}

int BufferedBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(_cache);
    bool moved_unit = false;

    bd_addr_t aligned_addr = align_down(addr, _bd_program_size);

    uint8_t *buf = static_cast<uint8_t *> (b);

    if (aligned_addr != _curr_aligned_addr) {
        // Need to flush if moved to another program unit
        flush();
        _curr_aligned_addr = aligned_addr;
        moved_unit = true;
    }

    while (size) {
        _curr_aligned_addr = align_down(addr, _bd_program_size);
        if (moved_unit) {
            int ret = _bd->read(_cache, _curr_aligned_addr, _bd_program_size);
            if (ret) {
                return ret;
            }
        }
        bd_addr_t offs_in_buf = addr - _curr_aligned_addr;
        bd_size_t chunk = std::min(_bd_program_size - offs_in_buf, size);
        memcpy(buf, _cache + offs_in_buf, chunk);
        moved_unit = true;
        buf += chunk;
        addr += chunk;
        size -= chunk;
    }

    return 0;
}

int BufferedBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(_cache);
    int ret;
    bool moved_unit = false;

    bd_addr_t aligned_addr = align_down(addr, _bd_program_size);

    const uint8_t *buf = static_cast <const uint8_t *> (b);

    // Need to flush if moved to another program unit
    if (aligned_addr != _curr_aligned_addr) {
        flush();
        _curr_aligned_addr = aligned_addr;
        moved_unit = true;
    }

    while (size) {
        _curr_aligned_addr = align_down(addr, _bd_program_size);
        bd_addr_t offs_in_buf = addr - _curr_aligned_addr;
        bd_size_t chunk = std::min(_bd_program_size - offs_in_buf, size);
        const uint8_t *prog_buf;
        if (chunk < _bd_program_size) {
            // If moved a unit, and program doesn't cover entire unit, it means we don't have the entire
            // program unit cached - need to complete it from underlying BD
            if (moved_unit) {
                ret = _bd->read(_cache, _curr_aligned_addr, _bd_program_size);
                if (ret) {
                    return ret;
                }
            }
            memcpy(_cache + offs_in_buf, buf, chunk);
            prog_buf = _cache;
        } else {
            // No need to copy data to our cache on each iteration. Just make sure it's updated
            // on the last iteration, when size is not greater than program size (can't be smaller, as
            // this is covered in the previous condition).
            prog_buf = buf;
            if (size == _bd_program_size) {
                memcpy(_cache, buf, _bd_program_size);
            }
        }

        // Don't flush on the last iteration, just on all preceding ones.
        if (size > chunk) {
            ret = _bd->program(prog_buf, _curr_aligned_addr, _bd_program_size);
            if (ret) {
                return ret;
            }
            _bd->sync();
        } else {
            _flushed = false;
        }

        moved_unit = true;
        buf += chunk;
        addr += chunk;
        size -= chunk;
    }

    return 0;
}

int BufferedBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_erase(addr, size));
    return _bd->erase(addr, size);
}

int BufferedBlockDevice::trim(bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_erase(addr, size));

    if ((_curr_aligned_addr >= addr) && (_curr_aligned_addr <= addr + size)) {
        _flushed = true;
        _curr_aligned_addr = _bd->size();
    }
    return _bd->trim(addr, size);
}

bd_size_t BufferedBlockDevice::get_read_size() const
{
    return 1;
}

bd_size_t BufferedBlockDevice::get_program_size() const
{
    return 1;
}

bd_size_t BufferedBlockDevice::get_erase_size() const
{
    return _bd->get_erase_size();
}

bd_size_t BufferedBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return _bd->get_erase_size(addr);
}

int BufferedBlockDevice::get_erase_value() const
{
    return _bd->get_erase_value();
}

bd_size_t BufferedBlockDevice::size() const
{
    return _bd->size();
}
