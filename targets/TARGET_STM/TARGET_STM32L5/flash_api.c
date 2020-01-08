/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "flash_api.h"

#if DEVICE_FLASH


/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
    uint32_t page = 0;

    if (READ_BIT(FLASH->OPTR, FLASH_OPTR_DBANK) != 0U) {
        if (Addr < (FLASH_BASE + FLASH_BANK_SIZE)) {
            /* Bank 1 */
            page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
        } else {
            /* Bank 2 */
            page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
        }
    } else {
        page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE_128_BITS;
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

    if (READ_BIT(FLASH->OPTR, FLASH_OPTR_DBANK) != 0U) {
        if (Addr < (FLASH_BASE + FLASH_BANK_SIZE)) {
            bank = FLASH_BANK_1;
        } else {
            bank = FLASH_BANK_2;
        }
    } else {
        bank = FLASH_BANK_1;
    }

    return bank;
}

/** Initialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_init(flash_t *obj)
{
#ifdef TARGET_TFM
/* TFM implementation needs dual bank configuration */
    if (READ_BIT(FLASH->OPTR, FLASH_OPTR_DBANK) != 0U) {
        return 0;
    } else {
        return -1;
    }
#else
    return 0;
#endif
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

    if ((address >= (flash_get_start_address(obj) + flash_get_size(obj))) || (address < flash_get_start_address(obj))) {
        return -1;
    }

    if (flash_unlock() != HAL_OK) {
        return -1;
    }

    /* Clear error programming flags */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

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

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
        return -1;
    }

    if (flash_lock() != HAL_OK) {
        return -1;
    }

    return 0;
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
    if ((address >= (flash_get_start_address(obj) + flash_get_size(obj))) || (address < flash_get_start_address(obj))) {
        return -1;
    }

    if ((size % 8) != 0) {
        /* L4 flash devices can only be programmed 64bits/8 bytes at a time */
        return -1;
    }

    if (flash_unlock() != HAL_OK) {
        return -1;
    }

    /* Clear error programming flags */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

    /* Program the user Flash area word by word */
    StartAddress = address;

    /*  HW needs an aligned address to program flash, which data
     *  parameters doesn't ensure  */
    if ((uint32_t) data % 4 != 0) {
        volatile uint64_t data64;
        while ((address < (StartAddress + size)) && (status == 0)) {
            for (uint8_t i = 0; i < 8; i++) {
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
                                  *((uint64_t *) data))
                    == HAL_OK) {
                address = address + 8;
                data = data + 8;
            } else {
                status = -1;
            }
        }
    }

    status = flash_unlock();

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
    if ((address >= (flash_get_start_address(obj) + flash_get_size(obj))) || (address < flash_get_start_address(obj))) {
        return MBED_FLASH_INVALID_SIZE;
    } else {
        if (READ_BIT(FLASH->OPTR, FLASH_OPTR_DBANK) != 0U) {
            return FLASH_PAGE_SIZE;
        } else {
            return FLASH_PAGE_SIZE_128_BITS;
        }
    }
}

/** Get page size
 *
 * @param obj The flash object
 * @return The size of a page
 */
uint32_t flash_get_page_size(const flash_t *obj)
{
    /*  Page size is the minimum programable size, which 8 bytes */
    return 8;
}

/** Get start address for the flash region
 *
 * @param obj The flash object
 * @return The start address for the flash region
 */
uint32_t flash_get_start_address(const flash_t *obj)
{
#if (DOMAIN_NS == 1)
    return NS_CODE_START;
#else
    return FLASH_BASE;
#endif
}

/** Get the flash region size
 *
 * @param obj The flash object
 * @return The flash region size
 */
uint32_t flash_get_size(const flash_t *obj)
{
#if (DOMAIN_NS == 1)
    return NS_CODE_SIZE;
#else
    return FLASH_SIZE;
#endif
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
