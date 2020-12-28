/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
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

