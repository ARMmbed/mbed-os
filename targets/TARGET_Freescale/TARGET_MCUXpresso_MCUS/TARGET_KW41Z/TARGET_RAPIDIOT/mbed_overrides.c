/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "gpio_api.h"
#include "fsl_rtc.h"
#include "fsl_clock_config.h"

// called before main - implement here if board needs it otherwise, let
//  the application override this if necessary
void mbed_sdk_init()
{
    rtc_config_t rtc_basic_config;
    uint32_t u32cTPR_counter = 0;

    BOARD_BootClockRUN();
    /* Set the TPM clock source to be OSCERCLK, do not change as TPM2 is used for the usticker */
    CLOCK_SetTpmClock(2U);

    CLOCK_EnableClock(kCLOCK_Rtc0);

    /* Check if the Rtc oscillator is enabled */
    if ((RTC->CR & RTC_CR_OSCE_MASK) == 0u) {
        /*Init the RTC with default configuration*/
        RTC_GetDefaultConfig(&rtc_basic_config);

        /* Setup the 32K RTC OSC */
        RTC_Init(RTC, &rtc_basic_config);

        /* Enable the RTC 32KHz oscillator */
        RTC->CR |= RTC_CR_OSCE_MASK;

        /* Start the RTC time counter */
        RTC_StartTimer(RTC);

        /* Verify TPR register reaches 4096 counts */
        while (u32cTPR_counter < 4096) {
            u32cTPR_counter = RTC->TPR;
        }
        /* 32kHz Oscillator is ready. */
        RTC_Deinit(RTC);
    }

    CLOCK_DisableClock(kCLOCK_Rtc0);
}
