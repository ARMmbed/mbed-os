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

/** \addtogroup hal_analogin_tests */
/** @{*/

#ifndef MBED_FPGA_ANALOG_IN_TEST_H
#define MBED_FPGA_ANALOG_IN_TEST_H

#if DEVICE_ANALOGIN

#ifdef __cplusplus
extern "C" {
#endif

/** Test that the alalogin can be initialized using all possible analogin pins.
 *
 * Given board provides analogin support.
 * When analogin is initialized using valid analogin pin.
 * Then the operation is successfull.
 *
 */
void fpga_analogin_init_test(PinName pin);

/** Test that analogin correctly interprets given input voltage.
 *
 * Given board provides analogin support.
 * When 0.0/3.3 V is provided to analogin pin.
 * Then analogin_read returns 0.0/1.0,
 *      analogin_read_u16 returns 0/65535.
 *
 */
void fpga_analogin_test(PinName pin);


/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
