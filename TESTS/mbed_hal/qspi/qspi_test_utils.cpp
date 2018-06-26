/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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


#include "hal/qspi_api.h"
#include "qspi_test_utils.h"

#include "unity/unity.h"

#include <string.h> // for memset

#include "flash_configs/flash_configs.h"
#include "mbed.h" // for WEAK

//====================================================================================
// all WEAK functions can be overridden, to fit board or memory chip specific features
// override in flash_configs/vendor/target/qspi_test_utils_specific.cpp
//====================================================================================


void QspiCommand::configure(qspi_bus_width_t inst_width, qspi_bus_width_t addr_width,
                            qspi_bus_width_t data_width, qspi_bus_width_t alt_width,
                            qspi_address_size_t addr_size, qspi_alt_size_t alt_size,
                            int dummy_cycles)
{
    memset(&cmd, 0,  sizeof(qspi_command_t) );
    cmd.instruction.disabled = cmd.address.disabled = cmd.alt.disabled = true;

    cmd.instruction.bus_width = inst_width;
    cmd.address.bus_width = addr_width;
    cmd.address.size = addr_size;
    cmd.alt.bus_width = alt_width;
    cmd.alt.size = alt_size;
    cmd.data.bus_width = data_width;
    cmd.dummy_count = dummy_cycles;
}

void QspiCommand::build(int instruction, int address, int alt)
{
    cmd.instruction.disabled = (instruction == QSPI_NONE);
    if (!cmd.instruction.disabled) {
        cmd.instruction.value = instruction;
    }

    cmd.address.disabled = (address == QSPI_NONE);
    if (!cmd.address.disabled) {
        cmd.address.value = address;
    }

    cmd.alt.disabled = (alt == QSPI_NONE);
    if (!cmd.alt.disabled) {
        cmd.alt.value = alt;
    }
}

qspi_command_t* QspiCommand::get()
{
    return &cmd;
}


qspi_status_t read_register(uint32_t cmd, uint8_t *buf, uint32_t size, Qspi &q)
{
    q.cmd.build(cmd);
    return qspi_command_transfer(&q.handle, q.cmd.get(), NULL, 0, buf, size);
}


bool flash_wait_for(uint32_t time_us, Qspi &qspi)
{
    uint8_t reg[QSPI_STATUS_REGISTER_SIZE];
    qspi_status_t ret;
    reg[0] = STATUS_BIT_WIP;

    const ticker_data_t *const ticker = get_us_ticker_data();
    const uint32_t start = ticker_read(ticker);

    do {
        ret = read_register(STATUS_REG, reg, QSPI_STATUS_REGISTER_SIZE, qspi);
    } while (((reg[0] & STATUS_BIT_WIP) != 0) && ((ticker_read(ticker) - start) < (uint32_t)time_us));

    return (((reg[0] & STATUS_BIT_WIP) == 0) && (ret == QSPI_STATUS_OK));
}

void flash_init(Qspi &qspi)
{
    uint8_t status[2] = { 0 };
    qspi_status_t ret;

    qspi.cmd.build(QSPI_CMD_RDSR);
    ret = qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, status, 1);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

    WAIT_FOR(WRSR_MAX_TIME, qspi);

    qspi.cmd.build(QSPI_CMD_RSTEN);
    ret = qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

    WAIT_FOR(WRSR_MAX_TIME, qspi);

    qspi.cmd.build(QSPI_CMD_RST);
    ret = qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

    WAIT_FOR(WAIT_MAX_TIME, qspi);
}


qspi_status_t write_enable(Qspi &qspi)
{
    uint8_t reg[QSPI_STATUS_REGISTER_SIZE];
    qspi.cmd.build(QSPI_CMD_WREN);

    if (qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }
    WAIT_FOR(WRSR_MAX_TIME, qspi);

    memset(reg, 0, QSPI_STATUS_REGISTER_SIZE);
    if (read_register(STATUS_REG, reg, QSPI_STATUS_REGISTER_SIZE, qspi) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }

    return ((reg[0] & STATUS_BIT_WEL) != 0 ? QSPI_STATUS_OK : QSPI_STATUS_ERROR);
}

