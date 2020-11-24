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

#include "ospi_test_utils.h"

#include "utest/utest.h"

#include "hal/ospi_api.h"
#include "hal/us_ticker_api.h"
#include "unity/unity.h"

#include <string.h> // for memset

#include "flash_configs/flash_configs.h"
#include "mbed.h"

static ospi_status_t extended_enable(Ospi &ospi);
static ospi_status_t extended_disable(Ospi &ospi);
static ospi_status_t dual_enable(Ospi &ospi);
static ospi_status_t dual_disable(Ospi &ospi);
static ospi_status_t quad_enable(Ospi &ospi);
static ospi_status_t quad_disable(Ospi &ospi);
static ospi_status_t octa_enable(Ospi &ospi);
static ospi_status_t octa_disable(Ospi &ospi);
static ospi_status_t octa_dtr_enable(Ospi &ospi);
static ospi_status_t octa_dtr_disable(Ospi &ospi);

void OspiCommand::configure(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width,
                            ospi_bus_width_t data_width, ospi_bus_width_t alt_width,
                            ospi_address_size_t addr_size, ospi_alt_size_t alt_size,
                            int dummy_cycles)
{
    memset(&_cmd, 0,  sizeof(ospi_command_t));
    _cmd.instruction.disabled = _cmd.address.disabled = _cmd.alt.disabled = true;

    _cmd.instruction.bus_width = inst_width;
    _cmd.address.bus_width = addr_width;
    _cmd.address.size = addr_size;
    _cmd.alt.bus_width = alt_width;
    _cmd.alt.size = alt_size;
    _cmd.data.bus_width = data_width;
    _cmd.dummy_count = dummy_cycles;
}

void OspiCommand::set_dummy_cycles(int dummy_cycles)
{
    _cmd.dummy_count = dummy_cycles;
}

void OspiCommand::build(int instruction, int address, int alt)
{
    _cmd.instruction.disabled = (instruction == -1);
    if (!_cmd.instruction.disabled) {
        _cmd.instruction.value = instruction;
    }

    _cmd.address.disabled = (address == OSPI_NONE);
    if (!_cmd.address.disabled) {
        _cmd.address.value = address;
    }

    _cmd.alt.disabled = (alt == OSPI_NONE);
    if (!_cmd.alt.disabled) {
        _cmd.alt.value = alt;
    }

    if ((_cmd.instruction.bus_width == OSPI_CFG_BUS_OCTA) || (_cmd.instruction.bus_width == OSPI_CFG_BUS_OCTA_DTR)) {
        if (instruction == STATUS_REG) {
            _cmd.address.disabled = 0;
            _cmd.address.value = 0;
            _cmd.dummy_count = OSPI_READ_FAST_DUMMY_CYCLE;
        } else if (instruction == OSPI_CMD_RDCR2) {
            _cmd.dummy_count = OSPI_READ_FAST_DUMMY_CYCLE;
        } else if ((instruction == OSPI_CMD_READ_OPI) || (instruction ==  OSPI_CMD_READ_DOPI)) {
            _cmd.dummy_count = OSPI_READ_8IO_DUMMY_CYCLE;
        } else {
            _cmd.dummy_count = 0;
        }
    }
}

ospi_command_t *OspiCommand::get()
{
    return &_cmd;
}


ospi_status_t read_register(uint32_t cmd, uint8_t *buf, uint32_t size, Ospi &q)
{
    q.cmd.build(cmd);
    return ospi_command_transfer(&q.handle, q.cmd.get(), NULL, 0, buf, size);
}

ospi_status_t write_register(uint32_t cmd, uint8_t *buf, uint32_t size, Ospi &q)
{
    q.cmd.build(cmd);
    return ospi_command_transfer(&q.handle, q.cmd.get(), buf, size, NULL, 0);
}

ospi_status_t read_config_register_2(uint32_t cmd, uint32_t addr, uint8_t *buf, uint32_t size, Ospi &q)
{
    q.cmd.build(cmd, addr);
    return ospi_command_transfer(&q.handle, q.cmd.get(), NULL, 0, buf, size);
}

ospi_status_t write_config_register_2(uint32_t cmd, uint32_t addr, uint8_t *buf, uint32_t size, Ospi &q)
{
    q.cmd.build(cmd, addr);
    return ospi_command_transfer(&q.handle, q.cmd.get(), buf, size, NULL, 0);
}

