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

#include "Watchdog.h"
#include "VirtualWatchdog.h"

#define MS_TO_US(x) ((x) * 1000) //macro to convert millisecond to microsecond

namespace mbed {

#if DEVICE_LPTICKER
    /** Create singleton instance of LowPowerTicker for watchdog periodic call back of kick.
     */
    SingletonPtr<LowPowerTicker> Watchdog::_ticker;
#else
    /** Create singleton instance of Ticker for watchdog periodic call back of kick.
     */
    SingletonPtr<Ticker> Watchdog::_ticker;
#endif

bool Watchdog::_running = false;
static const uint32_t elapsed_ms = MBED_CONF_TARGET_WATCHDOG_TIMEOUT / 2;

bool Watchdog::start()
{
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
        us_timestamp_t timeout = (MS_TO_US(((elapsed_ms <= 0) ? 1 : elapsed_ms)));
        _ticker->attach_us(callback(&Watchdog::kick), timeout);
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
    // VirtualWatchdog will access the watchdog process method to verify
    // all registered users/threads in alive state */
    VirtualWatchdog::process(((elapsed_ms <= 0) ? 1 : elapsed_ms));
    core_util_critical_section_exit();
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
