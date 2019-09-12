/* Copyright (c) 2017-2019 ARM Limited
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
 *
 * @section DESCRIPTION
 *
 * LPC1768 Watchdog
 *
 */
#ifdef DEVICE_WATCHDOG

#include "watchdog_api.h"
#include "reset_reason_api.h"
#include "device.h"
#include "mbed_error.h"
#include <stdbool.h>



watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    LPC_WDT->WDCLKSEL = 0x1;                //CLK src to PCLK
    uint32_t clk = SystemCoreClock / 16; // WD has a fixed /4 prescaler, PCLK default is /4
    LPC_WDT->WDTC = ((float)config->timeout_ms )* clk/1000;
    LPC_WDT->WDMOD = 0x3;                   // Enable and Reset
    hal_watchdog_kick();

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    LPC_WDT->WDFEED = 0xAA;
    LPC_WDT->WDFEED = 0x55;
}
watchdog_status_t hal_watchdog_stop(void)
{
    return WATCHDOG_STATUS_NOT_SUPPORTED;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    uint32_t clk = SystemCoreClock / 16; // WD has a fixed /4 prescaler, PCLK default is /4

    return (float)LPC_WDT->WDTC/clk*1000;
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    uint32_t clk = SystemCoreClock / 16; // WD has a fixed /4 prescaler, PCLK default is /4
    watchdog_features_t features;
    features.max_timeout = ((float)INT32_MAX/clk)*1000;
    features.update_config = true;
    features.disable_watchdog = false;
    features.clock_typical_frequency = 4000000;
    features.clock_max_frequency = 4040000;

    return features;
}

#endif // DEVICE_WATCHDOG
