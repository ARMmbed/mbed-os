/* mbed Microcontroller Library
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

#include "mbed_error.h"
#include "watchdog_api.h"
#include "cyhal_wdt.h"

#if DEVICE_WATCHDOG

#ifdef __cplusplus
extern "C" {
#endif

static bool initialized = false;
static cyhal_wdt_t cy_wdt;

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    //MBED can call init multiple times so we need to free before the second init
    if (initialized) {
        cyhal_wdt_free(&cy_wdt);
    }

    cy_rslt_t result = cyhal_wdt_init(&cy_wdt, config->timeout_ms);

    switch (result) {
        case CY_RSLT_SUCCESS:
            initialized = true;
            return WATCHDOG_STATUS_OK;
        case CY_RSLT_WDT_INVALID_TIMEOUT:
            return WATCHDOG_STATUS_INVALID_ARGUMENT;
        default:
            return WATCHDOG_STATUS_NOT_SUPPORTED;
    }
}

void hal_watchdog_kick(void)
{
    cyhal_wdt_kick(&cy_wdt);
}

watchdog_status_t hal_watchdog_stop(void)
{
    cyhal_wdt_stop(&cy_wdt);
    return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    return cyhal_wdt_get_timeout_ms(&cy_wdt);
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features = {
        /* .max_timeout =             */ cyhal_wdt_get_max_timeout_ms(),
        /* .update_config =           */ true,
        /* .disable_watchdog =        */ true,
        /* .clock_typical_frequency = */ 32000,
        /* .clock_max_frequency =     */ 36100
    };
    return features;
}

#ifdef __cplusplus
}
#endif

#endif // DEVICE_WATCHDOG
