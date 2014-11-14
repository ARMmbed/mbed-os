/* mbed Microcontroller Library
 * Copyright (c) 2014, STMicroelectronics
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
 */
#include <stddef.h>
#include "us_ticker_api.h"
#include "PeripheralNames.h"

// 32-bit timer selection
#define TIM_MST      TIM2
#define TIM_MST_IRQ  TIM2_IRQn
#define TIM_MST_RCC  __TIM2_CLK_ENABLE()

static TIM_HandleTypeDef TimMasterHandle;
static int us_ticker_inited = 0;

void us_ticker_init(void)
{
    if (us_ticker_inited) return;
    us_ticker_inited = 1;

    // Update the SystemCoreClock variable
    SystemCoreClockUpdate();

    // Enable timer clock
    TIM_MST_RCC;

    // Configure time base
    TimMasterHandle.Instance = TIM_MST;
    TimMasterHandle.Init.Period            = 0xFFFFFFFF;
    TimMasterHandle.Init.Prescaler         = (uint32_t)(SystemCoreClock / 1000000) - 1; // 1 us tick
    TimMasterHandle.Init.ClockDivision     = 0;
    TimMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimMasterHandle.Init.RepetitionCounter = 0;
    HAL_TIM_OC_Init(&TimMasterHandle);

    NVIC_SetVector(TIM_MST_IRQ, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(TIM_MST_IRQ);

    // Enable timer
    HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1);
}

uint32_t us_ticker_read()
{
    if (!us_ticker_inited) us_ticker_init();
    return TIM_MST->CNT;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    // Set new output compare value
    __HAL_TIM_SetCompare(&TimMasterHandle, TIM_CHANNEL_1, (uint32_t)timestamp);
    // Enable IT
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_disable_interrupt(void)
{
    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_clear_interrupt(void)
{
    __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC1);
}
