/**
 *******************************************************************************
 * @file    lpticker.c
 * @brief Implementation of a Low Power Ticker functionality based on RTC
 * @internal
 * @author ON Semiconductor
 * $Rev:  $
 * $Date: $
 ******************************************************************************
 * @copyright (c) 2015 ON Semiconductor. All rights reserved.
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
 * @ingroup lpticker
 *
 * @details
 * This is Low Power ticker derived from RTC
 *
 */

#include "device.h"
#if DEVICE_LOWPOWERTIMER

#include "sleep_api.h"
#include "cmsis_nvic.h"
#include "lp_ticker_api.h"
#include "rtc.h"
#include "rtc_map.h"
#include "sleep.h"

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

/*Return the time that gets cut off when you return just a 32 bit  us resolution number */
uint32_t lp_ticker_get_overflows_counter(void)
{
 /* To check; do we need an counter in software in RTC to find overflows */
  	uint64_t now = fRtcRead();
	uint32_t overflow = (now & 0xFFFFFFFF00000000) >> 32;
	return overflow;
}

/* Return the RTC Match counter contents */
uint32_t lp_ticker_get_compare_match()
{
	/* read the alarms and convert to us */
	uint16_t sub_second_alarm = RTCREG->SUB_SECOND_ALARM;
	uint32_t second_alarm = RTCREG->SECOND_ALARM;
	uint64_t alarm_us = (uint64_t)((((float)sub_second_alarm / RTC_CLOCK_HZ) * RTC_SEC_TO_US) +
					               (second_alarm * RTC_SEC_TO_US));
	/* TODO truncating to 32 bits */
	return (uint32_t)(alarm_us & 0xFFFFFFFF);
}

/* sleep until alarm */
void lp_ticker_sleep_until(uint32_t now, uint32_t time)
{
	/* Set the interrupt */
	lp_ticker_set_interrupt(time);

	/* Go to sleep */
	sleep_t obj;
	obj.SleepType 	 = SLEEP_TYPE_NONE;
	obj.timeToSleep = time - now;

	mbed_enter_sleep(&obj);
	/* TBD: This is dummy exit for now; once the entered sleep it should be
	removed and sleep exit should happen through interrupt */
	mbed_exit_sleep(&obj);
}

#ifdef TOOLCHAIN_ARM
/* Dummy functions added for the compilation of ARMCC compiler toolchain */
/** Disable low power ticker interrupt
 *
 */
void lp_ticker_disable_interrupt(void)
{
	//TODO : dummy function to get compilation running
}

/** Clear the low power ticker interrupt
 *
 */
void lp_ticker_clear_interrupt(void)
{
	//TODO : dummy function to get compilation running
}
#endif /* TOOLCHAIN_ARM */

#endif /* DEVICE_LOWPOWERTIMER */
