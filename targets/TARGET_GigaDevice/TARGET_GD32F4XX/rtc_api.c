/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
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
#if DEVICE_RTC

#include "rtc_api.h"
#include "mbed_mktime.h"

#define BKP_VALUE    0x32F0
#define RTC_SUNDAY ((uint8_t)0x07)

static uint8_t rtc_init_flag = 0;
static uint32_t prescaler_a = 0, prescaler_s = 0;
rtc_parameter_struct rtc_initpara;
static uint8_t rtc_bcd_to_byte(uint8_t value);
static unsigned int rtc_hex_to_bcd(unsigned char hex_data);

/** Initialize the RTC peripheral
 *
 * Powerup the RTC in perpetration for access. This function must be called
 * before any other RTC functions ares called. This does not change the state
 * of the RTC. It just enables access to it.
 *
 * @note This function is safe to call repeatedly - Tested by ::rtc_init_test
 */
void rtc_init(void)
{
    /* make sure RTC only init once */
    if (rtc_init_flag) {
        return;
    }
    rtc_init_flag = 1;

    /* enable PMU and BKPI clocks */
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();

    rcu_osci_on(RCU_LXTAL);
    rcu_osci_stab_wait(RCU_LXTAL);
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
    prescaler_s = 0xFF;
    prescaler_a = 0x7F;

    /* enable RTC clock */
    rcu_periph_clock_enable(RCU_RTC);
    /* wait for RTC registers synchronization */
    rtc_register_sync_wait();

    rtc_initpara.factor_asyn = prescaler_a;
    rtc_initpara.factor_syn = prescaler_s;
    rtc_initpara.year = 0x16;
    rtc_initpara.day_of_week = RTC_SATURDAY;
    rtc_initpara.month = RTC_APR;
    rtc_initpara.date = 0x30;
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;
    if (rcu_flag_get(RCU_FLAG_SWRST) != RESET) {
        rcu_all_reset_flag_clear();
        return;
    } else {
        rtc_para_init(&rtc_initpara);
    }
}

/** Deinitialize RTC
 *
 * Powerdown the RTC in preparation for sleep, powerdown or reset. That should only
 * affect the CPU domain and not the time keeping logic.
 * After this function is called no other RTC functions should be called
 * except for ::rtc_init.
 */
void rtc_free(void)
{
}

/** Check if the RTC has the time set and is counting
 *
 * @retval 0 The time reported by the RTC is not valid
 * @retval 1 The time has been set the RTC is counting
 */
int rtc_isenabled(void)
{
    if (RESET == rtc_flag_get(RTC_FLAG_YCM)) {
        return 0;
    } else {
        return 1;
    }
}

/** Get the current time from the RTC peripheral
 *
 * @return The current time in seconds
 *
 * @note Some RTCs are not synchronized with the main clock. If
 * this is the case with your RTC then you must read the RTC time
 * in a loop to prevent reading the wrong time due to a glitch.
 * The test ::rtc_glitch_test is intended to catch this bug.
 */
time_t rtc_read(void)
{
    time_t t;
    struct tm tm_para;

    /* wait the RSYNF flag to set */
    while (0 == rtc_register_sync_wait());
    rtc_current_time_get(&rtc_initpara);

    tm_para.tm_year = rtc_initpara.year;
    tm_para.tm_mon = rtc_initpara.month;
    tm_para.tm_mday = rtc_initpara.date;
    tm_para.tm_wday = rtc_initpara.day_of_week;
    tm_para.tm_hour = rtc_initpara.hour;
    tm_para.tm_min = rtc_initpara.minute;
    tm_para.tm_sec = rtc_initpara.second;

    /* get current time and date once again */
    while (0 == rtc_register_sync_wait());
    rtc_current_time_get(&rtc_initpara);

    /* compare the read result of first and second, if not equal, get current time and date once again */
    if ((tm_para.tm_year != rtc_initpara.year) || (tm_para.tm_mon != rtc_initpara.month) || \
            (tm_para.tm_mday != rtc_initpara.date) || (tm_para.tm_wday != rtc_initpara.day_of_week) || \
            (tm_para.tm_hour != rtc_initpara.hour) || (tm_para.tm_min != rtc_initpara.minute) || (tm_para.tm_sec != rtc_initpara.second)) {
        while (0 == rtc_register_sync_wait());
        rtc_current_time_get(&rtc_initpara);
    }

    tm_para.tm_year = rtc_bcd_to_byte(rtc_initpara.year) + 68;
    tm_para.tm_mon  = rtc_bcd_to_byte(rtc_initpara.month) - 1;
    tm_para.tm_mday = rtc_bcd_to_byte(rtc_initpara.date);
    tm_para.tm_hour = rtc_bcd_to_byte(rtc_initpara.hour);
    tm_para.tm_min  = rtc_bcd_to_byte(rtc_initpara.minute);
    tm_para.tm_sec  = rtc_bcd_to_byte(rtc_initpara.second);

    /*convert calendar to timestamp*/
    if (_rtc_maketime(&tm_para, &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return 0;
    }

    return t;
}

/** Write the current time in seconds to the RTC peripheral
 *
 * @param t The current time to be set in seconds.
 */
void rtc_write(time_t t)
{
    struct tm tm_para;

    /* convert t into structure tm_para */
    if (_rtc_localtime(t, &tm_para, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return;
    }

    if (tm_para.tm_wday == 0) {
        rtc_initpara.day_of_week = RTC_SUNDAY;
    } else {
        rtc_initpara.day_of_week = tm_para.tm_wday;
    }
    rtc_initpara.month  = rtc_hex_to_bcd(tm_para.tm_mon + 1);
    rtc_initpara.date   = rtc_hex_to_bcd(tm_para.tm_mday);
    rtc_initpara.year   = rtc_hex_to_bcd(tm_para.tm_year - 68);
    rtc_initpara.hour   = rtc_hex_to_bcd(tm_para.tm_hour);
    rtc_initpara.minute = rtc_hex_to_bcd(tm_para.tm_min);
    rtc_initpara.second = rtc_hex_to_bcd(tm_para.tm_sec);
    rtc_para_init(&rtc_initpara);
}

/* converts from BCD to Binary */
static uint8_t rtc_bcd_to_byte(uint8_t value)
{
    uint32_t tmp = 0U;
    tmp = ((uint8_t)(value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
    return (tmp + (value & (uint8_t)0x0F));
}

/* converts from HEX to BCD */
static unsigned int rtc_hex_to_bcd(uint8_t hex_data)
{
    uint32_t bcdhigh = 0U;

    while (hex_data >= 10U) {
        bcdhigh++;
        hex_data -= 10U;
    }

    return ((uint8_t)(bcdhigh << 4U) | hex_data);
}
#endif /* DEVICE_RTC */

