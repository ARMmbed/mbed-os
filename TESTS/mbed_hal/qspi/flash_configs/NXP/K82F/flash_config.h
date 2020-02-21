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
#ifndef MBED_QSPI_FLASH_CONFIG_H
#define MBED_QSPI_FLASH_CONFIG_H

#include "../../MX25RXX35F_config.h"

/* Fast mode not supported in MX25U3235F */
#undef FAST_MODE_ENABLE
#undef FAST_MODE_DISABLE

#ifdef QSPI_SECTOR_COUNT
#undef QSPI_SECTOR_COUNT
#define QSPI_SECTOR_COUNT                       1024 // for MX25U3235F
#endif

/* The values for MX25U3235F are different, specify this here */
#undef QSPI_COMMON_MAX_FREQUENCY
#undef QSPI_WRSR_MAX_TIME
#undef QSPI_PAGE_PROG_MAX_TIME
#undef QSPI_ERASE_SECTOR_MAX_TIME
#undef QSPI_ERASE_BLOCK_32_MAX_TIME
#undef QSPI_ERASE_BLOCK_64_MAX_TIME

/* Implementation of these macros are slightly different for MX25U3235F */
#undef EXTENDED_SPI_ENABLE
#undef EXTENDED_SPI_DISABLE

/* Max frequency for basic rw operation in MX25U3235F */
#define QSPI_COMMON_MAX_FREQUENCY               54000000

/* WRSR operations max time [us] (datasheet max time + 15%) */
#define QSPI_WRSR_MAX_TIME                      46000   // 40ms

/* Write operations max time [us] (datasheet max time + 15%) */
#define QSPI_PAGE_PROG_MAX_TIME                 3450   // 3ms

/* erase operations max time [us] (datasheet max time + 15%) */
#define QSPI_ERASE_SECTOR_MAX_TIME              230000      // 200 ms
#define QSPI_ERASE_BLOCK_32_MAX_TIME            1150000     // 1s
#define QSPI_ERASE_BLOCK_64_MAX_TIME            2300000     // 2s

#define EXTENDED_SPI_ENABLE()                                               \
                                                                            \
    const int32_t reg_size = QSPI_STATUS_REG_SIZE;                          \
    uint8_t reg_data[reg_size] = { 0 };                                     \
                                                                            \
    if (read_register(STATUS_REG, reg_data,                                 \
            QSPI_STATUS_REG_SIZE, qspi) != QSPI_STATUS_OK) {                \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
    if (write_enable(qspi) != QSPI_STATUS_OK) {                             \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    reg_data[0] = STATUS_BIT_QE;                                            \
    if (write_register(QSPI_CMD_WRSR, reg_data,                             \
            reg_size, qspi) != QSPI_STATUS_OK) {                            \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
    WAIT_FOR(WRSR_MAX_TIME, qspi);                                          \
                                                                            \
    memset(reg_data, 0, QSPI_STATUS_REG_SIZE);                              \
    if (read_register(STATUS_REG, reg_data,                                 \
            QSPI_STATUS_REG_SIZE, qspi) != QSPI_STATUS_OK) {                \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    return ((reg_data[0] & STATUS_BIT_QE) != 0 ?                            \
            QSPI_STATUS_OK : QSPI_STATUS_ERROR)



#define EXTENDED_SPI_DISABLE()                                              \
                                                                            \
    const int32_t reg_size = QSPI_STATUS_REG_SIZE;                          \
    uint8_t reg_data[reg_size] = { 0 };                                     \
                                                                            \
    if (read_register(STATUS_REG, reg_data,                                 \
            QSPI_STATUS_REG_SIZE, qspi) != QSPI_STATUS_OK) {                \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    if (write_enable(qspi) != QSPI_STATUS_OK) {                             \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    reg_data[0] &= ~(STATUS_BIT_QE);                                        \
                                                                            \
    if (write_register(QSPI_CMD_WRSR, reg_data,                             \
            reg_size, qspi) != QSPI_STATUS_OK) {                            \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
    WAIT_FOR(WRSR_MAX_TIME, qspi);                                          \
                                                                            \
    reg_data[0] = 0;                                                        \
    if (read_register(STATUS_REG, reg_data,                                 \
        QSPI_STATUS_REG_SIZE, qspi) != QSPI_STATUS_OK) {                    \
        return QSPI_STATUS_ERROR;                                           \
    }                                                                       \
                                                                            \
    return ((reg_data[0] & STATUS_BIT_QE) == 0 ?                            \
            QSPI_STATUS_OK : QSPI_STATUS_ERROR)

#endif // MBED_QSPI_FLASH_CONFIG_H
