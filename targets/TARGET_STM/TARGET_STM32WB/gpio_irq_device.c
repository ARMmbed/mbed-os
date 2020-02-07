/* mbed Microcontroller Library
 * Copyright (c) 2019 STMicroelectronics
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

#include "cmsis.h"
#include "gpio_irq_device.h"

// Used to return the index for channels array.
const exti_lines_t pin_lines_desc[16] = {
    // EXTI0
    {.gpio_idx = 0, .irq_index = 0, .irq_n = EXTI0_IRQn}, // pin 0
    // EXTI1
    {.gpio_idx = 0, .irq_index = 1, .irq_n = EXTI1_IRQn}, // pin 1
    // EXTI2
    {.gpio_idx = 0, .irq_index = 2, .irq_n = EXTI2_IRQn}, // pin 2
    // EXTI3
    {.gpio_idx = 0, .irq_index = 3, .irq_n = EXTI3_IRQn}, // pin 3
    // EXTI4
    {.gpio_idx = 0, .irq_index = 4, .irq_n = EXTI4_IRQn}, // pin 4
    // EXTI5_9
    {.gpio_idx = 0, .irq_index = 5, .irq_n = EXTI9_5_IRQn},// pin 5
    {.gpio_idx = 1, .irq_index = 5, .irq_n = EXTI9_5_IRQn},// pin 6
    {.gpio_idx = 2, .irq_index = 5, .irq_n = EXTI9_5_IRQn},// pin 7
    {.gpio_idx = 3, .irq_index = 5, .irq_n = EXTI9_5_IRQn},// pin 8
    {.gpio_idx = 4, .irq_index = 5, .irq_n = EXTI9_5_IRQn},// pin 9
    // EXTI10_15
    {.gpio_idx = 0, .irq_index = 6, .irq_n = EXTI15_10_IRQn},// pin 10
    {.gpio_idx = 1, .irq_index = 6, .irq_n = EXTI15_10_IRQn},// pin 11
    {.gpio_idx = 2, .irq_index = 6, .irq_n = EXTI15_10_IRQn},// pin 12
    {.gpio_idx = 3, .irq_index = 6, .irq_n = EXTI15_10_IRQn},// pin 13
    {.gpio_idx = 4, .irq_index = 6, .irq_n = EXTI15_10_IRQn},// pin 14
    {.gpio_idx = 5, .irq_index = 6, .irq_n = EXTI15_10_IRQn}// pin 15
};

