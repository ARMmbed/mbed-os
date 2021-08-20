/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
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
#include "flash_data.h"
#include "platform/mbed_critical.h"

static uint32_t GetSector(uint32_t Address);
static uint32_t GetSectorSize(uint32_t Sector);

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
    static FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t FirstSector;
    uint32_t SectorError = 0;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    core_util_critical_section_enter();

    /* Get the 1st sector to erase */
    FirstSector = GetSector(address);

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = 1;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        status = -1;
    }

    core_util_critical_section_exit();

    if (HAL_FLASH_Lock() != HAL_OK) {
        return -1;
    }

    return status;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
       you have to make sure that these data are rewritten before they are accessed during code
       execution. If this cannot be done safely, it is recommended to flush the caches by setting the
       DCRST and ICRST bits in the FLASH_CR register. */
    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();

    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();

    while ((size > 0) && (status == 0)) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address, (uint64_t)*data) != HAL_OK) {
            status = -1;
        } else {
            size--;
            address++;
            data++;
        }
    }

    if (HAL_FLASH_Lock() != HAL_OK) {
        return -1;
    }

    return status;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return MBED_FLASH_INVALID_SIZE;
    }

    return (GetSectorSize(GetSector(address)));
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    // Flash of STM32F4 devices can be programed 1 byte at a time
    return (1);
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_BASE;
}
uint32_t flash_get_size(const flash_t *obj)
{
    return FLASH_SIZE;
}

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t address)
{
    uint32_t sector = 0;
    uint32_t tmp = address - ADDR_FLASH_SECTOR_0;
    /* This function supports 1Mb and 2Mb flash sizes */
#if defined(ADDR_FLASH_SECTOR_16)
    if (address & 0x100000) { // handle 2nd bank
        /*  Sector will be at least 12 */
        sector = FLASH_SECTOR_12;
        tmp -= 0x100000;
        address -= 0x100000;
    }
#endif
    if (address < ADDR_FLASH_SECTOR_4) { // 16k sectorsize
        sector += tmp >> 14;
    }
#if defined(ADDR_FLASH_SECTOR_5)
    else if (address < ADDR_FLASH_SECTOR_5) { //64k sector size
        sector += FLASH_SECTOR_4;
    } else {
        sector += 4 + (tmp >> 17);
    }
#else
    // In case ADDR_FLASH_SECTOR_5 is not defined, sector 4 is the last one.
    else { //64k sector size
        sector += FLASH_SECTOR_4;
    }
#endif
    return sector;
}

/**
  * @brief  Gets sector Size
  * @param  None
  * @retval The size of a given sector
  */
static uint32_t GetSectorSize(uint32_t Sector)
{
    uint32_t sectorsize = 0x00;
#if defined(FLASH_SECTOR_16)
    if ((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || \
            (Sector == FLASH_SECTOR_3) || (Sector == FLASH_SECTOR_12) || (Sector == FLASH_SECTOR_13) || \
            (Sector == FLASH_SECTOR_14) || (Sector == FLASH_SECTOR_15)) {
        sectorsize = 16 * 1024;
    } else if ((Sector == FLASH_SECTOR_4) || (Sector == FLASH_SECTOR_16)) {
#else
    if ((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || \
            (Sector == FLASH_SECTOR_3)) {
        sectorsize = 16 * 1024;
    } else if (Sector == FLASH_SECTOR_4) {
#endif
        sectorsize = 64 * 1024;
    } else {
        sectorsize = 128 * 1024;
    }
    return sectorsize;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
