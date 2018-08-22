/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2018, STMicroelectronics
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

#include "rtc_api_hal.h"
#include "mbed_mktime.h"
#include "mbed_error.h"
#include "mbed_critical.h"

#if DEVICE_LPTICKER && !MBED_CONF_TARGET_LPTICKER_LPTIM
volatile uint32_t LP_continuous_time = 0;
volatile uint32_t LP_last_RTC_time = 0;
#endif

static int RTC_inited = 0;

static RTC_HandleTypeDef RtcHandle;

void rtc_init(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    if (RTC_inited) {
        return;
    }
    RTC_inited = 1;

    // Enable access to Backup domain
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

#if MBED_CONF_TARGET_LSE_AVAILABLE
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // Mandatory, otherwise the PLL is reconfigured!
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error("Cannot initialize RTC with LSE\n");
    }

    __HAL_RCC_RTC_CLKPRESCALER(RCC_RTCCLKSOURCE_LSE);
    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        error("PeriphClkInitStruct RTC failed with LSE\n");
    }
#else /*  MBED_CONF_TARGET_LSE_AVAILABLE */
    // Reset Backup domain
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();

    // Enable LSI clock
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // Mandatory, otherwise the PLL is reconfigured!
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
#endif /* MBED_CONF_TARGET_LSE_AVAILABLE */

    // Enable RTC
    __HAL_RCC_RTC_ENABLE();

    RtcHandle.Instance = RTC;
    RtcHandle.State = HAL_RTC_STATE_RESET;

#if TARGET_STM32F1
    RtcHandle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
#else /* TARGET_STM32F1 */
    RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
    RtcHandle.Init.AsynchPrediv   = PREDIV_A_VALUE;
    RtcHandle.Init.SynchPrediv    = PREDIV_S_VALUE;
    RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
#endif /* TARGET_STM32F1 */

    if (HAL_RTC_Init(&RtcHandle) != HAL_OK) {
        error("RTC initialization failed");
    }

#if !(TARGET_STM32F1) && !(TARGET_STM32F2)
    /* STM32F1 : there are no shadow registers */
    /* STM32F2 : shadow registers can not be bypassed */
    if (HAL_RTCEx_EnableBypassShadow(&RtcHandle) != HAL_OK) {
        error("EnableBypassShadow error");
    }
#endif /* TARGET_STM32F1 || TARGET_STM32F2 */
}

void rtc_free(void)
{
    /* RTC clock can not be reset */
}


