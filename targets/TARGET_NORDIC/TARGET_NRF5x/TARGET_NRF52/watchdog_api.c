/*
 * Copyright (c) 2019 Trampoline SRL
 * Copyright (c) 2019 Giampaolo Mancini <giampaolo@trampolineup.com>
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

#include "watchdog_api.h"
#include "nrfx_wdt.h"

#if DEVICE_WATCHDOG

static void dummy(void) {
}

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    nrfx_err_t err_code;

    // The nRF watchdogs accept a range from 0xF to maximum 0xFFFF_FFFF
    // 32768 Hz ticks. (Plase, see
    // https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v15.3.0%2Fstructnrfx__wdt__config__t.html)
    // Min timeout values in ms range from 0x1 to (0xFFFFFFFF / 0x8000) * 1000
    if ( config->timeout_ms < 0x1 && config->timeout_ms > 0x07CFFFFF ) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    nrfx_wdt_config_t nrf_cfg;

    nrf_cfg.reload_value = config->timeout_ms;
    nrf_cfg.behaviour = NRF_WDT_BEHAVIOUR_RUN_SLEEP_HALT;
    nrf_cfg.interrupt_priority = NRFX_WDT_CONFIG_NO_IRQ;

    err_code = nrfx_wdt_init(&nrf_cfg, dummy);
    if (err_code != NRFX_SUCCESS) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    nrfx_wdt_channel_id channel;
    if (nrfx_wdt_channel_alloc(&channel) != NRF_SUCCESS) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }
    nrfx_wdt_enable();
    nrfx_wdt_feed();

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    nrfx_wdt_feed();
}

watchdog_status_t hal_watchdog_stop(void)
{
    return WATCHDOG_STATUS_NOT_SUPPORTED;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    // Convert to milliseconds from 32768 Hz clock ticks.
    return (uint64_t)nrf_wdt_reload_value_get() / 32768U * 1000;
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features;

    features.max_timeout = 0x07CFFFFF;
    features.update_config = false;
    features.disable_watchdog = false;
    return features;
}

#endif // DEVICE_WATCHDOG
