/*******************************************************************************
 * mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
 *******************************************************************************/

#ifndef __SLEEP__H__
#define __SLEEP__H__


/* Enumeration to specify peripheral clock types:
   General purpose timer clocks 0-2,
   I2C clock,
   GPIO clock,
   RGB timer clock.
   Peripheral clock (PCLK) controls all the peripheral clocks, including
   all the clocks mentioned previously
*/
typedef enum {
    GPT0_CLOCK = 0,
    GPT1_CLOCK,
    GPT2_CLOCK,
    I2C_CLOCK,
    GPIO_CLOCK,
    TIMER_RGB_CLOCK,
    PCLK
} peripheral_clk_t;


/* Function to enable/disable clock gating for the available clocks.
   PCLK overrides all the other clocks.
*/
void set_clock_gating(peripheral_clk_t eClk, int enable);

#endif  // #ifndef __SLEEP_H__
