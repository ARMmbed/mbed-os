/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#if DEVICE_RTC
#include <stdlib.h>
#include "cmsis.h"
#include "rtc_api.h"
#include "mbed_mktime.h"
#include "mbed_critical.h"
#include "mbed_error.h"
#include "mbed_debug.h"

#define BP6A_RTC_START_TIME     946684800
#define BP6A_MAX_REG            0x0FFFFFFF
static bool rtc_initialized = false;
static bool g_before2000 = false;
static time_t g_rtc_offset = 0;

void rtc_init(void)
{
    if (!rtc_initialized) {
        uint32_t sys_clk = bp6a_cmu_get_clock_freq(CMU_FCLK_AHBCLK);

        bp6a_set_rtc_delay((uint32_t)((float)sys_clk * 2 / 32789));
        bp6a_rtc_init();
        g_rtc_offset =  bp6a_rtc_read_offset(&g_before2000);
        rtc_initialized = true;
    }
}

void rtc_free(void)
{
    rtc_initialized = 0;
}

int rtc_isenabled(void)
{
    return 1;
}

time_t rtc_read(void)
{
    struct rtc_bcd_s rtc_val;
    struct tm timeinfo;
    time_t t;

    bp6a_rtc_getdatetime(&rtc_val);

    timeinfo.tm_sec = bp6a_rtc_bcd2bin(rtc_val.sec);
    timeinfo.tm_min = bp6a_rtc_bcd2bin(rtc_val.min);
    timeinfo.tm_hour = bp6a_rtc_bcd2bin(rtc_val.hour);
    timeinfo.tm_mday = bp6a_rtc_bcd2bin(rtc_val.day);
    timeinfo.tm_mon = bp6a_rtc_bcd2bin(rtc_val.mon);
    timeinfo.tm_year = bp6a_rtc_bcd2bin(rtc_val.year);
    timeinfo.tm_year += 100;
    if (_rtc_maketime(&timeinfo, &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return 0;
    }

    t += g_rtc_offset;

    if (g_before2000) {
        t -= BP6A_RTC_START_TIME;
    }
    return t;
}

void rtc_write(time_t t)
{
    struct rtc_bcd_s rtc_val;
    struct tm timeinfo;

    if (t > BP6A_MAX_REG) {
        g_rtc_offset = t;
        t = 0;
    } else
        g_rtc_offset = 0;

    /*BP6A : The implicit number of thousands place is 20.*/
    if (t < BP6A_RTC_START_TIME) {
        g_before2000 = true;
        t += BP6A_RTC_START_TIME;
    } else {
        g_before2000 = false;
    }

    bp6a_rtc_offset_write(g_rtc_offset, (uint32_t)g_before2000);

    if (_rtc_localtime(t, &timeinfo, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return;
    }

    rtc_val.sec = timeinfo.tm_sec;
    rtc_val.min = timeinfo.tm_min;
    rtc_val.hour = timeinfo.tm_hour;
    rtc_val.day = timeinfo.tm_mday;
    rtc_val.mon = timeinfo.tm_mon;
    rtc_val.year = timeinfo.tm_year - 100;

    bp6a_rtc_setdatetime(&rtc_val);

}

#endif /* DEVICE_RTC */
