/*
 * Copyright (c) 2018-2019 Arm Limited
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

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
/* Use memcpy */
#include <string.h>

#include "qspi_ip6514e_drv.h"

/** Setter bit manipulation macro */
#define SET_BIT(WORD, BIT_INDEX) ((WORD) |= (1U << (BIT_INDEX)))
/** Clearing bit manipulation macro */
#define CLR_BIT(WORD, BIT_INDEX) ((WORD) &= ~(1U << (BIT_INDEX)))
/** Getter bit manipulation macro */
#define GET_BIT(WORD, BIT_INDEX) (bool)(((WORD) & (1U << (BIT_INDEX))))

#define WORD_ALIGN_4B_MASK    0x3U /* Mask the first 2 bits */
#define IS_ADDR_ALIGNED(ADDR) (((uint32_t)(ADDR) & (WORD_ALIGN_4B_MASK)) == 0U)

#define BITS_PER_BYTE               8U
#define BITS_PER_WORD               32U

#define CFG_READS  true
#define CFG_WRITES false

#define ARG_NOT_USED     0
#define ARG_PTR_NOT_USED NULL

#define DATA_REG_NUMBER 2U
#define DATA_REG_LOWER  0U
#define DATA_REG_UPPER  1U

#define ERROR_VALUE 0xFFFFFFFFU

/**
 * \brief QSPI IP6514E register map structure
 */
struct _qspi_ip6514e_reg_map_t {
    volatile uint32_t qspi_cfg;                           /*!< 0x00 (R/W) */
    volatile uint32_t device_read_inst;                   /*!< 0x04 (R/W) */
    volatile uint32_t device_write_inst;                  /*!< 0x08 (R/W) */
    volatile uint32_t hidden1[2];
    volatile uint32_t device_size;                        /*!< 0x14 (R/W) */
    volatile uint32_t hidden2[3];
    volatile uint32_t remap_addr;                         /*!< 0x24 (R/W) */
    volatile uint32_t hidden3[26];
    volatile uint32_t flash_cmd_ctrl;                     /*!< 0x90 (R/W) */
    volatile uint32_t flash_cmd_addr;                     /*!< 0x94 (R/W) */
    volatile uint32_t hidden4[2];
    volatile uint32_t flash_cmd_read_data_lower;          /*!< 0xA0 (R/ ) */
    volatile uint32_t flash_cmd_read_data_upper;          /*!< 0xA4 (R/ ) */
    volatile uint32_t flash_cmd_write_data_lower;         /*!< 0xA8 (R/W) */
    volatile uint32_t flash_cmd_write_data_upper;         /*!< 0xAC (R/W) */
    volatile uint32_t hidden5[2];
};

/** QSPI Configuration register description (offset 0x00) */
#define QSPI_CFG_ENABLE_POS            0U
#define QSPI_CFG_ENABLE_ADDR_REMAP_POS 16U
#define QSPI_CFG_BAUD_DIV_POS          19U
    #define QSPI_CFG_BAUD_DIV_MIN          2U
    #define QSPI_CFG_BAUD_DIV_MAX          32U
    #define QSPI_CFG_BAUD_DIV_BITS         4U
#define QSPI_CFG_IDLE_POS              31U

/**
 * Device Read/Write Instruction registers description (offset 0x04 and 0x08).
 * These values are the same for the Device Read Instruction register at offset
 * 0x04 and the Device Write Instruction register at offset 0x08.
 */
#define DEVICE_READ_WRITE_INST_OPCODE_POS        0U
#define DEVICE_READ_INST_INST_TYPE_POS           8U /* Only applies to the Read
                                                     * register. */
#define DEVICE_READ_WRITE_INST_ADDR_TYPE_POS     12U
#define DEVICE_READ_WRITE_INST_DATA_TYPE_POS     16U
    #define DEVICE_READ_WRITE_INST_MODE_QSPI         2U
    #define DEVICE_READ_WRITE_INST_MODE_DSPI         1U
    #define DEVICE_READ_WRITE_INST_MODE_SPI          0U
    #define DEVICE_READ_WRITE_INST_MODE_BITS         2U
