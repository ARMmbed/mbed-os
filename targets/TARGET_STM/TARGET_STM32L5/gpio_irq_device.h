/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef MBED_GPIO_IRQ_DEVICE_H
#define MBED_GPIO_IRQ_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l5xx_ll_exti.h"

// Number of EXTI irq vectors (EXTI0 to EXTI15)
#define CHANNEL_NUM (16)

#define EXTI_IRQ0_NUM_LINES 1
#define EXTI_IRQ1_NUM_LINES 1
#define EXTI_IRQ2_NUM_LINES 1
#define EXTI_IRQ3_NUM_LINES 1
#define EXTI_IRQ4_NUM_LINES 1
#define EXTI_IRQ5_NUM_LINES 1
#define EXTI_IRQ6_NUM_LINES 1
#define EXTI_IRQ7_NUM_LINES 1
#define EXTI_IRQ8_NUM_LINES 1
#define EXTI_IRQ9_NUM_LINES 1
#define EXTI_IRQ10_NUM_LINES 1
#define EXTI_IRQ11_NUM_LINES 1
#define EXTI_IRQ12_NUM_LINES 1
#define EXTI_IRQ13_NUM_LINES 1
#define EXTI_IRQ14_NUM_LINES 1
#define EXTI_IRQ15_NUM_LINES 1

// Max pins for one line
#define MAX_PIN_LINE (1)

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
