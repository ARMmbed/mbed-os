/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

/** \addtogroup hal_spi_tests
 *  @{
 */

#ifndef MBED_SPI_COM_TEST_H
#define MBED_SPI_COM_TEST_H

#if DEVICE_SPI

#ifdef __cplusplus
extern "C" {
#endif

/** Test that the SPI transfer can be performed in various configurations.
 *
 * Given board provides at least 2 SPI peripherals, SPI slave and master mode.
 * When SPI transmission is performed using specified configuration.
 * Then data is successfully transfered.
 *
 */
void test_spi_transfer();

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
