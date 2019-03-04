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
#include "gclk.h"
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

static float us_ticker_prescaler_correction = 1.0f;
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
    uint16_t            prescaler_value;

    if (us_ticker_inited) return;
    us_ticker_inited = 1;

    if (g_sys_init == 0) {
        system_init();
        g_sys_init = 1;
    }

    tc_get_config_defaults(&config_tc);

    cycles_per_us = system_gclk_gen_get_hz(config_tc.clock_source) / 1000000;
    MBED_ASSERT(cycles_per_us > 0);

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

    // on SAML21J18A the clock frequency is 48MHz, but there is no prescaler value that matches this
    // so we need to correct... This matches tc.h
    switch (prescaler) {
        case 0: prescaler_value = 1; break;
        case 1: prescaler_value = 2; break;
        case 2: prescaler_value = 4; break;
        case 3: prescaler_value = 8; break;
        case 4: prescaler_value = 16; break;
        case 5: prescaler_value = 64; break;
        case 6: prescaler_value = 256; break;
        case 7: prescaler_value = 1024; break;
    }

    us_ticker_prescaler_correction = ((float)system_gclk_gen_get_hz(config_tc.clock_source) / 1000000) / (float)prescaler_value;

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

    /* Enable the timer interrupt */
    tc_disable_callback(&us_ticker_module, TC_CALLBACK_CC_CHANNEL0);
    NVIC_SetVector(TICKER_COUNTER_IRQn, (uint32_t)TICKER_COUNTER_Handlr);
    NVIC_EnableIRQ(TICKER_COUNTER_IRQn);
}

uint32_t us_ticker_read()
{
    if (!us_ticker_inited)
        us_ticker_init();

    uint32_t tc_value = tc_get_count_value(&us_ticker_module);
    if (us_ticker_prescaler_correction != 1.0f) {
        tc_value = (uint32_t)((float)tc_value / us_ticker_prescaler_correction);
    }
    return tc_value;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t tc_timestamp = (uint32_t)timestamp;
    if (us_ticker_prescaler_correction != 1.0f) {
        tc_timestamp = (uint32_t)((float)tc_timestamp * us_ticker_prescaler_correction);
    }

    /* Enable the callback */
    tc_enable_callback(&us_ticker_module, TC_CALLBACK_CC_CHANNEL0);
    tc_set_compare_value(&us_ticker_module, TC_COMPARE_CAPTURE_CHANNEL_0, tc_timestamp);
}

void us_ticker_disable_interrupt(void)
{
    /* Disable the callback */
    tc_disable_callback(&us_ticker_module, TC_CALLBACK_CC_CHANNEL0);
}

void us_ticker_fire_interrupt(void)
{
    /**
     * We should be able to trigger this by just calling:
     *
     * NVIC_SetPendingIRQ(TICKER_COUNTER_IRQn);
     *
     * However, tc_interrupt.c -> _tc_interrupt_handler then gets the wrong value from reg.
     * It should be 48 (when called via the normal API), but is 32. I've tried setting
     * this register before triggering the interrupt, and this fails in the same way.
     * I have no clue how to fix this properly... If someone comes along who understands
     * Atmel IRQ handling better I'd love to get a better patch
     */

    // this gives us a minimum tick of 400 us.
    us_ticker_set_interrupt(us_ticker_read() + 400);
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

void us_ticker_free(void)
{
    tc_clear_interrupt(&us_ticker_module, TC_CALLBACK_CC_CHANNEL0);
    NVIC_DisableIRQ(TICKER_COUNTER_IRQn);

    us_ticker_inited = 0;
}
