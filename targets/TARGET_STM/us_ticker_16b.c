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
#include "hal_tick.h"

// A 16-bit timer is used
#if TIM_MST_16BIT

TIM_HandleTypeDef TimMasterHandle;

volatile uint32_t SlaveCounter = 0;
volatile uint32_t oc_int_part = 0;
volatile uint16_t oc_rem_part = 0;
volatile uint8_t tim_it_update; // TIM_IT_UPDATE event flag set in timer_irq_handler()
volatile uint32_t tim_it_counter = 0; // Time stamp to be updated by timer_irq_handler()

static int us_ticker_inited = 0;

void set_compare(uint16_t count) {
    TimMasterHandle.Instance = TIM_MST;
    // Set new output compare value
    __HAL_TIM_SetCompare(&TimMasterHandle, TIM_CHANNEL_1, count);
    // Enable IT
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_init(void) {
    if (us_ticker_inited) return;
    us_ticker_inited = 1;

    TimMasterHandle.Instance = TIM_MST;

    HAL_InitTick(0); // The passed value is not used
}

uint32_t us_ticker_read() {
    uint32_t counter;

    TimMasterHandle.Instance = TIM_MST;

    if (!us_ticker_inited) us_ticker_init();

    tim_it_update = 0; // Clear TIM_IT_UPDATE event flag

#if defined(TARGET_STM32L0)
    volatile uint16_t cntH_old, cntH, cntL;
    do {
        // For some reason on L0xx series we need to read and clear the 
        // overflow flag which give extra time to propelry handle possible
        // hiccup after ~60s
        if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_CC1OF) == SET) {
            __HAL_TIM_CLEAR_FLAG(&TimMasterHandle, TIM_FLAG_CC1OF);
        }
        cntH_old = SlaveCounter;
        if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_UPDATE) == SET) {
            cntH_old += 1;
        }
        cntL = TIM_MST->CNT;

        cntH = SlaveCounter;
        if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_UPDATE) == SET) {
            cntH += 1;
        }
    } while(cntH_old != cntH);
    
    // Glue the upper and lower part together to get a 32 bit timer
    counter = (uint32_t)(cntH << 16 | cntL);
#else
    counter = TIM_MST->CNT + (uint32_t)(SlaveCounter << 16); // Calculate new time stamp
#endif

    if (tim_it_update == 1) {
        return tim_it_counter; // In case of TIM_IT_UPDATE return the time stamp that was calculated in timer_irq_handler()
    }
    else {
        return counter; // Otherwise return the time stamp calculated here
    }
}

void us_ticker_set_interrupt(timestamp_t timestamp) {
    int delta = (int)((uint32_t)timestamp - us_ticker_read());

    uint16_t cval = TIM_MST->CNT;

    if (delta <= 0) { // This event was in the past
        us_ticker_irq_handler();
    } else {
        oc_int_part = (uint32_t)(delta >> 16);
        oc_rem_part = (uint16_t)(delta & 0xFFFF);
        if (oc_rem_part <= (0xFFFF - cval)) {
            set_compare(cval + oc_rem_part);
            oc_rem_part = 0;
        } else {
            set_compare(0xFFFF);
            oc_rem_part = oc_rem_part - (0xFFFF - cval);
        }
    }
}

void us_ticker_disable_interrupt(void) {
    TimMasterHandle.Instance = TIM_MST;
    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_clear_interrupt(void) {
    TimMasterHandle.Instance = TIM_MST;
    if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_CC1) == SET) {
        __HAL_TIM_CLEAR_FLAG(&TimMasterHandle, TIM_FLAG_CC1);
    }
}

#endif // TIM_MST_16BIT
