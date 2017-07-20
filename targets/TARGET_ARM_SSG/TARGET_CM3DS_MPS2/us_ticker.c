/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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

/* This file is derivative of us_ticker.c from BEETLE */

#include <stddef.h>
#include "cmsis.h"
#include "us_ticker_api.h"
#include "PeripheralNames.h"

#define TIMER_MAX_VALUE  0

/* Private data */
struct us_ticker_drv_data_t {
    uint32_t inited;          /* us ticker initialized */
    uint32_t overflow_delta;  /* us ticker overflow */
    uint32_t overflow_limit;  /* us ticker overflow limit */
};

static struct us_ticker_drv_data_t us_ticker_drv_data = {
    .inited = 0,
    .overflow_delta = 0,
    .overflow_limit = 0
};


void __us_ticker_irq_handler(void)
{
    Timer_ClearInterrupt(TIMER1);
    /*
     * For each overflow event adds the timer max represented value to
     * the delta. This allows the us_ticker to keep track of the elapsed
     * time:
     * elapsed_time = (num_overflow * overflow_limit) + current_time
     */
    us_ticker_drv_data.overflow_delta += us_ticker_drv_data.overflow_limit;
}

void us_ticker_init(void)
{
    uint32_t us_ticker_irqn0 = 0;
    uint32_t us_ticker_irqn1 = 0;

    if (us_ticker_drv_data.inited) {
        return;
    }

    us_ticker_drv_data.inited = 1;

    /* Initialize Timer 0 */
    Timer_Initialize(TIMER0, TIMER_MAX_VALUE);
    /* Enable Timer 0 */
    Timer_Enable(TIMER0);

    /* Initialize Timer 1 */
    Timer_Initialize(TIMER1, TIMER_MAX_VALUE);
    /* Enable Timer 1 */
    Timer_Enable(TIMER1);

    /* Timer 0 get IRQn */
    us_ticker_irqn0 = Timer_GetIRQn(TIMER0);
    NVIC_SetVector((IRQn_Type)us_ticker_irqn0, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ((IRQn_Type)us_ticker_irqn0);

    /* Timer 1 get IRQn */
    us_ticker_irqn1 = Timer_GetIRQn(TIMER1);
    NVIC_SetVector((IRQn_Type)us_ticker_irqn1, (uint32_t)__us_ticker_irq_handler);
    NVIC_EnableIRQ((IRQn_Type)us_ticker_irqn1);

    /* Timer set interrupt on TIMER1 */
    Timer_SetInterrupt(TIMER1, TIMER_DEFAULT_RELOAD);

    /*
     * Set us_ticker Overflow limit. The us_ticker overflow limit is required
     * to calculated the return value of the us_ticker read function in us
     * on 32bit.
     * A 32bit us value cannot be represented directly in the Timer Load
     * register if it is greater than (0xFFFFFFFF ticks)/TIMER_DIVIDER_US.
     */
    us_ticker_drv_data.overflow_limit = Timer_GetReloadValue(TIMER1);
}

uint32_t us_ticker_read()
{
    uint32_t return_value = 0;

    if (!us_ticker_drv_data.inited) {
        us_ticker_init();
    }

    return_value = us_ticker_drv_data.overflow_delta + Timer_Read(TIMER1);

    return return_value;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t delta = 0;

    if (!us_ticker_drv_data.inited) {
        us_ticker_init();
    }

    delta = timestamp - us_ticker_read();

    /* If the event was not in the past enable interrupt */
    Timer_SetInterrupt(TIMER0, delta);

}

void us_ticker_fire_interrupt(void)
{
    uint32_t us_ticker_irqn1 = Timer_GetIRQn(TIMER1);
    NVIC_SetPendingIRQ((IRQn_Type)us_ticker_irqn1);
}

void us_ticker_disable_interrupt(void)
{
    Timer_DisableInterrupt(TIMER0);
}

void us_ticker_clear_interrupt(void)
{
    Timer_ClearInterrupt(TIMER0);
}
