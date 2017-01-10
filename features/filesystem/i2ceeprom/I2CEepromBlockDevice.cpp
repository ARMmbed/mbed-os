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
#include "I2CEepromBlockDevice.h"
 
I2CEepromBlockDevice::I2CEepromBlockDevice(PinName sda, PinName scl, uint8_t address,
        bd_size_t block_size, bd_count_t block_count, int bus_speed)
    : _i2c(sda, scl), _i2c_address(address)
    , _block_size(block_size), _block_count(block_count)
{
    _i2c.frequency(bus_speed);
}

bd_count_or_error_t I2CEepromBlockDevice::read(void *buffer, bd_block_t block, bd_count_t count)
{
    // Check the address and size fit onto the chip.
    if (block + count > _block_count) {
        return BD_ERROR_PARAMETER;
    }

    uint16_t address = block*_block_size;
    _i2c.start();
    if (!_i2c.write(_i2c_address | 0) ||
        !_i2c.write((char)(address >> 8)) ||
        !_i2c.write((char)(address & 0xff))) {
        return BD_ERROR_DEVICE_ERROR;
    }
    _i2c.stop();

    if (_i2c.read(_i2c_address, static_cast<char*>(buffer), count*_block_size) < 0) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return count;
}
 
bd_count_or_error_t I2CEepromBlockDevice::write(const void *buffer, bd_block_t block, bd_count_t count)
{
    // Check the address and size fit onto the chip.
    if (block + count > _block_count) {
        return BD_ERROR_PARAMETER;
    }
        
    // While we have some more data to write.
    for (bd_block_t b = 0; b < count; b++) {
        uint16_t address = block*_block_size + b*_block_size;
        _i2c.start();
        if (!_i2c.write(_i2c_address | 0) ||
            !_i2c.write((char)(address >> 8)) ||
            !_i2c.write((char)(address & 0xff))) {
            return BD_ERROR_DEVICE_ERROR;
        }

        for (bd_size_t i = 0; i < _block_size; i++) {
            _i2c.write(static_cast<const char*>(buffer)[b*_block_size + i]);
        }
        _i2c.stop();

        bd_error_t err = sync();
        if (err) {
            return err;
        }
    }

    return count;
}

bd_error_t I2CEepromBlockDevice::sync()
{
    // The chip doesn't ACK while writing to the actual EEPROM
    // so loop trying to do a zero byte write until it is ACKed
    // by the chip.
    while (true) {
        _i2c.start();
        int ack = _i2c.write(_i2c_address | 0);
        _i2c.stop();
        if (ack) {
            return 0;
        }

        wait_ms(1);
    }
}
 
bd_size_t I2CEepromBlockDevice::get_block_size()
{
    return _block_size;
}

bd_count_t I2CEepromBlockDevice::get_block_count()
{
    return _block_count;
}
