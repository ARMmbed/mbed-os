/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

#include "drivers/QSPI.h"
#include "platform/mbed_critical.h"
#include <string.h>

#if DEVICE_QSPI

namespace mbed {

QSPI *QSPI::_owner = NULL;
SingletonPtr<PlatformMutex> QSPI::_mutex;

uint8_t convert_bus_width_to_line_count(qspi_bus_width_t width)
{
    switch (width) {
        case QSPI_CFG_BUS_SINGLE:
            return 1;
        case QSPI_CFG_BUS_DUAL:
            return 2;
        case QSPI_CFG_BUS_QUAD:
            return 4;
        default:
            // Unrecognized bus width
            return 0;
    }
}

QSPI::QSPI(PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, int mode) : _qspi()
{
    _qspi_io0 = io0;
    _qspi_io1 = io1;
    _qspi_io2 = io2;
    _qspi_io3 = io3;
    _qspi_clk = sclk;
    _qspi_cs = ssel;
    _inst_width = QSPI_CFG_BUS_SINGLE;
    _address_width = QSPI_CFG_BUS_SINGLE;
    _address_size = QSPI_CFG_ADDR_SIZE_24;
    _alt_width = QSPI_CFG_BUS_SINGLE;
    _alt_size = 0;
    _data_width = QSPI_CFG_BUS_SINGLE;
    _num_dummy_cycles = 0;
    _mode = mode;
    _hz = ONE_MHZ;
    _initialized = false;

    //Go ahead init the device here with the default config
    bool success = _initialize();
    MBED_ASSERT(success);
}

qspi_status_t QSPI::configure_format(qspi_bus_width_t inst_width, qspi_bus_width_t address_width, qspi_address_size_t address_size, qspi_bus_width_t alt_width, qspi_alt_size_t alt_size, qspi_bus_width_t data_width, int dummy_cycles)
{
    // Check that alt_size/alt_width are a valid combination
    uint8_t alt_lines = convert_bus_width_to_line_count(alt_width);
    if (alt_lines == 0) {
        return QSPI_STATUS_ERROR;
    } else if (alt_size % alt_lines != 0) {
        // Invalid alt size/width combination (alt size is not a multiple of the number of bus lines used to transmit it)
        return QSPI_STATUS_ERROR;
    }

    lock();
    _inst_width = inst_width;
    _address_width = address_width;
    _address_size = address_size;
    _alt_width = alt_width;
    _alt_size = alt_size;
    _data_width = data_width;
    _num_dummy_cycles = dummy_cycles;
    unlock();

    return QSPI_STATUS_OK;
}

qspi_status_t QSPI::set_frequency(int hz)
{
    qspi_status_t ret_status = QSPI_STATUS_OK;

    if (_initialized) {
        lock();
        _hz = hz;
        //If the same owner, just change freq.
        //Otherwise we may have to change mode as well, so call _acquire
        if (_owner == this) {
            if (QSPI_STATUS_OK != qspi_frequency(&_qspi, _hz)) {
                ret_status = QSPI_STATUS_ERROR;
            }
        } else {
            _acquire();
        }
        unlock();
    } else {
        ret_status = QSPI_STATUS_ERROR;
    }

    return ret_status;
}

qspi_status_t QSPI::read(int address, char *rx_buffer, size_t *rx_length)
{
    qspi_status_t ret_status = QSPI_STATUS_ERROR;

    if (_initialized) {
        if ((rx_length != NULL) && (rx_buffer != NULL)) {
            if (*rx_length != 0) {
                lock();
                if (true == _acquire()) {
                    _build_qspi_command(QSPI_NO_INST, address, -1);
                    if (QSPI_STATUS_OK == qspi_read(&_qspi, &_qspi_command, rx_buffer, rx_length)) {
                        ret_status = QSPI_STATUS_OK;
                    }
                }
                unlock();
            }
        } else {
            ret_status = QSPI_STATUS_INVALID_PARAMETER;
        }
    }

    return ret_status;
}

qspi_status_t QSPI::write(int address, const char *tx_buffer, size_t *tx_length)
{
    qspi_status_t ret_status = QSPI_STATUS_ERROR;

    if (_initialized) {
        if ((tx_length != NULL) && (tx_buffer != NULL)) {
            if (*tx_length != 0) {
                lock();
                if (true == _acquire()) {
                    _build_qspi_command(QSPI_NO_INST, address, -1);
                    if (QSPI_STATUS_OK == qspi_write(&_qspi, &_qspi_command, tx_buffer, tx_length)) {
                        ret_status = QSPI_STATUS_OK;
                    }
                }
                unlock();
            }
        } else {
            ret_status = QSPI_STATUS_INVALID_PARAMETER;
        }
    }

    return ret_status;
}

qspi_status_t QSPI::read(qspi_inst_t instruction, int alt, int address, char *rx_buffer, size_t *rx_length)
{
    qspi_status_t ret_status = QSPI_STATUS_ERROR;

    if (_initialized) {
        if ((rx_length != NULL) && (rx_buffer != NULL)) {
            if (*rx_length != 0) {
                lock();
                if (true == _acquire()) {
                    _build_qspi_command(instruction, address, alt);
                    if (QSPI_STATUS_OK == qspi_read(&_qspi, &_qspi_command, rx_buffer, rx_length)) {
                        ret_status = QSPI_STATUS_OK;
                    }
                }
                unlock();
            }
        } else {
            ret_status = QSPI_STATUS_INVALID_PARAMETER;
        }
    }

    return ret_status;
}

qspi_status_t QSPI::write(qspi_inst_t instruction, int alt, int address, const char *tx_buffer, size_t *tx_length)
{
    qspi_status_t ret_status = QSPI_STATUS_ERROR;

    if (_initialized) {
        if ((tx_length != NULL) && (tx_buffer != NULL)) {
            if (*tx_length != 0) {
                lock();
                if (true == _acquire()) {
                    _build_qspi_command(instruction, address, alt);
                    if (QSPI_STATUS_OK == qspi_write(&_qspi, &_qspi_command, tx_buffer, tx_length)) {
                        ret_status = QSPI_STATUS_OK;
                    }
                }
                unlock();
            }
        } else {
            ret_status = QSPI_STATUS_INVALID_PARAMETER;
        }
    }

    return ret_status;
}

qspi_status_t QSPI::command_transfer(qspi_inst_t instruction, int address, const char *tx_buffer, size_t tx_length, const char *rx_buffer, size_t rx_length)
{
    qspi_status_t ret_status = QSPI_STATUS_ERROR;

    if (_initialized) {
        lock();
        if (true == _acquire()) {
            _build_qspi_command(instruction, address, -1); //We just need the command
            if (QSPI_STATUS_OK == qspi_command_transfer(&_qspi, &_qspi_command, (const void *)tx_buffer, tx_length, (void *)rx_buffer, rx_length)) {
                ret_status = QSPI_STATUS_OK;
            }
        }
        unlock();
    }

    return ret_status;
}

void QSPI::lock()
{
    _mutex->lock();
}

void QSPI::unlock()
{
    _mutex->unlock();
}

// Note: Private helper function to initialize qspi HAL
bool QSPI::_initialize()
{
    if (_mode != 0 && _mode != 1) {
        _initialized = false;
        return _initialized;
    }

    qspi_status_t ret = qspi_init(&_qspi, _qspi_io0, _qspi_io1, _qspi_io2, _qspi_io3, _qspi_clk, _qspi_cs, _hz, _mode);
    if (QSPI_STATUS_OK == ret) {
        _initialized = true;
    } else {
        _initialized = false;
    }

    return _initialized;
}

// Note: Private function with no locking
bool QSPI::_acquire()
{
    if (_owner != this) {
        //This will set freq as well
        _initialize();
        _owner = this;
    }

    return _initialized;
}

void QSPI::_build_qspi_command(qspi_inst_t instruction, int address, int alt)
{
    memset(&_qspi_command, 0,  sizeof(qspi_command_t));
    //Set up instruction phase parameters
    _qspi_command.instruction.bus_width = _inst_width;
    if (instruction != QSPI_NO_INST) {
        _qspi_command.instruction.value = instruction;
        _qspi_command.instruction.disabled = false;
    } else {
        _qspi_command.instruction.disabled = true;
    }

    //Set up address phase parameters
    _qspi_command.address.bus_width = _address_width;
    _qspi_command.address.size = _address_size;
    if (address != -1) {
        _qspi_command.address.value = address;
        _qspi_command.address.disabled = false;
    } else {
        _qspi_command.address.disabled = true;
    }

    //Set up alt phase parameters
    _qspi_command.alt.bus_width = _alt_width;
    _qspi_command.alt.size = _alt_size;
    if (alt != -1) {
        _qspi_command.alt.value = alt;
        _qspi_command.alt.disabled = false;
    } else {
        _qspi_command.alt.disabled = true;
    }

    _qspi_command.dummy_count = _num_dummy_cycles;

    //Set up bus width for data phase
    _qspi_command.data.bus_width = _data_width;
}

} // namespace mbed

#endif
