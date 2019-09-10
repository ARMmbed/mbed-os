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
#include "mbed_error.h"

static uint32_t GetSector(uint32_t Address);
static uint32_t GetSectorSize(uint32_t Sector);
static uint32_t GetSectorBase(uint32_t SectorId);

int32_t flash_init(flash_t *obj)
{
    // Check Dual Bank option byte (nDBANK) on devices supporting both single and dual bank configurations
#ifdef FLASH_OPTCR_nDBANK
    FLASH_OBProgramInitTypeDef OBInit;
    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Unlock();
    /* Get the Dual bank configuration status */
    HAL_FLASHEx_OBGetConfig(&OBInit);
    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Lock();
#if MBED_CONF_TARGET_FLASH_DUAL_BANK
    if ((OBInit.USERConfig & OB_NDBANK_SINGLE_BANK) == OB_NDBANK_SINGLE_BANK) {
        error("The Dual Bank mode option byte (nDBANK) must be enabled (box unchecked)\n");
        return -1;
    }
#else // SINGLE BANK
    if ((OBInit.USERConfig & OB_NDBANK_SINGLE_BANK) == OB_NDBANK_DUAL_BANK) {
        error("The Dual Bank mode option byte (nDBANK) must be disabled (box checked)\n");
        return -1;
    }
#endif
#else // Devices supporting Single Bank only
#if MBED_CONF_TARGET_FLASH_DUAL_BANK
#error "The Dual Bank configuration is not supported on this device."
#endif
#endif
    return 0;
}

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

    if (flash_unlock() != HAL_OK) {
        return -1;
    }

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
       you have to make sure that these data are rewritten before they are accessed during code
       execution. If this cannot be done safely, it is recommended to flush the caches by setting the
       DCRST and ICRST bits in the FLASH_CR register. */
    __HAL_FLASH_ART_DISABLE();
    __HAL_FLASH_ART_RESET();
    __HAL_FLASH_ART_ENABLE();

    /* Get the 1st sector to erase */
    SectorId = GetSector(address);

    /* Fill EraseInit structure */
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = SectorId;
    EraseInitStruct.NbSectors = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        status = -1;
    }

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)GetSectorBase(SectorId), GetSectorSize(SectorId));
    SCB_InvalidateICache();

    flash_lock();

    return status;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data,
                           uint32_t size)
{
    int32_t status = 0;
    uint32_t StartAddress = address;
    uint32_t FullSize = size;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

    if (flash_unlock() != HAL_OK) {
        return -1;
    }

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
       you have to make sure that these data are rewritten before they are accessed during code
       execution. If this cannot be done safely, it is recommended to flush the caches by setting the
       DCRST and ICRST bits in the FLASH_CR register. */
    __HAL_FLASH_ART_DISABLE();
    __HAL_FLASH_ART_RESET();
    __HAL_FLASH_ART_ENABLE();

    while ((size > 0) && (status == 0)) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,
                              address, (uint64_t)*data) != HAL_OK) {
            status = -1;
        } else {
            size--;
            address++;
            data++;
        }
    }

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)StartAddress, FullSize);
    SCB_InvalidateICache();

    flash_lock();

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
    // Flash of STM32F7 devices can be programed 1 byte at a time
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
#if (MBED_CONF_TARGET_FLASH_DUAL_BANK) && defined(FLASH_OPTCR_nDBANK)
    if (address < ADDR_FLASH_SECTOR_4) { // Sectors 0 to 3
        sector += tmp >> 14;
    } else if (address < ADDR_FLASH_SECTOR_5) { // Sector 4
        sector += FLASH_SECTOR_4;
    } else if (address < ADDR_FLASH_SECTOR_12) { // Sectors 5 to 11
        sector += 4 + (tmp >> 17);
    } else if (address < ADDR_FLASH_SECTOR_16) { // Sectors 12 to 15
        tmp = address - ADDR_FLASH_SECTOR_12;
        sector += 12 + (tmp >> 14);
    } else if (address < ADDR_FLASH_SECTOR_17) { // Sector 16
        sector += FLASH_SECTOR_16;
    } else { // Sectors 17 to 23
        tmp = address - ADDR_FLASH_SECTOR_12;
        sector += 16 + (tmp >> 17);
    }
#else // SINGLE BANK
    if (address < ADDR_FLASH_SECTOR_4) { // Sectors 0 to 3
        sector += tmp >> 15;
    } else if (address < ADDR_FLASH_SECTOR_5) { // Sector 4
        sector += FLASH_SECTOR_4;
    } else { // Sectors 5 to 11
        sector += 4 + (tmp >> 18);
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
#if (MBED_CONF_TARGET_FLASH_DUAL_BANK) && defined(FLASH_OPTCR_nDBANK)
    if ((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || \
            (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3) || \
            (Sector == FLASH_SECTOR_12) || (Sector == FLASH_SECTOR_13) || \
            (Sector == FLASH_SECTOR_14) || (Sector == FLASH_SECTOR_15)) {
        sectorsize = 16 * 1024;
    } else if ((Sector == FLASH_SECTOR_4) || (Sector == FLASH_SECTOR_16)) {
        sectorsize = 64 * 1024;
    } else {
        sectorsize = 128 * 1024;
    }
#else // SINGLE BANK
    if ((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || \
            (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3)) {
        sectorsize = 32 * 1024;
    } else if (Sector == FLASH_SECTOR_4) {
        sectorsize = 128 * 1024;
    } else {
        sectorsize = 256 * 1024;
    }
#endif
    return sectorsize;
}

/**
  * @brief  Gets sector base address
  * @param  SectorId
  * @retval base address of a given sector
  */
static uint32_t GetSectorBase(uint32_t SectorId)
{
    uint32_t i = 0;
    uint32_t address_sector = FLASH_BASE;

    for (i = 0; i < SectorId; i++) {
        address_sector += GetSectorSize(i);
    }
    return address_sector;
}


uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
