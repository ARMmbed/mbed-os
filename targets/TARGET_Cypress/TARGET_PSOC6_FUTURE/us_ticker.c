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
#include <limits.h>
#include "device.h"
#include "PeripheralNames.h"
#include "us_ticker_api.h"
#include "mbed_error.h"
#include "psoc6_utils.h"

#include "cy_sysint.h"
#include "cy_sysclk.h"
#include "cy_tcpwm_counter.h"
#include "cy_syspm.h"

/** Each CPU core in PSoC6 needs its own usec timer.
 ** Although each of TCPWM timers have two compare registers,
 ** it has only one interrupt line, so we need to allocate
 ** two TCPWM counters for the purpose of us_ticker
 **/


#if defined(TARGET_MCU_PSOC6_M0)

#define TICKER_COUNTER_UNIT                 TCPWM0
#define TICKER_COUNTER_NUM                  0
#define TICKER_COUNTER_INTERRUPT_SOURCE     tcpwm_0_interrupts_0_IRQn
#define TICKER_COUNTER_NVIC_IRQN            CY_M0_CORE_IRQ_CHANNEL_US_TICKER
#define TICKER_COUNTER_INTERRUPT_PRIORITY   3
#define TICKER_CLOCK_DIVIDER_NUM            0

#elif defined(TARGET_MCU_PSOC6_M4)

#define TICKER_COUNTER_UNIT                 TCPWM0
#define TICKER_COUNTER_NUM                  1
#define TICKER_COUNTER_INTERRUPT_SOURCE     tcpwm_0_interrupts_1_IRQn
#define TICKER_COUNTER_NVIC_IRQN            TICKER_COUNTER_INTERRUPT_SOURCE
#define TICKER_COUNTER_INTERRUPT_PRIORITY   6
#define TICKER_CLOCK_DIVIDER_NUM            1

#else
#error "Unknown MCU type."
#endif


static const ticker_info_t us_ticker_info = {
    .frequency = 1000000UL,
    .bits = 32,
};

static const cy_stc_sysint_t us_ticker_sysint_cfg = {
    .intrSrc = TICKER_COUNTER_NVIC_IRQN,
#if defined(TARGET_MCU_PSOC6_M0)
    .cm0pSrc = TICKER_COUNTER_INTERRUPT_SOURCE,
#endif
    .intrPriority = TICKER_COUNTER_INTERRUPT_PRIORITY
};

static int      us_ticker_inited = 0;

static const cy_stc_tcpwm_counter_config_t cy_counter_config = {
    .period = 0xFFFFFFFFUL,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_COMPARE,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_ON_CC,
    .countInputMode = CY_TCPWM_INPUT_LEVEL,
    .countInput = CY_TCPWM_INPUT_1
};

// PM callback to be executed when exiting deep sleep.
static cy_en_syspm_status_t ticker_pm_callback(cy_stc_syspm_callback_params_t *callbackParams);

static cy_stc_syspm_callback_params_t ticker_pm_callback_params = {
    .base = TICKER_COUNTER_UNIT
};

static cy_stc_syspm_callback_t ticker_pm_callback_handler = {
    .callback = ticker_pm_callback,
    .type = CY_SYSPM_DEEPSLEEP,
    .skipMode = CY_SYSPM_SKIP_CHECK_READY | CY_SYSPM_SKIP_CHECK_FAIL | CY_SYSPM_SKIP_BEFORE_TRANSITION,
    .callbackParams = &ticker_pm_callback_params
};


/*
 * Callback handler to restart the timer after deep sleep.
 */
static cy_en_syspm_status_t ticker_pm_callback(cy_stc_syspm_callback_params_t *params)
{
    if (params->mode == CY_SYSPM_AFTER_TRANSITION) {
        Cy_TCPWM_Counter_Enable(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM);
        Cy_TCPWM_TriggerStart(TICKER_COUNTER_UNIT, 1UL << TICKER_COUNTER_NUM);
    }
    return CY_SYSPM_SUCCESS;
}

/*
 * Interrupt handler.
 */
static void local_irq_handler(void)
{
    us_ticker_clear_interrupt();
    us_ticker_disable_interrupt();
    us_ticker_irq_handler();
}


void us_ticker_init(void)
{
    us_ticker_disable_interrupt();
    us_ticker_clear_interrupt();

    if (us_ticker_inited)
        return;

    us_ticker_inited = 1;

    // Configure the clock, us_ticker 1 MHz from PCLK 50 MHz
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM0_CLOCKS0 + TICKER_COUNTER_NUM, CY_SYSCLK_DIV_8_BIT, TICKER_CLOCK_DIVIDER_NUM);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, TICKER_CLOCK_DIVIDER_NUM, (CY_CLK_PERICLK_FREQ_HZ / 1000000UL) - 1);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, TICKER_CLOCK_DIVIDER_NUM);

    /*
        Configure the counter
    */

    Cy_TCPWM_Counter_Init(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM, &cy_counter_config);
    Cy_TCPWM_Counter_Enable(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM);
    if (!Cy_SysPm_RegisterCallback(&ticker_pm_callback_handler)) {
        error("PM callback registration failed!");
    }
    Cy_TCPWM_TriggerStart(TICKER_COUNTER_UNIT, 1UL << TICKER_COUNTER_NUM);

#if defined (TARGET_MCU_PSOC6_M0)
    if (cy_m0_nvic_reserve_channel(TICKER_COUNTER_NVIC_IRQN, CY_US_TICKER_IRQN_ID) == (IRQn_Type)(-1)) {
        error("Microsecond ticker NVIC channel reservation conflict.");
    }
#endif //

    Cy_SysInt_Init(&us_ticker_sysint_cfg, local_irq_handler);
}

void us_ticker_free(void)
{
    us_ticker_disable_interrupt();
    Cy_TCPWM_Counter_Disable(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM);
    Cy_SysPm_UnregisterCallback(&ticker_pm_callback_handler);
#if defined (TARGET_MCU_PSOC6_M0)
    cy_m0_nvic_release_channel(TICKER_COUNTER_NVIC_IRQN, CY_US_TICKER_IRQN_ID);
#endif //
    us_ticker_inited = 0;
}

uint32_t us_ticker_read(void)
{
    if (!us_ticker_inited)
        us_ticker_init();
    return Cy_TCPWM_Counter_GetCounter(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t current_ts = Cy_TCPWM_Counter_GetCounter(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM);
    uint32_t delta = timestamp - current_ts;

    if (!us_ticker_inited)
        us_ticker_init();

    // Set new output compare value
    if ((delta < 2) || (delta  > (uint32_t)(-3))) {
        timestamp = current_ts + 2;
    }
    Cy_TCPWM_Counter_SetCompare0(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM, timestamp);
    // Enable int
    NVIC_EnableIRQ(TICKER_COUNTER_NVIC_IRQN);
}

void us_ticker_disable_interrupt(void)
{
    NVIC_DisableIRQ(TICKER_COUNTER_NVIC_IRQN);
}

void us_ticker_clear_interrupt(void)
{
    Cy_TCPWM_ClearInterrupt(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM, CY_TCPWM_INT_ON_CC);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_EnableIRQ(TICKER_COUNTER_NVIC_IRQN);
    NVIC_SetPendingIRQ(TICKER_COUNTER_NVIC_IRQN);
}

const ticker_info_t* us_ticker_get_info(void)
{
    return &us_ticker_info;
}
