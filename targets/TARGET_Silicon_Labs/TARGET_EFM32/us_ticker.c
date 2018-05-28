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

#define TICKER_FREQUENCY ((REFERENCE_FREQUENCY > 24000000) ? REFERENCE_FREQUENCY / 16 : REFERENCE_FREQUENCY / 8)

const ticker_info_t* us_ticker_get_info(void)
{
    static const ticker_info_t info = {
        TICKER_FREQUENCY,
        16
    };
    return &info;
}

static bool us_ticker_inited = false;            // Is ticker initialized yet

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        /* calling init again should cancel current interrupt */
        us_ticker_disable_interrupt();
        return;
    }
    us_ticker_inited = true;

    /* Enable clock for TIMERs */
    CMU_ClockEnable(US_TICKER_TIMER_CLOCK, true);

    if (REFERENCE_FREQUENCY > 24000000) {
        US_TICKER_TIMER->CTRL = (US_TICKER_TIMER->CTRL & ~_TIMER_CTRL_PRESC_MASK) | (4 << _TIMER_CTRL_PRESC_SHIFT);
    } else {
        US_TICKER_TIMER->CTRL = (US_TICKER_TIMER->CTRL & ~_TIMER_CTRL_PRESC_MASK) | (3 << _TIMER_CTRL_PRESC_SHIFT);
    }

    /* Clear TIMER counter value */
    TIMER_CounterSet(US_TICKER_TIMER, 0);

    /* Start TIMER */
    TIMER_Enable(US_TICKER_TIMER, true);

    /* Select Compare Channel parameters */
    TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;
    timerCCInit.mode = timerCCModeCompare;

    /* Configure Compare Channel 0 */
    TIMER_InitCC(US_TICKER_TIMER, 0, &timerCCInit);

    /* Enable interrupt vector in NVIC */
    TIMER_IntClear(US_TICKER_TIMER, TIMER_IEN_CC0);
    NVIC_SetVector(US_TICKER_TIMER_IRQ, (uint32_t) us_ticker_irq_handler);
    NVIC_EnableIRQ(US_TICKER_TIMER_IRQ);
}

void us_ticker_free(void)
{
    if (us_ticker_inited) {
        us_ticker_disable_interrupt();
        NVIC_DisableIRQ(US_TICKER_TIMER_IRQ);

        TIMER_Enable(US_TICKER_TIMER, false);

        CMU_ClockEnable(US_TICKER_TIMER_CLOCK, false);

        us_ticker_inited = false;
    }
}

uint32_t us_ticker_read()
{
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    return US_TICKER_TIMER->CNT;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    TIMER_IntDisable(US_TICKER_TIMER, TIMER_IEN_CC0);

    TIMER_IntClear(US_TICKER_TIMER, TIMER_IEN_CC0);

    TIMER_CompareSet(US_TICKER_TIMER, 0, timestamp);

    TIMER_IntEnable(US_TICKER_TIMER, TIMER_IEN_CC0);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(US_TICKER_TIMER_IRQ);
}

void us_ticker_disable_interrupt(void)
{
    /* Disable compare channel interrupts */
    TIMER_IntDisable(US_TICKER_TIMER, TIMER_IEN_CC0);
}

void us_ticker_clear_interrupt(void)
{
    /* Clear compare channel interrupts */
    TIMER_IntClear(US_TICKER_TIMER, TIMER_IFC_CC0);
}
