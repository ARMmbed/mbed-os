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
#include "gpio_api.h"
#include "fsl_rtc.h"
#include "fsl_clock_config.h"

// called before main
void mbed_sdk_init()
{
    rtc_config_t rtc_basic_config;
    uint32_t u32cTPR_counter = 0;

    BOARD_BootClockRUN();

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

// Change the NMI pin to an input. This allows NMI pin to
//  be used as a low power mode wakeup.  The application will
//  need to change the pin back to NMI_b or wakeup only occurs once!
void NMI_Handler(void)
{
    gpio_t gpio;
    gpio_init_in(&gpio, PTA4);
}

// Set the UART clock source
void serial_clock_init(void)
{
    CLOCK_SetLpuartClock(2U);
}

// Get the QSPI clock frequency
uint32_t qspi_get_freq(void)
{
    return CLOCK_GetFreq(kCLOCK_McgPll0Clk);
}

