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

#ifndef MBED_GPIO_IRQ_DEVICE_H
#define MBED_GPIO_IRQ_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32wbxx_ll_exti.h"

// Number of EXTI irq vectors (EXTI0, EXTI1, EXTI2, EXTI3, EXTI4, EXTI5_9, EXTI10_15)
#define CHANNEL_NUM (7)

#define EXTI_IRQ0_NUM_LINES 1
#define EXTI_IRQ1_NUM_LINES 1
#define EXTI_IRQ2_NUM_LINES 1
#define EXTI_IRQ3_NUM_LINES 1
#define EXTI_IRQ4_NUM_LINES 1
#define EXTI_IRQ5_NUM_LINES 5
#define EXTI_IRQ6_NUM_LINES 6

// Max pins for one line (max with EXTI10_15)
#define MAX_PIN_LINE (EXTI_IRQ6_NUM_LINES)

/*  Structure to describe how the HW EXTI lines are defined in this HW */
typedef struct exti_lines {
    uint32_t gpio_idx;   // an index entry for each EXIT line
    uint32_t irq_index;  // the IRQ index
    IRQn_Type  irq_n;    // the corresponding EXTI IRQn
} exti_lines_t;

// Used to return the index for channels array.
extern const exti_lines_t pin_lines_desc[];

#ifdef __cplusplus
}
#endif

#endif
