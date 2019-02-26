/*
 * Copyright (c) 2018-2019 Arm Limited
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

/*
 * This library provides functions to control the MT25QL256ABA-1EW7-OSIT flash
 * memory from Micron and should work for similar devices from the same vendor.
 */

#ifndef __MT25QL_H__
#define __MT25QL_H__

#include "qspi_ip6514e_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief MT25QL Flash Memory documentation defined values.
 */
#define FLASH_PAGE_SIZE     (256U)          /* 256B */
#define SUBSECTOR_4KB       (0x00001000U)   /* 4KB */
#define SUBSECTOR_32KB      (0x00008000U)   /* 32KB */
#define SECTOR_64KB         (0x00010000U)   /* 64KB */
#define ADDR_BYTES          (3U)

enum mt25ql_error_t {
    MT25QL_ERR_NONE               = QSPI_IP6514E_ERR_NONE,
    MT25QL_ERR_WRONG_ARGUMENT     = QSPI_IP6514E_ERR_WRONG_ARGUMENT,
    MT25QL_ERR_CTRL_NOT_DISABLED  = QSPI_IP6514E_ERR_CONTROLLER_NOT_DISABLED,
    MT25QL_ERR_READ_IN_PROGRESS   = QSPI_IP6514E_ERR_READ_IN_PROGRESS,
    MT25QL_ERR_WRITE_IN_PROGRESS  = QSPI_IP6514E_ERR_WRITE_IN_PROGRESS,
    MT25QL_ERR_ADDR_NOT_ALIGNED,
    MT25QL_ERR_ADDR_TOO_BIG,
};

enum mt25ql_erase_t {
    MT25QL_ERASE_ALL_FLASH     = 0U,             /*!< Erase all flash */
    MT25QL_ERASE_SUBSECTOR_4K  = SUBSECTOR_4KB,  /*!< Erase a 4 KB subsector */
    MT25QL_ERASE_SUBSECTOR_32K = SUBSECTOR_32KB, /*!< Erase a 32 KB subsector */
    MT25QL_ERASE_SECTOR_64K    = SECTOR_64KB,    /*!< Erase a sector (64 KB) */
};

enum mt25ql_functional_state_t {
    MT25QL_FUNC_STATE_DEFAULT   = 0U,
        /*!< The QSPI Flash controller and memory is in default state,
         *   in the same state as after reset.
         */
    MT25QL_FUNC_STATE_FAST      = 1U,
        /*!< The QSPI Flash controller and memory is configured to operate in
         *   single SPI mode and fast Flash commands could be used for read and
         *   program operations.
         */
    MT25QL_FUNC_STATE_QUAD_FAST = 2U,
        /*!< The QSPI Flash controller and memory is configured to operate in
         *   Quad SPI mode and fast Flash commands could be used for read and
         *   program operations.
         */
};

struct mt25ql_dev_t {
    struct qspi_ip6514e_dev_t *controller;
        /*!< QSPI Flash controller. */
    uint32_t direct_access_start_addr;
        /*!< AHB address to directly access the contents of the Flash memory
         *   through the QSPI Controller.
         */
    uint32_t baud_rate_div;
        /*!< Clock divisor that will be used to configure the QSPI Flash
         *   Controller to access the Flash memory. The clock which frequency is
         *   divived is the one linked to the QSPI Flash controller. It can only
         *   be an even number between 2 and 32 (both included). It needs to be
         *   high enough to support the Quad Output Fast Read command with 8
         *   dummy cycles and the Quad Input Fast Program with 0 dummy cycles.
         */
    uint32_t size; /*!< Total size of the MT25QL Flash memory */
    enum mt25ql_functional_state_t func_state;
        /*!< Functional state (operational parameter settings) of the
         *   QSPI Flash controller and memory.
         */
};

/**
 * \brief Change configuration of the QSPI Flash controller and MT25QL memory
 *
 *        Changes the configuration of the QSPI Flash controller and MT25QL
 *        Flash memory to operate in the specified SPI mode and to use the
 *        appropriate Flash commands for read and program operations.
 *        It also sets:
 *          + The number of dummy cycles for each operation
 *          + The bytes per page constant to 256 (MT25QL Flash specific)
 *          + The number of address bytes to 3
 *
 * \param[in] dev       Pointer to MT25QL device structure \ref mt25ql_dev_t
 * \param[in] config    Operational configuration to be set on flash controller
 *                      and device \ref mt25ql_functional_state_t
 *
 * \return Return error code as specified in \ref mt25ql_error_t
 *
 * \note This function assumes that the Flash memory device and the QSPI Flash
 *       controller operates with the same SPI protocol. This function will fail
 *       if the Flash device is in a different configuration.
 */
enum mt25ql_error_t mt25ql_config_mode(struct mt25ql_dev_t* dev,
                                       enum mt25ql_functional_state_t config);

/**
 * \brief Restore the QSPI Flash controller and MT25QL to default state.
 *
 * \param[in] dev     Pointer to MT25QL device structure \ref mt25ql_dev_t
 *
 * \return Return error code as specified in \ref mt25ql_error_t
 *
 * \note This function assumes that the Flash memory device and the QSPI Flash
 *       controller operates with the same SPI protocol. This function will fail
 *       if the Flash device is in a different configuration.
 */
