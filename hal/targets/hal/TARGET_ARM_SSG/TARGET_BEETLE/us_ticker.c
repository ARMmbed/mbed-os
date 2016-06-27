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
#include <stddef.h>
#include "cmsis.h"
#include "us_ticker_api.h"
#include "PeripheralNames.h"
/* Private data */
/* us_ticker reload value */
static uint32_t us_ticker_reload = 0x0; /* Max Value */
/* us ticker initialized */
static uint32_t us_ticker_inited = 0;
/* us ticker overflow */
static uint32_t us_ticker_overflow = 0;

void __us_ticker_irq_handler(void) {
    Timer_ClearInterrupt(TIMER1);
    us_ticker_overflow++;
}

void us_ticker_init(void) {
    uint32_t us_ticker_irqn0 = 0;
    uint32_t us_ticker_irqn1 = 0;

    if (us_ticker_inited)
        return;
    us_ticker_inited = 1;

    /* Initialize Timer 0 */
    Timer_Initialize(TIMER0, us_ticker_reload);
    /* Enable Timer 0 */
    Timer_Enable(TIMER0);

    /* Initialize Timer 1 */
    Timer_Initialize(TIMER1, us_ticker_reload);
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
}

uint32_t us_ticker_read() {
    uint32_t return_value = 0;

    if (!us_ticker_inited)
        us_ticker_init();
    return_value = Timer_Read(TIMER1);
    return return_value;
}

void us_ticker_set_interrupt(timestamp_t timestamp) {
    int32_t delta = 0;

    if (!us_ticker_inited)
        us_ticker_init();
    delta = (int32_t)(timestamp - us_ticker_read());
    /* Check if the event was in the past */
    if (delta <= 0) {
        /* This event was in the past */
        Timer_SetInterrupt(TIMER0, 0);
        return;
    }

    /* If the event was not in the past enable interrupt */
    Timer_SetInterrupt(TIMER0, delta);
}

void us_ticker_disable_interrupt(void) {
    Timer_DisableInterrupt(TIMER0);
}

void us_ticker_clear_interrupt(void) {
    Timer_ClearInterrupt(TIMER0);
}
