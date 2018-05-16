/**
******************************************************************************
* @file rtc_api.c
* @brief Implementation of a RTC driver
* @internal
* @author ON Semiconductor
* $Rev: 0.1 $
* $Date: 2016-01-20 12:09:00 +0530 (Wed, 20 Jan 2016) $
******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup rtc
*
*/
#include "rtc_api.h"
#if DEVICE_RTC
#include "device.h"

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "mbed_assert.h"
#include <string.h>
#include "rtc.h"
#include "cmsis_nvic.h"

#define US_PER_SEC 1000000

static time_t m_time_base;

static uint32_t rtc_seconds_get()
{
    return (uint32_t)((fRtcRead() / US_PER_SEC) & 0xFFFFFFFF);
}

/* See rtc_apc.h for description */

void rtc_init(void)
{
    fRtcInit();
}

/* See rtc_apc.h for description */
void rtc_free(void)
{
    fRtcFree();
}

/* See rtc_apc.h for description */
int rtc_isenabled(void)
{
    return(fIsRtcEnabled());
}

/* See rtc_apc.h for description */
time_t rtc_read(void)
{
    return m_time_base + rtc_seconds_get();
}

/* See rtc_apc.h for description */
void rtc_write(time_t t)
{
    uint32_t seconds;
    do {
        seconds = rtc_seconds_get();
        m_time_base = t - seconds;
    /* If the number of seconds indicated by the counter changed during the
       update of the time base, just repeat the update, now using the new
       number of seconds. */
    } while (seconds != rtc_seconds_get());
}

#endif /* DEVICE_RTC */
