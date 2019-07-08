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

#ifndef MBED_FPGA_SLAVE_SPI_TEST_H
#define MBED_FPGA_SLAVE_SPI_TEST_H

#if DEVICE_SPI

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BUFFERS_COMMON,     // common case rx/tx buffers are defined and have the same size
    BUFFERS_TX_UNDEF,   // tx buffer is undefined
    BUFFERS_RX_UNDEF,   // rx buffer is undefined
    BUFFERS_TX_GT_RX,   // tx buffer length is greater than rx buffer length
    BUFFERS_TX_LT_RX,   // tx buffer length is less than rx buffer length
    BUFFERS_ONE_SYM, // one symbol only is transmitted in both directions
} test_buffers_t;

typedef struct {
    PinName mosi;
    PinName miso;
    PinName sclk;
    PinName ssel;
    SPITester::SpiMode spi_mode;
    SPITester::SpiBitOrder spi_bit_order;
    uint32_t sym_size;
    uint32_t spi_freq;
    test_buffers_t test_buffers;
    bool sync_mode;
    SPITester::SpiDuplex duplex;
} test_config_t;

/** Test that the spi-Slave can be initialized/de-initialized using all possible
 *  SPI pins.
 *
 * Given board provides SPI-Slave support.
 * When SPI-Slave is initialized (and then de-initialized) using valid set of SPI pins.
 * Then the operation is successfull.
 *
 */
void fpga_spi_slave_test_init_free(PinName mosi, PinName miso, PinName sclk, PinName cs);

/** Test that the SPI-Slave transfer can be performed in various configurations.
 *
 * Given board provides SPI-Slave support.
 * When SPI transmission is performed using different settings.
 * Then data is successfully transferred.
 *
 */
void fpga_spi_slave_test_common(const test_config_t *tc_config);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
