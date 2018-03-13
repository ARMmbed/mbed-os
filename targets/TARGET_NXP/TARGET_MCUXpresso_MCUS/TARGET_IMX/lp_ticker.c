/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
#include "fsl_snvs_hp.h"
#include "fsl_gpt.h"
#include "cmsis.h"
#include "rtc_api.h"

#define LOWFREQ_REF_CLK_HZ (32768)

static bool lp_ticker_inited = false;

static void gpt_isr(void)
{
    GPT_ClearStatusFlags(GPT2, kGPT_OutputCompare1Flag);
    GPT_StopTimer(GPT2);

    lp_ticker_irq_handler();
}

/** Initialize the low power ticker
 *
 */
void lp_ticker_init(void)
{
    gpt_config_t gptConfig;

    if (lp_ticker_inited) {
        return;
    }
    lp_ticker_inited = true;

    /* Setup low resolution clock - RTC */
    if (!rtc_isenabled()) {
        rtc_init();
    }

    /* Setup GPT */
    GPT_GetDefaultConfig(&gptConfig);
    /* Use 32kHz drive */
    gptConfig.clockSource = kGPT_ClockSource_LowFreq;
    GPT_Init(GPT2, &gptConfig);
    GPT_EnableInterrupts(GPT2, kGPT_OutputCompare1InterruptEnable);
    NVIC_ClearPendingIRQ(GPT2_IRQn);
    NVIC_SetVector(GPT2_IRQn, (uint32_t)gpt_isr);
    EnableIRQ(GPT2_IRQn);
}

/** Read the current counter
 *
 * @return The current timer's counter value in microseconds
 */
uint32_t lp_ticker_read(void)
{
    uint32_t ticks = 0;
    uint64_t tmp = 0;

    if (!lp_ticker_inited) {
        lp_ticker_init();
    }

    /* Do consecutive reads until value is correct */
    do
    {
        ticks = tmp;
        tmp = SNVS->HPRTCLR;
    } while (tmp != ticks);

    return COUNT_TO_USEC(ticks, LOWFREQ_REF_CLK_HZ);;
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in microseconds to be set
 */
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t now_us, delta_us, delta_ticks;

    if (!lp_ticker_inited) {
        lp_ticker_init();
    }

    now_us = lp_ticker_read();
    delta_us = timestamp > now_us ? timestamp - now_us : (uint32_t)((uint64_t)timestamp + 0xFFFFFFFF - now_us);

    delta_ticks = USEC_TO_COUNT(delta_us, LOWFREQ_REF_CLK_HZ);
    if (delta_ticks == 0) {
        delta_ticks = 1;
    }

    GPT_SetOutputCompareValue(GPT2, kGPT_OutputCompare_Channel1, delta_ticks);
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
