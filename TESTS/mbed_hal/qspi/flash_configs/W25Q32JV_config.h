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
#ifndef MBED_QSPI_FLASH_W25Q32JV_H
#define MBED_QSPI_FLASH_W25Q32JV_H


#define QSPI_FLASH_CHIP_STRING "Winbond W25Q32JV"

// Command for reading status register
#define QSPI_CMD_RDSR                           0x05
// Command for reading configuration register
#define QSPI_CMD_RDCR0                          0x35
#define QSPI_CMD_RDCR1                          0x15
// Command for writing status/configuration register
#define QSPI_CMD_WRSR                           0x01
// Command for writing configuration register
#define QSPI_CMD_WRCR0                          0x31
#define QSPI_CMD_WRCR1                          0x11
// Command for reading security register
#define QSPI_CMD_RDSCUR                         0x48

// Command for setting Reset Enable
#define QSPI_CMD_RSTEN                          0x66
// Command for setting Reset
#define QSPI_CMD_RST                            0x99

// Command for setting write enable
#define QSPI_CMD_WREN                           0x06
// Command for setting write disable
#define QSPI_CMD_WRDI                           0x04

// WRSR operations max time [us] (datasheet max time + 15%)
#define QSPI_WRSR_MAX_TIME                      34500   // 30ms
// general wait max time [us]
#define QSPI_WAIT_MAX_TIME                      100000  // 100ms


// Commands for writing (page programming)
#define QSPI_CMD_WRITE_1IO                      0x02    // 1-1-1 mode

// write operations max time [us] (datasheet max time + 15%)
#define QSPI_PAGE_PROG_MAX_TIME                 11500   // 10ms

#define QSPI_PAGE_SIZE                          256     // 256B
#define QSPI_SECTOR_SIZE                        4096    // 4kB
#define QSPI_SECTOR_COUNT                       1024

// Commands for reading
#define QSPI_CMD_READ_1IO_FAST                  0x0B   // 1-1-1 mode
#define QSPI_CMD_READ_1IO                       0x03   // 1-1-1 mode
#define QSPI_CMD_READ_2IO                       0xBB   // 1-2-2 mode
#define QSPI_CMD_READ_1I2O                      0x3B   // 1-1-2 mode
#define QSPI_CMD_READ_4IO                       0xEB   // 1-4-4 mode
#define QSPI_CMD_READ_1I4O                      0x6B   // 1-1-4 mode

#define QSPI_READ_1IO_DUMMY_CYCLE               0
#define QSPI_READ_FAST_DUMMY_CYCLE              8
#define QSPI_READ_2IO_DUMMY_CYCLE               4
#define QSPI_READ_1I2O_DUMMY_CYCLE              8
#define QSPI_READ_4IO_DUMMY_CYCLE               6
#define QSPI_READ_1I4O_DUMMY_CYCLE              8

// Commands for erasing
#define QSPI_CMD_ERASE_SECTOR                   0x20    // 4kB
#define QSPI_CMD_ERASE_BLOCK_32                 0x52    // 32kB
#define QSPI_CMD_ERASE_BLOCK_64                 0xD8    // 64kB
#define QSPI_CMD_ERASE_CHIP                     0x60    // or 0xC7

// erase operations max time [us] (datasheet max time + 15%)
#define QSPI_ERASE_SECTOR_MAX_TIME              480000      // 400 ms
#define QSPI_ERASE_BLOCK_32_MAX_TIME            3450000     // 3s
#define QSPI_ERASE_BLOCK_64_MAX_TIME            4025000     // 3.5s

// max frequency for basic rw operation (for fast mode)
#define QSPI_COMMON_MAX_FREQUENCY               32000000

#define QSPI_STATUS_REG_SIZE                    1
#define QSPI_CONFIG_REG_0_SIZE                  1
#define QSPI_CONFIG_REG_1_SIZE                  1

