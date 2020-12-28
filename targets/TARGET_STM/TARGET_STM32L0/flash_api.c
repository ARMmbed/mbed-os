/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 * Copyright (c) 2017 STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
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

#include "flash_api.h"
#include "mbed_critical.h"

#if DEVICE_FLASH
#include "mbed_assert.h"
#include "cmsis.h"

/*  L0 targets embed 32 pages sectors */
#define NUM_PAGES_IN_SECTOR 32

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
    uint32_t PAGEError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {

        return -1;
    }

    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    core_util_critical_section_enter();

    /* Clear OPTVERR bit set on virgin samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    /* MBED HAL erases 1 sector at a time */
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = address;
    EraseInitStruct.NbPages     = NUM_PAGES_IN_SECTOR;

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
        status = -1;
    }

    core_util_critical_section_exit();

    if (HAL_FLASH_Lock() != HAL_OK) {
        return -1;
    }

    return status;

}

int32_t flash_program_page(flash_t *obj, uint32_t address,
                           const uint8_t *data, uint32_t size)
{
    uint32_t StartAddress = 0;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

    if ((size % 4) != 0) {
        /* L4 flash devices can only be programmed 64bits/8 bytes at a time */
        return -1;
    }

    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    core_util_critical_section_enter();

    /* Program the user Flash area word by word */
    StartAddress = address;

    /*  HW needs an aligned address to program flash, which data
     *  parameters doesn't ensure  */
    if ((uint32_t) data % 4 != 0) {
        volatile uint32_t data32;
        while ((address < (StartAddress + size)) && (status == 0)) {
            for (uint8_t i = 0; i < 4; i++) {
                *(((uint8_t *) &data32) + i) = *(data + i);
            }

            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data32) == HAL_OK) {
                address = address + 4;
                data = data + 4;
            } else {
                status = -1;
            }
        }
    } else { /*  case where data is aligned, so let's avoid any copy */
        while ((address < (StartAddress + size)) && (status == 0)) {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *((uint32_t *) data)) == HAL_OK) {
                address = address + 4;
                data = data + 4;
            } else {
                status = -1;
            }
        }
    }

    core_util_critical_section_exit();

    if (HAL_FLASH_Lock() != HAL_OK) {
        return -1;
    }

    return status;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return MBED_FLASH_INVALID_SIZE;
    } else {
        /*  sector composed of N pages */
        return (NUM_PAGES_IN_SECTOR * FLASH_PAGE_SIZE);
    }
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    /*  Page size is the minimum programable size, which 4 bytes */
    return 4;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_BASE;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return FLASH_SIZE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0x0;
}

#endif
