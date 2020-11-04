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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#if DEVICE_WATCHDOG

#include <string.h>
#include <stdio.h>
#include "cmsis.h"
#include "watchdog_api.h"


static uint32_t wdt_timeout_reload_ms = 0;

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    /* Check validity of arguments */
    if (!config || !config->timeout_ms) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    wdt_timeout_reload_ms = config->timeout_ms;

    bp6a_wdog_init(wdt_timeout_reload_ms);

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    bp6a_wdog_set_reload_time(wdt_timeout_reload_ms);
}

watchdog_status_t hal_watchdog_stop(void)
{
    bp6a_watchdog_enable(false);
    bp6a_wdog_reset_enable(false);
    return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    return wdt_timeout_reload_ms;
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t wdt_feat;
    uint32_t wdog_clock = bp6a_cmu_get_clock_freq(CMU_WATCHDOG_APBCLK);
    /* We can support timeout values between 1 and UINT32_MAX by cascading. */
    wdt_feat.max_timeout = (uint32_t)(UINT32_MAX / wdog_clock);
    /* Support re-configuring watchdog timer */
    wdt_feat.update_config = 1;
    /* Support stopping watchdog timer */
    wdt_feat.disable_watchdog = 1;
    wdt_feat.clock_typical_frequency = wdog_clock;
    wdt_feat.clock_max_frequency = wdog_clock * 1.1;

    return wdt_feat;
}
#endif
