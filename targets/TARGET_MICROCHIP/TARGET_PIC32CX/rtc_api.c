/***************************************************************************//**
* @file rtc_api.c
*******************************************************************************
* @section License
* <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
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

#if DEVICE_RTC
#include <time.h>
#include "pic32cx.h"
#include "rtc_api.h"
#include "pinmap.h"
#include "rtc.h"
#include "PeripheralPins.h"
#include "pmc.h"


static bool rtc_time_set = false;

void rtc_init(void)
{
    /* Check external clock */
    while (!pmc_osc_is_ready_32kxtal()) {
        pmc_switch_sclk_to_32kxtal(0);
    }

    /* Default RTC configuration, 24-hour mode */
	rtc_set_hour_mode(RTC, 0);
}

void rtc_free(void)
{
    /* None to do */
}

/*
 * Little check routine to see if the RTC has been enabled
 * 0 = Disabled, 1 = Enabled
 */
int rtc_isenabled(void)
{
    bool rtc_free_run;

    rtc_free_run = ((RTC->RTC_SR & RTC_SR_ACKUPD) == RTC_SR_ACKUPD_FREERUN);

    return (rtc_free_run & rtc_time_set);
}

time_t rtc_read(void)
{
    struct tm t;
    time_t t_of_day;

    /* Get Human-readable time */
    rtc_get_time(RTC, (uint32_t *)&t.tm_hour, (uint32_t *)&t.tm_min, (uint32_t *)&t.tm_sec);

    /* Get Human-readable date */
    rtc_get_date(RTC, (uint32_t *)&t.tm_year, (uint32_t *)&t.tm_mon, (uint32_t *)&t.tm_mday,
    (uint32_t *)&t.tm_wday);
    t.tm_year -= 1900; /* Year - 1900 */
    t.tm_mon--;  /* Month 0 = Jan */
    t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown

    /* Get Epoch Time */
    t_of_day = mktime(&t);

    return t_of_day;
}

void rtc_write(time_t t)
{
    struct tm ts;

    /* Get Human-readable date */
    ts = *localtime(&t);

    /* Set RTC time */
    rtc_set_time(RTC, ts.tm_hour, ts.tm_min, ts.tm_sec);
    /* Set RTC date */
    rtc_set_date(RTC, ts.tm_year + 1900, ts.tm_mon + 1, ts.tm_mday, ts.tm_wday);

    /* Set time flag */
    rtc_time_set = true;
}

#endif
