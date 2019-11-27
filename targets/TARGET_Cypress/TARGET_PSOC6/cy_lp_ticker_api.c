/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#include "lp_ticker_api.h"
#include "mbed_error.h"
#include "cyhal_lptimer.h"
#include "cycfg.h"

#if DEVICE_LPTICKER

#ifdef __cplusplus
extern "C" {
#endif

static cyhal_lptimer_t cy_lptimer0;
static bool cy_lptimer_initialized = false;

static void cy_lp_ticker_handler(MBED_UNUSED void *unused1, MBED_UNUSED cyhal_lptimer_event_t unused2)
{
    lp_ticker_irq_handler();
}

void lp_ticker_init(void)
{
    // It should be safe to call this function more than once
    if (!cy_lptimer_initialized) {
        if (CY_RSLT_SUCCESS != cyhal_lptimer_init(&cy_lptimer0)) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_lptimer_init");
        }
        cy_lptimer_initialized = true;
    }
    lp_ticker_disable_interrupt();
    cyhal_lptimer_register_callback(&cy_lptimer0, &cy_lp_ticker_handler, NULL);
}

void lp_ticker_free(void)
{
    cyhal_lptimer_free(&cy_lptimer0);
    cy_lptimer_initialized = false;
}

uint32_t lp_ticker_read(void)
{
    return cyhal_lptimer_read(&cy_lptimer0);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    if (CY_RSLT_SUCCESS != cyhal_lptimer_set_match(&cy_lptimer0, (uint32_t)timestamp)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_lptimer_set_match");
    }
}

void lp_ticker_disable_interrupt(void)
{
    cyhal_lptimer_enable_event(&cy_lptimer0, CYHAL_LPTIMER_COMPARE_MATCH, CYHAL_ISR_PRIORITY_DEFAULT, false);
}

void lp_ticker_clear_interrupt(void)
{
    // LPTIMER driver does this automatically; nothing to do
}

void lp_ticker_fire_interrupt(void)
{
    cyhal_lptimer_irq_trigger(&cy_lptimer0);
}

const ticker_info_t *lp_ticker_get_info(void)
{
    static const ticker_info_t info = {
        /* .frequency = */ CY_CFG_SYSCLK_CLKLF_FREQ_HZ,
        /* .bits =      */ 32
    };
    return &info;
}

#ifdef __cplusplus
}
#endif

#endif
