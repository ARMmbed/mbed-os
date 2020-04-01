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

/** Defining HAL_MspInit strong function
 * in user defined file as described in documentation
 */

void HAL_MspInit(void)
{
    set_minimum_battery_voltage();
}

void set_minimum_battery_voltage()
{
    i2c_t i2c_obj = {0};
    int data_read;
    i2c_init(&i2c_obj, I2C_SDA_B, I2C_SCL_B);
    i2c_frequency(&i2c_obj, I2C_FREQUENCY);

    if (read_from_i2c(BQ24295_I2C_ADDRESS, 0, &data_read, i2c_obj)) {
        data_read = data_read & MIN_BATTERY_VOLTAGE_MASK;
        write_to_i2c(BQ24295_I2C_ADDRESS, 0, data_read, i2c_obj);
        //Battery Voltage is set to 3880mV
    } else {
        // Minimum battery voltage could not be set.  This is not a critical error, no need to stop execution
        // It simply means that longer cabling or USB ports with lower output voltages may cause problems.
    }
}

char write_to_i2c(int slave_addr, int reg_addr, int data_write, i2c_t i2c_obj)
{
    char ret_code = 0;
    if (!i2c_start(&i2c_obj)) {
        if ((i2c_byte_write(&i2c_obj, slave_addr << 1) == 1) &&
                (i2c_byte_write(&i2c_obj, reg_addr) == 1) &&
                (i2c_byte_write(&i2c_obj, data_write) == 1)) {
            ret_code = 1;
        }
        i2c_stop(&i2c_obj);
    }
    return ret_code;
}

char read_from_i2c(int slave_addr, int reg_addr, int *data_read, i2c_t i2c_obj)
{
    char ret_code = 0;
    if (!i2c_start(&i2c_obj)) {
        if ((i2c_byte_write(&i2c_obj, (slave_addr << 1)) == 1) &&
                (i2c_byte_write(&i2c_obj, reg_addr) == 1) &&
                (!i2c_start(&i2c_obj)) &&
                (i2c_byte_write(&i2c_obj, ((slave_addr << 1) | 0x01)) == 1)) {
            *data_read = i2c_byte_read(&i2c_obj, 1);
            ret_code = 1;
        }
        i2c_stop(&i2c_obj);
    }
    return ret_code;
}
