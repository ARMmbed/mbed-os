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
#include "adi_rtc_def.h"


#ifdef DEVICE_LOWPOWERTIMER

// define base LF clock frequncy in Hz
#define LFCLK_FREQUENCY_HZ		32768
// RTC prescaler for the LF clock
#define RTC_PRESCALER         	0

/* time for each tick of the LF clock in us */
#define TIME_US_PER_TICK 	((float)1000000/(float)(LFCLK_FREQUENCY_HZ>>RTC_PRESCALER))

// The number of RTC clock ticks it takes to set & enable the alarm
#define TICKS_TO_ENABLE_ALARM 10

// Mask to limit the value of the RTC value to 27 bits so the converted time in us fits
// in 32-bits
#define MAX_TICK_MASK ((1 << 27) - 1)

static unsigned char rtc1_memory[ADI_RTC_MEMORY_SIZE];
static ADI_RTC_HANDLE hRTC1_Device;


/**
 * \defgroup hal_LpTicker Low Power Ticker Functions
 * @{
 */
/**
 * Local stream-lined alarm setting function.
 *
 */
static int set_rtc_alarm_interrupt(ADI_RTC_HANDLE const hDevice, uint32_t nAlarm)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint16_t cr0;
    ADI_INT_STATUS_ALLOC();

    // Section to enable interrupts
    // The interrupt used is ADI_RTC_ALARM_INT
    // Set the Alarm interrupt enable and Alarm enable bits in cr0
    cr0 = BITM_RTC_CR0_ALMEN | (1u << BITP_RTC_CR0_ALMINTEN);

    // Set the alarm value
    /* Wait till previously posted write to Alram Register to complete */
    PEND_BEFORE_WRITE(SR1,(BITM_RTC_SR1_WPNDALM0|BITM_RTC_SR1_WPNDALM1))

    ADI_ENTER_CRITICAL_REGION();
    /* Program the alarm count registers. */
    pDevice->pRTCRegs->ALM0 = (uint16_t)nAlarm;
    pDevice->pRTCRegs->ALM1 = (uint16_t)(nAlarm >> 16);
    pDevice->pRTCRegs->ALM2 = 0u;
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */
    SYNC_AFTER_WRITE(SR0,(BITM_RTC_SR0_WSYNCALM0|BITM_RTC_SR0_WSYNCALM1))

    // Enable alarm
    /* Wait till previously posted write to Control Register to complete */
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDALM1|BITM_RTC_SR1_WPNDALM0)

    ADI_ENTER_CRITICAL_REGION();
    /* set RTC alarm and interrupt enable */
    pDevice->pRTCRegs->CR0 |= cr0;
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCR0)

    return ADI_RTC_SUCCESS;
}



/**
 * Local RTC 1 ISR callback function.
 *
 */
static void rtc1_Callback (void *pCBParam, uint32_t nEvent, void *EventArg)
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

    // Throw away top 5 bits to avoid overflow and convert ticks to us
    count &= MAX_TICK_MASK;
    t = (float)count * TIME_US_PER_TICK;

    return (uint32_t)t;
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in microseconds to be set
 */
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t rtcCount, trunc_rtcCount, set_tick, tick_delta, alarm_tick;

    // compute the tick value based on the given alarm time
    set_tick = (uint32_t)((float)(timestamp) / TIME_US_PER_TICK);

    // get current count
    adi_rtc_GetCount(hRTC1_Device, &rtcCount);

    // compute the number of ticks required for the alarm
    trunc_rtcCount = rtcCount & MAX_TICK_MASK;

    // if set_tick is less than the current RTC count, the counter has
    // missed the desired time, so set tick_delta to 0
    if (set_tick >= trunc_rtcCount) {
        tick_delta = set_tick - trunc_rtcCount;
    } else {
        tick_delta = 0;
    }

    // compute the absolute RTC alarm count required
    alarm_tick = rtcCount + tick_delta;

    // check if the desired alarm duration is less than TICKS_TO_ENABLE_ALARM,
    // if so just wait it out rather than setting the alarm
    if (tick_delta <= TICKS_TO_ENABLE_ALARM) {
        // otherwise if the alarm time is less than the current RTC count + the time
        // it takes to enable the alarm, just wait until the desired number of counts
        // has expired rather than using the interrupt, then call the user ISR directly.
        do {
            adi_rtc_GetCount(hRTC1_Device, &rtcCount);
        } while (rtcCount < alarm_tick);

        rtc1_Callback(NULL, ADI_RTC_ALARM_INT, NULL);
        return;
    }

    // set the alarm otherwise
    set_rtc_alarm_interrupt(hRTC1_Device, alarm_tick);
}

/** Disable low power ticker interrupt
 *
 */
void lp_ticker_disable_interrupt()
{
    adi_rtc_EnableInterrupts(hRTC1_Device, ADI_RTC_ALARM_INT, false);
}

/** Clear the low power ticker interrupt
 *
 */
void lp_ticker_clear_interrupt()
{
    ADI_RTC_DEVICE *pDevice = hRTC1_Device;

    NVIC_ClearPendingIRQ(pDevice->eIRQn);

    return;
}


/** Set pending interrupt that should be fired right away.
 *
 * The ticker should be initialized prior calling this function.
 */
void lp_ticker_fire_interrupt(void)
{
    ADI_RTC_DEVICE *pDevice = hRTC1_Device;

    NVIC_SetPendingIRQ(pDevice->eIRQn);
}


#endif
/**@}*/
