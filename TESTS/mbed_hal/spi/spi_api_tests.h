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

/** \addtogroup hal_spi_tests */
/** @{*/

#ifndef MBED_SPI_API_TESTS_H
#define MBED_SPI_API_TESTS_H

#include "device.h"

#if DEVICE_SPI

#ifdef __cplusplus
extern "C" {
#endif

/** Test that spi_get_module() returns the SPIName - unique identifier to the peripheral associated
 *  to this SPI channel.
 *
 *  Given is platform with SPI support.
 *  When spi_get_module() is called with valid SPI pins.
 *  Then function returns the SPIName - unique identifier to the peripheral associated
 *  to this SPI channel.
 *
 *  @note `mosi` or `miso` pin can be undefined (but not both). `mclk` must be defined.
 */
void test_get_module();

/** Test that spi_get_capabilities() fills the given spi_capabilities_t instance with capabilities
 *  of the specified SPI peripheral.
 *
 *  Given is platform with SPI support.
 *  When spi_get_capabilities() is called with the valid SPI name (and SS pin).
 *  Then function fills the given spi_capabilities_t instance with capabilities
 *  of the specified SPI peripheral.
 *
 *  @note spi_get_capabilities() considers the `ssel` pin when evaluation the
 *        support_slave_mode capability.
 *
 */
void test_get_capabilities();

/** Test that spi_init() successfully initializes the pins and spi_free() can successfully
 *  reset the pins to their default state.
 *
 *  Given is platform with SPI support.
 *  When spi_init() is called with the valid pins configuration and then spi_free() is called on
 *       the created SPI object.
 *  Then both operations are successfully performed.
 *
 */
void test_init_free();

/** Test that spi_format() sets/updates the transmission format of the SPI peripheral.
 *
 *  Given is platform with SPI support.
 *  When spi_format() is called and valid format is specified.
 *  Then function is executed successfully.
 *
 */
void test_set_format();

/** Test that test_set_frequency() sets the frequency used during the SPI transfer.
 *
 *  Given is platform with SPI support.
 *  When test_set_frequency() is called and valid frequency is specified.
 *  Then function is executed successfully and actual frequency which will be used during the transfer is returned.
 *
 *  @note Frequency can be only set by SPI peripheral operating in master mode.
 *  @note Frequency must be in range specified by the capabilities of the SPI peripheral.
 *
 */
void test_set_frequency();

/** Test that spi_transfer() can successfully perform transfer in master mode
 *  (TX/RX buffers are defined and have the same sizes) and returns the number of
 *  symbols clocked on the bus during this transfer.
 *
 *  Given is platform with SPI support.
 *  When spi_transfer() is called for SPI peripheral operating in master mode and
 *       both TX/RX buffers are specified and have equal sizes.
 *  Then function is executed successfully and returns number of symbols clocked on the bus during this transfer.
 *
 */
void test_transfer_master();

/** Test that spi_transfer() can successfully perform transfer in master mode
 *  (TX/RX buffers are undefined or have different sizes) and returns the number of
 *  symbols clocked on the bus during this transfer.
 *
 *  Given is platform with SPI support.
 *  When spi_transfer() is called for SPI peripheral operating in master mode and
 *       TX/RX buffers are undefined or have different sizes.
 *  Then function is executed successfully and returns number of symbols clocked on the bus during this transfer.
 *
 */
void test_transfer_master_fill_sym();

/** Test that spi_transfer_async() can successfully perform asynchronous transfer
 *  in master mode.
 *
 *  Given is platform with SPI support.
 *  When spi_transfer_async() is called for SPI peripheral operating in master mode.
 *  Then function returns true on success, performs transfer and invokes specified callback
 *       when the transfer completes (passing the operation status).
 *
 */
void test_transfer_master_async();

/** Test that spi_transfer_async_abort() can successfully abort an on-going async transfer.
 *
 *  Given is platform with SPI support.
 *  When spi_transfer_async() is called for SPI peripheral operating in master mode and
 *       operation is aborted immediately by means of spi_transfer_async_abort().
 *  Then operation is successfully performed and specified callback is invoked with
 *       the status which indicates that the transfer has been aborted.
 *
 */
void test_transfer_master_async_abort();

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
