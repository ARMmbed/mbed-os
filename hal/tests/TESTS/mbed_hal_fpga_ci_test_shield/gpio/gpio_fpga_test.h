/* mbed Microcontroller Library
 * Copyright (c) 2019-2020 ARM Limited
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

/** \addtogroup hal_gpio_tests */
/** @{*/

#ifndef MBED_FPGA_GPIO_TEST_H
#define MBED_FPGA_GPIO_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

/* Test basic input & output operations.
 *
 * Given a GPIO instance initialized with a generic gpio_init() function,
 * when basic input and output operations are performed,
 * then all operations succeed.
 */
void fpga_test_basic_input_output(PinName pin);

/* Test input pull modes.
 *
 * Given a GPIO instance configured with an input pull mode,
 * when basic input operations are performed,
 * then all operations succeed.
 */
void fpga_test_input_pull_modes(PinName pin);

/* Test explicit input initialization.
 *
 * Given a GPIO instance,
 * when additional parameters are passed to the input init function,
 * then the GPIO is correctly initialized as an input.
 */
void fpga_test_explicit_input(PinName pin);

/* Test explicit output initialization.
 *
 * Given a GPIO instance,
 * when additional parameters are passed to the output init function,
 * then the GPIO is correctly initialized as an output.
 */
void fpga_test_explicit_output(PinName pin);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif


/**@}*/
