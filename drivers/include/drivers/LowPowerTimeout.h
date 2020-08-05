/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#ifndef MBED_LOWPOWERTIMEOUT_H
#define MBED_LOWPOWERTIMEOUT_H

#include "platform/platform.h"

#if DEVICE_LPTICKER || defined(DOXYGEN_ONLY)

#include "drivers/LowPowerClock.h"
#include "drivers/Timeout.h"

namespace mbed {
/**
 * \defgroup drivers_LowPowerTimeout LowPowerTimeout class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/** Low Power Timout
 *
 * @note Synchronization level: Interrupt safe
 */
class LowPowerTimeout : public TimeoutBase {
public:
    LowPowerTimeout();

    /** Clock to use with attach_absolute, guaranteeing running only while attached or manually locked */
    using clock = LowPowerClock;

    /** Clock to use with attach_absolute, running always */
    using steady_clock = LowPowerClock;

    /** @copydoc TimeoutBase::scheduled_time() */
    LowPowerClock::time_point scheduled_time() const
    {
        /* Massage from virtual TickerDataClock::time_point used internally to true LowPowerClock::time_point */
        return LowPowerClock::time_point{TimeoutBase::scheduled_time().time_since_epoch()};
    }

    /** Attach a function to be called by the Timeout, specifying the absolute time
     *
     *  @param func pointer to the function to be called
     *  @param abs_time the absolute time for the call, referenced to LowPowerClock
     *
     *  @note setting @a abs_time to a time in the past means the event will be scheduled immediately
     *  resulting in an instant call to the function.
     */
    template <class F>
    void attach_absolute(F &&func, LowPowerClock::time_point abs_time)
    {
        /* Massage from true LowPowerClock::time_point to virtual TickerDataClock::time_point used internally */
        TimeoutBase::attach_absolute(std::forward<F>(func), TickerDataClock::time_point{abs_time.time_since_epoch()});
    }
};

/** @}*/

} // namespace mbed

#endif

#endif
