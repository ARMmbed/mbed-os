/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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
#include "rtc_api.h"

#if DEVICE_RTC

#include "common_rtc.h"
#include "nrf_drv_clock.h"
#include "app_util_platform.h"

static time_t m_time_base;

void rtc_init(void)
{
    common_rtc_init();
}

void rtc_free(void)
{
    // A common counter is used for RTC, lp_ticker and us_ticker, so it can't be
    // disabled here, but this does not cause any extra cost. Besides, currently
    // this function is not used by RTC API in mbed-drivers.
}

int rtc_isenabled(void)
{
    return m_common_rtc_enabled;
}

static uint32_t rtc_seconds_get(void)
{
    // Convert current counter value to seconds.
    uint32_t seconds = nrf_rtc_counter_get(COMMON_RTC_INSTANCE) / RTC_INPUT_FREQ;
    // Add proper amount of seconds for each registered overflow of the counter.
    uint32_t seconds_per_overflow = (1uL << RTC_COUNTER_BITS) / RTC_INPUT_FREQ;
    return (seconds + (m_common_rtc_overflows * seconds_per_overflow));
}

time_t rtc_read(void)
{
    return m_time_base + rtc_seconds_get();
}

void rtc_write(time_t t)
{
    uint32_t seconds;
    do {
        seconds = rtc_seconds_get();
        m_time_base = t - seconds;
    // If the number of seconds indicated by the counter changed during the
    // update of the time base, just repeat the update, now using the new
    // number of seconds.
    } while (seconds != rtc_seconds_get());
}

#endif // DEVICE_RTC
