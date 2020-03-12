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

#ifndef MIN_BATTERY_VOLTAGE_H
#define MIN_BATTERY_VOLTAGE_H

#include "hal/i2c_api.h"

#ifdef __cplusplus
extern"C" {
#endif

#define BQ24295_I2C_ADDRESS (0x6B)
#define I2C_FREQUENCY 100000
#define MIN_BATTERY_VOLTAGE_MASK (0x87)

/** Initializes an instance of class BatteryChargerI2c which is using the STM HAL I2C APIs
 *  This allows longer USB cables or USB ports with lower output voltages to power the board correctly.
 */

void set_minimum_battery_voltage(void);
char read_from_i2c(int slave_addr, int reg_addr, int *data_read, i2c_t i2c_obj);
char write_to_i2c(int slave_addr, int reg_addr, int data_write, i2c_t i2c_obj);

#ifdef __cplusplus
}
#endif

#endif  // MIN_BATTERY_VOLTAGE_H
