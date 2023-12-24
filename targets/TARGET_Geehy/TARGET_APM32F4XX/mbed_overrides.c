/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
 *
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

#include "apm32f4xx.h"
#include "cmsis.h"
#include "hal_tick.h"
#include "apm32f4xx_misc.h"

int mbed_sdk_inited = 0;

#if TICKER_TIMER_WIDTH_BIT == 16
extern void ticker_16bits_timer_init(void);
#else
extern void ticker_32bits_timer_init(void);
#endif

/**
 * SDK hook for running code before ctors or OS
 *
 * This is a weak function which can be overridden by a target's
 * SDK to allow code to run after ram is initialized but before
 * the OS has been started or constructors have run.
 *
 * Preconditions:
 * - Ram is initialized
 * - NVIC is setup
 */

  /*!
 * @brief     This function is called after RAM initialization and before main.
 *
 * @param     None
 *
 * @retval    None
 */
void mbed_sdk_init()
{
    /* Update the SystemCoreClock */
    SystemCoreClockUpdate();
    NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_4);

    /* configure 1ms tick */
#if TICKER_TIMER_WIDTH_BIT == 16
    ticker_16bits_timer_init();
#else
    ticker_32bits_timer_init();
#endif

    SystemCoreClockUpdate();

    mbed_sdk_inited = 1;
}
