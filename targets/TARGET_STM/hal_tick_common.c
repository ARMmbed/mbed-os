/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "hal/us_ticker_api.h"

// Overwrite default HAL functions defined as "weak"

uint32_t HAL_GetTick()
{
    return ticker_read_us(get_us_ticker_data()) / 1000; // 1 ms tick is required for ST HAL
}

void HAL_SuspendTick(void)
{
    // Do nothing
}

void HAL_ResumeTick(void)
{
    // Do nothing
}
