/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include <stdbool.h>
#include "us_ticker_api.h"
#include "tmpm46b_tmrb.h"

#define MAX_TICK_16_BIT         0xFFFF
#define TMRB_CLK_DIV            0x3

static bool us_ticker_inited = false;                // Is ticker initialized yet?

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        1875000, // 1875000,
        16       // 16 bit counter
    };
    return &info;
}

// initialize us_ticker
void us_ticker_init(void)
{
    TMRB_InitTypeDef m_tmrb0;

    if (us_ticker_inited) {
        us_ticker_disable_interrupt();
        return;
    }
    us_ticker_inited = true;

    // TSB_TB0 using free-run
    m_tmrb0.Mode = TMRB_INTERVAL_TIMER;
    m_tmrb0.ClkDiv = TMRB_CLK_DIV;
    m_tmrb0.UpCntCtrl = TMRB_FREE_RUN;
    m_tmrb0.TrailingTiming = MAX_TICK_16_BIT;
    m_tmrb0.LeadingTiming = MAX_TICK_16_BIT;

    // Enable channel 0
    TMRB_Enable(TSB_TB0);
    // Stops and clear count operation
    TMRB_SetRunState(TSB_TB0, TMRB_STOP);
    // Mask All interrupts
    TMRB_SetINTMask(TSB_TB0, TMRB_MASK_MATCH_LEADING_INT | TMRB_MASK_MATCH_TRAILING_INT | TMRB_MASK_OVERFLOW_INT);
    TMRB_Init(TSB_TB0, &m_tmrb0);
    // Enable TMRB when system is in idle mode
    TMRB_SetIdleMode(TSB_TB0, ENABLE);
    // Starts TSB_TB0
    TMRB_SetRunState(TSB_TB0, TMRB_RUN);
    NVIC_SetVector(INTTB0_IRQn, (uint32_t)us_ticker_irq_handler);
}

uint32_t us_ticker_read(void)
{
    uint32_t ret_val = 0;

    if (!us_ticker_inited) {
        us_ticker_init();
    }

    ret_val = (uint32_t)TMRB_GetUpCntValue(TSB_TB0);

    return ret_val;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    NVIC_DisableIRQ(INTTB0_IRQn);
    NVIC_ClearPendingIRQ(INTTB0_IRQn);
    TMRB_ChangeTrailingTiming(TSB_TB0, timestamp);
    // Mask all Interrupts except trailing edge interrupt
    TMRB_SetINTMask(TSB_TB0, TMRB_MASK_MATCH_LEADING_INT | TMRB_MASK_OVERFLOW_INT);
    NVIC_EnableIRQ(INTTB0_IRQn);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(INTTB0_IRQn);
    NVIC_EnableIRQ(INTTB0_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    // Mask All interrupts
    TMRB_SetINTMask(TSB_TB0, TMRB_MASK_MATCH_LEADING_INT | TMRB_MASK_MATCH_TRAILING_INT | TMRB_MASK_OVERFLOW_INT);
    // Also clear and disable interrupts by NVIC
    NVIC_ClearPendingIRQ(INTTB0_IRQn);
    NVIC_DisableIRQ(INTTB0_IRQn);
}

void us_ticker_clear_interrupt(void)
{
    NVIC_ClearPendingIRQ(INTTB0_IRQn);
}

void us_ticker_free(void)
{
    TMRB_SetINTMask(TSB_TB0, TMRB_MASK_MATCH_LEADING_INT | TMRB_MASK_MATCH_TRAILING_INT | TMRB_MASK_OVERFLOW_INT);
    NVIC_ClearPendingIRQ(INTTB0_IRQn);
    NVIC_DisableIRQ(INTTB0_IRQn);
    TMRB_SetRunState(TSB_TB0, TMRB_STOP);
    TMRB_Disable(TSB_TB0);
    us_ticker_inited = false;
}
