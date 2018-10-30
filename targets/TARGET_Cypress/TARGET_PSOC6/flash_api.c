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
#include "drivers/peripheral/flash/cy_flash.h"

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
    if (Cy_Flash_ProgramRow(address, (const uint32_t *)data) != CY_FLASH_DRV_SUCCESS) {
        status = -1;
    }

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
    return CY_FLASH_SIZEOF_ROW;
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

    return 0xFF;
}

#endif // DEVICE_FLASH
