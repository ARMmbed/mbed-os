/*
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
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

#include "flash_api.h"
#include <string.h>

int32_t flash_init(flash_t *obj)
{
    return 0;
}

int32_t flash_free(flash_t *obj)
{
    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    if (!ISADDRFLASH(address)) {
        return -1;
    }
    uint32_t status = am_hal_flash_page_erase(AM_HAL_FLASH_PROGRAM_KEY, AM_HAL_FLASH_ADDR2INST(address), AM_HAL_FLASH_ADDR2PAGE(address));
    if (status != AM_HAL_STATUS_SUCCESS) {
        return -1;
    }
    return 0;
}

int32_t flash_read(flash_t *obj, uint32_t address, uint8_t *data, uint32_t size)
{
    memcpy(data, (void *)address, size);
    return 0;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    if (address & 0x03) {
        return -1;
    }
    if (((uint32_t)data) & 0x03) {
        return -1;
    }
    uint32_t words = (size + 3) / 4;
    uint32_t status = am_hal_flash_program_main(AM_HAL_FLASH_PROGRAM_KEY, (uint32_t *)data, (uint32_t *)address, words);
    if (status != AM_HAL_STATUS_SUCCESS) {
        return -1;
    }
    return 0;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if (address > AM_HAL_FLASH_LARGEST_VALID_ADDR) {
        return -1;
    }
    return AM_HAL_FLASH_PAGE_SIZE;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return 4;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return AM_HAL_FLASH_ADDR;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return AM_HAL_FLASH_TOTAL_SIZE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    return 0xFF;
}
