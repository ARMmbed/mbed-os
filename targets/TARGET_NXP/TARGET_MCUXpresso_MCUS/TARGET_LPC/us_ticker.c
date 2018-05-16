/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#include "fsl_ctimer.h"
#include "PeripheralNames.h"

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        1000000,    // 1 MHz
             32     // 32 bit counter
    };
    return &info;
}

static bool us_ticker_inited = false;

/** Initialize the high frequency ticker
 *
 */
void us_ticker_init(void) {
    ctimer_config_t config;

    uint32_t pclk = CLOCK_GetFreq(kCLOCK_BusClk);
    uint32_t prescale = pclk / 1000000; // default to 1MHz (1 us ticks)

    /* Let the timer to count if re-init. */
    if (!us_ticker_inited) {

        CTIMER_GetDefaultConfig(&config);
        config.prescale = prescale - 1;
        CTIMER_Init(CTIMER1, &config);
        CTIMER_Reset(CTIMER1);
        CTIMER_StartTimer(CTIMER1);
    }
    NVIC_SetVector(CTIMER1_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(CTIMER1_IRQn);
    CTIMER1->MCR &= ~1;

    us_ticker_inited = true;
}

/** Read the current counter
 *
 * @return The current timer's counter value in ticks
 */
uint32_t us_ticker_read(void) {
    return CTIMER1->TC;
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in ticks when interrupt should be generated
 */
void us_ticker_set_interrupt(timestamp_t timestamp) {
    ctimer_match_config_t matchConfig;

    matchConfig.enableCounterReset = false;
    matchConfig.enableCounterStop = false;
    matchConfig.matchValue = (uint32_t)timestamp;
    matchConfig.outControl = kCTIMER_Output_NoAction;
    matchConfig.outPinInitState = true;
    matchConfig.enableInterrupt = true;

    CTIMER_SetupMatch(CTIMER1, kCTIMER_Match_0, &matchConfig);
}

/** Disable us ticker interrupt
 *
 */
void us_ticker_disable_interrupt(void) {
    CTIMER1->MCR &= ~1;
}

/** Clear us ticker interrupt
 *
 */
void us_ticker_clear_interrupt(void) {
    CTIMER1->IR = 1;
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(CTIMER1_IRQn);
}
