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
#ifndef MBED_TIMEREVENT_H
#define MBED_TIMEREVENT_H

#include "hal/ticker_api.h"
#include "platform/mbed_toolchain.h"
#include "platform/NonCopyable.h"
#include "drivers/TickerDataClock.h"

namespace mbed {

/**
 * \defgroup drivers_TimerEvent TimerEvent class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/** Base abstraction for timer interrupts
 *
 * @note Synchronization level: Interrupt safe
 */
class TimerEvent : private NonCopyable<TimerEvent> {
#if !defined(DOXYGEN_ONLY)
protected:
    TimerEvent(const ticker_data_t *data);

    /** The handler registered with the underlying timer interrupt
     *
     *  @param id       Timer Event ID
     */
    static void irq(uint32_t id);

    /** Destruction removes it...
     */
    ~TimerEvent();

    // The handler called to service the timer event of the derived class
    virtual void handler() = 0;

    /** Set absolute timestamp of the internal event.
     * @param   timestamp   event's 32-bit us timestamp
     *
     * @warning
     * Do not insert more than one timestamp.
     * The same @a event object is used for every @a insert/insert_absolute call.
     *
     * @warning
     * Ticker's present time is used for reference. The wrapping of the 32-bit
     * timestamp is handled by assuming that the time specified is always in the
     * future. For reference @see convert_timestamp. This could cause problems
     * under load, as if the requested time is missed, the event will be scheduled
     * one whole 32-bit wrap (over an hour) in the future.
     *
     * @warning
     * Despite previous documentation, this sets an absolute time, based on
     * a wrapping 32-bit timestamp, not a time relative to now. The replacement
     * Chrono-based call is `insert_absolute(TickerDataClock::time_point timestamp)`,
     * not `insert(std::chrono::microseconds rel_time)`. However
     * `insert(5ms)` can be used instead of `insert_absolute(_ticker_data.now() + 5ms)`.
     * Due to unclear documentation, it's possible some users may have been
     * doing `insert(5000)` without adding the current time, and they could be
     * corrected to `insert(5ms)`.
     *
     * @deprecated use `insert_absolute(TickerDataClock::time_point timestamp)`
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Pass a chrono time_point, not an integer microsecond count. For example use `last_chrono_time + 5ms` rather than `last_us_time + 5000`.")
    void insert(timestamp_t timestamp);

    /** Set relative timestamp of the internal event.
     * @param   rel_time   relative time for event's us timestamp
     *
     * @warning
     * Do not insert more than one timestamp.
     * The same @a event object is used for every @a insert/insert_absolute call.
     *
     * @note
     * Ticker's present timestamp is used for reference. If the relative time is
     * non-positive, the event will be run immediately.
     */
    void insert(std::chrono::microseconds rel_time);

    /** Set absolute timestamp of the internal event.
     * @param   timestamp   event's us timestamp
     *
     * @warning
     * Do not insert more than one timestamp.
     * The same @a event object is used for every @a insert/insert_absolute call.
     *
     * @deprecated use `insert_absolute(TickerDataClock::time_point timestamp)`
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Pass a chrono time_point, not an integer microsecond count. For example use `last_chrono_time + 5ms` rather than `last_us_time + 5000`.")
    void insert_absolute(us_timestamp_t timestamp);

    /** Set absolute timestamp of the internal event.
     * @param   timestamp   event's us timestamp
     *
     * @warning
     * Do not insert more than one timestamp.
     * The same @a event object is used for every @a insert/insert_absolute call.
     *
     * @note
     * If the timestamp is in the past, the event will be run immediately.
     */
    void insert_absolute(TickerDataClock::time_point timestamp);

    /** Remove timestamp.
     */
    void remove();

    ticker_event_t event;

    TickerDataClock _ticker_data;
#endif
};

/** @}*/

} // namespace mbed

#endif
