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
 * On Silicon Labs devices, the lowest width RTC implementation has a 24-bit
 * counter, which gets extended with a further 32-bit software counter. This
 * gives 56 bits of actual width, which with the default speed maps to
 * 557462 years before the extended RTC counter wraps around. We are pretty
 * certain no device is going to have that amount of uptime.
 * (At max speed the wraparound is at 69730 years, which is unlikely as well)
 ******************************************************************************/

#include "rtc_api.h"
#include "rtc_api_HAL.h"
#include "lp_ticker_api.h"
#include "mbed_critical.h"

static int rtc_reserved = 0;

void lp_ticker_init()
{
    if(!rtc_reserved) {
        core_util_critical_section_enter();
        rtc_init_real(RTC_INIT_LPTIMER);
        rtc_set_comp0_handler((uint32_t)lp_ticker_irq_handler);
        rtc_reserved = 1;
        core_util_critical_section_exit();
    }
}

void lp_ticker_free()
{
    if(rtc_reserved) {
        core_util_critical_section_enter();
        rtc_free_real(RTC_INIT_LPTIMER);
        rtc_reserved = 0;
        core_util_critical_section_exit();
    }
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

inline void lp_ticker_fire_interrupt(void)
{
    rtc_force_comp0();
}

inline void lp_ticker_disable_interrupt()
{
    rtc_enable_comp0(false);
}

inline void lp_ticker_clear_interrupt()
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

#endif
