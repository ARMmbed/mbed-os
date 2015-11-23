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
#include "compiler.h"
#include "sysclk.h"
#include "tc.h"

static uint8_t us_ticker_inited = 0;
extern uint8_t g_sys_init;

#define TICKER_COUNTER_CLK      ID_TC0
#define TICKER_COUNTER_uS       TC0
#define TICKER_COUNTER_CHANNEL  0
#define TICKER_COUNTER_IRQn     TC0_IRQn
#define TICKER_COUNTER_Handlr   TC0_Handler

void TICKER_COUNTER_Handlr(void)
{
    if ((tc_get_status(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL) & TC_IER_CPAS) == TC_IER_CPAS) {
        us_ticker_irq_handler();
    }
}

void us_ticker_init(void)
{
    if (us_ticker_inited) return;
    us_ticker_inited = 1;

    if (g_sys_init == 0) {
        sysclk_init();
        g_sys_init = 1;
    }

    /* Configure the PMC to enable the TC module. */
    sysclk_enable_peripheral_clock(TICKER_COUNTER_CLK);

#if SAMG55
    /* Enable PCK output */
    pmc_disable_pck(PMC_PCK_3);
    pmc_switch_pck_to_mck(PMC_PCK_3, PMC_PCK_PRES_CLK_1);
    pmc_enable_pck(PMC_PCK_3);
#endif

    /* Init TC to waveform mode. */
    tc_init(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL,
            TC_CMR_TCCLKS_TIMER_CLOCK4
           );

    tc_start(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL);
}

uint32_t us_ticker_read()
{
    if (!us_ticker_inited)
        us_ticker_init();

    return tc_read_cv(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL);
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

    tc_enable_interrupt(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL, TC_IER_CPAS);
    tc_write_ra(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL, (uint32_t)timestamp);

    NVIC_ClearPendingIRQ(TICKER_COUNTER_IRQn);
    NVIC_SetPriority(TICKER_COUNTER_IRQn, 0);
    NVIC_EnableIRQ(TICKER_COUNTER_IRQn);


}

void us_ticker_disable_interrupt(void)
{
    tc_stop(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL);
    tc_disable_interrupt(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL, TC_IER_CPAS);
    NVIC_DisableIRQ(TICKER_COUNTER_IRQn);
}

void us_ticker_clear_interrupt(void)
{
    NVIC_ClearPendingIRQ(TICKER_COUNTER_IRQn);
}