/** \addtogroup hal */
/** @{*/

/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef MBED_FLASH_API_H
#define MBED_FLASH_API_H

#include "device.h"
#include <stdint.h>

#if DEVICE_FLASH

#define MBED_FLASH_INVALID_SIZE     0xFFFFFFFF

typedef struct flash_s flash_t;

#if TARGET_FLASH_CMSIS_ALGO
#include "flash_data.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup flash_hal Flash HAL API
 * @{
 */

/** Initialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_init(flash_t *obj);

/** Uninitialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_free(flash_t *obj);

/** Erase one sector starting at defined address
 *
 * The address should be at sector boundary. This function does not do any check for address alignments
 * @param obj The flash object
 * @param address The sector starting address
 * @return 0 for success, -1 for error
 */
int32_t flash_erase_sector(flash_t *obj, uint32_t address);

/** Read data starting at defined address
 *
 * This function has a WEAK implementation using memcpy for backwards compatibility.
 * @param obj The flash object
 * @param address Address to begin reading from
 * @param data The buffer to read data into
 * @param size The number of bytes to read
 * @return 0 for success, -1 for error
 */
int32_t flash_read(flash_t *obj, uint32_t address, uint8_t *data, uint32_t size);

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
int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size);

/** Get sector size
 *
 * @param obj The flash object
 * @param address The sector starting address
 * @return The size of a sector
 */
uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address);

/** Get page size
 *
 * The page size defines the writable page size
 * @param obj The flash object
 * @return The size of a page
 */
uint32_t flash_get_page_size(const flash_t *obj);

/** Get start address for the flash region
 *
 * @param obj The flash object
 * @return The start address for the flash region
 */
uint32_t flash_get_start_address(const flash_t *obj);

/** Get the flash region size
 *
 * @param obj The flash object
 * @return The flash region size
 */
uint32_t flash_get_size(const flash_t *obj);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
