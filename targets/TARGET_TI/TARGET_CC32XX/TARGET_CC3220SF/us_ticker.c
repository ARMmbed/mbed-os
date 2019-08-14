/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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

#include "stdlib.h"
#include "us_ticker_api.h"
#include "ti/devices/cc32xx/inc/hw_types.h"
#include "ti/devices/cc32xx/inc/hw_memmap.h"
#include "ti/devices/cc32xx/driverlib/ti_timer.h"

/* There're 4 timers running from system clock (80MHz). Configure split 16b TimerA timer counting down. We'll post process it to make it appear count up.
 * When the timer is configured in periodic down count mode, GPTMTAPR register acts as a true prescaler. A scaler of 80 yields 1MHz timer.
 */
#define US_TICKER_COUNTER_BITS        16u
#define US_TICKER_FREQ                1000000

bool us_ticker_initialized = false;

const ticker_info_t *us_ticker_get_info()
{
    static const ticker_info_t info = {
        US_TICKER_FREQ,
        US_TICKER_COUNTER_BITS
    };
    return &info;
}
void us_ticker_init(void)
{
    if (!us_ticker_initialized) {
        TimerDisable(TIMERA0_BASE, TIMER_A);
        TimerConfigure(TIMERA0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
        TimerIntClear(TIMERA0_BASE, TIMER_TIMA_DMA | TIMER_TIMA_MATCH | TIMER_CAPA_EVENT |
                      TIMER_CAPA_MATCH | TIMER_TIMA_TIMEOUT);
        TimerPrescaleSet(TIMERA0_BASE, TIMER_A, (80 - 1));
        TimerEnable(TIMERA0_BASE, TIMER_A);
        NVIC_ClearPendingIRQ(INT_TIMERA0A_IRQn);
        NVIC_SetVector(INT_TIMERA0A_IRQn, (uint32_t)us_ticker_irq_handler);
        NVIC_EnableIRQ(INT_TIMERA0A_IRQn);
        us_ticker_initialized = true;
    } else {
        // Disable match interrupt. This is mbed OS requirement.
        TimerIntDisable(TIMERA0_BASE, TIMER_TIMA_MATCH);
        // Clear pending interrupt
        TimerIntClear(TIMERA0_BASE, TIMER_TIMA_MATCH | TIMER_TIMA_TIMEOUT);
    }
}

uint32_t us_ticker_read(void)
{
    return (0xFFFF & ~TimerValueGet(TIMERA0_BASE, TIMER_A));
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    // Clear pending interrupt
    TimerIntClear(TIMERA0_BASE, TIMER_TIMA_MATCH);
    TimerMatchSet(TIMERA0_BASE, TIMER_A, 0xFFFF & (~(timestamp & 0xFFFF)));
    TimerIntEnable(TIMERA0_BASE, TIMER_TIMA_MATCH);
}

void us_ticker_disable_interrupt(void)
{
    TimerIntDisable(TIMERA0_BASE, TIMER_TIMA_MATCH);
}

void us_ticker_clear_interrupt(void)
{
    TimerIntClear(TIMERA0_BASE, TIMER_TIMA_MATCH | TIMER_TIMA_TIMEOUT);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(INT_TIMERA0A_IRQn);
}

void us_ticker_free(void)
{
    if (us_ticker_initialized) {
        TimerDisable(TIMERA0_BASE, TIMER_A);
        NVIC_DisableIRQ(INT_TIMERA0A_IRQn);
        us_ticker_initialized = false;
    }
}
