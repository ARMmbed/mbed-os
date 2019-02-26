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

#include <stdlib.h>
/* Use memcpy function */
#include <string.h>

#include "mt25ql_flash_lib.h"

/** Setter bit manipulation macro */
#define SET_BIT(WORD, BIT_INDEX) ((WORD) |= (1U << (BIT_INDEX)))
/** Clearing bit manipulation macro */
#define CLR_BIT(WORD, BIT_INDEX) ((WORD) &= ~(1U << (BIT_INDEX)))
/** Getter bit manipulation macro */
#define GET_BIT(WORD, BIT_INDEX) (bool)(((WORD) & (1U << (BIT_INDEX))))

#define BITS_PER_WORD  32U
#define BYTES_PER_WORD 4U

#define ARG_NOT_USED     0
#define ARG_PTR_NOT_USED NULL

/** MT25QL used command */
#define WRITE_ENABLE_CMD                    0x06U
#define READ_ENHANCED_VOLATILE_CFG_REG_CMD  0x65U
#define WRITE_ENHANCED_VOLATILE_CFG_REG_CMD 0x61U
#define READ_VOLATILE_CFG_REG_CMD           0x85U
#define WRITE_VOLATILE_CFG_REG_CMD          0x81U
#define READ_FLAG_STATUS_REG_CMD            0x70U
#define SUBSECTOR_ERASE_32KB_CMD            0x52U
#define SUBSECTOR_ERASE_4KB_CMD             0x20U
#define SECTOR_ERASE_CMD                    0xD8U
#define BULK_ERASE_CMD                      0xC7U
/*
 * The baud rate divisor in \ref mt25ql_dev_t needs to be configured adequately
 * to handle those commands.
 */
#define QUAD_OUTPUT_FAST_READ_CMD           0x6BU
#define FAST_READ_CMD                       0x0BU
#define READ_CMD                            0x03U
#define QUAD_INPUT_FAST_PROGRAM_CMD         0x32U
#define PAGE_PROGRAM_CMD                    0x02U

/** MT25QL Enhanced Volatile Configuration Register access */
#define ENHANCED_VOLATILE_CFG_REG_LEN      1U
#define ENHANCED_VOLATILE_CFG_REG_QSPI_POS 7U
#define ENHANCED_VOLATILE_CFG_REG_DSPI_POS 6U

/** MT25QL Volatile Configuration Register access */
#define VOLATILE_CFG_REG_LEN               1U
#define VOLATILE_CFG_REG_DUMMY_CYCLES_POS  4U
#define VOLATILE_CFG_REG_DUMMY_CYCLES_BITS 4U

/** MT25QL Flag Status Register access */
#define FLAG_STATUS_REG_LEN       1U
#define FLAG_STATUS_REG_READY_POS 7U

/*
 * 8 is the minimal number of dummy clock cycles needed to reach the maximal
 * frequency of the Quad Output Fast Read Command.
 */
#define QUAD_OUTPUT_FAST_READ_DUMMY_CYCLES    8U
#define FAST_READ_DUMMY_CYCLES                8U
#define DEFAULT_READ_DUMMY_CYCLES             0U
#define QUAD_INPUT_FAST_PROGRAM_DUMMY_CYCLES  0U
#define PAGE_PROGRAM_DUMMY_CYCLES             0U

/* Only up to 8 bytes can be read or written using the Flash commands. */
#define CMD_DATA_MAX_SIZE 8U

/**
 * \brief Change specific bits in a 32 bits word.
 *
 * \param[in,out] word         Pointer of the word to change
 * \param[in]     bits         bits_length bits to put at bits_pos in the word
 *                             pointed
 * \param[in]     bits_length  Number of bits to change
 * \param[in]     bits_pos     Position of the bits to change
 *
 * \note This function will do nothing if the parameters given are incorrect:
 *         * word is NULL
 *         * bits_length + bits_pos > 32
 *         * bits_length is 0
 */
