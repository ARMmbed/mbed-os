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

#if DEVICE_RTC

#include "fsl_snvs_lp.h"

static bool rtc_time_set = false;

void rtc_init(void)
{
    snvs_lp_srtc_config_t snvsRtcConfig;

    SNVS_LP_SRTC_GetDefaultConfig(&snvsRtcConfig);
    SNVS_LP_SRTC_Init(SNVS, &snvsRtcConfig);

    SNVS_LP_SRTC_StartTimer(SNVS);
}

void rtc_free(void)
{
#if (!(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && \
         defined(SNVS_LP_CLOCKS))
    CLOCK_DisableClock(kCLOCK_SnvsLp0);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/*
 * Little check routine to see if the RTC has been enabled
 * 0 = Disabled, 1 = Enabled
 */
int rtc_isenabled(void)
{
#if (!(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && \
         defined(SNVS_LP_CLOCKS))
    CLOCK_EnableClock(kCLOCK_SnvsLp0);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */


    const bool rtc_init_done = ((SNVS->LPCR & SNVS_LPCR_SRTC_ENV_MASK) >> SNVS_LPCR_SRTC_ENV_SHIFT);

    /* If RTC is not initialized, then disable the clock gate on exit. */
    if(!rtc_init_done) {
        rtc_free();
    }

    return (rtc_init_done & rtc_time_set);
}

time_t rtc_read(void)
{
    uint32_t seconds = 0;
    uint32_t tmp = 0;

    /* Do consecutive reads until value is correct */
    do {
        seconds = tmp;
        tmp = (SNVS->LPSRTCMR << 17U) | (SNVS->LPSRTCLR >> 15U);
    } while (tmp != seconds);

    return (time_t)seconds;
}

void rtc_write(time_t t)
{
    if (t == 0) {
        t = 1;
    }

    SNVS_LP_SRTC_StopTimer(SNVS);

    SNVS->LPSRTCMR = (uint32_t)(t >> 17U);
    SNVS->LPSRTCLR = (uint32_t)(t << 15U);

    SNVS_LP_SRTC_StartTimer(SNVS);

    rtc_time_set = true;
}

#endif
