/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#if DEVICE_RTC

#include <stdio.h>
#include <string.h>
#include "rtc_api.h"
#include "rtc.h"
#include "mxc_sys.h"


//******************************************************************************
void rtc_init(void)
{
    // Enable clock
    MXC_SYS_RTCClockEnable();
    
    while(MXC_RTC_Start() == E_BUSY) {
        ;
    }
}

//******************************************************************************
void rtc_free(void)
{

}

//******************************************************************************
int rtc_isenabled(void)
{
    return ((MXC_RTC->ctrl & MXC_F_RTC_CTRL_EN) >> MXC_F_RTC_CTRL_EN_POS);
}

//******************************************************************************
void rtc_write(time_t t)
{
    MXC_RTC_Stop();
    MXC_RTC_Init(t, 0); 

    while(MXC_RTC_Start() == E_BUSY) {
       ;
    }
}

//******************************************************************************
time_t rtc_read(void)
{
    uint32_t sec = 0;
    uint32_t subsec = 0;
    
    while (MXC_RTC_GetTime (&sec, &subsec) != 0) {
        ;
    }

    subsec &= 0xFFF; //It is 12 bits, 0xFFF = (1<<12)-1
    if (subsec > 2048) {
        sec += 1; // rolover
    }

    return sec;
}

#endif // DEVICE_RTC
