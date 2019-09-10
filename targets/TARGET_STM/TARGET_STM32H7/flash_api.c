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

#if DEVICE_FLASH
#include "flash_api.h"
#include "flash_data.h"
#include "platform/mbed_critical.h"

static uint32_t GetSector(uint32_t Address);
static uint32_t GetSectorSize(uint32_t Sector);

int32_t flash_init(flash_t *obj)
{
    /* Clear pending flags (if any) */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR);
    return 0;
}

int32_t flash_free(flash_t *obj)
{
    return 0;
}


int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    /* Variable used for Erase procedure */
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorId;
    uint32_t SectorError = 0;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    /* Get the 1st sector to erase */
    SectorId = GetSector(address);

    /* Fill EraseInit structure */
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = SectorId;
    EraseInitStruct.NbSectors = 1;

    if (address < ADDR_FLASH_SECTOR_0_BANK2) {
        EraseInitStruct.Banks = FLASH_BANK_1;
        if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
            status =  -1;
        }
    } else {
        EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        EraseInitStruct.Banks = FLASH_BANK_2;
        if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
            status = -1;
        }
    }

    HAL_FLASH_Lock();

    return status;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data,
                           uint32_t size)
{
    uint32_t StartAddress = 0;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

    if ((size % 32) != 0) {
        /* H7 flash devices can only be programmed 256bits/32 bytes at a time */
        return -1;
    }

    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    StartAddress = address;
    while ((address < (StartAddress + size)) && (status == 0)) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, address, (uint32_t)data) == HAL_OK) {
            address = address + 32;
            data = data + 32;
        } else {
            status = -1;
        }
    }

    HAL_FLASH_Lock();

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
    return (32);
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
  * @param  Address: Flash address
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if (((Address < ADDR_FLASH_SECTOR_1_BANK1) && (Address >= ADDR_FLASH_SECTOR_0_BANK1)) || \
            ((Address < ADDR_FLASH_SECTOR_1_BANK2) && (Address >= ADDR_FLASH_SECTOR_0_BANK2))) {
        sector = FLASH_SECTOR_0;
    } else if (((Address < ADDR_FLASH_SECTOR_2_BANK1) && (Address >= ADDR_FLASH_SECTOR_1_BANK1)) || \
               ((Address < ADDR_FLASH_SECTOR_2_BANK2) && (Address >= ADDR_FLASH_SECTOR_1_BANK2))) {
        sector = FLASH_SECTOR_1;
    } else if (((Address < ADDR_FLASH_SECTOR_3_BANK1) && (Address >= ADDR_FLASH_SECTOR_2_BANK1)) || \
               ((Address < ADDR_FLASH_SECTOR_3_BANK2) && (Address >= ADDR_FLASH_SECTOR_2_BANK2))) {
        sector = FLASH_SECTOR_2;
    } else if (((Address < ADDR_FLASH_SECTOR_4_BANK1) && (Address >= ADDR_FLASH_SECTOR_3_BANK1)) || \
               ((Address < ADDR_FLASH_SECTOR_4_BANK2) && (Address >= ADDR_FLASH_SECTOR_3_BANK2))) {
        sector = FLASH_SECTOR_3;
    } else if (((Address < ADDR_FLASH_SECTOR_5_BANK1) && (Address >= ADDR_FLASH_SECTOR_4_BANK1)) || \
               ((Address < ADDR_FLASH_SECTOR_5_BANK2) && (Address >= ADDR_FLASH_SECTOR_4_BANK2))) {
        sector = FLASH_SECTOR_4;
    } else if (((Address < ADDR_FLASH_SECTOR_6_BANK1) && (Address >= ADDR_FLASH_SECTOR_5_BANK1)) || \
               ((Address < ADDR_FLASH_SECTOR_6_BANK2) && (Address >= ADDR_FLASH_SECTOR_5_BANK2))) {
        sector = FLASH_SECTOR_5;
    } else if (((Address < ADDR_FLASH_SECTOR_7_BANK1) && (Address >= ADDR_FLASH_SECTOR_6_BANK1)) || \
               ((Address < ADDR_FLASH_SECTOR_7_BANK2) && (Address >= ADDR_FLASH_SECTOR_6_BANK2))) {
        sector = FLASH_SECTOR_6;
    } else {
        sector = FLASH_SECTOR_7;
    }

    return sector;
}

/**
  * @brief  Gets sector Size
  * @param  None
  * @retval The size of a given sector
  */
static uint32_t GetSectorSize(uint32_t Sector)
{
    return (uint32_t)(128 * 1024); // 128 KB
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
