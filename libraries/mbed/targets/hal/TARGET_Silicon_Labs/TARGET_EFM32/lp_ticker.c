/***************************************************************************//**
 * @file lp_ticker.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include "device.h"
#if DEVICE_LOWPOWERTIMER

#include "rtc_api.h"
#include "rtc_api_HAL.h"
#include "lp_ticker_api.h"

void lp_ticker_init()
{
    rtc_init_real(RTC_INIT_LPTIMER);
    rtc_set_comp0_handler((uint32_t)lp_ticker_irq_handler);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint64_t timestamp_ticks;
    uint64_t current_ticks = RTC_CounterGet();
    timestamp_t current_time = ((uint64_t)(current_ticks * 1000000) / (LOW_ENERGY_CLOCK_FREQUENCY / RTC_CLOCKDIV_INT));


    /* calculate offset value */
    timestamp_t offset = timestamp - current_time;
    if(offset > 0xEFFFFFFF) offset = 100;

    /* map offset to RTC value */
    // ticks = offset * RTC frequency div 1000000
    timestamp_ticks = ((uint64_t)offset * (LOW_ENERGY_CLOCK_FREQUENCY / RTC_CLOCKDIV_INT)) / 1000000;
    timestamp_ticks += current_ticks;

    /* RTC has 24 bit resolution */
    timestamp_ticks &= 0xFFFFFF;

    /* check for RTC limitation */
    if((timestamp_ticks - RTC_CounterGet()) >= 0x800000) timestamp_ticks = RTC_CounterGet() + 2;

    /* Set callback */
    RTC_FreezeEnable(true);
    RTC_CompareSet(0, (uint32_t)timestamp_ticks);
    RTC_IntEnable(RTC_IF_COMP0);
    RTC_FreezeEnable(false);
}

inline void lp_ticker_disable_interrupt()
{
    RTC_IntDisable(RTC_IF_COMP0);
}

inline void lp_ticker_clear_interrupt()
{
    RTC_IntClear(RTC_IF_COMP0);
}

timestamp_t lp_ticker_read()
{
    uint64_t ticks_temp;
    uint64_t ticks = RTC_CounterGet();

    /* ticks = counter tick value
     * timestamp = value in microseconds
     * timestamp = ticks * 1.000.000 / RTC frequency
     */

    ticks_temp = (ticks * 1000000) / (LOW_ENERGY_CLOCK_FREQUENCY / RTC_CLOCKDIV_INT);
    return (timestamp_t) (ticks_temp & 0xFFFFFFFF);
}

#endif
