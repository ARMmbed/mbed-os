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

#include "utest/utest.h"

#include "hal/qspi_api.h"
#include "hal/us_ticker_api.h"
#include "qspi_test_utils.h"

#include "unity/unity.h"

#include <string.h> // for memset

#include "flash_configs/flash_configs.h"
#include "mbed.h"

static qspi_status_t extended_enable(Qspi &qspi);
static qspi_status_t extended_disable(Qspi &qspi);
static qspi_status_t dual_enable(Qspi &qspi);
static qspi_status_t dual_disable(Qspi &qspi);
static qspi_status_t quad_enable(Qspi &qspi);
static qspi_status_t quad_disable(Qspi &qspi);

void QspiCommand::configure(qspi_bus_width_t inst_width, qspi_bus_width_t addr_width,
                            qspi_bus_width_t data_width, qspi_bus_width_t alt_width,
                            qspi_address_size_t addr_size, qspi_alt_size_t alt_size,
                            int dummy_cycles)
{
    memset(&_cmd, 0,  sizeof(qspi_command_t));
    _cmd.instruction.disabled = _cmd.address.disabled = _cmd.alt.disabled = true;

    _cmd.instruction.bus_width = inst_width;
    _cmd.address.bus_width = addr_width;
    _cmd.address.size = addr_size;
    _cmd.alt.bus_width = alt_width;
    _cmd.alt.size = alt_size;
    _cmd.data.bus_width = data_width;
    _cmd.dummy_count = dummy_cycles;
}

void QspiCommand::set_dummy_cycles(int dummy_cycles)
{
    _cmd.dummy_count = dummy_cycles;
}

void QspiCommand::build(int instruction, int address, int alt)
{
    _cmd.instruction.disabled = (instruction == QSPI_NO_INST);
    if (!_cmd.instruction.disabled) {
        _cmd.instruction.value = instruction;
    }

    _cmd.address.disabled = (address == QSPI_NONE);
    if (!_cmd.address.disabled) {
        _cmd.address.value = address;
    }

    _cmd.alt.disabled = (alt == QSPI_NONE);
    if (!_cmd.alt.disabled) {
        _cmd.alt.value = alt;
    }
}

qspi_command_t *QspiCommand::get()
{
    return &_cmd;
}


qspi_status_t read_register(uint32_t cmd, uint8_t *buf, uint32_t size, Qspi &q)
{
    q.cmd.build(cmd);
    return qspi_command_transfer(&q.handle, q.cmd.get(), NULL, 0, buf, size);
}

qspi_status_t write_register(uint32_t cmd, uint8_t *buf, uint32_t size, Qspi &q)
{
    q.cmd.build(cmd);
    return qspi_command_transfer(&q.handle, q.cmd.get(), buf, size, NULL, 0);
}


QspiStatus flash_wait_for(uint32_t time_us, Qspi &qspi)
{
    uint8_t reg[QSPI_STATUS_REG_SIZE];
    qspi_status_t ret;
    uint32_t curr_time;

    const ticker_data_t *const ticker = get_us_ticker_data();
    const uint32_t start = ticker_read(ticker);

    memset(reg, 255, QSPI_STATUS_REG_SIZE);
    do {
        ret = read_register(STATUS_REG, reg, QSPI_STATUS_REG_SIZE, qspi);
        curr_time = ticker_read(ticker);
    } while (((reg[0] & STATUS_BIT_WIP) != 0) && ((curr_time - start) < time_us));

    if (((reg[0] & STATUS_BIT_WIP) == 0) && (ret == QSPI_STATUS_OK)) {
        return sOK;
    } else if (ret != QSPI_STATUS_OK) {
        return sError;
    } else if ((curr_time - start) >= time_us) {
        return sTimeout;
    }
    return sUnknown;
}

void flash_init(Qspi &qspi)
{
    uint8_t status[QSPI_STATUS_REG_SIZE];
    qspi_status_t ret;

    qspi.cmd.build(QSPI_CMD_RDSR);
    ret = qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, status, QSPI_STATUS_REG_SIZE);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

    // Only do reset enable if device needs it
    if (QSPI_CMD_RSTEN != 0) {
        qspi.cmd.build(QSPI_CMD_RSTEN);
        ret = qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0);
        TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

        WAIT_FOR(WRSR_MAX_TIME, qspi);
    }

    qspi.cmd.build(QSPI_CMD_RST);
    ret = qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

    WAIT_FOR(WAIT_MAX_TIME, qspi);

    // Zero out status register to attempt to clear block protection bits
    uint8_t blanks[QSPI_STATUS_REG_SIZE] = {0};

    qspi.cmd.build(QSPI_CMD_WREN);
    ret = qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

    qspi.cmd.build(QSPI_CMD_WRSR);
    ret = qspi_command_transfer(&qspi.handle, qspi.cmd.get(), blanks, 1, NULL, 0);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

    WAIT_FOR(WRSR_MAX_TIME, qspi);
}


