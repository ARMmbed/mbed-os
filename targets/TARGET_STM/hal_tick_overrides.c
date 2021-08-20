/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#include "us_ticker_defines.h"
#include "mbed_critical.h"

// This variable is set to 1 at the of mbed_sdk_init function.
// The ticker_read_us function must not be called until the mbed_sdk_init is terminated.
extern int mbed_sdk_inited;

// Defined in us_ticker.c
void init_16bit_timer(void);
void init_32bit_timer(void);

#if TIM_MST_BIT_WIDTH <= 16
static uint16_t prev_time;
#else
static uint32_t prev_time;
#endif
static uint32_t total_ticks;
static uint16_t prev_tick_remainder;

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
    // Prevent a task switch causing errors since this function isn't re-entrant
    core_util_critical_section_enter();

    uint32_t new_time = us_ticker_read();
    uint32_t elapsed_time = (((new_time - prev_time) & US_TICKER_MASK) + prev_tick_remainder);
    prev_time = new_time;
    uint32_t elapsed_ticks;
    // Speed optimisation for small time intervals, avoiding a potentially-slow C library divide.
    if (TIM_MST_BIT_WIDTH <= 16 || elapsed_time < 65536) {
        elapsed_ticks = 0;
        while (elapsed_time >= 1000) {
            elapsed_ticks++;
            elapsed_time -= 1000;
        }
        prev_tick_remainder = elapsed_time;
    } else {
        elapsed_ticks = elapsed_time / 1000;
        prev_tick_remainder = elapsed_time % 1000;
    }
    total_ticks += elapsed_ticks;

    core_util_critical_section_exit();
    return total_ticks;
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
