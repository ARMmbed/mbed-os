/***************************************************************************//**
* \file cy_serial_flash_qspi.h
*
* \brief
* Provides APIs for interacting with an external flash connected to the SPI or
* QSPI interface. Flash operations read, write, and erase are implemented as
* blocking functions.
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
* \addtogroup group_board_libs Serial Flash
* \{
*/

#pragma once

#include <stddef.h>
#include "cy_result.h"
#include "cy_pdl.h"
#include "cyhal.h"

#ifdef CY_IP_MXSMIF

#if defined(__cplusplus)
extern "C" {
#endif

/** The function or operation is not supported on the target or the memory */
#define CY_RSLT_SERIAL_FLASH_ERR_UNSUPPORTED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_BOARD_LIB_SERIAL_FLASH, 1))

/**
 * \brief Initializes the serial flash memory. This function accepts up to 8
 * I/Os. Number of I/Os depend on the type of memory interface. Pass NC when an
 * I/O is unused.
 * Single SPI - (io0, io1) or (io2, io3) or (io4, io5) or (io6, io7)
 * Dual SPI   - (io0, io1) or (io2, io3) or (io4, io5) or (io6, io7)
 * Quad SPI   - (io0, io1, io2, io3) or (io4, io5, io6, io7)
 * Octal SPI  - All 8 I/Os are used.
 * \param mem_config Memory configuration to be used for initializing
 * \param io0 Data/IO pin 0 connected to the memory, Pass NC when unused.
 * \param io1 Data/IO pin 1 connected to the memory, Pass NC when unused.
 * \param io2 Data/IO pin 2 connected to the memory, Pass NC when unused.
 * \param io3 Data/IO pin 3 connected to the memory, Pass NC when unused.
 * \param io4 Data/IO pin 4 connected to the memory, Pass NC when unused.
 * \param io5 Data/IO pin 5 connected to the memory, Pass NC when unused.
 * \param io6 Data/IO pin 6 connected to the memory, Pass NC when unused.
 * \param io7 Data/IO pin 7 connected to the memory, Pass NC when unused.
 * \param sclk Clock pin connected to the memory
 * \param ssel Slave select pin connected to the memory
 * \param hz Clock frequency to be used with the memory.
 * \returns CY_RSLT_SUCCESS if the initialization was successful, an error code
 *          otherwise.
 */
cy_rslt_t cy_serial_flash_qspi_init(
    const cy_stc_smif_mem_config_t* mem_config,
    cyhal_gpio_t io0,
    cyhal_gpio_t io1,
    cyhal_gpio_t io2,
    cyhal_gpio_t io3,
    cyhal_gpio_t io4,
    cyhal_gpio_t io5,
    cyhal_gpio_t io6,
    cyhal_gpio_t io7,
    cyhal_gpio_t sclk,
    cyhal_gpio_t ssel,
    uint32_t hz);

/**
 * \brief De-initializes the serial flash memory.
 */
void cy_serial_flash_qspi_deinit(void);

/**
 * \brief Returns the size of the serial flash memory in bytes.
 * \returns Memory size in bytes.
 */
size_t cy_serial_flash_qspi_get_size(void);

/**
 * \brief Returns the size of the erase sector to which the given address
 * belongs. Address is used only for a memory with hybrid sector size.
 * \param addr Address that belongs to the sector for which size is returned.
 * \returns Erase sector size in bytes.
 */
size_t cy_serial_flash_qspi_get_erase_size(uint32_t addr);

/**
 * \brief Returns the page size for programming of the sector to which the given
 * address belongs. Address is used only for a memory with hybrid sector size.
 * \param addr Address that belongs to the sector for which size is returned.
 * \returns Page size in bytes.
 */
size_t cy_serial_flash_qspi_get_prog_size(uint32_t addr);

/**
 * \brief Utility function to calculate the starting address of an erase sector
 * to which the given address belongs.
 * \param addr Address in the sector for which the starting address is returned.
 * \returns Starting address of the sector
 */
__STATIC_INLINE uint32_t cy_serial_flash_get_sector_start_address(uint32_t addr)
{
    return (addr & ~(cy_serial_flash_qspi_get_erase_size(addr) - 1));
}

/**
 * \brief Reads data from the serial flash memory. This is a blocking
 * function. Returns error if (addr + length) exceeds the flash size.
 * \param addr Starting address to read from
 * \param length Number of data bytes to read
 * \param buf Pointer to the buffer to store the data read from the memory
 * \returns CY_RSLT_SUCCESS if the read was successful, an error code otherwise.
 */
cy_rslt_t cy_serial_flash_qspi_read(uint32_t addr, size_t length, uint8_t *buf);

/**
 * \brief Writes the data to the serial flash memory. The program area
 * must have been erased prior to calling this API using
 * \ref cy_serial_flash_qspi_erase() This is a blocking function. Returns error if
 * (addr + length) exceeds the flash size.
 * \param addr Starting address to write to
 * \param length Number of bytes to write
 * \param buf Pointer to the buffer storing the data to be written
 * \returns CY_RSLT_SUCCESS if the write was successful, an error code
 *          otherwise.
 */
cy_rslt_t cy_serial_flash_qspi_write(uint32_t addr, size_t length, const uint8_t *buf);

/**
 * \brief Erases the serial flash memory, uses chip erase command when
 * addr = 0 and length = flash_size otherwise uses sector erase command. This is
 * a blocking function. Returns error if addr or (addr + length) is not aligned
 * to the sector size or if (addr + length) exceeds the flash size.
 * Call \ref cy_serial_flash_qspi_get_size() to get the flash size and
 * call \ref cy_serial_flash_qspi_get_erase_size() to get the size of an erase
 * sector.
 *
 * \param addr Starting address to begin erasing
 * \param length Number of bytes to erase
 * \returns CY_RSLT_SUCCESS if the erase was successful, an error code
 *          otherwise.
 */
cy_rslt_t cy_serial_flash_qspi_erase(uint32_t addr, size_t length);

/**
 * \brief Enables Execute-in-Place (memory mapped) mode on the MCU. This
 * function does not send any command to the serial flash. This may not be
 * supported on all the targets in which case
 * CY_RSLT_SERIAL_FLASH_ERR_UNSUPPORTED is returned.
 * \param enable true: XIP mode is set, false: normal mode is set
 * \returns CY_RSLT_SUCCESS if the operation was successful.
 *          CY_RSLT_SERIAL_FLASH_ERR_UNSUPPORTED if the target does not
 *          support XIP.
 */
cy_rslt_t cy_serial_flash_qspi_enable_xip(bool enable);

/**
 * \brief Changes QSPI interrupt priority
 * \param priority interrupt priority to be set
 */
void cy_serial_flash_qspi_set_interrupt_priority(uint8_t priority);

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSMIF */

/** \} group_board_libs */
