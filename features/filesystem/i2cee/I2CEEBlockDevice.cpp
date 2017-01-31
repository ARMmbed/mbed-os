/* Simple access class for I2C EEPROM chips like Microchip 24LC
 * Copyright (c) 2015 Robin Hourahane
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

#define I2CEE_TIMEOUT 10000
 

I2CEEBlockDevice::I2CEEBlockDevice(
        PinName sda, PinName scl, uint8_t addr,
        bd_size_t size, bd_size_t block, int freq)
    : _i2c(sda, scl), _i2c_addr(addr), _size(size), _block(block)
{
    _i2c.frequency(freq);
}

bd_error_t I2CEEBlockDevice::init()
{
    return _sync();
}

bd_error_t I2CEEBlockDevice::deinit()
{
    return 0;
}

bd_error_t I2CEEBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    // Check the address and size fit onto the chip.
    if (!is_valid_read(addr, size)) {
        return BD_ERROR_PARAMETER;
    }

    _i2c.start();
    if (!_i2c.write(_i2c_addr | 0) ||
        !_i2c.write((char)(addr >> 8)) ||
        !_i2c.write((char)(addr & 0xff))) {
        return BD_ERROR_DEVICE_ERROR;
    }
    _i2c.stop();

    if (_i2c.read(_i2c_addr, static_cast<char*>(buffer), size) < 0) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return 0;
}
 
bd_error_t I2CEEBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    // Check the addr and size fit onto the chip.
    if (!is_valid_program(addr, size)) {
        return BD_ERROR_PARAMETER;
    }
        
    // While we have some more data to write.
    while (size > 0) {
        _i2c.start();
        if (!_i2c.write(_i2c_addr | 0) ||
            !_i2c.write((char)(addr >> 8)) ||
            !_i2c.write((char)(addr & 0xff))) {
            return BD_ERROR_DEVICE_ERROR;
        }

        for (unsigned i = 0; i < _block; i++) {
            _i2c.write(static_cast<const char*>(buffer)[i]);
        }
        _i2c.stop();

        bd_error_t err = _sync();
        if (err) {
            return err;
        }

        addr += _block;
        size -= _block;
        buffer = static_cast<const char*>(buffer) + _block;
    }

    return 0;
}

bd_error_t I2CEEBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    // No erase needed
    return 0;
}

bd_error_t I2CEEBlockDevice::_sync()
{
    // The chip doesn't ACK while writing to the actual EEPROM
    // so loop trying to do a zero byte write until it is ACKed
    // by the chip.
    for (int i = 0; i < I2CEE_TIMEOUT; i++) {
        if (_i2c.write(_i2c_addr | 0, 0, 0) < 1) {
            return 0;
        }

        wait_ms(1);
    }

    return BD_ERROR_DEVICE_ERROR;
}
 
bd_size_t I2CEEBlockDevice::get_read_size()
{
    return 1;
}

bd_size_t I2CEEBlockDevice::get_program_size()
{
    return _block;
}

bd_size_t I2CEEBlockDevice::get_erase_size()
{
    return _block;
}

bd_size_t I2CEEBlockDevice::size()
{
    return _size;
}
