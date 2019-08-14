/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#include "ti/devices/cc32xx/inc/hw_types.h"
#include "ti/devices/cc32xx/driverlib/prcm.h"

void rtc_init(void)
{
    static bool rtc_initialized = false;
    if (!rtc_initialized) {
        if (!PRCMRTCInUseGet()) {
            PRCMRTCInUseSet();
        }
        rtc_initialized = true;
    }
}

void rtc_free(void)
{
}

int rtc_isenabled(void)
{
    return PRCMRTCInUseGet();
}

time_t rtc_read(void)
{
    unsigned long ulSecs = 0;
    unsigned short usMsec = 0;
    PRCMRTCGet(&ulSecs, &usMsec);
    return ulSecs;
}

void rtc_write(time_t t)
{
    PRCMRTCSet(t, 0);
}
#endif
