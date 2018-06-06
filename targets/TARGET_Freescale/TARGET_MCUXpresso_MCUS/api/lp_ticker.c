/* mbed Microcontroller Library
 * Copyright (c) 2016 - 2018 ARM Limited
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

#if DEVICE_LPTICKER

#include "lp_ticker_api.h"
#include "fsl_lptmr.h"
#include "cmsis.h"

const ticker_info_t* lp_ticker_get_info()
{
    static const ticker_info_t info = {
        32768,        // 32kHz
           16         // 16 bit counter
    };
    return &info;
}

#define OSC32K_CLK_HZ (32768)

static bool lp_ticker_inited = false;

extern void rtc_setup_oscillator(RTC_Type *base);

static void lptmr_isr(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    lp_ticker_irq_handler();
}

/** Initialize the low power ticker
 *
 */
void lp_ticker_init(void)
{
    lptmr_config_t lptmrConfig;

    if (!lp_ticker_inited) {
        /* Setup high resolution clock - LPTMR */
        LPTMR_GetDefaultConfig(&lptmrConfig);

        /* Setup the RTC 32KHz oscillator */
        CLOCK_EnableClock(kCLOCK_Rtc0);
        rtc_setup_oscillator(RTC);
        /* Use 32kHz drive */
        CLOCK_SetXtal32Freq(OSC32K_CLK_HZ);

        lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_2;
        lptmrConfig.enableFreeRunning = true;
        LPTMR_Init(LPTMR0, &lptmrConfig);
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        NVIC_ClearPendingIRQ(LPTMR0_IRQn);
        NVIC_SetVector(LPTMR0_IRQn, (uint32_t)lptmr_isr);
        EnableIRQ(LPTMR0_IRQn);

        lp_ticker_inited = true;
    } else {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    }
}

/** Read the current counter
 *
 * @return The current timer's counter value in ticks
 */
uint32_t lp_ticker_read(void)
{
    return LPTMR_GetCurrentTimerCount(LPTMR0);
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in ticks to be set
 */
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    if (timestamp == 0) {
        timestamp = 1;
    }
    LPTMR_SetTimerPeriod(LPTMR0, timestamp);
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(LPTMR0);
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(LPTMR0_IRQn);
}

/** Disable low power ticker interrupt
 *
 */
void lp_ticker_disable_interrupt(void)
{
    LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
}

/** Clear the low power ticker interrupt
 *
 */
void lp_ticker_clear_interrupt(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
}

#endif /* DEVICE_LPTICKER */
