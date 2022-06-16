/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
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

#include "platform/Callback.h"
#include "ONFINANDBlockDevice.h"
#include <string.h>
#include "rtos/ThisThread.h"

#ifndef MBED_CONF_MBED_TRACE_ENABLE
#define MBED_CONF_MBED_TRACE_ENABLE        0
#endif
#define ONFINAND_BLOCK_OFFSET              0x40000

#include "mbed_trace.h"
#define TRACE_GROUP "ONFINAND"

using namespace std::chrono;
using namespace mbed;

/********* Public API Functions *********/
/****************************************/
ONFINANDBlockDevice::ONFINANDBlockDevice(PinName d0, PinName d1, PinName d2, PinName d3, PinName d4, PinName d5, PinName d6,
                                         PinName d7, PinName add09, PinName add11, PinName add12, PinName ba1, PinName rdy, PinName csx):
    _onfi(d0, d1, d2, d3, d4, d5, d6, d7, add09, add11, add12, ba1, rdy, csx), _init_ref_count(0)
{
    _is_initialized = false;
}

int ONFINANDBlockDevice::init()
{
    int status = ONFINAND_BD_ERROR_OK;

    _mutex.lock();

    int result = _onfi.init();

    if (result) {
        tr_error("Initial Failed");
        status = ONFINAND_BD_ERROR_DEVICE_ERROR;
    }

    if (!_is_initialized) {
        _init_ref_count = 0;
    }

    _init_ref_count++;

    if (_init_ref_count != 1) {
        goto exit_point;
    }

    _is_initialized = true;

exit_point:
    _mutex.unlock();

    return status;
}

int ONFINANDBlockDevice::deinit()
{
    int status = ONFINAND_BD_ERROR_OK;

    _mutex.lock();

    if (!_is_initialized) {
        _init_ref_count = 0;
        _mutex.unlock();
        return status;
    }

    _init_ref_count--;

    if (_init_ref_count) {
        _mutex.unlock();
        return status;
    }

    int result = _onfi.deinit();

    if (result) {
        tr_error("Deinit Failed");
        status = ONFINAND_BD_ERROR_DEVICE_ERROR;
    }

    _is_initialized = false;

    _mutex.unlock();

    return status;
}

int ONFINANDBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    int status = ONFINAND_BD_ERROR_OK;
    tr_debug("Read - Buff: %p, addr: %llu, size: %llu", buffer, addr, size);

    _mutex.lock();

    int result = _onfi.read(addr, (uint8_t *)buffer, size);

    if (result) {
        tr_error("Read Failed");
        status = ONFINAND_BD_ERROR_DEVICE_ERROR;
    }

    _mutex.unlock();

    return status;
}

int ONFINANDBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    int status = ONFINAND_BD_ERROR_OK;
    tr_debug("Program - Buff: %p, addr: %llu, size: %llu", buffer, addr, size);

    _mutex.lock();

    int result = _onfi.write(addr, buffer, size);

    if (result) {
        tr_error("Program Failed");
        status = ONFINAND_BD_ERROR_DEVICE_ERROR;
    }

    _mutex.unlock();

    return status;
}

int ONFINANDBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    int status = ONFINAND_BD_ERROR_OK;
    tr_debug("Erase - addr: %llu, size: %llu", addr, size);

    if ((addr + size) > MBED_CONF_ONFINAND_ONFINAND_FLASH_SIZE) {
        tr_error("Erase exceeds flash device size");
        return ONFINAND_BD_ERROR_INVALID_ERASE_PARAMS;
    }

    if (((addr % ONFINAND_BLOCK_OFFSET) != 0) || ((size % get_erase_size()) != 0)) {
        tr_error("Invalid erase - unaligned address and size");
        return ONFINAND_BD_ERROR_INVALID_ERASE_PARAMS;
    }

    _mutex.lock();

    int result = _onfi.erase(addr, size);

    if (result) {
        status = ONFINAND_BD_ERROR_DEVICE_ERROR;
    }

    _mutex.unlock();

    return status;
}

bd_size_t ONFINANDBlockDevice::get_read_size() const
{
    // Return minimum read size in bytes for the device
    return MBED_CONF_ONFINAND_ONFINAND_MIN_READ_SIZE;
}

bd_size_t ONFINANDBlockDevice::get_program_size() const
{
    // Return minimum program size in bytes for the device
    return MBED_CONF_ONFINAND_ONFINAND_MIN_PROG_SIZE;
}

bd_size_t ONFINANDBlockDevice::get_erase_size() const
{
    return MBED_CONF_ONFINAND_ONFINAND_BLOCK_SIZE;
}

bd_size_t ONFINANDBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return MBED_CONF_ONFINAND_ONFINAND_BLOCK_SIZE;
}

const char *ONFINANDBlockDevice::get_type() const
{
    return "ONFINAND";
}

bd_size_t ONFINANDBlockDevice::size() const
{
    return MBED_CONF_ONFINAND_ONFINAND_FLASH_SIZE;
}

int ONFINANDBlockDevice::get_erase_value() const
{
    return 0xFF;
}
