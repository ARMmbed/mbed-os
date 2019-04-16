/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "device.h"
#include "flash_api.h"
#include "cy_flash.h"
#include <string.h>

#if DEVICE_FLASH

int32_t flash_init(flash_t *obj)
{
    (void)(obj);
    return 0;
}

int32_t flash_free(flash_t *obj)
{
    (void)(obj);
    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    (void)(obj);
    int32_t status = 0;
    if (Cy_Flash_EraseRow(address) != CY_FLASH_DRV_SUCCESS) {
        status = -1;
    }

    return status;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    (void)(obj);
    int32_t status = 0;
    static uint32_t prog_buf[CY_FLASH_SIZEOF_ROW / sizeof(uint32_t)];
    while (size) {
        uint32_t offset = address % CY_FLASH_SIZEOF_ROW;
        uint32_t chunk_size;
        if (offset + size > CY_FLASH_SIZEOF_ROW) {
            chunk_size = CY_FLASH_SIZEOF_ROW - offset;
        } else {
            chunk_size = size;
        }
        uint32_t row_address = address / CY_FLASH_SIZEOF_ROW * CY_FLASH_SIZEOF_ROW;
        memcpy(prog_buf, (const void *)row_address, CY_FLASH_SIZEOF_ROW);
        memcpy((uint8_t *)prog_buf + offset, data, chunk_size);

        if (Cy_Flash_ProgramRow(row_address, (const uint32_t *)prog_buf) != CY_FLASH_DRV_SUCCESS) {
            status = -1;
        }
        address += chunk_size;
        size -= chunk_size;
    }

    Cy_SysLib_ClearFlashCacheAndBuffer();
    return status;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    (void)(obj);
    if ((address >= CY_FLASH_BASE) && (address < CY_FLASH_BASE + CY_FLASH_SIZE)) {
        return CY_FLASH_SIZEOF_ROW;
    }

    return MBED_FLASH_INVALID_SIZE;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    (void)(obj);
    return CY_FLASH_EFFECTIVE_PAGE_SIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    (void)(obj);
    return CY_FLASH_BASE;
}

uint32_t flash_get_size(const flash_t *obj)
{
    (void)(obj);
    return CY_FLASH_SIZE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0x00;
}

#endif // DEVICE_FLASH
