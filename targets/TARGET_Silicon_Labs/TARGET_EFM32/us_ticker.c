/***************************************************************************//**
 * @file us_ticker.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include <stddef.h>
#include "us_ticker_api.h"
#include "device.h"
#include "mbed_assert.h"
#include "em_cmu.h"
#include "em_timer.h"
#include "clocking.h"
#include "sleep_api.h"
#include "sleepmodes.h"

#define TIMER_LEAST_ACTIVE_SLEEPMODE EM1
/**
 * Timer functions for microsecond ticker.
 * mbed expects a 32-bit timer. Since the EFM32 only has 16-bit timers,
 * the upper 16 bits are implemented in software.
 */

static uint8_t us_ticker_inited = 0;            // Is ticker initialized yet

static volatile uint32_t ticker_cnt = 0;        // Internal overflow count, used to extend internal 16-bit counter to (MHz * 32-bit)
static volatile uint32_t ticker_int_cnt = 0;    // Amount of overflows until user interrupt
static volatile uint32_t ticker_freq_khz = 0;   // Frequency of timer in MHz
static volatile uint32_t ticker_top_ms = 0;     // Amount of ms corresponding to the top value of the timer
static volatile uint32_t soft_timer_top = 0;    // When to wrap the software counter

void us_ticker_irq_handler_internal(void)
{
    /* Handle timer overflow */
    if (TIMER_IntGet(US_TICKER_TIMER) & TIMER_IF_OF) {
        ticker_cnt++;

        /* Wrap ticker_cnt when we've gone over 32-bit us value */
        if (ticker_cnt >= soft_timer_top) {
            ticker_cnt = 0;
        }

        TIMER_IntClear(US_TICKER_TIMER, TIMER_IF_OF);
    }

    /* Check for user interrupt expiration */
    if (TIMER_IntGet(US_TICKER_TIMER) & TIMER_IF_CC0) {
        if (ticker_int_cnt > 0) {
            ticker_int_cnt--;
            TIMER_IntClear(US_TICKER_TIMER, TIMER_IF_CC0);
        } else {
            us_ticker_irq_handler();
        }
    }
}

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        return;
    }
    us_ticker_inited = 1;

    /* Enable clock for TIMERs */
    CMU_ClockEnable(US_TICKER_TIMER_CLOCK, true);

    /* Clear TIMER counter value */
    TIMER_CounterSet(US_TICKER_TIMER, 0);

    /* Get frequency of clock in kHz for scaling ticks to microseconds */
    ticker_freq_khz = (REFERENCE_FREQUENCY / 1000);
    MBED_ASSERT(ticker_freq_khz > 0);

    /*
     * Calculate maximum prescaler that gives at least 1 MHz frequency, giving us 1us resolution.
     * Limit prescaling to maximum prescaler value, which is 10 (DIV1024).
     */
    uint32_t prescaler = 0;
    while((ticker_freq_khz >= 2000) && prescaler <= 10) {
        ticker_freq_khz = ticker_freq_khz >> 1;
        prescaler++;
    }

    /* Set prescaler */
    US_TICKER_TIMER->CTRL = (US_TICKER_TIMER->CTRL & ~_TIMER_CTRL_PRESC_MASK) | (prescaler << _TIMER_CTRL_PRESC_SHIFT);

    /* calculate top value.*/
    ticker_top_ms = (uint32_t) 0x10000 / ticker_freq_khz;

    /* calculate software timer overflow */
    soft_timer_top = ((0xFFFFFFFFUL / 1000UL) / ticker_top_ms) + 1;

    /* Select Compare Channel parameters */
    TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;
    timerCCInit.mode = timerCCModeCompare;

    /* Configure Compare Channel 0 */
    TIMER_InitCC(US_TICKER_TIMER, 0, &timerCCInit);

    /* Enable interrupt vector in NVIC */
    TIMER_IntEnable(US_TICKER_TIMER, TIMER_IEN_OF);
    NVIC_SetVector(US_TICKER_TIMER_IRQ, (uint32_t) us_ticker_irq_handler_internal);
    NVIC_EnableIRQ(US_TICKER_TIMER_IRQ);

    /* Set top value */
    TIMER_TopSet(US_TICKER_TIMER, (ticker_top_ms * ticker_freq_khz) - 1);

    /* Start TIMER */
    TIMER_Enable(US_TICKER_TIMER, true);
}

