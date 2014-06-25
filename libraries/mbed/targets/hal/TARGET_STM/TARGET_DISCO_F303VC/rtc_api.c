/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
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

#if DEVICE_RTC

#include "wait_api.h"

#define LSE_STARTUP_TIMEOUT ((uint16_t)500) // delay in ms

static int rtc_inited = 0;

void rtc_init(void) {
    uint32_t StartUpCounter = 0;
    uint32_t LSEStatus = 0;
    uint32_t rtc_freq = 0;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // Enable PWR clock

    PWR_BackupAccessCmd(ENABLE); // Enable access to Backup domain

    // Reset back up registers
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
  
    // Enable LSE clock
    RCC_LSEConfig(RCC_LSE_ON);

    // Wait till LSE is ready
    do {
        LSEStatus = RCC_GetFlagStatus(RCC_FLAG_LSERDY);
        wait_ms(1);
        StartUpCounter++;
    } while ((LSEStatus == 0) && (StartUpCounter <= LSE_STARTUP_TIMEOUT));

    if (StartUpCounter > LSE_STARTUP_TIMEOUT) {
        // The LSE has not started, use LSI instead.
    // The RTC Clock may vary due to LSI frequency dispersion.
        RCC_LSEConfig(RCC_LSE_OFF);
    RCC_LSICmd(ENABLE); // Enable LSI
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) {} // Wait until ready
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI); // Select the RTC Clock Source
        rtc_freq = 40000; // [TODO] To be measured precisely using a timer input capture
    } else {
        // The LSE has correctly started
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // Select the RTC Clock Source
        rtc_freq = LSE_VALUE;
    }
  
    RCC_RTCCLKCmd(ENABLE); // Enable RTC Clock 
      
    RTC_WaitForSynchro(); // Wait for RTC registers synchronization

    RTC_InitTypeDef RTC_InitStructure;
    RTC_InitStructure.RTC_AsynchPrediv = 127;
    RTC_InitStructure.RTC_SynchPrediv    = (rtc_freq / 128) - 1;
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);
    
    PWR_BackupAccessCmd(DISABLE); // Disable access to Backup domain

    rtc_inited = 1;
}

void rtc_free(void) {
    // Reset RTC
    PWR_BackupAccessCmd(ENABLE); // Enable access to Backup Domain
    RTC_DeInit();
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
    // Disable RTC, LSE and LSI clocks
    RCC_RTCCLKCmd(DISABLE);
    RCC_LSEConfig(RCC_LSE_OFF);
    RCC_LSICmd(DISABLE);

    rtc_inited = 0;
}

int rtc_isenabled(void) {
    return rtc_inited;
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
        
    // Read actual date and time
    RTC_GetTime(RTC_Format_BIN, &timeStruct);
    RTC_GetDate(RTC_Format_BIN, &dateStruct);
    
    // Setup a tm structure based on the RTC
    timeinfo.tm_wday = dateStruct.RTC_WeekDay;
    timeinfo.tm_mon  = dateStruct.RTC_Month - 1;
    timeinfo.tm_mday = dateStruct.RTC_Date;
    timeinfo.tm_year = dateStruct.RTC_Year + 100;
    timeinfo.tm_hour = timeStruct.RTC_Hours;
    timeinfo.tm_min  = timeStruct.RTC_Minutes;
    timeinfo.tm_sec  = timeStruct.RTC_Seconds;
    
    // Convert to timestamp
    time_t t = mktime(&timeinfo);
    
    return t;    
}

void rtc_write(time_t t) {
    RTC_DateTypeDef dateStruct;
    RTC_TimeTypeDef timeStruct;

    // Convert the time into a tm
    struct tm *timeinfo = localtime(&t);
    
    // Fill RTC structures
    dateStruct.RTC_WeekDay = timeinfo->tm_wday;
    dateStruct.RTC_Month   = timeinfo->tm_mon + 1;
    dateStruct.RTC_Date    = timeinfo->tm_mday;
    dateStruct.RTC_Year    = timeinfo->tm_year - 100;
    timeStruct.RTC_Hours   = timeinfo->tm_hour;
    timeStruct.RTC_Minutes = timeinfo->tm_min;
    timeStruct.RTC_Seconds = timeinfo->tm_sec;
    timeStruct.RTC_H12     = RTC_HourFormat_24;
    
    // Change the RTC current date/time
    PWR_BackupAccessCmd(ENABLE); // Enable access to RTC    
    RTC_SetDate(RTC_Format_BIN, &dateStruct);
    RTC_SetTime(RTC_Format_BIN, &timeStruct);    
    PWR_BackupAccessCmd(DISABLE); // Disable access to RTC
}

#endif
