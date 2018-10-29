/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "drivers/I2CSlave.h"

#if DEVICE_I2CSLAVE

namespace mbed {

I2CSlave::I2CSlave(PinName sda, PinName scl) : _i2c()
{
    i2c_init(&_i2c, sda, scl, true);
    i2c_frequency(&_i2c, 100000);
}

void I2CSlave::frequency(int hz)
{
    i2c_frequency(&_i2c, hz);
}

void I2CSlave::address(int address)
{
    int addr = (address & 0xFF) | 1;

    i2c_slave_address(&_i2c, addr);
}

int I2CSlave::receive(void)
{
    return i2c_slave_status(&_i2c);
}

int I2CSlave::read(char *data, int length)
{
    return i2c_read(&_i2c, 0, data, length, false) != length;
}

int I2CSlave::read(void)
{
    int ret;
    i2c_read(&_i2c, 0, &ret, 1, false);

    return ret;
}

int I2CSlave::write(const char *data, int length)
{
    return i2c_write(&_i2c, 0, data, length, false) != length;
}

int I2CSlave::write(int data)
{
    return i2c_write(&_i2c, 0, (void *)&data, 1, false);
}

void I2CSlave::stop(void)
{
    i2c_stop(&_i2c);
}

} // namespace mbed

#endif // DEVICE_I2CSLAVE