#define DEVICE_READ_WRITE_INST_DUMMY_CYCLES_POS  24U
    #define DEVICE_READ_WRITE_INST_DUMMY_CYCLES_BITS 5U
    #define DEVICE_READ_WRITE_INST_DUMMY_CYCLES_MAX  31U

/** Device Size Configuration register description (offset 0x14) */
#define DEVICE_SIZE_ADDR_BYTES_POS  0U
    #define DEVICE_SIZE_ADDR_BYTES_MIN  1U
    #define DEVICE_SIZE_ADDR_BYTES_MAX  16U
    #define DEVICE_SIZE_ADDR_BYTES_BITS 4U
#define DEVICE_SIZE_PAGE_BYTES_POS  4U
    #define DEVICE_SIZE_PAGE_BYTES_MAX  4095U
    #define DEVICE_SIZE_PAGE_BYTES_BITS 12U

/** Flash Command Control register description (offset 0x90) */
#define FLASH_CMD_CTRL_EXECUTE_POS        0U
#define FLASH_CMD_CTRL_BUSY_POS           1U
#define FLASH_CMD_CTRL_DUMMY_CYCLES_POS   7U
    #define FLASH_CMD_CTRL_DUMMY_CYCLES_MAX   31U
    #define FLASH_CMD_CTRL_DUMMY_CYCLES_BITS  5U
#define FLASH_CMD_CTRL_WRITE_BYTES_POS    12U
    #define FLASH_CMD_CTRL_WRITE_BYTES_MAX    8U
    #define FLASH_CMD_CTRL_WRITE_BYTES_BITS   3U
#define FLASH_CMD_CTRL_WRITE_ENABLE_POS   15U
#define FLASH_CMD_CTRL_ADDR_BYTES_POS     16U
    #define FLASH_CMD_CTRL_ADDR_BYTES_MAX     4U
    #define FLASH_CMD_CTRL_ADDR_BYTES_BITS    2U
#define FLASH_CMD_CTRL_ADDR_ENABLE_POS    19U
#define FLASH_CMD_CTRL_READ_BYTES_POS     20U
    #define FLASH_CMD_CTRL_READ_BYTES_MAX     8U
    #define FLASH_CMD_CTRL_READ_BYTES_BITS    3U
#define FLASH_CMD_CTRL_READ_ENABLE_POS    23U
#define FLASH_CMD_CTRL_OPCODE_POS         24U

/** Default register values of the QSPI Flash controller */
#define QSPI_CFG_REG_RESET_VALUE              (0x80080080U)
#define DEVICE_READ_INSTR_REG_RESET_VALUE     (0x080220EBU)
#define DEVICE_WRITE_INSTR_REG_RESET_VALUE    (0x00000002U)
#define DEVICE_SIZE_CFG_REG_RESET_VALUE       (0x00101002U)
#define REMAP_ADDR_REG_RESET_VALUE            (0x00000000U)
#define FLASH_CMD_CONTROL_REG_RESET_VALUE     (0x00000000U)
#define FLASH_CMD_ADDRESS_REG_RESET_VALUE     (0x00000000U)
#define FLASH_CMD_WRITE_DATA_REG_RESET_VALUE  (0x00000000U)

/**
 * \brief Change specific bits in a 32 bits word.
 *
 * \param[in,out] word         Pointer of the word to change
 * \param[in]     bits         bits_length bits to put at bits_pos in the word
 *                             pointed
 * \param[in]     bits_length  Number of bits to change
 * \param[in]     bits_pos     Position of the bits to change
 *
 * \note This function will do nothing if the parameters given are incorret:
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
 * \brief Configure reads or writes commands for direct operations.
 *
 * \param[in] dev             QSPI IP6514E device struct \ref qspi_ip6514e_dev_t
 * \param[in] opcode          Read/write opcode that will be used for every
 *                            direct read/write
 * \param[in] dummy_cycles    Number of dummy cycles to wait before triggering
 *                            the command, this value must be between 0 and 31
 *                            (both included)
 * \param[in] is_reads_cfg    true to configure direct reads, false to configure
 *                            direct writes
 *
 * \return Returns error code as specified in \ref qspi_ip6514e_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 */
