/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
#include "flash_ext.h"

#define FLASH_START       (SPI_FLASH_BASE + FLASH_OFS_START)
#define FLASH_END         (SPI_FLASH_BASE + FLASH_OFS_END)
#define FLASH_OFS(addr)   ((addr) - SPI_FLASH_BASE)

int32_t flash_init(flash_t *obj)
{
    __flash_ext_turnon();

    return 0;
}

int32_t flash_free(flash_t *obj)
{
    __flash_ext_turnoff();

    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    __flash_ext_erase_sector(obj, FLASH_OFS(address));

    return 0;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    return __flash_ext_stream_write(obj, FLASH_OFS(address), size, data);
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if (address < FLASH_START || address >= FLASH_END)
        return 0;

    return FLASH_SECTOR_SIZE;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return FLASH_PAGE_SIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_START;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return FLASH_SIZE;
}

