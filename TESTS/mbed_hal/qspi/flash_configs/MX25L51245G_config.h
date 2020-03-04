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
#ifndef MBED_QSPI_FLASH_MX25L51245G_H
#define MBED_QSPI_FLASH_MX25L51245G_H


#define QSPI_FLASH_CHIP_STRING "macronix MX25L51245G"

// Command for reading status register
#define QSPI_CMD_RDSR                           0x05
// Command for reading configuration register
#define QSPI_CMD_RDCR0                          0x15
// Command for writing status/configuration register
#define QSPI_CMD_WRSR                           0x01
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
#define QSPI_WRSR_MAX_TIME                      34500   // 30ms
// general wait max time [us]
#define QSPI_WAIT_MAX_TIME                      100000  // 100ms


// Commands for writing (page programming)
#define QSPI_CMD_WRITE_1IO                      0x02    // 1-1-1 mode
#define QSPI_CMD_WRITE_4IO                      0x38    // 1-4-4 mode

// write operations max time [us] (datasheet max time + 15%)
#define QSPI_PAGE_PROG_MAX_TIME                 11500   // 10ms

#define QSPI_PAGE_SIZE                          256     // 256B
#define QSPI_SECTOR_SIZE                        4096    // 4kB
#define QSPI_SECTOR_COUNT                       2048    // 

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
#define QSPI_ERASE_BLOCK_32_MAX_TIME            1200000     // 1s
#define QSPI_ERASE_BLOCK_64_MAX_TIME            2400000     // 2s

// max frequency for basic rw operation (for fast mode)
#define QSPI_COMMON_MAX_FREQUENCY               32000000

#define QSPI_STATUS_REG_SIZE                    1
#define QSPI_CONFIG_REG_0_SIZE                  2
#define QSPI_SECURITY_REG_SIZE                  1
#define QSPI_MAX_REG_SIZE                       2

// status register
#define STATUS_BIT_WIP   (1 << 0)   // write in progress bit
#define STATUS_BIT_WEL   (1 << 1)   // write enable latch
#define STATUS_BIT_BP0   (1 << 2)   //
#define STATUS_BIT_BP1   (1 << 3)   //
#define STATUS_BIT_BP2   (1 << 4)   //
#define STATUS_BIT_BP3   (1 << 5)   //
#define STATUS_BIT_QE    (1 << 6)   // Quad Enable
#define STATUS_BIT_SRWD  (1 << 7)   // status register write protect

// configuration register 0
// bit 0, 1, 2, 4, 5, 7 reserved
#define CONFIG0_BIT_TB   (1 << 3)   // Top/Bottom area protect

#endif // MBED_QSPI_FLASH_MX25L51245G_H
