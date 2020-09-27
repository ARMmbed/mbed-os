/*
 * Copyright (c) 2006-2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_MBED_SLEEP_TIMER_H
#define MBED_MBED_SLEEP_TIMER_H

#include <chrono>
#include "platform/internal/SysTimer.h"

#if MBED_CONF_RTOS_PRESENT
extern "C" {
#include "rtx_lib.h"
}
#endif

namespace mbed {
namespace internal {

#if MBED_CONF_RTOS_PRESENT
using OsTimer = SysTimer<std::ratio<1, OS_TICK_FREQ>>;
#else
using OsTimer = SysTimer<std::milli>;
#endif

/* A SysTimer is used to provide the timed sleep - this provides access to share it for
 * other use, such as ticks. If accessed this way, it must not be in use when a sleep function below is called.
 */
extern OsTimer *os_timer;
OsTimer *init_os_timer();

/** A C++11 chrono TrivialClock for os_timer
 *
 * Due to the nature of OsTimer/SysTimer, this does not have a single `now` method, but has
 * multiple ways to report the current state:
 *
 *      High-res timeline -------------------------------------------------------------
 *      Ticks               |  a  |  b  |  b  |  b  |  c  |  c  |  c  |  c  |  c  | d ^
 *                                ^                 ^                             ^   os_timer->get_time()
 *                         acknowledged_ticks()   reported_ticks()               now()
 *
 * (a) is time read from hardware by OsTimer, reported to the user of OsTimer, and acknowledged by that user.
 * (b) is time read from hardware by OsTimer, reported to the user of OsTimer, but not yet acknowledged.
 * (c) is time already elapsed in the hardware but yet to be read and processed as ticks by OsTimer.
 * (d) is time already elapsed in the hardware that doesn't yet form a tick.
 *
 * Time is "reported" either by:
 *   * calls to the OsTimer's handler following start_tick - these must be acknowledged
 *   * the result of OsTimer::update_and_get_tick() / OsClock::now() - calling this implies acknowledgment.
 *
 * As such `now()` is used when the ticker is not in use - it processes ticks that would have been
 * processed by the tick handler. If the ticker is in uses `reported_ticks` or `acknowleged_ticks` must be used.
 *
 * @note To fit better into the chrono framework, OsClock uses
 *       chrono::milliseconds as its representation, which makes it signed
 *       and at least 45 bits, so it will be int64_t or equivalent, unlike
 *       OsTimer which uses uint64_t rep.
 */
struct OsClock {
    /* Standard TrivialClock fields */
    using period = OsTimer::period;
    using rep = std::chrono::milliseconds::rep;
    using duration = std::chrono::duration<rep, period>; // == std::chrono::milliseconds, if period is std::milli
    using time_point = std::chrono::time_point<OsClock, duration>;
    static constexpr bool is_steady = true;
    // Read the hardware, and return the updated time_point.
    // Initialize the timing system if necessary - this could be the first call.
    // See SysTimer::update_and_get_tick for more details.
    static time_point now()
    {
        // We are a real Clock with a well-defined epoch. As such we distinguish ourselves
        // from the less-well-defined SysTimer pseudo-Clock. This means our time_points
        // are not convertible, so need to fiddle here.
        return time_point(init_os_timer()->update_and_get_tick().time_since_epoch());
    }
    // Return the current reported tick count, without update.
    // Assumes timer has already been initialized, as ticker should have been in use to
    // keep that tick count up-to-date. See SysTimer::get_tick for more details.
    static time_point reported_ticks()
    {
        return time_point(os_timer->get_tick().time_since_epoch());
    }
    // Return the acknowledged tick count.
    static time_point acknowledged_ticks()
    {
        return reported_ticks() - os_timer->unacknowledged_ticks();
    }
    // Slightly-optimised variant of OsClock::now() that assumes os_timer is initialised.
    static time_point now_with_init_done()
    {
        return time_point(os_timer->update_and_get_tick().time_since_epoch());
    }
    static void set_wake_time(time_point wake_time)
    {
        return os_timer->set_wake_time(OsTimer::time_point(wake_time.time_since_epoch()));
    }
    /* Extension to
     * make it easy to use 32-bit durations for some APIs, as we historically do */
    using duration_u32 = std::chrono::duration<uint32_t, period>;
};

/* time_point::max() is effectively "sleep forever" */
OsClock::time_point do_timed_sleep_absolute(OsClock::time_point wake_time, bool (*wake_predicate)(void *) = NULL, void *wake_predicate_handle = NULL);

#if MBED_CONF_RTOS_PRESENT
/* Maximum sleep time is 2^32-1 ticks; timer is always set to achieve this */
/* Assumes that ticker has been in use prior to call, so restricted to RTOS use */
OsClock::duration_u32 do_timed_sleep_relative_to_acknowledged_ticks(OsClock::duration_u32 wake_delay, bool (*wake_predicate)(void *) = NULL, void *wake_predicate_handle = NULL);
#else

void do_untimed_sleep(bool (*wake_predicate)(void *), void *wake_predicate_handle = NULL);

/* duration_u32::max() delay is sleep forever */

void do_timed_sleep_relative_or_forever(OsClock::duration_u32 wake_delay, bool (*wake_predicate)(void *) = NULL, void *wake_predicate_handle = NULL);

#endif

}
}

#endif
