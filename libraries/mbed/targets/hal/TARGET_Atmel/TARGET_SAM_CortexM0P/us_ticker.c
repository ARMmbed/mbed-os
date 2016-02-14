/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include <stddef.h>
#include "us_ticker_api.h"
#include "cmsis.h"
#include "mbed_assert.h"
#include "ins_gclk.h"
#include "compiler.h"
#include "system.h"
#include "tc.h"
#include "tc_interrupt.h"

#if (SAMD21) || (SAMR21)
#define TICKER_COUNTER_uS		TC4
#define TICKER_COUNTER_IRQn		TC4_IRQn
#define TICKER_COUNTER_Handlr	TC4_Handler
#elif (SAML21) /*SAML21 TCC4 does not support 32 bit counter operations*/
#define TICKER_COUNTER_uS		TC0
#define TICKER_COUNTER_IRQn		TC0_IRQn
#define TICKER_COUNTER_Handlr	TC0_Handler
#endif

static int us_ticker_inited = 0;
extern uint8_t g_sys_init;

struct tc_module us_ticker_module;


static inline void tc_clear_interrupt(
    struct tc_module *const module,
    const enum tc_callback callback_type)
{
    /* Sanity check arguments */
    MBED_ASSERT(module);

    /* Clear interrupt flags */
    if (callback_type == TC_CALLBACK_CC_CHANNEL0) {
        module->hw->COUNT8.INTENCLR.reg = TC_INTFLAG_MC(1);
    } else if (callback_type == TC_CALLBACK_CC_CHANNEL1) {
        module->hw->COUNT8.INTENCLR.reg = TC_INTFLAG_MC(2);
    } else {
        module->hw->COUNT8.INTENCLR.reg = (1 << callback_type);
    }
}

void us_ticker_irq_handler_internal(struct tc_module* us_tc_module)
{
    uint32_t status_flags;

    /* Clear TC capture overflow and TC count overflow */
    status_flags = TC_STATUS_CAPTURE_OVERFLOW | TC_STATUS_COUNT_OVERFLOW;
    tc_clear_status(&us_ticker_module, status_flags);

    us_ticker_irq_handler();
}

void us_ticker_init(void)
{
    uint32_t			cycles_per_us;
    uint32_t			prescaler = 0;
    struct tc_config	config_tc;

    if (us_ticker_inited) return;
    us_ticker_inited = 1;

    if (g_sys_init == 0) {
        system_init();
        g_sys_init = 1;
    }

    tc_get_config_defaults(&config_tc);

    cycles_per_us = system_gclk_gen_get_hz(config_tc.clock_source) / 1000000;
    MBED_ASSERT(cycles_per_us > 0);
    /*while((cycles_per_us & 1) == 0 && prescaler <= 10) {
    	cycles_per_us = cycles_per_us >> 1;
    	prescaler++;
    }*/
    while((cycles_per_us > 1) && (prescaler <= 10)) {
        cycles_per_us = cycles_per_us >> 1;
        prescaler++;
    }
    if (prescaler >= 9) {
        prescaler = 7;
    } else if (prescaler >= 7) {
        prescaler = 6;
    } else if (prescaler >= 5) {
        prescaler = 5;
    }

    config_tc.clock_prescaler = (enum tc_clock_prescaler)TC_CTRLA_PRESCALER(prescaler);
    config_tc.counter_size = TC_COUNTER_SIZE_32BIT;
    config_tc.run_in_standby = true;
    config_tc.counter_32_bit.value = 0;
    config_tc.counter_32_bit.compare_capture_channel[TC_COMPARE_CAPTURE_CHANNEL_0] = 0xFFFFFFFF;

    /* Initialize the timer */
    tc_init(&us_ticker_module, TICKER_COUNTER_uS, &config_tc);

    /* Register callback function */
    tc_register_callback(&us_ticker_module, (tc_callback_t)us_ticker_irq_handler_internal, TC_CALLBACK_CC_CHANNEL0);

    /* Enable the timer module */
    tc_enable(&us_ticker_module);
}

uint32_t us_ticker_read()
{
    if (!us_ticker_inited)
        us_ticker_init();

    return tc_get_count_value(&us_ticker_module);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t cur_time;
    int32_t delta;

    cur_time = us_ticker_read();
    delta = (int32_t)((uint32_t)timestamp - cur_time);
    if (delta < 0) {
        /* Event already occurred in past */
        us_ticker_irq_handler();
        return;
    }

    NVIC_DisableIRQ(TICKER_COUNTER_IRQn);
    NVIC_SetVector(TICKER_COUNTER_IRQn, (uint32_t)TICKER_COUNTER_Handlr);

    /* Enable the callback */
    tc_enable_callback(&us_ticker_module, TC_CALLBACK_CC_CHANNEL0);
    tc_set_compare_value(&us_ticker_module, TC_COMPARE_CAPTURE_CHANNEL_0, (uint32_t)timestamp);

    NVIC_EnableIRQ(TICKER_COUNTER_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    /* Disable the callback */
    tc_disable_callback(&us_ticker_module, TC_CALLBACK_CC_CHANNEL0);
    NVIC_DisableIRQ(TICKER_COUNTER_IRQn);
}

void us_ticker_clear_interrupt(void)
{
    uint32_t status_flags;

    /* Clear TC channel 0 match */
    status_flags = TC_STATUS_CHANNEL_0_MATCH;
    tc_clear_status(&us_ticker_module, status_flags);

    /* Clear the interrupt */
    tc_clear_interrupt(&us_ticker_module, TC_CALLBACK_CC_CHANNEL0);
    NVIC_ClearPendingIRQ(TICKER_COUNTER_IRQn);
}
