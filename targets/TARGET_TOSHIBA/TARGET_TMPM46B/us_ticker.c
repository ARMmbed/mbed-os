/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
#include "us_ticker_api.h"
#include "tmpm46b_tmrb.h"

#define TMR16A_100US          0xFFFF
#define TMRB_CLK_DIV          0x3

static uint8_t us_ticker_inited = 0;                // Is ticker initialized yet?
static volatile uint32_t us_ticker = 0;             // timer counter

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
    TMRB_FFOutputTypeDef FFStruct;

    if (us_ticker_inited) {
        return;
    }
    us_ticker_inited = 1;

    // Enable channel 0
    TMRB_Enable(TSB_TB0);
    // Stops and clear count operation
    TMRB_SetRunState(TSB_TB0, TMRB_STOP);
    // Disable to TBxFF0 reverse trigger
    FFStruct.FlipflopCtrl = TMRB_FLIPFLOP_CLEAR;
    FFStruct.FlipflopReverseTrg =TMRB_DISABLE_FLIPFLOP;
    TMRB_SetFlipFlop(TSB_TB0, &FFStruct);

    // TSB_TB0 using free-run
    m_tmrb0.Mode = TMRB_INTERVAL_TIMER;
    m_tmrb0.ClkDiv = TMRB_CLK_DIV;
    m_tmrb0.UpCntCtrl = TMRB_AUTO_CLEAR;
    m_tmrb0.TrailingTiming = TMR16A_100US;
    m_tmrb0.LeadingTiming = TMR16A_100US;
    TMRB_Init(TSB_TB0, &m_tmrb0);

    // Enable TMRB when system is in idle mode
    TMRB_SetIdleMode(TSB_TB0, ENABLE);
    // Starts TSB_TB0
    TMRB_SetRunState(TSB_TB0, TMRB_RUN);
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
    TMRB_InitTypeDef m_tmrb1;
    TMRB_FFOutputTypeDef FFStruct;

    const uint32_t now_ticks = us_ticker_read();
    uint32_t delta_ticks =
        timestamp >= now_ticks ? timestamp - now_ticks : (uint32_t)((uint64_t) timestamp + 0xFFFF - now_ticks);

    if (delta_ticks == 0) {
        /* The requested delay is less than the minimum resolution of this counter. */
        delta_ticks = 1;
    }

    // Ticker interrupt handle
    TMRB_Enable(TSB_TB1);
    TMRB_SetRunState(TSB_TB1, TMRB_STOP);
    NVIC_SetVector(INTTB1_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(INTTB1_IRQn);

    // Split delta for preventing the Multiply overflowing
    FFStruct.FlipflopCtrl = TMRB_FLIPFLOP_CLEAR;
    FFStruct.FlipflopReverseTrg = TMRB_DISABLE_FLIPFLOP;
    TMRB_SetFlipFlop(TSB_TB1, &FFStruct);

    // TSB_TB0 using free-run
    m_tmrb1.Mode = TMRB_INTERVAL_TIMER;
    m_tmrb1.ClkDiv = TMRB_CLK_DIV;
    m_tmrb1.UpCntCtrl = TMRB_AUTO_CLEAR;
    m_tmrb1.TrailingTiming = delta_ticks;
    m_tmrb1.LeadingTiming = delta_ticks;
    TMRB_Init(TSB_TB1, &m_tmrb1);
    TMRB_SetINTMask(TSB_TB1,TMRB_MASK_OVERFLOW_INT | TMRB_MASK_MATCH_LEADING_INT);
    // Enable TMRB when system is in idle mode
    TMRB_SetIdleMode(TSB_TB1, ENABLE);
    TMRB_SetRunState(TSB_TB1, TMRB_RUN);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(INTTB1_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    // Also disable interrupts by NVIC
    NVIC_DisableIRQ(INTTB1_IRQn);
}

void us_ticker_clear_interrupt(void)
{
    // No flag to clear
}
