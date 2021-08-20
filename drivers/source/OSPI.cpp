/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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

#include "drivers/OSPI.h"
#include "platform/mbed_critical.h"
#include <string.h>

#if DEVICE_OSPI

namespace mbed {

SingletonPtr<PlatformMutex> OSPI::_mutex;

uint8_t convert_bus_width_to_line_count(ospi_bus_width_t width)
{
    switch (width) {
        case OSPI_CFG_BUS_SINGLE:
            return 1;
        case OSPI_CFG_BUS_DUAL:
            return 2;
        case OSPI_CFG_BUS_QUAD:
            return 4;
        case OSPI_CFG_BUS_OCTA:
        case OSPI_CFG_BUS_OCTA_DTR:
            return 8;
        default:
            // Unrecognized bus width
            return 0;
    }
}

OSPI::OSPI(PinName io0, PinName io1, PinName io2, PinName io3, PinName io4, PinName io5, PinName io6, PinName io7,
           PinName sclk, PinName ssel, PinName dqs, int mode) : _ospi()
{
    _ospi_io0 = io0;
    _ospi_io1 = io1;
    _ospi_io2 = io2;
    _ospi_io3 = io3;
    _ospi_io4 = io4;
    _ospi_io5 = io5;
    _ospi_io6 = io6;
    _ospi_io7 = io7;
    _ospi_clk = sclk;
    _ospi_cs = ssel;
    _ospi_dqs = dqs;
    _static_pinmap = NULL;
    _inst_width = OSPI_CFG_BUS_SINGLE;
    _inst_size = OSPI_CFG_INST_SIZE_8;
    _address_width = OSPI_CFG_BUS_SINGLE;
    _address_size = OSPI_CFG_ADDR_SIZE_24;
    _alt_width = OSPI_CFG_BUS_SINGLE;
    _alt_size = 0;
    _data_width = OSPI_CFG_BUS_SINGLE;
    _num_dummy_cycles = 0;
    _mode = mode;
    _hz = ONE_MHZ;
    _initialized = false;
    _init_func = &OSPI::_initialize;

    //Go ahead init the device here with the default config
    bool success = (this->*_init_func)();
    MBED_ASSERT(success);
}

OSPI::OSPI(const ospi_pinmap_t &pinmap, int mode) : _ospi()
{
    _ospi_io0 = pinmap.data0_pin;
    _ospi_io1 = pinmap.data1_pin;
    _ospi_io2 = pinmap.data2_pin;
    _ospi_io3 = pinmap.data3_pin;
    _ospi_io4 = pinmap.data4_pin;
    _ospi_io5 = pinmap.data5_pin;
    _ospi_io6 = pinmap.data6_pin;
    _ospi_io7 = pinmap.data7_pin;
    _ospi_clk = pinmap.sclk_pin;
    _ospi_cs = pinmap.ssel_pin;
    _ospi_dqs = pinmap.dqs_pin;
    _static_pinmap = &pinmap;
    _inst_width = OSPI_CFG_BUS_SINGLE;
    _inst_size = OSPI_CFG_INST_SIZE_8;
    _address_width = OSPI_CFG_BUS_SINGLE;
    _address_size = OSPI_CFG_ADDR_SIZE_24;
    _alt_width = OSPI_CFG_BUS_SINGLE;
    _alt_size = OSPI_CFG_ALT_SIZE_8;
    _data_width = OSPI_CFG_BUS_SINGLE;
    _num_dummy_cycles = 0;
    _mode = mode;
    _hz = ONE_MHZ;
    _initialized = false;
    _init_func = &OSPI::_initialize_direct;

    //Go ahead init the device here with the default config
    bool success = (this->*_init_func)();
    MBED_ASSERT(success);
}

ospi_status_t OSPI::configure_format(ospi_bus_width_t inst_width, ospi_inst_size_t inst_size,
                                     ospi_bus_width_t address_width, ospi_address_size_t address_size,
                                     ospi_bus_width_t alt_width, ospi_alt_size_t alt_size,
                                     ospi_bus_width_t data_width, int dummy_cycles)
{
    // Check that alt_size/alt_width are a valid combination
    uint8_t alt_lines = convert_bus_width_to_line_count(alt_width);
    if (alt_lines == 0) {
        return OSPI_STATUS_ERROR;
    } else if (alt_size % alt_lines != 0) {
        // Invalid alt size/width combination (alt size is not a multiple of the number of bus lines used to transmit it)
        return OSPI_STATUS_ERROR;
    }

    lock();
    _inst_width = inst_width;
    _inst_size = inst_size;
    _address_width = address_width;
    _address_size = address_size;
    _alt_width = alt_width;
    _alt_size = alt_size;
    _data_width = data_width;
    _num_dummy_cycles = dummy_cycles;
    unlock();

    return OSPI_STATUS_OK;
}

ospi_status_t OSPI::set_frequency(int hz)
{
    ospi_status_t ret_status = OSPI_STATUS_OK;

    if (_initialized) {
        lock();
        _hz = hz;
        if (OSPI_STATUS_OK != ospi_frequency(&_ospi, _hz)) {
            ret_status = OSPI_STATUS_ERROR;
        }
        unlock();
    } else {
        ret_status = OSPI_STATUS_ERROR;
    }

    return ret_status;
}

ospi_status_t OSPI::read(int address, char *rx_buffer, size_t *rx_length)
{
    ospi_status_t ret_status = OSPI_STATUS_ERROR;

    if (_initialized) {
        if ((rx_length != NULL) && (rx_buffer != NULL)) {
            if (*rx_length != 0) {
                lock();
                _build_ospi_command(OSPI_NO_INST, address, -1);
                if (OSPI_STATUS_OK == ospi_read(&_ospi, &_ospi_command, rx_buffer, rx_length)) {
                    ret_status = OSPI_STATUS_OK;
                }
                unlock();
            }
        } else {
            ret_status = OSPI_STATUS_INVALID_PARAMETER;
        }
    }

    return ret_status;
}

ospi_status_t OSPI::write(int address, const char *tx_buffer, size_t *tx_length)
{
    ospi_status_t ret_status = OSPI_STATUS_ERROR;

    if (_initialized) {
        if ((tx_length != NULL) && (tx_buffer != NULL)) {
            if (*tx_length != 0) {
                lock();
                _build_ospi_command(OSPI_NO_INST, address, -1);
                if (OSPI_STATUS_OK == ospi_write(&_ospi, &_ospi_command, tx_buffer, tx_length)) {
                    ret_status = OSPI_STATUS_OK;
                }
                unlock();
            }
        } else {
            ret_status = OSPI_STATUS_INVALID_PARAMETER;
        }
    }

    return ret_status;
}

ospi_status_t OSPI::read(ospi_inst_t instruction, int alt, int address, char *rx_buffer, size_t *rx_length)
{
    ospi_status_t ret_status = OSPI_STATUS_ERROR;

    if (_initialized) {
        if ((rx_length != NULL) && (rx_buffer != NULL)) {
            if (*rx_length != 0) {
                lock();
                _build_ospi_command(instruction, address, alt);
                if (OSPI_STATUS_OK == ospi_read(&_ospi, &_ospi_command, rx_buffer, rx_length)) {
                    ret_status = OSPI_STATUS_OK;
                }
                unlock();
            }
        } else {
            ret_status = OSPI_STATUS_INVALID_PARAMETER;
        }
    }

    return ret_status;
}

ospi_status_t OSPI::write(ospi_inst_t instruction, int alt, int address, const char *tx_buffer, size_t *tx_length)
{
    ospi_status_t ret_status = OSPI_STATUS_ERROR;

    if (_initialized) {
        if ((tx_length != NULL) && (tx_buffer != NULL)) {
            if (*tx_length != 0) {
                lock();
                _build_ospi_command(instruction, address, alt);
                if (OSPI_STATUS_OK == ospi_write(&_ospi, &_ospi_command, tx_buffer, tx_length)) {
                    ret_status = OSPI_STATUS_OK;
                }
                unlock();
            }
        } else {
            ret_status = OSPI_STATUS_INVALID_PARAMETER;
        }
    }

    return ret_status;
}

ospi_status_t OSPI::command_transfer(ospi_inst_t instruction, int address, const char *tx_buffer, size_t tx_length, const char *rx_buffer, size_t rx_length)
{
    ospi_status_t ret_status = OSPI_STATUS_ERROR;

    if (_initialized) {
        lock();
        _build_ospi_command(instruction, address, -1); //We just need the command
        if (OSPI_STATUS_OK == ospi_command_transfer(&_ospi, &_ospi_command, (const void *)tx_buffer, tx_length, (void *)rx_buffer, rx_length)) {
            ret_status = OSPI_STATUS_OK;
        }
        unlock();
    }

    return ret_status;
}

void OSPI::lock()
{
    _mutex->lock();
}

void OSPI::unlock()
{
    _mutex->unlock();
}

// Note: Private helper function to initialize ospi HAL
bool OSPI::_initialize()
{
    if (_mode != 0 && _mode != 1) {
        _initialized = false;
        return _initialized;
    }

    ospi_status_t ret = ospi_init(&_ospi, _ospi_io0, _ospi_io1, _ospi_io2, _ospi_io3, _ospi_io4, _ospi_io5, _ospi_io6, _ospi_io7, _ospi_clk, _ospi_cs, _ospi_dqs, _hz, _mode);
    if (OSPI_STATUS_OK == ret) {
        _initialized = true;
    } else {
        _initialized = false;
    }

    return _initialized;
}

// Note: Private helper function to initialize ospi HAL
bool OSPI::_initialize_direct()
{
    if (_mode != 0 && _mode != 1) {
        _initialized = false;
        return _initialized;
    }

    ospi_status_t ret = ospi_init_direct(&_ospi, _static_pinmap, _hz, _mode);
    if (OSPI_STATUS_OK == ret) {
        _initialized = true;
    } else {
        _initialized = false;
    }

    return _initialized;
}

void OSPI::_build_ospi_command(ospi_inst_t instruction, int address, int alt)
{
    memset(&_ospi_command, 0,  sizeof(ospi_command_t));
    //Set up instruction phase parameters
    _ospi_command.instruction.bus_width = _inst_width;
    if (instruction != OSPI_NO_INST) {
        _ospi_command.instruction.value = instruction;
        _ospi_command.instruction.disabled = false;
    } else {
        _ospi_command.instruction.disabled = true;
    }

    //Set up address phase parameters
    _ospi_command.address.bus_width = _address_width;
    _ospi_command.address.size = _address_size;
    if (address != -1) {
        _ospi_command.address.value = address;
        _ospi_command.address.disabled = false;
    } else {
        _ospi_command.address.disabled = true;
    }

    //Set up alt phase parameters
    _ospi_command.alt.bus_width = _alt_width;
    _ospi_command.alt.size = _alt_size;
    if (alt != -1) {
        _ospi_command.alt.value = alt;
        _ospi_command.alt.disabled = false;
    } else {
        _ospi_command.alt.disabled = true;
    }

    _ospi_command.dummy_count = _num_dummy_cycles;

    //Set up bus width for data phase
    _ospi_command.data.bus_width = _data_width;
}

} // namespace mbed

#endif
