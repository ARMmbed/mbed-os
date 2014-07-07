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
#include "em_cmu.h"
#include "em_timer.h"
#include "device_peripherals.h"

/**
 * Timer functions for microsecond ticker.
 * mbed expects a 32-bit timer. Since the EFM32 only has 16-bit timers, we chain two of them.
 */

static int us_ticker_inited = 0;	// Is ticker initialized yet
static int lowerOnly = 0;			// Status bit for last interrupt on lower timer
static int extraLower = 0;			// Status bit for interrupts on lower timer
static uint32_t tsL = 0;			// Lower 16 bits of timestamp
static uint32_t tsH = 0;			// Upper 16 bits of timestamp

void us_ticker_irq_handler_internal(void) {
    if(lowerOnly) {
        lowerOnly = 0;
        /* Clear interrupt */
        TIMER_IntClear(US_TIMER_L, TIMER_IF_CC0);
        /* Call mbed common IRQ handler */
        us_ticker_irq_handler();
    } else {
        if(extraLower) { /* First time. Fallthrough and use pre-set lower timer */
            extraLower = 0;
        } else { /* Second time. Change lower timer. */
            lowerOnly = 1;
            TIMER_CompareSet(US_TIMER_L, 0, tsL);
        }
        /* Clear interrupt */
        TIMER_IntClear(US_TIMER_L, TIMER_IF_CC0);
        TIMER_IntClear(US_TIMER_H, TIMER_IF_CC0);
        /* Enable lower timer Compare Channel interrupt */
        TIMER_IntEnable(US_TIMER_L, TIMER_IF_CC0);
    }
}

void us_ticker_init(void) {
    if (us_ticker_inited) return;
    us_ticker_inited = 1;

    /* Enable clock for TIMERs */
    CMU_ClockEnable(US_TIMER_H_CLOCK, true);
    CMU_ClockEnable(US_TIMER_L_CLOCK, true);

    /* Clear TIMER counter values */
    TIMER_CounterSet(US_TIMER_L, 0);
    TIMER_CounterSet(US_TIMER_H, 0);

    /* Set lower timer prescaler */
    US_TIMER_L->CTRL = (US_TIMER_L->CTRL & ~_TIMER_CTRL_PRESC_MASK) |  TIMER_CTRL_PRESC_DIV2;

    /* Set lower timer to tick with HFPERCLK (14 MHz) */
    US_TIMER_L->CTRL |= TIMER_CTRL_CLKSEL_PRESCHFPERCLK;
    /* Set upper timer to tick when lower timer overflows */
    US_TIMER_H->CTRL |= TIMER_CTRL_CLKSEL_TIMEROUF;

    /* Select Compare Channel parameters */
    TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;
    timerCCInit.mode = timerCCModeCompare;

    /* Configure Compare Channel 0 */
    TIMER_InitCC(US_TIMER_L, 0, &timerCCInit);
    TIMER_InitCC(US_TIMER_H, 0, &timerCCInit);

    /* Enable interrupt vector in NVIC */
    NVIC_SetVector(US_TIMER_L_IRQn, (uint32_t)us_ticker_irq_handler_internal);
    NVIC_SetVector(US_TIMER_H_IRQn, (uint32_t)us_ticker_irq_handler_internal);
    NVIC_EnableIRQ(US_TIMER_L_IRQn);
    NVIC_EnableIRQ(US_TIMER_H_IRQn);

    /* Set top value */
    TIMER_TopSet(US_TIMER_L, 0xffff);
    TIMER_TopSet(US_TIMER_H, 0xffff);

    /* Start TIMERs */
    TIMER_Enable(US_TIMER_L, true);
    TIMER_Enable(US_TIMER_H, true);
}

uint32_t us_ticker_read() {
    uint32_t countH_old, countH, countL;

    if (!us_ticker_inited) {
        us_ticker_init();
    }

    /* Avoid jumping in time by reading high bits twice */
    do {
        countH_old = US_TIMER_H->CNT;
        countL = US_TIMER_L->CNT;
        countH = US_TIMER_H->CNT;
    } while(countH_old != countH);

    /* Divide by 7 to get 1 µs ticks on the 14 MHz clock with divider 2 */
    return ((countH << 16) | countL) / 7;
}


void us_ticker_set_interrupt(unsigned int timestamp) {
    /* Multiply by 7 to get clock ticks (14 MHz with divisor 2) */
    timestamp = timestamp * 7;

    /* Split timestamp between timers */
    tsL = 0xFFFF & timestamp;
    tsH = timestamp >> 16;

    if(tsH > 0) {
        /* In order to prevent short interrupt times on the lower/fast timer, we make sure it runs for more than 0x8000 ticks
         * Case 1) tsL is large => run as-is
         * Case 2) tsL is small => subtract 1 from tsH, run tsL+0x8000 and then lower timer again to tsL.
         */
        if(tsL > 0x8000) {
            /* Set Compare register for upper timer */
            TIMER_CompareSet(US_TIMER_H, 0, tsH);
            /* Enable interrupt for upper timer */
            TIMER_IntEnable(US_TIMER_H, TIMER_IF_CC0);
        } else {
            /* Set Compare registers */
            TIMER_CompareSet(US_TIMER_L, 0, tsL + 0x8000);
            TIMER_CompareSet(US_TIMER_H, 0, tsH - 1);
            extraLower = 1;
            /* Enable interrupt for upper timer */
            TIMER_IntEnable(US_TIMER_H, TIMER_IF_CC0);
        }
    } else {
        lowerOnly = 1;
        /* Set Compare register 0 for lower timer */
        TIMER_CompareSet(US_TIMER_L, 0, tsL);
        /* Enable interrupt for lower timer */
        TIMER_IntEnable(US_TIMER_L, TIMER_IF_CC0);
    }
}

void us_ticker_disable_interrupt(void) {
    /* Disable compare channel interrupts */
    TIMER_IntDisable(US_TIMER_L, TIMER_IF_CC0);
    TIMER_IntDisable(US_TIMER_H, TIMER_IF_CC0);
}

void us_ticker_clear_interrupt(void) {
    /* Clear compare channel interrupts */
    TIMER_IntClear(US_TIMER_L, TIMER_IF_CC0);
    TIMER_IntClear(US_TIMER_H, TIMER_IF_CC0);
}