OspiStatus flash_wait_for(uint32_t time_us, Ospi &ospi)
{
    uint8_t reg[OSPI_STATUS_REG_SIZE];
    ospi_status_t ret;
    uint32_t curr_time;

    const ticker_data_t *const ticker = get_us_ticker_data();
    const uint32_t start = ticker_read(ticker);

    memset(reg, 255, OSPI_STATUS_REG_SIZE);
    do {
        ret = read_register(STATUS_REG, reg, OSPI_STATUS_REG_SIZE, ospi);
        curr_time = ticker_read(ticker);
    } while (((reg[0] & STATUS_BIT_WIP) != 0) && ((curr_time - start) < time_us));

    if (((reg[0] & STATUS_BIT_WIP) == 0) && (ret == OSPI_STATUS_OK)) {
        return sOK;
    } else if (ret != OSPI_STATUS_OK) {
        return sError;
    } else if ((curr_time - start) >= time_us) {
        return sTimeout;
    }
    return sUnknown;
}

void flash_init(Ospi &ospi)
{
    uint8_t status[OSPI_STATUS_REG_SIZE];
    ospi_status_t ret;

    ospi.cmd.build(OSPI_CMD_RDSR);
    ret = ospi_command_transfer(&ospi.handle, ospi.cmd.get(), NULL, 0, status, OSPI_STATUS_REG_SIZE);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

    // Only do reset enable if device needs it
    if (OSPI_CMD_RSTEN != 0) {
        ospi.cmd.build(OSPI_CMD_RSTEN);
        ret = ospi_command_transfer(&ospi.handle, ospi.cmd.get(), NULL, 0, NULL, 0);
        TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

        WAIT_FOR(WRSR_MAX_TIME, ospi);
    }

    ospi.cmd.build(OSPI_CMD_RST);
    ret = ospi_command_transfer(&ospi.handle, ospi.cmd.get(), NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

    WAIT_FOR(WAIT_MAX_TIME, ospi);

    // Zero out status register to attempt to clear block protection bits
    uint8_t blanks[OSPI_STATUS_REG_SIZE] = {0};

    ospi.cmd.build(OSPI_CMD_WREN);
    ret = ospi_command_transfer(&ospi.handle, ospi.cmd.get(), NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

    ospi.cmd.build(OSPI_CMD_WRSR);
    ret = ospi_command_transfer(&ospi.handle, ospi.cmd.get(), blanks, 1, NULL, 0);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

    WAIT_FOR(WRSR_MAX_TIME, ospi);
}


ospi_status_t write_enable(Ospi &ospi)
{
    uint8_t reg[OSPI_STATUS_REG_SIZE];
    ospi.cmd.build(OSPI_CMD_WREN);

    if (ospi_command_transfer(&ospi.handle, ospi.cmd.get(), NULL, 0, NULL, 0) != OSPI_STATUS_OK) {
        return OSPI_STATUS_ERROR;
    }
    WAIT_FOR(WRSR_MAX_TIME, ospi);

    memset(reg, 0, OSPI_STATUS_REG_SIZE);
    if (read_register(STATUS_REG, reg, OSPI_STATUS_REG_SIZE, ospi) != OSPI_STATUS_OK) {
        return OSPI_STATUS_ERROR;
    }

    return ((reg[0] & STATUS_BIT_WEL) != 0 ? OSPI_STATUS_OK : OSPI_STATUS_ERROR);
}

ospi_status_t write_disable(Ospi &ospi)
{
    uint8_t reg[OSPI_STATUS_REG_SIZE];
    ospi.cmd.build(OSPI_CMD_WRDI);

    if (ospi_command_transfer(&ospi.handle, ospi.cmd.get(), NULL, 0, NULL, 0) != OSPI_STATUS_OK) {
        return OSPI_STATUS_ERROR;
    }
    WAIT_FOR(WRSR_MAX_TIME, ospi);

    memset(reg, 0, OSPI_STATUS_REG_SIZE);
    if (read_register(STATUS_REG, reg, OSPI_STATUS_REG_SIZE, ospi) != OSPI_STATUS_OK) {
        return OSPI_STATUS_ERROR;
    }

    return ((reg[0] & STATUS_BIT_WEL) == 0 ? OSPI_STATUS_OK : OSPI_STATUS_ERROR);
}

void log_register(uint32_t cmd, uint32_t reg_size, Ospi &ospi, const char *str)
{
    ospi_status_t ret;
    static uint8_t reg[OSPI_MAX_REG_SIZE];

    ret = read_register(cmd, reg, reg_size, ospi);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

    for (uint32_t j = 0; j < reg_size; j++) {
        utest_printf("%s byte %u (MSB first): ", str != NULL ? str : "", j);
        for (int i = 0; i < 8; i++) {
            utest_printf("%s ", ((reg[j] & (1 << (7 - i))) & 0xFF) == 0 ? "0" : "1");
        }
        utest_printf("\r\n");
    }
}

ospi_status_t erase(uint32_t erase_cmd, uint32_t flash_addr, Ospi &ospi)
{
    ospi.cmd.build(erase_cmd, flash_addr);
    return ospi_command_transfer(&ospi.handle, ospi.cmd.get(), NULL, 0, NULL, 0);
}

ospi_status_t mode_enable(Ospi &ospi, ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width)
{
    if (is_extended_mode(inst_width, addr_width, data_width)) {
        return extended_enable(ospi);
    } else if (is_dual_mode(inst_width, addr_width, data_width)) {
        return dual_enable(ospi);
    } else if (is_quad_mode(inst_width, addr_width, data_width)) {
        return quad_enable(ospi);
    } else if (is_octa_mode(inst_width, addr_width, data_width)) {
        return octa_enable(ospi);
    } else if (is_octa_dtr_mode(inst_width, addr_width, data_width)) {
        return octa_dtr_enable(ospi);
    } else {
        return OSPI_STATUS_OK;
    }
}

ospi_status_t mode_disable(Ospi &ospi, ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width)
{
    if (is_extended_mode(inst_width, addr_width, data_width)) {
        return extended_disable(ospi);
    } else if (is_dual_mode(inst_width, addr_width, data_width)) {
        return dual_disable(ospi);
    } else if (is_quad_mode(inst_width, addr_width, data_width)) {
        return quad_disable(ospi);
    } else if (is_octa_mode(inst_width, addr_width, data_width)) {
        return octa_disable(ospi);
    } else if (is_octa_dtr_mode(inst_width, addr_width, data_width)) {
        return octa_dtr_disable(ospi);
    } else {
        return OSPI_STATUS_OK;
    }
}

static ospi_status_t extended_enable(Ospi &ospi)
{
#ifdef EXTENDED_SPI_ENABLE
    EXTENDED_SPI_ENABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

static ospi_status_t extended_disable(Ospi &ospi)
{
#ifdef EXTENDED_SPI_DISABLE
    EXTENDED_SPI_DISABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

static ospi_status_t dual_enable(Ospi &ospi)
{
#ifdef DUAL_ENABLE
    DUAL_ENABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

static ospi_status_t dual_disable(Ospi &ospi)
{
#ifdef DUAL_DISABLE
    DUAL_DISABLE();
#else
    return OSPI_STATUS_OK;
#endif

}

static ospi_status_t quad_enable(Ospi &ospi)
{
#ifdef QUAD_ENABLE
    QUAD_ENABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

static ospi_status_t quad_disable(Ospi &ospi)
{
#ifdef QUAD_DISABLE
    QUAD_DISABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

static ospi_status_t octa_enable(Ospi &ospi)
{
#ifdef OCTA_ENABLE
    OCTA_ENABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

static ospi_status_t octa_disable(Ospi &ospi)
{
#ifdef OCTA_DISABLE
    OCTA_DISABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

static ospi_status_t octa_dtr_enable(Ospi &ospi)
{
#ifdef OCTA_DTR_ENABLE
    OCTA_DTR_ENABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

static ospi_status_t octa_dtr_disable(Ospi &ospi)
{
#ifdef OCTA_DTR_DISABLE
    OCTA_DTR_DISABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

ospi_status_t fast_mode_enable(Ospi &ospi)
{
#ifdef FAST_MODE_ENABLE
    FAST_MODE_ENABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

ospi_status_t fast_mode_disable(Ospi &ospi)
{
#ifdef FAST_MODE_DISABLE
    FAST_MODE_DISABLE();
#else
    return OSPI_STATUS_OK;
#endif
}

bool is_extended_mode(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width)
{
    return (inst_width == OSPI_CFG_BUS_SINGLE) && ((addr_width != OSPI_CFG_BUS_SINGLE) || (data_width != OSPI_CFG_BUS_SINGLE));
}

bool is_dual_mode(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width)
{
    return (inst_width == OSPI_CFG_BUS_DUAL) && (addr_width == OSPI_CFG_BUS_DUAL) && (data_width == OSPI_CFG_BUS_DUAL);
}

bool is_quad_mode(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width)
{
    return (inst_width == OSPI_CFG_BUS_QUAD) && (addr_width == OSPI_CFG_BUS_QUAD) && (data_width == OSPI_CFG_BUS_QUAD);
}

bool is_octa_mode(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width)
{
    return (inst_width == OSPI_CFG_BUS_OCTA) && (addr_width == OSPI_CFG_BUS_OCTA) && (data_width == OSPI_CFG_BUS_OCTA);
}

bool is_octa_dtr_mode(ospi_bus_width_t inst_width, ospi_bus_width_t addr_width, ospi_bus_width_t data_width)
{
    return (inst_width == OSPI_CFG_BUS_OCTA_DTR) && (addr_width == OSPI_CFG_BUS_OCTA_DTR) && (data_width == OSPI_CFG_BUS_OCTA_DTR);
}
