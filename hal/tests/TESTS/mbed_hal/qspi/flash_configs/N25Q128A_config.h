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
#ifndef MBED_QSPI_FLASH_N25Q128A_H
#define MBED_QSPI_FLASH_N25Q128A_H


#define QSPI_FLASH_CHIP_STRING "Micron N25Q128A"

// Command for reading status register
#define QSPI_CMD_RDSR                           0x05
// Command for reading configuration register 0 (NONVOLATILE CONFIGURATION REGISTER)
#define QSPI_CMD_RDCR0                          0xB5
// Command for reading configuration register 1 (VOLATILE CONFIGURATION REGISTER)
#define QSPI_CMD_RDCR1                          0x85
// Command for reading configuration register 2 (ENHANCED VOLATILE CONFIGURATION REGISTER)
#define QSPI_CMD_RDCR2                          0x65
// Command for writing status
#define QSPI_CMD_WRSR                           0x01
// Command for writing configuration register 0 (NONVOLATILE CONFIGURATION REGISTER)
#define QSPI_CMD_WRCR0                          0xB1
// Command for writing configuration register 1 (VOLATILE CONFIGURATION REGISTER)
#define QSPI_CMD_WRCR1                          0x81
// Command for writing configuration register 2 (ENHANCED VOLATILE CONFIGURATION REGISTER)
#define QSPI_CMD_WRCR2                          0x61
// Command for reading security register
#define QSPI_CMD_RDSCUR                         0x2B

// Command for setting Reset Enable
#define QSPI_CMD_RSTEN                          0x66
// Command for setting Reset
#define QSPI_CMD_RST                            0x99

// Command for setting write enable
#define QSPI_CMD_WREN                           0x06
// Command for setting write disable
#define QSPI_CMD_WRDI                           0x04

// WRSR operations max time [us] (datasheet max time + 15%)
#define QSPI_WRSR_MAX_TIME                      9200   // 8ms
// general wait max time [us]
#define QSPI_WAIT_MAX_TIME                      100000   // 100ms


// Commands for writing (page programming)
#define QSPI_CMD_WRITE_1IO                      0x02    // 1-1-1 mode
#define QSPI_CMD_WRITE_2IO                      0xD2    // 1-2-2 mode
#define QSPI_CMD_WRITE_4IO                      0x12    // 1-4-4 mode
#define QSPI_CMD_WRITE_DPI                      0xD2    // 2-2-2 mode
#define QSPI_CMD_WRITE_QPI                      0x12    // 4-4-4 mode

// write operations max time [us] (datasheet max time + 15%)
#define QSPI_PAGE_PROG_MAX_TIME                 5750   // 5ms

#define QSPI_PAGE_SIZE                          256    // 256B
#define QSPI_SECTOR_SIZE                        4096   // 4kB
#define QSPI_SECTOR_COUNT                       4096

// Commands for reading
#define QSPI_CMD_READ_1IO_FAST                  0x0B   // 1-1-1 mode
#define QSPI_CMD_READ_1IO                       0x03   // 1-1-1 mode
#define QSPI_CMD_READ_2IO                       0xBB   // 1-2-2 mode
#define QSPI_CMD_READ_DPI                       0xBB   // 2-2-2 mode
#define QSPI_CMD_READ_1I2O                      0x3B   // 1-1-2 mode
#define QSPI_CMD_READ_4IO                       0xEB   // 1-4-4 mode
#define QSPI_CMD_READ_QPI                       0xEB   // 4-4-4 mode
#define QSPI_CMD_READ_1I4O                      0x6B   // 1-1-4 mode


#define QSPI_READ_1IO_DUMMY_CYCLE               0
#define QSPI_READ_FAST_DUMMY_CYCLE              8
// 8 dummy (10 dummy when quad SPI protocol is enabled)
#define QSPI_READ_2IO_DUMMY_CYCLE               8
#define QSPI_READ_1I2O_DUMMY_CYCLE              8
#define QSPI_READ_4IO_DUMMY_CYCLE               10
#define QSPI_READ_1I4O_DUMMY_CYCLE              8

// Commands for erasing
#define QSPI_CMD_ERASE_SECTOR                   0x20    // 4kB
#define QSPI_CMD_ERASE_BLOCK_32                 0x52    // 32kB
#define QSPI_CMD_ERASE_BLOCK_64                 0xD8    // 64kB
#define QSPI_CMD_ERASE_CHIP                     0x60    // or 0xC7

