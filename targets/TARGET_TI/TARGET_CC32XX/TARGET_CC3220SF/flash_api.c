/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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

#if DEVICE_FLASH

#include "stdbool.h"
#include "ti/devices/cc32xx/inc/hw_types.h"
#include "flash_api.h"
#include "ti/devices/cc32xx/driverlib/flash.h"

#define CC3200_FLASH_SECTOR_SIZE 0x800
#define CC3200_FLASH_PAGE_SIZE   0x4
#define CC3200_FLASH_START_ADDRESS 0x01000000
#define CC3200_FLASH_SIZE        (1024*1024)

int32_t flash_init(flash_t *obj)
{
    return 0;
}

int32_t flash_free(flash_t *obj)
{
    //FlashDisable();
    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    return FlashErase(address);
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    return FlashProgram((unsigned long *)data, (unsigned long)address,
                        (unsigned long)size);
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if ((address >= CC3200_FLASH_START_ADDRESS) && address < (CC3200_FLASH_START_ADDRESS + CC3200_FLASH_SIZE)) {
        return CC3200_FLASH_SECTOR_SIZE;
    } else {
        return MBED_FLASH_INVALID_SIZE;
    }
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return CC3200_FLASH_PAGE_SIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return CC3200_FLASH_START_ADDRESS;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return CC3200_FLASH_SIZE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}
#endif
