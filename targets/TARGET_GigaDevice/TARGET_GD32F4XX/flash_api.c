/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
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

#include "flash_api.h"
#include "mbed_critical.h"

#if DEVICE_FLASH
#include "cmsis.h"

#define FLASH_SIZE                (0x00200000U)
#define FLASH_START_ADDR          (0x08000000U)
#define FLASH_END_ADDR            (0x081FFFFFU)
#define FLASH_BANK0_START_ADDR    (0x08000000U)
#define FLASH_BANK1_START_ADDR    (0x08100000U)
#define SIZE_16KB                 (0x00004000U)
#define SIZE_64KB                 (0x00010000U)
#define SIZE_128KB                (0x00020000U)
#define WRONG_SECTOR_NUM          (0xFFFFFFFFU)
#define WORD_SIZE                 (4U)

typedef struct sector_information {
    uint32_t sector_num;
    uint32_t sector_size;
} sector_t;

/* get the sector number and size of the given address
 *
 * @param address The sector starting address
 * @return The struct of a sector
 */
static sector_t flash_sector_info_get(uint32_t addr)
{
    sector_t sector_info;
    uint32_t temp = 0x00000000;
    if ((FLASH_BANK0_START_ADDR <= addr) && (FLASH_BANK1_START_ADDR > addr)) {
        temp = (addr - FLASH_BANK0_START_ADDR) / SIZE_16KB;
        if (4U > temp) {
            sector_info.sector_num = temp;
            sector_info.sector_size = SIZE_16KB;
        } else if (8U > temp) {
            sector_info.sector_num = 4U;
            sector_info.sector_size = SIZE_64KB;
        } else {
            temp = (addr - FLASH_BANK0_START_ADDR) / SIZE_128KB;
            sector_info.sector_num = temp + 4U;
            sector_info.sector_size = SIZE_128KB;
        }
    } else if ((FLASH_BANK1_START_ADDR <= addr) && (FLASH_END_ADDR >= addr)) {
        temp = (addr - FLASH_BANK1_START_ADDR) / SIZE_16KB;
        if (4U > temp) {
            sector_info.sector_num = temp + 12U;
            sector_info.sector_size = SIZE_16KB;
        } else if (8U > temp) {
            sector_info.sector_num = 16U;
            sector_info.sector_size = SIZE_64KB;
        } else {
            temp = (addr - FLASH_BANK1_START_ADDR) / SIZE_128KB;
            sector_info.sector_num = temp + 16U;
            sector_info.sector_size = SIZE_128KB;
        }
    } else {
        sector_info.sector_num = WRONG_SECTOR_NUM;
        sector_info.sector_size = MBED_FLASH_INVALID_SIZE;
    }
    return sector_info;
}


/* unlock the main FLASH operation
 *
 * @return 0 for success, -1 for error
*/
static int32_t flash_unlock(void)
{
    fmc_unlock();
    if (RESET != (FMC_CTL & FMC_CTL_LK)) {
        return -1;
    }
    return 0;
}

/* lock the main FLASH operation
 *
 * @return 0 for success, -1 for error
*/
static int32_t flash_lock(void)
{
    fmc_lock();
    if (RESET == (FMC_CTL & FMC_CTL_LK)) {
        return -1;
    }
    return 0;
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
    uint32_t number;
    sector_t sector_info;
    int32_t flash_state = 0;
    flash_unlock();

    /* clear FLASH flag */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    /* make sure the address is a right page address */
    sector_info = flash_sector_info_get(address);
    if (11U >= sector_info.sector_num) {
        number = sector_info.sector_num;
    } else if (23U >= sector_info.sector_num) {
        number = sector_info.sector_num + 4U;
    } else {
        number = sector_info.sector_num - 12U;
    }
    if (FMC_READY != fmc_sector_erase(CTL_SN(number))) {
        flash_state = -1;
    }

    flash_lock();
    return flash_state;
}

/** Program pages starting at defined address
 *
 * The pages should not cross multiple sectors.
 * This function does not do any check for address alignments or if size is aligned to a page size.
 * @param obj The flash object
 * @param address The sector starting address
 * @param data The data buffer to be programmed
 * @param size The number of bytes to program
 * @return 0 for success, -1 for error
 */
int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    uint32_t *p_data;
    p_data = (uint32_t *)data;
    uint32_t num = 0;
    int32_t flash_state = 0;
    flash_unlock();

    /* clear FLASH flag */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    if (size % 4) {
        num = size / 4 + 1;
    } else {
        num = size / 4;
    }
    for (uint32_t i = 0; i < num; i++) {

        if (FMC_READY != fmc_word_program(address, *(p_data + i))) {
            flash_state = -1;
            break;
        }
        address += 4;
    }
    flash_lock();
    return flash_state;
}

/** Get sector size
 *
 * @param obj The flash object
 * @param address The sector starting address
 * @return The size of a sector
 */
uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    sector_t sector_info;
    sector_info = flash_sector_info_get(address);
    return (sector_info.sector_size);
}

/** Get page size
 *
 * The page size defines the writable page size
 * @param obj The flash object
 * @return The size of a page
 */
uint32_t flash_get_page_size(const flash_t *obj)
{
    return WORD_SIZE;
}

/** Get start address for the flash region
 *
 * @param obj The flash object
 * @return The start address for the flash region
 */
uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_START_ADDR;
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

/** Get the flash erase value
 *
 * @param obj The flash object
 * @return The flash erase value
 */
uint8_t flash_get_erase_value(const flash_t *obj)
{
    return 0xFF;
}

#endif /* DEVICE_FLASH */
