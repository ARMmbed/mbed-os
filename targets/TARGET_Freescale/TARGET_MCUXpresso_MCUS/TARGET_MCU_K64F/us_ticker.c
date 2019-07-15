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
#include "us_ticker_defines.h"
#include "PeripheralNames.h"
#include "fsl_pit.h"
#include "fsl_clock_config.h"

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        1000000,    // 1 MHz
             32     // 32 bit counter
    };
    return &info;
}

static bool us_ticker_inited = false;

static void pit_isr(void)
{
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, PIT_TFLG_TIF_MASK);
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, PIT_TFLG_TIF_MASK);
    PIT_StopTimer(PIT, kPIT_Chnl_2);
    PIT_StopTimer(PIT, kPIT_Chnl_3);

    us_ticker_irq_handler();
}

/** Initialize the high frequency ticker
 *
 */
void us_ticker_init(void)
{
    /* Common for ticker/timer. */
    uint32_t busClock;
    /* Structure to initialize PIT. */
    pit_config_t pitConfig;

    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    busClock = CLOCK_GetFreq(kCLOCK_BusClk);

    /* Let the timer to count if re-init. */
    if (!us_ticker_inited) {

        PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, busClock / 1000000 - 1);
        PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, 0xFFFFFFFF);
        PIT_SetTimerChainMode(PIT, kPIT_Chnl_1, true);
        PIT_StartTimer(PIT, kPIT_Chnl_0);
        PIT_StartTimer(PIT, kPIT_Chnl_1);
    }

    /* Configure interrupt generation counters and disable ticker interrupts. */
    PIT_StopTimer(PIT, kPIT_Chnl_3);
    PIT_StopTimer(PIT, kPIT_Chnl_2);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, busClock / 1000000 - 1);
    PIT_SetTimerChainMode(PIT, kPIT_Chnl_3, true);
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, PIT_TFLG_TIF_MASK);
    NVIC_SetVector(PIT3_IRQn, (uint32_t) pit_isr);
    NVIC_EnableIRQ(PIT3_IRQn);
    PIT_DisableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);

    us_ticker_inited = true;
}

/** Read the current counter
 *
 * @return The current timer's counter value in ticks
 */
uint32_t (us_ticker_read)()
{
    return us_ticker_read();
}

/** Disable us ticker interrupt
 *
 */
void us_ticker_disable_interrupt(void)
{
    PIT_DisableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
}

/** Clear us ticker interrupt
 *
 */
void us_ticker_clear_interrupt(void)
{
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, PIT_TFLG_TIF_MASK);
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in ticks when interrupt should be generated
 */
void us_ticker_set_interrupt(timestamp_t timestamp)
{
    /* We get here absolute interrupt time which takes into account counter overflow.
     * Since we use additional count-down timer to generate interrupt we need to calculate
     * load value based on time-stamp.
     */
    const uint32_t now_ticks = us_ticker_read();
    uint32_t delta_ticks =
            timestamp >= now_ticks ? timestamp - now_ticks : (uint32_t)((uint64_t) timestamp + 0xFFFFFFFF - now_ticks);

    if (delta_ticks == 0) {
        /* The requested delay is less than the minimum resolution of this counter. */
        delta_ticks = 1;
    }

    PIT_StopTimer(PIT, kPIT_Chnl_3);
    PIT_StopTimer(PIT, kPIT_Chnl_2);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_3, delta_ticks);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
    PIT_StartTimer(PIT, kPIT_Chnl_3);
    PIT_StartTimer(PIT, kPIT_Chnl_2);
}

/** Fire us ticker interrupt
 *
 */
void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(PIT3_IRQn);
}

void us_ticker_free(void)
{
    PIT_StartTimer(PIT, kPIT_Chnl_3);
    PIT_StartTimer(PIT, kPIT_Chnl_2);
    PIT_StartTimer(PIT, kPIT_Chnl_1);
    PIT_StartTimer(PIT, kPIT_Chnl_0);

    PIT_DisableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
    NVIC_DisableIRQ(PIT3_IRQn);

    us_ticker_inited = false;
}
