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
#include "platform/mbed_assert.h"
#include "platform/mbed_atomic.h"
#include <algorithm>
#include <string.h>

namespace mbed {

static inline uint32_t align_down(bd_size_t val, bd_size_t size)
{
    return val / size * size;
}

BufferedBlockDevice::BufferedBlockDevice(BlockDevice *bd)
    : _bd(bd), _bd_program_size(0), _bd_read_size(0), _bd_size(0), _write_cache_addr(0), _write_cache_valid(false),
      _write_cache(0), _read_buf(0), _init_ref_count(0), _is_initialized(false)
{
    MBED_ASSERT(_bd);
}

BufferedBlockDevice::~BufferedBlockDevice()
{
    deinit();
}

int BufferedBlockDevice::init()
{
    uint32_t val = core_util_atomic_incr_u32(&_init_ref_count, 1);

    if (val != 1) {
        return BD_ERROR_OK;
    }

    int err = _bd->init();
    if (err) {
        return err;
    }

    _bd_read_size = _bd->get_read_size();
    _bd_program_size = _bd->get_program_size();
    _bd_size = _bd->size();

    if (!_write_cache) {
        _write_cache = new uint8_t[_bd_program_size];
    }

    if (!_read_buf) {
        _read_buf = new uint8_t[_bd_read_size];
    }

    invalidate_write_cache();

    _is_initialized = true;
    return BD_ERROR_OK;
}

int BufferedBlockDevice::deinit()
{
    if (!_is_initialized) {
        return BD_ERROR_OK;
    }

    // Flush out all data from buffers
    int err = sync();
    if (err) {
        return err;
    }

    uint32_t val = core_util_atomic_decr_u32(&_init_ref_count, 1);

    if (val) {
        return BD_ERROR_OK;
    }

    delete[] _write_cache;
    _write_cache = 0;
    delete[] _read_buf;
    _read_buf = 0;
    _is_initialized = false;
    return _bd->deinit();
}

int BufferedBlockDevice::flush()
{
    MBED_ASSERT(_write_cache);
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    if (_write_cache_valid) {
        int ret = _bd->program(_write_cache, _write_cache_addr, _bd_program_size);
        if (ret) {
            return ret;
        }
        invalidate_write_cache();
    }
    return 0;
}

void BufferedBlockDevice::invalidate_write_cache()
{
    _write_cache_addr = _bd_size;
    _write_cache_valid = false;
}

int BufferedBlockDevice::sync()
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    MBED_ASSERT(_write_cache);
    int ret = flush();
    if (ret) {
        return ret;
    }
    return _bd->sync();
}

int BufferedBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    MBED_ASSERT(_write_cache && _read_buf);

    if (!is_valid_read(addr, size)) {
        return BD_ERROR_DEVICE_ERROR;
    }

    // Common case - no need to involve write cache or read buffer
    if (_bd->is_valid_read(addr, size) &&
            ((addr + size <= _write_cache_addr) || (addr > _write_cache_addr + _bd_program_size))) {
        return _bd->read(b, addr, size);
    }

    uint8_t *buf = static_cast<uint8_t *>(b);

    // Read logic: Split read to chunks, according to whether we cross the write cache
    while (size) {
        bd_size_t chunk;
        bool read_from_bd = true;
        if (_write_cache_valid && addr < _write_cache_addr) {
            chunk = std::min(size, _write_cache_addr - addr);
        } else if (_write_cache_valid && (addr >= _write_cache_addr) && (addr < _write_cache_addr + _bd_program_size)) {
            // One case we need to take our data from cache
            chunk = std::min(size, _bd_program_size - addr % _bd_program_size);
            memcpy(buf, _write_cache + addr % _bd_program_size, chunk);
            read_from_bd = false;
        } else {
            chunk = size;
        }

        // Now, in case we read from the BD, make sure we are aligned with its read size.
        // If not, use read buffer as a helper.
        if (read_from_bd) {
            bd_size_t offs_in_read_buf = addr % _bd_read_size;
            int ret;
            if (offs_in_read_buf || (chunk < _bd_read_size)) {
                chunk = std::min(chunk, _bd_read_size - offs_in_read_buf);
                ret = _bd->read(_read_buf, addr - offs_in_read_buf, _bd_read_size);
                memcpy(buf, _read_buf + offs_in_read_buf, chunk);
            } else {
                chunk = align_down(chunk, _bd_read_size);
                ret = _bd->read(buf, addr, chunk);
            }
            if (ret) {
                return ret;
            }
        }

        buf += chunk;
        addr += chunk;
        size -= chunk;
    }

    return 0;
}

int BufferedBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    MBED_ASSERT(_write_cache);

    int ret;

    bd_addr_t aligned_addr = align_down(addr, _bd_program_size);

    const uint8_t *buf = static_cast <const uint8_t *>(b);

    // Need to flush if moved to another program unit
    if (aligned_addr != _write_cache_addr) {
        ret = flush();
        if (ret) {
            return ret;
        }
    }

    // Write logic: Keep data in cache as long as we don't reach the end of the program unit.
    // Otherwise, program to the underlying BD.
    while (size) {
        _write_cache_addr = align_down(addr, _bd_program_size);
        bd_addr_t offs_in_buf = addr - _write_cache_addr;
        bd_size_t chunk;
        if (offs_in_buf) {
            chunk = std::min(_bd_program_size - offs_in_buf, size);
        } else if (size >= _bd_program_size) {
            chunk = align_down(size, _bd_program_size);
        } else {
            chunk = size;
        }

        const uint8_t *prog_buf;
        if (chunk < _bd_program_size) {
            // If cache not valid, and program doesn't cover an entire unit, it means we need to
            // read it from the underlying BD
            if (!_write_cache_valid) {
                ret = _bd->read(_write_cache, _write_cache_addr, _bd_program_size);
                if (ret) {
                    return ret;
                }
            }
            memcpy(_write_cache + offs_in_buf, buf, chunk);
            prog_buf = _write_cache;
        } else {
            prog_buf = buf;
        }

        // Only program if we reached the end of a program unit
        if (!((offs_in_buf + chunk) % _bd_program_size)) {
            ret = _bd->program(prog_buf, _write_cache_addr, std::max(chunk, _bd_program_size));
            if (ret) {
                return ret;
            }
            invalidate_write_cache();
            ret = _bd->sync();
            if (ret) {
                return ret;
            }
        } else {
            _write_cache_valid = true;
        }

        buf += chunk;
        addr += chunk;
        size -= chunk;
    }

    return 0;
}

int BufferedBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    if (!is_valid_erase(addr, size)) {
        return BD_ERROR_DEVICE_ERROR;
    }

    if ((_write_cache_addr >= addr) && (_write_cache_addr <= addr + size)) {
        invalidate_write_cache();
    }
    return _bd->erase(addr, size);
}

int BufferedBlockDevice::trim(bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    if (!is_valid_erase(addr, size)) {
        return BD_ERROR_DEVICE_ERROR;
    }

    if ((_write_cache_addr >= addr) && (_write_cache_addr <= addr + size)) {
        invalidate_write_cache();
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
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_erase_size();
}

bd_size_t BufferedBlockDevice::get_erase_size(bd_addr_t addr) const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_erase_size(addr);
}

int BufferedBlockDevice::get_erase_value() const
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _bd->get_erase_value();
}

bd_size_t BufferedBlockDevice::size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd_size;
}

const char *BufferedBlockDevice::get_type() const
{
    return _bd->get_type();
}

} // namespace mbed