// erase operations max time [us] (datasheet max time + 15%)
#define QSPI_ERASE_SECTOR_MAX_TIME              920000      // 0.8s
#define QSPI_ERASE_BLOCK_32_MAX_TIME            3000000     // 3s
#define QSPI_ERASE_BLOCK_64_MAX_TIME            3500000     // 3.5s

// max frequency for basic rw operation
#define QSPI_COMMON_MAX_FREQUENCY               50000000

#define QSPI_STATUS_REG_SIZE                    1
#define QSPI_CONFIG_REG_0_SIZE                  2
#define QSPI_CONFIG_REG_1_SIZE                  1
#define QSPI_CONFIG_REG_2_SIZE                  1
#define QSPI_MAX_REG_SIZE                       2

// status register
#define STATUS_BIT_WIP   (1 << 0)   // write in progress bit
#define STATUS_BIT_WEL   (1 << 1)   // write enable latch
#define STATUS_BIT_BP0   (1 << 2)   // block
#define STATUS_BIT_BP1   (1 << 3)   //
#define STATUS_BIT_BP2   (1 << 4)   //
#define STATUS_BIT_BP_TB (1 << 5)   // Block protect top/bottom
#define STATUS_BIT_BP3   (1 << 6)   //
#define STATUS_BIT_SRWD  (1 << 7)   // status register write protect

// configuration register 0 (Nonvolatile Configuration Register)
// bit 1, 5, reserved
#define CONFIG0_BIT_LOCK        (1 << 0)   // Lock nonvolatile configuration register
#define CONFIG0_BIT_DE          (1 << 2)   // Dual Enable   0 = Enabled / 1 = Disabled
#define CONFIG0_BIT_QE          (1 << 3)   // Quad Enable   0 = Enabled / 1 = Disabled
#define CONFIG0_BIT_RH          (1 << 4)   // Reset/hold
#define CONFIG0_BIT_ODS0        (1 << 6)   // Output driver strength
#define CONFIG0_BIT_ODS1        (1 << 7)   // Output driver strength
#define CONFIG0_BIT_ODS2        (1 << 8)   // Output driver strength
#define CONFIG0_BIT_XIP_MODE0   (1 << 9)   // XIP mode at power-on reset
#define CONFIG0_BIT_XIP_MODE1   (1 << 10)  // XIP mode at power-on reset
#define CONFIG0_BIT_XIP_MODE2   (1 << 11)  // XIP mode at power-on reset
#define CONFIG0_BIT_DCYCLE0     (1 << 12)  // Dummy Cycle
#define CONFIG0_BIT_DCYCLE1     (1 << 13)  // Dummy Cycle
#define CONFIG0_BIT_DCYCLE2     (1 << 14)  // Dummy Cycle
#define CONFIG0_BIT_DCYCLE3     (1 << 15)  // Dummy Cycle
#define CONFIG0_BITS_DEFAULT    0xFFFF  // reg default state


// configuration register 1 (Volatile Configuration Register)
// bit 2, reserved
#define CONFIG1_BIT_WRAP0   (1 << 0)   // Output data wrap
#define CONFIG1_BIT_WRAP1   (1 << 1)   // Output data wrap
#define CONFIG1_BIT_XIP     (1 << 3)   // 0 = Enable / 1 = Disable (default)
#define CONFIG1_BIT_DCYCLE0 (1 << 4)   // Number of dummy clock cycles
#define CONFIG1_BIT_DCYCLE1 (1 << 5)   // Number of dummy clock cycles
#define CONFIG1_BIT_DCYCLE2 (1 << 6)   // Number of dummy clock cycles
#define CONFIG1_BIT_DCYCLE3 (1 << 7)   // Number of dummy clock cycles
#define CONFIG1_BITS_DEFAULT    0xB  // reg default state


// configuration register 2 (Enhanced Volatile Configuration Register)
// bit 5, reserved
#define CONFIG2_BIT_ODS0   (1 << 0)   // Output driver strength     111 = 30 Ohms (Default)
#define CONFIG2_BIT_ODS1   (1 << 1)   // Output driver strength
#define CONFIG2_BIT_ODS2   (1 << 2)   // Output driver strength
#define CONFIG2_BIT_VPP    (1 << 3)   // VPP accelerator            1 = Disabled (Default)
#define CONFIG2_BIT_RH     (1 << 4)   // Reset/hold
#define CONFIG2_BIT_DE     (1 << 6)   // Dual I/O protocol          0 = Enabled / 1 = Disabled (Default, extended SPI protocol)
#define CONFIG2_BIT_QE     (1 << 7)   // Quad I/O protocol          0 = Enabled / 1 = Disabled (Default, extended SPI protocol)
#define CONFIG2_BITS_DEFAULT    0xDF  // reg default state


