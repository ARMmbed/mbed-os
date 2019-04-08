/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#include "lp_ticker_api.h"
#include "PeripheralNames.h"

#define LP_TICKER_INTERRUPT   CMSDK_TIMER0
#define LP_TICKER_COUNTER     CMSDK_TIMER1
#define LP_TICKER_TIMER_IRQn  TIMER0_IRQn


/*  mbed OS HAL API defined lp_ticker as an increment ticker
 *  MPS2 platform provided in SSE-200 are decrement tickers
 *  with interrupt fired counter reaches 0.
 *
 *  So 2 Timers are used to construct mbed OS HAL low power ticker.
 *
 *  TIMER0 is for generating interrupts
 *  and TIMER0 will turned off when it is generating interrupts
 *
 *  TIMER1 is for counting, and returns inverted binary when read from it
 *  Because TIMER1 is running at the speed of 25Mhz, it need to be shift by 10,
 *  in order to meet mbed HAL lp_ticker definitions
 *
 */

static int lp_ticker_inited = 0;

void lp_ticker_internal_handler(void)
{
    LP_TICKER_INTERRUPT->CTRL &= ~CMSDK_TIMER_CTRL_IRQEN_Msk;
    LP_TICKER_INTERRUPT->CTRL &= ~CMSDK_TIMER_CTRL_EN_Msk;
    lp_ticker_irq_handler();
}


void lp_ticker_init(void)
{
    if (lp_ticker_inited) {
        lp_ticker_disable_interrupt();
        return;
    }

    LP_TICKER_COUNTER->CTRL   = 0x0ul;
    LP_TICKER_INTERRUPT->CTRL = 0x0ul;

    LP_TICKER_COUNTER->RELOAD   = 0xFFFFFFFFul;
    LP_TICKER_INTERRUPT->RELOAD = 0xFFFFFFFFul;

    LP_TICKER_COUNTER->CTRL |= CMSDK_TIMER_CTRL_EN_Msk;

    NVIC_SetVector(LP_TICKER_TIMER_IRQn, (uint32_t)lp_ticker_internal_handler);
    lp_ticker_inited = 1;
}

void lp_ticker_free(void)
{
    LP_TICKER_COUNTER->CTRL &= ~CMSDK_TIMER_CTRL_EN_Msk;
    lp_ticker_disable_interrupt();
    lp_ticker_inited = 0;
}

uint32_t lp_ticker_read()
{
    return (~LP_TICKER_COUNTER->VALUE) >> 10;
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t delta = (timestamp << 10) - (~LP_TICKER_COUNTER->VALUE);

    LP_TICKER_INTERRUPT->CTRL &= ~CMSDK_TIMER_CTRL_EN_Msk;
    LP_TICKER_INTERRUPT->RELOAD = delta;
    LP_TICKER_INTERRUPT->CTRL |= CMSDK_TIMER_CTRL_IRQEN_Msk;
    LP_TICKER_INTERRUPT->CTRL |= CMSDK_TIMER_CTRL_EN_Msk;

    NVIC_EnableIRQ(LP_TICKER_TIMER_IRQn);
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_EnableIRQ(LP_TICKER_TIMER_IRQn);
    NVIC_SetPendingIRQ(LP_TICKER_TIMER_IRQn);
}

void lp_ticker_disable_interrupt(void)
{
    LP_TICKER_INTERRUPT->CTRL &= ~CMSDK_TIMER_CTRL_IRQEN_Msk;
    LP_TICKER_INTERRUPT->CTRL &= ~CMSDK_TIMER_CTRL_EN_Msk;
    NVIC_DisableIRQ(LP_TICKER_TIMER_IRQn);
}

void lp_ticker_clear_interrupt(void)
{
    LP_TICKER_INTERRUPT->INTCLEAR = CMSDK_TIMER_INTCLEAR_Msk;
}

const ticker_info_t *lp_ticker_get_info(void)
{
    static const ticker_info_t info = {
        24414, // 10 stages scaled from 25MHz (dived by 1024)
        22     // 22 bit counter
    };
    return &info;
}


