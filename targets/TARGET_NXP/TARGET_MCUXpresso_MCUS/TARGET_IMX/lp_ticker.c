/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#if DEVICE_LPTICKER

#include "lp_ticker_api.h"
#include "fsl_gpt.h"
#include "cmsis.h"

const ticker_info_t* lp_ticker_get_info()
{
    static const ticker_info_t info = {
        32768,        // 32kHz
           32         // 32 bit counter
    };
    return &info;
}

static bool lp_ticker_inited = false;

static void gpt_isr(void)
{
    GPT_ClearStatusFlags(GPT2, kGPT_OutputCompare1Flag);
    GPT_DisableInterrupts(GPT2, kGPT_OutputCompare1InterruptEnable);
    lp_ticker_irq_handler();
}

/** Initialize the low power ticker
 *
 */
void lp_ticker_init(void)
{
    gpt_config_t gptConfig;

    if (!lp_ticker_inited) {
        /* Setup GPT */
        GPT_GetDefaultConfig(&gptConfig);
        /* Use 32kHz drive */
        gptConfig.clockSource = kGPT_ClockSource_LowFreq;
        gptConfig.enableFreeRun = true;
        gptConfig.enableMode = false;

        GPT_Init(GPT2, &gptConfig);
        GPT_EnableInterrupts(GPT2, kGPT_OutputCompare1InterruptEnable);
        NVIC_ClearPendingIRQ(GPT2_IRQn);
        NVIC_SetVector(GPT2_IRQn, (uint32_t)gpt_isr);
        EnableIRQ(GPT2_IRQn);
        GPT_StartTimer(GPT2);
        lp_ticker_inited = true;
    } else {
        GPT_DisableInterrupts(GPT2, kGPT_OutputCompare1InterruptEnable);
    }
}

/** Read the current counter
 *
 * @return The current timer's counter value in ticks
 */
uint32_t lp_ticker_read(void)
{
    return GPT_GetCurrentTimerCount(GPT2);
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in ticks to be set
 */
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    if (timestamp == 0) {
        timestamp = 1;
    }

    GPT_StopTimer(GPT2);
    GPT_SetOutputCompareValue(GPT2, kGPT_OutputCompare_Channel1, timestamp);
    GPT_ClearStatusFlags(GPT2, kGPT_OutputCompare1Flag);
    GPT_EnableInterrupts(GPT2, kGPT_OutputCompare1InterruptEnable);
    GPT_StartTimer(GPT2);
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(GPT2_IRQn);
}

/** Disable low power ticker interrupt
 *
 */
void lp_ticker_disable_interrupt(void)
{
    GPT_DisableInterrupts(GPT2, kGPT_OutputCompare1InterruptEnable);
}

/** Clear the low power ticker interrupt
 *
 */
void lp_ticker_clear_interrupt(void)
{
    GPT_ClearStatusFlags(GPT2, kGPT_OutputCompare1Flag);
}

#endif /* DEVICE_LPTICKER */
