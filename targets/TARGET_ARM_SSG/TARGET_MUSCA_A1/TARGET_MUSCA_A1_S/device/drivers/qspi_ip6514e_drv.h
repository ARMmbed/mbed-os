/*
 * Copyright (c) 2018 Arm Limited
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

/**
 * \file qspi_ip6514e_drv.h
 * \brief Driver for Cadence QSPI Flash Controller IP.
 *        There are two ways to communicate with the flash memory device:
 *         - issue AHB requests for direct read and writes in the Flash memory
 *           mapped address zone. The commands used for those can be configured
 *           by the driver
 *         - send a command to the device to access his internal registers and
 *           do other operations like erasing a sector
 *        At reset, the QSPI controller will work in a default mode which will
 *        allow to do basic commands. It should be configured with the
 *        flash memory device specifications for optimal use for commands and
 *        direct reads/writes. Here is an example of configuration:
 *         - send command to activate QSPI mode on the flash memory device
 *         - send command to change dummy cycles on the flash memory device
 *         - check if any operation is ungoing
 *         - disable the QSPI controller
 *         - change the baud rate divisor
 *         - activate the QSPI mode on the controller
 *         - change the dummy cycles number and opcode for reads/writes
 *         - change the number of bytes per page
 *         - change the number of address bytes
 *         - activate the QSPI controller
 *
 *        Warning: none of the functions declared here check if the dev
 *        argument points to NULL.
 */

#ifndef __QSPI_IP6514E_DRV_H__
#define __QSPI_IP6514E_DRV_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Cadence QSPI IP6514E error enumeration types
 */
enum qspi_ip6514e_error_t {
    QSPI_IP6514E_ERR_NONE,
    QSPI_IP6514E_ERR_WRONG_ARGUMENT,
    QSPI_IP6514E_ERR_CONTROLLER_NOT_DISABLED,
    QSPI_IP6514E_ERR_READ_IN_PROGRESS,
    QSPI_IP6514E_ERR_WRITE_IN_PROGRESS,
    /* Any new error should be added to the enumeration type error of
     * the corresponding Flash device library as well.
     */
};

/**
 * \brief Cadence QSPI IP6514E SPI modes
 */
enum qspi_ip6514e_spi_mode_t {
    QSPI_IP6514E_SPI_MODE,
        /*!< Use 1 line for Instruction, Address and Data */
    QSPI_IP6514E_DSPI_MODE,
        /*!< Use 2 lines for Instruction, Address and Data */
    QSPI_IP6514E_QSPI_MODE,
        /*!< Use 4 lines for Instruction, Address and Data */
};

/**
 * \brief Cadence QSPI IP6514E device configuration structure
 */
struct qspi_ip6514e_dev_cfg_t {
    const uint32_t base; /*!< QSPI IP6514E base address */
    /*
     * If not all the AHB wires are connected to the QSPI Flash Controller the
     * driver can still access all of the Flash memory. The bits of this value
     * should be put to 1 for every wire that is connected. Set it to
     * 0xFFFFFFFFU if all AHB address wires are connected to the
     * QSPI Flash Controller.
     */
    uint32_t addr_mask;
};

/**
 * \brief Cadence QSPI IP6514E device structure
 */
struct qspi_ip6514e_dev_t {
    const struct qspi_ip6514e_dev_cfg_t* const cfg;
                                              /*!< QSPI IP6514E configuration */
};

/**
 * \brief Check if the controller is idle.
 *
 * \param[in] dev QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 *
 * \return true if the controller is idle, false otherwise.
 */
bool qspi_ip6514e_is_idle(struct qspi_ip6514e_dev_t* dev);

/**
 * \brief Check if the controller is enabled.
 *
 * \param[in] dev QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 *
 * \return true if the controller is enabled, false otherwise.
 */
bool qspi_ip6514e_is_enabled(struct qspi_ip6514e_dev_t* dev);

/**
 * \brief Disable the QSPI controller.
 *
 * \param[in] dev QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 */
void qspi_ip6514e_disable(struct qspi_ip6514e_dev_t* dev);

/**
 * \brief Enable the QSPI controller.
 *
 * \param[in] dev QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 */
void qspi_ip6514e_enable(struct qspi_ip6514e_dev_t* dev);

/**
 * \brief Change the baud rate divisor.
 *
 * \param[in] dev QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 * \param[in] div Baud rate divisor value. It can only be an even number
 *                    between 2 and 32 (both included).
 *
 * \return Returns error code as specified in \ref qspi_ip6514e_error_t
 *
 * \note The QSPI frequency is calculated dividing the QSPI controller clock by
 *       this divisor. Please check Flash memory device specifications to know
 *       the maximal frequency that can be used.
 * \note The QSPI controller should be disabled before calling this function.
 */
