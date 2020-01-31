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

/** \addtogroup hal_GeneralSPI_tests */
/** @{*/

#ifndef MBED_FPGA_SPI_TEST_H
#define MBED_FPGA_SPI_TEST_H

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
void fpga_spi_test_init_free(PinName mosi, PinName miso, PinName sclk, PinName ssel);

/** Test that the SPI-Master transfer can be performed in various configurations (SSEL handled by hardware).
 *
 * Given board provides SPI-Master support.
 * When SPI transmission is performed using different settings.
 * Then data is successfully transferred.
 *
 */
void fpga_spi_test_common(PinName mosi, PinName miso, PinName sclk, PinName ssel);

/** Test that the SPI-Master transfer can be performed in various configurations (SSEL handled manually).
 *
 * Given board provides SPI-Master support.
 * When SPI transmission is performed using different settings.
 * Then data is successfully transferred.
 *
 */
void fpga_spi_test_common_no_ss(PinName mosi, PinName miso, PinName sclk);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
