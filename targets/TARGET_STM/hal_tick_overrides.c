/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

#if DEVICE_USTICKER

#include "hal/us_ticker_api.h"
#include "us_ticker_data.h"

// This variable is set to 1 at the of mbed_sdk_init function.
// The ticker_read_us function must not be called until the mbed_sdk_init is terminated.
extern int mbed_sdk_inited;

// Defined in us_ticker.c
void init_16bit_timer(void);
void init_32bit_timer(void);

#if TIM_MST_BIT_WIDTH == 16
// Variables also reset in us_ticker_init()
uint32_t prev_time = 0;
uint32_t elapsed_time = 0;
#endif

// Overwrite default HAL functions defined as "weak"

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
#if TIM_MST_BIT_WIDTH == 16
    init_16bit_timer();
#else
    init_32bit_timer();
#endif
    return HAL_OK;
}

uint32_t HAL_GetTick()
{
#if TIM_MST_BIT_WIDTH == 16
    uint32_t new_time;
    if (mbed_sdk_inited) {
        // Apply the latest time recorded just before the sdk is inited
        new_time = ticker_read_us(get_us_ticker_data()) + prev_time;
        prev_time = 0; // Use this time only once
        return (new_time / 1000);
    } else {
        new_time = us_ticker_read();
        elapsed_time += (new_time - prev_time) & 0xFFFF; // Only use the lower 16 bits
        prev_time = new_time;
        return (elapsed_time / 1000);
    }
#else // 32-bit timer
    if (mbed_sdk_inited) {
        return (ticker_read_us(get_us_ticker_data()) / 1000);
    } else {
        return (us_ticker_read() / 1000);
    }
#endif
}

void HAL_SuspendTick(void)
{
    // Do nothing
}

void HAL_ResumeTick(void)
{
    // Do nothing
}

#endif /* DEVICE_USTICKER */
