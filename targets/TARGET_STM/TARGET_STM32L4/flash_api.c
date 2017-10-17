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

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
    uint32_t page = 0;

    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE)) {
        /* Bank 1 */
        page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
    } else {
        /* Bank 2 */
        page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
    }

    return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
    uint32_t bank = 0;
#if defined(SYSCFG_MEMRMP_FB_MODE)
    if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0) {
        /* No Bank swap */
        if (Addr < (FLASH_BASE + FLASH_BANK_SIZE)) {
            bank = FLASH_BANK_1;
        } else {
            bank = FLASH_BANK_2;
        }
    } else {
        /* Bank swap */
        if (Addr < (FLASH_BASE + FLASH_BANK_SIZE)) {
            bank = FLASH_BANK_2;
        } else {
            bank = FLASH_BANK_1;
        }
    }
#else
    /* Device like L432KC */
    bank = FLASH_BANK_1;
#endif

    return bank;
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

static int32_t flash_unlock(void)
{
    /* Allow Access to Flash control registers and user Falsh */
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

/** Erase one sector starting at defined address
 *
 * The address should be at sector boundary. This function does not do any check for address alignments
 * @param obj The flash object
 * @param address The sector starting address
 * @return 0 for success, -1 for error
 */
int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    uint32_t FirstPage = 0, BankNumber = 0;
    uint32_t PAGEError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {

        return -1;
    }

    if (flash_unlock() != HAL_OK) {
        return -1;
    }

      /* Clear OPTVERR bit set on virgin samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    /* Get the 1st page to erase */
    FirstPage = GetPage(address);
    /* MBED HAL erases 1 page  / sector at a time */
    /* Get the bank */
    BankNumber = GetBank(address);
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks       = BankNumber;
    EraseInitStruct.Page        = FirstPage;
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
int32_t flash_program_page(flash_t *obj, uint32_t address,
        const uint8_t *data, uint32_t size)
{
    uint32_t StartAddress = 0;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

    if ((size % 8) != 0) {
        /* L4 flash devices can only be programmed 64bits/8 bytes at a time */
        return -1;
    }

    if (flash_unlock() != HAL_OK) {
        return -1;
    }

    /* Program the user Flash area word by word */
    StartAddress = address;

    /*  HW needs an aligned address to program flash, which data
     *  parameters doesn't ensure  */
    if ((uint32_t) data % 4 != 0) {
        volatile uint64_t data64;
        while ((address < (StartAddress + size)) && (status == 0)) {
            for (uint8_t i =0; i < 8; i++) {
                *(((uint8_t *) &data64) + i) = *(data + i);
            }

            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data64)
                    == HAL_OK) {
                address = address + 8;
                data = data + 8;
            } else {
                status = -1;
            }
        }
    } else { /*  case where data is aligned, so let's avoid any copy */
        while ((address < (StartAddress + size)) && (status == 0)) {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address,
                        *((uint64_t*) data))
                    == HAL_OK) {
                address = address + 8;
                data = data + 8;
            } else {
                status = -1;
            }
        }
    }

    flash_lock();

    return status;
}

/** Get sector size
 *
 * @param obj The flash object
 * @param address The sector starting address
 * @return The size of a sector
 */
uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address) {
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
 * @param address The page starting address
 * @return The size of a page
 */
uint32_t flash_get_page_size(const flash_t *obj) {
    /*  Page size is the minimum programable size, which 8 bytes */
    return 8;
}

/** Get start address for the flash region
 *
 * @param obj The flash object
 * @return The start address for the flash region
 */
uint32_t flash_get_start_address(const flash_t *obj) {
    return FLASH_BASE;
}

/** Get the flash region size
 *
 * @param obj The flash object
 * @return The flash region size
 */
uint32_t flash_get_size(const flash_t *obj) {
    return FLASH_SIZE;
}

#endif
