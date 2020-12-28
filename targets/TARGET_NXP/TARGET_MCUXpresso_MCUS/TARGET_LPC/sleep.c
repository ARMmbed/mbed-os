/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "sleep_api.h"
#include "cmsis.h"
#include "fsl_power.h"
#include "clock_config.h"

void hal_sleep(void)
{
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}

void hal_deepsleep(void)
{
    LPC_CLOCK_INTERNAL_IRC;

    /* Enter Deep Sleep mode */
#if (defined(FSL_FEATURE_POWERLIB_NIOBE4_EXTEND ) && FSL_FEATURE_POWERLIB_NIOBE4_EXTEND )
    POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP, 0x0, WAKEUP_GPIO_INT0_0, 0x0);
#else
    POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
#endif

    LPC_CLOCK_RUN;
}
