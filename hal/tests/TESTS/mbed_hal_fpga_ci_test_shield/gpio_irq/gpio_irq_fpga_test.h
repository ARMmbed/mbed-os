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

/** \addtogroup hal_gpioirq_tests */
/** @{*/

#ifndef MBED_FPGA_GPIO_IRQ_TEST_H
#define MBED_FPGA_GPIO_IRQ_TEST_H

#if DEVICE_INTERRUPTIN

#ifdef __cplusplus
extern "C" {
#endif

/** Test that the GPIO IRQ can be initialized/de-initialized using all possible
 *  GPIO IRQ pins.
 *
 * Given board provides GPIO IRQ support.
 * When GPIO IRQ is initialized (and then de-initialized) using valid GPIO IRQ pin.
 * Then the operation is successfull.
 *
 */
void fpga_gpio_irq_test(PinName pin);

/** Test that the gpio interrupt is generated correctly.
 *
 * Given board provides interrupt-in feature.
 * When gpio interrupt is configured to fire on rasing/falling/both edge(s).
 * Then on rasing/falling/any edge registered interrupt handler is called.
 *
 */
void fpga_gpio_irq_init_free_test(PinName pin);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif
/**@}*/
