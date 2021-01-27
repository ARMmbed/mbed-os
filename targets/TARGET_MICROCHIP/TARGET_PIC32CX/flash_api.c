/***************************************************************************//**
* @file flash_api.c
*******************************************************************************
* @section License
* <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
*******************************************************************************
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License"); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************/

#include "device.h"

#if DEVICE_FLASH
#include "pic32cx.h"
#include "flash_api.h"
#include "flash_efc.h"

/** Initialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_init(flash_t *obj)
{
    obj->sfec = SEFC0;
    return 0;
}

/** Uninitialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_free(flash_t *obj)
{
    uint32_t pul_actual_start;
    uint32_t pul_actual_end;

    obj->sfec = NULL;

    flash_efc_lock(IFLASH0_CNC_ADDR, IFLASH0_CNC_ADDR + IFLASH0_SIZE, &pul_actual_start, &pul_actual_end);
    flash_efc_lock(IFLASH1_CNC_ADDR, IFLASH1_CNC_ADDR + IFLASH1_SIZE, &pul_actual_start, &pul_actual_end);

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
    uint32_t ret = FLASH_RC_ERROR;
    uint32_t pul_actual_start;
    uint32_t pul_actual_end;

    (void)obj;

    /* Unlock before erasing */
    if (flash_efc_unlock(address, address + 0x1000, &pul_actual_start, &pul_actual_end) == FLASH_RC_OK) {
        /* Minimum Erase operation by 4 KBytes */
        ret = flash_efc_erase_page(address, IFLASH_ERASE_PAGES_8);
    }

    return (ret == FLASH_RC_OK) ? 0 : -1;
}

/** Program one page starting at defined address
 *
 * The page should be at page boundary, should not cross multiple sectors.
 * This function does not do any check for address alignments or if size is aligned to a page size.
 * @param obj The flash object
 * @param address The sector starting address
 * @param data The data buffer to be programmed
 * @param size The number of bytes to program
 * @return 0 for success, -1 for error
 */
int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    uint32_t ret;

    (void)obj;

    ret = flash_efc_write(address, data, size, 0);

    return (ret == FLASH_RC_OK) ? 0 : -1;
}

/** Get sector size (erase block size)
 *
 * @param obj The flash object
 * @param address The sector starting address
 * @return The size of a sector
 */
uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    (void)obj;
    (void)address;

    return 0x1000; /* Minimum Erase operation by 4 KBytes */
}

/** Get page size
 *
 * The page size defines the writable page size
 * @param obj The flash object
 * @return The size of a page
 */
uint32_t flash_get_page_size(const flash_t *obj)
{
    (void)obj;
    return IFLASH0_PAGE_SIZE;
}

/** Get start address for the flash region
 *
 * @param obj The flash object
 * @return The start address for the flash region
 */
uint32_t flash_get_start_address(const flash_t *obj)
{
    (void)obj;
    return IFLASH0_CNC_ADDR;
}

/** Get the flash region size
 *
 * @param obj The flash object
 * @return The flash region size
 */
uint32_t flash_get_size(const flash_t *obj)
{
    (void)obj;
#ifdef IFLASH1_SIZE
    return IFLASH0_SIZE + IFLASH1_SIZE;
#else
    return IFLASH0_SIZE;
#endif
}

/** Get the flash erase value
 *
 * @param obj The flash object
 * @return The flash erase value
 */
uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif // DEVICE_FLASH
