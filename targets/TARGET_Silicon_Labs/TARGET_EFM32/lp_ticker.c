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
static time_t       time_base   = 0;
static uint32_t     time_extend = 0;
static uint32_t     extended_comp0 = 0;

void RTC_IRQHandler(void)
{
    uint32_t flags;
    flags = RTC_IntGet();
    if (flags & RTC_IF_OF) {
        RTC_IntClear(RTC_IF_OF);
        /* RTC has overflowed (24 bits). Use time_extend as software counter for 32 more bits. */
        time_extend += 1;
    }
    if (flags & RTC_IF_COMP0) {
        RTC_IntClear(RTC_IF_COMP0);
        if (lp_ticker_irq_handler != NULL && time_extend == extended_comp0) {
            lp_ticker_irq_handler();
        }
    }
}

uint64_t rtc_get_full(void)
{
    uint64_t ticks = 0;

    do
    {
        /* In case someone's trying to read time in a critical section */
        if (RTC_IntGet() & RTC_IF_OF) {
            RTC_IntClear(RTC_IF_OF);
            time_extend += 1;
        }

        ticks = (uint64_t)time_extend << RTC_BITS;
        ticks += RTC_CounterGet();
    }
    while ( (ticks & RTC_MAX_VALUE) != RTC_CounterGet() );

    return ticks;
}

void lp_ticker_init()
{
    core_util_critical_section_enter();
    if (!rtc_inited) {
        CMU_ClockEnable(cmuClock_RTC, true);

        /* Scale clock to save power */
        CMU_ClockDivSet(cmuClock_RTC, RTC_CLOCKDIV);

        /* Initialize RTC */
        RTC_Init_TypeDef init = RTC_INIT_DEFAULT;
        init.enable = 1;
        /* Don't use compare register 0 as top value */
        init.comp0Top = 0;

        /* Enable Interrupt from RTC */
        RTC_IntEnable(RTC_IEN_OF);
        NVIC_SetVector(RTC_IRQn, (uint32_t)RTC_IRQHandler);
        NVIC_EnableIRQ(RTC_IRQn);

        /* Initialize */
        RTC_Init(&init);

        rtc_inited = true;
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
}

void rtc_enable_comp0(bool enable)
{
    RTC_FreezeEnable(true);
    if (!enable) {
        RTC_IntDisable(RTC_IF_COMP0);
    } else {
        RTC_IntEnable(RTC_IF_COMP0);
    }
    RTC_FreezeEnable(false);
}

void rtc_set_comp0_value(uint64_t value, bool enable)
{
    rtc_enable_comp0(false);

    /* Set callback */
    RTC_FreezeEnable(true);
    extended_comp0 = (uint32_t) (value >> RTC_BITS);
    RTC_CompareSet(0, (uint32_t) (value & RTC_MAX_VALUE));
    RTC_FreezeEnable(false);

    rtc_enable_comp0(enable);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint64_t rtc_compare_value;
    uint64_t current_ticks = rtc_get_full();
    timestamp_t current_time = lp_ticker_read();

    /* calculate offset value */
    timestamp_t offset = timestamp - current_time;

    /* If the requested timestamp is too far in the future, we might not be able
     * to set the interrupt accurately due to potentially having ticked between
     * calculating the timestamp to set and us calculating the offset. */
    if(offset > 0xFFFF0000) offset = 100;

    /* map offset to RTC value */
    // ticks = offset * RTC frequency div 1000000
    rtc_compare_value = ((uint64_t)offset * (LOW_ENERGY_CLOCK_FREQUENCY / RTC_CLOCKDIV_INT)) / 1000000;

    /* If RTC offset is less then 2 RTC ticks, the interrupt won't fire */
    if(rtc_compare_value < 2) {
        rtc_compare_value = 2;
    }

    rtc_compare_value += current_ticks;

    rtc_set_comp0_value(rtc_compare_value, true);
}

void lp_ticker_fire_interrupt(void)
{
    RTC_IntSet(RTC_IFS_COMP0);
}

void lp_ticker_disable_interrupt()
{
    rtc_enable_comp0(false);
}

void lp_ticker_clear_interrupt()
{
    /* No need to clear interrupt flag, since that already happens at RTC level */
}

timestamp_t lp_ticker_read()
{
    lp_ticker_init();

    uint64_t ticks_temp;
    uint64_t ticks = rtc_get_full();

    /* ticks = counter tick value
     * timestamp = value in microseconds
     * timestamp = ticks * 1.000.000 / RTC frequency
     */

    ticks_temp = (ticks * 1000000) / (LOW_ENERGY_CLOCK_FREQUENCY / RTC_CLOCKDIV_INT);
    return (timestamp_t) (ticks_temp & 0xFFFFFFFF);
}

#elif defined(RTCC_PRESENT)
/* lp_ticker api is implemented in rtc_rtcc.c */
#endif /* RTC_PRESENT */
#endif /* DEVICE_LOWPOWERTIMER */