static void change_bits_in_word(volatile uint32_t *word,
                                uint32_t bits,
                                uint32_t bits_length,
                                uint32_t bits_pos)
{
    uint32_t mask;

    if ((word == NULL) ||
        ((bits_length + bits_pos) > BITS_PER_WORD) ||
        (bits_length == 0U)) {
        /* Silently fail */
        return;
    }

    /* Change all the bits */
    if (bits_length == BITS_PER_WORD) {
        *word = bits;
        return;
    }

    mask = ((1U << bits_length) - 1);
    /*
     * We change the bits in three steps:
     *   - clear bits_length bits with zeroes at bits_pos in the word
     *   - mask bits in case it contains more than bits_length bits
     *   - set the new bits in the cleared word
     * Because the data pointed by word is only read once, the data will still
     * be coherent after an interruption that changes it.
     */
    *word = ((*word & ~(mask << bits_pos)) | ((bits & mask) << bits_pos));
}

/**
 * \brief Send the Write Enable command, needed before any write.
 *
 * \param[in] dev     Pointer to MT25QL device structure \ref mt25ql_dev_t
 */
static void send_write_enable(struct mt25ql_dev_t* dev)
{
    qspi_ip6514e_send_simple_cmd(dev->controller, WRITE_ENABLE_CMD);
}

/**
 * \brief Set SPI mode on the flash device and on the controller.
 *
 * \param[in] dev       Pointer to MT25QL device structure \ref mt25ql_dev_t
 * \param[in] spi_mode  SPI mode to be set on flash device and controller
 *                      \ref qspi_ip6514e_spi_mode_t
 *
 * \return Return error code as specified in \ref mt25ql_error_t
 */
static enum mt25ql_error_t set_spi_mode(struct mt25ql_dev_t* dev,
                                        enum qspi_ip6514e_spi_mode_t spi_mode)
{
    uint8_t enhanced_volatile_cfg_reg = 0;
    enum qspi_ip6514e_error_t controller_error;

    /* Read the Enhanced Volatile Configuration Register, modify it according
     * to the requested SPI mode then write back the modified value to the
     * register. This will activate the SPI mode on the flash side.
     */
    controller_error = qspi_ip6514e_send_read_cmd(
                                             dev->controller,
                                             READ_ENHANCED_VOLATILE_CFG_REG_CMD,
                                             &enhanced_volatile_cfg_reg,
                                             ENHANCED_VOLATILE_CFG_REG_LEN,
                                             ARG_NOT_USED,
                                             ARG_NOT_USED,
                                             0); /* No dummy cycles needed for
                                                    this command. */
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    switch(spi_mode) {
    case QSPI_IP6514E_SPI_MODE:
        /* Disable the Dual- and Quad-SPI modes.
         * Clearing the bit enables the mode, setting it disables it.
         */
        SET_BIT(enhanced_volatile_cfg_reg, ENHANCED_VOLATILE_CFG_REG_DSPI_POS);
        SET_BIT(enhanced_volatile_cfg_reg, ENHANCED_VOLATILE_CFG_REG_QSPI_POS);
        break;
    case QSPI_IP6514E_DSPI_MODE:
        /* Disable the Quad-SPI mode and activate DSPI mode.
         * Clearing the bit enables the mode, setting it disables it.
         */
        CLR_BIT(enhanced_volatile_cfg_reg, ENHANCED_VOLATILE_CFG_REG_DSPI_POS);
        SET_BIT(enhanced_volatile_cfg_reg, ENHANCED_VOLATILE_CFG_REG_QSPI_POS);
        break;
    case QSPI_IP6514E_QSPI_MODE:
        /* Disable the Dual-SPI mode and activate QSPI mode.
         * Clearing the bit enables the mode, setting it disables it.
         */
        SET_BIT(enhanced_volatile_cfg_reg, ENHANCED_VOLATILE_CFG_REG_DSPI_POS);
        CLR_BIT(enhanced_volatile_cfg_reg, ENHANCED_VOLATILE_CFG_REG_QSPI_POS);
        break;
    default:
        return MT25QL_ERR_WRONG_ARGUMENT;
    }

    send_write_enable(dev);

