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

static uint8_t us_ticker_inited = 0;    // Is ticker initialized yet

static volatile uint32_t ticker_cnt = 0;  //Internal overflow count, used to extend internal 16-bit counter to (MHz * 32-bit)
static volatile uint32_t ticker_int_cnt = 0;    //Amount of overflows until user interrupt
static volatile uint8_t  ticker_freq_mhz = 0;   //Frequency of timer in MHz
static volatile uint32_t ticker_top_us = 0;   //Amount of us corresponding to the top value of the timer

void us_ticker_irq_handler_internal(void)
{
  /* Handle timer overflow */
  if (TIMER_IntGet(US_TICKER_TIMER) & TIMER_IF_OF) {
      ticker_cnt++;
      if(ticker_cnt >= ((uint32_t)ticker_freq_mhz << 16)) ticker_cnt = 0;
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

    /* Get frequency of clock in MHz for scaling ticks to microseconds */
    ticker_freq_mhz = (REFERENCE_FREQUENCY / 1000000);
    MBED_ASSERT(ticker_freq_mhz > 0);

    /*
     * Calculate maximum prescaler that gives at least 1 MHz frequency, while keeping clock as an integer multiple of 1 MHz.
     * Example: 14 MHz => prescaler = 1 (i.e. DIV2), ticker_freq_mhz = 7;
     *          24 MHz => prescaler = 3 (i.e. DIV8), ticker_freq_mhz = 3;
     *          48 MHz => prescaler = 4 (i.e. DIV16), ticker_freq_mhz = 3;
     * Limit prescaling to maximum prescaler value, which is 10 (DIV1024).
     */
    uint32_t prescaler = 0;
    while((ticker_freq_mhz & 1) == 0 && prescaler <= 10) {
        ticker_freq_mhz = ticker_freq_mhz >> 1;
        prescaler++;
    }

    /* Set prescaler */
    US_TICKER_TIMER->CTRL = (US_TICKER_TIMER->CTRL & ~_TIMER_CTRL_PRESC_MASK) | (prescaler << _TIMER_CTRL_PRESC_SHIFT);

    /* calculate top value */
    ticker_top_us = (uint32_t) 0x10000 / ticker_freq_mhz;

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
    TIMER_TopSet(US_TICKER_TIMER, (ticker_top_us * ticker_freq_mhz) - 1);

    /* Start TIMER */
    TIMER_Enable(US_TICKER_TIMER, true);
}

uint32_t us_ticker_read()
{
    uint32_t countH_old, countH;
    uint16_t countL;

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
    return (countL / ticker_freq_mhz) + (countH * ticker_top_us);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint64_t goal = timestamp;
    uint32_t trigger;

    if((US_TICKER_TIMER->IEN & TIMER_IEN_CC0) == 0) {
        //Timer was disabled, but is going to be enabled. Set sleep mode.
        blockSleepMode(TIMER_LEAST_ACTIVE_SLEEPMODE);
    }
    TIMER_IntDisable(US_TICKER_TIMER, TIMER_IEN_CC0);

    /* convert us delta value back to timer ticks */
    goal -= us_ticker_read();
    trigger = US_TICKER_TIMER->CNT;

    /* Catch "Going back in time" */
    if(goal < (50 / (REFERENCE_FREQUENCY / 1000000)) ||
       goal >= 0xFFFFFF00UL) {
        TIMER_IntClear(US_TICKER_TIMER, TIMER_IFC_CC0);
        TIMER_CompareSet(US_TICKER_TIMER, 0, (US_TICKER_TIMER->CNT + 3 > US_TICKER_TIMER->TOP ? 3 : US_TICKER_TIMER->CNT + 3));
        TIMER_IntEnable(US_TICKER_TIMER, TIMER_IEN_CC0);
        return;
    }

    /* Cap at 32 bit */
    goal &= 0xFFFFFFFFUL;
    /* Convert to ticker timebase */
    goal *= ticker_freq_mhz;

    /* Note: we should actually populate the following fields by the division and remainder
     * of goal / ticks_per_overflow, but since we're keeping the frequency as low
     * as possible, and ticks_per_overflow as close to FFFF as possible, we can
     * get away with ditching the division here and saving cycles.
     *
     * "exact" implementation:
     *    ticker_int_cnt = goal / TIMER_TopGet(US_TICKER_TIMER);
     *    ticker_int_rem = goal % TIMER_TopGet(US_TICKER_TIMER);
     */
    ticker_int_cnt = (goal >> 16) & 0xFFFFFFFF;

    /* Set compare channel 0 to (current position + lower 16 bits of target).
     * When lower 16 bits match, run complete cycles with ticker_int_rem as trigger value
     * for ticker_int_cnt times. */
    TIMER_IntClear(US_TICKER_TIMER, TIMER_IFC_CC0);

    /* Take top of timer into account so that we don't end up missing a cycle */
    /* Set trigger point by adding delta to current time */
    if((goal & 0xFFFF) >= TIMER_TopGet(US_TICKER_TIMER)) {
        trigger += (goal & 0xFFFF) - TIMER_TopGet(US_TICKER_TIMER);
        ticker_int_cnt++;
    } else {
        trigger += (goal & 0xFFFF);
    }

    if(trigger >= TIMER_TopGet(US_TICKER_TIMER)) {
        trigger -= TIMER_TopGet(US_TICKER_TIMER);
    }

    TIMER_CompareSet(US_TICKER_TIMER, 0, trigger);

    TIMER_IntEnable(US_TICKER_TIMER, TIMER_IEN_CC0);
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
