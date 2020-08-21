/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdbool.h>
#include "s1sbp6a.h"
#include "s1sbp6a_type.h"
#include "s1sbp6a_wdog.h"
#include "s1sbp6a_cmu.h"

#define WATCHDOG_RESET_ENABLED  1


static void bp6a_watchdog_lock(bool enable)
{
    putreg32(&(BP_WATCHDOG->LOCK), enable ? 0 :  0x1ACCE551);
}

void bp6a_watchdog_enable(bool enable)
{
    bp6a_watchdog_lock(false);
    modifyreg32(&(BP_WATCHDOG->CTRL), WDOGCTRL_INT_ENABLE_MASK,
                WDOGCTRL_INT_ENABLE(enable));
    bp6a_watchdog_lock(true);
}

void bp6a_wdog_reset_enable(bool enable)
{
    bp6a_watchdog_lock(false);
    modifyreg32(&(BP_WATCHDOG->CTRL), WDOGCTRL_RESET_ENABLE_MASK,
                WDOGCTRL_RESET_ENABLE(enable));
    bp6a_watchdog_lock(true);
}

void bp6a_wdog_set_reload_time(uint32_t ms)
{
    uint32_t clk = bp6a_cmu_get_clock_freq(CMU_WATCHDOG_APBCLK);
    uint32_t load_val = (uint32_t)((ms * clk) / 1000 + 0.5);

    bp6a_watchdog_lock(false);
    putreg32(&(BP_WATCHDOG->LOAD), load_val);
    bp6a_watchdog_lock(true);
}

void bp6a_nmi_handler(void)
{
    if (getreg32(&(BP_WATCHDOG->CTRL)) & WDOGCTRL_INT_ENABLE_MASK) {
        if (getreg32(&(BP_WATCHDOG->MIS))) {
            putreg32(&BP_WATCHDOG->INTCLR, 0x1);
        }
    }
}

void bp6a_wdog_init(uint32_t timeout)
{
    bp6a_watchdog_enable(false);

    NVIC_SetVector(NonMaskableInt_IRQn, (uint32_t) bp6a_nmi_handler);
#if defined (__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    SCB_InvalidateICache();
#endif

#if WATCHDOG_RESET_ENABLED
    bp6a_wdog_reset_enable(true);
#endif
    bp6a_wdog_set_reload_time(timeout);

    bp6a_watchdog_enable(true);
}
