/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#include "mbed_wait_api.h"
#include "mbed_error.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"

#define YEAR0       1900
//#define EPOCH_YR    1970
static int rtc_inited = 0;

static const struct nu_modinit_s rtc_modinit = {RTC_0, RTC_MODULE, 0, 0, 0, RTC_IRQn, NULL};

void rtc_init(void)
{
    if (rtc_inited) {
        return;
    }
    rtc_inited = 1;
    
    // Enable IP clock
    CLK_EnableModuleClock(rtc_modinit.clkidx);
    
    RTC_Open(NULL);
}

void rtc_free(void)
{
    // FIXME
}

int rtc_isenabled(void)
{
    return rtc_inited;
}

/*
 struct tm
   tm_sec      seconds after the minute 0-61
   tm_min      minutes after the hour 0-59
   tm_hour     hours since midnight 0-23
   tm_mday     day of the month 1-31
   tm_mon      months since January 0-11
   tm_year     years since 1900
   tm_wday     days since Sunday 0-6
   tm_yday     days since January 1 0-365
   tm_isdst    Daylight Saving Time flag
*/

time_t rtc_read(void)
{
    if (! rtc_inited) {
        rtc_init();
    }
    
    S_RTC_TIME_DATA_T rtc_datetime;
    RTC_GetDateAndTime(&rtc_datetime);
    
    struct tm timeinfo;

    // Convert struct tm to S_RTC_TIME_DATA_T
    timeinfo.tm_year = rtc_datetime.u32Year - YEAR0;
    timeinfo.tm_mon  = rtc_datetime.u32Month - 1;
    timeinfo.tm_mday = rtc_datetime.u32Day;
    timeinfo.tm_wday = rtc_datetime.u32DayOfWeek;
    timeinfo.tm_hour = rtc_datetime.u32Hour;
    timeinfo.tm_min  = rtc_datetime.u32Minute;
    timeinfo.tm_sec  = rtc_datetime.u32Second;

    // Convert to timestamp
    time_t t = mktime(&timeinfo);

    return t;
}

void rtc_write(time_t t)
{
    if (! rtc_inited) {
        rtc_init();
    }
    
    // Convert timestamp to struct tm
    struct tm *timeinfo = localtime(&t);

    S_RTC_TIME_DATA_T rtc_datetime;
    
    // Convert S_RTC_TIME_DATA_T to struct tm
    rtc_datetime.u32Year        = timeinfo->tm_year + YEAR0;
    rtc_datetime.u32Month       = timeinfo->tm_mon + 1;
    rtc_datetime.u32Day         = timeinfo->tm_mday;
    rtc_datetime.u32DayOfWeek   = timeinfo->tm_wday;
    rtc_datetime.u32Hour        = timeinfo->tm_hour;
    rtc_datetime.u32Minute      = timeinfo->tm_min;
    rtc_datetime.u32Second      = timeinfo->tm_sec;
    rtc_datetime.u32TimeScale   = RTC_CLOCK_24;
    
    // NOTE: Timing issue with write to RTC registers. This delay is empirical, not rational.
    RTC_SetDateAndTime(&rtc_datetime);
    //nu_nop(6000);
    wait_us(100);
}

#endif
