/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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
#ifndef MBED_QSPI_TEST_UTILS_H
#define MBED_QSPI_TEST_UTILS_H

#include "flash_configs/flash_configs.h"
#include "unity/unity.h"

#define QSPI_NONE (-1)

enum QspiStatus {
    sOK,
    sError,
    sTimeout,
    sUnknown
};

class QspiCommand {
public:
    void configure(qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width,
                   qspi_bus_width_t alt_width, qspi_address_size_t addr_size, qspi_alt_size_t alt_size,
                   int dummy_cycles = 0);

    void set_dummy_cycles(int dummy_cycles);

    void build(int instruction, int address = QSPI_NONE, int alt = QSPI_NONE);

    qspi_command_t *get();

private:
    qspi_command_t _cmd;
};

struct Qspi {
    qspi_t handle;
    QspiCommand cmd;
};

// MODE_Command_Address_Data_Alt
#define MODE_1_1_1      QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_SINGLE
#define MODE_1_1_2      QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_DUAL,   QSPI_CFG_BUS_DUAL
#define MODE_1_2_2      QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_DUAL,   QSPI_CFG_BUS_DUAL,   QSPI_CFG_BUS_DUAL
#define MODE_2_2_2      QSPI_CFG_BUS_DUAL,   QSPI_CFG_BUS_DUAL,   QSPI_CFG_BUS_DUAL,   QSPI_CFG_BUS_DUAL
#define MODE_1_1_4      QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_QUAD,   QSPI_CFG_BUS_QUAD
#define MODE_1_4_4      QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_QUAD,   QSPI_CFG_BUS_QUAD,   QSPI_CFG_BUS_QUAD
#define MODE_4_4_4      QSPI_CFG_BUS_QUAD,   QSPI_CFG_BUS_QUAD,   QSPI_CFG_BUS_QUAD,   QSPI_CFG_BUS_QUAD

#define WRITE_1_1_1     MODE_1_1_1, QSPI_CMD_WRITE_1IO
#ifdef QSPI_CMD_WRITE_2IO
#define WRITE_1_2_2     MODE_1_2_2, QSPI_CMD_WRITE_2IO
#endif
#ifdef QSPI_CMD_WRITE_1I4O      // Quad page program - command: 0x32
#define WRITE_1_1_4     MODE_1_1_4, QSPI_CMD_WRITE_1I4O
#endif
#ifdef QSPI_CMD_WRITE_4IO
#define WRITE_1_4_4     MODE_1_4_4, QSPI_CMD_WRITE_4IO
#endif
#ifdef QSPI_CMD_WRITE_DPI
#define WRITE_2_2_2     MODE_2_2_2, QSPI_CMD_WRITE_DPI
#endif
#ifdef QSPI_CMD_WRITE_QPI
#define WRITE_4_4_4     MODE_4_4_4, QSPI_CMD_WRITE_QPI
#endif


#define READ_1_1_1      MODE_1_1_1, QSPI_CMD_READ_1IO,  QSPI_READ_1IO_DUMMY_CYCLE
#ifdef QSPI_CMD_READ_1I2O
#define READ_1_1_2      MODE_1_1_2, QSPI_CMD_READ_1I2O, QSPI_READ_1I2O_DUMMY_CYCLE
#endif
#ifdef QSPI_CMD_READ_2IO
#define READ_1_2_2      MODE_1_2_2, QSPI_CMD_READ_2IO,  QSPI_READ_2IO_DUMMY_CYCLE
#endif
#ifdef QSPI_CMD_READ_1I4O
#define READ_1_1_4      MODE_1_1_4, QSPI_CMD_READ_1I4O, QSPI_READ_1I4O_DUMMY_CYCLE
#endif
#ifdef QSPI_CMD_READ_4IO
#define READ_1_4_4      MODE_1_4_4, QSPI_CMD_READ_4IO,  QSPI_READ_4IO_DUMMY_CYCLE
#endif

