/***************************************************************************//**
 * @file lp_ticker.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#include "clocking.h"
#if DEVICE_LPTICKER

/*******************************************************************************
 * The Silicon Labs lp_ticker implementation is mapped on top of an extended RTC
 * API, since the RTC is available in the lowest energy modes. By default, the
 * RTC counter is configured to run at 4kHz, giving us a quarter-ms resolution
 * for the low power timer, which should be good enough for a low power use
 * case.
 *
 * Mapping of mbed APIs to Silicon Labs peripherals:
 *  ---: Does not meet mbed API requirements
 *   X : Implemented to provide mbed API functionality
 *
 *  --------------------------------------------
 * | ------------- | RTCC | BURTC | RTC | TIMER |
 * | rtc_api       |  X   |   X   | --- | ----- |
 * | lp_ticker_api |  X   |       |  X  | ----- |
 * | us_ticker_api | ---  | ----- | --- |   X   |
 *  --------------------------------------------
 *
 * On Silicon Labs devices, the lowest width RTC implementation has a 24-bit
 * counter, which gets extended with a further 32-bit software counter. This
 * gives 56 bits of actual width, which with the default speed maps to
 * 557462 years before the extended RTC counter wraps around. We are pretty
 * certain no device is going to have that amount of uptime.
 * (At max speed the wraparound is at 69730 years, which is unlikely as well)
 ******************************************************************************/

#if defined(RTC_PRESENT)
#include "em_rtc.h"
#include "em_cmu.h"
#include "lp_ticker_api.h"
#include "mbed_critical.h"

#if RTC_CLOCKDIV_INT > 16
#error invalid prescaler value RTC_CLOCKDIV_INT, since LP ticker resolution will exceed 1ms.
#endif

#define RTC_BITS (24U)
#define RTC_MAX_VALUE (0xFFFFFFUL)

static bool         rtc_inited  = false;

const ticker_info_t* lp_ticker_get_info(void)
{
    static const ticker_info_t rtc_info = {
        LOW_ENERGY_CLOCK_FREQUENCY,
        RTC_BITS
    };
    return &rtc_info;
}

void RTC_IRQHandler(void)
{
    uint32_t flags;
    flags = RTC_IntGet();
    if ((flags & RTC_IF_COMP0) && rtc_inited) {
        RTC_IntClear(RTC_IF_COMP0);
        lp_ticker_irq_handler();
    }
}

void lp_ticker_init()
{
    core_util_critical_section_enter();
    if (!rtc_inited) {
        CMU_ClockEnable(cmuClock_RTC, true);

        /* Initialize RTC */
        RTC_Init_TypeDef init = RTC_INIT_DEFAULT;
        init.enable = 1;
        /* Don't use compare register 0 as top value */
        init.comp0Top = 0;

        /* Initialize */
        RTC_Init(&init);
        RTC_CounterSet(20);

        /* Enable Interrupt from RTC */
        RTC_IntDisable(RTC_IF_COMP0);
        RTC_IntClear(RTC_IF_COMP0);
        NVIC_SetVector(RTC_IRQn, (uint32_t)RTC_IRQHandler);
        NVIC_EnableIRQ(RTC_IRQn);

        rtc_inited = true;
    } else {
        /* Cancel current interrupt by virtue of calling init again */
        RTC_IntDisable(RTC_IF_COMP0);
        RTC_IntClear(RTC_IF_COMP0);
    }
    core_util_critical_section_exit();
}

void lp_ticker_free()
{
    /* Disable the RTC if it was inited and is no longer in use by anyone. */
    if (rtc_inited) {
        NVIC_DisableIRQ(RTC_IRQn);
        RTC_Reset();
        CMU_ClockEnable(cmuClock_RTC, false);
        rtc_inited = false;
    }
    RTC_FreezeEnable(false);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    RTC_IntDisable(RTC_IF_COMP0);
    RTC_IntClear(RTC_IF_COMP0);
    RTC_FreezeEnable(true);
    RTC_CompareSet(0, (uint32_t) (timestamp & RTC_MAX_VALUE));
    RTC_FreezeEnable(false);
    RTC_IntEnable(RTC_IF_COMP0);
}

void lp_ticker_fire_interrupt(void)
{
    RTC_IntEnable(RTC_IF_COMP0);
    RTC_IntSet(RTC_IF_COMP0);
}

void lp_ticker_disable_interrupt()
{
    RTC_IntDisable(RTC_IF_COMP0);
}

void lp_ticker_clear_interrupt()
{
    RTC_IntClear(RTC_IF_COMP0);
}

timestamp_t lp_ticker_read()
{
    return (timestamp_t) RTC_CounterGet();
}

#elif defined(RTCC_PRESENT)
/* lp_ticker api is implemented in rtc_rtcc.c */
#endif /* RTC_PRESENT */
#endif /* DEVICE_LPTICKER */
