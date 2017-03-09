/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#if DEVICE_LOWPOWERTIMER

#include "lp_ticker_api.h"
#include "fsl_rtc.h"
#include "fsl_lptmr.h"
#include "cmsis.h"
#include "rtc_api.h"

#define MAX_SEC_BITS (12)
#define MAX_SEC_MASK ((1 << MAX_SEC_BITS) - 1)
#define SEC_IN_USEC (1000000)
#define OSC32K_CLK_HZ (32768)
#define MAX_LPTMR_SLEEP ((1 << 16) - 1)

static bool lp_ticker_inited = false;
static int lptmr_schedule = 0;

static void rtc_isr(void)
{
    RTC_DisableInterrupts(RTC, kRTC_AlarmInterruptEnable);
    RTC->TAR = 0; /* Write clears the IRQ flag */

    /* Wait subsecond remainder if any */
    if (lptmr_schedule) {
        LPTMR_SetTimerPeriod(LPTMR0, lptmr_schedule);
        LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_StartTimer(LPTMR0);
    } else {
        lp_ticker_irq_handler();
    }
}

static void lptmr_isr(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    LPTMR_StopTimer(LPTMR0);

    lp_ticker_irq_handler();
}

/** Initialize the low power ticker
 *
 */
void lp_ticker_init(void)
{
    lptmr_config_t lptmrConfig;

    if (lp_ticker_inited) {
        return;
    }
    lp_ticker_inited = true;

    /* Setup low resolution clock - RTC */
    if (!rtc_isenabled()) {
        rtc_init();
        RTC_DisableInterrupts(RTC, kRTC_AlarmInterruptEnable | kRTC_SecondsInterruptEnable);
        RTC_StartTimer(RTC);
    }

    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_SetVector(RTC_IRQn, (uint32_t)rtc_isr);
    NVIC_EnableIRQ(RTC_IRQn);

    /* Setup high resolution clock - LPTMR */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* Use 32kHz drive */
    CLOCK_SetXtal32Freq(OSC32K_CLK_HZ);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_2;
    LPTMR_Init(LPTMR0, &lptmrConfig);
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    NVIC_ClearPendingIRQ(LPTMR0_IRQn);
    NVIC_SetVector(LPTMR0_IRQn, (uint32_t)lptmr_isr);
    EnableIRQ(LPTMR0_IRQn);
}

/** Read the current counter
 *
 * @return The current timer's counter value in microseconds
 */
uint32_t lp_ticker_read(void)
{
    uint32_t sec, pre;

    if (!lp_ticker_inited) {
        lp_ticker_init();
    }

    sec = RTC->TSR; /* 32b: Seconds */
    pre = RTC->TPR; /* 16b: Increments every 32.768kHz clock cycle (30us) */

    /* Final value: 11b (4095) for sec and 21b for usec (pre can reach 1,000,000us which is close to 1<<20) */
    uint32_t ret = (((sec & MAX_SEC_MASK) * SEC_IN_USEC) + (((uint64_t)pre * SEC_IN_USEC) / OSC32K_CLK_HZ));

    return ret;
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in microseconds to be set
 */
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t now_us, delta_us, delta_ticks;

    if (!lp_ticker_inited) {
        lp_ticker_init();
    }

    lptmr_schedule = 0;
    now_us = lp_ticker_read();
    delta_us = timestamp > now_us ? timestamp - now_us : (uint32_t)((uint64_t)timestamp + 0xFFFFFFFF - now_us);

    /* Checking if LPTRM can handle this sleep */
    delta_ticks = USEC_TO_COUNT(delta_us, CLOCK_GetFreq(kCLOCK_Er32kClk));
    if (delta_ticks > MAX_LPTMR_SLEEP) {
        /* Using RTC if wait time is over 16b (2s @32kHz) */
        uint32_t delta_sec;

        delta_us += COUNT_TO_USEC(RTC->TPR, CLOCK_GetFreq(kCLOCK_Er32kClk)); /* Accounting for started second */
        delta_sec = delta_us / SEC_IN_USEC;
        delta_us -= delta_sec * SEC_IN_USEC;

        RTC->TAR = RTC->TSR + delta_sec - 1;

        RTC_EnableInterrupts(RTC, kRTC_AlarmInterruptEnable);

        /* Set aditional, subsecond, sleep time */
        if (delta_us) {
            lptmr_schedule = USEC_TO_COUNT(delta_us, CLOCK_GetFreq(kCLOCK_Er32kClk));
        }
    } else {
        /* Below RTC resolution using LPTMR */
        LPTMR_SetTimerPeriod(LPTMR0, delta_ticks);
        LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_StartTimer(LPTMR0);
    }
}

/** Disable low power ticker interrupt
 *
 */
void lp_ticker_disable_interrupt(void)
{
    LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    RTC_DisableInterrupts(RTC, kRTC_AlarmInterruptEnable);
}

/** Clear the low power ticker interrupt
 *
 */
void lp_ticker_clear_interrupt(void)
{
    RTC->TAR = 0; /* Write clears the IRQ flag */
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
}
#endif /* DEVICE_LOWPOWERTIMER */
