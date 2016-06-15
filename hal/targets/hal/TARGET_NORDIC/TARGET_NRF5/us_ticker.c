/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "us_ticker_api.h"
#include "rtc_common.h"
#include "app_util.h"


//------------------------------------------------------------------------------
// Common stuff used also by lp_ticker and rtc_api (see "rtc_common.h").
//
#include "nrf_drv_clock.h"
#include "app_util_platform.h"

nrf_drv_rtc_t const m_rtc_common = NRF_DRV_RTC_INSTANCE(1);
bool                m_rtc_common_enabled = false;
uint32_t volatile   m_rtc_common_overflows = 0;

static void irq_handler(nrf_drv_rtc_int_type_t int_type)
{
    if (int_type == (NRF_DRV_RTC_INT_COMPARE0 + US_TICKER_CC_CHANNEL)) {
        us_ticker_irq_handler();
    }
    else if (int_type == NRF_DRV_RTC_INT_OVERFLOW) {
        ++m_rtc_common_overflows;
    }
}

void rtc_common_init(void)
{
    if (m_rtc_common_enabled) {
        return;
    }

    (void)nrf_drv_clock_init();
    // RTC is driven by the low frequency (32.768 kHz) clock, a proper request
    // must be made to have it running.
    nrf_drv_clock_lfclk_request(NULL);

    nrf_drv_rtc_config_t const config = {
        .prescaler          = 0, // no prescaling
        .interrupt_priority = APP_IRQ_PRIORITY_LOW,
        .reliable           = false
    };
    if (nrf_drv_rtc_init(&m_rtc_common, &config, irq_handler) != NRF_SUCCESS) {
        MBED_ASSERT(false); // initialization failed
        return;
    }
    nrf_drv_rtc_overflow_enable(&m_rtc_common, true);

    nrf_drv_rtc_enable(&m_rtc_common);
    m_rtc_common_enabled = true;
}

uint32_t rtc_common_32bit_ticks_get(void)
{
    uint32_t ticks = nrf_drv_rtc_counter_get(&m_rtc_common);
    // The counter used for time measurements is less than 32 bit wide,
    // so its value is complemented with the number of registered overflows
    // of the counter.
    ticks += (m_rtc_common_overflows << RTC_COUNTER_BITS);
    return ticks;
}
//------------------------------------------------------------------------------


void us_ticker_init(void)
{
    rtc_common_init();
}

static uint64_t us_ticker_64bit_get(void)
{
    uint32_t ticks = rtc_common_32bit_ticks_get();
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
    uint32_t closest_safe_compare = rtc_common_32bit_ticks_get() + 2;
    if ((int)(compare_value - closest_safe_compare) <= 0) {
        compare_value = closest_safe_compare;
    }
    ret_code_t result = nrf_drv_rtc_cc_set(&m_rtc_common, US_TICKER_CC_CHANNEL,
        compare_value, true);
    if (result != NRF_SUCCESS) {
        MBED_ASSERT(false);
    }
}

void us_ticker_disable_interrupt(void)
{
    nrf_drv_rtc_cc_disable(&m_rtc_common, US_TICKER_CC_CHANNEL);
}

void us_ticker_clear_interrupt(void)
{
    // No implementation needed. Interrupt flags are cleared by IRQ handler
    // in 'nrf_drv_rtc'.
}
