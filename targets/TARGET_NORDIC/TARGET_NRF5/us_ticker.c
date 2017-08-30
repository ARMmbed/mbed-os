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
#include "nrf_drv_common.h"
#include "lp_ticker_api.h"
#include "mbed_critical.h"

#if defined(NRF52_ERRATA_20)
    #include "softdevice_handler.h"
#endif

//------------------------------------------------------------------------------
// Common stuff used also by lp_ticker and rtc_api (see "common_rtc.h").
//
#include "app_util_platform.h"

bool              m_common_rtc_enabled = false;
uint32_t volatile m_common_rtc_overflows = 0;

__STATIC_INLINE void rtc_ovf_event_check(void)
{
    if (nrf_rtc_event_pending(COMMON_RTC_INSTANCE, NRF_RTC_EVENT_OVERFLOW)) {
        nrf_rtc_event_clear(COMMON_RTC_INSTANCE, NRF_RTC_EVENT_OVERFLOW);
        // Don't disable this event. It shall occur periodically.

        ++m_common_rtc_overflows;
    }
}

#if defined(TARGET_MCU_NRF51822)
void common_rtc_irq_handler(void)
#else
void COMMON_RTC_IRQ_HANDLER(void)
#endif
{

    rtc_ovf_event_check();

    if (nrf_rtc_event_pending(COMMON_RTC_INSTANCE, US_TICKER_EVENT)) {
        us_ticker_irq_handler();
    }

#if DEVICE_LOWPOWERTIMER
    if (nrf_rtc_event_pending(COMMON_RTC_INSTANCE, LP_TICKER_EVENT)) {

        lp_ticker_irq_handler();
    }
#endif
}

// Function for fix errata 20: RTC Register values are invalid
__STATIC_INLINE void errata_20(void)
{
#if defined(NRF52_ERRATA_20)
    if (!softdevice_handler_is_enabled())
    {
        NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
        NRF_CLOCK->TASKS_LFCLKSTART    = 1;

        while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
        {
        }
    }
    NRF_RTC1->TASKS_STOP = 0;
#endif
}

void RTC1_IRQHandler(void);

void common_rtc_init(void)
{
    if (m_common_rtc_enabled) {
        return;
    }

    errata_20();

    NVIC_SetVector(RTC1_IRQn, (uint32_t)RTC1_IRQHandler);
    
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
#ifdef NRF51
        APP_IRQ_PRIORITY_LOW
#elif defined(NRF52) || defined(NRF52840_XXAA)
        APP_IRQ_PRIORITY_LOWEST
#endif
        );

    nrf_rtc_task_trigger(COMMON_RTC_INSTANCE, NRF_RTC_TASK_START);

    m_common_rtc_enabled = true;
}

__STATIC_INLINE void rtc_ovf_event_safe_check(void)
{
    core_util_critical_section_enter();

    rtc_ovf_event_check();

    core_util_critical_section_exit();
}


uint32_t common_rtc_32bit_ticks_get(void)
{
    uint32_t ticks;
    uint32_t prev_overflows;

    do {
        prev_overflows = m_common_rtc_overflows;

        ticks = nrf_rtc_counter_get(COMMON_RTC_INSTANCE);
        // The counter used for time measurements is less than 32 bit wide,
        // so its value is complemented with the number of registered overflows
        // of the counter.
        ticks += (m_common_rtc_overflows << RTC_COUNTER_BITS);

        // Check in case that OVF occurred during execution of a RTC handler (apply if call was from RTC handler)
        // m_common_rtc_overflows might been updated in this call.
        rtc_ovf_event_safe_check();

        // If call was made from a low priority level m_common_rtc_overflows might have been updated in RTC handler.
    } while (m_common_rtc_overflows != prev_overflows);

    return ticks;
}

