/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "device.h"
#include "rtc_api.h"
#include "mbed_error.h"
#include "mbed_mktime.h"
#include "cy_rtc.h"


#if DEVICE_RTC

/*
 * Since Mbed tests insist on supporting 1970 - 2106 years range
 * and Cypress h/w supports only 2000 - 2099 years range,
* two backup registers are used to flag century correction.
 */
#define BR_LAST_YEAR_READ      14
#define BR_CENTURY_CORRECTION  15

static int enabled = 0;

static uint32_t rtc_read_convert_year(uint32_t short_year)
{
    uint32_t century = BACKUP->BREG[BR_CENTURY_CORRECTION];

    if (BACKUP->BREG[BR_LAST_YEAR_READ] > short_year) {
        BACKUP->BREG[BR_CENTURY_CORRECTION] = ++century;
    }
    BACKUP->BREG[BR_LAST_YEAR_READ] = short_year;

    return century * 100 + short_year;
}

static uint32_t rtc_write_convert_year(uint32_t long_year)
{
    uint32_t short_year = long_year;
    uint32_t century = short_year / 100;
    short_year -= century * 100;
    BACKUP->BREG[BR_CENTURY_CORRECTION] = century;
    BACKUP->BREG[BR_LAST_YEAR_READ] = short_year;
    return short_year;
}

void rtc_init(void)
{
    static cy_stc_rtc_config_t init_val = {
        /* Time information */
        .hrFormat = CY_RTC_24_HOURS,
        .sec     = 0,
        .min     = 0,
        .hour    = 0,
        .dayOfWeek = CY_RTC_SATURDAY,
        .date = 1,
        .month = 1,
        .year = 0       // 2000 - 30 == 1970
    };
    cy_stc_rtc_config_t cy_time;

    if (!enabled) {
        // Setup power management callback.
        // Setup century interrupt.
        // Verify RTC time consistency.
        Cy_RTC_GetDateAndTime(&cy_time);
        if ( CY_RTC_IS_SEC_VALID(cy_time.sec) &&
                CY_RTC_IS_MIN_VALID(cy_time.min) &&
                CY_RTC_IS_HOUR_VALID(cy_time.hour) &&
                CY_RTC_IS_DOW_VALID(cy_time.dayOfWeek) &&
                CY_RTC_IS_MONTH_VALID(cy_time.month) &&
                CY_RTC_IS_YEAR_SHORT_VALID(cy_time.year) &&
                (cy_time.hrFormat == CY_RTC_24_HOURS)) {
            enabled = 1;
        } else {
            // reinitialize
            init_val.year = rtc_write_convert_year(1970);
            if (Cy_RTC_Init(&init_val) == CY_RTC_SUCCESS) {
                enabled = 1;
            }
        }
    }
}

void rtc_free(void)
{
    // Nothing to do
}

int rtc_isenabled(void)
{
    return enabled;
}

time_t rtc_read(void)
{
    cy_stc_rtc_config_t cy_time;
    struct              tm gmt;
    time_t              timestamp = 0;
    uint32_t            interrupt_state;

    // Since RTC reading function is unreliable when the RTC is busy with previous update
    // we have to make sure it's not before calling it.
    while (CY_RTC_BUSY == Cy_RTC_GetSyncStatus()) {}

    interrupt_state = Cy_SysLib_EnterCriticalSection();
    Cy_RTC_GetDateAndTime(&cy_time);
    gmt.tm_sec     = cy_time.sec;
    gmt.tm_min     = cy_time.min;
    gmt.tm_hour    = cy_time.hour;
    gmt.tm_mday    = cy_time.date;
    gmt.tm_mon     = cy_time.month - 1;
    gmt.tm_year    = rtc_read_convert_year(cy_time.year);
    gmt.tm_isdst   = 0;
    Cy_SysLib_ExitCriticalSection(interrupt_state);

    _rtc_maketime(&gmt, &timestamp, RTC_4_YEAR_LEAP_YEAR_SUPPORT);
    return timestamp;
}

void rtc_write(time_t t)
{
    cy_en_rtc_status_t  status;
    struct tm           gmt;

    if ( _rtc_localtime(t, &gmt, RTC_4_YEAR_LEAP_YEAR_SUPPORT)) {
        uint32_t year;
        uint32_t interrupt_state;
        // Make sure RTC is not busy and can be updated.
        while (CY_RTC_BUSY == Cy_RTC_GetSyncStatus()) {}

        interrupt_state = Cy_SysLib_EnterCriticalSection();
        year = rtc_write_convert_year(gmt.tm_year);
        status = Cy_RTC_SetDateAndTimeDirect(gmt.tm_sec,
                                             gmt.tm_min,
                                             gmt.tm_hour,
                                             gmt.tm_mday,
                                             gmt.tm_mon + 1,
                                             year);
        Cy_SysLib_ExitCriticalSection(interrupt_state);
        if (status != CY_RTC_SUCCESS) {
            error("Error 0x%x while setting RTC time.", status);
        }
    }
}

#endif // DEVICE_RTC
