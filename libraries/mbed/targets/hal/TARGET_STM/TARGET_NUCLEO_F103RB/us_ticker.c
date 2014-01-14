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

int us_ticker_inited = 0;

void us_ticker_init(void) {
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    if (us_ticker_inited) return;
    us_ticker_inited = 1;
  
    // Enable Timers clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
    // Time base configuration
    // TIM1 is used as "master", "TIM4" as "slave". TIM4 is clocked by TIM1.
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock / 1000000) - 1; // 1 µs tick
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  

    // Master timer configuration
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    
    // Slave timer configuration
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_External1);
    TIM_SelectInputTrigger(TIM4, TIM_TS_ITR0);
  
    // Enable timers
    TIM_Cmd(TIM4, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

uint32_t us_ticker_read() {
    uint32_t counter, counter2;
    if (!us_ticker_inited) us_ticker_init();
    // A situation might appear when TIM1 overflows right after TIM4 is read and before the
    // new (overflowed) value of TIM1 is read, which would make the code below consider the
    // previous (incorrect) value of TIM4 and the new value of TIM1, which would return a
    // value in the past. Avoid this by computing consecutive values of the timer until they
    // are properly ordered.
    counter = counter2 = (uint32_t)((uint32_t)TIM_GetCounter(TIM4) << 16) + (uint32_t)TIM_GetCounter(TIM1);
    while (1) {
        counter2 = (uint32_t)((uint32_t)TIM_GetCounter(TIM4) << 16) + (uint32_t)TIM_GetCounter(TIM1);
        if (counter2 > counter)
            break;
        counter = counter2;
    }
    return counter2;
}

void us_ticker_set_interrupt(unsigned int timestamp) {
    if (timestamp > 0xFFFF) {
        TIM_SetCompare1(TIM4, (uint16_t)((timestamp >> 16) & 0xFFFF));
        TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
        NVIC_SetVector(TIM4_IRQn, (uint32_t)us_ticker_irq_handler);
        NVIC_EnableIRQ(TIM4_IRQn);      
    }
    else {
        TIM_SetCompare1(TIM1, (uint16_t)timestamp);
        TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);  
        NVIC_SetVector(TIM1_CC_IRQn, (uint32_t)us_ticker_irq_handler);
        NVIC_EnableIRQ(TIM1_CC_IRQn);
    }
}

void us_ticker_disable_interrupt(void) {
    TIM_ITConfig(TIM1, TIM_IT_CC1, DISABLE);
    TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);
}

void us_ticker_clear_interrupt(void) {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
}
