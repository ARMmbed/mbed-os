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
#ifndef MBED_REALTIMECLOCK_H
#define MBED_REALTIMECLOCK_H

#include <chrono>
#include "hal/rtc_api.h"

namespace mbed {
/**
 * \defgroup drivers_RealTimeClock RealTimeClock class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/**
 * An implementation of a C++11 Clock representing the HAL real-time clock.
 *
 * This is intended to be usable as a substitute for @c std::chrono::system_clock,
 * but is provided as a separate class, due to inconsistencies in retargetting
 * support between toolchains.
 *
 * This is only a thin wrapper around the HAL RTC API, and as such it requires the
 * same use of initialization.
 *
 */
class RealTimeClock {
public:
    using duration = std::chrono::seconds;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<RealTimeClock>;
    static const bool is_steady = false;

    /** Initialize the RTC peripheral
     *
     * Power up the RTC in preparation for access. This function must be called
     * before any other RealTimeClock methods are called. This does not change the state
     * of the RTC. It just enables access to it.
     *
     * @see ::rtc_init
     */
    static void init()
    {
        return rtc_init();
    }

    /** Deinitialize RTC
     *
     * Powerdown the RTC in preparation for sleep, powerdown or reset. That should only
     * affect the CPU domain and not the time keeping logic.
     * After this function is called no other RealTimeClock methods should be called
     * except for RealTimeClock::init.
     *
     * @see ::rtc_free
     */
    static void free()
    {
        return rtc_free();
    }

    /** Check if the RTC has the time set and is counting
     *
     * @retval false The time reported by the RTC is not valid
     * @retval true  The time has been set and the RTC is counting
     *
     * @see ::rtc_isenabled
     */
    static bool isenabled() noexcept
    {
        return bool(rtc_isenabled());
    }

    /** Get the current time from the RTC peripheral
     *
     * @return The current time in seconds
     *
     * @see ::rtc_read
     */
    static time_point now() noexcept
    {
        return from_time_t(rtc_read());
    }

    /** Write the current time in seconds to the RTC peripheral
     *
     * @param t The current time to be set in seconds.
     *
     * @see ::rtc_write
     */
    static void write(time_point t) noexcept
    {
        rtc_write(to_time_t(t));
    }

    /** Convert a C time_t to C++ Chrono time_point
     *
     * @param t a @c time_t object
     * @return a RealTimeClock::time_point object that represents the same point in time as @p t.
     */
    static time_point from_time_t(time_t t) noexcept
    {
        return time_point{std::chrono::duration_cast<duration>(std::chrono::duration<time_t>{t})};
    }

    /** Convert a C++ Chrono time_point to a C time_t
     *
     * @param t a RealTimeClock::time_point object
     * @return a @c time_t object that represents the same point in time as @p t.
     */
    static time_t to_time_t(const time_point &t) noexcept
    {
        return std::chrono::duration_cast<std::chrono::duration<time_t>>(t.time_since_epoch()).count();
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
#endif /* MBED_TICKERCLOCK_H */
