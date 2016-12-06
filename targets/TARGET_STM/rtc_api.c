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

    __PWR_CLK_ENABLE();

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
    __PWR_CLK_ENABLE();

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
 RTC Registers
   RTC_WeekDay 1=monday, 2=tuesday, ..., 7=sunday
   RTC_Month   1=january, 2=february, ..., 12=december
   RTC_Date    day of the month 1-31
   RTC_Year    year 0-99
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
time_t rtc_read(void)
{
    RTC_DateTypeDef dateStruct;
    RTC_TimeTypeDef timeStruct;
    struct tm timeinfo;

    RtcHandle.Instance = RTC;

    // Read actual date and time
    // Warning: the time must be read first!
    HAL_RTC_GetTime(&RtcHandle, &timeStruct, FORMAT_BIN);
    HAL_RTC_GetDate(&RtcHandle, &dateStruct, FORMAT_BIN);

    // Setup a tm structure based on the RTC
    timeinfo.tm_wday = dateStruct.WeekDay;
    timeinfo.tm_mon  = dateStruct.Month - 1;
    timeinfo.tm_mday = dateStruct.Date;
    timeinfo.tm_year = dateStruct.Year + 68;
    timeinfo.tm_hour = timeStruct.Hours;
    timeinfo.tm_min  = timeStruct.Minutes;
    timeinfo.tm_sec  = timeStruct.Seconds;
    // Daylight Saving Time information is not available
    timeinfo.tm_isdst  = -1;

    // Convert to timestamp
    time_t t = mktime(&timeinfo);

    return t;
}

void rtc_write(time_t t)
{
    RTC_DateTypeDef dateStruct;
    RTC_TimeTypeDef timeStruct;

    RtcHandle.Instance = RTC;

    // Convert the time into a tm
    struct tm *timeinfo = localtime(&t);

    // Fill RTC structures
    dateStruct.WeekDay        = timeinfo->tm_wday;
    dateStruct.Month          = timeinfo->tm_mon + 1;
    dateStruct.Date           = timeinfo->tm_mday;
    dateStruct.Year           = timeinfo->tm_year - 68;
    timeStruct.Hours          = timeinfo->tm_hour;
    timeStruct.Minutes        = timeinfo->tm_min;
    timeStruct.Seconds        = timeinfo->tm_sec;

#if !(TARGET_STM32F1)
    timeStruct.TimeFormat     = RTC_HOURFORMAT_24;
    timeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    timeStruct.StoreOperation = RTC_STOREOPERATION_RESET;
#endif /* TARGET_STM32F1 */

    // Change the RTC current date/time
    HAL_RTC_SetDate(&RtcHandle, &dateStruct, FORMAT_BIN);
    HAL_RTC_SetTime(&RtcHandle, &timeStruct, FORMAT_BIN);
}

int rtc_isenabled(void)
{
#if DEVICE_LOWPOWERTIMER
    if ((RTC->ISR & RTC_ISR_INITS) ==  RTC_ISR_INITS) {
        return 1;
    } else {
        return 0;
    }
#else /* DEVICE_LOWPOWERTIMER */
    return 1;
#endif /* DEVICE_LOWPOWERTIMER */
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