#define QSPI_SECURITY_REG_SIZE                  1
#define QSPI_MAX_REG_SIZE                       1

// status register
#define STATUS_BIT_WIP   (1 << 0)   // write in progress bit
#define STATUS_BIT_WEL   (1 << 1)   // write enable latch
#define STATUS_BIT_BP0   (1 << 2)   // block protect 0
#define STATUS_BIT_BP1   (1 << 3)   // block protect 1
#define STATUS_BIT_BP2   (1 << 4)   // block protect 2
#define STATUS_BIT_BP_TB (1 << 5)   // block protect top/bottom
#define STATUS_BIT_SP    (1 << 6)   // sector protect
#define STATUS_BIT_SRWD  (1 << 7)   // status register protect

// configuration register 0
// bit 2 reserved
#define CONFIG0_BIT_SRL  (1 << 0)   // status register lock
#define CONFIG0_BIT_QE   (1 << 1)   // quad enable
#define CONFIG0_BIT_LB1  (1 << 3)   // security register lock 1
#define CONFIG0_BIT_LB2  (1 << 4)   // security register lock 2
#define CONFIG0_BIT_LB3  (1 << 5)   // security register lock 3
#define CONFIG0_BIT_CMP  (1 << 6)   // complement protect
#define CONFIG0_BIT_SUS  (1 << 7)   // suspend status

// configuration register 1
// bits 0, 1, 3, 4, 7 reserved
#define CONFIG1_BIT_WPS  (1 << 2)   // write protect selection
#define CONFIG1_BIT_DRV2 (1 << 5)   // output driver strength 2
#define CONFIG1_BIT_DRV1 (1 << 6)   // output driver strength 1



#define QUAD_ENABLE()                                                           \
                                                                                \
    uint8_t reg_data[QSPI_CONFIG_REG_0_SIZE];                                   \
                                                                                \
    memset(reg_data, 0, QSPI_CONFIG_REG_0_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR0, reg_data,                                 \
            QSPI_CONFIG_REG_0_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    if (write_enable(qspi) != QSPI_STATUS_OK) {                                 \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    reg_data[0] = reg_data[0] & ~(CONFIG0_BIT_QE);                              \
    if (write_register(QSPI_CMD_WRCR0, reg_data,                                \
            QSPI_CONFIG_REG_0_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    qspi.cmd.configure(MODE_4_4_4, ADDR_SIZE_24, ALT_SIZE_8);                   \
    WAIT_FOR(WRSR_MAX_TIME, qspi);                                              \
    memset(reg_data, 0, QSPI_CONFIG_REG_0_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR0, reg_data,                                 \
            QSPI_CONFIG_REG_0_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    return ((reg_data[0] & (CONFIG0_BIT_QE)) == 0 ?                             \
                QSPI_STATUS_OK : QSPI_STATUS_ERROR)


#define QUAD_DISABLE()                                                          \
                                                                                \
    uint8_t reg_data[QSPI_CONFIG_REG_0_SIZE];                                   \
                                                                                \
    memset(reg_data, 0, QSPI_CONFIG_REG_0_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR0, reg_data,                                 \
            QSPI_CONFIG_REG_0_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    if (write_enable(qspi) != QSPI_STATUS_OK) {                                 \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    reg_data[0] = reg_data[0] | (CONFIG0_BIT_QE);                               \
    if (write_register(QSPI_CMD_WRCR0, reg_data,                                \
            QSPI_CONFIG_REG_0_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    WAIT_FOR(WRSR_MAX_TIME, qspi);                                              \
    qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);                   \
    memset(reg_data, 0, QSPI_CONFIG_REG_0_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR0, reg_data,                                 \
            QSPI_CONFIG_REG_0_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    return ((reg_data[0] & CONFIG0_BIT_QE) != 1 ?                               \
                QSPI_STATUS_OK : QSPI_STATUS_ERROR)

#endif // MBED_QSPI_FLASH_W25Q32JV_H
