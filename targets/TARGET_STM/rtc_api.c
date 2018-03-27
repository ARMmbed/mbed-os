/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
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

static RTC_HandleTypeDef RtcHandle;

#if DEVICE_LOWPOWERTIMER && !MBED_CONF_TARGET_LOWPOWERTIMER_LPTIM

#define GET_TICK_PERIOD(VALUE) (2048 * 1000000 / VALUE) /* 1s / SynchPrediv value * 2^11 (value to get the maximum precision value with no u32 overflow) */

static void (*irq_handler)(void);
static void RTC_IRQHandler(void);
static uint32_t lp_TickPeriod_us = GET_TICK_PERIOD(4095); /* default SynchPrediv value = 4095 */
#endif /* DEVICE_LOWPOWERTIMER && !MBED_CONF_TARGET_LOWPOWERTIMER_LPTIM */

void rtc_init(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    // Enable access to Backup domain
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    if (rtc_isenabled()) {
        return;
    }

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

    /* PREDIV_A : 7-bit asynchronous prescaler */
#if DEVICE_LOWPOWERTIMER && !MBED_CONF_TARGET_LOWPOWERTIMER_LPTIM
    /* PREDIV_A is set to a small value to improve the SubSeconds resolution */
    /* with a 32768Hz clock, PREDIV_A=7 gives a precision of 244us */
    RtcHandle.Init.AsynchPrediv = 7;
#else
    /* PREDIV_A is set to the maximum value to improve the consumption */
    RtcHandle.Init.AsynchPrediv   = 0x007F;
#endif
    /* PREDIV_S : 15-bit synchronous prescaler */
    /* PREDIV_S is set in order to get a 1 Hz clock */
    RtcHandle.Init.SynchPrediv    = RTC_CLOCK / (RtcHandle.Init.AsynchPrediv + 1) - 1;
    RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
#endif /* TARGET_STM32F1 */

#if DEVICE_LOWPOWERTIMER && !MBED_CONF_TARGET_LOWPOWERTIMER_LPTIM
    lp_TickPeriod_us = GET_TICK_PERIOD(RtcHandle.Init.SynchPrediv);
#endif

    if (HAL_RTC_Init(&RtcHandle) != HAL_OK) {
        error("RTC initialization failed");
    }

    rtc_synchronize(); // Wait for RSF

    if (!rtc_isenabled()) {
        rtc_write(0);
    }
}

