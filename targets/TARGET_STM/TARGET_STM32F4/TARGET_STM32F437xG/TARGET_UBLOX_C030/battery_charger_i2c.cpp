/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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

#include "battery_charger_i2c.h"

BatteryChargerI2c::BatteryChargerI2c(PinName sda, PinName scl):_i2c(), _hz(100000)
{
    i2c_init(&_i2c, sda, scl);
}

bool BatteryChargerI2c::read_from_i2c(int i2c_address, char* data_read, int length)
{
    int bytes_read = i2c_read(&_i2c, i2c_address, data_read, length, 1);
    return (length == bytes_read);
}

bool BatteryChargerI2c::write_to_i2c(int i2c_address, const char* data_write, int length)
{
    int bytes_written = i2c_write(&_i2c, i2c_address, data_write, length, 1);
    return (length == bytes_written);
}
