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

/** \addtogroup hal_GeneralSerial_tests */
/** @{*/

#ifndef MBED_FPGA_UART_TEST_H
#define MBED_FPGA_UART_TEST_H

#if DEVICE_SERIAL

#ifdef __cplusplus
extern "C" {
#endif

/** Test that the uart can be initialized/de-initialized using all possible
 *  uart pins (flow control enabled).
 *
 * Given board provides uart support with flow control.
 * When uart is initialized (and then de-initialized) using valid set of uart pins.
 * Then the operation is successfull.
 *
 */
void fpga_uart_init_free_test(PinName tx, PinName rx, PinName cts = NC, PinName rts = NC);

/** Test that the uart can be initialized/de-initialized using all possible
 *  uart pins (flow control disabled).
 *
 * Given board provides uart support without flow control.
 * When uart is initialized (and then de-initialized) using valid set of uart pins.
 * Then the operation is successfull.
 *
 */
void fpga_uart_init_free_test_no_fc(PinName tx, PinName rx);

/** Test that the uart transfer can be performed in various configurations (flow control enabled).
 *
 * Given board provides uart support with flow control.
 * When uart transmission is performed using different settings.
 * Then data is successfully transfered.
 *
 */
void fpga_uart_test_common(PinName tx, PinName rx, PinName cts = NC, PinName rts = NC);

/** Test that the uart transfer can be performed in various configurations (flow control disabled).
 *
 * Given board provides uart support without flow control.
 * When uart transmission is performed using different settings.
 * Then data is successfully transfered.
 *
 */
void fpga_uart_test_common_no_fc(PinName tx, PinName rx);

/* Common test function. */
static void uart_test_common(int baudrate, int data_bits, SerialParity parity, int stop_bits, bool init_direct, PinName tx, PinName rx, PinName cts = NC, PinName rts = NC);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
