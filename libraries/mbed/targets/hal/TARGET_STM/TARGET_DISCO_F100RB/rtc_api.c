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