    controller_error = qspi_ip6514e_send_write_cmd(
                                            dev->controller,
                                            WRITE_ENHANCED_VOLATILE_CFG_REG_CMD,
                                            &enhanced_volatile_cfg_reg,
                                            ENHANCED_VOLATILE_CFG_REG_LEN,
                                            ARG_NOT_USED,
                                            ARG_NOT_USED,
                                            0); /* No dummy cycles needed for
                                                   this command. */
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    /* Activate the requested SPI mode on the controller side as well. */
    controller_error = qspi_ip6514e_set_spi_mode(dev->controller,
                                                 spi_mode,
                                                 spi_mode,
                                                 spi_mode);
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    return MT25QL_ERR_NONE;
}

/**
 * \brief Change the number of dummy clock cycles subsequent to all FAST READ
 *        commands.
 *
 * \param[in] dev          Pointer to MT25QL device structure \ref mt25ql_dev_t
 * \param[in] dummy_cycles Dummy clock cycles to set
 *
 * \return Return error code as specified in \ref mt25ql_error_t
 */
static enum mt25ql_error_t change_dummy_cycles(struct mt25ql_dev_t* dev,
                                               uint32_t dummy_cycles)
{
    uint32_t volatile_cfg_reg = 0;
    enum qspi_ip6514e_error_t controller_error;

    /*
     * Changes the number of dummy cycles in the Volatile Configuration
     * Register.
     */
    controller_error = qspi_ip6514e_send_read_cmd(dev->controller,
                                                  READ_VOLATILE_CFG_REG_CMD,
                                                  &volatile_cfg_reg,
                                                  VOLATILE_CFG_REG_LEN,
                                                  ARG_NOT_USED,
                                                  ARG_NOT_USED,
                                                  0); /* No dummy cycles needed
                                                         for this command. */
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    change_bits_in_word(&volatile_cfg_reg,
                        dummy_cycles,
                        VOLATILE_CFG_REG_DUMMY_CYCLES_BITS,
                        VOLATILE_CFG_REG_DUMMY_CYCLES_POS);

    send_write_enable(dev);

    controller_error = qspi_ip6514e_send_write_cmd(dev->controller,
                                                   WRITE_VOLATILE_CFG_REG_CMD,
                                                   &volatile_cfg_reg,
                                                   VOLATILE_CFG_REG_LEN,
                                                   ARG_NOT_USED,
                                                   ARG_NOT_USED,
                                                   0); /* No dummy cycles needed
                                                          for this command. */
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    return MT25QL_ERR_NONE;
}

/**
 * \brief Wait until the current program/erase is finished.
 *
 * \param[in] dev     Pointer to MT25QL device structure \ref mt25ql_dev_t
 *
 * \return Return error code as specified in \ref mt25ql_error_t
 */
static enum mt25ql_error_t wait_program_or_erase_complete(
                                                       struct mt25ql_dev_t* dev)
{
    enum qspi_ip6514e_error_t controller_error;
    uint8_t flag_status_reg = 0;

    /* Wait until the ready bit of the Flag Status Register is set */
    while (!GET_BIT(flag_status_reg, FLAG_STATUS_REG_READY_POS)) {
        controller_error = qspi_ip6514e_send_read_cmd(dev->controller,
                                                      READ_FLAG_STATUS_REG_CMD,
                                                      &flag_status_reg,
                                                      FLAG_STATUS_REG_LEN,
                                                      ARG_NOT_USED,
                                                      ARG_NOT_USED,
                                                      0); /* No dummy cycles
                                                             needed for this
                                                             command. */
        if (controller_error != QSPI_IP6514E_ERR_NONE) {
            return (enum mt25ql_error_t)controller_error;
        }
    }

    return MT25QL_ERR_NONE;
}

/**
 * \brief Execute a program command that crosses the page size boundary.
 *
 * \param[in]  dev               Pointer to MT25QL device structure
 *                               \ref mt25ql_dev_t
 * \param[in]  opcode            Opcode for the command.
 * \param[in]  write_data        Pointer to a memory zone where the write_len
 *                               number of bytes are located to write for this
 *                               command.
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
 * \return Return error code as specified in \ref mt25ql_error_t
 *
 * \note This function will execute two commands: one to program the bytes up to
 *       the page boundary and another one to program the rest. It will wait
 *       that bytes are programmed from first command before triggering the
 *       second one.
 * \note This function does not send a write enable command before the first
 *       command and does not check that bytes were programmed after the second
 *       command.
 */
