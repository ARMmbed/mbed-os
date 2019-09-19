/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** \addtogroup hal_i2c_tests
 *  @{
 */

#ifndef I2C_API_TESTS_H
#define I2C_API_TESTS_H

enum TransferType {
    WRITE = 0,
    READ = 1,
    WRITE_READ = 2
};

#ifdef __cplusplus
extern "C" {
#endif

/** Test that i2c_get_capabilities() fills given i2c_capabilities_t instance with capabilities.
 *
 * Given is platform with I2C support.
 * When i2c_get_capabilities() is called.
 * Then function fills the given i2c_capabilities_t instance.
 */
void fpga_i2c_test_get_capabilities();

/** Test that i2c_init() successfully initializes and i2c_free() successfully frees i2c interface.
 *
 * Given is platform with I2C support.
 * When i2c_init() is called with the valid pins configuration and then i2c_free() is called on
 *       the created I2C object.
 * Then both operations are successfully performed.
 */
void fpga_i2c_test_init_free(PinName sda, PinName scl);

/** Test that i2c_frequency() sets the frequency used during the I2C transfer.
 *
 *  Given is platform with I2C support.
 *  When i2c_frequency() is called and valid frequency is specified.
 *  Then function is executed successfully and actual frequency which will be used during the transfer is returned.
 *
 *  @note Frequency must be in range specified by the capabilities of the I2C peripheral.
 */
void fpga_i2c_test_frequency(PinName sda, PinName scl);

/** Test blocking transmission timeout.
 *
 * Given board provides I2C-Master support.
 * When I2C transmission is performed using different settings.
 * Then data is successfully transfered.
 */
void fpga_i2c_test_blocking_transmission_timeout(PinName sda, PinName scl, uint32_t frequency);

/** Test blocking transmission when no slave.
 *
 * Given board provides I2C-Master support.
 * When I2C transmission with address of nonexistent slave is performed using
 * Then transfer fails returning appropriate error.
 */
void fpga_i2c_test_transmission_no_slave(PinName sda, PinName scl, uint32_t frequency);

/** Test blocking transmission.
 *
 * Given board provides I2C-Master support.
 * When I2C transmission is performed using different settings.
 * Then data is successfully transfered.
 */
void fpga_i2c_test_blocking_write_read(PinName sda, PinName scl, uint16_t addr, uint32_t frequency, TransferType ttype, uint32_t data_size, uint32_t transfer_count, bool stop_each);

/** Test asynchronous transmission.
 *
 * Given board provides I2C-Master support.
 * When I2C transmission is performed using different settings.
 * Then data is successfully transfered.
 */
void fpga_i2c_test_async_write_read(PinName sda, PinName scl, uint16_t addr, uint32_t frequency, TransferType ttype, uint32_t data_size, uint32_t transfer_count, bool stop_each);

/** Test that @i2c_abort_async call aborts ongoing asynchronous transfer.
 *
 * Given is asynchronous transfer ongoing.
 * When call @i2c_abort_async.
 * Then transfer is aborted,
 *      and callback doesn't triggers
 *      and stop condition is generated
 */
void fpga_i2c_test_async_abort(PinName sda, PinName scl, uint32_t frequency);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
