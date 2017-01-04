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
#include "PeripheralNames.h"
#include "fsl_pit.h"
#include "fsl_clock_config.h"

static int us_ticker_inited = 0;

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
    NVIC_SetVector(PIT0_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(PIT0_IRQn);
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
    int delta = (int)(timestamp - us_ticker_read());
    if (delta <= 0) {
        // This event was in the past.
        // Set the interrupt as pending, but don't process it here.
        // This prevents a recurive loop under heavy load
        // which can lead to a stack overflow.
        NVIC_SetPendingIRQ(PIT0_IRQn);
        return;
    }

    PIT_StopTimer(PIT, kPIT_Chnl_3);
    PIT_StopTimer(PIT, kPIT_Chnl_2);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_3, (uint32_t)delta);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
    PIT_StartTimer(PIT, kPIT_Chnl_3);
    PIT_StartTimer(PIT, kPIT_Chnl_2);
}
