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

#include "WatchdogManager.h"

namespace mbed {

#if DEVICE_LPTICKER
    /** Create singleton instance of LowPowerTicker for watchdog periodic call back of kick.
     */
    SingletonPtr<LowPowerTicker> _ticker;
#else
    /** Create singleton instance of Ticker for watchdog periodic call back of kick.
     */
    SingletonPtr<Ticker> _ticker;
#endif

void WatchdogManager::attach(Callback<void()> func, us_timestamp_t timeout)
{
    _ticker->attach_us(func, timeout);
}

void WatchdogManager::detach()
{
    _ticker->detach();
}

} // namespace mbed


#endif // DEVICE_WATCHDOG
