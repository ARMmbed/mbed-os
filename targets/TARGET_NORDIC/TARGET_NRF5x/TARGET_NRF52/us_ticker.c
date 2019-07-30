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

#include "us_ticker.h"

#include "us_ticker_api.h"
#include "nrf_timer.h"
#include "app_util_platform.h"
#include "nrfx_common.h"
#include "mbed_critical.h"

bool us_ticker_initialized = false;

/* us ticker is driven by 1MHz clock and counter length is 32 bits for nRF52 */
const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        US_TICKER_FREQ,
        US_TICKER_COUNTER_BITS
    };
    return &info;
}

void us_ticker_init(void)
{
    if (us_ticker_initialized) {
        nrf_timer_event_clear(NRF_TIMER1, NRF_TIMER_EVENT_COMPARE0);
        nrf_timer_int_disable(NRF_TIMER1, nrf_timer_compare_int_get(NRF_TIMER_CC_CHANNEL0));
        return;
    }

    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_STOP);

    nrf_timer_int_disable(NRF_TIMER1, nrf_timer_compare_int_get(NRF_TIMER_CC_CHANNEL0));

    /* Configure timer as follows:
     * - timer mode,
     * - timer width 16 bits for NRF51 and 32 bits for NRF52,
     * - timer freq 1 MHz.
     */
    nrf_timer_mode_set(NRF_TIMER1, NRF_TIMER_MODE_TIMER);

    nrf_timer_frequency_set(NRF_TIMER1, NRF_TIMER_FREQ_1MHz);

#ifdef TARGET_NRF52
    nrf_timer_bit_width_set(NRF_TIMER1, NRF_TIMER_BIT_WIDTH_32);
#else
    nrf_timer_bit_width_set(NRF_TIMER1, NRF_TIMER_BIT_WIDTH_16);
#endif

    nrf_timer_cc_write(NRF_TIMER1, NRF_TIMER_CC_CHANNEL0, 0);

    nrf_timer_event_clear(NRF_TIMER1, NRF_TIMER_EVENT_COMPARE0);

    NVIC_SetVector(TIMER1_IRQn, (uint32_t)us_ticker_irq_handler);

    NRFX_IRQ_PRIORITY_SET(TIMER1_IRQn, APP_IRQ_PRIORITY_HIGH);
    NRFX_IRQ_ENABLE(TIMER1_IRQn);

    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_START);

    /* Bug fix. First value can't be trusted. */
    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CAPTURE1);

    us_ticker_initialized = true;
}

uint32_t us_ticker_read()
{
    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CAPTURE1);

    return nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL1);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    core_util_critical_section_enter();

    const uint32_t counter_mask = ((1ULL << US_TICKER_COUNTER_BITS) - 1);

    nrf_timer_cc_write(NRF_TIMER1, NRF_TIMER_CC_CHANNEL0, timestamp & counter_mask);

    if (!nrf_timer_int_enable_check(NRF_TIMER1, nrf_timer_compare_int_get(NRF_TIMER_CC_CHANNEL0))) {
        nrf_timer_event_clear(NRF_TIMER1, NRF_TIMER_EVENT_COMPARE0);
        nrf_timer_int_enable(NRF_TIMER1, nrf_timer_compare_int_get(NRF_TIMER_CC_CHANNEL0));
    }

    core_util_critical_section_exit();
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(TIMER1_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    nrf_timer_int_disable(NRF_TIMER1, nrf_timer_compare_int_get(NRF_TIMER_CC_CHANNEL0));
}

void us_ticker_clear_interrupt(void)
{
    nrf_timer_event_clear(NRF_TIMER1, NRF_TIMER_EVENT_COMPARE0);
}

void us_ticker_free(void)
{
    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_STOP);
    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_SHUTDOWN);
    nrf_timer_int_disable(NRF_TIMER1, nrf_timer_compare_int_get(NRF_TIMER_CC_CHANNEL0));
    NVIC_DisableIRQ(TIMER1_IRQn);
    us_ticker_initialized = false;
}
