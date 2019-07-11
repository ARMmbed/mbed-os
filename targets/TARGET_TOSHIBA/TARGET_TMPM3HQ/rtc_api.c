/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include "rtc_api.h"
#include "mbed_mktime.h"

#define RTC_24_HOUR_MODE                ((uint8_t)0x01)
#define PAGER_PAGE_ONE                  ((uint8_t)0x01)
#define PAGER_PAGE_ZERO                 ((uint8_t)0xEE)
#define RTC_CLK_ENABLE                  ((uint8_t)0x08)
#define RTC_CLK_DISABLE                 ((uint8_t)0xE7)
#define RTCRESTR_RSTTMR_MASK            ((uint8_t)0x20)
#define RTCRESTR_RSTTMR_R_RUN           ((uint8_t)0x20)
#define CGWUPLCR_WUPTL_HIGH_MASK        ((uint32_t)0x07FFF000)
#define CGWUPLCR_WULEF_MASK             ((uint32_t)0x00000002)
#define CGWUPLCR_WULEF_R_DONE           ((uint32_t)0x00000000)
#define CGWUPLCR_WULON_W_ENABLE         ((uint32_t)0x00000001)
#define RLMLOSCCR_XTEN_RW_ENABLE        ((uint32_t)0x00000001)
#define ELOSC_CFG_WARM_UP_TIME          ((uint64_t)(5000))
#define ELOSC_CFG_CLOCK                 ((uint64_t)(32768))
#define HEX2DEC(val)                    ((val >> 4U) * 10U + val % 16U)  // Hex to Dec conversion macro
#define DEC2HEX(val)                    ((val / 10U) * 16U + val % 10U)  // Dec to Hex conversion macro

static int rtc_inited = 0;
static int diff_year = 100;         //our RTC register only support 2000~2099
static void external_losc_enable(void);

void rtc_init(void)
{
    if (!rtc_inited) {
        TSB_CG_FSYSENA_IPENA23 = 1;  // Enable Sys Clock for RTC
        external_losc_enable();      // Enable low-speed oscillator

        TSB_RTC->PAGER = 0x00;       // Disable clock and alarm

        while ((TSB_RTC->RESTR & RTCRESTR_RSTTMR_MASK) == RTCRESTR_RSTTMR_R_RUN) {
            // Reset RTC sec counter
        }

        TSB_RTC->RESTR = 0xE7;

        while ((TSB_RTC->RESTR & RTCRESTR_RSTTMR_MASK) == RTCRESTR_RSTTMR_R_RUN) {
            // Reset RTC sec counter
        }

        TSB_RTC->PAGER   |= PAGER_PAGE_ONE;
        TSB_RTC->YEARR   = 0x03;              // Set leap year state
        TSB_RTC->MONTHR  = RTC_24_HOUR_MODE;  // Set hour mode
        TSB_RTC->PAGER  &= PAGER_PAGE_ZERO;   // Set hour mode
        TSB_RTC->YEARR   = 0x01;              // Set year value
        TSB_RTC->MONTHR  = (uint8_t)0x01;     // Set month value
        TSB_RTC->DATER   = (uint8_t)0x01;     // Set date value
        TSB_RTC->DAYR    = (uint8_t)0x0;      // Set day value
        TSB_RTC->HOURR   = (uint8_t)0x01;     // Set hour value
        TSB_RTC->MINR    = (uint8_t)0x02;     // Set minute value
        TSB_RTC->SECR    = (uint8_t)0x22;     // Set second value
        TSB_RTC->PAGER  |= RTC_CLK_ENABLE;    // Enable Clock
        rtc_inited       = 1;                 // Enable RTC initialzed status
    }
}

void rtc_free(void)
{
    rtc_inited = 0;  // Set status of RTC peripheral driver as DISABLE
}

int rtc_isenabled(void)
{
    return rtc_inited;  // Return status of RTC peripheral driver
}