static enum mt25ql_error_t send_boundary_cross_write_cmd(
                                                     struct mt25ql_dev_t* dev,
                                                     uint8_t opcode,
                                                     const void *write_data,
                                                     uint32_t write_len,
                                                     uint32_t addr,
                                                     uint32_t addr_bytes_number,
                                                     uint32_t dummy_cycles)
{
    enum qspi_ip6514e_error_t controller_error;
    enum mt25ql_error_t library_error;
    /*
     * Remaining bytes between the current address and the end of the current
     * page.
     */
    uint32_t page_remainder = FLASH_PAGE_SIZE - (addr % FLASH_PAGE_SIZE);

    /* First write up to the end of the current page. */
    controller_error = qspi_ip6514e_send_write_cmd(dev->controller,
                                                   opcode,
                                                   write_data,
                                                   page_remainder,
                                                   addr,
                                                   addr_bytes_number,
                                                   dummy_cycles);
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    write_data = (void *)((uint32_t)write_data + page_remainder);
    addr += page_remainder;

    /* Wait for the page to be written before sending new commands. */
    library_error = wait_program_or_erase_complete(dev);
    if (library_error != MT25QL_ERR_NONE) {
        return library_error;
    }

    /* Then write the remaining data of the write_len bytes. */
    send_write_enable(dev);
    controller_error = qspi_ip6514e_send_write_cmd(dev->controller,
                                                   opcode,
                                                   write_data,
                                                   write_len - page_remainder,
                                                   addr,
                                                   addr_bytes_number,
                                                   dummy_cycles);
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    return MT25QL_ERR_NONE;
}

enum mt25ql_error_t mt25ql_config_mode(struct mt25ql_dev_t* dev,
                                       enum mt25ql_functional_state_t config)
{
    enum qspi_ip6514e_spi_mode_t spi_mode;
    enum qspi_ip6514e_error_t controller_error;
    enum mt25ql_error_t library_error;
    uint8_t opcode_read;
    uint8_t opcode_write;
    uint32_t dummy_cycles_read;
    uint32_t dummy_cycles_write;

    switch(config) {
    case MT25QL_FUNC_STATE_DEFAULT:
        spi_mode            = QSPI_IP6514E_SPI_MODE;
        opcode_read         = READ_CMD;
        dummy_cycles_read   = DEFAULT_READ_DUMMY_CYCLES;
        opcode_write        = PAGE_PROGRAM_CMD;
        dummy_cycles_write  = PAGE_PROGRAM_DUMMY_CYCLES;
        break;
    case MT25QL_FUNC_STATE_FAST:
        spi_mode            = QSPI_IP6514E_SPI_MODE;
        opcode_read         = FAST_READ_CMD;
        dummy_cycles_read   = FAST_READ_DUMMY_CYCLES;
        opcode_write        = PAGE_PROGRAM_CMD;
        dummy_cycles_write  = PAGE_PROGRAM_DUMMY_CYCLES;
        break;
    case MT25QL_FUNC_STATE_QUAD_FAST:
        spi_mode            = QSPI_IP6514E_QSPI_MODE;
        opcode_read         = QUAD_OUTPUT_FAST_READ_CMD;
        dummy_cycles_read   = QUAD_OUTPUT_FAST_READ_DUMMY_CYCLES;
        opcode_write        = QUAD_INPUT_FAST_PROGRAM_CMD;
        dummy_cycles_write  = QUAD_INPUT_FAST_PROGRAM_DUMMY_CYCLES;
        break;
    default:
        return MT25QL_ERR_WRONG_ARGUMENT;
    }

    /* This function will first set the Flash memory SPI mode and then set
     * the controller's SPI mode. It will fail if the two sides do not have
     * the same mode when this function is called.
     */
    library_error = set_spi_mode(dev, spi_mode);
    if (library_error != MT25QL_ERR_NONE) {
        return library_error;
    }

    /* Set the number of dummy cycles for read commands. */
    library_error = change_dummy_cycles(dev, dummy_cycles_read);
    if (library_error != MT25QL_ERR_NONE) {
        return library_error;
    }

