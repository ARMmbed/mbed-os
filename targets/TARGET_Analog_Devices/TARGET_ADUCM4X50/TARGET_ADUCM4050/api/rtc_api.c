/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#if DEVICE_RTC

#include "adi_rtc.h"
#include "adi_pwr.h"

#define RTC_DEVICE_NUM    0
static uint8_t aRtcDevMem0[ADI_RTC_MEMORY_SIZE];
static ADI_RTC_HANDLE hDevice0 = NULL;


void rtc_init(void)
{
    /* Setup the RTC 32KHz oscillator */
    adi_pwr_SetLFClockMux(ADI_CLOCK_MUX_LFCLK_LFXTAL);
    adi_pwr_EnableClockSource(ADI_CLOCK_SOURCE_LFXTAL,true);

    /* initialize driver */
    adi_rtc_Open(RTC_DEVICE_NUM,aRtcDevMem0,ADI_RTC_MEMORY_SIZE,&hDevice0);

    adi_rtc_Enable(hDevice0, true);
}

void rtc_free(void)
{
    adi_rtc_Close(hDevice0);
}

/*
 * Little check routine to see if the RTC has been enabled
 * 0 = Disabled, 1 = Enabled
 */
int rtc_isenabled(void)
{
    uint32_t ControlReg;

    adi_rtc_GetControl (hDevice0, ADI_RTC_CONTROL_REGISTER_0 ,&ControlReg);

    return((int) (ControlReg & BITM_RTC_CR0_CNTEN));
}

time_t rtc_read(void)
{
    time_t currentCount;

    adi_rtc_GetCount(hDevice0, (uint32_t *)(&currentCount));

    return(currentCount);
}

void rtc_write(time_t t)
{
    adi_rtc_SetCount (hDevice0, t);
}

#endif //  #if DEVICE_RTC

