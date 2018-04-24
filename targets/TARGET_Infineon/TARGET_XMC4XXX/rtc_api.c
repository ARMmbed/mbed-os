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

/***************************************************************** Includes **/
#include "mbed_assert.h"
#include "xmc_scu.h"
#include "xmc_rtc.h"
#include "rtc_api.h"

/****************************************************************** Globals **/
XMC_RTC_CONFIG_t rtc_config =
{
  .prescaler = 0x7fffU
};

/**************************************************************** Functions **/

/** Initialize the RTC peripheral
 *
 */
void rtc_init(void){

    XMC_SCU_HIB_EnableHibernateDomain();
    XMC_SCU_HIB_SetRtcClockSource(XMC_SCU_HIB_RTCCLKSRC_OSI);

    XMC_RTC_Init(&rtc_config);
    XMC_RTC_Enable();
    XMC_RTC_Start();
}

/** Deinitialize RTC
 *
 */
void rtc_free(void){

    XMC_RTC_Stop();
    XMC_RTC_Disable();
}

/** Get the RTC enable status
 *
 * @retval 0 disabled
 * @retval 1 enabled
 */
int rtc_isenabled(void){

    if(XMC_RTC_IsEnabled()){
        return 1;
    }else{
        return 0;
    }
}

/** Get the current time from the RTC peripheral
 *
 * @return The current time
 */
time_t rtc_read(void){

    struct tm t;

    XMC_RTC_GetTimeStdFormat(&t);
    return mktime(&t);
}

/** Set the current time to the RTC peripheral
 *
 * @param t The current time to be set
 */
void rtc_write(time_t t){

    XMC_RTC_SetTimeStdFormat(localtime(&t));
}

/*EOF*/
