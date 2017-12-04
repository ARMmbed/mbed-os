/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifdef DEVICE_WATCHDOG

#include "Watchdog.h"

namespace mbed
{

watchdog_status_t Watchdog::start(const uint32_t timeout)
{
    watchdog_config_t config;
    config.timeout_ms = timeout;

    return hal_watchdog_init(&config);
}


void Watchdog::kick()
{
    hal_watchdog_kick();
}


watchdog_status_t Watchdog::stop()
{
    return hal_watchdog_stop();
}


uint32_t Watchdog::reload_value() const
{
    return hal_watchdog_get_reload_value();
}


uint32_t Watchdog::max_timeout()
{
    const watchdog_features_t features = hal_watchdog_get_platform_features();

    return features.max_timeout;
}

} // namespace mbed

#endif // DEVICE_WATCHDOG
