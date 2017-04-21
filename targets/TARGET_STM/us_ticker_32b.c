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

// A 32-bit timer is used
#if !TIM_MST_16BIT

TIM_HandleTypeDef TimMasterHandle;

static int us_ticker_inited = 0;

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
    return TIM_MST->CNT;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    TimMasterHandle.Instance = TIM_MST;
    // Set new output compare value
    __HAL_TIM_SET_COMPARE(&TimMasterHandle, TIM_CHANNEL_1, (uint32_t)timestamp);
    // Enable IT
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_disable_interrupt(void)
{
    TimMasterHandle.Instance = TIM_MST;
    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_clear_interrupt(void)
{
    TimMasterHandle.Instance = TIM_MST;
    __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC1);
}

#endif // !TIM_MST_16BIT
