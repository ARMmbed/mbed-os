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

#ifndef BATTERY_CHARGER_I2C
#define BATTERY_CHARGER_I2C

#include "hal/i2c_api.h"

#ifdef __cplusplus
extern"C"{
#endif

class BatteryChargerI2c{

public:
    BatteryChargerI2c(PinName sda, PinName scl);
    bool read_from_i2c(int i2c_address, char *data_read, int length);
    bool write_to_i2c(int i2c_address, const char *data_write, int length);
    virtual ~BatteryChargerI2c() {}

private:
    i2c_t _i2c;
    int   _hz;
};

#ifdef __cplusplus
}
#endif

#endif //BATTERY_CHARGER_I2C
