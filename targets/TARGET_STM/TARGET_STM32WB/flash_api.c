/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * Copyright (c) 2019 STMicroelectronics
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

#if DEVICE_FLASH

#include "flash_api.h"
#include "flash_data.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include "cmsis.h"

/*  Family specific include for WB with HW semaphores */
#include "hw.h"
#include "hw_conf.h"

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
    return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
}

/** Initialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_init(flash_t *obj)
{
    return 0;
}

/** Uninitialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_free(flash_t *obj)
{
    return 0;
}

/** Erase one sector starting at defined address
 *
 * The address should be at sector boundary. This function does not do any check for address alignments
 * @param obj The flash object
 * @param address The sector starting address
 * @return 0 for success, -1 for error
 */
int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    uint32_t PageNumber = 0;
    uint32_t PAGEError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

#if defined(CFG_HW_FLASH_SEMID)
    /*  In case RNG is a shared ressource, get the HW semaphore first */
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_FLASH_SEMID));
#endif

    /* Unlock the Flash to enable the flash control register access */
    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    /* Clear OPTVERR bit set on virgin samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    /* Get the page number associated to the address */
    PageNumber = GetPage(address);

    /* MBED HAL erases 1 page at a time */
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Page      = PageNumber;
    EraseInitStruct.NbPages   = 1;

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
        status = -1;
    }

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) */
    HAL_FLASH_Lock();

#if defined(CFG_HW_FLASH_SEMID)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_FLASH_SEMID, 0);
#endif

    return status;
}

/** Program one page starting at defined address
 *
 * The page should be at page boundary, should not cross multiple sectors.
 * This function does not do any check for address alignments or if size
 * is aligned to a page size.
 * @param obj The flash object
 * @param address The sector starting address
 * @param data The data buffer to be programmed
 * @param size The number of bytes to program
 * @return 0 for success, -1 for error
 */
int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    uint32_t StartAddress = 0;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

    if ((size % 8) != 0) {
        return -1;
    }

#if defined(CFG_HW_FLASH_SEMID)
    /*  In case RNG is a shared ressource, get the HW semaphore first */
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_FLASH_SEMID));
#endif

    /* Unlock the Flash to enable the flash control register access */
    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    /* Program the user Flash area word by word */
    StartAddress = address;

    /*  HW needs an aligned address to program flash, which data parameters doesn't ensure */
    if ((uint32_t) data % 8 != 0) { // Data is not aligned, copy data in a temp buffer before programming it
        volatile uint64_t data64;
        while ((address < (StartAddress + size)) && (status == 0)) {
            for (uint8_t i = 0; i < 8; i++) {
                *(((uint8_t *) &data64) + i) = *(data + i);
            }
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data64) == HAL_OK) {
                address = address + 8;
                data = data + 8;
            } else {
                status = -1;
            }
        }
    } else { // Data is aligned, so let's avoid any copy
        while ((address < (StartAddress + size)) && (status == 0)) {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, *((uint64_t *) data)) == HAL_OK) {
                address = address + 8;
                data = data + 8;
            } else {
                status = -1;
            }
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) */
    HAL_FLASH_Lock();

#if defined(CFG_HW_FLASH_SEMID)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_FLASH_SEMID, 0);
#endif

    return status;
}

/** Get sector size
 *
 * @param obj The flash object
 * @param address The sector starting address
 * @return The size of a sector
 */
uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    /*  considering 1 sector = 1 page */
    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return MBED_FLASH_INVALID_SIZE;
    } else {
        return FLASH_PAGE_SIZE;
    }
}

/** Get page size
 *
 * @param obj The flash object
 * @return Minimum programmable page size in bytes
 */
uint32_t flash_get_page_size(const flash_t *obj)
{
    return 8;
}

/** Get start address for the flash region
 *
 * @param obj The flash object
 * @return The start address for the flash region
 */
uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_BASE;
}

/** Get the flash region size
 *
 * @param obj The flash object
 * @return The flash region size
 */
uint32_t flash_get_size(const flash_t *obj)
{
    return FLASH_SIZE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