    /* The rest of the configuration needs the controller to be disabled */
    while(!qspi_ip6514e_is_idle(dev->controller));
    qspi_ip6514e_disable(dev->controller);

    /* Set the baud rate divisor as configured in the device structure. */
    controller_error = qspi_ip6514e_set_baud_rate_div(dev->controller,
                                                      dev->baud_rate_div);
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    /* Set opcode and dummy cycles needed for read commands. */
    controller_error = qspi_ip6514e_cfg_reads(dev->controller,
                                              opcode_read,
                                              dummy_cycles_read);
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    /* Set opcode and dummy cycles needed for write commands. */
    controller_error = qspi_ip6514e_cfg_writes(dev->controller,
                                               opcode_write,
                                               dummy_cycles_write);
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    /* Set Flash memory constants: bytes per page and address bytes. */
    controller_error = qspi_ip6514e_cfg_page_size(dev->controller,
                                                  FLASH_PAGE_SIZE);
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    controller_error = qspi_ip6514e_cfg_addr_bytes(dev->controller,
                                                   ADDR_BYTES);
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    qspi_ip6514e_enable(dev->controller);

    dev->func_state = config;

    return MT25QL_ERR_NONE;
}

enum mt25ql_error_t mt25ql_restore_default_state(struct mt25ql_dev_t* dev)
{
    enum mt25ql_error_t library_error;

    /*
     * This function will first change the Flash memory mode to single SPI and
     * then change the controller to single SPI. It will fail if the two sides
     * do not have the same mode when this function is called.
     */
    library_error = set_spi_mode(dev, QSPI_IP6514E_SPI_MODE);
    if (library_error != MT25QL_ERR_NONE) {
        return library_error;
    }

    /* Set the default number of dummy cycles for read commands. */
    library_error = change_dummy_cycles(dev, DEFAULT_READ_DUMMY_CYCLES);
    if (library_error != MT25QL_ERR_NONE) {
        return library_error;
    }

    /* The rest of the configuration needs the controller to be disabled */
    while(!qspi_ip6514e_is_idle(dev->controller));
    qspi_ip6514e_disable(dev->controller);

    /* Restore the default value of the QSPI controller registers. */
    qspi_ip6514e_reset_regs(dev->controller);

    qspi_ip6514e_enable(dev->controller);

    dev->func_state = MT25QL_FUNC_STATE_DEFAULT;

    return MT25QL_ERR_NONE;
}

enum mt25ql_error_t mt25ql_direct_read(struct mt25ql_dev_t* dev,
                                       uint32_t addr,
                                       void *data,
                                       uint32_t len)
{
    /*
     * The direct access window size is the size of the memory that can be
     * accessed with a direct access.
     */
    uint32_t direct_access_window_size = dev->controller->cfg->addr_mask + 1;
    /*
     * The window number is the number of times it will be needed to remap the
     * address with the remap register. We move this Direct Access window first
     * window_number times starting at the beginning address to read full
     * windows of direct_access_window_size bytes. Then we read the remainder
     * bytes.
     */
    uint32_t window_number = len / direct_access_window_size;

    if (data == NULL || len == 0) {
        return MT25QL_ERR_WRONG_ARGUMENT;
    }

    if ((addr + len) >= dev->size) {
        return MT25QL_ERR_ADDR_TOO_BIG;
    }

    /*
     * There is no limitation reading through a Flash page boundary hence we
     * do not add the same logic here than in the write function.
     */

    /* Transfer the bytes for the window_number windows first. */
    for (uint32_t window = 0; window < window_number; window++) {
        qspi_ip6514e_remap_addr(dev->controller, addr);

        /*
         * The AHB address to access the Flash memory does not change but it
         * will be translated differently thanks to the remap function.
         */
        memcpy(data,
               (void *)dev->direct_access_start_addr,
               direct_access_window_size);

        len -= direct_access_window_size;
        data = (void *)((uint32_t)data + direct_access_window_size);
        addr += direct_access_window_size;
    }

    if (len) {
        /* Transfer the reminder bytes */
        qspi_ip6514e_remap_addr(dev->controller, addr);

        memcpy(data, (void *)dev->direct_access_start_addr, len);
    }

    /* Disable remapping for direct accesses outside of this function. */
    qspi_ip6514e_disable_remap(dev->controller);

    return MT25QL_ERR_NONE;
}

