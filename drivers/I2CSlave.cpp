/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
    i2c_init(&_i2c, sda, scl);
    i2c_frequency(&_i2c, 100000);
    i2c_slave_mode(&_i2c, 1);
}

void I2CSlave::address(int address)
{
    int addr = (address & 0xFF) | 1;
    i2c_slave_address(&_i2c, 0, addr, 0);
}

} // namespace mbed

#endif
