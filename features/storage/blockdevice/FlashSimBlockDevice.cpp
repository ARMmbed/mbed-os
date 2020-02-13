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

#include "FlashSimBlockDevice.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_atomic.h"
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include "mbed_assert.h"

namespace mbed {

static const bd_size_t min_blank_buf_size = 32;

static inline uint32_t align_up(bd_size_t val, bd_size_t size)
{
    return (((val - 1) / size) + 1) * size;
}

FlashSimBlockDevice::FlashSimBlockDevice(BlockDevice *bd, uint8_t erase_value) :
    _erase_value(erase_value), _blank_buf_size(0),
    _blank_buf(0), _bd(bd), _init_ref_count(0), _is_initialized(false)
{
    MBED_ASSERT(bd);
}

FlashSimBlockDevice::~FlashSimBlockDevice()
{
    deinit();
    delete[] _blank_buf;
}

int FlashSimBlockDevice::init()
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
    _blank_buf_size = align_up(min_blank_buf_size, _bd->get_program_size());
    if (!_blank_buf) {
        _blank_buf = new uint8_t[_blank_buf_size];
        memset(_blank_buf, 0, _blank_buf_size);
        MBED_ASSERT(_blank_buf);
    }

    _is_initialized = true;
    return BD_ERROR_OK;

fail:
    _is_initialized = false;
    _init_ref_count = 0;
    return err;
}

int FlashSimBlockDevice::deinit()
{
    if (!_is_initialized) {
        return BD_ERROR_OK;
    }

    uint32_t val = core_util_atomic_decr_u32(&_init_ref_count, 1);

    if (val) {
        return BD_ERROR_OK;
    }

    _is_initialized = false;
    return _bd->deinit();
}

int FlashSimBlockDevice::sync()
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _bd->sync();
}

bd_size_t FlashSimBlockDevice::get_read_size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_read_size();
}

bd_size_t FlashSimBlockDevice::get_program_size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_program_size();
}

bd_size_t FlashSimBlockDevice::get_erase_size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_erase_size();
}

bd_size_t FlashSimBlockDevice::get_erase_size(bd_addr_t addr) const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_erase_size(addr);
}

bd_size_t FlashSimBlockDevice::size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->size();
}

int FlashSimBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _bd->read(b, addr, size);
}

int FlashSimBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    if (!is_valid_program(addr, size)) {
        return BD_ERROR_DEVICE_ERROR;
    }

    bd_addr_t curr_addr = addr;
    bd_size_t curr_size = size;

    const uint8_t *buf = (const uint8_t *) b;
    while (curr_size) {
        bd_size_t read_size = std::min(_blank_buf_size, curr_size);
        int ret = _bd->read(_blank_buf, curr_addr, read_size);
        if (ret) {
            return ret;
        }
        for (bd_size_t i = 0; i < read_size; i++) {
            // Allow either programming on blanks or programming the same value
            // (as real flash devices do)
            if ((_blank_buf[i] != _erase_value) && (_blank_buf[i] != *buf)) {
                return BD_ERROR_NOT_ERASED;
            }
            buf++;
        }
        curr_addr += read_size;
        curr_size -= read_size;
    }

    return _bd->program(b, addr, size);
}

int FlashSimBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    if (!is_valid_erase(addr, size)) {
        return BD_ERROR_DEVICE_ERROR;
    }

    bd_addr_t curr_addr = addr;
    bd_size_t curr_size = size;

    memset(_blank_buf, _erase_value, (unsigned int) _blank_buf_size);

    while (curr_size) {
        bd_size_t prog_size = std::min(_blank_buf_size, curr_size);
        int ret = _bd->program(_blank_buf, curr_addr, prog_size);
        if (ret) {
            return ret;
        }
        curr_addr += prog_size;
        curr_size -= prog_size;
    }

    return BD_ERROR_OK;
}

int FlashSimBlockDevice::get_erase_value() const
{
    return _erase_value;
}

const char *FlashSimBlockDevice::get_type() const
{
    if (_bd != NULL) {
        return _bd->get_type();
    }

    return NULL;
}

} // namespace mbed

