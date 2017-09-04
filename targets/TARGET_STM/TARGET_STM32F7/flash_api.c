/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
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
    FLASH_OBProgramInitTypeDef OBInit;
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

    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Unlock();
    /* Get the Dual bank configuration status */
    HAL_FLASHEx_OBGetConfig(&OBInit);
    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Lock();

#if defined (FLASH_OPTCR_nDBANK)
    /* On targets that support dynamic single or dual bank configuration
     * Check that we're in SINGLE Bank mode, only supported mode now.
     */
    if((OBInit.USERConfig & OB_NDBANK_SINGLE_BANK) != OB_NDBANK_SINGLE_BANK) {
        /*  We don't support the DUAL BANK MODE for now, so return error */
        return -1;
    }
#endif

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = SectorId;
    EraseInitStruct.NbSectors = 1;

    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK){
        status = -1;
    }

    flash_lock();

    return status;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data,
        uint32_t size)
{
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

    if (address < ADDR_FLASH_SECTOR_4) {
        // 32k sectorsize
        sector += tmp >>15;
    } else if (address < ADDR_FLASH_SECTOR_5) {
        //64k sector size
        sector += FLASH_SECTOR_4;
    } else {
        sector += 4 + (tmp >>18);
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
    uint32_t sectorsize = 0x00;
    if ((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) ||\
        (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3)) {
            sectorsize = 32 * 1024;
    } else if (Sector == FLASH_SECTOR_4) {
            sectorsize = 128 * 1024;
    } else {
            sectorsize = 256 * 1024;
    }

    return sectorsize;
}

#endif
