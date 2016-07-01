/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "us_ticker_api.h"
#include "common_rtc.h"
#include "app_util.h"


//------------------------------------------------------------------------------
// Common stuff used also by lp_ticker and rtc_api (see "common_rtc.h").
//
#include "nrf_drv_clock.h"
#include "app_util_platform.h"

bool              m_common_rtc_enabled = false;
uint32_t volatile m_common_rtc_overflows = 0;

#if defined(TARGET_MCU_NRF51822)
void common_rtc_irq_handler(void)
#else
void COMMON_RTC_IRQ_HANDLER(void)
#endif
{
    nrf_rtc_event_t event;
    uint32_t int_mask;

    event = US_TICKER_EVENT;
    int_mask = US_TICKER_INT_MASK;
    if (nrf_rtc_event_pending(COMMON_RTC_INSTANCE, event)) {
        nrf_rtc_event_clear(COMMON_RTC_INSTANCE, event);
        nrf_rtc_event_disable(COMMON_RTC_INSTANCE, int_mask);

        us_ticker_irq_handler();
    }

#if DEVICE_LOWPOWERTIMER
    event = LP_TICKER_EVENT;
    int_mask = LP_TICKER_INT_MASK;
    if (nrf_rtc_event_pending(COMMON_RTC_INSTANCE, event)) {
        nrf_rtc_event_clear(COMMON_RTC_INSTANCE, event);
        nrf_rtc_event_disable(COMMON_RTC_INSTANCE, int_mask);
    }
#endif

    event = NRF_RTC_EVENT_OVERFLOW;
    if (nrf_rtc_event_pending(COMMON_RTC_INSTANCE, event)) {
        nrf_rtc_event_clear(COMMON_RTC_INSTANCE, event);
        // Don't disable this event. It shall occur periodically.

        ++m_common_rtc_overflows;
    }
}

void common_rtc_init(void)
{
    if (m_common_rtc_enabled) {
        return;
    }

    // RTC is driven by the low frequency (32.768 kHz) clock, a proper request
    // must be made to have it running.
    // Currently this clock is started in 'SystemInit' (see "system_nrf51.c"
    // or "system_nrf52.c", respectively).

    nrf_rtc_prescaler_set(COMMON_RTC_INSTANCE, 0);

    nrf_rtc_event_clear(COMMON_RTC_INSTANCE, US_TICKER_EVENT);
#if defined(TARGET_MCU_NRF51822)
    nrf_rtc_event_clear(COMMON_RTC_INSTANCE, OS_TICK_EVENT);
#endif
#if DEVICE_LOWPOWERTIMER
    nrf_rtc_event_clear(COMMON_RTC_INSTANCE, LP_TICKER_EVENT);
#endif
    nrf_rtc_event_clear(COMMON_RTC_INSTANCE, NRF_RTC_EVENT_OVERFLOW);

    // Interrupts on all related events are enabled permanently. Particular
    // events will be enabled or disabled as needed (such approach is more
    // energy efficient).
    nrf_rtc_int_enable(COMMON_RTC_INSTANCE,
    #if defined(TARGET_MCU_NRF51822)
        OS_TICK_INT_MASK |
    #endif
    #if DEVICE_LOWPOWERTIMER
        LP_TICKER_INT_MASK |
    #endif
        US_TICKER_INT_MASK |
        NRF_RTC_INT_OVERFLOW_MASK);

    // This event is enabled permanently, since overflow indications are needed
    // continuously.
    nrf_rtc_event_enable(COMMON_RTC_INSTANCE, NRF_RTC_INT_OVERFLOW_MASK);
    // All other relevant events are initially disabled.
    nrf_rtc_event_disable(COMMON_RTC_INSTANCE,
    #if defined(TARGET_MCU_NRF51822)
        OS_TICK_INT_MASK |
    #endif
    #if DEVICE_LOWPOWERTIMER
        LP_TICKER_INT_MASK |
    #endif
        US_TICKER_INT_MASK);

    nrf_drv_common_irq_enable(nrf_drv_get_IRQn(COMMON_RTC_INSTANCE),
        APP_IRQ_PRIORITY_LOW);

    nrf_rtc_task_trigger(COMMON_RTC_INSTANCE, NRF_RTC_TASK_START);

    m_common_rtc_enabled = true;
}

uint32_t common_rtc_32bit_ticks_get(void)
{
    uint32_t ticks = nrf_rtc_counter_get(COMMON_RTC_INSTANCE);
    // The counter used for time measurements is less than 32 bit wide,
    // so its value is complemented with the number of registered overflows
    // of the counter.
    ticks += (m_common_rtc_overflows << RTC_COUNTER_BITS);
    return ticks;
}
//------------------------------------------------------------------------------


void us_ticker_init(void)
{
    common_rtc_init();
}

static uint64_t us_ticker_64bit_get(void)
{
    uint32_t ticks = common_rtc_32bit_ticks_get();
    // [ticks -> microseconds]
    return ROUNDED_DIV(((uint64_t)ticks) * 1000000, RTC_INPUT_FREQ);
}

uint32_t us_ticker_read()
{
    return (uint32_t)us_ticker_64bit_get();
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    // The internal counter is clocked with a frequency that cannot be easily
    // multiplied to 1 MHz, therefore besides the translation of values
    // (microsecond <-> ticks) a special care of overflows handling must be
    // taken. Here the 32-bit timestamp value is complemented with information
    // about current the system up time of (ticks + number of overflows of tick
    // counter on upper bits, converted to microseconds), and such 64-bit value
    // is then translated to counter ticks. Finally, the lower 24 bits of thus
    // calculated value is written to the counter compare register to prepare
    // the interrupt generation.
    uint64_t current_time64 = us_ticker_64bit_get();
    // [add upper 32 bits from the current time to the timestamp value]
    uint64_t timestamp64 = timestamp + (current_time64 & ~(uint64_t)0xFFFFFFFF);
    // [if the original timestamp value happens to be after the 32 bit counter
    //  of microsends overflows, correct the upper 32 bits accordingly]
    if (timestamp < (uint32_t)(current_time64 & 0xFFFFFFFF)) {
        timestamp64 += ((uint64_t)1 << 32);
    }
    // [microseconds -> ticks, always round the result up to avoid too early
    //  interrupt generation]
    uint32_t compare_value =
        (uint32_t)CEIL_DIV((timestamp64) * RTC_INPUT_FREQ, 1000000);

    // The COMPARE event occurs when the value in compare register is N and
    // the counter value changes from N-1 to N. Therefore, the minimal safe
    // difference between the compare value to be set and the current counter
    // value is 2 ticks. This guarantees that the compare trigger is properly
    // setup before the compare condition occurs.
    uint32_t closest_safe_compare = common_rtc_32bit_ticks_get() + 2;
    if ((int)(compare_value - closest_safe_compare) <= 0) {
        compare_value = closest_safe_compare;
    }

    nrf_rtc_cc_set(COMMON_RTC_INSTANCE, US_TICKER_CC_CHANNEL,
        RTC_WRAP(compare_value));
    nrf_rtc_event_enable(COMMON_RTC_INSTANCE, US_TICKER_INT_MASK);
}

void us_ticker_disable_interrupt(void)
{
    nrf_rtc_event_disable(COMMON_RTC_INSTANCE, US_TICKER_INT_MASK);
}

void us_ticker_clear_interrupt(void)
{
    // No implementation needed. The event that triggers the interrupt is
    // cleared in 'common_rtc_irq_handler'.
}
