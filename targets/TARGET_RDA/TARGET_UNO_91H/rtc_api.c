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
#include "rtc_api.h"

#if DEVICE_RTC

#include "us_ticker_api.h"

#define RTC_TIMER_INITVAL_REG       (RDA_SCU->FTMRINITVAL)
#define RTC_TIMER_TIMESTAMP_REG     (RDA_SCU->FTMRTS)

#define RTC_TIMER_CLOCK_SOURCE      (32768)

static time_t sw_timebase = 0U;
static time_t sw_timeofst = 0U;
static uint32_t round_ticks = 0U;
static uint32_t remain_ticks = 0U;
static int is_rtc_enabled = 0;

void rtc_init(void)
{
    uint32_t start_time;
    /* Make sure us_ticker is running */
    start_time = us_ticker_read();
    /* To fix compiling warning */
    start_time = start_time;
    /* Record the ticks */
    round_ticks = RTC_TIMER_INITVAL_REG;
    is_rtc_enabled = 1;
}

void rtc_free(void)
{
    is_rtc_enabled = 0;
}

int rtc_isenabled(void)
{
    return is_rtc_enabled;
}

time_t rtc_read(void)
{
    /* Get hw timestamp in seconds, ">>15" equals "/RTC_TIMER_CLOCK_SOURCE" (="/32768") */
    time_t hw_ts = (time_t)((RTC_TIMER_TIMESTAMP_REG + remain_ticks) >> 15);
    /* Calculate current timestamp */
    time_t t = sw_timebase + hw_ts - sw_timeofst;
    return t;
}

void rtc_write(time_t t)
{
    /* Get hw timestamp in seconds */
    uint32_t rtc_cur_ticks = RTC_TIMER_TIMESTAMP_REG;
    uint32_t rtc_rpl_ticks = (rtc_cur_ticks + remain_ticks) & 0x00007FFFUL;
    uint32_t sw_rpl = 0U;
    time_t hw_ts = (time_t)((rtc_cur_ticks + remain_ticks) >> 15);
    /* Set remaining ticks */
    remain_ticks += rtc_rpl_ticks;
    sw_rpl = remain_ticks >> 15;
    remain_ticks &= 0x00007FFFUL;
    /* Set sw timestamp in seconds */
    if (t < hw_ts) {
        sw_timebase = sw_rpl;
        sw_timeofst = hw_ts - t;
    } else {
        sw_timebase = sw_rpl + t - hw_ts;
        sw_timeofst = 0U;
    }
}

void rtc_base_update(void)
{
    if (is_rtc_enabled) {
        uint32_t sw_rpl = 0U;
        remain_ticks += round_ticks & 0x00007FFFUL;
        sw_rpl = remain_ticks >> 15;
        remain_ticks &= 0x00007FFFUL;
        sw_timebase += (time_t)((round_ticks >> 15) + sw_rpl);
    }
}
#endif