/*
Information about STM32F0, STM32F2, STM32F3, STM32F4, STM32F7, STM32L0, STM32L1, STM32L4:
BCD format is used to store the date in the RTC. The year is store on 2 * 4 bits.
Because the first year is reserved to see if the RTC is init, the supposed range is 01-99.
1st point is to cover the standard range from 1970 to 2038 (limited by the 32 bits of time_t).
2nd point is to keep the year 1970 and the leap years synchronized.

So by moving it 68 years forward from 1970, it become 1969-2067 which include 1970-2038.
68 is also a multiple of 4 so it let the leap year synchronized.

Information about STM32F1:
32bit register is used (no BCD format) for the seconds.
For date, there is no specific register, only a software structure.
It is then not a problem to not use shifts.
*/
#if TARGET_STM32F1
time_t rtc_read(void)
{
    RTC_DateTypeDef dateStruct = {0};
    RTC_TimeTypeDef timeStruct = {0};
    struct tm timeinfo;

    RtcHandle.Instance = RTC;

    // Read actual date and time
    // Warning: the time must be read first!
    HAL_RTC_GetTime(&RtcHandle, &timeStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&RtcHandle, &dateStruct, RTC_FORMAT_BIN);

    /* date information is null before first write procedure */
    /* set 01/01/1970 as default values */
    if (dateStruct.Year == 0) {
        dateStruct.Year = 2 ;
        dateStruct.Month = 1 ;
        dateStruct.Date = 1 ;
    }

    // Setup a tm structure based on the RTC
    /* tm_wday information is ignored by _rtc_maketime */
    /* tm_isdst information is ignored by _rtc_maketime */
    timeinfo.tm_mon  = dateStruct.Month - 1;
    timeinfo.tm_mday = dateStruct.Date;
    timeinfo.tm_year = dateStruct.Year + 68;
    timeinfo.tm_hour = timeStruct.Hours;
    timeinfo.tm_min  = timeStruct.Minutes;
    timeinfo.tm_sec  = timeStruct.Seconds;

    // Convert to timestamp
    time_t t;
    if (_rtc_maketime(&timeinfo, &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return 0;
    }

    return t;
}

#else /* TARGET_STM32F1 */

time_t rtc_read(void)
{
    struct tm timeinfo;

    /* Since the shadow registers are bypassed we have to read the time twice and compare them until both times are the same */
    uint32_t Read_time = RTC->TR & RTC_TR_RESERVED_MASK;
    uint32_t Read_date = RTC->DR & RTC_DR_RESERVED_MASK;

    while ((Read_time != (RTC->TR & RTC_TR_RESERVED_MASK)) || (Read_date != (RTC->DR & RTC_DR_RESERVED_MASK))) {
        Read_time = RTC->TR & RTC_TR_RESERVED_MASK;
        Read_date = RTC->DR & RTC_DR_RESERVED_MASK;
    }

    /* Setup a tm structure based on the RTC
    struct tm :
        tm_sec      seconds after the minute 0-61
        tm_min      minutes after the hour 0-59
        tm_hour     hours since midnight 0-23
        tm_mday     day of the month 1-31
        tm_mon      months since January 0-11
        tm_year     years since 1900
        tm_yday     information is ignored by _rtc_maketime
        tm_wday     information is ignored by _rtc_maketime
        tm_isdst    information is ignored by _rtc_maketime
    */
    timeinfo.tm_mday = RTC_Bcd2ToByte((uint8_t)(Read_date & (RTC_DR_DT | RTC_DR_DU)));
    timeinfo.tm_mon  = RTC_Bcd2ToByte((uint8_t)((Read_date & (RTC_DR_MT | RTC_DR_MU))  >> 8)) - 1;
    timeinfo.tm_year = RTC_Bcd2ToByte((uint8_t)((Read_date & (RTC_DR_YT | RTC_DR_YU))  >> 16)) + 68;
    timeinfo.tm_hour = RTC_Bcd2ToByte((uint8_t)((Read_time & (RTC_TR_HT  | RTC_TR_HU))  >> 16));
    timeinfo.tm_min  = RTC_Bcd2ToByte((uint8_t)((Read_time & (RTC_TR_MNT | RTC_TR_MNU)) >> 8));
    timeinfo.tm_sec  = RTC_Bcd2ToByte((uint8_t)((Read_time & (RTC_TR_ST  | RTC_TR_SU))  >> 0));

    // Convert to timestamp
    time_t t;
    if (_rtc_maketime(&timeinfo, &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return 0;
    }

    return t;
}

#endif /* TARGET_STM32F1 */

void rtc_write(time_t t)
{
    RTC_DateTypeDef dateStruct = {0};
    RTC_TimeTypeDef timeStruct = {0};

    core_util_critical_section_enter();
    RtcHandle.Instance = RTC;

    // Convert the time into a tm
    struct tm timeinfo;
    if (_rtc_localtime(t, &timeinfo, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return;
    }

    // Fill RTC structures
    if (timeinfo.tm_wday == 0) { /* Sunday specific case */
        dateStruct.WeekDay    = RTC_WEEKDAY_SUNDAY;
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

#if DEVICE_LPTICKER && !MBED_CONF_TARGET_LPTICKER_LPTIM
    /* Need to update LP_continuous_time value before new RTC time */
    rtc_read_lp();

    /* LP_last_RTC_time value is updated with the new RTC time */
    LP_last_RTC_time = timeStruct.Seconds + timeStruct.Minutes * 60 + timeStruct.Hours * 60 * 60;

    /* Save current SSR */
    uint32_t Read_SubSeconds = (uint32_t)(RTC->SSR);
#endif /* DEVICE_LPTICKER && !MBED_CONF_TARGET_LPTICKER_LPTIM */

    // Change the RTC current date/time
    if (HAL_RTC_SetDate(&RtcHandle, &dateStruct, RTC_FORMAT_BIN) != HAL_OK) {
        error("HAL_RTC_SetDate error\n");
    }
    if (HAL_RTC_SetTime(&RtcHandle, &timeStruct, RTC_FORMAT_BIN) != HAL_OK) {
        error("HAL_RTC_SetTime error\n");
    }

#if DEVICE_LPTICKER && !MBED_CONF_TARGET_LPTICKER_LPTIM
    while (Read_SubSeconds != (RTC->SSR)) {
    }
#endif /* DEVICE_LPTICKER && !MBED_CONF_TARGET_LPTICKER_LPTIM */

    core_util_critical_section_exit();
}

int rtc_isenabled(void)
{
#if !(TARGET_STM32F1)
    return ((RTC->ISR & RTC_ISR_INITS) ==  RTC_ISR_INITS);
#else /* TARGET_STM32F1 */
    return ((RTC->CRL & RTC_CRL_RSF) ==  RTC_CRL_RSF);
#endif /* TARGET_STM32F1 */
}


#if DEVICE_LPTICKER && !MBED_CONF_TARGET_LPTICKER_LPTIM

static void RTC_IRQHandler(void);
static void (*irq_handler)(void);

volatile uint8_t lp_Fired = 0;

static void RTC_IRQHandler(void)
{
    /*  Update HAL state */
    RtcHandle.Instance = RTC;
    if (__HAL_RTC_WAKEUPTIMER_GET_IT(&RtcHandle, RTC_IT_WUT)) {
        /* Get the status of the Interrupt */
        if ((uint32_t)(RTC->CR & RTC_IT_WUT) != (uint32_t)RESET) {
            /* Clear the WAKEUPTIMER interrupt pending bit */
            __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RtcHandle, RTC_FLAG_WUTF);

            lp_Fired = 0;
            if (irq_handler) {
                irq_handler();
            }
        }
    }

    if (lp_Fired) {
        lp_Fired = 0;
        if (irq_handler) {
            irq_handler();
        }
    }

    __HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
}

uint32_t rtc_read_lp(void)
{
    struct tm timeinfo;

    /* Since the shadow registers are bypassed we have to read the time twice and compare them until both times are the same */
    /* We don't have to read date as we bypass shadow registers */
    uint32_t Read_SecondFraction = (uint32_t)(RTC->PRER & RTC_PRER_PREDIV_S);
    uint32_t Read_time = (uint32_t)(RTC->TR & RTC_TR_RESERVED_MASK);
    uint32_t Read_SubSeconds = (uint32_t)(RTC->SSR);

    while ((Read_time != (RTC->TR & RTC_TR_RESERVED_MASK)) || (Read_SubSeconds != (RTC->SSR))) {
        Read_time = (uint32_t)(RTC->TR & RTC_TR_RESERVED_MASK);
        Read_SubSeconds = (uint32_t)(RTC->SSR);
    }

    timeinfo.tm_hour = RTC_Bcd2ToByte((uint8_t)((Read_time & (RTC_TR_HT  | RTC_TR_HU))  >> 16));
    timeinfo.tm_min  = RTC_Bcd2ToByte((uint8_t)((Read_time & (RTC_TR_MNT | RTC_TR_MNU)) >> 8));
    timeinfo.tm_sec  = RTC_Bcd2ToByte((uint8_t)((Read_time & (RTC_TR_ST  | RTC_TR_SU))  >> 0));

    uint32_t RTC_time_s = timeinfo.tm_sec + timeinfo.tm_min * 60 + timeinfo.tm_hour * 60 * 60; // Max 0x0001-517F => * 8191 + 8191 = 0x2A2E-AE80

    if (LP_last_RTC_time <= RTC_time_s) {
        LP_continuous_time += (RTC_time_s - LP_last_RTC_time);
    } else {
        /* Add 24h */
        LP_continuous_time += (24 * 60 * 60 + RTC_time_s - LP_last_RTC_time);
    }
    LP_last_RTC_time = RTC_time_s;

    return LP_continuous_time * PREDIV_S_VALUE + Read_SecondFraction - Read_SubSeconds;
}

void rtc_set_wake_up_timer(timestamp_t timestamp)
{
    uint32_t WakeUpCounter;
    uint32_t current_lp_time;

    current_lp_time = rtc_read_lp();

    if (timestamp < current_lp_time) {
        WakeUpCounter = 0xFFFFFFFF - current_lp_time + timestamp;
    } else {
        WakeUpCounter = timestamp - current_lp_time;
    }

    if (WakeUpCounter > 0xFFFF) {
        WakeUpCounter = 0xFFFF;
    }

    RtcHandle.Instance = RTC;
    if (HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, WakeUpCounter, RTC_WAKEUPCLOCK_RTCCLK_DIV4) != HAL_OK) {
        error("rtc_set_wake_up_timer init error\n");
    }

    NVIC_SetVector(RTC_WKUP_IRQn, (uint32_t)RTC_IRQHandler);
    irq_handler = (void (*)(void))lp_ticker_irq_handler;
    NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

void rtc_fire_interrupt(void)
{
    lp_Fired = 1;
    NVIC_SetVector(RTC_WKUP_IRQn, (uint32_t)RTC_IRQHandler);
    irq_handler = (void (*)(void))lp_ticker_irq_handler;
    NVIC_SetPendingIRQ(RTC_WKUP_IRQn);
    NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

void rtc_deactivate_wake_up_timer(void)
{
    RtcHandle.Instance = RTC;
    __HAL_RTC_WRITEPROTECTION_DISABLE(&RtcHandle);
    __HAL_RTC_WAKEUPTIMER_DISABLE(&RtcHandle);
    __HAL_RTC_WAKEUPTIMER_DISABLE_IT(&RtcHandle, RTC_IT_WUT);
    __HAL_RTC_WRITEPROTECTION_ENABLE(&RtcHandle);
    NVIC_DisableIRQ(RTC_WKUP_IRQn);
}

#endif /* DEVICE_LPTICKER && !MBED_CONF_TARGET_LPTICKER_LPTIM */

#endif /* DEVICE_RTC */
