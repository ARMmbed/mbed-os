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

#include "flash_api.h"
#include "mbed_critical.h"

#if DEVICE_FLASH
#include "mbed_assert.h"
#include "cmsis.h"

#ifndef FLASH_SIZE
#define FLASH_SIZE (uint32_t)(*((uint16_t *)FLASHSIZE_BASE) * 1024U)
#endif

// Minimum number of bytes to be programmed at a time
#define MIN_PROG_SIZE (4U)

static int32_t flash_unlock(void)
{
    /* Allow Access to Flash control registers and user Flash */
    if (HAL_FLASH_Unlock()) {
        return -1;
    } else {
        return 0;
    }
}

static int32_t flash_lock(void)
{
    /* Disable the Flash option control register access (recommended to protect
    the option Bytes against possible unwanted operations) */
    if (HAL_FLASH_Lock()) {
        return -1;
    } else {
        return 0;
    }
}

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

    if (!(IS_FLASH_PROGRAM_ADDRESS(address))) {
        return -1;
    }

    if (flash_unlock() != HAL_OK) {
        return -1;
    }

    // Clear Flash status register's flags
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_OPTVERR);

    /* MBED HAL erases 1 sector at a time */
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = address;
    EraseInitStruct.NbPages     = 1;

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
        status = -1;
    }

    flash_lock();

    return status;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    uint32_t StartAddress = 0;
    int32_t status = 0;

    if (!(IS_FLASH_PROGRAM_ADDRESS(address))) {
        return -1;
    }

    if ((size % MIN_PROG_SIZE) != 0) {
        return -1;
    }

    if (flash_unlock() != HAL_OK) {
        return -1;
    }

    /* Program the user Flash area word by word */
    StartAddress = address;

    /* HW needs an aligned address to program flash, which data parameter doesn't ensure */
    if ((uint32_t) data % 4 != 0) {

        volatile uint32_t data32;
        while (address < (StartAddress + size) && (status == 0)) {
            for (uint8_t i = 0; i < MIN_PROG_SIZE; i++) {
                *(((uint8_t *) &data32) + i) = *(data + i);
            }

            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data32) == HAL_OK) {
                address = address + MIN_PROG_SIZE;
                data = data + MIN_PROG_SIZE;
            } else {
                status = -1;
            }
        }
    } else { /*  case where data is aligned, so let's avoid any copy */
        while ((address < (StartAddress + size)) && (status == 0)) {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *((uint32_t*) data)) == HAL_OK) {
                address = address + MIN_PROG_SIZE;
                data = data + MIN_PROG_SIZE;
            } else {
                status = -1;
            }
        }
    }

    flash_lock();

    return status;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if (!(IS_FLASH_PROGRAM_ADDRESS(address))) {
        return MBED_FLASH_INVALID_SIZE;
    } else {
        return FLASH_PAGE_SIZE;
    }
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return MIN_PROG_SIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_BASE;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return FLASH_SIZE;
}

#endif
