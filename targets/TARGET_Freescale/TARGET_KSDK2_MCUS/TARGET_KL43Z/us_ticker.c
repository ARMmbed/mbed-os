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
#include "fsl_lptmr.h"
#include "fsl_clock_config.h"

static int us_ticker_inited = 0;

void us_ticker_init(void) {
    if (us_ticker_inited) {
        return;
    }
    us_ticker_inited = 1;

    //Timer uses PIT
    //Common for ticker/timer
    uint32_t busClock;

    // Structure to initialize PIT
    pit_config_t pitConfig;

    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    busClock = CLOCK_GetFreq(kCLOCK_BusClk);

    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, busClock / 1000000 - 1);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, 0xFFFFFFFF);
    PIT_SetTimerChainMode(PIT, kPIT_Chnl_1, true);
    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PIT_StartTimer(PIT, kPIT_Chnl_1);

    //Ticker uses LPTMR
    lptmr_config_t lptmrConfig;
    LPTMR_GetDefaultConfig(&lptmrConfig);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_0;
    LPTMR_Init(LPTMR0, &lptmrConfig);

    busClock = CLOCK_GetFreq(kCLOCK_McgInternalRefClk);
    LPTMR_SetTimerPeriod(LPTMR0, busClock / 1000000 - 1);
    /* Set interrupt handler */
    NVIC_SetVector(LPTMR0_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(LPTMR0_IRQn);
}


uint32_t us_ticker_read() {
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    return ~(PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_1));
}

void us_ticker_disable_interrupt(void) {
    LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
}

void us_ticker_clear_interrupt(void) {
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
}

void us_ticker_set_interrupt(timestamp_t timestamp) {
    int delta = (int)(timestamp - us_ticker_read());
    if (delta <= 0) {
        // This event was in the past.
        // Set the interrupt as pending, but don't process it here.
        // This prevents a recurive loop under heavy load
        // which can lead to a stack overflow.
        NVIC_SetPendingIRQ(LPTMR0_IRQn);
        return;
    }

    LPTMR_StopTimer(LPTMR0);
    LPTMR_SetTimerPeriod(LPTMR0, (uint32_t)delta);
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(LPTMR0);
}
