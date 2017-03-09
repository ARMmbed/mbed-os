/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "mbed_assert.h"
#include "rtc_api.h"
#include "cmsis.h"
#include "sysclk.h"
#include "rtc.h"

static int rtc_inited = 0;

extern uint8_t g_sys_init;

void rtc_init(void)
{
    if (g_sys_init == 0) {
        sysclk_init();
        system_board_init();
        g_sys_init = 1;
    }
    /* Default RTC configuration, 24-hour mode */
    rtc_set_hour_mode(RTC, 0);
    rtc_inited = 1;
}

void rtc_free(void)
{
    /*This is a free running peripheral and cannot be disabled*/
    rtc_inited = 0;
}


int rtc_isenabled(void)
{
    return rtc_inited;
}


time_t rtc_read(void)
{
    if (!rtc_inited) {
        /* Return invalid time for now! */
        return 0;
    }
    struct tm timeinfo;
    uint32_t ul_hour, ul_minute, ul_second;
    uint32_t ul_year, ul_month, ul_day, ul_week;

    rtc_get_time(RTC, &ul_hour, &ul_minute, &ul_second);
    rtc_get_date(RTC, &ul_year, &ul_month, &ul_day, &ul_week);

    timeinfo.tm_sec = ul_second;
    timeinfo.tm_min = ul_minute;
    timeinfo.tm_hour = ul_hour;
    timeinfo.tm_mday = ul_day;
    timeinfo.tm_wday = ul_week;
    timeinfo.tm_mon = ul_month;
    timeinfo.tm_year = (ul_year - 1900);

    /* Convert to timestamp */
    time_t t = mktime(&timeinfo);
    return t;
}

void rtc_write(time_t t)
{
    if (!rtc_inited) {
        /* Initialize the RTC is not yet initialized */
        rtc_init();
    }
    struct tm *timeinfo = localtime(&t);
    uint32_t ul_hour, ul_minute, ul_second;
    uint32_t ul_year, ul_month, ul_day, ul_week;

    ul_second = timeinfo->tm_sec;
    ul_minute = timeinfo->tm_min;
    ul_hour = timeinfo->tm_hour;
    ul_day = timeinfo->tm_mday;
    ul_week = timeinfo->tm_wday;
    ul_month = timeinfo->tm_mon;
    ul_year = timeinfo->tm_year;

    /* Set the RTC  */
    rtc_set_time(RTC, ul_hour, ul_minute, ul_second);
    rtc_set_date(RTC, ul_year, ul_month, ul_day, ul_week);
}