uint32_t us_ticker_read()
{
    uint32_t countH_old, countH;
    uint32_t countL;

    if (!us_ticker_inited) {
        us_ticker_init();
    }

    /* Avoid jumping in time by reading high bits twice */
    do {
        countH_old = ticker_cnt;
        if (TIMER_IntGet(US_TICKER_TIMER) & TIMER_IF_OF) {
            countH_old++;
        }
        countL = US_TICKER_TIMER->CNT;
        countH = ticker_cnt;
        if (TIMER_IntGet(US_TICKER_TIMER) & TIMER_IF_OF) {
            countH++;
        }
    } while (countH_old != countH);

    /* Timer count value needs to be div'ed by the frequency to get to 1MHz ticks.
     * For the software-extended part, the amount of us in one overflow is constant.
     */
    return ((countL * 1000UL) / ticker_freq_khz) + (countH * ticker_top_ms * 1000);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t goal = timestamp;
    uint32_t trigger;

    if((US_TICKER_TIMER->IEN & TIMER_IEN_CC0) == 0) {
        //Timer was disabled, but is going to be enabled. Set sleep mode.
        blockSleepMode(TIMER_LEAST_ACTIVE_SLEEPMODE);
    }
    TIMER_IntDisable(US_TICKER_TIMER, TIMER_IEN_CC0);

    /* convert us delta value back to timer ticks */
    trigger = us_ticker_read();
    if (trigger < goal) {
        goal -= trigger;
    } else {
        goal = (0xFFFFFFFFUL - (trigger - goal));
    }
    trigger = US_TICKER_TIMER->CNT;

    /* Catch "Going back in time" */
    if(goal < 10 ||
       goal >= 0xFFFFFF00UL) {
        TIMER_IntClear(US_TICKER_TIMER, TIMER_IFC_CC0);
        TIMER_CompareSet(US_TICKER_TIMER, 0, (US_TICKER_TIMER->CNT + 3 >= US_TICKER_TIMER->TOP ? 3 : US_TICKER_TIMER->CNT + 3));
        TIMER_IntEnable(US_TICKER_TIMER, TIMER_IEN_CC0);
        return;
    }

    uint32_t timer_top = TIMER_TopGet(US_TICKER_TIMER);
    uint32_t top_us = 1000 * ticker_top_ms;

    /* Amount of times we expect to overflow: us offset / us period of timer */
    ticker_int_cnt = goal / top_us;

    /* Leftover microseconds need to be converted to timer timebase */
    trigger += (((goal % top_us) * ticker_freq_khz) / 1000);

    /* Cap compare value to timer top */
    if (trigger >= timer_top) {
        trigger -= timer_top;
    }

    /* Set compare channel 0 to (current position + lower 16 bits of target).
     * When lower 16 bits match, run complete cycles with ticker_int_rem as trigger value
     * for ticker_int_cnt times. */
    TIMER_IntClear(US_TICKER_TIMER, TIMER_IEN_CC0);

    TIMER_CompareSet(US_TICKER_TIMER, 0, trigger);

    TIMER_IntEnable(US_TICKER_TIMER, TIMER_IEN_CC0);
}

void us_ticker_fire_interrupt(void)
{
    ticker_int_cnt = 0;
    TIMER_IntSet(US_TICKER_TIMER, TIMER_IF_CC0);
    NVIC_SetPendingIRQ(US_TICKER_TIMER_IRQ);
}

void us_ticker_disable_interrupt(void)
{
    if((US_TICKER_TIMER->IEN & TIMER_IEN_CC0) != 0) {
        //Timer was enabled, but is going to get disabled. Clear sleepmode.
        unblockSleepMode(TIMER_LEAST_ACTIVE_SLEEPMODE);
    }
    /* Disable compare channel interrupts */
    TIMER_IntDisable(US_TICKER_TIMER, TIMER_IEN_CC0);
}

void us_ticker_clear_interrupt(void)
{
    /* Clear compare channel interrupts */
    TIMER_IntClear(US_TICKER_TIMER, TIMER_IFC_CC0);
}