static enum qspi_ip6514e_error_t qspi_ip6514e_cfg_reads_writes(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint8_t opcode,
                                                 uint32_t dummy_cycles,
                                                 bool is_reads_cfg)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;
    /*
     * Select the good register address if we want to configure reads or writes.
     */
    volatile uint32_t *device_read_write_inst_reg = is_reads_cfg ?
                                                  &(reg_map->device_read_inst) :
                                                  &(reg_map->device_write_inst);
    uint32_t device_read_write_inst_reg_copy = *device_read_write_inst_reg;

    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_ip6514e_is_idle(dev));

    if (dummy_cycles > DEVICE_READ_WRITE_INST_DUMMY_CYCLES_MAX) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    change_bits_in_word(&device_read_write_inst_reg_copy,
                        (uint32_t)opcode,
                        BITS_PER_BYTE,
                        DEVICE_READ_WRITE_INST_OPCODE_POS);
    change_bits_in_word(&device_read_write_inst_reg_copy,
                        dummy_cycles,
                        DEVICE_READ_WRITE_INST_DUMMY_CYCLES_BITS,
                        DEVICE_READ_WRITE_INST_DUMMY_CYCLES_POS);

    *device_read_write_inst_reg = device_read_write_inst_reg_copy;

    return QSPI_IP6514E_ERR_NONE;
}

/**
 * \brief Given the public SPI mode enumeration, returns the private value it
 *        maps to in the register field.
 *
 * \param[in] spi_mode Read/write opcode that will be used for every direct
 *            read/write
 *
 * \return Return the correct DEVICE_READ_WRITE_INST_MODE value.
 */
static uint32_t spi_mode_field_value(enum qspi_ip6514e_spi_mode_t spi_mode)
{
    switch (spi_mode) {
        case QSPI_IP6514E_SPI_MODE:
            return DEVICE_READ_WRITE_INST_MODE_SPI;
        case QSPI_IP6514E_DSPI_MODE:
            return DEVICE_READ_WRITE_INST_MODE_DSPI;
        case QSPI_IP6514E_QSPI_MODE:
            return DEVICE_READ_WRITE_INST_MODE_QSPI;
        default:
            return ERROR_VALUE;
    }
}

bool qspi_ip6514e_is_idle(struct qspi_ip6514e_dev_t* dev)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;

    return GET_BIT(reg_map->qspi_cfg, QSPI_CFG_IDLE_POS);
}

bool qspi_ip6514e_is_enabled(struct qspi_ip6514e_dev_t* dev)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;

    return GET_BIT(reg_map->qspi_cfg, QSPI_CFG_ENABLE_POS);
}

void qspi_ip6514e_disable(struct qspi_ip6514e_dev_t* dev)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;

    CLR_BIT(reg_map->qspi_cfg, QSPI_CFG_ENABLE_POS);
}

void qspi_ip6514e_enable(struct qspi_ip6514e_dev_t* dev)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;

    SET_BIT(reg_map->qspi_cfg, QSPI_CFG_ENABLE_POS);
}

enum qspi_ip6514e_error_t qspi_ip6514e_set_baud_rate_div(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint32_t div)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;

    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_ip6514e_is_idle(dev));

    /* div should be an even number. */
    if (((div & 1U) == 1) ||
        (div < QSPI_CFG_BAUD_DIV_MIN) ||
        (div > QSPI_CFG_BAUD_DIV_MAX)) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    /*
     * The div value (between 2 and 32) needs to be stored in the register on a
     * 4 bits field.
     */
    change_bits_in_word(&(reg_map->qspi_cfg),
                        (div / 2) - 1,
                        QSPI_CFG_BAUD_DIV_BITS,
                        QSPI_CFG_BAUD_DIV_POS);

    return QSPI_IP6514E_ERR_NONE;
}

