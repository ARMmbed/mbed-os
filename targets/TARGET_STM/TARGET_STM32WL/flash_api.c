/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if DEVICE_FLASH

#include "flash_api.h"
#include "platform/mbed_critical.h"

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

    /* Unlock the Flash to enable the flash control register access */
    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    core_util_critical_section_enter();

    /* Clear OPTVERR bit set on virgin samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    /* Get the page number associated to the address */
    PageNumber = GetPage(address);
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Page      = PageNumber;
    EraseInitStruct.NbPages   = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
        status = -1;
    }

    core_util_critical_section_exit();

    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_CFGBSY));

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) */
    HAL_FLASH_Lock();

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

    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_CFGBSY));

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) */
    HAL_FLASH_Lock();

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
