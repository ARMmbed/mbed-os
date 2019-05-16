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

#define FLASH_SIZE                (0x00020000U)
#define FLASH_PAGE_SIZE           (0x00000400U)
#define FLASH_START_ADDR          (0X08000000U)
#define FLASH_END_ADDR            (0x0801FFFFU)
#define WORD_SIZE                 (4U)

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
    int32_t flash_state = 0;
    flash_unlock();

    /* clear FLASH flag */
    fmc_flag_clear(FMC_FLAG_PGERR);
    fmc_flag_clear(FMC_FLAG_PGAERR);
    fmc_flag_clear(FMC_FLAG_WPERR);
    fmc_flag_clear(FMC_FLAG_END);

    /* make sure the address is a right page address */
    if (FMC_READY != fmc_page_erase(address)) {
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
    fmc_flag_clear(FMC_FLAG_PGERR);
    fmc_flag_clear(FMC_FLAG_PGAERR);
    fmc_flag_clear(FMC_FLAG_WPERR);
    fmc_flag_clear(FMC_FLAG_END);

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
    if ((FLASH_START_ADDR > address) || (FLASH_END_ADDR < address)) {
        return MBED_FLASH_INVALID_SIZE;
    }
    return FLASH_PAGE_SIZE;
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
