/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_LOWPOWERCLOCK_H
#define MBED_LOWPOWERCLOCK_H

#include <chrono>
#if DEVICE_LPTICKER
#include "hal/lp_ticker_api.h"

namespace mbed {
/**
 * \defgroup drivers_LowPowerClock LowPowerClock class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/**
 * A C++11 Clock representing the HAL lp_ticker.
 *
 * Dummy lock/unlock methods are provided to have the same generic API
 * as UnlockedHighPowerClock. No action is required as the lp_ticker
 * runs continuously.
 */
class LowPowerClock {
public:
    /* duration is C++11 standard microseconds, so representation will be 64-bit signed integer */
    using duration = std::chrono::microseconds;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<LowPowerClock>;
    static const bool is_steady = true;

    /** Read the current time */
    static time_point now()
    {
        return time_point{duration{ticker_read_us(get_lp_ticker_data())}};
    }

    /** Lock the clock to ensure it stays running; dummy for API compatibility with HighResClock */
    static void lock()
    {
    }

    /** Unlock the clock, allowing it to stop during power saving; dummy for API compatibility with HighResClock */
    static void unlock()
    {
    }
};

/** @}*/

}
#endif /* DEVICE_LPTICKER */
#endif /* MBED_LOWPOWERCLOCK_H */