enum qspi_ip6514e_error_t qspi_ip6514e_set_baud_rate_div(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint32_t div);

/**
 * \brief Set SPI mode for instruction, address and data.
 *
 * \param[in] dev       QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 * \param[in] inst_type SPI mode to use for the instruction part of the command
 * \param[in] addr_type SPI mode to use for the address part of the command
 * \param[in] data_type SPI mode to use for the data part of the command
 *
 * \return Returns error code as specified in \ref qspi_ip6514e_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 * \note Changing this setting will affect commands and direct operations.
 */
enum qspi_ip6514e_error_t qspi_ip6514e_set_spi_mode(
                                        struct qspi_ip6514e_dev_t* dev,
                                        enum qspi_ip6514e_spi_mode_t inst_type,
                                        enum qspi_ip6514e_spi_mode_t addr_type,
                                        enum qspi_ip6514e_spi_mode_t data_type);

/**
 * \brief Configure read commands for direct reads.
 *
 * \param[in] dev          QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 * \param[in] opcode       Read opcode that will be used for every direct read
 * \param[in] dummy_cycles Number of dummy cycles to wait before triggering the
 *                         command, this value must be between 0 and 31
 *                         (both included)
 *
 * \return Returns error code as specified in \ref qspi_ip6514e_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 */
enum qspi_ip6514e_error_t qspi_ip6514e_cfg_reads(struct qspi_ip6514e_dev_t* dev,
                                                 uint8_t opcode,
                                                 uint32_t dummy_cycles);

/**
 * \brief Configure write commands for direct writes.
 *
 * \param[in] dev          QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 * \param[in] opcode       Write opcode that will be used for every direct write
 * \param[in] dummy_cycles Number of dummy cycles to wait before triggering the
 *                         command, this value must be between 0 and 31
 *                         (both included)
 *
 * \return Returns error code as specified in \ref qspi_ip6514e_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 */
enum qspi_ip6514e_error_t qspi_ip6514e_cfg_writes(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint8_t opcode,
                                                 uint32_t dummy_cycles);

/**
 * \brief Change the number of bytes per device page.
 *
 * \param[in] dev          QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 * \param[in] page_size    Number of bytes per device page, must be between 0
 *                         and 4095 (both included)
 *
 * \return Returns error code as specified in \ref qspi_ip6514e_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 * \note This function will affect direct reads/writes.
 */
enum qspi_ip6514e_error_t qspi_ip6514e_cfg_page_size(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint32_t page_size);

/**
 * \brief Change the number of device address bytes.
 *
 * \param[in] dev          QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 * \param[in] bytes_number Number of device address bytes, must be between 1
 *                         and 16 (both included)
 *
 * \return Returns error code as specified in \ref qspi_ip6514e_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 * \note This function will affect direct reads/writes.
 */
enum qspi_ip6514e_error_t qspi_ip6514e_cfg_addr_bytes(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint32_t bytes_number);

/**
 * \brief Remap the incoming AHB address with an offset for direct accesses.
 *
 * \param[in] dev    QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 * \param[in] offset Offset that will be added to the incoming AHB address to
 *                   access the Flash memory
 *
 * \note This function will only affect direct reads/writes.
 * \note This function does not check if the resulting address is out of memory
 *       bounds.
 */
void qspi_ip6514e_remap_addr(struct qspi_ip6514e_dev_t* dev, uint32_t offset);

/**
 * \brief Disable AHB address remapping for direct accesses.
 *
 * \param[in] dev    QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 *
 * \note This function will disable the controller if it is not already
 *       disabled and enable it again (if it was).
 * \note This function will only affect direct reads/writes.
 */
void qspi_ip6514e_disable_remap(struct qspi_ip6514e_dev_t* dev);

/**
 * \brief Restore the default value of the QSPI controller registers.
 *
 * \param[in] dev    QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 *
 * \note The QSPI controller should be disabled before calling this function.
 */
void qspi_ip6514e_reset_regs(struct qspi_ip6514e_dev_t* dev);

