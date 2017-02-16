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

#define SW_RTC_TIMER_ID        TIMER4

static gtimer_t sw_rtc;
static struct tm rtc_timeinfo;
static int sw_rtc_en=0;

static const u8 dim[14] = { 
	31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31, 28 };

static inline bool is_leap_year(unsigned int year)
{
	return (!(year % 4) && (year % 100)) || !(year % 400);
}

	
static u8 days_in_month (u8 month, u8 year)
{
	u8 ret = dim [ month - 1 ];
	if (ret == 0)
		ret = is_leap_year (year) ? 29 : 28;
	return ret;
}	

void sw_rtc_tick_handler(uint32_t id)
{
    if(++rtc_timeinfo.tm_sec > 59) {                               // Increment seconds, check for overflow
        rtc_timeinfo.tm_sec = 0;                                   // Reset seconds
        if(++rtc_timeinfo.tm_min > 59) {                           // Increment minutes, check for overflow
            rtc_timeinfo.tm_min = 0;                               // Reset minutes
            if(++rtc_timeinfo.tm_hour > 23) {                      // Increment hours, check for overflow
                rtc_timeinfo.tm_hour = 0;                          // Reset hours
                ++rtc_timeinfo.tm_yday;                            // Increment day of year
                if(++rtc_timeinfo.tm_wday > 6)                     // Increment day of week, check for overflow
                    rtc_timeinfo.tm_wday = 0;                      // Reset day of week
                                                        // Increment day of month, check for overflow
                if(++rtc_timeinfo.tm_mday >
                    days_in_month(rtc_timeinfo.tm_mon, rtc_timeinfo.tm_year + 1900)) {
                    rtc_timeinfo.tm_mday = 1;                      // Reset day of month
                    if(++rtc_timeinfo.tm_mon > 11) {               // Increment month, check for overflow
                        rtc_timeinfo.tm_mon = 0;                   // Reset month
                        rtc_timeinfo.tm_yday = 0;                  // Reset day of year
                        ++rtc_timeinfo.tm_year;                    // Increment year
                    }                                   // - year       
                }                                       // - month
            }                                           // - day
        }                                               // - hour
    }                                     
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
    time_t t;
    
    // Convert to timestamp
    t = mktime(&rtc_timeinfo);

    return t;
}

void rtc_write(time_t t)
{
    // Convert the time in to a tm
    struct tm *timeinfo = localtime(&t);

    if (timeinfo == NULL) {
        // Error
        return;
    }
    
    gtimer_stop(&sw_rtc);

    // Set the RTC
    rtc_timeinfo.tm_sec = timeinfo->tm_sec;
    rtc_timeinfo.tm_min = timeinfo->tm_min;
    rtc_timeinfo.tm_hour = timeinfo->tm_hour;
    rtc_timeinfo.tm_mday = timeinfo->tm_mday;
    rtc_timeinfo.tm_wday = timeinfo->tm_wday;
    rtc_timeinfo.tm_yday = timeinfo->tm_yday;
    rtc_timeinfo.tm_mon = timeinfo->tm_mon;
    rtc_timeinfo.tm_year = timeinfo->tm_year;

    gtimer_start(&sw_rtc);    
}

#endif  // endof "#if DEVICE_RTC"
