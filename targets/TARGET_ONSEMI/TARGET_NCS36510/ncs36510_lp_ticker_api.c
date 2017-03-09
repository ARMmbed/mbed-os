/**
 *******************************************************************************
 * @file    lpticker.c
 * @brief Implementation of a Low Power Ticker functionality based on RTC
 * @internal
 * @author ON Semiconductor
 * $Rev:  $
 * $Date: $
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
 * @ingroup lpticker
 *
 * @details
 * This is Low Power ticker derived from RTC
 *
 */

#include "device.h"
#if DEVICE_LOWPOWERTIMER

#include "cmsis_nvic.h"
#include "lp_ticker_api.h"
#include "rtc.h"
#include "rtc_map.h"

/* Initialize the RTC for low power ticker */
void lp_ticker_init()
{
    fRtcInit();
}

/* Return the current RTC counter value in us */
uint32_t lp_ticker_read()
{
    return (uint32_t)(fRtcRead() & 0xFFFFFFFF); /* TODO Truncating 64 bit value to 32 bit */
}

/* Set interrupt for specified time */
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    /* The RTC Match register needs to be Set to the RTC alarm value */
    fRtcSetInterrupt(timestamp);
}

/** Disable low power ticker interrupt
 *
 */
void lp_ticker_disable_interrupt(void)
{
    fRtcDisableInterrupt();
}

/** Clear the low power ticker interrupt
 *
 */
void lp_ticker_clear_interrupt(void)
{
    fRtcClearInterrupt();
}

#endif /* DEVICE_LOWPOWERTIMER */