/**
 * \brief Send a command to the flash memory device using the Software Triggered
 *        Instruction Generator (STIG).
 *
 * \param[in]  dev               QSPI IP6514E device struct
 *                               \ref qspi_ip6514e_dev_t
 * \param[in]  opcode            Opcode for the command.
 * \param[out] read_data         Pointer to a memory zone where the read_len
 *                               bytes read will be written to. If no data is to
 *                               be read for the command,
 *                               this argument should be NULL.
 * \param[in]  read_len          Number of bytes to read for the command. If
 *                               no bytes are to be read, use 0 for argument
 *                               otherwise between 1 and 8 bytes (both
 *                               included) can be read.
 * \param[in]  write_data        Pointer to a memory zone where are
 *                               located the write_len bytes to write for
 *                               this command. If no bytes are to be written,
 *                               use NULL as argument.
 * \param[in]  write_len         Number of bytes to write for the command. If
 *                               no bytes are to be written, use 0 for
 *                               argument otherwise between 1 and 8 bytes
 *                               (both included) can be written.
 * \param[in]  addr              Address used for the command
 * \param[in]  addr_bytes_number Number of address bytes for this command.
 *                               If an address is not needed for the command,
 *                               use 0 for argument, otherwise between 1 and
 *                               4 bytes (both included) can be used.
 * \param[in]  dummy_cycles      Number of dummy cycles required for the
 *                               command, between 0 and 31 (both included).
 *
 * \return Returns error code as specified in \ref qspi_ip6514e_error_t
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_ip6514e_activate_qspi_mode function or the default one.
 */
enum qspi_ip6514e_error_t qspi_ip6514e_send_cmd(struct qspi_ip6514e_dev_t* dev,
                                                uint8_t opcode,
                                                void *read_data,
                                                uint32_t read_len,
                                                const void *write_data,
                                                uint32_t write_len,
                                                uint32_t addr,
                                                uint32_t addr_bytes_number,
                                                uint32_t dummy_cycles);

/**
 * \brief Send a simple command to the flash memory device using the Software
 *        Triggered Instruction Generator (STIG) with no data arguments.
 *        This command can be used for example to send the WRITE ENABLE command.
 *
 * \param[in]  dev    QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 * \param[in]  opcode Opcode for the command.
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_ip6514e_activate_qspi_mode function or the default one.
 */
void qspi_ip6514e_send_simple_cmd(struct qspi_ip6514e_dev_t* dev,
                                  uint8_t opcode);

/**
 * \brief Send a read command to the flash memory device using the Software
 *        Triggered Instruction Generator (STIG). This command can be used to
 *        read Flash memory data or registers.
 *
 * \param[in]  dev               QSPI IP6514E device struct
 *                               \ref qspi_ip6514e_dev_t
 * \param[in]  opcode            Opcode for the command.
 * \param[out] read_data         Pointer to a memory zone where the
 *                               read_len bytes read will be written to.
 * \param[in]  read_len          Number of bytes to read for the command.
 *                               Between 1 and 8 bytes (both included) can be
 *                               read.
 * \param[in]  addr              Address used for the command
 * \param[in]  addr_bytes_number Number of address bytes for this command.
 *                               If an address is not needed for the command,
 *                               use 0 for argument, otherwise between 1 and
 *                               4 bytes (both included) can be used.
 * \param[in]  dummy_cycles      Number of dummy cycles required for the
 *                               command, between 0 and 31 (both included).
 *
 * \return Returns error code as specified in \ref qspi_ip6514e_error_t
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_ip6514e_activate_qspi_mode function or the default one.
 */
enum qspi_ip6514e_error_t qspi_ip6514e_send_read_cmd(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint8_t opcode,
                                                 void *read_data,
                                                 uint32_t read_len,
                                                 uint32_t addr,
                                                 uint32_t addr_bytes_number,
                                                 uint32_t dummy_cycles);

/**
 * \brief Send a write command to the flash memory device using the Software
 *        Triggered Instruction Generator (STIG). This command can be used to
 *        write Flash memory or registers.
 *
 * \param[in]  dev               QSPI IP6514E device struct
 *                               \ref qspi_ip6514e_dev_t
 * \param[in]  opcode            Opcode for the command.
 * \param[in]  write_data        Pointer to a memory zone where are
 *                               located the write_len bytes to write for
 *                               this command.
 * \param[in]  write_len         Number of bytes to write for the command.
 *                               Between 1 and 8 bytes (both included) can be
 *                               written.
 * \param[in]  addr              Address used for the command
 * \param[in]  addr_bytes_number Number of address bytes for this command.
 *                               If an address is not needed for the command,
 *                               use 0 for argument, otherwise between 1 and
 *                               4 bytes (both included) can be used.
 * \param[in]  dummy_cycles      Number of dummy cycles required for the
 *                               command, between 0 and 31 (both included).
 *
 * \return Returns error code as specified in \ref qspi_ip6514e_error_t
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_ip6514e_activate_qspi_mode function or the default one.
 */
enum qspi_ip6514e_error_t qspi_ip6514e_send_write_cmd(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint8_t opcode,
                                                 const void *write_data,
                                                 uint32_t write_len,
                                                 uint32_t addr,
                                                 uint32_t addr_bytes_number,
                                                 uint32_t dummy_cycles);

#ifdef __cplusplus
}
#endif

#endif /* __QSPI_IP6514E_DRV_H__ */