qspi_status_t write_disable(Qspi &qspi)
{
    uint8_t reg[QSPI_STATUS_REGISTER_SIZE];
    qspi.cmd.build(QSPI_CMD_WRDI);

    if (qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }
    WAIT_FOR(WRSR_MAX_TIME, qspi);

    memset(reg, 0, QSPI_STATUS_REGISTER_SIZE);
    if (read_register(STATUS_REG, reg, QSPI_STATUS_REGISTER_SIZE, qspi) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }

    return ((reg[0] & STATUS_BIT_WEL) == 0 ? QSPI_STATUS_OK : QSPI_STATUS_ERROR);
}

WEAK qspi_status_t dual_enable(Qspi &qspi)
{
    return QSPI_STATUS_OK;
}

WEAK qspi_status_t dual_disable(Qspi &qspi)
{
    return QSPI_STATUS_OK;
}

WEAK qspi_status_t quad_enable(Qspi &qspi)
{
    uint8_t reg_data[QSPI_STATUS_REGISTER_SIZE];

    reg_data[0] = STATUS_BIT_QE;
    qspi.cmd.build(QSPI_CMD_WRSR);

    if (qspi_command_transfer(&qspi.handle, qspi.cmd.get(), reg_data, QSPI_STATUS_REGISTER_SIZE, NULL, 0) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }
    WAIT_FOR(WRSR_MAX_TIME, qspi);

    memset(reg_data, 0, QSPI_STATUS_REGISTER_SIZE);
    if (read_register(STATUS_REG, reg_data, QSPI_STATUS_REGISTER_SIZE, qspi) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }

    return ((reg_data[0] & STATUS_BIT_QE) != 0 ? QSPI_STATUS_OK : QSPI_STATUS_ERROR);
}

WEAK qspi_status_t quad_disable(Qspi &qspi)
{
    uint8_t reg_data[QSPI_STATUS_REGISTER_SIZE];

    reg_data[0] = 0;
    qspi.cmd.build(QSPI_CMD_WRSR);

    if (qspi_command_transfer(&qspi.handle, qspi.cmd.get(), reg_data, QSPI_STATUS_REGISTER_SIZE, NULL, 0) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }
    WAIT_FOR(WRSR_MAX_TIME, qspi);

    reg_data[0] = 0;
    if (read_register(STATUS_REG, reg_data, QSPI_STATUS_REGISTER_SIZE, qspi) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }

    return ((reg_data[0] & STATUS_BIT_QE) == 0 ? QSPI_STATUS_OK : QSPI_STATUS_ERROR);
}

WEAK qspi_status_t fast_mode_enable(Qspi &qspi)
{
    qspi_status_t ret;
    const int32_t reg_size = QSPI_STATUS_REGISTER_SIZE + QSPI_CONFIGURATION_REGISTER_SIZE;
    uint8_t reg_data[reg_size];

    if (read_register(STATUS_REG, reg_data, QSPI_STATUS_REGISTER_SIZE, qspi) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }
    if (read_register(CONFIG_REG, reg_data + QSPI_STATUS_REGISTER_SIZE, QSPI_CONFIGURATION_REGISTER_SIZE, qspi) != QSPI_STATUS_OK) {
        return QSPI_STATUS_ERROR;
    }

    reg_data[2] |= CONFIG1_BIT_LH;
    qspi.cmd.build(QSPI_CMD_WRSR);

    return qspi_command_transfer(&qspi.handle, qspi.cmd.get(), reg_data, reg_size, NULL, 0);
}

qspi_status_t erase(uint32_t erase_cmd, uint32_t flash_addr, Qspi &qspi)
{
    qspi.cmd.build(erase_cmd, flash_addr);
    return qspi_command_transfer(&qspi.handle, qspi.cmd.get(), NULL, 0, NULL, 0);
}