enum qspi_ip6514e_error_t qspi_ip6514e_set_spi_mode(
                                         struct qspi_ip6514e_dev_t* dev,
                                         enum qspi_ip6514e_spi_mode_t inst_type,
                                         enum qspi_ip6514e_spi_mode_t addr_type,
                                         enum qspi_ip6514e_spi_mode_t data_type)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;
    uint32_t inst_spi_mode, addr_spi_mode, data_spi_mode;
    /*
     * A local copy of the Device Read Instruction and Device Write Instruction
     * registers is used to limit APB accesses.
     */
    uint32_t device_read_inst_cpy = reg_map->device_read_inst;
    uint32_t device_write_inst_cpy = reg_map->device_write_inst;

    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_ip6514e_is_idle(dev));

    /*
     * First check that the instruction mode is not SPI. If that is the case,
     * the address and data mode register fields become DO NOT CARE.
     */
    inst_spi_mode = spi_mode_field_value(inst_type);
    if (inst_spi_mode == ERROR_VALUE) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }
    if (inst_type != QSPI_IP6514E_SPI_MODE) {
        change_bits_in_word(&(reg_map->device_read_inst),
                            inst_spi_mode,
                            DEVICE_READ_WRITE_INST_MODE_BITS,
                            DEVICE_READ_INST_INST_TYPE_POS);
        return QSPI_IP6514E_ERR_NONE;
    }

    /* Now check and set address and data modes. */
    addr_spi_mode = spi_mode_field_value(addr_type);
    data_spi_mode = spi_mode_field_value(data_type);
    if ((addr_spi_mode == ERROR_VALUE) || (data_spi_mode == ERROR_VALUE)) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    /* Change the Device Read Instruction register. */
    change_bits_in_word(&device_read_inst_cpy,
                        inst_spi_mode,
                        DEVICE_READ_WRITE_INST_MODE_BITS,
                        DEVICE_READ_INST_INST_TYPE_POS);
    change_bits_in_word(&device_read_inst_cpy,
                        addr_spi_mode,
                        DEVICE_READ_WRITE_INST_MODE_BITS,
                        DEVICE_READ_WRITE_INST_ADDR_TYPE_POS);
    change_bits_in_word(&device_read_inst_cpy,
                        data_spi_mode,
                        DEVICE_READ_WRITE_INST_MODE_BITS,
                        DEVICE_READ_WRITE_INST_DATA_TYPE_POS);

    /* Change the Device Write Instruction register. */
    change_bits_in_word(&device_write_inst_cpy,
                        addr_spi_mode,
                        DEVICE_READ_WRITE_INST_MODE_BITS,
                        DEVICE_READ_WRITE_INST_ADDR_TYPE_POS);
    change_bits_in_word(&device_write_inst_cpy,
                        data_spi_mode,
                        DEVICE_READ_WRITE_INST_MODE_BITS,
                        DEVICE_READ_WRITE_INST_DATA_TYPE_POS);

    /* Save the changes. */
    reg_map->device_read_inst = device_read_inst_cpy;
    reg_map->device_write_inst = device_write_inst_cpy;

    return QSPI_IP6514E_ERR_NONE;
}

enum qspi_ip6514e_error_t qspi_ip6514e_cfg_reads(struct qspi_ip6514e_dev_t* dev,
                                                 uint8_t opcode,
                                                 uint32_t dummy_cycles)
{
    return qspi_ip6514e_cfg_reads_writes(dev, opcode, dummy_cycles, CFG_READS);
}

enum qspi_ip6514e_error_t qspi_ip6514e_cfg_writes(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint8_t opcode,
                                                 uint32_t dummy_cycles)
{
    return qspi_ip6514e_cfg_reads_writes(dev, opcode, dummy_cycles, CFG_WRITES);
}

