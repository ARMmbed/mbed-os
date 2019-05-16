/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "fsl_flashiap.h"

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
    uint32_t n;
    uint32_t status;
    int32_t ret = -1;

    /* We need to prevent flash accesses during erase operation */
    core_util_critical_section_enter();

    n = address / FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES;   // Get Sector Number

    status = FLASHIAP_PrepareSectorForWrite(n, n);
    if (status == kStatus_FLASHIAP_Success) {
        status = FLASHIAP_EraseSector(n, n, SystemCoreClock);
        if (status == kStatus_FLASHIAP_Success) {
            ret = 0;
        }
    }

    core_util_critical_section_exit();

    return ret;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    uint32_t n;
    uint32_t sector_number;
    uint32_t num_of_bytes = size;
    uint32_t status;
    int32_t ret = -1;

    if (address == 0) {                              // Check for Vector Table
        n = *((unsigned long *)(data + 0)) +
            *((unsigned long *)(data + 1)) +
            *((unsigned long *)(data + 2)) +
            *((unsigned long *)(data + 3)) +
            *((unsigned long *)(data + 4)) +
            *((unsigned long *)(data + 5)) +
            *((unsigned long *)(data + 6));
        *((unsigned long *)(data + 7)) = 0 - n;  // Signature at Reserved Vector
    }

    /* We need to prevent flash accesses during program operation */
    core_util_critical_section_enter();

    sector_number = address / FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES;  // Get Sector Number

    status = FLASHIAP_PrepareSectorForWrite(sector_number, sector_number);
    if (status == kStatus_FLASHIAP_Success) {
        /* Check if the number of bytes to write is aligned to page size */
        if (size % FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES) {
            uint8_t page_buffer[FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES] = { 0 };
            uint32_t remaining_bytes = 0;

            /* Find the number of pages and remaining bytes */
            num_of_bytes = FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES * (size / FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES);
            remaining_bytes = (size - num_of_bytes);

            /* Copy the remaining bytes into a temp buffer whose size is page-aligned */
            memcpy(page_buffer, data + num_of_bytes, remaining_bytes);

            if (num_of_bytes) {
                /* First write page size aligned bytes of data */
                status = FLASHIAP_CopyRamToFlash(address, (uint32_t *)data, num_of_bytes, SystemCoreClock);
                if (status == kStatus_FLASHIAP_Success) {
                    /* Prepare the next write for the remaining data */
                    status = FLASHIAP_PrepareSectorForWrite(sector_number, sector_number);
                }
            }

            /* Write the remaining data */
            if (status == kStatus_FLASHIAP_Success) {
                status = FLASHIAP_CopyRamToFlash((address + num_of_bytes), (uint32_t *)page_buffer, FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES, SystemCoreClock);
                if (status == kStatus_FLASHIAP_Success) {
                    ret = 0;
                }
            }
        } else {
            status = FLASHIAP_CopyRamToFlash(address, (uint32_t *)data, num_of_bytes, SystemCoreClock);
            if (status == kStatus_FLASHIAP_Success) {
                ret = 0;
            }
        }
    }

    core_util_critical_section_exit();

    return ret;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    uint32_t sectorsize = MBED_FLASH_INVALID_SIZE;
    uint32_t devicesize = FSL_FEATURE_SYSCON_FLASH_SIZE_BYTES;
    uint32_t startaddr = 0;

    if ((address >= startaddr) && (address < (startaddr + devicesize))) {
        sectorsize = FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES;
    }

    return sectorsize;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    uint32_t startaddr = 0;

    return startaddr;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return FSL_FEATURE_SYSCON_FLASH_SIZE_BYTES;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
