/**
 * @file xmc_rtc.c
 * @date 2015-05-19
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial <br>
 *      
 * 2015-06-20:
 *     - Removed GetDriverVersion API
 * 
 * 2016-05-19:
 *     - Added XMC_RTC_SetTimeStdFormat() and XMC_RTC_SetAlarmStdFormat()
 *
 * @endcond 
 *
 */

/**
 *
 * @brief RTC driver for XMC microcontroller family.
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
 
#include "xmc_scu.h"
#include "xmc_rtc.h"

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

#define XMC_RTC_MAXSECONDS  (59U)   /**< RTC time : Maximum seconds */
#define XMC_RTC_MAXMINUTES  (59U)   /**< RTC time : Maximum minutes */
#define XMC_RTC_MAXHOURS    (23U)   /**< RTC time : Maximum hours */
#define XMC_RTC_MAXDAYS     (31U)   /**< RTC time : Maximum days */
#define XMC_RTC_MAXDAYSOFWEEK  (7U) /**< RTC time : Maximum days of week */
#define XMC_RTC_MAXMONTH  (12U)     /**< RTC time : Maximum month */
#define XMC_RTC_MAXYEAR  (0xFFFFU)  /**< RTC time : Maximum year */
#define XMC_RTC_MAXPRESCALER  (0xFFFFU)  /**< RTC time : Maximum prescaler */
#define XMC_RTC_YEAR_OFFSET (1900U)      /**< RTC year offset : Year offset */

#if (UC_FAMILY == XMC4)
#define XMC_RTC_INIT_SEQUENCE  (1U)
#endif
#if (UC_FAMILY == XMC1)
#define XMC_RTC_INIT_SEQUENCE  (0U)
#endif

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/

/*
 * Enables RTC peripheral to start counting time
 */
void XMC_RTC_Start(void)
{
  while((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_CTR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->CTR |= (uint32_t)RTC_CTR_ENB_Msk;
}

/*
 * Disables RTC peripheral to start counting time
 */
void XMC_RTC_Stop(void)
{
  while((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_CTR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->CTR &= ~(uint32_t)RTC_CTR_ENB_Msk;
}

/*
 * Sets the RTC module prescaler value
 */
void XMC_RTC_SetPrescaler(uint16_t prescaler)
{
  XMC_ASSERT("XMC_RTC_SetPrescaler:Wrong prescaler value", (prescaler < XMC_RTC_MAXPRESCALER));
  
  while((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_CTR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->CTR = (RTC->CTR & ~(uint32_t)RTC_CTR_DIV_Msk) |
             ((uint32_t)prescaler << (uint32_t)RTC_CTR_DIV_Pos);
}

/*
 * Sets the RTC_TIM0, RTC_TIM1 registers with time values
 */
void XMC_RTC_SetTime(const XMC_RTC_TIME_t *const time)
{
  XMC_ASSERT("XMC_RTC_SetTime:Wrong seconds value", ((uint32_t)time->seconds < XMC_RTC_MAXSECONDS));
  XMC_ASSERT("XMC_RTC_SetTime:Wrong minutes value", ((uint32_t)time->minutes < XMC_RTC_MAXMINUTES));
  XMC_ASSERT("XMC_RTC_SetTime:Wrong hours value", ((uint32_t)time->hours < XMC_RTC_MAXHOURS));
  XMC_ASSERT("XMC_RTC_SetTime:Wrong month day value", ((uint32_t)time->days < XMC_RTC_MAXDAYS));
  XMC_ASSERT("XMC_RTC_SetTime:Wrong week day value", ((uint32_t)time->daysofweek < XMC_RTC_MAXDAYSOFWEEK));
  XMC_ASSERT("XMC_RTC_SetTime:Wrong month value", ((uint32_t)time->month < XMC_RTC_MAXMONTH));
  XMC_ASSERT("XMC_RTC_SetTime:Wrong year value", ((uint32_t)time->year < XMC_RTC_MAXYEAR));

  #if (XMC_RTC_INIT_SEQUENCE == 1U)
      while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_TIM0_Msk) != 0U)
      { 
         /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
      }
      RTC->TIM0 = time->raw0;
      
      while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_TIM1_Msk) != 0U)
      {
        /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
      }
      RTC->TIM1 = time->raw1;	     
  #endif
  #if (XMC_RTC_INIT_SEQUENCE == 0U)
      while ((XMC_SCU_GetMirrorStatus() & (SCU_GENERAL_MIRRSTS_RTC_TIM0_Msk | SCU_GENERAL_MIRRSTS_RTC_TIM1_Msk)) != 0U)
      {
        /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
      }
      RTC->TIM0 = time->raw0;
      RTC->TIM1 = time->raw1;	; 
  #endif  
}

/*
 * Gets the RTC module time value
 */
void XMC_RTC_GetTime(XMC_RTC_TIME_t *const time)
{
  time->raw0 = RTC->TIM0;
  time->raw1 = RTC->TIM1;
}

/*
 * Sets the RTC module time values in standard format
 */
void XMC_RTC_SetTimeStdFormat(const struct tm *const stdtime)
{

  XMC_RTC_TIME_t time;

  time.seconds = stdtime->tm_sec;
  time.minutes = stdtime->tm_min;
  time.hours = stdtime->tm_hour;
  time.days = stdtime->tm_mday - 1;
  time.month = stdtime->tm_mon;
  time.year = stdtime->tm_year + XMC_RTC_YEAR_OFFSET;
  time.daysofweek = stdtime->tm_wday;

  XMC_RTC_SetTime(&time);
}

/*
 * Gets the RTC module time values in standard format
 */
void XMC_RTC_GetTimeStdFormat(struct tm *const stdtime)
{
  XMC_RTC_TIME_t time;
  time.raw0 = RTC->TIM0;
  time.raw1 = RTC->TIM1;
  
  stdtime->tm_sec = (int8_t)time.seconds;
  stdtime->tm_min = (int8_t)time.minutes;
  stdtime->tm_hour = (int8_t)time.hours;
  stdtime->tm_mday = ((int8_t)time.days + (int8_t)1);
  stdtime->tm_mon = (int8_t)time.month;
  stdtime->tm_year = (int32_t)time.year - (int32_t)XMC_RTC_YEAR_OFFSET;
  stdtime->tm_wday = (int8_t)time.daysofweek;
}

/*
 * Sets the RTC module alarm time value
 */
void XMC_RTC_SetAlarm(const XMC_RTC_ALARM_t *const alarm)
{
  XMC_ASSERT("XMC_RTC_SetAlarm:Wrong seconds value", ((uint32_t)alarm->seconds < XMC_RTC_MAXSECONDS));
  XMC_ASSERT("XMC_RTC_SetAlarm:Wrong minutes value", ((uint32_t)alarm->minutes < XMC_RTC_MAXMINUTES));
  XMC_ASSERT("XMC_RTC_SetAlarm:Wrong hours value", ((uint32_t)alarm->hours < XMC_RTC_MAXHOURS));
  XMC_ASSERT("XMC_RTC_SetAlarm:Wrong days value", ((uint32_t)alarm->days < XMC_RTC_MAXDAYS));
  XMC_ASSERT("XMC_RTC_SetAlarm:Wrong month value", ((uint32_t)alarm->month < XMC_RTC_MAXMONTH));
  XMC_ASSERT("XMC_RTC_SetAlarm:Wrong year value", ((uint32_t)alarm->year < XMC_RTC_MAXYEAR));

  #if (XMC_RTC_INIT_SEQUENCE == 1U)
      while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_ATIM0_Msk) != 0U)
      {
        /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
      }
      RTC->ATIM0 = alarm->raw0;
      
      while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_ATIM1_Msk) != 0U)
      {
        /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
      }
      RTC->ATIM1 = alarm->raw1; 
  #endif
  #if (XMC_RTC_INIT_SEQUENCE == 0U)
      while ((XMC_SCU_GetMirrorStatus() & (SCU_GENERAL_MIRRSTS_RTC_ATIM0_Msk | SCU_GENERAL_MIRRSTS_RTC_ATIM1_Msk)) != 0U)
      {
        /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
      }
      RTC->ATIM0 = alarm->raw0;
      RTC->ATIM1 = alarm->raw1; 
  #endif  
}

