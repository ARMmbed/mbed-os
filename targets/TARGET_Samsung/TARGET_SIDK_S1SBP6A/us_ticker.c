/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stddef.h>
#include "us_ticker_api.h"
#include "PeripheralNames.h"
#include "cmsis.h"

#define US_TICKER_COUNTER     BP_DUAL_TIMER0
#define US_TICKER_INTERRUPT   BP_DUAL_TIMER1
#define US_TICKER_TIMER_IRQn  DUAL_TIMER_IRQn

static int us_ticker_inited = 0;

void __us_ticker_irq_handler(void *arg)
{
    UNUSED(arg);
    us_ticker_disable_interrupt();
    us_ticker_clear_interrupt();
    us_ticker_irq_handler();
}

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        us_ticker_disable_interrupt();
        return;
    }

    if (!bp6a_cmu_get_clock_enabled(CMU_DUALTIMER_APBCLK)) {
        bp6a_cmu_enable_clock(CMU_DUALTIMER_APBCLK, true);
    }

    US_TICKER_COUNTER->CTRL    = 0x0ul; // disable TIMER1 and reset all control
    US_TICKER_INTERRUPT->CTRL  = 0x0ul; // disable TIMER2 and reset all control

    US_TICKER_COUNTER->LOAD = 0xFFFFFFFFul;
    US_TICKER_INTERRUPT->LOAD = 0xFFFFFFFFul;
    US_TICKER_INTERRUPT->BGLOAD = 0xFFFFFFFFul;
    US_TICKER_COUNTER->CTRL |= DUALTIMER_CTRL_SIZE(1); // set TIMER1 to 32 bit counter
    US_TICKER_INTERRUPT->CTRL |= DUALTIMER_CTRL_SIZE(1); // set TIMER2 to 32 bit counter

    US_TICKER_COUNTER->CTRL |= DUALTIMER_CTRL_PRESCALE(1); // set TIMER1 with 1 stages prescale
    US_TICKER_INTERRUPT->CTRL |= DUALTIMER_CTRL_PRESCALE(1); // set TIMER2 with 1 stages prescale

    US_TICKER_INTERRUPT->CTRL |= DUALTIMER_CTRL_MODE(1);     // set TIMER2 periodic mode
    US_TICKER_INTERRUPT->CTRL |= DUALTIMER_CTRL_ONESHOOT(1); // set TIMER2 one-shot mode

    US_TICKER_COUNTER->CTRL |= DUALTIMER_CTRL_EN(1); // enable TIMER1 counter

    NVIC_SetVector(US_TICKER_TIMER_IRQn, (uint32_t)__us_ticker_irq_handler);
    us_ticker_inited = 1;
}

void us_ticker_free(void)
{
    US_TICKER_COUNTER->CTRL    &= ~DUALTIMER_CTRL_EN_MASK; // disable TIMER1
    US_TICKER_INTERRUPT->CTRL  &= ~DUALTIMER_CTRL_EN_MASK; // disable TIMER2
    us_ticker_disable_interrupt();
    us_ticker_inited = 0;
}

uint32_t us_ticker_read()
{
    return  ~US_TICKER_COUNTER->VALUE;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t delta =  timestamp - us_ticker_read();

    US_TICKER_INTERRUPT->CTRL &= ~DUALTIMER_CTRL_EN_MASK;   // disable TIMER2
    US_TICKER_INTERRUPT->LOAD = delta;                      // Set TIMER2 load value
    US_TICKER_INTERRUPT->CTRL |= DUALTIMER_CTRL_INTEN_MASK; // enable TIMER2 interrupt
    US_TICKER_INTERRUPT->CTRL |= DUALTIMER_CTRL_EN_MASK;    // enable TIMER2 counter
    NVIC_EnableIRQ(US_TICKER_TIMER_IRQn);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_EnableIRQ(US_TICKER_TIMER_IRQn);
    NVIC_SetPendingIRQ(US_TICKER_TIMER_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    US_TICKER_INTERRUPT->CTRL &= ~DUALTIMER_CTRL_INTEN_MASK;
    US_TICKER_INTERRUPT->CTRL &= ~DUALTIMER_CTRL_EN_MASK;  // disable TIMER2
    NVIC_DisableIRQ(US_TICKER_TIMER_IRQn);
}

void us_ticker_clear_interrupt(void)
{
    US_TICKER_INTERRUPT->INTCLR = DUALTIMER_INTCLR_MASK;
}

const ticker_info_t *us_ticker_get_info(void)
{
    uint32_t timer_clk = bp6a_get_clock_src_freq(CMU_DUALTIMER_APBCLK);

    static ticker_info_t info;
    info.frequency = timer_clk / 16;
    info.bits = 32;

    return &info;
}
