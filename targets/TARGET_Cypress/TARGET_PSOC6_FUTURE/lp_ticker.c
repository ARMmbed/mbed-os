/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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
#include "device.h"
#include "mbed_error.h"
#include "lp_ticker_api.h"
#include "cy_mcwdt.h"
#include "cy_sysint.h"
#include "psoc6_utils.h"

#if DEVICE_LPTICKER

/*
 * Low Power Timer API on PSoC6 uses MCWD0 timer0 to implement functionality.
 */

#if defined(TARGET_MCU_PSOC6_M0)
#define LPT_MCWDT_UNIT          MCWDT_STRUCT0
#define LPT_INTERRUPT_PRIORITY  3
#define LPT_INTERRUPT_SOURCE    srss_interrupt_mcwdt_0_IRQn
#else
#define LPT_MCWDT_UNIT          MCWDT_STRUCT1
#define LPT_INTERRUPT_PRIORITY  6
#define LPT_INTERRUPT_SOURCE    srss_interrupt_mcwdt_1_IRQn
#endif
#define LPT_MCWDT_DELAY_WAIT    0                // Recommended value is 93, but then we fail function execution time test.

static const ticker_info_t lp_ticker_info = {
    .frequency = CY_CLK_WCO_FREQ_HZ,
    .bits = 16,
};

static bool lpt_init_done = false;
// Timer h/w configuration.
static cy_stc_mcwdt_config_t config = {
    .c0Match = 0,
    .c1Match = 0,
    .c0Mode = CY_MCWDT_MODE_INT,
    .c1Mode = CY_MCWDT_MODE_NONE,
    .c2ToggleBit = 0,
    .c2Mode = CY_MCWDT_MODE_NONE,
    .c0ClearOnMatch = false,
    .c1ClearOnMatch = false,
    .c0c1Cascade = false,
    .c1c2Cascade = false
};

// Interrupt configuration.
static cy_stc_sysint_t lpt_sysint_config = {
#if defined(TARGET_MCU_PSOC6_M0)
    .intrSrc = CY_M0_CORE_IRQ_CHANNEL_LP_TICKER,
    .cm0pSrc = LPT_INTERRUPT_SOURCE,
#else
    .intrSrc = LPT_INTERRUPT_SOURCE,
#endif
    .intrPriority = LPT_INTERRUPT_PRIORITY
};


void lp_ticker_init(void)
{
    lp_ticker_disable_interrupt();
    lp_ticker_clear_interrupt();

    if (lpt_init_done) {
        return;
    }

#ifdef TARGET_MCU_PSOC6_M0
    // Reserve NVIC channel.
    if (cy_m0_nvic_reserve_channel(CY_M0_CORE_IRQ_CHANNEL_LP_TICKER, CY_LP_TICKER_IRQN_ID) == (IRQn_Type)(-1)) {
        // No free NVIC channel.
        error("LP_TICKER NVIC channel reservation conflict.");
        return;
    }
#endif

    Cy_MCWDT_Init(LPT_MCWDT_UNIT, &config);
    Cy_SysInt_Init(&lpt_sysint_config, lp_ticker_irq_handler);
    NVIC_EnableIRQ(lpt_sysint_config.intrSrc);
    Cy_MCWDT_Enable(LPT_MCWDT_UNIT, CY_MCWDT_CTR0, LPT_MCWDT_DELAY_WAIT);
    lpt_init_done = true;
}

void lp_ticker_free(void)
{
    NVIC_DisableIRQ(lpt_sysint_config.intrSrc);
    Cy_MCWDT_Disable(LPT_MCWDT_UNIT, CY_MCWDT_CTR0, LPT_MCWDT_DELAY_WAIT);
#ifdef TARGET_MCU_PSOC6_M0
    cy_m0_nvic_release_channel(CY_LP_TICKER_IRQN_ID, lpt_sysint_config.intrSrc);
    lpt_sysint_config.intrSrc = (IRQn_Type)(-1);
#endif
    lpt_init_done = 0;
}

uint32_t lp_ticker_read(void)
{
    return Cy_MCWDT_GetCount(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER0);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint16_t delay;
    uint16_t current = Cy_MCWDT_GetCount(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER0);
    uint16_t new_ts = (uint16_t)timestamp;
    delay = new_ts - current;
    // Make sure the event is set for the future. Mbed internally will not schedule
    // delays longer than 0x7000, so too large delay means it should occur already.
    // MCWDT has internal delay of about 1.5 LF clock ticks, so this is the minimum
    // that we can schedule.
    if ((delay < 3) || (delay > (uint16_t)(-3))) {
        // Cheating a bit here.
        new_ts = current + 3;
    }

    // Cypress PDL manual says that valid match range is 1..65535.
    if (new_ts == 0) {
        new_ts = 1;
    }

    // Set up and enable match interrupt.
    Cy_MCWDT_SetMatch(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER0, new_ts, LPT_MCWDT_DELAY_WAIT);
    Cy_MCWDT_SetInterruptMask(LPT_MCWDT_UNIT, CY_MCWDT_CTR0);
}

void lp_ticker_disable_interrupt(void)
{
    Cy_MCWDT_SetInterruptMask(LPT_MCWDT_UNIT, 0);
}

void lp_ticker_clear_interrupt(void)
{
    Cy_MCWDT_ClearInterrupt(LPT_MCWDT_UNIT, CY_MCWDT_CTR0);
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(lpt_sysint_config.intrSrc);
}

const ticker_info_t* lp_ticker_get_info(void)
{
    return &lp_ticker_info;
}

#endif // DEVICE_LPTICKER