/*
 * Gets the RTC module alarm time value
 */
void XMC_RTC_GetAlarm(XMC_RTC_ALARM_t *const alarm)
{
  alarm->raw0 = RTC->ATIM0;
  alarm->raw1 = RTC->ATIM1;
}


/*
 * Sets the RTC module alarm time value in standard format
 */
void XMC_RTC_SetAlarmStdFormat(const struct tm *const stdtime)
{
  XMC_RTC_ALARM_t alarm;
  

  alarm.seconds = stdtime->tm_sec;
  alarm.minutes = stdtime->tm_min;
  alarm.hours = stdtime->tm_hour;
  alarm.days = stdtime->tm_mday - 1;
  alarm.month = stdtime->tm_mon;
  alarm.year = stdtime->tm_year + XMC_RTC_YEAR_OFFSET;

  XMC_RTC_SetAlarm(&alarm);
}

/*
 * Gets the RTC module alarm time value in standard format
 */
void XMC_RTC_GetAlarmStdFormat(struct tm *const stdtime)
{
  XMC_RTC_ALARM_t alarm;
  
  alarm.raw0 = RTC->ATIM0;
  alarm.raw1 = RTC->ATIM1;

  stdtime->tm_sec = (int8_t)alarm.seconds;
  stdtime->tm_min = (int8_t)alarm.minutes;
  stdtime->tm_hour = (int8_t)alarm.hours;
  stdtime->tm_mday = ((int8_t)alarm.days + (int8_t)1);
  stdtime->tm_mon = (int8_t)alarm.month;
  stdtime->tm_year = (int32_t)alarm.year - (int32_t)XMC_RTC_YEAR_OFFSET;
}

/*
 * Gets the RTC periodic and alarm event(s) status
 */
uint32_t XMC_RTC_GetEventStatus(void)
{
  return RTC->STSSR;
}