uint64_t common_rtc_64bit_us_get(void)
{
    uint32_t ticks = common_rtc_32bit_ticks_get();
    // [ticks -> microseconds]
    return ROUNDED_DIV(((uint64_t)ticks) * 1000000, RTC_INPUT_FREQ);
}

void common_rtc_set_interrupt(uint32_t us_timestamp, uint32_t cc_channel,
                              uint32_t int_mask)
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
    uint64_t current_time64 = common_rtc_64bit_us_get();
    // [add upper 32 bits from the current time to the timestamp value]
    uint64_t timestamp64 = us_timestamp +
        (current_time64 & ~(uint64_t)0xFFFFFFFF);
    // [if the original timestamp value happens to be after the 32 bit counter
    //  of microsends overflows, correct the upper 32 bits accordingly]
    if (us_timestamp < (uint32_t)(current_time64 & 0xFFFFFFFF)) {
        timestamp64 += ((uint64_t)1 << 32);
    }
    // [microseconds -> ticks, always round the result up to avoid too early
    //  interrupt generation]
    uint32_t compare_value =
        (uint32_t)CEIL_DIV((timestamp64) * RTC_INPUT_FREQ, 1000000);


    core_util_critical_section_enter();
    // The COMPARE event occurs when the value in compare register is N and
    // the counter value changes from N-1 to N. Therefore, the minimal safe
    // difference between the compare value to be set and the current counter
    // value is 2 ticks. This guarantees that the compare trigger is properly
    // setup before the compare condition occurs.
    uint32_t closest_safe_compare = common_rtc_32bit_ticks_get() + 2;
    if ((int)(compare_value - closest_safe_compare) <= 0) {
        compare_value = closest_safe_compare;
    }

    nrf_rtc_cc_set(COMMON_RTC_INSTANCE, cc_channel, RTC_WRAP(compare_value));
    nrf_rtc_event_enable(COMMON_RTC_INSTANCE, int_mask);

    core_util_critical_section_exit();
}
//------------------------------------------------------------------------------


void us_ticker_init(void)
{
    common_rtc_init();
}

uint32_t us_ticker_read()
{
    us_ticker_init();
    return (uint32_t)common_rtc_64bit_us_get();
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    common_rtc_set_interrupt(timestamp,
        US_TICKER_CC_CHANNEL, US_TICKER_INT_MASK);
}

void us_ticker_fire_interrupt(void)
{
    uint32_t closest_safe_compare = common_rtc_32bit_ticks_get() + 2;

    nrf_rtc_cc_set(COMMON_RTC_INSTANCE, US_TICKER_CC_CHANNEL, RTC_WRAP(closest_safe_compare));
    nrf_rtc_event_enable(COMMON_RTC_INSTANCE, US_TICKER_INT_MASK);
}

void us_ticker_disable_interrupt(void)
{
    nrf_rtc_event_disable(COMMON_RTC_INSTANCE, US_TICKER_INT_MASK);
}

void us_ticker_clear_interrupt(void)
{
    nrf_rtc_event_clear(COMMON_RTC_INSTANCE, US_TICKER_EVENT);
}


// Since there is no SysTick on NRF51, the RTC1 channel 1 is used as an
// alternative source of RTOS ticks.
#if defined(TARGET_MCU_NRF51822)

#include "mbed_toolchain.h"


#define MAX_RTC_COUNTER_VAL     ((1uL << RTC_COUNTER_BITS) - 1)

#ifndef RTC1_CONFIG_FREQUENCY
    #define RTC1_CONFIG_FREQUENCY    32678 // [Hz]
#endif



void COMMON_RTC_IRQ_HANDLER(void)
{
    if(!nrf_rtc_event_pending(COMMON_RTC_INSTANCE, OS_TICK_EVENT)) {
        common_rtc_irq_handler();
    }
}

IRQn_Type mbed_get_m0_tick_irqn()
{
    return SWI3_IRQn;
}


#endif // defined(TARGET_MCU_NRF51822)
