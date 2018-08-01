/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
#include <time.h>
#include "timer_api.h"      // software-RTC: use a g-timer for the tick of the RTC
#include "mbed_mktime.h"

#define SW_RTC_TIMER_ID        TIMER4

static gtimer_t sw_rtc;
static int sw_rtc_en=0;
static time_t rtc_time;

void sw_rtc_tick_handler(uint32_t id)
{
    rtc_time++;
}

void rtc_init(void)
{
    // Initial a periodical timer
    gtimer_init(&sw_rtc, SW_RTC_TIMER_ID);
    // Tick every 1 sec
    gtimer_start_periodical(&sw_rtc, 1000000, (void*)sw_rtc_tick_handler, (uint32_t)&sw_rtc);
    sw_rtc_en = 1;
}

void rtc_free(void)
{
    sw_rtc_en = 0;
    gtimer_stop(&sw_rtc);
    gtimer_deinit(&sw_rtc);
}

int rtc_isenabled(void)
{
    return(sw_rtc_en);
}

time_t rtc_read(void)
{
    return rtc_time;
}

void rtc_write(time_t t)
{
    gtimer_stop(&sw_rtc);

    // Set the RTC
    rtc_time = t;

    gtimer_start(&sw_rtc);    
}

#endif  // endof "#if DEVICE_RTC"
