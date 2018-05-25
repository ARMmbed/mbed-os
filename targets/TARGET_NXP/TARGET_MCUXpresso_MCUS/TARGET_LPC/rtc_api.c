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

#if DEVICE_RTC || DEVICE_LPTICKER

#include "pinmap.h"
#include "fsl_rtc.h"
#include "PeripheralPins.h"

extern void rtc_setup_oscillator(void);

static bool rtc_time_set = false;

void rtc_init(void)
{
    rtc_setup_oscillator();

    RTC_Init(RTC);

    RTC_StartTimer(RTC);
}

void rtc_free(void)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Gate the module clock */
    CLOCK_DisableClock(kCLOCK_Rtc);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/*
 * Little check routine to see if the RTC has been enabled
 * 0 = Disabled, 1 = Enabled
 */
int rtc_isenabled(void)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the RTC peripheral clock */
    CLOCK_EnableClock(kCLOCK_Rtc);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    const bool rtc_init_done = ((RTC->CTRL & RTC_CTRL_RTC_EN_MASK) >> RTC_CTRL_RTC_EN_SHIFT);

    /* If RTC is not initialized, then disable the clock gate on exit. */
    if(!rtc_init_done) {
        rtc_free();
    }

    return (rtc_init_done & rtc_time_set);
}

time_t rtc_read(void)
{
    return (time_t)RTC->COUNT;
}

void rtc_write(time_t t)
{
    RTC_StopTimer(RTC);
    RTC->COUNT = t;
    RTC_StartTimer(RTC);

    rtc_time_set = true;
}

#endif
