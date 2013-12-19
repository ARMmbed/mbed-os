/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

static int rtc_inited = 0;

void rtc_init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // Enable PWR and Backup clock

    PWR_BackupAccessCmd(ENABLE); // Allow access to Backup Domain
  
    BKP_DeInit(); // Reset Backup Domain
  
    // Uncomment these lines if you use the LSE
    // Enable LSE and wait till it's ready
    //RCC_LSEConfig(RCC_LSE_ON);
    //while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {}     
    //RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // Select LSE as RTC Clock Source
      
    // Uncomment these lines if you use the LSI
    // Enable LSI and wait till it's ready  
    RCC_LSICmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) {}
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI); // Select LSI as RTC Clock Source
  
    RCC_RTCCLKCmd(ENABLE); // Enable RTC Clock 
      
    RTC_WaitForSynchro(); // Wait for RTC registers synchronization
      
    RTC_WaitForLastTask(); // Wait until last write operation on RTC registers has finished

    // Set RTC period to 1 sec
    // For LSE: prescaler = RTCCLK/RTC period = 32768Hz/1Hz = 32768
    // For LSI: prescaler = RTCCLK/RTC period = 40000Hz/1Hz = 40000      
    RTC_SetPrescaler(39999);
    
    RTC_WaitForLastTask(); // Wait until last write operation on RTC registers has finished
      
    rtc_inited = 1;
}

void rtc_free(void) {
    RCC_DeInit(); // Resets the RCC clock configuration to the default reset state
    rtc_inited = 0;
}

int rtc_isenabled(void) {
    return rtc_inited;
}

time_t rtc_read(void) {
    return (time_t)RTC_GetCounter();
}

void rtc_write(time_t t) {
    RTC_WaitForLastTask(); // Wait until last write operation on RTC registers has finished
    RTC_SetCounter(t); // Change the current time
    RTC_WaitForLastTask(); // Wait until last write operation on RTC registers has finished
}
