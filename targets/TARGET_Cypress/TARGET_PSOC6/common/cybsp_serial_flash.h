/***************************************************************************//**
* \file cybsp_serial_flash.h
*
* \brief
* Provides APIs for interacting with an external flash connected to the SPI or
* QSPI interface. Flash operations read, write, and erase are implemented as
* blocking functions.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
* \addtogroup group_bsp_serial_flash Serial Flash
* \{
* Driver for interfacing with the serial flash (QSPI NOR flash) on Cypress boards. 
*
* \defgroup group_bsp_serial_flash_macros Macros
* \defgroup group_bsp_serial_flash_functions Functions
*/

#pragma once

#include <stddef.h>
#include "cy_result.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_bsp_serial_flash_macros
* \{
*/

/** The function or operation is not supported on the target or the memory */
#define CYBSP_RSLT_SERIAL_FLASH_ERR_UNSUPPORTED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_BSP, 6))

/** \} group_bsp_serial_flash_macros */

/**
* \addtogroup group_bsp_serial_flash_functions
* \{
*/

/**
 * \brief Initializes the serial flash on the board.
 * \returns CY_RSLT_SUCCESS if the initialization was successful, an error code
 *          otherwise.
 */
cy_rslt_t cybsp_serial_flash_init(void);

/**
 * \brief De-initializes the serial flash on the board.
 */
void cybsp_serial_flash_deinit(void);

/**
 * \brief Returns the size of the serial flash on the board in bytes.
 * \returns Memory size in bytes.
 */
size_t cybsp_serial_flash_get_size(void);

/**
 * \brief Returns the size of the erase sector to which the given address
 * belongs. Address is used only for a memory with hybrid sector size.
 * \param addr Address that belongs to the sector for which size is returned.
 * \returns Erase sector size in bytes.
 */
size_t cybsp_serial_flash_get_erase_size(uint32_t addr);

/**
 * \brief Reads data from the serial flash on the board. This is a blocking
 * function. Returns error if (addr + length) exceeds the flash size.
 * \param addr Starting address to read from
 * \param length Number of data bytes to read
 * \param buf Pointer to the buffer to store the data read from the memory
 * \returns CY_RSLT_SUCCESS if the read was successful, an error code otherwise.
 */
cy_rslt_t cybsp_serial_flash_read(uint32_t addr, size_t length, uint8_t *buf);

/**
 * \brief Writes the data to the serial flash on the board. The program area
 * must have been erased prior to calling this API using
 * \ref cybsp_serial_flash_erase() This is a blocking function. Returns error if
 * (addr + length) exceeds the flash size.
 * \param addr Starting address to write to
 * \param length Number of bytes to write
 * \param buf Pointer to the buffer storing the data to be written
 * \returns CY_RSLT_SUCCESS if the write was successful, an error code
 * 			otherwise.
 */
cy_rslt_t cybsp_serial_flash_write(uint32_t addr, size_t length, const uint8_t *buf);

/**
 * \brief Erases the serial flash on the board, uses chip erase command when
 * addr = 0 and length = flash_size otherwise uses sector erase command. This is
 * a blocking function. Returns error if addr or (addr + length) is not aligned
 * to the sector size or if (addr + length) exceeds the flash size.
 * Call \ref cybsp_serial_flash_get_size() to get the flash size and
 * call \ref cybsp_serial_flash_get_erase_size() to get the size of an erase
 * sector.
 *
 * \param addr Starting address to begin erasing
 * \param length Number of bytes to erase
 * \returns CY_RSLT_SUCCESS if the erase was successful, an error code
 *          otherwise.
 */
cy_rslt_t cybsp_serial_flash_erase(uint32_t addr, size_t length);

/**
 * \brief Enables Execute-in-Place (memory mapped) mode on the MCU. This
 * function does not send any command to the serial flash. This may not be
 * supported on all the targets in which case
 * CYBSP_RSLT_SERIAL_FLASH_ERR_UNSUPPORTED is returned.
 * \param enable true: XIP mode is set, false: normal mode is set
 * \returns CY_RSLT_SUCCESS if the operation was successful.
 *          CYBSP_RSLT_SERIAL_FLASH_ERR_UNSUPPORTED if the target does not
 *          support XIP.
 */
cy_rslt_t cybsp_serial_flash_enable_xip(bool enable);

/** \} group_bsp_serial_flash_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_bsp_serial_flash */
