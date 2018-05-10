/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include "FlashIAP.h"
#include "mbed_assert.h"


#ifdef DEVICE_FLASH

namespace mbed {

SingletonPtr<PlatformMutex> FlashIAP::_mutex;

static inline bool is_aligned(uint32_t number, uint32_t alignment)
{
    if ((number % alignment) != 0) {
        return false;
    } else {
        return true;
    }
}

FlashIAP::FlashIAP()
{

}

FlashIAP::~FlashIAP()
{

}

int FlashIAP::init()
{
    int ret = 0;
    _mutex->lock();
    if (flash_init(&_flash)) {
        ret = -1;
    }
    uint32_t page_size = get_page_size();
    _page_buf = new uint8_t[page_size];

    _mutex->unlock();
    return ret;
}

int FlashIAP::deinit()
{
    int ret = 0;
    _mutex->lock();
    if (flash_free(&_flash)) {
        ret = -1;
    }
    delete[] _page_buf;
    _mutex->unlock();
    return ret;
}


int FlashIAP::read(void *buffer, uint32_t addr, uint32_t size)
{
    int32_t ret = -1;
    _mutex->lock();
    ret = flash_read(&_flash, addr, (uint8_t *) buffer, size);
    _mutex->unlock();
    return ret;
}

int FlashIAP::program(const void *buffer, uint32_t addr, uint32_t size)
{
    uint32_t page_size = get_page_size();
    uint32_t flash_size = flash_get_size(&_flash);
    uint32_t flash_start_addr = flash_get_start_address(&_flash);
    uint32_t chunk, prog_size;
    const uint8_t *buf = (uint8_t *) buffer;
    const uint8_t *prog_buf;

    // addr should be aligned to page size
    if (!is_aligned(addr, page_size) || (!buffer) ||
        ((addr + size) > (flash_start_addr + flash_size))) {
        return -1;
    }

    int ret = 0;
    _mutex->lock();
    while (size) {
        uint32_t current_sector_size = flash_get_sector_size(&_flash, addr);
        bool unaligned_src = (((size_t) buf / sizeof(uint32_t) * sizeof(uint32_t)) != (size_t) buf);
        chunk = std::min(current_sector_size - (addr % current_sector_size), size);
        // Need to use the internal page buffer in any of these two cases:
        // 1. Size is not page aligned
        // 2. Source buffer is not aligned to uint32_t. This is not supported by many targets (although
        //    the pointer they accept is of uint8_t).
        if (unaligned_src || (chunk < page_size)) {
            chunk = std::min(chunk, page_size);
            memcpy(_page_buf, buf, chunk);
            if (chunk < page_size) {
                memset(_page_buf + chunk, 0xFF, page_size - chunk);
            }
            prog_buf = _page_buf;
            prog_size = page_size;
        } else {
            chunk = chunk / page_size * page_size;
            prog_buf = buf;
            prog_size = chunk;
        }
        if (flash_program_page(&_flash, addr, prog_buf, prog_size)) {
            ret = -1;
            break;
        }
        size -= chunk;
        addr += chunk;
        buf += chunk;
    }
    _mutex->unlock();

    return ret;
}

bool FlashIAP::is_aligned_to_sector(uint32_t addr, uint32_t size)
{
    uint32_t current_sector_size = flash_get_sector_size(&_flash, addr);
    if (!is_aligned(size, current_sector_size) ||
        !is_aligned(addr, current_sector_size)) {
        return false;
    } else {
        return true;
    }
}

int FlashIAP::erase(uint32_t addr, uint32_t size)
{
    uint32_t current_sector_size;
    uint32_t flash_size = flash_get_size(&_flash);
    uint32_t flash_start_addr = flash_get_start_address(&_flash);
    uint32_t flash_end_addr = flash_start_addr + flash_size;
    uint32_t erase_end_addr = addr + size;

    if (erase_end_addr > flash_end_addr) {
        return -1;
    } else if (erase_end_addr < flash_end_addr){
        uint32_t following_sector_size = flash_get_sector_size(&_flash, erase_end_addr);
        if (!is_aligned(erase_end_addr, following_sector_size)) {
            return -1;
        }
    }

    int32_t ret = 0;
    _mutex->lock();
    while (size) {
        ret = flash_erase_sector(&_flash, addr);
        if (ret != 0) {
            ret = -1;
            break;
        }
        current_sector_size = flash_get_sector_size(&_flash, addr);
        size -= current_sector_size;
        addr += current_sector_size;
    }
    _mutex->unlock();
    return ret;
}

uint32_t FlashIAP::get_page_size() const
{
    return flash_get_page_size(&_flash);
}

uint32_t FlashIAP::get_sector_size(uint32_t addr) const
{
    return flash_get_sector_size(&_flash, addr);
}

uint32_t FlashIAP::get_flash_start() const
{
    return flash_get_start_address(&_flash);
}

uint32_t FlashIAP::get_flash_size() const
{
    return flash_get_size(&_flash);
}

}

#endif
