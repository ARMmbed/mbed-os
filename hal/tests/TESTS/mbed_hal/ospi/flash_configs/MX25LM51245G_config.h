/* mbed Microcontroller Library
 * Copyright (c) 2018-2020 ARM Limited
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
#ifndef MBED_OSPI_FLASH_MX25LM51245G_H
#define MBED_OSPI_FLASH_MX25LM51245G_H


#define OSPI_FLASH_CHIP_STRING "macronix MX25LM51245G"

#define OSPI_CMD_RDID                           0x9F
// Command for reading status register
#define OSPI_CMD_RDSR                           0x05
// Command for reading configuration register
#define OSPI_CMD_RDCR0                          0x15
#define OSPI_CMD_RDCR2                          0x71
// Command for writing status/configuration register
#define OSPI_CMD_WRSR                           0x01

#define OSPI_CMD_WRCR2                          0x72

// Command for reading security register
#define OSPI_CMD_RDSCUR                         0x2B

// Command for setting Reset Enable
#define OSPI_CMD_RSTEN                          0x66
// Command for setting Reset
#define OSPI_CMD_RST                            0x99

// Command for setting write enable
#define OSPI_CMD_WREN                           0x06
// Command for setting write disable
#define OSPI_CMD_WRDI                           0x04

// WRSR operations max time [us] (datasheet max time + 15%)
#define OSPI_WRSR_MAX_TIME                      34500   // 30ms
// general wait max time [us]
#define OSPI_WAIT_MAX_TIME                      100000  // 100ms


// Commands for writing (page programming)
// Only single/octal mode supported with this memory
// So only single 1-1-1 mode in this OSPI config
//#define OSPI_CMD_WRITE_1IO                      0x02    // 1-1-1 mode
#define OSPI_CMD_WRITE_1IO                      0x12    // 4 byte addr 1-1-1 mode
#define OSPI_CMD_WRITE_OPI                      0x12    // 8-8-8 mode

// write operations max time [us] (datasheet max time + 15%)
#define OSPI_PAGE_PROG_MAX_TIME                 11500   // 10ms

#define OSPI_PAGE_SIZE                          256     // 256B
#define OSPI_SECTOR_SIZE                        4096    // 4kB
#define OSPI_SECTOR_COUNT                       2048

// Commands for reading
// Only single/octal mode supported with this memory
// So only single 1-1-1 mode in this OSPI config
#define OSPI_CMD_READ_1IO_FAST                  0x0B   // 1-1-1 mode
//#define OSPI_CMD_READ_1IO                       0x03   // 1-1-1 mode
#define OSPI_CMD_READ_1IO                       0x13   // 4 byte addr 1-1-1 mode
#define OSPI_CMD_READ_OPI                       0xEC   // 8-8-8 mode
#define OSPI_CMD_READ_DOPI                      0xEE   // 8D-8D-8D mode

#define OSPI_READ_1IO_DUMMY_CYCLE               0
#define OSPI_READ_FAST_DUMMY_CYCLE              8
#define OSPI_READ_8IO_DUMMY_CYCLE               20

// Commands for erasing
#define OSPI_CMD_ERASE_SECTOR                   0x21    // 4kB
#define OSPI_CMD_ERASE_SECTOR_4B                0x21    // 4kB
//#define OSPI_CMD_ERASE_BLOCK_32   // not supported, only ersae block 64
#define OSPI_CMD_ERASE_BLOCK_64                 0xD8    // 64kB
#define OSPI_CMD_ERASE_CHIP                     0x60    // or 0xC7

// erase operations max time [us] (datasheet max time + 15%)
#define OSPI_ERASE_SECTOR_MAX_TIME              480000      // 400 ms
#define OSPI_ERASE_BLOCK_64_MAX_TIME            2400000     // 2s

// max frequency for basic rw operation (for fast mode)
#define OSPI_COMMON_MAX_FREQUENCY               66000000

#define OSPI_STATUS_REG_SIZE                    2 //2 ??
#define OSPI_CONFIG_REG_0_SIZE                  1
#define OSPI_CONFIG_REG_1_SIZE                  2
#define OSPI_CONFIG_REG_2_SIZE                  2 //1 byte in spi and sopi mode;2 byte in dopi
#define OSPI_SECURITY_REG_SIZE                  1
#define OSPI_MAX_REG_SIZE                       2

// status register
#define STATUS_BIT_WIP   (1 << 0)   // write in progress bit
#define STATUS_BIT_WEL   (1 << 1)   // write enable latch
#define STATUS_BIT_BP0   (1 << 2)   //
#define STATUS_BIT_BP1   (1 << 3)   //
#define STATUS_BIT_BP2   (1 << 4)   //
#define STATUS_BIT_BP3   (1 << 5)   //
//#define STATUS_BIT_QE    (1 << 6)   // Not supported
//#define STATUS_BIT_SRWD  (1 << 7)   // Not supported

// configuration register 0
// bit 0, 1, 2, 4, 5, 7 reserved
#define CONFIG0_BIT_TB   (1 << 3)   // Top/Bottom area protect

#define CONFIG2_OPI_EN_ADDR   0x00000000

// configuration register 2
#define CONFIG2_BIT_SOPI_EN   (1 << 0)   // sopi mode enable
#define CONFIG2_BIT_DOPI_EN   (1 << 1)   // dopi mode enable
#define CONFIG2_SPI_EN        0          // spi mode enable

#define OCTA_ENABLE()                                                       \
                                                                            \
    uint8_t reg_data[OSPI_CONFIG_REG_2_SIZE];                               \
                                                                            \
    memset(reg_data, 0, OSPI_CONFIG_REG_2_SIZE);                            \
    if (write_enable(ospi) != OSPI_STATUS_OK) {                             \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    reg_data[0] = CONFIG2_BIT_SOPI_EN;                                      \
    ospi.cmd.configure(MODE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8);               \
    if (write_config_register_2(OSPI_CMD_WRCR2, CONFIG2_OPI_EN_ADDR,        \
        reg_data, 1, ospi) != OSPI_STATUS_OK) {                             \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
    ospi.cmd.configure(MODE_8_8_8, ADDR_SIZE_32, ALT_SIZE_8);               \
    WAIT_FOR(WRSR_MAX_TIME, ospi);                                          \
    memset(reg_data, 0, OSPI_CONFIG_REG_2_SIZE);                            \
                                                                            \
    if (read_config_register_2(OSPI_CMD_RDCR2, CONFIG2_OPI_EN_ADDR,         \
        reg_data, OSPI_CONFIG_REG_2_SIZE, ospi) != OSPI_STATUS_OK) {        \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
    return (reg_data[0] == CONFIG2_BIT_SOPI_EN ?                            \
            OSPI_STATUS_OK : OSPI_STATUS_ERROR)


#define OCTA_DISABLE()                                                      \
                                                                            \
    uint8_t reg_data[OSPI_CONFIG_REG_2_SIZE];                               \
                                                                            \
    memset(reg_data, 0, OSPI_CONFIG_REG_2_SIZE);                            \
    if (write_enable(ospi) != OSPI_STATUS_OK) {                             \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    reg_data[0] = CONFIG2_SPI_EN;                                           \
    if (write_config_register_2(OSPI_CMD_WRCR2, CONFIG2_OPI_EN_ADDR,        \
        reg_data, 1, ospi) != OSPI_STATUS_OK) {                             \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
    ospi.cmd.configure(MODE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8);               \
    WAIT_FOR(WRSR_MAX_TIME, ospi);                                          \
    memset(reg_data, 0, OSPI_CONFIG_REG_2_SIZE);                            \
    if (read_config_register_2(OSPI_CMD_RDCR2, CONFIG2_OPI_EN_ADDR,         \
        reg_data, 1, ospi) != OSPI_STATUS_OK) {                             \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
    return (reg_data[0] == CONFIG2_SPI_EN ?                                 \
            OSPI_STATUS_OK : OSPI_STATUS_ERROR)

#define OCTA_DTR_ENABLE()                                                   \
                                                                            \
    uint8_t reg_data[OSPI_CONFIG_REG_2_SIZE];                               \
                                                                            \
    memset(reg_data, 0, OSPI_CONFIG_REG_2_SIZE);                            \
    if (write_enable(ospi) != OSPI_STATUS_OK) {                             \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    reg_data[0] = CONFIG2_BIT_DOPI_EN;                                      \
    ospi.cmd.configure(MODE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8);               \
    if (write_config_register_2(OSPI_CMD_WRCR2, CONFIG2_OPI_EN_ADDR,        \
        reg_data, 1, ospi) != OSPI_STATUS_OK) {                             \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
    ospi.cmd.configure(MODE_8D_8D_8D, ADDR_SIZE_32, ALT_SIZE_8);            \
    WAIT_FOR(WRSR_MAX_TIME, ospi);                                          \
    memset(reg_data, 0, OSPI_CONFIG_REG_2_SIZE);                            \
    if (read_config_register_2(OSPI_CMD_RDCR2, CONFIG2_OPI_EN_ADDR,         \
        reg_data, OSPI_CONFIG_REG_2_SIZE, ospi) != OSPI_STATUS_OK) {        \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
    return (reg_data[0] == CONFIG2_BIT_DOPI_EN ?                            \
            OSPI_STATUS_OK : OSPI_STATUS_ERROR)


#define OCTA_DTR_DISABLE()                                                  \
                                                                            \
    uint8_t reg_data[OSPI_CONFIG_REG_2_SIZE];                               \
                                                                            \
    memset(reg_data, 0, OSPI_CONFIG_REG_2_SIZE);                            \
    if (write_enable(ospi) != OSPI_STATUS_OK) {                             \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    reg_data[0] = CONFIG2_SPI_EN;                                           \
    if (write_config_register_2(OSPI_CMD_WRCR2, CONFIG2_OPI_EN_ADDR,        \
        reg_data, OSPI_CONFIG_REG_2_SIZE, ospi) != OSPI_STATUS_OK) {        \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
    ospi.cmd.configure(MODE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8);               \
    WAIT_FOR(WRSR_MAX_TIME, ospi);                                          \
    memset(reg_data, 0, OSPI_CONFIG_REG_2_SIZE);                            \
    if (read_config_register_2(OSPI_CMD_RDCR2, CONFIG2_OPI_EN_ADDR,         \
        reg_data, OSPI_CONFIG_REG_2_SIZE, ospi) != OSPI_STATUS_OK) {        \
        return OSPI_STATUS_ERROR;                                           \
    }                                                                       \
    return (reg_data[0] == CONFIG2_SPI_EN ?                                 \
            OSPI_STATUS_OK : OSPI_STATUS_ERROR)

#endif // MBED_OSPI_FLASH_MX25LM51245G_H
