/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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
#include "watchdog_api.h"

#if DEVICE_WATCHDOG

#include "reset_reason_api.h"
#include "fsl_rtwdog.h"
#include "fsl_clock.h"
#include "platform/mbed_wait_api.h"

// Platform specific watchdog definitions
#define LPO_CLOCK_FREQUENCY 32768
#define MAX_TIMEOUT         0xFFFFUL
#define DELAY_TIME          100000U

// Number of decrements in the timeout register per millisecond
#define TICKS_PER_MS ((LPO_CLOCK_FREQUENCY) / 1000)

// Maximum timeout that can be specified in milliseconds
#define MAX_TIMEOUT_MS ((MAX_TIMEOUT) / (TICKS_PER_MS))

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    uint32_t temp;
    rtwdog_config_t cfg;

    /* When system is boot up, WDOG32 is disabled. We must wait for at least 2.5
     * periods of wdog32 clock to reconfigure wodg32. So Delay a while to wait for
     * the previous configuration taking effect. */
    for (temp = 0; temp < DELAY_TIME; temp++) {
        __NOP();
    }

    RTWDOG_GetDefaultConfig(&cfg);

    cfg.workMode.enableStop = true;
    cfg.workMode.enableDebug = true;
    cfg.timeoutValue = (TICKS_PER_MS * config->timeout_ms);

    RTWDOG_Init(RTWDOG, &cfg);

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    RTWDOG_Refresh(RTWDOG);
}

watchdog_status_t hal_watchdog_stop(void)
{
    RTWDOG_Deinit(RTWDOG);

    return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    const uint32_t timeout = RTWDOG->TOVAL;

    return (timeout / TICKS_PER_MS);
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features;
    features.max_timeout = MAX_TIMEOUT_MS;
    features.update_config = true;
    features.disable_watchdog = true;
    features.clock_typical_frequency = 32000;
    features.clock_max_frequency = 32768;

    return features;
}

#endif // DEVICE_WATCHDOG
