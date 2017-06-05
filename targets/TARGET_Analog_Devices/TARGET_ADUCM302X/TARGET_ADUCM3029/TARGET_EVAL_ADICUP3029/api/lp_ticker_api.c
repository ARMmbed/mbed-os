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

#include <sys/platform.h>
#include "lp_ticker_api.h"
#include <drivers/rtc/adi_rtc.h>
#include <drivers/pwr/adi_pwr.h>

#ifdef DEVICE_LOWPOWERTIMER

// define base LF clock frequncy in Hz
#define LFCLK_FREQUENCY_HZ		32768
// RTC prescaler for the LF clock
#define RTC_PRESCALER         	0

/* time for each tick of the LF clock in us */
#define TIME_US_PER_TICK 	((float)1000000/(float)(LFCLK_FREQUENCY_HZ>>RTC_PRESCALER))

// The number of clock ticks it takes to set & enable the alarm
#define TICKS_TO_ENABLE_ALARM 40

static unsigned char rtc1_memory[ADI_RTC_MEMORY_SIZE];
static ADI_RTC_HANDLE hRTC1_Device;
static volatile unsigned int rtc1_use_interrupt;

/**
 * \defgroup hal_LpTicker Low Power Ticker Functions
 * @{
 */
/**
 * Local RTC 1 ISR callback function.
 *
 */
void rtc1_Callback (void *pCBParam, uint32_t nEvent, void *EventArg)
{
    /* process RTC interrupts (cleared by driver) */
    if (ADI_RTC_ALARM_INT & nEvent) {
        lp_ticker_irq_handler();
    }
}


/** Get low power ticker's data
 *
 * @return The low power ticker data
 */
/*
const ticker_data_t* get_lp_ticker_data()
{
}*/


/* HAL lp ticker */

/** Initialize the low power ticker
 *
 */
void lp_ticker_init()
{
    // select LF clock
    adi_pwr_SetLFClockMux(ADI_CLOCK_MUX_LFCLK_LFOSC);

    // open the rtc device
    adi_rtc_Open(1, rtc1_memory, ADI_RTC_MEMORY_SIZE, &hRTC1_Device);

    // set the prescaler value
    adi_rtc_SetPreScale(hRTC1_Device, RTC_PRESCALER);

    // register call back for events
    adi_rtc_RegisterCallback(hRTC1_Device, rtc1_Callback, hRTC1_Device);

    // reset the RTC counter
    adi_rtc_SetCount(hRTC1_Device, 0);

    // set the trim value
    adi_rtc_SetTrim(hRTC1_Device, ADI_RTC_TRIM_INTERVAL_14, ADI_RTC_TRIM_1, ADI_RTC_TRIM_SUB);

    adi_rtc_EnableTrim(hRTC1_Device, true);

    // enable the RTC
    adi_rtc_Enable(hRTC1_Device, true);

    // interrupt is not yet used
    rtc1_use_interrupt = 0;
}

/** Read the current counter
 *
 * @return The current timer's counter value in microseconds
 */
uint32_t lp_ticker_read()
{
    uint32_t count;
    float t;

    // get current count
    adi_rtc_GetCount(hRTC1_Device, &count);

    // convert ticks to us
    t = (float)count * TIME_US_PER_TICK;

    count = (uint32_t)t;

    return count;
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in microseconds to be set
 */
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t rtcCount, set_time;

    // compute the tick value based on the given alarm time
    set_time = (uint32_t)((float)(timestamp) / TIME_US_PER_TICK);

    // get current count
    adi_rtc_GetCount(hRTC1_Device, &rtcCount);

    // alarm value needs to be greater than the current time
    // if already expired, call user ISR immediately
    if (set_time <= rtcCount) {
        rtc1_use_interrupt = 0;
        rtc1_Callback(NULL, ADI_RTC_ALARM_INT, NULL);
        return;
    } else if (set_time <= rtcCount + TICKS_TO_ENABLE_ALARM) {
        // otherwise if the alarm time is less than the current RTC count + the time
        // it takes to enable the alarm, just wait until the desired number of counts
        // has expired rather than using the interrupt, then call the user ISR directly.
        do {
            adi_rtc_GetCount(hRTC1_Device, &rtcCount);
        } while (rtcCount < set_time);

        rtc1_use_interrupt = 0;
        rtc1_Callback(NULL, ADI_RTC_ALARM_INT, NULL);
        return;
    }

    // set the alarm
    adi_rtc_EnableInterrupts(hRTC1_Device, ADI_RTC_ALARM_INT, true);
    adi_rtc_SetAlarm(hRTC1_Device, set_time);
    adi_rtc_EnableAlarm(hRTC1_Device,true);
    rtc1_use_interrupt = 1;
}

/** Disable low power ticker interrupt
 *
 */
void lp_ticker_disable_interrupt()
{
    // Disable alarm only if it's used in the current context
    // This is done to get around the issue that it takes a while for the
    // adi_rtc_EnableInterrupts() to disable the interrupts since the RTC
    // is in a slower clock domain. For alarms in the 1ms or less range,
    // disabling interrupt can take much longer than the alarm duration. In
    // these cases, interrupt is not used, so the interrupt does not need to
    // be disabled.
    if (rtc1_use_interrupt) {
        adi_rtc_EnableInterrupts(hRTC1_Device, ADI_RTC_ALARM_INT, false);
    }
}

/** Clear the low power ticker interrupt
 *
 */
void lp_ticker_clear_interrupt()
{
    return;
}

#endif
/**@}*/
