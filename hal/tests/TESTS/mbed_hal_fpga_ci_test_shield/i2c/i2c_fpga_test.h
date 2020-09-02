/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

/** \addtogroup hal_GeneralI2C_tests */
/** @{*/

#ifndef MBED_FPGA_I2C_TEST_H
#define MBED_FPGA_I2C_TEST_H

#if DEVICE_I2C

#ifdef __cplusplus
extern "C" {
#endif

/** Test that the i2c-master can be initialized/de-initialized using all possible
 *  i2c pins.
 *
 * Given board provides i2c-master support.
 * When i2c-master is initialized (and then de-initialized) using valid set of i2c pins.
 * Then the operation is successfull.
 *
 */
void fpga_test_i2c_init_free(PinName sda, PinName scl);

/** Test that I2C master is able to read data from I2C bus using i2c_byte_read.
 *
 * Given board provides I2C master support.
 * When I2C master reads data from I2C bus using i2c_byte_read.
 * Then data is successfully read.
 *
 */
void fpga_i2c_test_byte_read(PinName sda, PinName scl);

/** Test that I2C master is able to write data to I2C bus using i2c_byte_write.
 *
 * Given board provides I2C master support.
 * When I2C master writes data to the I2C bus using i2c_byte_write.
 * Then data is successfully transmitted.
 *
 */
void fpga_i2c_test_byte_write(PinName sda, PinName scl);

/** Test that I2C master is able to read data from I2C bus using i2c_read.
 *
 * Given board provides I2C master support.
 * When I2C master reads data from I2C bus using i2c_read.
 * Then data is successfully read.
 *
 */
void fpga_i2c_test_read(PinName sda, PinName scl);

/** Test that I2C master is able to write data to I2C bus using i2c_write.
 *
 * Given board provides I2C master support.
 * When I2C master writes data to the I2C bus using i2c_write.
 * Then data is successfully transmitted.
 *
 */
void fpga_i2c_test_write(PinName sda, PinName scl);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
