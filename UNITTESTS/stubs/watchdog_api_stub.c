/*
 * Copyright (c) 2018-2019 Arm Limited and affiliates.
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

#if DEVICE_WATCHDOG

static uint32_t _timeout = 0;

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    _timeout = config->timeout_ms;
    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{

}

watchdog_status_t hal_watchdog_stop(void)
{
    return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    return _timeout;
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features;
    features.max_timeout = 0xFFFFFFFF;
    return features;
}

#endif // DEVICE_WATCHDOG
