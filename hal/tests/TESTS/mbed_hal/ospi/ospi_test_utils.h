/* mbed Microcontroller Library
 * Copyright (c) 2018-2020 ARM Limited
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
#ifndef MBED_OSPI_TEST_UTILS_H
#define MBED_OSPI_TEST_UTILS_H

#include "flash_configs/flash_configs.h"
#include "unity/unity.h"
#include "hal/ospi_api.h"
#define OSPI_NONE (-1)

enum OspiStatus {
    sOK,
    sError,
    sTimeout,
    sUnknown
};

class OspiCommand {
public:
    void configure(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width,
                   ospi_bus_width_t alt_width, ospi_address_size_t addr_size, ospi_alt_size_t alt_size,
                   int dummy_cycles = 0);

    void set_dummy_cycles(int dummy_cycles);

    void build(int instruction, int address = OSPI_NONE, int alt = OSPI_NONE);

    ospi_command_t *get();

private:
    ospi_command_t _cmd;
};

struct Ospi {
    ospi_t handle;
    OspiCommand cmd;
};

// MODE_Command_Address_Data_Alt
#define MODE_1_1_1      OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_SINGLE
#define MODE_1_1_2      OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_DUAL,   OSPI_CFG_BUS_DUAL
#define MODE_1_2_2      OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_DUAL,   OSPI_CFG_BUS_DUAL,   OSPI_CFG_BUS_DUAL
#define MODE_2_2_2      OSPI_CFG_BUS_DUAL,   OSPI_CFG_BUS_DUAL,   OSPI_CFG_BUS_DUAL,   OSPI_CFG_BUS_DUAL
#define MODE_1_1_4      OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_QUAD,   OSPI_CFG_BUS_QUAD
#define MODE_1_4_4      OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_QUAD,   OSPI_CFG_BUS_QUAD,   OSPI_CFG_BUS_QUAD
#define MODE_4_4_4      OSPI_CFG_BUS_QUAD,   OSPI_CFG_BUS_QUAD,   OSPI_CFG_BUS_QUAD,   OSPI_CFG_BUS_QUAD
#define MODE_8_8_8      OSPI_CFG_BUS_OCTA,   OSPI_CFG_BUS_OCTA,   OSPI_CFG_BUS_OCTA,   OSPI_CFG_BUS_OCTA
#define MODE_8D_8D_8D   OSPI_CFG_BUS_OCTA_DTR, OSPI_CFG_BUS_OCTA_DTR, OSPI_CFG_BUS_OCTA_DTR, OSPI_CFG_BUS_OCTA_DTR


#define WRITE_1_1_1     MODE_1_1_1, OSPI_CMD_WRITE_1IO
#ifdef OSPI_CMD_WRITE_2IO
#define WRITE_1_2_2     MODE_1_2_2, OSPI_CMD_WRITE_2IO
#endif
#ifdef OSPI_CMD_WRITE_1I4O      // Quad page program - command: 0x32
#define WRITE_1_1_4     MODE_1_1_4, OSPI_CMD_WRITE_1I4O
#endif
#ifdef OSPI_CMD_WRITE_4IO
#define WRITE_1_4_4     MODE_1_4_4, OSPI_CMD_WRITE_4IO
#endif
#ifdef OSPI_CMD_WRITE_DPI
#define WRITE_2_2_2     MODE_2_2_2, OSPI_CMD_WRITE_DPI
#endif
#ifdef OSPI_CMD_WRITE_QPI
#define WRITE_4_4_4     MODE_4_4_4, OSPI_CMD_WRITE_QPI
#endif
#ifdef OSPI_CMD_WRITE_OPI
#define WRITE_8_8_8     MODE_8_8_8, OSPI_CMD_WRITE_OPI
#endif
#ifdef OSPI_CMD_WRITE_OPI
#define WRITE_8D_8D_8D  MODE_8D_8D_8D, OSPI_CMD_WRITE_OPI
#endif

#define READ_1_1_1      MODE_1_1_1, OSPI_CMD_READ_1IO,  OSPI_READ_1IO_DUMMY_CYCLE
#ifdef OSPI_CMD_READ_1I2O
#define READ_1_1_2      MODE_1_1_2, OSPI_CMD_READ_1I2O, OSPI_READ_1I2O_DUMMY_CYCLE
#endif
#ifdef OSPI_CMD_READ_2IO
#define READ_1_2_2      MODE_1_2_2, OSPI_CMD_READ_2IO,  OSPI_READ_2IO_DUMMY_CYCLE
#endif
#ifdef OSPI_CMD_READ_1I4O
#define READ_1_1_4      MODE_1_1_4, OSPI_CMD_READ_1I4O, OSPI_READ_1I4O_DUMMY_CYCLE
#endif
#ifdef OSPI_CMD_READ_4IO
#define READ_1_4_4      MODE_1_4_4, OSPI_CMD_READ_4IO,  OSPI_READ_4IO_DUMMY_CYCLE
#endif
#ifdef OSPI_CMD_READ_OPI
#define READ_8_8_8      MODE_8_8_8, OSPI_CMD_READ_OPI,  OSPI_READ_8IO_DUMMY_CYCLE
#endif
#ifdef OSPI_CMD_READ_DOPI
#define READ_8D_8D_8D   MODE_8D_8D_8D, OSPI_CMD_READ_DOPI,  OSPI_READ_8IO_DUMMY_CYCLE
#endif

#ifdef OSPI_CMD_READ_DPI
#define READ_2_2_2      MODE_2_2_2, OSPI_CMD_READ_DPI,  OSPI_READ_2IO_DUMMY_CYCLE
#endif
#ifdef OSPI_CMD_READ_QPI
#define READ_4_4_4      MODE_4_4_4, OSPI_CMD_READ_QPI,  OSPI_READ_4IO_DUMMY_CYCLE
#endif

#define ADDR_SIZE_8  OSPI_CFG_ADDR_SIZE_8
#define ADDR_SIZE_16 OSPI_CFG_ADDR_SIZE_16
#define ADDR_SIZE_24 OSPI_CFG_ADDR_SIZE_24
#define ADDR_SIZE_32 OSPI_CFG_ADDR_SIZE_32

#define ALT_SIZE_8  OSPI_CFG_ALT_SIZE_8
#define ALT_SIZE_16 OSPI_CFG_ALT_SIZE_16
#define ALT_SIZE_24 OSPI_CFG_ALT_SIZE_24
#define ALT_SIZE_32 OSPI_CFG_ALT_SIZE_32

#define STATUS_REG      OSPI_CMD_RDSR
#define CONFIG_REG0      OSPI_CMD_RDCR0
#ifdef OSPI_CMD_RDCR1
#define CONFIG_REG1      OSPI_CMD_RDCR1
#endif
#ifdef OSPI_CMD_RDCR2
#define CONFIG_REG2      OSPI_CMD_RDCR2
#endif
#define SECURITY_REG    OSPI_CMD_RDSCUR

#ifndef OSPI_CONFIG_REG_1_SIZE
#define OSPI_CONFIG_REG_1_SIZE 0
#endif

#ifndef OSPI_CONFIG_REG_2_SIZE
#define OSPI_CONFIG_REG_2_SIZE 0
#endif


#define SECTOR_ERASE  OSPI_CMD_ERASE_SECTOR
#define BLOCK_ERASE   OSPI_CMD_ERASE_BLOCK_64


#define SECTOR_ERASE_MAX_TIME   OSPI_ERASE_SECTOR_MAX_TIME
#define BLOCK32_ERASE_MAX_TIME  OSPI_ERASE_BLOCK_32_MAX_TIME
#define BLOCK64_ERASE_MAX_TIME  OSPI_ERASE_BLOCK_64_MAX_TIME
#define PAGE_PROG_MAX_TIME      OSPI_PAGE_PROG_MAX_TIME
#define WRSR_MAX_TIME           OSPI_WRSR_MAX_TIME
#define WAIT_MAX_TIME           OSPI_WAIT_MAX_TIME



ospi_status_t read_register(uint32_t cmd, uint8_t *buf, uint32_t size, Ospi &q);
ospi_status_t write_register(uint32_t cmd, uint8_t *buf, uint32_t size, Ospi &q);

OspiStatus flash_wait_for(uint32_t time_us, Ospi &ospi);

void flash_init(Ospi &ospi);

ospi_status_t write_enable(Ospi &ospi);
ospi_status_t write_disable(Ospi &ospi);

void log_register(uint32_t cmd, uint32_t reg_size, Ospi &ospi, const char *str = NULL);

ospi_status_t mode_enable(Ospi &ospi, ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width);
ospi_status_t mode_disable(Ospi &ospi, ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width);

ospi_status_t fast_mode_enable(Ospi &ospi);
ospi_status_t fast_mode_disable(Ospi &ospi);

ospi_status_t erase(uint32_t erase_cmd, uint32_t flash_addr, Ospi &ospi);

bool is_extended_mode(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width);
bool is_dual_mode(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width);
bool is_quad_mode(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width);
bool is_octa_mode(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width);
bool is_octa_dtr_mode(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width);

#define  WAIT_FOR(timeout, q)   TEST_ASSERT_EQUAL_MESSAGE(sOK, flash_wait_for(timeout, q), "flash_wait_for failed!!!")


#endif // MBED_OSPI_TEST_UTILS_H
