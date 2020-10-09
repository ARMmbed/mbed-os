/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

#include "mbed_assert.h"
#include "device.h"

#if DEVICE_RTC

#include "rtc_api.h"
#include "iodefine.h"
#include "mbed_drv_cfg.h"

#ifdef USE_RTCX1_CLK
#elif defined(USE_EXTAL_CLK)
#else
#error Select RTC clock input !
#endif

#define READ_LOOP_MAX    (2000)
#define TIME_ERROR_VAL   (0u)

/*
 * Setup the RTC based on a time structure.
 * The rtc_init function should be executed first.
 * [in]
 * None.
 * [out]
 * None.
 */
void rtc_init(void)
{
    volatile int i;

    CPG.STBCR5.BIT.MSTP52 = 0;

    // Set control register
#if defined(USE_RTCX1_CLK)
    RTC_BCNT1.RCR4.BIT.RCKSEL = 0;
    RTC_BCNT1.RCR3.BIT.RTCEN  = 1;
#elif defined(USE_EXTAL_CLK)
    PMG.RTCXTALSEL.BIT.RTC1XT = 1;
    RTC_BCNT1.RCR4.BIT.RCKSEL = 1;
    RTC_BCNT1.RCR3.BIT.RTCEN  = 0;
#endif

    i = 0;
    while (i < 1000) {
        i++;
    }

    RTC_BCNT1.RCR2.BIT.START = 0;
    for (i = 0; (i < READ_LOOP_MAX) && (RTC_BCNT1.RCR2.BIT.START != 0); i++) {
        ;
    }

#if defined(USE_EXTAL_CLK)
    // Clockin  = 24MHz
    RTC_BCNT1.RFRH.WORD = 0x0001;
    RTC_BCNT1.RFRL.WORD = 0x6E35;
#endif

    RTC_BCNT1.RCR2.BIT.CNTMD = 1;
    for (i = 0; (i < READ_LOOP_MAX) && (RTC_BCNT1.RCR2.BIT.CNTMD != 1); i++) {
        ;
    }

    RTC_BCNT1.RCR2.BIT.RESET = 1;
    for (i = 0; (i < READ_LOOP_MAX) && (RTC_BCNT1.RCR2.BIT.RESET != 0); i++) {
        ;
    }

    RTC_BCNT1.RCR1.BIT.AIE = 1;
    RTC_BCNT1.RCR1.BIT.PIE = 0;

    RTC_BCNT1.RCR2.BIT.START = 1;
    for (i = 0; (i < READ_LOOP_MAX) && (RTC_BCNT1.RCR2.BIT.START != 1); i++) {
        ;
    }
}


/*
 * Release the RTC based on a time structure.
 * @note This function does not stop the RTC from counting
 * [in]
 * None.
 * [out]
 * None.
 */
void rtc_free(void)
{
#if defined(USE_EXTAL_CLK)
    PMG.RTCXTALSEL.BIT.RTC1XT = 0;
#endif
}


/*
 * Check the RTC has been enabled.
 * Clock Control Register RTC_BCNT1.RCR1.BYTE(bit3): 0 = Disabled, 1 = Enabled.
 * [in]
 * None.
 * [out]
 * 0:Disabled, 1:Enabled.
 */
int rtc_isenabled(void)
{
    int ret_val = 0;

    if (RTC_BCNT1.RCR2.BIT.START == 1) { // RTC ON ?
        ret_val = 1;
    }

    return ret_val;
}


/*
 * RTC read function.
 * [in]
 * None.
 * [out]
 * UNIX timestamp value.
 */
time_t rtc_read(void)
{
    time_t t;

    if (rtc_isenabled() != 0) {
        RTC_BCNT1.RCR1.BIT.CIE = 0; // CIE = 0
        do {
            RTC_BCNT1.RSR.BIT.CF = 0;

            // Read RTC register
            t = ((time_t)RTC_BCNT1.BCNT0.BYTE <<  0)
                | ((time_t)RTC_BCNT1.BCNT1.BYTE <<  8)
                | ((time_t)RTC_BCNT1.BCNT2.BYTE << 16)
                | ((time_t)RTC_BCNT1.BCNT3.BYTE << 24);
        } while (RTC_BCNT1.RSR.BIT.CF != 0);
    } else {
        // Error
        t = TIME_ERROR_VAL;
    }

    return t;
}

/*
 * RTC write function
 * [in]
 * t:UNIX timestamp value
 * [out]
 * None.
 */
void rtc_write(time_t t)
{
    volatile int i;

    if (rtc_isenabled() != 0) {
        RTC_BCNT1.RCR2.BIT.START = 0;
        for (i = 0; (i < READ_LOOP_MAX) && (RTC_BCNT1.RCR2.BIT.START != 0); i++) {
            ;
        }

        RTC_BCNT1.RCR2.BIT.RESET = 1;
        for (i = 0; (i < READ_LOOP_MAX) && (RTC_BCNT1.RCR2.BIT.RESET != 0); i++) {
            ;
        }

        RTC_BCNT1.BCNT0.BYTE = (uint8_t)(t >>  0);
        RTC_BCNT1.BCNT1.BYTE = (uint8_t)(t >>  8);
        RTC_BCNT1.BCNT2.BYTE = (uint8_t)(t >> 16);
        RTC_BCNT1.BCNT3.BYTE = (uint8_t)(t >> 24);

        RTC_BCNT1.RCR2.BIT.START = 1;
        for (i = 0; (i < READ_LOOP_MAX) && (RTC_BCNT1.RCR2.BIT.START != 1); i++) {
            ;
        }
    }
}

#endif /* DEVICE_RTC */
