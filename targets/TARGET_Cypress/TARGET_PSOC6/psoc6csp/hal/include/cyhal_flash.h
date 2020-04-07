/***************************************************************************//**
* \file cyhal_flash.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Flash memory.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

/**
* \addtogroup group_hal_flash Flash
* \ingroup group_hal
* \{
* High level interface for interacting with internal flash memory.
*
* This driver allows data to be read from and written to flash. It also
* provides the ability to obtain information about the address and
* characteristics of the flash block(s) contained on the device.
*/


#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"
#include "cyhal_modules.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Invalid argument */
#define CYHAL_FLASH_RSLT_ERR_ADDRESS (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_FLASH, 0))

/** @brief Information about a single block of flash memory */
typedef struct
{
    uint32_t start_address; //!< Start address of the memory
    uint32_t size;          //!< Size of the flash memory
    uint32_t sector_size;   //!< Sector size of the memory
    uint32_t page_size;     //!< Sector size of the memory
    uint8_t  erase_value;   //!< The flash erase value
} cyhal_flash_block_info_t;

/** @brief Information about all of the blocks of flash memory */
typedef struct
{
    uint8_t block_count; //!< The number of distinct flash blocks
    const cyhal_flash_block_info_t *blocks; //!< Array of the distinct flash blocks
} cyhal_flash_info_t;

/*******************************************************************************
*       Functions
*******************************************************************************/

/** Initialize the flash_t object for access to flash through the HAL
 *
 * @param[out] obj The flash object
 * @return The status of the init request
 */
cy_rslt_t cyhal_flash_init(cyhal_flash_t *obj);

/** Free resources associated with access to flash through the HAL
 *
 * @param[out] obj The flash object.
 */
void cyhal_flash_free(cyhal_flash_t *obj);

/** Gets key flash characteristics including the start address size,
 * and erase values
 *
 * @param[in]  obj  The flash object.
 * @param[out] info The flash configuration info.
 */
void cyhal_flash_get_info(const cyhal_flash_t *obj, cyhal_flash_info_t *info);

/** Read data starting at defined address
 *
 * @param[in]  obj     The flash object.
 * @param[in]  address Address to begin reading from.
 * @param[out] data    The buffer to read data into.
 * @param[in]  size    The number of bytes to read.
 * @return The status of the read request.
 */
cy_rslt_t cyhal_flash_read(cyhal_flash_t *obj, uint32_t address, uint8_t *data, size_t size);

/** Erase one page starting at defined address. The address must be at page boundary. This
 *  will block until the erase operation is complete.
 *
 *  @see cyhal_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total erase size.
 *
 * @param[in] obj The flash object
 * @param[in] address The page starting address
 * @return The status of the erase request
 */
cy_rslt_t cyhal_flash_erase(cyhal_flash_t *obj, uint32_t address);

/** Write one page starting at defined address. The address must be at page boundary. This
 *  will block until the write operation is complete.
 *  This function erases the page prior to writing the new data.
 *
 *  @see cyhal_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total write size. The provided data buffer must be at least as large as the flash
 *  page_size.
 *
 * @param[in] obj The flash object
 * @param[in] address The page starting address
 * @param[in] data The data to write to the flash
 * @return The status of the write request
 */
cy_rslt_t cyhal_flash_write(cyhal_flash_t *obj, uint32_t address, const uint32_t *data);

/** Program one page starting at defined address. The address must be at page boundary. This
 *  will block until the write operation is complete.
 *  Note: This function does not erase the page prior to writing. The page must be erased
 *  first via a separate call to erase.
 *
 *  @see cyhal_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total program size. The provided data buffer must be at least as large as the flash
 *  page_size.
 *
 * @param[in] obj The flash object
 * @param[in] address The sector starting address
 * @param[in] data The data buffer to be programmed
 * @return The status of the program request
 */
cy_rslt_t cyhal_flash_program(cyhal_flash_t *obj, uint32_t address, const uint32_t *data);

/** Starts an asynchronous erase of a single page of flash. Returns immediately and reports
 *  a successful start or reason for failure.
 *
 *  @see cyhal_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total erase size.
 *
 * @param[in] obj The Flash object being operated on
 * @param[in] address The address to start erasing from
 * @return The status of the start_erase request
 */
cy_rslt_t cyhal_flash_start_erase(cyhal_flash_t *obj, uint32_t address);

/** Starts an asynchronous write to a single page of flash. Returns immediately and reports
 *  a successful start or reason for failure.
 *
 *  @see cyhal_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total write size. The provided data buffer must be at least as large as the flash
 *  page_size.
 *
 * @param[in] obj The Flash object being operated on
 * @param[in] address The address to start writing to
 * @param[in] data The data to write to flash
 * @return The status of the start_write request
 */
cy_rslt_t cyhal_flash_start_write(cyhal_flash_t *obj, uint32_t address, const uint32_t* data);

/** Starts asynchronous programming of a single page of flash. Returns immediately and reports
 *  a successful start or reason for failure.
 *
 *  @see cyhal_flash_get_info() to get the flash charactoristics for legal address values and
 *  the total program size. The provided data buffer must be at least as large as the flash
 *  page_size.
 *
 * @param[in] obj The Flash object being operated on
 * @param[in] address The address to start programming
 * @param[in] data The data to write to flash
 * @return The status of the start_program request
 */
cy_rslt_t cyhal_flash_start_program(cyhal_flash_t *obj, uint32_t address, const uint32_t* data);

/** Reports a successful operation result, reason of failure or busy status
 *
 * @param[in]  obj      The Flash object being operated on
 * @return Whether the flash operation is complete
 */
bool cyhal_flash_is_operation_complete(cyhal_flash_t *obj);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_FLASH_IMPL_HEADER
#include CYHAL_FLASH_IMPL_HEADER
#endif /* CYHAL_FLASH_IMPL_HEADER */

/** \} group_hal_flash */
