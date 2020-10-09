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
#ifndef MBED_HIGHRESCLOCK_H
#define MBED_HIGHRESCLOCK_H

#include <chrono>
#include "hal/us_ticker_api.h"
#include "platform/mbed_power_mgmt.h"

namespace mbed {
/**
 * \defgroup drivers_HighResClock HighResClock class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/**
 * A C++11 Clock representing the HAL us_ticker.
 *
 * The high resolution clock will pause whenever deep sleep is entered.
 *
 * Lock and unlock methods are provided to control deep sleep to keep the
 * clock running. LowPowerClock provides the same methods as dummies to support
 * generic code that works with either clock.
 *
 * Locks and unlocks are passed to the deep sleep manager, so are reference counted.
 *
 * The lock and unlock methods mean this is a C++11 BasicLockable class,
 * so scoped_lock<HighResClock> or unique_lock<HighResClock> could be used to
 * manage ownership and guarantee balanced unlocking.
 *
 * @note To avoid the need to worry about locking, consider using the Timer, Ticker or
 *       Timeout classes, which lock automatically and only whenever they are active
 */
class HighResClock {
public:
    /* duration is C++11 standard microseconds, so representation will be 64-bit signed integer */
    using duration = std::chrono::microseconds;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<HighResClock>;
    static const bool is_steady = false;

    /** Read the current time */
    static time_point now()
    {
        return time_point{duration{ticker_read_us(get_us_ticker_data())}};
    }

    /** Lock the clock to ensure it stays running */
    static void lock()
    {
        sleep_manager_lock_deep_sleep();
    }

    /** Unlock the clock, allowing it to stop during power saving */
    static void unlock()
    {
        sleep_manager_unlock_deep_sleep();
    }
};

/** @}*/

}
#endif /* MBED_HIGHRESCLOCK_H */
