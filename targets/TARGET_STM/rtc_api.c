/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#if DEVICE_RTC

#include <stdbool.h>

#include "rtc_api.h"
#include "rtc_api_hal.h"
#include "mbed_error.h"

static RTC_HandleTypeDef RtcHandle;

#if RTC_LSI
#define RTC_CLOCK LSI_VALUE
#else
#define RTC_CLOCK LSE_VALUE
#endif

#if DEVICE_LOWPOWERTIMER
#define RTC_ASYNCH_PREDIV ((RTC_CLOCK - 1) / 0x8000)
#define RTC_SYNCH_PREDIV  (RTC_CLOCK / (RTC_ASYNCH_PREDIV + 1) - 1)
#else
#define RTC_ASYNCH_PREDIV (0x007F)
#define RTC_SYNCH_PREDIV  (RTC_CLOCK / (RTC_ASYNCH_PREDIV + 1) - 1)
#endif

#if DEVICE_LOWPOWERTIMER
static void (*irq_handler)(void);
static void RTC_IRQHandler(void);
#endif

/*
 * time constants 
 */
#define SECONDS_BY_MINUTES 60
#define MINUTES_BY_HOUR 60
#define SECONDS_BY_HOUR (SECONDS_BY_MINUTES * MINUTES_BY_HOUR)
#define HOURS_BY_DAY 24 
#define SECONDS_BY_DAY (SECONDS_BY_HOUR * HOURS_BY_DAY)

/*
 * 2 dimensional array containing the number of seconds elapsed before a given 
 * month.
 * The second index map to the month while the first map to the type of year:
 *   - 0: non leap year 
 *   - 1: leap year
 */
static const uint32_t seconds_before_month[2][12] = {
    {
        0,
        31 * SECONDS_BY_DAY,
        (31 + 28) * SECONDS_BY_DAY,
        (31 + 28 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
    },
    {
        0,
        31 * SECONDS_BY_DAY,
        (31 + 29) * SECONDS_BY_DAY,
        (31 + 29 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
    }
};

/*
 * Compute if a year is a leap year or not. 
 * year 0 is translated into year 1900 CE.
 */
bool st_rtc_is_leap_year(int year) {
    /* 
     * since in practice, the value manipulated by this algorithm lie in the 
     * range [70 : 138], the algorith can be reduced to: year % 4.
     * The algorithm valid over the full range of value is: 

        year = 1900 + year;
        if (year % 4) {
            return false;
        } else if (year % 100) {
            return true;
        } else if (year % 400) {
            return false;
        }
        return true;

     */ 
    return (year) % 4 ? false : true;
}

/*
 * Thread safe (partial) replacement for mktime. 
 * This function is tailored around ST RTC specification and is not a full 
 * replacement for mktime. 
 * The fields from tm used are:
 *   - tm_sec
 *   - tm_min
 *   - tm_hour
 *   - tm_mday
 *   - tm_mon
 *   - tm_year
 * Other fields are ignored and won't be normalized by the call.
 * If the time in input is less than UNIX epoch (1st january of 1970 at 00:00:00),
 * then this function consider the input as invalid and will return time_t(-1).
 * Values in output range from 0 to INT_MAX.
 * Leap seconds are not supported.
 */  
time_t st_rtc_mktime(const struct tm* time) {
    // partial check for the upper bound of the range
    // normalization might happen at the end of the function 
    // this solution is faster than checking if the input is after the 19th of 
    // january 2038 at 03:14:07.  
    if ((time->tm_year < 70) || (time->tm_year > 138)) { 
        return ((time_t) -1);
    }

    uint32_t result = time->tm_sec;
    result += time->tm_min * SECONDS_BY_MINUTES;
    result += time->tm_hour * SECONDS_BY_HOUR;
    result += (time->tm_mday - 1) * SECONDS_BY_DAY;
    result += seconds_before_month[st_rtc_is_leap_year(time->tm_year)][time->tm_mon];

    if (time->tm_year > 70) { 
        // valid in the range [70:138] 
        uint32_t count_of_leap_days = ((time->tm_year - 1) / 4) - (70 / 4);
        result += (((time->tm_year - 70) * 365) + count_of_leap_days) * SECONDS_BY_DAY;
    }

    if (result > INT32_MAX) { 
        return -1;
    }

    return result;
}

/* 
 * Thread safe (partial) replacement for localtime. 
 * This function is tailored around ST RTC specification and is not a full 
 * replacement for localtime.
 * The tm fields filled by this function are:
 *   - tm_sec
 *   - tm_min
 *   - tm_hour
 *   - tm_mday
 *   - tm_mon
 *   - tm_year
 *   - tm_wday
 * The time in input shall be in the range [0, INT32_MAX] otherwise the function 
 * will return false and the structure time_info in input will remain untouch.
 */
bool st_rtc_localtime(time_t timestamp, struct tm* time_info) {
    if (((int32_t) timestamp) < 0) { 
        return false;
    } 

    time_info->tm_sec = timestamp % 60;
    timestamp = timestamp / 60;   // timestamp in minutes
    time_info->tm_min = timestamp % 60;
    timestamp = timestamp / 60;  // timestamp in hours
    time_info->tm_hour = timestamp % 24;
    timestamp = timestamp / 24;  // timestamp in days;

    // compute the weekday
    // The 1st of January 1970 was a Thursday which is equal to 4 in the weekday
    // representation ranging from [0:6]
    time_info->tm_wday = (timestamp + 4) % 7;

    // years start at 70
    time_info->tm_year = 70;
    while (true) { 
        if (st_rtc_is_leap_year(time_info->tm_year) && timestamp >= 366) { 
            ++time_info->tm_year;
            timestamp -= 366;
        } else if (!st_rtc_is_leap_year(time_info->tm_year) && timestamp >= 365) { 
            ++time_info->tm_year;
            timestamp -= 365;
        } else {
            // the remaining days are less than a years
            break;
        }
    }

    // convert days into seconds and find the current month
    timestamp *= SECONDS_BY_DAY;
    time_info->tm_mon = 11;
    bool leap = st_rtc_is_leap_year(time_info->tm_year);
    for (uint32_t i = 0; i < 12; ++i) {
        if ((uint32_t) timestamp < seconds_before_month[leap][i]) {
            time_info->tm_mon = i - 1;
            break;
        }
    }

    // remove month from timestamp and compute the number of days.
    // note: unlike other fields, days are not 0 indexed.
    timestamp -= seconds_before_month[leap][time_info->tm_mon];
    time_info->tm_mday = (timestamp / SECONDS_BY_DAY) + 1;

    return true;
}

void rtc_init(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    // Enable access to Backup domain
    HAL_PWR_EnableBkUpAccess();

    RtcHandle.Instance = RTC;

#if !RTC_LSI
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // Mandatory, otherwise the PLL is reconfigured!
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON;
    RCC_OscInitStruct.LSIState       = RCC_LSI_OFF;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK) {
        __HAL_RCC_RTC_CLKPRESCALER(RCC_RTCCLKSOURCE_LSE);
        __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
    } else {
        error("Cannot initialize RTC with LSE\n");
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        error("PeriphClkInitStruct RTC failed with LSE\n");
    }
#else /* !RTC_LSI */
    __HAL_RCC_PWR_CLK_ENABLE();

    // Reset Backup domain
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();

    // Enable LSI clock
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // Mandatory, otherwise the PLL is reconfigured!
    RCC_OscInitStruct.LSEState       = RCC_LSE_OFF;
    RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error("Cannot initialize RTC with LSI\n");
    }

    __HAL_RCC_RTC_CLKPRESCALER(RCC_RTCCLKSOURCE_LSI);
    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSI);

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        error("PeriphClkInitStruct RTC failed with LSI\n");
    }
