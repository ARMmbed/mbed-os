/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2015, STMicroelectronics
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
#include "rtc_api.h"
#include "rtc_api_hal.h"
#include "stm32f0xx.h"
#include "stm32f0xx_hal_rtc_ex.h"

#if DEVICE_RTC

#include "mbed_error.h"

#if DEVICE_RTC_LSI
static int rtc_inited = 0;
#endif

static RTC_HandleTypeDef RtcHandle;

#if DEVICE_LOWPOWERTIMER
static uint32_t m_synch_prediv = RTC_SYNCH_PREDIV;
static uint32_t m_asynch_prediv = RTC_ASYNCH_PREDIV;

static void (*irq_handler)(void);

static void rtc_configure_time_and_date()
{
    RTC_TimeTypeDef mTime;
    RTC_DateTypeDef mDate;

    mDate.WeekDay = 1;
    mDate.Month = 1;
    mDate.Date = 1;
    mDate.Year = 2;
    if (HAL_RTC_SetDate(&RtcHandle, &mDate, RTC_FORMAT_BIN) != HAL_OK) {
        error("Date set failed\n");
    }

    mTime.Hours = 0;
    mTime.Minutes = 0;
    mTime.Seconds = 0;
    mTime.TimeFormat = RTC_HOURFORMAT_24;
    mTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    mTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&RtcHandle, &mTime, RTC_FORMAT_BIN) != HAL_OK) {
        error("Time set failed\n");
    }
}

void RTC_IRQHandler()
{
    HAL_RTC_AlarmIRQHandler(&RtcHandle);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    if (irq_handler)
    {
        // Fire the user callback
        irq_handler();
    }
}

void rtc_set_irq_handler(uint32_t handler)
{
    irq_handler = (void (*)(void)) handler;
}

#endif

void rtc_init(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct;
    uint32_t rtc_freq = 0;

#if DEVICE_RTC_LSI
    if (rtc_inited) return;
    rtc_inited = 1;
#endif

    RtcHandle.Instance = RTC;

#if !DEVICE_RTC_LSI
    // Enable LSE Oscillator
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // Mandatory, otherwise the PLL is reconfigured!
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON; // External 32.768 kHz clock on OSC_IN/OSC_OUT
    RCC_OscInitStruct.LSIState       = RCC_LSI_OFF;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK) { // Check if LSE has started correctly
        // Connect LSE to RTC
        __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
        rtc_freq = LSE_VALUE;
    } else {
	    error("Cannot initialize RTC with LSE\n");
    }
#else
    // Enable Power clock
    __PWR_CLK_ENABLE();

    // Enable access to Backup domain
    HAL_PWR_EnableBkUpAccess();

    // Reset Backup domain
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();
	
	// Enable LSI clock
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // Mandatory, otherwise the PLL is reconfigured!
	RCC_OscInitStruct.LSEState       = RCC_LSE_OFF;
	RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		error("Cannot initialize RTC with LSI\n");
	}
	// Connect LSI to RTC
	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSI);
	// This value is LSI typical value. To be measured precisely using a timer input capture for example.
	rtc_freq = LSI_VALUE;
#endif

    // Enable RTC
    __HAL_RCC_RTC_ENABLE();

    RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
#if !DEVICE_LOWPOWERTIMER
    RtcHandle.Init.AsynchPrediv   = 127;
    RtcHandle.Init.SynchPrediv    = (rtc_freq / 128) - 1;
#else
    RtcHandle.Init.AsynchPrediv   = m_asynch_prediv;
    RtcHandle.Init.SynchPrediv    = m_synch_prediv;
#endif

    RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

    if (HAL_RTC_Init(&RtcHandle) != HAL_OK) {
        error("RTC error: RTC initialization failed.");
    }

#if DEVICE_LOWPOWERTIMER
    rtc_configure_time_and_date();
    NVIC_SetVector(RTC_IRQn, (uint32_t)&RTC_IRQHandler);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
#endif
}

void rtc_free(void) {
#if DEVICE_RTC_LSI
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

#if DEVICE_RTC_LSI
    rtc_inited = 0;
#endif
}

int rtc_isenabled(void) {
#if DEVICE_RTC_LSI
  return rtc_inited;
#else
  if ((RTC->ISR & RTC_ISR_INITS) ==  RTC_ISR_INITS) {
    return 1;
  } else {
    return 0;
  }
#endif
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
time_t rtc_read(void) {
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
    timeinfo.tm_isdst  = -1;

    // Convert to timestamp
    time_t t = mktime(&timeinfo);

    return t;
}

void rtc_write(time_t t) {
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
    timeStruct.TimeFormat     = RTC_HOURFORMAT_24;
    timeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    timeStruct.StoreOperation = RTC_STOREOPERATION_RESET;

    // Change the RTC current date/time
    HAL_RTC_SetDate(&RtcHandle, &dateStruct, FORMAT_BIN);
    HAL_RTC_SetTime(&RtcHandle, &timeStruct, FORMAT_BIN);
}

#if DEVICE_LOWPOWERTIMER
void rtc_set_alarm(struct tm *ti, uint32_t subsecs)
{
    RTC_AlarmTypeDef mAlarm;

    mAlarm.AlarmTime.Hours = ti->tm_hour;
    mAlarm.AlarmTime.Minutes = ti->tm_min;
    mAlarm.AlarmTime.Seconds = ti->tm_sec;
    mAlarm.AlarmTime.SubSeconds = subsecs;
    mAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
    mAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
    mAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
    mAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    mAlarm.AlarmDateWeekDay = 1;
    mAlarm.Alarm = RTC_ALARM_A;

    if (HAL_RTC_SetAlarm_IT(&RtcHandle, &mAlarm, RTC_FORMAT_BIN) != HAL_OK) {
        error("Set Alarm failed\n");
    }
}

void rtc_reconfigure_prescalers()
{
    m_synch_prediv = 0x3FF;
    m_asynch_prediv = 0x1F;
    rtc_init();
}

uint32_t rtc_ticker_get_synch_presc()
{
    return m_synch_prediv;
}

uint32_t rtc_read_subseconds()
{
    return RTC->SSR;
}

void rtc_ticker_disable_irq()
{
    HAL_RTC_DeactivateAlarm(&RtcHandle, RTC_ALARM_A);
}
#endif // DEVICE_LOWPOWERTIMER

#endif
