/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#ifndef MBED_GPIO_IRQ_DEVICE_H
#define MBED_GPIO_IRQ_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

// when LL is available, below include can be used
// #include "stm32f2xx_ll_exti.h"
// until then let's define locally the required functions
__STATIC_INLINE void LL_EXTI_EnableRisingTrig_0_31(uint32_t ExtiLine)
{
  SET_BIT(EXTI->RTSR, ExtiLine);
}
__STATIC_INLINE void LL_EXTI_DisableRisingTrig_0_31(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->RTSR, ExtiLine);
}
__STATIC_INLINE void LL_EXTI_EnableFallingTrig_0_31(uint32_t ExtiLine)
{
  SET_BIT(EXTI->FTSR, ExtiLine);
}
__STATIC_INLINE void LL_EXTI_DisableFallingTrig_0_31(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->FTSR, ExtiLine);
}
__STATIC_INLINE void LL_EXTI_EnableIT_0_31(uint32_t ExtiLine)
{
  SET_BIT(EXTI->IMR, ExtiLine);
}
__STATIC_INLINE void LL_EXTI_DisableIT_0_31(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->IMR, ExtiLine);
}
// Above lines shall be later defined in LL

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