time_t rtc_read(void)
{
    if (!rtc_inited) {
        // Return invalid time for now!
        return 0;
    }

    struct tm timeinfo;
    uint8_t read_1 = 0U;
    uint8_t read_2 = 0U;

    timeinfo.tm_isdst = 0;//no summer time

    TSB_RTC->PAGER &= PAGER_PAGE_ZERO;

    read_1 = TSB_RTC->SECR;  // Get sec value
    timeinfo.tm_sec = HEX2DEC(read_1);

    do {  // Get minute value
        read_1 = TSB_RTC->MINR;
        read_2 = TSB_RTC->MINR;
    } while (read_1 != read_2);
    timeinfo.tm_min = HEX2DEC(read_1);

    do {  // Get hour value
        read_1 = TSB_RTC->HOURR;
        read_2 = TSB_RTC->HOURR;
    } while (read_1 != read_2);
    timeinfo.tm_hour = HEX2DEC(read_1);

    do {  // Get Month date value
        read_1 = TSB_RTC->DATER;
        read_2 = TSB_RTC->DATER;
    } while (read_1 != read_2);
    timeinfo.tm_mday = HEX2DEC(read_1);

    do {  // Get Month value
        read_1 = TSB_RTC->MONTHR;
        read_2 = TSB_RTC->MONTHR;
    } while (read_1 != read_2);
    timeinfo.tm_mon = HEX2DEC(read_1)-1;

    do {  // Get weekday value
        read_1 = TSB_RTC->DAYR;
        read_2 = TSB_RTC->DAYR;
    } while (read_1 != read_2);
    timeinfo.tm_wday = HEX2DEC(read_1);

    do {  // Get year value
        read_1 = TSB_RTC->YEARR;
        read_2 = TSB_RTC->YEARR;
    } while (read_1 != read_2);
    timeinfo.tm_year = (HEX2DEC(read_1)+ diff_year);

    time_t t;

    if (_rtc_maketime(&timeinfo, &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return 0;
    }
    return t;
}

void rtc_write(time_t t)
{
    if (!rtc_inited) {
        // Initialize the RTC as not yet initialized
        rtc_init();
    }

    struct tm timeinfo;
    if (_rtc_localtime(t, &timeinfo, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return;
    }

    diff_year = timeinfo.tm_year - (timeinfo.tm_year % 100);
    TSB_RTC->PAGER &= RTC_CLK_DISABLE;    // Disable clock

    // Check current year is leap year or not
    if (((timeinfo.tm_year % 4) == 0 && (timeinfo.tm_year % 100) != 0) ||
            (timeinfo.tm_year % 400) == 0) {
        TSB_RTC->PAGER |= PAGER_PAGE_ONE;  // Current year is a leap year
        TSB_RTC->YEARR = 0x00;
    } else if ((timeinfo.tm_year % 4) == 1) {
        TSB_RTC->PAGER |= PAGER_PAGE_ONE;  // Current year is the year following a leap year
        TSB_RTC->YEARR = 0x01;
    } else if ((timeinfo.tm_year % 4) == 2) {
        TSB_RTC->PAGER |= PAGER_PAGE_ONE;  // Current year is two years after a leap year
        TSB_RTC->YEARR = 0x02;
    } else {
        TSB_RTC->PAGER |= PAGER_PAGE_ONE;  // Current year is three years after a leap year
        TSB_RTC->YEARR = 0x03;
    }

    TSB_RTC->PAGER &= PAGER_PAGE_ZERO;                                    // Select PAGE 0

    TSB_RTC->YEARR  = (uint8_t)DEC2HEX((timeinfo.tm_year - diff_year));  // Set year value
    // Set month value, tm_mon=0 means Jan while 1 is Jan
    TSB_RTC->MONTHR = (uint8_t)DEC2HEX((timeinfo.tm_mon+1));
    TSB_RTC->DATER  = (uint8_t)DEC2HEX(timeinfo.tm_mday);                // Set date value
    TSB_RTC->DAYR   = (uint8_t)(timeinfo.tm_wday);                       // Set week day value
    TSB_RTC->HOURR  = (uint8_t)DEC2HEX(timeinfo.tm_hour);                // Set hour value
    TSB_RTC->MINR   = (uint8_t)DEC2HEX(timeinfo.tm_min);                 // Set minute value
    TSB_RTC->SECR   = (uint8_t)DEC2HEX(timeinfo.tm_sec);                 // Set second value

    TSB_RTC->RESTR |= RTCRESTR_RSTTMR_R_RUN;
    while ((TSB_RTC->RESTR & RTCRESTR_RSTTMR_MASK) == RTCRESTR_RSTTMR_R_RUN) {
        // Reset RTC sec counter, otherwise the 1st second will not be accurate
    }

    // Setting Wait
    // When stop mode is selected, CaseA or CaseB is need.
    // CaseA: Wait for RTC 1Hz interrupt.
    // CaseB: Check the clock register setting.
    {
        uint8_t flag = 1;
        time_t time_read = {0};
        while(flag) {
            time_read = rtc_read();
            if( time_read == t) {      // Wait for setting successfully
                flag = 0;
            }
        }
    }
    TSB_RTC->PAGER |= RTC_CLK_ENABLE;  // Enable Clock
}

static void external_losc_enable(void)
{
    uint32_t work;
    if( (TSB_RLM->LOSCCR & 0x01) == 0 ) {   //external losc is not enabled.
        uint64_t x = (uint64_t)(ELOSC_CFG_WARM_UP_TIME * ELOSC_CFG_CLOCK);
        x = (uint64_t)(x / (uint64_t)(1000000));
        work = (uint32_t)x;
        work  &= (uint32_t)(0xFFFFFFF0);
        work <<= 8;
        TSB_CG->WUPLCR = work;
        TSB_RLM->LOSCCR = RLMLOSCCR_XTEN_RW_ENABLE;
        work = (uint32_t)(TSB_CG->WUPLCR & CGWUPLCR_WUPTL_HIGH_MASK);
        TSB_CG->WUPLCR = (uint32_t)(work | CGWUPLCR_WULON_W_ENABLE);
        while ((TSB_CG->WUPLCR & CGWUPLCR_WULEF_MASK) != CGWUPLCR_WULEF_R_DONE) {
            // No processing
        }
    }
}
