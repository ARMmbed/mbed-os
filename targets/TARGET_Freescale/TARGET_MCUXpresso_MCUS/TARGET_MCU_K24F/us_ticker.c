/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#include "PeripheralNames.h"
#include "fsl_pit.h"
#include "fsl_clock_config.h"

static int us_ticker_inited = 0;

static void pit_isr(void)
{
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, PIT_TFLG_TIF_MASK);
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, PIT_TFLG_TIF_MASK);
    PIT_StopTimer(PIT, kPIT_Chnl_2);
    PIT_StopTimer(PIT, kPIT_Chnl_3);

    us_ticker_irq_handler();
}

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        return;
    }
    us_ticker_inited = 1;
    //Common for ticker/timer
    uint32_t busClock;
    // Structure to initialize PIT
    pit_config_t pitConfig;

    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    busClock = CLOCK_GetFreq(kCLOCK_BusClk);

    //Timer
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, busClock / 1000000 - 1);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, 0xFFFFFFFF);
    PIT_SetTimerChainMode(PIT, kPIT_Chnl_1, true);
    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PIT_StartTimer(PIT, kPIT_Chnl_1);

    //Ticker
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, busClock / 1000000 - 1);
    PIT_SetTimerChainMode(PIT, kPIT_Chnl_3, true);
    NVIC_SetVector(PIT3_IRQn, (uint32_t)pit_isr);
    NVIC_EnableIRQ(PIT3_IRQn);
}


uint32_t us_ticker_read()
{
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    return ~(PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_1));
}

void us_ticker_disable_interrupt(void)
{
    PIT_DisableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
}

void us_ticker_clear_interrupt(void)
{
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, PIT_TFLG_TIF_MASK);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t delta = timestamp - us_ticker_read();
    PIT_StopTimer(PIT, kPIT_Chnl_3);
    PIT_StopTimer(PIT, kPIT_Chnl_2);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_3, (uint32_t)delta);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
    PIT_StartTimer(PIT, kPIT_Chnl_3);
    PIT_StartTimer(PIT, kPIT_Chnl_2);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(PIT3_IRQn);
}