enum mt25ql_error_t mt25ql_direct_write(struct mt25ql_dev_t* dev,
                                        uint32_t addr,
                                        const void *data,
                                        uint32_t len)
{
    enum mt25ql_error_t library_error;
    /*
     * The direct access window size is the size of the memory that can be
     * accessed with a direct access.
     */
    uint32_t direct_access_window_size = dev->controller->cfg->addr_mask + 1;
    uint32_t window_number;
    /* Offset between address and the previous 32 bits aligned word */
    uint32_t word_offset;

    if (data == NULL || len == 0) {
        return MT25QL_ERR_WRONG_ARGUMENT;
    }

    if ((addr + len) >= dev->size) {
        return MT25QL_ERR_ADDR_TOO_BIG;
    }

    /*
     * If the remapping address is not aligned on a 32 bits boundary, a direct
     * access of one word could cross a Flash page boundary. If that happens,
     * the bytes of that word that are over the page boundary will instead be
     * written at the beginning of the same page.
     * To counter this problem, we align the remapping address and add the word
     * offset to the address of the direct access for the first window only.
     */
    word_offset = addr % BYTES_PER_WORD;
    /* Make address aligned on a 32 bits alignment. */
    addr -= word_offset;
    /*
     * Only direct_access_window_size address locations are available by direct
     * access. We calculate the number of windows that we will need to transfer
     * len bytes. We have to add in the window the offset that we add in the
     * beginning.
     */
    window_number = (len + word_offset) / direct_access_window_size;

    /*
     * This function assumes that the flash has already been erased.
     * Transfer the bytes for the window_number windows first.
     */
    for (uint32_t window = 0; window < window_number; window++) {
        /* The controller needs to be disabled while remapping is done. */
        qspi_ip6514e_remap_addr(dev->controller, addr);

        /*
         * The AHB address to access the Flash memory does not change but it
         * will be translated differently thanks to the remap function.
         */
        memcpy((void *)(dev->direct_access_start_addr + word_offset),
               data,
               direct_access_window_size - word_offset);

        len -= (direct_access_window_size - word_offset);
        data = (void *)((uint32_t)data +
                        (direct_access_window_size - word_offset));
        addr += direct_access_window_size;

        /*
         * The address is now aligned, there is no need to add an offset for the
         * remaining windows.
         */
        word_offset = 0;

        /*
         * Wait until the last program operation is complete before changing
         * the remap address.
         */
        library_error = wait_program_or_erase_complete(dev);
        if (library_error != MT25QL_ERR_NONE) {
            return library_error;
        }
    }

    if (len) {
        /* Transfer the reminder bytes */
        qspi_ip6514e_remap_addr(dev->controller, addr);

        memcpy((void *)(dev->direct_access_start_addr + word_offset),
               data,
               len);

        /* Wait until the last program operation is complete */
        library_error = wait_program_or_erase_complete(dev);
        if (library_error != MT25QL_ERR_NONE) {
            return library_error;
        }
    }

    /*
     * Disable the default remap address for direct accesses outside of this
     * function.
     */
    qspi_ip6514e_disable_remap(dev->controller);

    return MT25QL_ERR_NONE;
}

