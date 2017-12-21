/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "rtc_api.h"

#if DEVICE_RTC

#include "adi_rtc.h"
#include "adi_pwr.h"

#define RTC_DEVICE_NUM    0
/*******************************************************************************
   ADI_RTC_DEVICE Instance memory containing memory pointer should guarantee
   4 byte alignmnet.
 *******************************************************************************/
static uint32_t       aRtcDevMem0[(ADI_RTC_MEMORY_SIZE + 3)/4];
static ADI_RTC_HANDLE hDevice0 = NULL;


void rtc_init(void)
{
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

    adi_rtc_GetControl (hDevice0, ADI_RTC_CONTROL_REGISTER_0,&ControlReg);

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