#ifdef QSPI_CMD_READ_DPI
#define READ_2_2_2      MODE_2_2_2, QSPI_CMD_READ_DPI,  QSPI_READ_2IO_DUMMY_CYCLE
#endif
#ifdef QSPI_CMD_READ_QPI
#define READ_4_4_4      MODE_4_4_4, QSPI_CMD_READ_QPI,  QSPI_READ_4IO_DUMMY_CYCLE
#endif

#define ADDR_SIZE_8  QSPI_CFG_ADDR_SIZE_8
#define ADDR_SIZE_16 QSPI_CFG_ADDR_SIZE_16
#define ADDR_SIZE_24 QSPI_CFG_ADDR_SIZE_24
#define ADDR_SIZE_32 QSPI_CFG_ADDR_SIZE_32

#define ALT_SIZE_8  QSPI_CFG_ALT_SIZE_8
#define ALT_SIZE_16 QSPI_CFG_ALT_SIZE_16
#define ALT_SIZE_24 QSPI_CFG_ALT_SIZE_24
#define ALT_SIZE_32 QSPI_CFG_ALT_SIZE_32

#define STATUS_REG      QSPI_CMD_RDSR
#define CONFIG_REG0      QSPI_CMD_RDCR0
#ifdef QSPI_CMD_RDCR1
#define CONFIG_REG1      QSPI_CMD_RDCR1
#endif
#ifdef QSPI_CMD_RDCR2
#define CONFIG_REG2      QSPI_CMD_RDCR2
#endif
#define SECURITY_REG    QSPI_CMD_RDSCUR

#ifndef QSPI_CONFIG_REG_1_SIZE
#define QSPI_CONFIG_REG_1_SIZE 0
#endif

#ifndef QSPI_CONFIG_REG_2_SIZE
#define QSPI_CONFIG_REG_2_SIZE 0
#endif


#define SECTOR_ERASE  QSPI_CMD_ERASE_SECTOR
#define BLOCK_ERASE   QSPI_CMD_ERASE_BLOCK_64


#define SECTOR_ERASE_MAX_TIME   QSPI_ERASE_SECTOR_MAX_TIME
#define BLOCK32_ERASE_MAX_TIME  QSPI_ERASE_BLOCK_32_MAX_TIME
#define BLOCK64_ERASE_MAX_TIME  QSPI_ERASE_BLOCK_64_MAX_TIME
#define PAGE_PROG_MAX_TIME      QSPI_PAGE_PROG_MAX_TIME
#define WRSR_MAX_TIME           QSPI_WRSR_MAX_TIME
#define WAIT_MAX_TIME           QSPI_WAIT_MAX_TIME



qspi_status_t read_register(uint32_t cmd, uint8_t *buf, uint32_t size, Qspi &q);
qspi_status_t write_register(uint32_t cmd, uint8_t *buf, uint32_t size, Qspi &q);

QspiStatus flash_wait_for(uint32_t time_us, Qspi &qspi);

void flash_init(Qspi &qspi);

qspi_status_t write_enable(Qspi &qspi);
qspi_status_t write_disable(Qspi &qspi);

void log_register(uint32_t cmd, uint32_t reg_size, Qspi &qspi, const char *str = NULL);

qspi_status_t mode_enable(Qspi &qspi, qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width);
qspi_status_t mode_disable(Qspi &qspi, qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width);

qspi_status_t fast_mode_enable(Qspi &qspi);
qspi_status_t fast_mode_disable(Qspi &qspi);

qspi_status_t erase(uint32_t erase_cmd, uint32_t flash_addr, Qspi &qspi);

bool is_extended_mode(qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width);
bool is_dual_mode(qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width);
bool is_quad_mode(qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width);

#define  WAIT_FOR(timeout, q)   TEST_ASSERT_EQUAL_MESSAGE(sOK, flash_wait_for(timeout, q), "flash_wait_for failed!!!")


#endif // MBED_QSPI_TEST_UTILS_H
