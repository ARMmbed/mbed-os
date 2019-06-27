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
#ifdef DEVICE_WATCHDOG

#include "drivers/Watchdog.h"
#include "drivers/WatchdogManager.h"

#define MS_TO_US(x) ((x) * 1000) //macro to convert millisecond to microsecond

namespace mbed {

Watchdog::Watchdog() : _running(false), _callback(NULL)
{
}

Watchdog::~Watchdog()
{
}

bool Watchdog::start(Callback<void()> func, uint32_t timeout)
{
    _callback = func;
    watchdog_status_t sts;
    MBED_ASSERT(MBED_CONF_TARGET_WATCHDOG_TIMEOUT < get_max_timeout());
    core_util_critical_section_enter();
    if (_running) {
        core_util_critical_section_exit();
        return false;
    }
    watchdog_config_t config;
    config.timeout_ms = MBED_CONF_TARGET_WATCHDOG_TIMEOUT;
    sts = hal_watchdog_init(&config);
    if (sts == WATCHDOG_STATUS_OK) {
        _running = true;
    }
    core_util_critical_section_exit();
    if (_running) {
        us_timestamp_t ticker_timeout = (MS_TO_US(((timeout <= 0) ? 1 : timeout)));
        _ticker->attach(callback(this, &Watchdog::kick), ticker_timeout);
    }
    return _running;
}


bool Watchdog::stop()
{
    watchdog_status_t sts;
    bool msts = true;
    core_util_critical_section_enter();
    if (_running) {
        sts = hal_watchdog_stop();
        if (sts != WATCHDOG_STATUS_OK) {
            msts = false;
        } else {
            _ticker->detach();
            _running = false;
            _callback = NULL;
        }

    } else {
        msts = false;
    }
    core_util_critical_section_exit();
    return msts;
}

void Watchdog::kick()
{
    core_util_critical_section_enter();
    hal_watchdog_kick();
    core_util_critical_section_exit();

    if (_callback) {
        _callback();
    }
}


bool Watchdog::is_running() const
{
    return _running;
}

uint32_t Watchdog::get_timeout() const
{
    return hal_watchdog_get_reload_value();
}

uint32_t Watchdog::get_max_timeout() const
{
    const watchdog_features_t features = hal_watchdog_get_platform_features();
    return features.max_timeout;
}

} // namespace mbed


#endif // DEVICE_WATCHDOG
