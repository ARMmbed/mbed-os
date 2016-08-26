/**
******************************************************************************
* @file rtc_api.c
* @brief Implementation of a RTC driver
* @internal
* @author ON Semiconductor
* $Rev: 0.1 $
* $Date: 2016-01-20 12:09:00 +0530 (Wed, 20 Jan 2016) $
******************************************************************************
* @copyright (c) 2012 ON Semiconductor. All rights reserved.
* ON Semiconductor is supplying this software for use with ON Semiconductor
* processor based microcontrollers only.
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

static IRQn_Type Irq;

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
    return (uint32_t)(fRtcRead() & 0xFFFFFFFF); /* TODO Truncating 64 bit value to 32 bit */
}

/* See rtc_apc.h for description */
void rtc_write(time_t t)
{
    fRtcWrite(t);
}

#endif /* DEVICE_RTC */
