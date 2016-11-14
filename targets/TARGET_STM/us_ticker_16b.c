/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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

void set_compare(uint16_t count)
{
    TimMasterHandle.Instance = TIM_MST;
    // Set new output compare value
    __HAL_TIM_SetCompare(&TimMasterHandle, TIM_CHANNEL_1, count);
    // Enable IT
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_init(void)
{
    if (us_ticker_inited) return;
    us_ticker_inited = 1;

    TimMasterHandle.Instance = TIM_MST;

    HAL_InitTick(0); // The passed value is not used
}

uint32_t us_ticker_read()
{
    uint32_t counter;

    TimMasterHandle.Instance = TIM_MST;

    if (!us_ticker_inited) us_ticker_init();

#if defined(TARGET_STM32L0)
    uint16_t cntH_old, cntH, cntL;
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
    return (uint32_t)(cntH << 16 | cntL);
#else
    tim_it_update = 0; // Clear TIM_IT_UPDATE event flag
    counter = TIM_MST->CNT + (uint32_t)(SlaveCounter << 16); // Calculate new time stamp
    if (tim_it_update == 1) {
        return tim_it_counter; // In case of TIM_IT_UPDATE return the time stamp that was calculated in timer_irq_handler()
    }
    else {
        return counter; // Otherwise return the time stamp calculated here
    }
#endif
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
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

void us_ticker_disable_interrupt(void)
{
    TimMasterHandle.Instance = TIM_MST;
    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_clear_interrupt(void)
{
    TimMasterHandle.Instance = TIM_MST;
    if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_CC1) == SET) {
        __HAL_TIM_CLEAR_FLAG(&TimMasterHandle, TIM_FLAG_CC1);
    }
}

#endif // TIM_MST_16BIT
