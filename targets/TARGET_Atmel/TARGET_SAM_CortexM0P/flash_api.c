/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#include "nvm.h"

/** Initialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_init(flash_t *obj)
{
	nvm_get_parameters(&obj->params);

	const struct nvm_config config = {
		NVM_SLEEP_POWER_MODE_WAKEUPINSTANT, // sleep_power_mode
		false, // manual_page_write
		0, // wait_states
		false, // disable_cache
		NVM_CACHE_READMODE_DETERMINISTIC // nvm_cache
	};
	enum status_code config_status = nvm_set_config(&config);
	if (config_status != STATUS_OK) {
        return -1;
    }

    return 0;
}

/** Uninitialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_free(flash_t *obj)
{
    // no-op
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
    enum status_code status = nvm_erase_row(address);

    return status == STATUS_OK ? 0 : -1;
}

/** Read data starting at defined address
 *
 * This function has a WEAK implementation using memcpy for backwards compatibility.
 * @param obj The flash object
 * @param address Address to begin reading from
 * @param data The buffer to read data into
 * @param size The number of bytes to read
 * @return 0 for success, -1 for error
 */
int32_t flash_read(flash_t *obj, uint32_t address, uint8_t *data, uint32_t size)
{
    // apparently you can read / write multiple pages...
    uint32_t page_size = flash_get_page_size(obj);
    if (size % page_size != 0) return -1;

    int32_t bytes_to_read = (int32_t)size;
    uint8_t *data_slice = data;
    while (bytes_to_read > 0) {
        enum status_code status = nvm_read_buffer(address, data_slice, page_size);

        if (status != STATUS_OK) {
            return -1;
        }

        data_slice += page_size;
        bytes_to_read -= page_size;
    }

    if (bytes_to_read != 0) {
        return -1;
    }

    return 0;
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
    // apparently you can read / write multiple pages...
    uint32_t page_size = flash_get_page_size(obj);
    if (size % page_size != 0) return -1;

    int32_t bytes_to_write = (int32_t)size;
    uint8_t *data_slice = data;
    while (bytes_to_write > 0) {
        enum status_code status = nvm_write_buffer(address, data_slice, page_size);

        if (status != STATUS_OK) {
            return -1;
        }

        data_slice += page_size;
        bytes_to_write -= page_size;
    }

    if (bytes_to_write != 0) {
        return -1;
    }

    return 0;
}

/** Get sector size
 *
 * @param obj The flash object
 * @param address The sector starting address
 * @return The size of a sector
 */
uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if (address < flash_get_start_address(obj)) {
        return MBED_FLASH_INVALID_SIZE;
    }

    if (address >= flash_get_start_address(obj) + flash_get_size(obj)) {
        return MBED_FLASH_INVALID_SIZE;
    }

    return obj->params.page_size * NVMCTRL_ROW_PAGES;
}

/** Get page size
 *
 * The page size defines the writable page size
 * @param obj The flash object
 * @return The size of a page
 */
uint32_t flash_get_page_size(const flash_t *obj)
{
    return obj->params.page_size;
}

/** Get start address for the flash region
 *
 * @param obj The flash object
 * @return The start address for the flash region
 */
uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_ADDR;
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
    return 0xff;
}