#define DUAL_ENABLE()                                                           \
                                                                                \
    uint8_t reg_data[QSPI_CONFIG_REG_2_SIZE];                                   \
                                                                                \
    memset(reg_data, 0, QSPI_CONFIG_REG_2_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR2, reg_data,                                 \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    if (write_enable(qspi) != QSPI_STATUS_OK) {                                 \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    reg_data[0] = reg_data[0] & ~(CONFIG2_BIT_DE);                              \
    if (write_register(QSPI_CMD_WRCR2, reg_data,                                \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    qspi.cmd.configure(MODE_2_2_2, ADDR_SIZE_24, ALT_SIZE_8);                   \
    WAIT_FOR(WRSR_MAX_TIME, qspi);                                              \
    memset(reg_data, 0, QSPI_CONFIG_REG_2_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR2, reg_data,                                 \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    return ((reg_data[0] & (CONFIG2_BIT_DE)) == 0 ?                             \
                QSPI_STATUS_OK : QSPI_STATUS_ERROR)


#define DUAL_DISABLE()                                                          \
                                                                                \
    uint8_t reg_data[QSPI_CONFIG_REG_2_SIZE];                                   \
                                                                                \
    memset(reg_data, 0, QSPI_CONFIG_REG_2_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR2, reg_data,                                 \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    if (write_enable(qspi) != QSPI_STATUS_OK) {                                 \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    reg_data[0] = reg_data[0] | (CONFIG2_BIT_DE);                               \
    if (write_register(QSPI_CMD_WRCR2, reg_data,                                \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    WAIT_FOR(WRSR_MAX_TIME, qspi);                                              \
    qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);                   \
    memset(reg_data, 0, QSPI_CONFIG_REG_2_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR2, reg_data,                                 \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    return ((reg_data[0] & CONFIG2_BIT_DE) != 1 ?                               \
                QSPI_STATUS_OK : QSPI_STATUS_ERROR)


#define QUAD_ENABLE()                                                           \
                                                                                \
    uint8_t reg_data[QSPI_CONFIG_REG_2_SIZE];                                   \
                                                                                \
    memset(reg_data, 0, QSPI_CONFIG_REG_2_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR2, reg_data,                                 \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    if (write_enable(qspi) != QSPI_STATUS_OK) {                                 \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    reg_data[0] = reg_data[0] & ~(CONFIG2_BIT_QE);                              \
    if (write_register(QSPI_CMD_WRCR2, reg_data,                                \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    qspi.cmd.configure(MODE_4_4_4, ADDR_SIZE_24, ALT_SIZE_8);                   \
    WAIT_FOR(WRSR_MAX_TIME, qspi);                                              \
    memset(reg_data, 0, QSPI_CONFIG_REG_2_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR2, reg_data,                                 \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    return ((reg_data[0] & (CONFIG2_BIT_QE)) == 0 ?                             \
                QSPI_STATUS_OK : QSPI_STATUS_ERROR)


#define QUAD_DISABLE()                                                          \
                                                                                \
    uint8_t reg_data[QSPI_CONFIG_REG_2_SIZE];                                   \
                                                                                \
    memset(reg_data, 0, QSPI_CONFIG_REG_2_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR2, reg_data,                                 \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    if (write_enable(qspi) != QSPI_STATUS_OK) {                                 \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    reg_data[0] = reg_data[0] | (CONFIG2_BIT_QE);                               \
    if (write_register(QSPI_CMD_WRCR2, reg_data,                                \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
    WAIT_FOR(WRSR_MAX_TIME, qspi);                                              \
    qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);                   \
    memset(reg_data, 0, QSPI_CONFIG_REG_2_SIZE);                                \
    if (read_register(QSPI_CMD_RDCR2, reg_data,                                 \
            QSPI_CONFIG_REG_2_SIZE, qspi) != QSPI_STATUS_OK) {                  \
        return QSPI_STATUS_ERROR;                                               \
    }                                                                           \
                                                                                \
    return ((reg_data[0] & CONFIG2_BIT_QE) != 1 ?                               \
                QSPI_STATUS_OK : QSPI_STATUS_ERROR)

#endif // MBED_QSPI_FLASH_N25Q128A_H