#endif /* !RTC_LSI */

    // Enable RTC
    __HAL_RCC_RTC_ENABLE();

#if TARGET_STM32F1
    RtcHandle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
#else /* TARGET_STM32F1 */
    RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
    RtcHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
    RtcHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
    RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
#endif /* TARGET_STM32F1 */

    if (HAL_RTC_Init(&RtcHandle) != HAL_OK) {
        error("RTC error: RTC initialization failed.");
    }

#if DEVICE_LOWPOWERTIMER

#if !RTC_LSI
    if (!rtc_isenabled())
#endif /* !RTC_LSI */
    {
        rtc_write(0);
    }

    NVIC_ClearPendingIRQ(RTC_WKUP_IRQn);
    NVIC_DisableIRQ(RTC_WKUP_IRQn);
    NVIC_SetVector(RTC_WKUP_IRQn, (uint32_t)RTC_IRQHandler);
    NVIC_EnableIRQ(RTC_WKUP_IRQn);

#endif /* DEVICE_LOWPOWERTIMER */
}

void rtc_free(void)
{
#if RTC_LSI
    // Enable Power clock
    __HAL_RCC_PWR_CLK_ENABLE();

    // Enable access to Backup domain
    HAL_PWR_EnableBkUpAccess();

    // Reset Backup domain
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();

    // Disable access to Backup domain
    HAL_PWR_DisableBkUpAccess();
#endif

    // Disable LSI and LSE clocks
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
    RCC_OscInitStruct.LSIState       = RCC_LSI_OFF;
    RCC_OscInitStruct.LSEState       = RCC_LSE_OFF;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
}

