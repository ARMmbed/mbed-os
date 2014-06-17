/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

int us_ticker_inited = 0;

void us_ticker_init(void) {
    if (us_ticker_inited) return;
    us_ticker_inited = 1;

    /* Enable clock for TIMERs */
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_TIMER0;
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_TIMER1;

    /* Clear TIMER counter values */
    TIMER0->CNT = 0;
    TIMER1->CNT = 0;

    /* Set TIMER0 prescaler */
//    TIMER0->CTRL = (TIMER0->CTRL & ~_TIMER_CTRL_PRESC_MASK) |  TIMER_CTRL_PRESC_DIV2;

    /* Enable overflow interrupt */
//    TIMER0->IEN |= TIMER_IF_OF;
//    TIMER1->IEN |= TIMER_IF_OF;

    /* Set TIMER0 to tick with HFPERCLK (14 MHz) */
    TIMER0->CTRL |= TIMER_CTRL_CLKSEL_PRESCHFPERCLK;
    /* Set TIMER1 to tick when TIMER0 overflows */
    TIMER1->CTRL |= TIMER_CTRL_CLKSEL_TIMEROUF;

    /* Enable TIMER0 interrupt vector in NVIC */
//    NVIC_SetVector(TIMER0_IRQn, (uint32_t)us_ticker_irq_handler);
//    NVIC_EnableIRQ(TIMER1_IRQn);

    /* Set top value */
    TIMER0->TOP = 0xFFFF;
    TIMER1->TOP = 0xFFFF;

    /* Start TIMERs */
    TIMER0->CMD = TIMER_CMD_START;
    TIMER1->CMD = TIMER_CMD_START;
}

uint32_t us_ticker_read() {
    if (!us_ticker_inited)
        us_ticker_init();
    
    uint32_t cnt = (((uint32_t)TIMER1->CNT << 16) | TIMER0->CNT);

    return (uint32_t)(cnt / 14);
}


void us_ticker_set_interrupt(unsigned int timestamp) {
    // set match value
//    US_TICKER_TIMER->CCR1 = timestamp;
    // enable compare interrupt
//    US_TICKER_TIMER->DIER |= TIM_DIER_CC1IE;
}

void us_ticker_disable_interrupt(void) {
    /* Disable overflow interrupt */
    TIMER0->IEN &= ~TIMER_IF_OF;
}

void us_ticker_clear_interrupt(void) {
    /* Clear overflow interrupt */
    TIMER0->IFC = TIMER_IF_OF;
}

