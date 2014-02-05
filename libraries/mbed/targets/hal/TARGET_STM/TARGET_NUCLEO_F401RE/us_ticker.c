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
#include "stm32f4xx_hal.h"

// Timer selection:

#define TIM_MST            TIM1
#define TIM_MST_UP_IRQ     TIM1_UP_TIM10_IRQn
#define TIM_MST_OC_IRQ     TIM1_CC_IRQn
#define TIM_MST_RCC        __TIM1_CLK_ENABLE()

static TIM_HandleTypeDef TimMasterHandle;
    
static int      us_ticker_inited = 0;
static uint32_t SlaveCounter = 0;
static uint32_t us_ticker_int_counter = 0;
static uint16_t us_ticker_int_remainder = 0;

// Used to increment the slave counter
static void tim_update_irq_handler(void) {
    SlaveCounter++;
    if (__HAL_TIM_GET_ITSTATUS(&TimMasterHandle, TIM_IT_UPDATE) == SET) {
        __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_UPDATE);
        __HAL_TIM_SetCounter(&TimMasterHandle, 0); // Reset counter !!!
    }
}

// Used by interrupt system
static void tim_oc_irq_handler(void) {
    // Clear interrupt flag
    if (__HAL_TIM_GET_ITSTATUS(&TimMasterHandle, TIM_IT_CC1) == SET) {
        __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC1);
    }
    
    if (us_ticker_int_counter > 0) {
        __HAL_TIM_SetCompare(&TimMasterHandle, TIM_CHANNEL_1, 0xFFFF);
        us_ticker_int_counter--;
    } else {
        if (us_ticker_int_remainder > 0) {
            __HAL_TIM_SetCompare(&TimMasterHandle, TIM_CHANNEL_1, us_ticker_int_remainder);
            us_ticker_int_remainder = 0;
        } else {
            // This function is going to disable the interrupts if there are
            // no other events in the queue
            us_ticker_irq_handler();
        }
    }    
}

void us_ticker_init(void) {
    if (us_ticker_inited) return;
    us_ticker_inited = 1;
  
    // Enable Timer clock
    TIM_MST_RCC;
  
    // Configure time base
    TimMasterHandle.Instance = TIM_MST;
    TimMasterHandle.Init.Period            = 0xFFFF;
    TimMasterHandle.Init.Prescaler         = (uint32_t)(SystemCoreClock / 1000000) - 1; // 1 µs tick
    TimMasterHandle.Init.ClockDivision     = 0;
    TimMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimMasterHandle.Init.RepetitionCounter = 0;
    //HAL_TIM_Base_Init(&TimMasterHandle);
    HAL_TIM_OC_Init(&TimMasterHandle);

    /*
    TIM_OC_InitTypeDef sConfig;
    sConfig.OCMode     = TIM_OCMODE_INACTIVE;
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.Pulse      = 0;
    HAL_TIM_OC_ConfigChannel(&TimMasterHandle, &sConfig, TIM_CHANNEL_1);
    */
    
    // Configure interrupts
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC1);
    
    // For 32-bit counter
    NVIC_SetVector(TIM_MST_UP_IRQ, (uint32_t)tim_update_irq_handler);
    NVIC_EnableIRQ(TIM_MST_UP_IRQ);
    
    // For ouput compare
    NVIC_SetVector(TIM_MST_OC_IRQ, (uint32_t)tim_oc_irq_handler);
    NVIC_EnableIRQ(TIM_MST_OC_IRQ);
  
    // Enable timer
    //HAL_TIM_Base_Start(&TimMasterHandle);
    HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1);
}

uint32_t us_ticker_read() {
    uint32_t counter, counter2;
    if (!us_ticker_inited) us_ticker_init();
    // A situation might appear when Master overflows right after Slave is read and before the
    // new (overflowed) value of Master is read. Which would make the code below consider the
    // previous (incorrect) value of Slave and the new value of Master, which would return a
    // value in the past. Avoid this by computing consecutive values of the timer until they
    // are properly ordered.
    counter = (uint32_t)(SlaveCounter << 16);
    counter += TIM_MST->CNT;
    while (1) {
        counter2 = (uint32_t)(SlaveCounter << 16);
        counter2 += TIM_MST->CNT;
        if (counter2 > counter) {
            break;
        }
        counter = counter2;
    }
    return counter2;
}

void us_ticker_set_interrupt(unsigned int timestamp) {
    int delta = (int)(timestamp - us_ticker_read());

    if (delta <= 0) { // This event was in the past
        us_ticker_irq_handler();
        return;
    }
    else {
        us_ticker_int_counter   = (uint32_t)(delta >> 16);
        us_ticker_int_remainder = (uint16_t)(delta & 0xFFFF);
        if (us_ticker_int_counter > 0) { // means delta > 0xFFFF
            __HAL_TIM_SetCompare(&TimMasterHandle, TIM_CHANNEL_1, 0xFFFF);
            us_ticker_int_counter--;
        } else {
            __HAL_TIM_SetCompare(&TimMasterHandle, TIM_CHANNEL_1, us_ticker_int_remainder);
            us_ticker_int_remainder = 0;
        }
        __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC1);
    }
}

void us_ticker_disable_interrupt(void) {
    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_clear_interrupt(void) {
    if (__HAL_TIM_GET_ITSTATUS(&TimMasterHandle, TIM_IT_CC1) == SET) {
        __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC1);
    }
}