/*
 ST RTC_DateTypeDef structure
   WeekDay 1=monday, 2=tuesday, ..., 7=sunday
   Month   0x1=january, 0x2=february, ..., 0x12=december
   Date    day of the month 1-31
   Year    year 0-99

 ST RTC_TimeTypeDef structure
  Hours           0-12 if the RTC_HourFormat_12 is selected during init
                  0-23 if the RTC_HourFormat_24 is selected during init
  Minutes         0-59
  Seconds         0-59
  TimeFormat      RTC_HOURFORMAT12_AM/RTC_HOURFORMAT12_PM
  SubSeconds      time unit range between [0-1] Second with [1 Sec / SecondFraction +1] granularity
  SecondFraction  range or granularity of Sub Second register content corresponding to Synchronous pre-scaler factor value (PREDIV_S)
  DayLightSaving  RTC_DAYLIGHTSAVING_SUB1H/RTC_DAYLIGHTSAVING_ADD1H/RTC_DAYLIGHTSAVING_NONE
  StoreOperation  RTC_STOREOPERATION_RESET/RTC_STOREOPERATION_SET

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

/*
Information about STM32F0, STM32F2, STM32F3, STM32F4, STM32F7, STM32L0, STM32L1, STM32L4:
BCD format is used to store the date in the RTC. The year is store on 2 * 4 bits.
Because the first year is reserved to see if the RTC is init, the supposed range is 01-99.
1st point is to cover the standard range from 1970 to 2038 (limited by the 32 bits of time_t).
2nd point is to keep the year 1970 and the leap years synchronized.

So by moving it 68 years forward from 1970, it become 1969-2067 which include 1970-2038.
68 is also a multiple of 4 so it let the leap year synchronized.

Information about STM32F1:
32bit register is used (no BCD format) for the seconds and a software structure to store dates.
It is then not a problem to not use shifts.
*/

time_t rtc_read(void)
{
    RTC_DateTypeDef dateStruct;
    RTC_TimeTypeDef timeStruct;
    struct tm timeinfo;

    RtcHandle.Instance = RTC;

    // Read actual date and time
    // Warning: the time must be read first!
    HAL_RTC_GetTime(&RtcHandle, &timeStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&RtcHandle, &dateStruct, RTC_FORMAT_BIN);

    // Setup a tm structure based on the RTC
    /* tm_wday information is ignored by mktime */
    timeinfo.tm_mon  = dateStruct.Month - 1;
    timeinfo.tm_mday = dateStruct.Date;
    timeinfo.tm_year = dateStruct.Year + 68;
    timeinfo.tm_hour = timeStruct.Hours;
    timeinfo.tm_min  = timeStruct.Minutes;
    timeinfo.tm_sec  = timeStruct.Seconds;
    // Daylight Saving Time information is not available
    timeinfo.tm_isdst  = -1;

    // Convert to timestamp
    time_t t = st_rtc_mktime(&timeinfo);

    return t;
}

void rtc_write(time_t t)
{
    RTC_DateTypeDef dateStruct;
    RTC_TimeTypeDef timeStruct;

    RtcHandle.Instance = RTC;

    // Convert the time into a tm
    struct tm timeinfo;
    if (st_rtc_localtime(t, &timeinfo) == false) { 
        return;
    }

    // Fill RTC structures
    if (timeinfo.tm_wday == 0) {
        dateStruct.WeekDay    = 7;
    } else {
        dateStruct.WeekDay    = timeinfo.tm_wday;
    }
    dateStruct.Month          = timeinfo.tm_mon + 1;
    dateStruct.Date           = timeinfo.tm_mday;
    dateStruct.Year           = timeinfo.tm_year - 68;
    timeStruct.Hours          = timeinfo.tm_hour;
    timeStruct.Minutes        = timeinfo.tm_min;
    timeStruct.Seconds        = timeinfo.tm_sec;

#if !(TARGET_STM32F1)
    timeStruct.TimeFormat     = RTC_HOURFORMAT_24;
    timeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    timeStruct.StoreOperation = RTC_STOREOPERATION_RESET;
#endif /* TARGET_STM32F1 */

    // Change the RTC current date/time
    HAL_RTC_SetDate(&RtcHandle, &dateStruct, RTC_FORMAT_BIN);
    HAL_RTC_SetTime(&RtcHandle, &timeStruct, RTC_FORMAT_BIN);
}

int rtc_isenabled(void)
{
#if !(TARGET_STM32F1)
    return ( ((RTC->ISR & RTC_ISR_INITS) ==  RTC_ISR_INITS) && ((RTC->ISR & RTC_ISR_RSF) ==  RTC_ISR_RSF) );
#else /* TARGET_STM32F1 */
    return ((RTC->CRL & RTC_CRL_RSF) ==  RTC_CRL_RSF);
#endif /* TARGET_STM32F1 */
}

#if DEVICE_LOWPOWERTIMER

static void RTC_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RtcHandle);
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    if (irq_handler) {
        // Fire the user callback
        irq_handler();
    }
}

void rtc_set_irq_handler(uint32_t handler)
{
    irq_handler = (void (*)(void))handler;
}

uint32_t rtc_read_subseconds(void)
{
    return 1000000.f * ((double)(RTC_SYNCH_PREDIV - RTC->SSR) / (RTC_SYNCH_PREDIV + 1));
}

void rtc_set_wake_up_timer(uint32_t delta)
{
    uint32_t wake_up_counter = delta / (2000000 / RTC_CLOCK);

    if (HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, wake_up_counter,
                                    RTC_WAKEUPCLOCK_RTCCLK_DIV2) != HAL_OK) {
        error("Set wake up timer failed\n");
    }
}

void rtc_deactivate_wake_up_timer(void)
{
    HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);
}

void rtc_synchronize(void)
{
    HAL_RTC_WaitForSynchro(&RtcHandle);
}
#endif /* DEVICE_LOWPOWERTIMER */

#endif /* DEVICE_RTC */
