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
#ifndef MBED_QSPI_FLASH_S25FS512S_H
#define MBED_QSPI_FLASH_S25FS512S_H

#define QSPI_FLASH_CHIP_STRING "Cypress S25FS512S"
#define QSPI_FLASH_CYPRESS_S25FS512S

// Command for reading configuration register
#define QSPI_CMD_RDCR0                         0x35  // To read Quad (QE) enable bit
// Command for writing status/configuration register
#define QSPI_CMD_WRSR                          0x01 // To write Qual (QE) enable bit
// Command for reading status register
#define QSPI_CMD_RDSR                          0x05 // To read WIP bit of status register 1



// Command for reading security register
#define QSPI_CMD_RDSCUR                         0x2B

// Command for setting Reset Enable
#define QSPI_CMD_RSTEN                          0x66
// Command for setting Reset
#define QSPI_CMD_RST                            0xF0 //0x99

// Command for setting write enable
#define QSPI_CMD_WREN                           0x06
// Command for setting write disable
#define QSPI_CMD_WRDI                           0x04

// WRSR operations max time [us] (datasheet max time + 15%)
#define QSPI_WRSR_MAX_TIME                      2300000   // 2.3 seconds
// general wait max time [us]
#define QSPI_WAIT_MAX_TIME                      100000  // 100ms


// Commands for writing (page programming)
#define QSPI_CMD_WRITE_1IO                      0x02    // 1-1-1 mode
#define QSPI_CMD_WRITE_1I4O                     0x32    // 1-1-4 mode // 1-4-4 is not supported by S25FS512S

// write operations max time [us] (datasheet max time + 15%)
#define QSPI_PAGE_PROG_MAX_TIME                 2300   // 2.3ms

#define QSPI_PAGE_SIZE                          256     // 256B
#define QSPI_SECTOR_SIZE                        262144  // 256kB
#define QSPI_SECTOR_COUNT                       256     // 64 MB

// Commands for reading
#define QSPI_CMD_READ_1IO_FAST                  0x0B   // 1-1-1 mode
#define QSPI_CMD_READ_1IO                       0x03   // 1-1-1 mode
#define QSPI_CMD_READ_2IO                       0xBB   // 1-2-2 mode    - dual I/O
#define QSPI_CMD_READ_1I2O                      0x3B   // 1-1-2 mode    - dual output
#define QSPI_CMD_READ_4IO                       0xEB   // 1-4-4 mode    - quad I/O
#define QSPI_CMD_READ_1I4O                      0x6B   // 1-1-4 mode    - quad output

// Alt (mode) value for quad I/O read
#define QSPI_ALT_READ_4IO                       0x01 // 1-4-4 mode only

#define QSPI_READ_1IO_DUMMY_CYCLE               0
#define QSPI_READ_FAST_DUMMY_CYCLE              8
#define QSPI_READ_2IO_DUMMY_CYCLE               4 // dual I/O
#define QSPI_READ_1I2O_DUMMY_CYCLE              8 // dual output
#define QSPI_READ_4IO_DUMMY_CYCLE               6 // quad I/O - 2 cycles for Mode or Alt (4 bits per cycle x 2 cycles = 1 byte) + 4 dummy cycles
#define QSPI_READ_1I4O_DUMMY_CYCLE              8 // quad output

// Commands for erasing
#define QSPI_CMD_ERASE_SECTOR                   0xD8    // 256kB
#define QSPI_CMD_ERASE_CHIP                     0x60    // or 0xC7

// erase operations max time [us] (datasheet max time + 15%)
#define QSPI_ERASE_SECTOR_MAX_TIME              850000      // 1.15*725 ~ 850 ms

// max frequency for basic rw operation (for fast mode)
#define QSPI_COMMON_MAX_FREQUENCY               50000000

#define QSPI_STATUS_REG_SIZE                    1
#define QSPI_CONFIG_REG_0_SIZE                  1
#define QSPI_SECURITY_REG_SIZE                  1
#define QSPI_MAX_REG_SIZE                       2