enum qspi_ip6514e_error_t qspi_ip6514e_cfg_page_size(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint32_t page_size)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;

    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_ip6514e_is_idle(dev));

    if (page_size > DEVICE_SIZE_PAGE_BYTES_MAX) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    change_bits_in_word(&(reg_map->device_size),
                        page_size,
                        DEVICE_SIZE_PAGE_BYTES_BITS,
                        DEVICE_SIZE_PAGE_BYTES_POS);

    return QSPI_IP6514E_ERR_NONE;
}

enum qspi_ip6514e_error_t qspi_ip6514e_cfg_addr_bytes(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint32_t bytes_number)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;

    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_ip6514e_is_idle(dev));

    if (bytes_number < DEVICE_SIZE_ADDR_BYTES_MIN ||
        bytes_number > DEVICE_SIZE_ADDR_BYTES_MAX) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    change_bits_in_word(&(reg_map->device_size),
                        bytes_number - 1,
                        DEVICE_SIZE_ADDR_BYTES_BITS,
                        DEVICE_SIZE_ADDR_BYTES_POS);


    return QSPI_IP6514E_ERR_NONE;
}

void qspi_ip6514e_remap_addr(struct qspi_ip6514e_dev_t* dev, uint32_t offset)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;
    /* Save the enable state to restore it after. */
    bool is_enabled = qspi_ip6514e_is_enabled(dev);

    if (is_enabled) {
        qspi_ip6514e_disable(dev);
    }

    reg_map->remap_addr = offset;
    SET_BIT(reg_map->qspi_cfg, QSPI_CFG_ENABLE_ADDR_REMAP_POS);

    if (is_enabled) {
        qspi_ip6514e_enable(dev);
    }
}

void qspi_ip6514e_disable_remap(struct qspi_ip6514e_dev_t* dev)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;
    /* Save the enable state to restore it after. */
    bool is_enabled = qspi_ip6514e_is_enabled(dev);

    if (is_enabled) {
        qspi_ip6514e_disable(dev);
    }

    CLR_BIT(reg_map->qspi_cfg, QSPI_CFG_ENABLE_ADDR_REMAP_POS);

    if (is_enabled) {
        qspi_ip6514e_enable(dev);
    }
}

void qspi_ip6514e_reset_regs(struct qspi_ip6514e_dev_t* dev)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;

    /* Restore the default value of the QSPI Configuration register. */
    reg_map->qspi_cfg = QSPI_CFG_REG_RESET_VALUE;

    /* Restore the default value of the Device R/W Instruction registers. */
    reg_map->device_read_inst = DEVICE_READ_INSTR_REG_RESET_VALUE;
    reg_map->device_write_inst = DEVICE_WRITE_INSTR_REG_RESET_VALUE;

    /* Restore the default value of the Device Size Configuration register. */
    reg_map->device_size = DEVICE_SIZE_CFG_REG_RESET_VALUE;

    /* Restore the default value of the Remap Address register. */
    reg_map->remap_addr = REMAP_ADDR_REG_RESET_VALUE;

    /* Restore the default value of the Flash Command Control register. */
    reg_map->flash_cmd_ctrl = FLASH_CMD_CONTROL_REG_RESET_VALUE;
    /* Restore the default value of the Flash Command Address register. */
    reg_map->flash_cmd_addr = FLASH_CMD_ADDRESS_REG_RESET_VALUE;

    /* Restore the default value of the Flash Command Write Data registers. */
    reg_map->flash_cmd_write_data_lower = FLASH_CMD_WRITE_DATA_REG_RESET_VALUE;
    reg_map->flash_cmd_write_data_upper = FLASH_CMD_WRITE_DATA_REG_RESET_VALUE;

    /*
     * This function does not affect the Flash Command Read Data registers
     * which are completely Read-Only.
     */
}

