/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2018-2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
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
#define LPT_MCWDT_DELAY_WAIT    93                // Recommended value is 93, but then we fail function execution time test.
#define LPT_MCWDT_DELAY_NO_WAIT 0
#if !defined (CY_CFG_SYSCLK_CLKLF_FREQ_HZ)
#define CY_CFG_SYSCLK_CLKLF_FREQ_HZ    32768UL  /* Default to 32K ILO */
#endif /* CY_CFG_SYSCLK_CLKLF_FREQ_HZ */

#define MCWDT_COUNTER0_MAX_COUNT        (0xffffUL)
#define MCWDT_COUNTER1_MAX_COUNT        (0xffffUL)
#define MCWDT_COUNTER2_MAX_COUNT        (0xffffffffUL)
#define MAX_MCWDT_DURATION_SEC          (35UL*60UL*60UL*1000UL)
#define PLATFORM_MAX_DEEP_SLEEP_TICKS   (MAX_MCWDT_DURATION_SEC*CY_CFG_SYSCLK_CLKLF_FREQ_HZ)
#define MAX_MCWDT_COUNTER0_DURATION_SEC ((MCWDT_COUNTER0_MAX_COUNT + 1)/(CY_CFG_SYSCLK_CLKLF_FREQ_HZ))
#define LPT_MCWDT_CTRL                  (CY_MCWDT_CTR0 | CY_MCWDT_CTR1 | CY_MCWDT_CTR2)

static const ticker_info_t lp_ticker_info = {
    .frequency = CY_CFG_SYSCLK_CLKLF_FREQ_HZ,
    .bits = 32UL,
};

static bool lpt_init_done = false;

// Timer h/w configuration.
static cy_stc_mcwdt_config_t config = {
    .c0Match = MCWDT_COUNTER0_MAX_COUNT,
    .c1Match = MCWDT_COUNTER1_MAX_COUNT,
    .c0Mode = CY_MCWDT_MODE_INT,
    .c1Mode = CY_MCWDT_MODE_INT,
    .c2Mode = CY_MCWDT_MODE_NONE,
    .c2ToggleBit = 0,
    .c0ClearOnMatch = false,
    .c1ClearOnMatch = false,
    .c0c1Cascade = true,
    .c1c2Cascade = false
};

// Interrupt configuration.
static cy_stc_sysint_t lpt_sysint_config = {
#if defined(TARGET_MCU_PSOC6_M0)
    .intrSrc = (IRQn_Type)(-1),
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
    // Allocate NVIC channel.
    lpt_sysint_config.intrSrc = cy_m0_nvic_allocate_channel(CY_LP_TICKER_IRQN_ID);
    if (lpt_sysint_config.intrSrc == (IRQn_Type)(-1)) {
        // No free NVIC channel.
        error("LP_TICKER NVIC channel allocation failed.");
        return;
    }
#endif

    Cy_MCWDT_Init(LPT_MCWDT_UNIT, &config);
    Cy_SysInt_Init(&lpt_sysint_config, lp_ticker_irq_handler);
    NVIC_EnableIRQ(lpt_sysint_config.intrSrc);
    Cy_MCWDT_Enable(LPT_MCWDT_UNIT, LPT_MCWDT_CTRL, LPT_MCWDT_DELAY_WAIT);
    lpt_init_done = true;
}

void lp_ticker_free(void)
{
    NVIC_DisableIRQ(lpt_sysint_config.intrSrc);
    Cy_MCWDT_Disable(LPT_MCWDT_UNIT, LPT_MCWDT_CTRL, LPT_MCWDT_DELAY_WAIT);
#ifdef TARGET_MCU_PSOC6_M0
    cy_m0_nvic_release_channel(CY_LP_TICKER_IRQN_ID, lpt_sysint_config.intrSrc);
    lpt_sysint_config.intrSrc = (IRQn_Type)(-1);
#endif
    lpt_init_done = 0;
}

uint32_t lp_ticker_read(void)
{
    return Cy_MCWDT_GetCount(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER2);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t delay;
    uint16_t c0_count = Cy_MCWDT_GetCount(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER0);
    uint16_t c1_count = Cy_MCWDT_GetCount(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER1);
    uint32_t c2_count = Cy_MCWDT_GetCount(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER2);

    uint32_t new_ts = (uint32_t)timestamp;

    delay = new_ts - c2_count;
    lp_ticker_clear_interrupt();

    if (delay > MCWDT_COUNTER0_MAX_COUNT) {
        uint32_t c0_increment = delay % (MCWDT_COUNTER0_MAX_COUNT + 1);
        uint32_t counter0_count = (c0_count + delay) % (MCWDT_COUNTER0_MAX_COUNT + 1);
        uint32_t counter1_count =  (delay - c0_increment) / (MCWDT_COUNTER1_MAX_COUNT + 1) ;
        counter1_count = (c1_count + counter1_count) % (MCWDT_COUNTER1_MAX_COUNT + 1);

        if(counter1_count == 0) {
            counter1_count = 1;
        }
        if(counter0_count == 0) {
            counter0_count = 1;
        }
        Cy_MCWDT_SetMatch(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER1, counter1_count, LPT_MCWDT_DELAY_NO_WAIT);
        Cy_MCWDT_SetMatch(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER0, counter0_count, LPT_MCWDT_DELAY_NO_WAIT);
        Cy_MCWDT_SetInterruptMask(LPT_MCWDT_UNIT, CY_MCWDT_CTR1);

    }
    else {
        uint16_t counter0_count = c0_count + (uint16_t)delay;

        // MCWDT has internal delay of about 1.5 LF clock ticks, so this is the minimum
        // that we can schedule.
        if (delay < 3) {
            // Cheating a bit here.
            counter0_count = c0_count + 3;
        }

        if(counter0_count == 0) {
            counter0_count = 1;
        }
        Cy_MCWDT_SetMatch(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER1, MCWDT_COUNTER1_MAX_COUNT, LPT_MCWDT_DELAY_NO_WAIT);
        Cy_MCWDT_SetMatch(LPT_MCWDT_UNIT, CY_MCWDT_COUNTER0, counter0_count, LPT_MCWDT_DELAY_NO_WAIT);
        Cy_MCWDT_SetInterruptMask(LPT_MCWDT_UNIT, CY_MCWDT_CTR0);
    }
}

void lp_ticker_disable_interrupt(void)
{
    Cy_MCWDT_SetInterruptMask(LPT_MCWDT_UNIT, 0);
}

void lp_ticker_clear_interrupt(void)
{
    Cy_MCWDT_ClearInterrupt(LPT_MCWDT_UNIT, (CY_MCWDT_CTR0 | CY_MCWDT_CTR1));
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(lpt_sysint_config.intrSrc);
}

const ticker_info_t *lp_ticker_get_info(void)
{
    return &lp_ticker_info;
}

#endif // DEVICE_LPTICKER
