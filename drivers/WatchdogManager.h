/*
 * Copyright (c) 2019 Arm Limited and affiliates.
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

#ifndef MBED_WATCHDOG_MANAGER_H
#define MBED_WATCHDOG_MANAGER_H

#include "platform/SingletonPtr.h"
#include "drivers/LowPowerTicker.h"
#include "platform/Callback.h"
#include "hal/ticker_api.h"

#include <cstdio>


namespace mbed {

/** \addtogroup drivers */

class WatchdogManager {
public:
    static void attach(Callback<void()> func, us_timestamp_t timeout);
    static void detach();
    // static uint32_t get_elapsed();
    static const uint32_t elapsed_ms = MBED_CONF_TARGET_WATCHDOG_TIMEOUT / 2;
private:
#if DEVICE_LPTICKER
    /** Create singleton instance of LowPowerTicker for watchdog periodic call back of kick.
     */
    static SingletonPtr<LowPowerTicker> _ticker;
#else
    /** Create singleton instance of Ticker for watchdog periodic call back of kick.
     */
    static SingletonPtr<Ticker> _ticker;
#endif
};

} // namespace mbed

#endif // MBED_WATCHDOG_MANAGER_H