enum qspi_ip6514e_error_t qspi_ip6514e_send_cmd(struct qspi_ip6514e_dev_t* dev,
                                                uint8_t opcode,
                                                void *read_data,
                                                uint32_t read_len,
                                                const void *write_data,
                                                uint32_t write_len,
                                                uint32_t addr,
                                                uint32_t addr_bytes_number,
                                                uint32_t dummy_cycles)
{
    struct _qspi_ip6514e_reg_map_t *reg_map =
                               (struct _qspi_ip6514e_reg_map_t *)dev->cfg->base;
    /* To limit APB accesses, we set this reg up locally before */
    uint32_t flash_cmd_ctrl = 0U;
    bool read_requested = ((read_data != NULL) && (read_len != 0));
    bool write_requested = ((write_data != NULL) && (write_len != 0));
    bool addr_requested = (addr_bytes_number != 0);
    /*
     * To prevent unaligned and byte or halfbyte accesses to the APB registers,
     * a word aligned buffer is used to temporary transfer the data before doing
     * word accesses on these registers from that buffer.
     */
    uint32_t data_regs[DATA_REG_NUMBER] = {0};

    if (read_len > FLASH_CMD_CTRL_READ_BYTES_MAX) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    if (write_len > FLASH_CMD_CTRL_WRITE_BYTES_MAX) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    if (addr_bytes_number > FLASH_CMD_CTRL_ADDR_BYTES_MAX) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    if (dummy_cycles > FLASH_CMD_CTRL_DUMMY_CYCLES_MAX) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    if (read_requested && write_requested) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    change_bits_in_word(&flash_cmd_ctrl,
                        (uint32_t)opcode,
                        BITS_PER_BYTE,
                        FLASH_CMD_CTRL_OPCODE_POS);

    /* Enable read if requested */
    if (read_requested) {
        SET_BIT(flash_cmd_ctrl, FLASH_CMD_CTRL_READ_ENABLE_POS);
        change_bits_in_word(&flash_cmd_ctrl,
                            read_len - 1,
                            FLASH_CMD_CTRL_READ_BYTES_BITS,
                            FLASH_CMD_CTRL_READ_BYTES_POS);
    }

    /* Enable write if requested */
    if (write_requested) {
        SET_BIT(flash_cmd_ctrl, FLASH_CMD_CTRL_WRITE_ENABLE_POS);
        change_bits_in_word(&flash_cmd_ctrl,
                            write_len - 1,
                            FLASH_CMD_CTRL_WRITE_BYTES_BITS,
                            FLASH_CMD_CTRL_WRITE_BYTES_POS);

        if (IS_ADDR_ALIGNED(write_data) && IS_ADDR_ALIGNED(write_len)) {
            /*
             * Optimised case when write_data is word aligned and write_len is
             * 4 or 8.
             */
            reg_map->flash_cmd_write_data_lower = *(uint32_t *)write_data;
            if (write_len == FLASH_CMD_CTRL_WRITE_BYTES_MAX) {
                reg_map->flash_cmd_write_data_upper =
                                                  *((uint32_t *)write_data + 1);
            }
        } else {
            /*
             * data_regs is used as a buffer to only do unaligned access on the
             * AHB bus and word aligned accesses to the APB registers.
             */
            memcpy((void *)data_regs, write_data, write_len);
            /*
             * Only write_len bytes will be written even if both data registers
             * are written.
             */
            reg_map->flash_cmd_write_data_lower = data_regs[DATA_REG_LOWER];
            reg_map->flash_cmd_write_data_upper = data_regs[DATA_REG_UPPER];
        }
    }

    /* Enable the address if requested */
    if (addr_requested) {
        SET_BIT(flash_cmd_ctrl, FLASH_CMD_CTRL_ADDR_ENABLE_POS);
        reg_map->flash_cmd_addr = addr;
        change_bits_in_word(&flash_cmd_ctrl,
                            addr_bytes_number - 1,
                            FLASH_CMD_CTRL_ADDR_BYTES_BITS,
                            FLASH_CMD_CTRL_ADDR_BYTES_POS);
    }

    /* Put dummy cycles number */
    change_bits_in_word(&flash_cmd_ctrl,
                        dummy_cycles,
                        FLASH_CMD_CTRL_DUMMY_CYCLES_BITS,
                        FLASH_CMD_CTRL_DUMMY_CYCLES_POS);

    /* Copy the Flash Command Control register and execute the command */
    reg_map->flash_cmd_ctrl = flash_cmd_ctrl;
    SET_BIT(reg_map->flash_cmd_ctrl, FLASH_CMD_CTRL_EXECUTE_POS);

    /* Wait for termination */
    while (GET_BIT(reg_map->flash_cmd_ctrl, FLASH_CMD_CTRL_BUSY_POS));

    /*
     * Recolt the read data if it was requested. read_len validity has already
     * been verified at this point.
     */
    if (read_requested) {
        if (IS_ADDR_ALIGNED(read_data) && IS_ADDR_ALIGNED(read_len)) {
            /*
             * Optimised case when read_data is word aligned and read_len is
             * 4 or 8.
             */
            *(uint32_t *)read_data = reg_map->flash_cmd_read_data_lower;
            if (read_len == FLASH_CMD_CTRL_READ_BYTES_MAX) {
                *((uint32_t *)read_data + 1) =
                                             reg_map->flash_cmd_read_data_upper;
            }
        } else {
            /*
             * Only read_len bytes have been written even if both data registers
             * are written.
             */
            data_regs[DATA_REG_LOWER] = reg_map->flash_cmd_read_data_lower;
            data_regs[DATA_REG_UPPER] = reg_map->flash_cmd_read_data_upper;
            /*
             * data_regs is used as a buffer to only do unaligned access on the
             * AHB bus and word aligned accesses to the APB registers.
             */
            memcpy(read_data, (void *)data_regs, read_len);
        }
    }

    return QSPI_IP6514E_ERR_NONE;
}

