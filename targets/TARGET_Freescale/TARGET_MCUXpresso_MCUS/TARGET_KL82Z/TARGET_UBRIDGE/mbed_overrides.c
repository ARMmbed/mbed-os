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
#include <hal/gpio_api.h>
#include "fsl_smc.h"
#include "fsl_rcm.h"
#include "fsl_pmc.h"
#include "fsl_clock_config.h"

rcm_reset_source_t wakeupSource;

// called before main
void mbed_sdk_init() {
    // check the power mode source
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    wakeupSource = (rcm_reset_source_t) (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM));
    if (wakeupSource == kRCM_SourceWakeup) /* Wakeup from VLLS. */
    {
        PMC_ClearPeriphIOIsolationFlag(PMC);
        NVIC_ClearPendingIRQ(LLWU_IRQn);

        /* Wait for PLL lock. */
        while (!(kMCG_Pll0LockFlag & CLOCK_GetStatusFlags())) {}
        CLOCK_SetPeeMode();
    }

    BOARD_BootClockRUN();
}

// Enable the RTC oscillator if available on the board
void rtc_setup_oscillator(RTC_Type *base)
{
    /* Enable the RTC oscillator */
    RTC->CR |= RTC_CR_OSCE_MASK;
}

// Change the NMI pin to an input. This allows NMI pin to
//  be used as a low power mode wakeup.  The application will
//  need to change the pin back to NMI_b or wakeup only occurs once!
void NMI_Handler(void)
{
    gpio_t gpio;
    gpio_init_in(&gpio, PTA4);
}