enum mt25ql_error_t mt25ql_command_read(struct mt25ql_dev_t* dev,
                                        uint32_t addr,
                                        void *data,
                                        uint32_t len)
{
    /* With one single command only 8 bytes can be read. */
    uint32_t cmd_number = len / CMD_DATA_MAX_SIZE;
    enum qspi_ip6514e_error_t controller_error;
    uint8_t opcode;
    uint32_t dummy_cycles;

    switch (dev->func_state) {
    case MT25QL_FUNC_STATE_QUAD_FAST:
        opcode = QUAD_OUTPUT_FAST_READ_CMD;
        dummy_cycles = QUAD_OUTPUT_FAST_READ_DUMMY_CYCLES;
        break;
    case MT25QL_FUNC_STATE_FAST:
        opcode = FAST_READ_CMD;
        dummy_cycles = FAST_READ_DUMMY_CYCLES;
        break;
    case MT25QL_FUNC_STATE_DEFAULT:
    default:
        opcode = READ_CMD;
        dummy_cycles = DEFAULT_READ_DUMMY_CYCLES;
        break;
    }

    for (uint32_t cmd_index = 0; cmd_index < cmd_number; cmd_index++) {
        controller_error = qspi_ip6514e_send_read_cmd(
                                                    dev->controller,
                                                    opcode,
                                                    data,
                                                    CMD_DATA_MAX_SIZE,
                                                    addr,
                                                    ADDR_BYTES,
                                                    dummy_cycles);
        if (controller_error != QSPI_IP6514E_ERR_NONE) {
            return (enum mt25ql_error_t)controller_error;
        }

        data = (void *)((uint32_t)data + CMD_DATA_MAX_SIZE);
        addr += CMD_DATA_MAX_SIZE;
        len -= CMD_DATA_MAX_SIZE;
    }

    if (len) {
        /* Read the remainder. */
        controller_error = qspi_ip6514e_send_read_cmd(
                                                     dev->controller,
                                                     opcode,
                                                     data,
                                                     len,
                                                     addr,
                                                     ADDR_BYTES,
                                                     dummy_cycles);
        if (controller_error != QSPI_IP6514E_ERR_NONE) {
            return (enum mt25ql_error_t)controller_error;
        }
    }

    return MT25QL_ERR_NONE;
}

enum mt25ql_error_t mt25ql_command_write(struct mt25ql_dev_t* dev,
                                         uint32_t addr,
                                         const void *data,
                                         uint32_t len)
{
    /* With one single command only 8 bytes can be written. */
    uint32_t cmd_number = len / CMD_DATA_MAX_SIZE;
    enum qspi_ip6514e_error_t controller_error;
    enum mt25ql_error_t library_error;
    uint8_t opcode;
    uint32_t dummy_cycles;

    switch (dev->func_state) {
    case MT25QL_FUNC_STATE_QUAD_FAST:
        opcode = QUAD_INPUT_FAST_PROGRAM_CMD;
        dummy_cycles = QUAD_INPUT_FAST_PROGRAM_DUMMY_CYCLES;
        break;
    case MT25QL_FUNC_STATE_FAST:
    case MT25QL_FUNC_STATE_DEFAULT:
    default:
        opcode = PAGE_PROGRAM_CMD;
        dummy_cycles = PAGE_PROGRAM_DUMMY_CYCLES;
        break;
    }

    for (uint32_t cmd_index = 0; cmd_index < cmd_number; cmd_index++) {
        send_write_enable(dev);

        /*
         * Check if this command is not writing over a page boundary: first and
         * last bytes are in the same page.
         */
        if ((addr / FLASH_PAGE_SIZE) !=
            ((addr + CMD_DATA_MAX_SIZE - 1) / FLASH_PAGE_SIZE)) {
            /* The CMD_DATA_MAX_SIZE bytes written are crossing the boundary. */
            library_error = send_boundary_cross_write_cmd(
                                                    dev,
                                                    opcode,
                                                    data,
                                                    CMD_DATA_MAX_SIZE,
                                                    addr,
                                                    ADDR_BYTES,
                                                    dummy_cycles);
            if (library_error != MT25QL_ERR_NONE) {
                return library_error;
            }
        } else {
            /* Normal case: not crossing the boundary. */
            controller_error = qspi_ip6514e_send_write_cmd(
                                                    dev->controller,
                                                    opcode,
                                                    data,
                                                    CMD_DATA_MAX_SIZE,
                                                    addr,
                                                    ADDR_BYTES,
                                                    dummy_cycles);
            if (controller_error != QSPI_IP6514E_ERR_NONE) {
                return (enum mt25ql_error_t)controller_error;
            }
        }

        /* Wait until the write operation is complete. */
        library_error = wait_program_or_erase_complete(dev);
        if (library_error != MT25QL_ERR_NONE) {
            return library_error;
        }

        data = (void *)((uint32_t)data + CMD_DATA_MAX_SIZE);
        addr += CMD_DATA_MAX_SIZE;
        len -= CMD_DATA_MAX_SIZE;
    }

    if (len) {
        /* Write the remainder. */
        send_write_enable(dev);
        /*
         * Check if this command is not writing over a page boundary: first and
         * last bytes are in the same page.
         */
        if ((addr / FLASH_PAGE_SIZE) != ((addr + len - 1) / FLASH_PAGE_SIZE)) {
            /* The CMD_DATA_MAX_SIZE bytes written are crossing the boundary. */
            library_error = send_boundary_cross_write_cmd(
                                                    dev,
                                                    opcode,
                                                    data,
                                                    len,
                                                    addr,
                                                    ADDR_BYTES,
                                                    dummy_cycles);
            if (library_error != MT25QL_ERR_NONE) {
                return library_error;
            }
        } else {
            /* Normal case: not crossing the boundary. */
            controller_error = qspi_ip6514e_send_write_cmd(
                                                    dev->controller,
                                                    opcode,
                                                    data,
                                                    len,
                                                    addr,
                                                    ADDR_BYTES,
                                                    dummy_cycles);
            if (controller_error != QSPI_IP6514E_ERR_NONE) {
                return (enum mt25ql_error_t)controller_error;
            }
        }

        /* Wait until the write operation is complete. */
        library_error = wait_program_or_erase_complete(dev);
        if (library_error != MT25QL_ERR_NONE) {
            return library_error;
        }
    }


    return MT25QL_ERR_NONE;
}