qspi_status_t write_enable(Qspi &qspi)
{
    uint8_t reg[QSPI_STATUS_REG_SIZE];
    qspi.cmd.build(QSPI_CMD_WREN);

    if (qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }
    WAIT_FOR(WRSR_MAX_TIME, qspi);

    memset(reg, 0, QSPI_STATUS_REG_SIZE);
    if (read_register(STATUS_REG, reg, QSPI_STATUS_REG_SIZE, qspi) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }

    return ((reg[0] & STATUS_BIT_WEL) != 0 ? QSPI_STATUS_OK : QSPI_STATUS_ERROR);
}

qspi_status_t write_disable(Qspi &qspi)
{
    uint8_t reg[QSPI_STATUS_REG_SIZE];
    qspi.cmd.build(QSPI_CMD_WRDI);

    if (qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }
    WAIT_FOR(WRSR_MAX_TIME, qspi);

    memset(reg, 0, QSPI_STATUS_REG_SIZE);
    if (read_register(STATUS_REG, reg, QSPI_STATUS_REG_SIZE, qspi) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }

    return ((reg[0] & STATUS_BIT_WEL) == 0 ? QSPI_STATUS_OK : QSPI_STATUS_ERROR);
}

void log_register(uint32_t cmd, uint32_t reg_size, Qspi &qspi, const char *str)
{
    qspi_status_t ret;
    static uint8_t reg[QSPI_MAX_REG_SIZE];

    ret = read_register(cmd, reg, reg_size, qspi);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

    for (uint32_t j = 0; j < reg_size; j++) {
        utest_printf("%s byte %u (MSB first): ", str != NULL ? str : "", j);
        for (int i = 0; i < 8; i++) {
            utest_printf("%s ", ((reg[j] & (1 << (7 - i))) & 0xFF) == 0 ? "0" : "1");
        }
        utest_printf("\r\n");
    }
}

qspi_status_t erase(uint32_t erase_cmd, uint32_t flash_addr, Qspi &qspi)
{
    qspi.cmd.build(erase_cmd, flash_addr);
    return qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0);
}

qspi_status_t mode_enable(Qspi &qspi, qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width)
{
    if (is_extended_mode(inst_width, addr_width, data_width)) {
        return extended_enable(qspi);
    } else if (is_dual_mode(inst_width, addr_width, data_width)) {
        return dual_enable(qspi);
    } else if (is_quad_mode(inst_width, addr_width, data_width)) {
        return quad_enable(qspi);
    } else {
        return QSPI_STATUS_OK;
    }
}

qspi_status_t mode_disable(Qspi &qspi, qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width)
{
    if (is_extended_mode(inst_width, addr_width, data_width)) {
        return extended_disable(qspi);
    } else if (is_dual_mode(inst_width, addr_width, data_width)) {
        return dual_disable(qspi);
    } else if (is_quad_mode(inst_width, addr_width, data_width)) {
        return quad_disable(qspi);
    } else {
        return QSPI_STATUS_OK;
    }
}

static qspi_status_t extended_enable(Qspi &qspi)
{
#ifdef EXTENDED_SPI_ENABLE
    EXTENDED_SPI_ENABLE();
#else
    return QSPI_STATUS_OK;
#endif
}

static qspi_status_t extended_disable(Qspi &qspi)
{
#ifdef EXTENDED_SPI_DISABLE
    EXTENDED_SPI_DISABLE();
#else
    return QSPI_STATUS_OK;
#endif
}

static qspi_status_t dual_enable(Qspi &qspi)
{
#ifdef DUAL_ENABLE
    DUAL_ENABLE();
#else
    return QSPI_STATUS_OK;
#endif
}

static qspi_status_t dual_disable(Qspi &qspi)
{
#ifdef DUAL_DISABLE
    DUAL_DISABLE();
#else
    return QSPI_STATUS_OK;
#endif

}

static qspi_status_t quad_enable(Qspi &qspi)
{
#ifdef QUAD_ENABLE
    QUAD_ENABLE();
#else
    return QSPI_STATUS_OK;
#endif
}

static qspi_status_t quad_disable(Qspi &qspi)
{
#ifdef QUAD_DISABLE
    QUAD_DISABLE();
#else
    return QSPI_STATUS_OK;
#endif
}

qspi_status_t fast_mode_enable(Qspi &qspi)
{
#ifdef FAST_MODE_ENABLE
    FAST_MODE_ENABLE();
#else
    return QSPI_STATUS_OK;
#endif
}

qspi_status_t fast_mode_disable(Qspi &qspi)
{
#ifdef FAST_MODE_DISABLE
    FAST_MODE_DISABLE();
#else
    return QSPI_STATUS_OK;
#endif
}

bool is_extended_mode(qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width)
{
    return (inst_width == QSPI_CFG_BUS_SINGLE) && ((addr_width != QSPI_CFG_BUS_SINGLE) || (data_width != QSPI_CFG_BUS_SINGLE));
}

bool is_dual_mode(qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width)
{
    return (inst_width == QSPI_CFG_BUS_DUAL) && (addr_width == QSPI_CFG_BUS_DUAL) && (data_width == QSPI_CFG_BUS_DUAL);
}

bool is_quad_mode(qspi_bus_width_t inst_width, qspi_bus_width_t addr_width, qspi_bus_width_t data_width)
{
    return (inst_width == QSPI_CFG_BUS_QUAD) && (addr_width == QSPI_CFG_BUS_QUAD) && (data_width == QSPI_CFG_BUS_QUAD);
}