enum mt25ql_error_t mt25ql_restore_default_state(struct mt25ql_dev_t* dev);

/**
 * \brief Read bytes from the flash memory (direct access)
 *
 * \param[in]  dev   Pointer to MT25QL device structure \ref mt25ql_dev_t
 * \param[in]  addr  Flash memory address for the read operation
 * \param[out] data  Pointer where len bytes read from the flash memory will be
 *                   written to
 * \param[in]  len   Number of bytes to read
 *
 * \return Return error code as specified in \ref mt25ql_error_t
 *
 * \note This function will use direct access to read from the Flash memory. It
 *       can be used to access above the direct accessible memory zone if
 *       not all the AHB address wires are connected.
 * \note The address given should be the address of the data inside the flash
 *       memory. To read the first byte inside the memory, use 0x00000000.
 */
enum mt25ql_error_t mt25ql_direct_read(struct mt25ql_dev_t* dev,
                                       uint32_t addr,
                                       void *data,
                                       uint32_t len);

/**
 * \brief Write bytes in the flash memory, at a location where data has already
 *        been erased (direct access)
 *
 * \param[in] dev   Pointer to MT25QL device structure \ref mt25ql_dev_t
 * \param[in] addr  Flash memory address for the write operation
 * \param[in] data  Pointer to the len bytes that will be written to the flash
 *                  memory
 * \param[in] len   Number of bytes to write
 *
 * \return Return error code as specified in \ref mt25ql_error_t
 *
 * \note This function will use direct access to write to the Flash memory. It
 *       can be used to access outside of the direct accessible memory zone if
 *       not all the AHB address wires are connected.
 * \note The address given should be the address of the data inside the flash
 *       memory. To write the first byte inside the memory, use 0x00000000.
 * \note Writing bytes in the flash memory clear them from 1 to 0, for that
 *       matter the location where data is written needs to be erased
 *       beforehand.
 */
enum mt25ql_error_t mt25ql_direct_write(struct mt25ql_dev_t* dev,
                                        uint32_t addr,
                                        const void *data,
                                        uint32_t len);

/**
 * \brief Read bytes from the flash memory (using Flash commands)
 *
 * \param[in]  dev   Pointer to MT25QL device structure \ref mt25ql_dev_t
 * \param[in]  addr  Flash memory address for the read operation
 * \param[out] data  Pointer where len bytes read from the flash memory will be
 *                   written to
 * \param[in]  len   Number of bytes to read
 *
 * \return Return error code as specified in \ref mt25ql_error_t
 *
 * \note This function will use the Software Triggered Instruction Generator to
 *       read from the Flash memory using Flash commands.
 * \note The address given should be the address of the data inside the flash
 *       memory. To read the first byte inside the memory, use 0x00000000.
 */
enum mt25ql_error_t mt25ql_command_read(struct mt25ql_dev_t* dev,
                                        uint32_t addr,
                                        void *data,
                                        uint32_t len);

/**
 * \brief Write bytes in the flash memory, at a location where data has already
 *        been erased (using Flash commands)
 *
 * \param[in] dev   Pointer to MT25QL device structure \ref mt25ql_dev_t
 * \param[in] addr  Flash memory address for the write operation
 * \param[in] data  Pointer to the len bytes that will be written to the flash
 *                  memory
 * \param[in] len   Number of bytes to write
 *
 * \return Return error code as specified in \ref mt25ql_error_t
 *
 * \note This function will use the Software Triggered Instruction Generator to
 *       write to the Flash memory using Flash commands.
 * \note The address given should be the address of the data inside the flash
 *       memory. To write the first byte inside the memory, use 0x00000000.
 * \note Writing bytes in the flash memory clear them from 1 to 0, for that
 *       matter the location where data is written needs to be erased
 *       beforehand.
 */
enum mt25ql_error_t mt25ql_command_write(struct mt25ql_dev_t* dev,
                                         uint32_t addr,
                                         const void *data,
                                         uint32_t len);

/**
 * \brief Erase all flash memory, a sector (64 KiB) or a subsector
 *        (32 KiB or 4 KiB)
 *
 * \param[in] dev        Pointer to MT25QL device structure \ref mt25ql_dev_t
 * \param[in] addr       Address where to erase in the flash memory
 * \param[in] erase_type Type of what to erase at the specified address:
 *                         * whole flash memory
 *                         * a subsector (4 KiB or 32 KiB)
 *                         * a sector (64 KiB)
 * \return Return error code as specified in \ref mt25ql_error_t
 *
 * \note The address need to be aligned with the size of what is erased or 0 if
 *       all flash memory is to be erased.
 */
enum mt25ql_error_t mt25ql_erase(struct mt25ql_dev_t* dev,
                                 uint32_t addr,
                                 enum mt25ql_erase_t erase_type);

#ifdef __cplusplus
}
#endif

#endif /* __MT25QL_H__ */
