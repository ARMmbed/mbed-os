/* Simple access class for I2C EEPROM chips like Microchip 24LC
 * Copyright (c) 2015 Robin Hourahane
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
#include "I2CEEBlockDevice.h"
#include "rtos/ThisThread.h"
using namespace mbed;

#define I2CEE_TIMEOUT 10000


I2CEEBlockDevice::I2CEEBlockDevice(
    PinName sda, PinName scl, uint8_t addr,
    bd_size_t size, bd_size_t block, int freq,
    bool address_is_eight_bit)
    : _i2c_addr(addr)
    , _address_is_eight_bit(address_is_eight_bit)
    , _size(size)
    , _block(block)
{
    _i2c = new (_i2c_buffer) I2C(sda, scl);
    _i2c->frequency(freq);
}

I2CEEBlockDevice::I2CEEBlockDevice(
    I2C *i2c_obj, uint8_t addr,
    bd_size_t size, bd_size_t block,
    bool address_is_eight_bit)
    : _i2c_addr(addr)
    , _address_is_eight_bit(address_is_eight_bit)
    , _size(size)
    , _block(block)
{
    _i2c = i2c_obj;
}
I2CEEBlockDevice::~I2CEEBlockDevice()
{
    if (_i2c == (I2C *)_i2c_buffer) {
        _i2c->~I2C();
    }
}

int I2CEEBlockDevice::init()
{
    return _sync();
}

int I2CEEBlockDevice::deinit()
{
    return 0;
}

int I2CEEBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    // Check the address and size fit onto the chip.
    MBED_ASSERT(is_valid_read(addr, size));

    auto *pBuffer = static_cast<char *>(buffer);

    _i2c->start();

    if (1 != _i2c->write(get_paged_device_address(addr))) {
        return BD_ERROR_DEVICE_ERROR;
    }

    if (!_address_is_eight_bit && 1 != _i2c->write((char)(addr >> 8u))) {
        return BD_ERROR_DEVICE_ERROR;
    }

    if (1 != _i2c->write((char)(addr & 0xffu))) {
        return BD_ERROR_DEVICE_ERROR;
    }

    _i2c->stop();

    if (0 != _i2c->read(_i2c_addr, pBuffer, size)) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return BD_ERROR_OK;
}

int I2CEEBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    // Check the addr and size fit onto the chip.
    MBED_ASSERT(is_valid_program(addr, size));

    const auto *pBuffer = static_cast<const char *>(buffer);

    // While we have some more data to write.
    while (size > 0) {
        uint32_t off = addr % _block;
        uint32_t chunk = (off + size < _block) ? size : (_block - off);

        _i2c->start();

        if (1 != _i2c->write(get_paged_device_address(addr))) {
            return BD_ERROR_DEVICE_ERROR;
        }

        if (!_address_is_eight_bit && 1 != _i2c->write((char)(addr >> 8u))) {
            return BD_ERROR_DEVICE_ERROR;
        }

        if (1 != _i2c->write((char)(addr & 0xffu))) {
            return BD_ERROR_DEVICE_ERROR;
        }

        for (unsigned i = 0; i < chunk; i++) {
            if (1 != _i2c->write(pBuffer[i])) {
                return BD_ERROR_DEVICE_ERROR;
            }
        }

        _i2c->stop();

        int err = _sync();

        if (err) {
            return err;
        }

        addr += chunk;
        size -= chunk;
        pBuffer += chunk;
    }

    return BD_ERROR_OK;
}

int I2CEEBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    // No erase needed
    return 0;
}

int I2CEEBlockDevice::_sync()
{
    // The chip doesn't ACK while writing to the actual EEPROM
    // so loop trying to do a zero byte write until it is ACKed
    // by the chip.
    for (int i = 0; i < I2CEE_TIMEOUT; i++) {
        if (_i2c->write(_i2c_addr | 0, 0, 0) < 1) {
            return 0;
        }

        rtos::ThisThread::sleep_for(1);
    }

    return BD_ERROR_DEVICE_ERROR;
}

bd_size_t I2CEEBlockDevice::get_read_size() const
{
    return 1;
}

bd_size_t I2CEEBlockDevice::get_program_size() const
{
    return 1;
}

bd_size_t I2CEEBlockDevice::get_erase_size() const
{
    return 1;
}

bd_size_t I2CEEBlockDevice::size() const
{
    return _size;
}

const char *I2CEEBlockDevice::get_type() const
{
    return "I2CEE";
}

uint8_t I2CEEBlockDevice::get_paged_device_address(bd_addr_t address)
{
    if (!_address_is_eight_bit) {
        return _i2c_addr;
    } else {
        // Use the three least significant bits of the 2nd byte as the page
        // The page will be bits 2-4 of the user defined addresses.
        return _i2c_addr | ((address & 0x0700u) >> 7u);
    }
}

