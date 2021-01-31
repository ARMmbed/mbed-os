/* mbed Microcontroller Library
 * Copyright (c) 2019-2020, Arm Limited and affiliates.
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
#include "cmsis.h"
#include "us_ticker_api.h"
#include "mbed_error.h"
#include "cy_us_ticker.h"
#include "cyhal_timer.h"
#include "cy_tcpwm_counter.h"

#define CY_US_TICKER_IRQ_PRIORITY 3

#ifdef __cplusplus
extern "C" {
#endif

static ticker_info_t cy_us_ticker_info;
static cyhal_timer_t cy_us_timer;
static bool cy_us_ticker_initialized = false;

static cy_en_syspm_status_t cy_us_ticker_pm_callback(cy_stc_syspm_callback_params_t *params, cy_en_syspm_callback_mode_t mode)
{
    if (mode == CY_SYSPM_AFTER_TRANSITION) {
        Cy_TCPWM_Counter_Enable(cy_us_timer.tcpwm.base, cy_us_timer.tcpwm.resource.channel_num);
        Cy_TCPWM_TriggerStart(cy_us_timer.tcpwm.base, 1u << cy_us_timer.tcpwm.resource.channel_num);
    }
    return CY_SYSPM_SUCCESS;
}

static cy_stc_syspm_callback_params_t cy_us_ticker_pm_params;
static cy_stc_syspm_callback_t cy_us_ticker_pm_data = {
    .callback = &cy_us_ticker_pm_callback,
    .type = CY_SYSPM_DEEPSLEEP,
    .callbackParams = &cy_us_ticker_pm_params,
};

static void cy_us_ticker_irq_handler(MBED_UNUSED void *arg, MBED_UNUSED cyhal_timer_event_t event)
{
    us_ticker_irq_handler();
}

void cy_us_ticker_start()
{
    cyhal_timer_start(&cy_us_timer);
}

void cy_us_ticker_stop()
{
    cyhal_timer_stop(&cy_us_timer);
}

void us_ticker_init(void)
{
    if (!cy_us_ticker_initialized) {

#ifdef TARGET_TFM
        /* There are two timers, Timer0 and Timer1, available on the PSoC64.
         * Timer0 has 8 channels and Timer1 has 24 channels. TF-M regression
         * tests make use of Timer0 Channel 1 and Timer0 Channel 2. Therefore,
         * reserve the timer channels used by TF-M. This approach can be
         * replaced once we have a way to allocate dedicated timers for TF-M
         * and Mbed OS. */
        cyhal_resource_inst_t res = { CYHAL_RSC_TCPWM, 0, 0 };
        if(CY_RSLT_SUCCESS != cyhal_hwmgr_reserve(&res)) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_timer_init");
        }
        res.channel_num = 1;
        if(CY_RSLT_SUCCESS != cyhal_hwmgr_reserve(&res)) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_timer_init");
        }
#endif
        if (CY_RSLT_SUCCESS != cyhal_timer_init(&cy_us_timer, NC, NULL)) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_timer_init");
        }
        // Configure clock for about 1 MHz
        MBED_ASSERT(cy_PeriClkFreqHz >= 1000000);
        uint32_t div_value = cy_PeriClkFreqHz / 1000000;
        cy_us_ticker_info.frequency = cy_PeriClkFreqHz / div_value;
        cy_us_ticker_info.bits = CYHAL_TCPWM_DATA[cy_us_timer.tcpwm.resource.block_num].max_count;
        Cy_SysClk_PeriphSetDivider(cy_us_timer.tcpwm.clock.div_type, cy_us_timer.tcpwm.clock.div_num, div_value - 1u);
        const cyhal_timer_cfg_t cfg = {
            .is_continuous = true,
            .direction = CYHAL_TIMER_DIR_UP,
            .is_compare = true,
            .period = (1 << cy_us_ticker_info.bits) - 1,
            .compare_value = (1 << cy_us_ticker_info.bits) - 1,
            .value = 0u
        };
        if (CY_RSLT_SUCCESS != cyhal_timer_configure(&cy_us_timer, &cfg)) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_timer_set_cfg");
        }
        Cy_TCPWM_ClearInterrupt(cy_us_timer.tcpwm.base, cy_us_timer.tcpwm.resource.channel_num, CY_TCPWM_INT_ON_CC_OR_TC);
        cyhal_timer_register_callback(&cy_us_timer, &cy_us_ticker_irq_handler, NULL);
        if (CY_RSLT_SUCCESS != cyhal_timer_start(&cy_us_timer)) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_timer_start");
        }
        if (!Cy_SysPm_RegisterCallback(&cy_us_ticker_pm_data)) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "Cy_SysPm_RegisterCallback");
        }
        cy_us_ticker_initialized = true;
    }
    us_ticker_disable_interrupt();
}

void us_ticker_free(void)
{
    if (cy_us_ticker_initialized) {
        cy_us_ticker_initialized = false;
        us_ticker_disable_interrupt();
        Cy_SysPm_UnregisterCallback(&cy_us_ticker_pm_data);
        if (CY_RSLT_SUCCESS != cyhal_timer_stop(&cy_us_timer)) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_timer_stop");
        }
        cyhal_timer_free(&cy_us_timer);
    }
}

uint32_t us_ticker_read(void)
{
    MBED_ASSERT(cy_us_ticker_initialized);
    return Cy_TCPWM_Counter_GetCounter(cy_us_timer.tcpwm.base, cy_us_timer.tcpwm.resource.channel_num);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    MBED_ASSERT(cy_us_ticker_initialized);
    Cy_TCPWM_Counter_SetCompare0(cy_us_timer.tcpwm.base, cy_us_timer.tcpwm.resource.channel_num, timestamp);
    if (CY_TCPWM_INT_NONE == Cy_TCPWM_GetInterruptMask(cy_us_timer.tcpwm.base, cy_us_timer.tcpwm.resource.channel_num)) {
        Cy_TCPWM_ClearInterrupt(cy_us_timer.tcpwm.base, cy_us_timer.tcpwm.resource.channel_num, CY_TCPWM_INT_ON_CC_OR_TC);
        cyhal_timer_enable_event(&cy_us_timer, CYHAL_TIMER_IRQ_CAPTURE_COMPARE, CY_US_TICKER_IRQ_PRIORITY, true);
    }
}

void us_ticker_disable_interrupt(void)
{
    MBED_ASSERT(cy_us_ticker_initialized);
    cyhal_timer_enable_event(&cy_us_timer, CYHAL_TIMER_IRQ_CAPTURE_COMPARE, CY_US_TICKER_IRQ_PRIORITY, false);
}

void us_ticker_clear_interrupt(void)
{
    // Nothing to do. cyhal_timer functions clear the interrupt.
}

void us_ticker_fire_interrupt(void)
{
    MBED_ASSERT(cy_us_ticker_initialized);
    // TODO: no HAL function for this. Needs to work even when masked
    IRQn_Type irq = cy_us_timer.tcpwm.resource.block_num == 0
                    ? tcpwm_0_interrupts_0_IRQn : tcpwm_1_interrupts_0_IRQn;
    irq = (IRQn_Type)(irq + cy_us_timer.tcpwm.resource.channel_num);
    NVIC_SetPendingIRQ(irq);
}

const ticker_info_t *us_ticker_get_info(void)
{
    MBED_ASSERT(cy_us_ticker_initialized);
    return &cy_us_ticker_info;
}

#ifdef __cplusplus
}
#endif
