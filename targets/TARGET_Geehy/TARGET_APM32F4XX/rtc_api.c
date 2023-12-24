/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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
#include "apm32f4xx_rtc.h"

#define BKP_VALUE    0x32F0

static uint8_t rtc_Init_flag = 0;
static uint32_t a_prescaler = 0, s_prescaler = 0;
RTC_Config_T rtcConfig;
static uint8_t rtc_bcd_to_byte(uint8_t val);
static unsigned int rtc_hex_to_bcd(unsigned char hex_dat);

 /*!
 * @brief     Initialize the RTC peripheral
 *
 * @param     None
 *
 * @retval    None
 */
void rtc_init(void)
{
    RTC_DateConfig_T dateConfig;
    RTC_TimeConfig_T TimeStruct;
    /* make sure RTC only init once */
    if (rtc_Init_flag)
    {
        return;
    }
    rtc_Init_flag = 1;

    /* enable PMU and BKPI clocks */
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);
    /* allow access to BKP domain */
    PMU_EnableBackupAccess();
    RCM_EnableLSI();

    /* Wait till LSE is ready */
    while (RCM_ReadStatusFlag(RCM_FLAG_LSIRDY) == RESET)
    {
    }
    RCM_ConfigRTCCLK(RCM_RTCCLK_LSI);
    RCM_EnableRTCCLK();

    /* Disable Write Proctection */
    RTC_DisableWriteProtection();

    /* Wait Synchro*/
    RTC_WaitForSynchro();

    RTC_ConfigStructInit(&rtcConfig);

    RTC_EnableInit();

    RTC_Config(&rtcConfig);

    /* Config Time */
    TimeStruct.h12 = RTC_H12_AM;
    TimeStruct.hours = 0;
    TimeStruct.minutes = 0;
    TimeStruct.seconds = 0;
    RTC_ConfigTime(RTC_FORMAT_BCD,&TimeStruct);

    dateConfig.month = RTC_MONTH_OCTOBER;
    dateConfig.date = 0x30;
    dateConfig.weekday = RTC_WEEKDAY_THURSDAY;
    dateConfig.year = 0x16;
    RTC_ConfigDate(RTC_FORMAT_BCD,&dateConfig);

}

 /*!
 * @brief     Deinitialize RTC.Powerdown the RTC in preparation for sleep, powerdown or reset. That should only
 *            affect the CPU domain and not the time keeping logic.
 * @param     None
 *
 * @retval    None
 */
void rtc_free(void)
{
}

 /*!
 * @brief     Check if the RTC has the time set and is counting
 *
 * @param     None
 *
 * @retval    0 The time reported by the RTC is not valid,1 The time has been set the RTC is counting
 */
int rtc_isenabled(void)
{
    return RTC_ReadStatusFlag(RTC_FLAG_ISF);
}

 /*!
 * @brief     Get the current time from the RTC peripheral
 *
 * @param     None
 *
 * @retval    The current time in seconds
 */
time_t rtc_read(void)
{
    time_t t;
    RTC_TimeConfig_T timeConfig;
    RTC_DateConfig_T dateConfig;
    struct tm tm_para;

    RTC_ReadTime(RTC_FORMAT_BCD,&timeConfig);
    RTC_ReadDate(RTC_FORMAT_BCD,&dateConfig);

    tm_para.tm_year = dateConfig.year;
    tm_para.tm_mon = dateConfig.month;
    tm_para.tm_mday = dateConfig.date;
    tm_para.tm_wday = dateConfig.weekday;
    tm_para.tm_hour = timeConfig.hours;
    tm_para.tm_min = timeConfig.minutes;
    tm_para.tm_sec = timeConfig.seconds;

    RTC_ReadTime(RTC_FORMAT_BCD,&timeConfig);

    RTC_ReadDate(RTC_FORMAT_BCD,&dateConfig);

    /* compare the read result of first and second, if not equal, get current time and date once again */
    if ((tm_para.tm_year != dateConfig.year) || (tm_para.tm_mon != dateConfig.month) || \
            (tm_para.tm_mday != dateConfig.date) || (tm_para.tm_wday != dateConfig.weekday) || \
            (tm_para.tm_hour != timeConfig.hours) || (tm_para.tm_min != timeConfig.minutes) || \
            (tm_para.tm_sec != timeConfig.seconds)) 
    {
        while ( RTC_WaitForSynchro() != SUCCESS);
        RTC_ReadTime(RTC_FORMAT_BCD,&timeConfig);
        RTC_ReadDate(RTC_FORMAT_BCD,&dateConfig);
    }

    tm_para.tm_year = rtc_bcd_to_byte(dateConfig.year) + 68;
    tm_para.tm_mon  = rtc_bcd_to_byte(dateConfig.month) - 1;
    tm_para.tm_mday = rtc_bcd_to_byte(dateConfig.date);
    tm_para.tm_hour = rtc_bcd_to_byte(timeConfig.hours);
    tm_para.tm_min  = rtc_bcd_to_byte(timeConfig.minutes);
    tm_para.tm_sec  = rtc_bcd_to_byte(timeConfig.seconds);
    
    /*convert calendar to timestamp*/
    if (_rtc_maketime(&tm_para, &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false)
    {
        return 0;
    }    

    return t;
}

 /*!
 * @brief     Write the current time in seconds to the RTC peripheral
 *
 * @param     t :The current time to be set in seconds
 *
 * @retval    None
 */
void rtc_write(time_t t)
{
    struct tm tm_para;
    RTC_TimeConfig_T timeConfig;
    RTC_DateConfig_T dateConfig;

    /* convert t into structure tm_para */
    if (_rtc_localtime(t, &tm_para, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false)
    {
        return;
    }

    if (tm_para.tm_wday == 0)
    {
        dateConfig.weekday = RTC_WEEKDAY_SUNDAY;
    }
    else
    {
        dateConfig.weekday = tm_para.tm_wday;
    }
    dateConfig.month   = tm_para.tm_mon + 1;
    dateConfig.date    = tm_para.tm_mday;
    dateConfig.year    = tm_para.tm_year - 68;
    timeConfig.hours   = tm_para.tm_hour;
    timeConfig.minutes = tm_para.tm_min;
    timeConfig.seconds = tm_para.tm_sec;

    RTC_ConfigDate(RTC_FORMAT_BIN,&dateConfig);
    RTC_ConfigTime(RTC_FORMAT_BIN,&timeConfig);
}

 /*!
 * @brief     converts from BCD to Binary
 *
 * @param     val
 *
 * @retval
 */
static uint8_t rtc_bcd_to_byte(uint8_t val)
{
    uint32_t tmp = 0U;
    tmp = ((uint8_t)(val & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
    return (tmp + (val & (uint8_t)0x0F));
}

 /*!
 * @brief     converts from HEX to BCD
 *
 * @param     hex_data
 *
 * @retval    None
 */
static unsigned int rtc_hex_to_bcd(uint8_t hex_data)
{
    uint32_t bcdhigh = 0U;

    while (hex_data >= 10U)
    {
        bcdhigh++;
        hex_data -= 10U;
    }

    return ((uint8_t)(bcdhigh << 4U) | hex_data);
}

#endif /* DEVICE_RTC */

