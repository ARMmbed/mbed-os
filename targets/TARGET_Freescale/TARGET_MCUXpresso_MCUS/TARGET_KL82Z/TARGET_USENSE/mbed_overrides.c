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

//!< this contains the wakeup source
rcm_reset_source_t kinetisResetSource;

// called before main
void mbed_sdk_init() {
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);

    // check the power mode source
    kinetisResetSource = (rcm_reset_source_t) (RCM_GetPreviousResetSources(RCM));
    // if waking up from VLLS, do some cleanup
    if (kinetisResetSource & kRCM_SourceWakeup) {
        PMC_ClearPeriphIOIsolationFlag(PMC);
        NVIC_ClearPendingIRQ(LLWU_IRQn);
    }

    BOARD_BootClockRUN();
}

// Change the NMI pin to an input. This allows NMI pin to
//  be used as a low power mode wakeup.  The application will
//  need to change the pin back to NMI_b or wakeup only occurs once!
void NMI_Handler(void)
{
    gpio_t gpio;
    gpio_init_in(&gpio, PTA4);
}

#if DEVICE_RTC || DEVICE_LPTICKER
// Enable the RTC oscillator if available on the board
void rtc_setup_oscillator(RTC_Type *base)
{
    /* Enable the RTC oscillator */
    RTC->CR |= RTC_CR_OSCE_MASK;
}
#endif

// Set the UART clock source
void serial_clock_init(void)
{
    CLOCK_SetLpuartClock(2U);
}