// status register
#define STATUS_BIT_WIP   (1 << 0)   // write in progress bit
#define STATUS_BIT_WEL   (1 << 1)   // write enable latch
#define STATUS_BIT_BP0   (1 << 2)   //
#define STATUS_BIT_BP1   (1 << 3)   //
#define STATUS_BIT_BP2   (1 << 4)   //
#define STATUS_BIT_BP3   (1 << 5)   //
#define STATUS_BIT_QE    (1 << 1)   // Quad Enable
#define STATUS_BIT_SRWD  (1 << 7)   // status register write protect

// configuration register 0
// bit 0, 1, 2, 4, 5, 7 reserved
#define CONFIG0_BIT_TB   (1 << 3)   // Top/Bottom area protect
#define CONFIG0_BIT_DC   (1 << 6)   // Dummy Cycle

// configuration register 1
// bit 0, 2, 3, 4, 5, 6, 7 reserved
#define CONFIG1_BIT_LH   (1 << 1)   // 0 = Ultra Low power mode, 1 = High performance mode



#define EXTENDED_SPI_ENABLE()                                               \
                                                                            \
    const int32_t reg_size = QSPI_STATUS_REG_SIZE + QSPI_CONFIG_REG_0_SIZE; \
    uint8_t reg_data[reg_size] = { 0 };                                     \
                                                                            \
    if (read_register(STATUS_REG, reg_data,                                 \
            QSPI_STATUS_REG_SIZE, qspi) != QSPI_STATUS_OK) {                \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
    if (read_register(CONFIG_REG0, reg_data + QSPI_STATUS_REG_SIZE,         \
            QSPI_CONFIG_REG_0_SIZE, qspi) != QSPI_STATUS_OK) {              \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    if (write_enable(qspi) != QSPI_STATUS_OK) {                             \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    reg_data[1] |= STATUS_BIT_QE;                                            \
    if (write_register(QSPI_CMD_WRSR, reg_data,                             \
            reg_size, qspi) != QSPI_STATUS_OK) {                            \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    WAIT_FOR(WRSR_MAX_TIME, qspi);                                          \
                                                                            \
    memset(reg_data, 0, QSPI_STATUS_REG_SIZE);                              \
    if (read_register(CONFIG_REG0, reg_data,                                 \
            QSPI_STATUS_REG_SIZE, qspi) != QSPI_STATUS_OK) {                \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    return ((reg_data[0] & STATUS_BIT_QE) != 0 ?                            \
            QSPI_STATUS_OK : QSPI_STATUS_ERROR)



#define EXTENDED_SPI_DISABLE()                                              \
                                                                            \
    const int32_t reg_size = QSPI_STATUS_REG_SIZE + QSPI_CONFIG_REG_0_SIZE; \
    uint8_t reg_data[reg_size] = { 0 };                                     \
                                                                            \
    if (read_register(STATUS_REG, reg_data,                                 \
            QSPI_STATUS_REG_SIZE, qspi) != QSPI_STATUS_OK) {                \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    if (read_register(CONFIG_REG0, reg_data + QSPI_STATUS_REG_SIZE,         \
            QSPI_CONFIG_REG_0_SIZE, qspi) != QSPI_STATUS_OK) {              \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    if (write_enable(qspi) != QSPI_STATUS_OK) {                             \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    reg_data[1] &= ~(STATUS_BIT_QE);                                        \
                                                                            \
    if (write_register(QSPI_CMD_WRSR, reg_data,                             \
            reg_size, qspi) != QSPI_STATUS_OK) {                            \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    WAIT_FOR(WRSR_MAX_TIME, qspi);                                          \
                                                                            \
    reg_data[0] = 0;                                                        \
    if (read_register(CONFIG_REG0, reg_data,                                \
        QSPI_STATUS_REG_SIZE, qspi) != QSPI_STATUS_OK) {                    \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    return ((reg_data[0] & STATUS_BIT_QE) == 0 ?                            \
            QSPI_STATUS_OK : QSPI_STATUS_ERROR)


#endif // MBED_QSPI_FLASH_S25FS512S_H
