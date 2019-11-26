/***************************************************************************//**
 * @file watchdog_api.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2018 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#include "watchdog_api.h"

#if DEVICE_WATCHDOG

#include "clocking.h"
#include "em_cmu.h"
#include "em_wdog.h"

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    if (config == 0 || config->timeout_ms > 262145 || config->timeout_ms == 0) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    uint32_t timeout = config->timeout_ms;
    if (timeout <= 9) {
        timeout = 0;
    } else {
        timeout -= 2;
        /* get bit position of highest bit set */
        timeout = 31 - __CLZ(timeout);
        /* need to go one over */
        timeout += 1;
        /* convert to wdog setting. if bit 4 is the highest, then the
         * watchdog setting == 1. That means watchdog setting 0xF == 2^18 = 256k */
        if (timeout > 18) {
            return WATCHDOG_STATUS_INVALID_ARGUMENT;
        }

        timeout -= 3;
    }

    WDOG_Init_TypeDef wdog_init = WDOG_INIT_DEFAULT;
    wdog_init.clkSel = wdogClkSelULFRCO;
    wdog_init.em2Run = true;
    wdog_init.em3Run = true;
    wdog_init.perSel = timeout;

    WDOGn_Init(DEFAULT_WDOG, &wdog_init);

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    WDOGn_Feed(DEFAULT_WDOG);
}

watchdog_status_t hal_watchdog_stop(void)
{
    WDOGn_Enable(DEFAULT_WDOG, false);
    return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    uint32_t period = (DEFAULT_WDOG->CTRL & _WDOG_CTRL_PERSEL_MASK) >> _WDOG_CTRL_PERSEL_SHIFT;
    if (period == 0) {
        return 9;
    } else {
        period += 3;
        return (1U << period) + 1;
    }
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t feat = {
        .max_timeout = 262145,
        .update_config = true,
        .disable_watchdog = true,
    #if defined(TARGET_EFM32GG) || defined(TARGET_EFM32HG) || defined(TARGET_EFM32LG) || defined(TARGET_EFM32WG) || defined(TARGET_EFM32ZG)
        .clock_typical_frequency = 1000,
        .clock_max_frequency = 1750
    #elif defined(TARGET_EFM32GG11)
        .clock_typical_frequency = 1000,
        .clock_max_frequency = 1120
    #elif defined(TARGET_EFM32PG) || defined(TARGET_EFM32PG12) || defined(TARGET_EFR32MG1) || defined(TARGET_EFR32MG12)
        .clock_typical_frequency = 1000,
        .clock_max_frequency = 1070
    #else
    #error "unsupported target" 
    #endif
    };
    return feat;
}

#endif /* DEVICE_WATCHDOG */
