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

static int us_ticker_inited = 0;

void set_compare(uint16_t count)
{
    TimMasterHandle.Instance = TIM_MST;
    // Set new output compare value
    __HAL_TIM_SET_COMPARE(&TimMasterHandle, TIM_CHANNEL_1, count);
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
    if (!us_ticker_inited) us_ticker_init();

    uint16_t cntH_old, cntH, cntL;
    do {
        cntH_old = SlaveCounter;
        /* SlaveCounter needs to be checked before AND after we read the
         * current counter TIM_MST->CNT, in case it wraps around.
         * there are 2 possible cases of wrap around
         * 1) in case this function is interrupted by timer_irq_handler and
         *    the SlaveCounter is updated. In that case we will loop again.
         * 2) in case this function is called from interrupt context during
         * wrap-around condtion. That would prevent/delay the timer_irq_handler
         * from being called so we need to locally check the FLAG_UPDATE and
         * update the cntH accordingly. The SlaveCounter variable itself will
         * be updated in the interrupt handler just after ...
         */
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
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    int current_time = us_ticker_read();
    int delta = (int)(timestamp - current_time);

    if (delta <= 0) { // This event was in the past
        /* Force the event to be handled in next interrupt context
         * This prevents calling interrupt handlers in loops as
         * us_ticker_set_interrupt might called again from the
         * application handler
         */
        oc_int_part = 0;
        TimMasterHandle.Instance = TIM_MST;
        HAL_TIM_GenerateEvent(&TimMasterHandle, TIM_EVENTSOURCE_CC1);
    } else {
        /*  set the comparator at the timestamp lower 16 bits
         *  and count the number of wrap-around loops to do with
         *  the upper 16 bits
         */
        oc_int_part = (uint32_t)(delta >> 16);
        set_compare(timestamp & 0xFFFF);
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
    __HAL_TIM_CLEAR_FLAG(&TimMasterHandle, TIM_FLAG_CC1);
}

#endif // TIM_MST_16BIT
