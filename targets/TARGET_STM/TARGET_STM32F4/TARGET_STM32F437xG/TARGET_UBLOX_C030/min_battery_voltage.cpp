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

#include "min_battery_voltage.h"
#include "battery_charger_i2c.h"

/** Defining HAL_MspInit strong function
 * in user defined file as described in documentation
 */

void HAL_MspInit(void)
{
	set_minimum_battery_voltage();
}

void set_minimum_battery_voltage()
{
    char data_write[2] = {0};
    char data_read;
    BatteryChargerI2c i2c_object(I2C_SDA_B, I2C_SCL_B);

    if (i2c_object.write_to_i2c(BQ24295_I2C_ADDRESS,&data_write[0] , 1)){
        i2c_object.read_from_i2c(BQ24295_I2C_ADDRESS, &data_read, 1);
        data_read = data_read & MIN_BATTERY_VOLTAGE_MASK;
        data_write[0] = 0x0;
        data_write[1] = data_read;
        if (i2c_object.write_to_i2c(BQ24295_I2C_ADDRESS,&data_write[0] , 2)){
            //Battery Voltage is set to 3880mV
            }
        }
    else{
        // Minimum battery voltage could not be set.  This is not a critical error, no need to stop execution
        // It simply means that longer cabling or USB ports with lower output voltages may cause problems.
    }
}
