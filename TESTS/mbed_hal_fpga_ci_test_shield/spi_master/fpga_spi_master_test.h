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

/** \addtogroup hal_spi_tests */
/** @{*/

#ifndef MBED_FPGA_MASTER_SPI_TEST_H
#define MBED_FPGA_MASTER_SPI_TEST_H

#if DEVICE_SPI

#ifdef __cplusplus
extern "C" {
#endif

/** Test that the spi-Master can be initialized/de-initialized using all possible
 *  SPI pins.
 *
 * Given board provides SPI-Master support.
 * When SPI-Master is initialized (and then de-initialized) using valid set of SPI pins.
 * Then the operation is successfull.
 *
 */
void fpga_spi_master_test_init_free(PinName mosi, PinName miso, PinName sclk);

/** Test that the spi_get_capabilities() indicates that slave mode is unsupported if the given `ssel`
 * pin cannot be managed by hardware.
 *
 * Given board provides SPI support.
 * When `ssel` pin passed to spi_get_capabilities() cannot be managed by hardware.
 * Then capabilities should indicate that slave mode is unsupported.
 *
 */
void fpga_spi_test_capabilities_ssel(PinName mosi, PinName miso, PinName sclk);

/** Test that the spi_frequency() returns actual SPI frequency.
 *
 * Given board provides SPI-Master support.
 * When specified frequency is selected by means of spi_frequency().
 * Then frequency returned by spi_frequency() is used for the transmission.
 *
 * Additionally we check that spi_format() updates the configuration of the peripheral (format)
 * except the baud rate generator.
 */
void fpga_spi_master_test_freq(PinName mosi, PinName miso, PinName sclk);

/** Test that the SPI-Master transfer can be performed in various configurations.
 *
 * Given board provides SPI-Master support.
 * When SPI transmission is performed using different settings.
 * Then data is successfully transfered.
 *
 * Additionally we check that spi_frequency() updates the baud rate generator
 * leaving other configurations (format) unchanged.
 */
void fpga_spi_master_test_common(PinName mosi, PinName miso, PinName sclk);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
