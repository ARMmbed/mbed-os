/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#include "lp_ticker_api.h"

#if DEVICE_LOWPOWERTIMER

#include "common_rtc.h"
#include "sleep_api.h"

void lp_ticker_init(void)
{
    common_rtc_init();
}

uint32_t lp_ticker_read(void)
{
    return common_rtc_32bit_ticks_get();
}

void lp_ticker_set_interrupt(uint32_t now, uint32_t time)
{
    (void)now;
    nrf_rtc_cc_set(COMMON_RTC_INSTANCE, LP_TICKER_CC_CHANNEL, RTC_WRAP(time));
    nrf_rtc_event_clear(COMMON_RTC_INSTANCE, NRF_RTC_EVENT_COMPARE_2);
    nrf_rtc_event_enable(COMMON_RTC_INSTANCE, NRF_RTC_INT_COMPARE2_MASK);
}

uint32_t lp_ticker_get_overflows_counter(void)
{
    // Cut out the part of 'm_common_rtc_overflows' used by
    // 'common_rtc_32bit_ticks_get()'.
    return (m_common_rtc_overflows >> (32u - RTC_COUNTER_BITS));
}

uint32_t lp_ticker_get_compare_match(void)
{
    return nrf_rtc_cc_get(COMMON_RTC_INSTANCE, LP_TICKER_CC_CHANNEL);
}

void lp_ticker_sleep_until(uint32_t now, uint32_t time)
{
    lp_ticker_set_interrupt(now, time);
    sleep_t sleep_obj;
    mbed_enter_sleep(&sleep_obj);
    mbed_exit_sleep(&sleep_obj);
}

#endif // DEVICE_LOWPOWERTIMER
