/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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

#include "reset_reason_api.h"

#ifdef DEVICE_WATCHDOG

#include "device.h"

#include <stdbool.h>

// Platform specific watchdog definitions
#define LPO_CLOCK_FREQUENCY 40000
#define MAX_PRESCALER       256
#define MAX_TIMEOUT         0xFFFULL

// Number of decrements in the timeout register per millisecond
#define TICKS_PER_MS (LPO_CLOCK_FREQUENCY / 1000)
// Maximum timeout that can be specified in milliseconds
const uint64_t max_timeout_ms = ((MAX_TIMEOUT / TICKS_PER_MS) * MAX_PRESCALER);

// Maximum supported watchdog timeout for given prescaler value
#define CALCULATE_MAX_TIMEOUT_MS(scale) \
   ((MAX_TIMEOUT / TICKS_PER_MS) * scale)


static uint32_t calculate_prescaler_value(const uint32_t timeout_ms)
{
    if (timeout_ms > max_timeout_ms) {
        return 0;
    }

    for (uint32_t scale = 0; scale < 7; ++scale) {
        const uint32_t prescaler = (4U << scale);

        if (timeout_ms < CALCULATE_MAX_TIMEOUT_MS(prescaler)) {
            return scale;
        }
    }

    return 0;
}


watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    const uint32_t prescaler = calculate_prescaler_value(config->timeout_ms);

    if (prescaler == 0) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    // Enable write access to Prescaler(IWDG_PR) and Reload(IWDG_RLR) registers
    IWDG->KR  = 0x5555;

    // Set the prescaler and timeout values
    IWDG->RLR = (TICKS_PER_MS * config->timeout_ms) / (4U << prescaler);
    IWDG->PR  = prescaler;

    // Reload the Watchdog Counter.
    IWDG->KR = 0xAAAA;
    // Enable the Independent Watchdog.
    IWDG->KR = 0xCCCC;

    return WATCHDOG_STATUS_OK;
}


void hal_watchdog_kick(void)
{
    IWDG->KR = 0xAAAA;
}


watchdog_status_t hal_watchdog_stop(void)
{
    return WATCHDOG_STATUS_NOT_SUPPORTED;
}


uint32_t hal_watchdog_get_reload_value(void)
{
    const uint32_t timeout   = (IWDG->RLR & 0xFFF);
    const uint32_t prescaler = (4U << (IWDG->PR & 0x7));

    return ((timeout / TICKS_PER_MS) * prescaler);
}


watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features;
    features.max_timeout = max_timeout_ms;
    features.update_config = true;
    features.disable_watchdog = false;

    return features;
}

#endif // DEVICE_WATCHDOG
