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

#include "fsl_snvs_hp.h"

void rtc_init(void)
{
    snvs_hp_rtc_config_t snvsRtcConfig;

    SNVS_HP_RTC_GetDefaultConfig(&snvsRtcConfig);
    SNVS_HP_RTC_Init(SNVS, &snvsRtcConfig);

    SNVS_HP_RTC_StartTimer(SNVS);
}

void rtc_free(void)
{
    SNVS_HP_RTC_Deinit(SNVS);
}

/*
 * Little check routine to see if the RTC has been enabled
 * 0 = Disabled, 1 = Enabled
 */
int rtc_isenabled(void)
{
    return (int)((SNVS->HPCR & SNVS_HPCR_RTC_EN_MASK) >> SNVS_HPCR_RTC_EN_SHIFT);
}

time_t rtc_read(void)
{
    uint64_t seconds = 0;
    uint64_t tmp = 0;

    /* Do consecutive reads until value is correct */
    do
    {
        seconds = tmp;
        tmp = SNVS->HPRTCLR;
    } while (tmp != seconds);

    return (time_t)seconds;
}

void rtc_write(time_t t)
{
    if (t == 0) {
        t = 1;
    }
    SNVS_HP_RTC_StopTimer(SNVS);

    SNVS->HPRTCLR = (uint32_t)t;

    SNVS_HP_RTC_StartTimer(SNVS);
}

#endif