void rtc_free(void)
{
    // Disable access to Backup domain
    HAL_PWR_DisableBkUpAccess();
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
32bit register is used (no BCD format) for the seconds.
For date, there is no specific register, only a software structure.
It is then not a problem to not use shifts.
*/

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

#if TARGET_STM32F1
    /* date information is null before first write procedure */
    /* set 01/01/1970 as default values */
    if (dateStruct.Year == 0) {
        dateStruct.Year = 2 ;
        dateStruct.Month = 1 ;
        dateStruct.Date = 1 ;
    }
#endif

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

void rtc_write(time_t t)
{
    RTC_DateTypeDef dateStruct = {0};
    RTC_TimeTypeDef timeStruct = {0};

    RtcHandle.Instance = RTC;

    // Convert the time into a tm
    struct tm timeinfo;
    if (_rtc_localtime(t, &timeinfo, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
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
    if (HAL_RTC_SetDate(&RtcHandle, &dateStruct, RTC_FORMAT_BIN) != HAL_OK) {
        error("HAL_RTC_SetDate error\n");
    }
    if (HAL_RTC_SetTime(&RtcHandle, &timeStruct, RTC_FORMAT_BIN) != HAL_OK) {
        error("HAL_RTC_SetTime error\n");
    }
}

int rtc_isenabled(void)
{
#if !(TARGET_STM32F1)
    return ( ((RTC->ISR & RTC_ISR_INITS) ==  RTC_ISR_INITS) && ((RTC->ISR & RTC_ISR_RSF) ==  RTC_ISR_RSF) );
#else /* TARGET_STM32F1 */
    return ((RTC->CRL & RTC_CRL_RSF) ==  RTC_CRL_RSF);
#endif /* TARGET_STM32F1 */
}

void rtc_synchronize(void)
{
    RtcHandle.Instance = RTC;
    if (HAL_RTC_WaitForSynchro(&RtcHandle) != HAL_OK) {
        error("rtc_synchronize error\n");
    }
}

#if DEVICE_LOWPOWERTIMER && !MBED_CONF_TARGET_LOWPOWERTIMER_LPTIM

static void RTC_IRQHandler(void)
{
    /*  Update HAL state */
    RtcHandle.Instance = RTC;
    HAL_RTCEx_WakeUpTimerIRQHandler(&RtcHandle);
    /* In case of registered handler, call it. */
    if (irq_handler) {
        irq_handler();
    }
}

uint32_t rtc_read_us(void)
{
    RTC_TimeTypeDef timeStruct = {0};
    RTC_DateTypeDef dateStruct = {0};

    RtcHandle.Instance = RTC;
    HAL_RTC_GetTime(&RtcHandle, &timeStruct, RTC_FORMAT_BIN);

    /* Reading RTC current time locks the values in calendar shadow registers until Current date is read
    to ensure consistency between the time and date values */
    HAL_RTC_GetDate(&RtcHandle, &dateStruct, RTC_FORMAT_BIN);

    if (timeStruct.SubSeconds > timeStruct.SecondFraction) {
        /* SS can be larger than PREDIV_S only after a shift operation. In that case, the correct
           time/date is one second less than as indicated by RTC_TR/RTC_DR. */
        timeStruct.Seconds -= 1;
    }
    uint32_t RTCTime = timeStruct.Seconds + timeStruct.Minutes * 60 + timeStruct.Hours * 60 * 60;
    uint32_t Time_us = ((timeStruct.SecondFraction - timeStruct.SubSeconds) * lp_TickPeriod_us) >> 11;

    return (RTCTime * 1000000) + Time_us ;
}

void rtc_set_wake_up_timer(uint32_t delta)
{
#define RTC_CLOCK_US (((uint64_t)RTC_CLOCK << 32 ) / 1000000)

    uint32_t WakeUpCounter;
    uint32_t WakeUpClock;

    /* Ex for Wakeup period resolution with RTCCLK=32768 Hz :
    *    RTCCLK_DIV2: ~122us < wakeup period < ~4s
    *    RTCCLK_DIV4: ~244us < wakeup period < ~8s
    *    RTCCLK_DIV8: ~488us < wakeup period < ~16s
    *    RTCCLK_DIV16: ~976us < wakeup period < ~32s
    *    CK_SPRE_16BITS: 1s < wakeup period < (0xFFFF+ 1) x 1 s = 65536 s (18 hours)
    *    CK_SPRE_17BITS: 18h+1s < wakeup period < (0x1FFFF+ 1) x 1 s = 131072 s (36 hours)
    */
    if (delta < (0x10000 * 2 / RTC_CLOCK * 1000000) ) { // (0xFFFF + 1) * RTCCLK_DIV2 / RTC_CLOCK * 1s
        WakeUpCounter = (((uint64_t)delta * RTC_CLOCK_US) >> 32) >> 1 ;
        WakeUpClock = RTC_WAKEUPCLOCK_RTCCLK_DIV2;
    } else if (delta < (0x10000 * 4 / RTC_CLOCK * 1000000) ) {
        WakeUpCounter = (((uint64_t)delta * RTC_CLOCK_US) >> 32) >> 2 ;
        WakeUpClock = RTC_WAKEUPCLOCK_RTCCLK_DIV4;
    } else if (delta < (0x10000 * 8 / RTC_CLOCK * 1000000) ) {
        WakeUpCounter = (((uint64_t)delta * RTC_CLOCK_US) >> 32) >> 3 ;
        WakeUpClock = RTC_WAKEUPCLOCK_RTCCLK_DIV8;
    } else if (delta < (0x10000 * 16 / RTC_CLOCK * 1000000) ) {
        WakeUpCounter = (((uint64_t)delta * RTC_CLOCK_US) >> 32) >> 4 ;
        WakeUpClock = RTC_WAKEUPCLOCK_RTCCLK_DIV16;
    } else {
        WakeUpCounter = (delta / 1000000) ;
        WakeUpClock = RTC_WAKEUPCLOCK_CK_SPRE_16BITS;
    }

    irq_handler = (void (*)(void))lp_ticker_irq_handler;
    NVIC_SetVector(RTC_WKUP_IRQn, (uint32_t)RTC_IRQHandler);
    NVIC_EnableIRQ(RTC_WKUP_IRQn);

    RtcHandle.Instance = RTC;
    if (HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, (uint32_t)WakeUpCounter, WakeUpClock) != HAL_OK) {
        error("rtc_set_wake_up_timer init error\n");
    }
}

void rtc_deactivate_wake_up_timer(void)
{
    RtcHandle.Instance = RTC;
    HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);
}

#endif /* DEVICE_LOWPOWERTIMER && !MBED_CONF_TARGET_LOWPOWERTIMER_LPTIM */

#endif /* DEVICE_RTC */
