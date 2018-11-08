/*
 * Copyright (c) 2017-2018 ARM Limited
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "device.h"
#include "flash_api.h"
#include "memory_zones.h"

/* CM3DS is deployed on MPS2 board with an FPGA image (AN511).
 * It doesn't have physical flash memory,the implementation emulates
 * flash over SRAM.
 */
#define FLASH_PAGE_SIZE   256
#define FLASH_OFS_START   ZBT_SSRAM1_START
#define FLASH_SECTOR_SIZE 0x1000
#define FLASH_OFS_END     (FLASH_OFS_START + FLASH_SIZE)

int32_t flash_init(flash_t *obj)
{
    (void)obj;

    return 0;
}

int32_t flash_free(flash_t *obj)
{
    (void)obj;

    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    (void)obj;

    memset((void *)address, 0xff, FLASH_SECTOR_SIZE);

    return 0;
}

int32_t flash_read(flash_t *obj, uint32_t address,
                   uint8_t *data, uint32_t size)
{
    (void)obj;

    memcpy(data, (void *)address, size);

    return 0;
}

int32_t flash_program_page(flash_t *obj, uint32_t address,
                           const uint8_t *data, uint32_t size)
{
    (void)obj;

    memcpy((void *)address, data, size);

    return 0;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    (void)obj;

    if (address < FLASH_OFS_START || address >= FLASH_OFS_END) {
        return MBED_FLASH_INVALID_SIZE;
    }

    return FLASH_SECTOR_SIZE;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    (void)obj;

    return FLASH_PAGE_SIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    (void)obj;

    return FLASH_OFS_START;
}

uint32_t flash_get_size(const flash_t *obj)
{
    (void)obj;

    return FLASH_SIZE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}