void qspi_ip6514e_send_simple_cmd(struct qspi_ip6514e_dev_t* dev,
                                      uint8_t opcode)
{
    /*
     * No read/write data, no address, no dummy cycles.
     * Given the arguments, this function can not fail.
     */
    (void)qspi_ip6514e_send_cmd(dev,
                                opcode,
                                ARG_PTR_NOT_USED,
                                ARG_NOT_USED,
                                ARG_PTR_NOT_USED,
                                ARG_NOT_USED,
                                ARG_NOT_USED,
                                ARG_NOT_USED,
                                0);
}

enum qspi_ip6514e_error_t qspi_ip6514e_send_read_cmd(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint8_t opcode,
                                                 void *read_data,
                                                 uint32_t read_len,
                                                 uint32_t addr,
                                                 uint32_t addr_bytes_number,
                                                 uint32_t dummy_cycles)
{
    /* Read arguments are expected */
    if (read_data == ARG_PTR_NOT_USED || read_len == ARG_NOT_USED) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    /* No write data */
    return qspi_ip6514e_send_cmd(dev,
                                 opcode,
                                 read_data,
                                 read_len,
                                 ARG_PTR_NOT_USED,
                                 ARG_NOT_USED,
                                 addr,
                                 addr_bytes_number,
                                 dummy_cycles);
}

enum qspi_ip6514e_error_t qspi_ip6514e_send_write_cmd(
                                                 struct qspi_ip6514e_dev_t* dev,
                                                 uint8_t opcode,
                                                 const void *write_data,
                                                 uint32_t write_len,
                                                 uint32_t addr,
                                                 uint32_t addr_bytes_number,
                                                 uint32_t dummy_cycles)
{
    /* Write arguments are expected */
    if (write_data == ARG_PTR_NOT_USED || write_len == ARG_NOT_USED) {
        return QSPI_IP6514E_ERR_WRONG_ARGUMENT;
    }

    /* No read data, no dummy cycles */
    return qspi_ip6514e_send_cmd(dev,
                                 opcode,
                                 ARG_PTR_NOT_USED,
                                 ARG_NOT_USED,
                                 write_data,
                                 write_len,
                                 addr,
                                 addr_bytes_number,
                                 dummy_cycles);
}
