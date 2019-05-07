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

#if defined(TARGET_NUCLEO_F429ZI)

#define SPI_TEST_MASTER_CS     PA_4
#define SPI_TEST_MASTER_SCK    PA_5
#define SPI_TEST_MASTER_MISO   PA_6
#define SPI_TEST_MASTER_MOSI   PB_5
#define SPI_TEST_SLAVE_CS      PE_4
#define SPI_TEST_SLAVE_SCK     PE_2
#define SPI_TEST_SLAVE_MISO    PE_5
#define SPI_TEST_SLAVE_MOSI    PE_6

#elif defined(TARGET_K66F)

#define SPI_TEST_MASTER_CS     PTB20 // pin 3 on j6 header
#define SPI_TEST_MASTER_SCK    PTD12
#define SPI_TEST_MASTER_MISO   PTB23
#define SPI_TEST_MASTER_MOSI   PTD13
#define SPI_TEST_SLAVE_CS      PTD0
#define SPI_TEST_SLAVE_SCK     PTD1
#define SPI_TEST_SLAVE_MISO    PTD2
#define SPI_TEST_SLAVE_MOSI    PTD3

#else
#error [NOT_SUPPORTED] Provide SPI pins for the communication test in spi_com.h.
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