enum mt25ql_error_t mt25ql_erase(struct mt25ql_dev_t* dev,
                                 uint32_t addr,
                                 enum mt25ql_erase_t erase_type)
{
    enum qspi_ip6514e_error_t controller_error;
    enum mt25ql_error_t library_error;
    uint8_t erase_cmd;
    uint32_t addr_bytes;

    send_write_enable(dev);

    switch (erase_type) {
    case MT25QL_ERASE_ALL_FLASH:
        if (addr != 0) {
            return MT25QL_ERR_ADDR_NOT_ALIGNED;
        }
        erase_cmd = BULK_ERASE_CMD;
        addr_bytes = ARG_NOT_USED;
        break;
    case MT25QL_ERASE_SECTOR_64K:
        erase_cmd = SECTOR_ERASE_CMD;
        addr_bytes = ADDR_BYTES;
        if ((addr % SECTOR_64KB) != 0) {
            return MT25QL_ERR_ADDR_NOT_ALIGNED;
        }
        break;
    case MT25QL_ERASE_SUBSECTOR_32K:
        erase_cmd = SUBSECTOR_ERASE_32KB_CMD;
        addr_bytes = ADDR_BYTES;
        if ((addr % SUBSECTOR_32KB) != 0) {
            return MT25QL_ERR_ADDR_NOT_ALIGNED;
        }
        break;
    case MT25QL_ERASE_SUBSECTOR_4K:
        erase_cmd = SUBSECTOR_ERASE_4KB_CMD;
        addr_bytes = ADDR_BYTES;
        if ((addr % SUBSECTOR_4KB) != 0) {
            return MT25QL_ERR_ADDR_NOT_ALIGNED;
        }
        break;
    default:
        return MT25QL_ERR_WRONG_ARGUMENT;
    }

    if (addr >= dev->size) {
        return MT25QL_ERR_ADDR_TOO_BIG;
    }

    controller_error = qspi_ip6514e_send_cmd(dev->controller,
                                             erase_cmd,
                                             ARG_PTR_NOT_USED,
                                             ARG_NOT_USED,
                                             ARG_PTR_NOT_USED,
                                             ARG_NOT_USED,
                                             addr,
                                             addr_bytes,
                                             0); /* No dummy cycles needed for
                                                    any erase command. */
    if (controller_error != QSPI_IP6514E_ERR_NONE) {
        return (enum mt25ql_error_t)controller_error;
    }

    /* Wait until the erase operation is complete */
    library_error = wait_program_or_erase_complete(dev);
    if (library_error != MT25QL_ERR_NONE) {
         return (enum mt25ql_error_t)controller_error;
    }

    return MT25QL_ERR_NONE;
}
