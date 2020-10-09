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
#ifndef MBED_TICKERDATACLOCK_H
#define MBED_TICKERDATACLOCK_H

#include <chrono>
#include "hal/ticker_api.h"

namespace mbed {
/**
 * \defgroup drivers_TickerDataClock TickerDataClock class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/**
 * A partial implementation of a C++11 Clock representing a HAL ticker.
 *
 * This class allows us to create chrono time_points for objects like Timer,
 * with the limitation that the tickers are not singletons. This means:
 *
 * * the now() function is not static - this will limit
 *   use with some algorithms,
 * * there is no distinction between time_points for different
 *   tickers
 *
 * This "pseudo-Clock" approach has been endorsed by Howard Hinnant
 * (designer of Chrono) here:
 * https://stackoverflow.com/questions/56400313/why-does-the-c-standard-require-the-clocknow-function-to-be-static
 *
 * TickerDataClock::time_point values should only be used with mbed APIs specifically taking
 * them, not passed to generic templated chrono algorithms, and it is up to the user to use them
 * in conjunction with the correct TickerDataClock.
 *
 * operators for `->` and conversion to `ticker_data_t *` are provided allowing
 * TickerDataClock to be easily substituted in place of a `ticker_data_t *`.
 */
class TickerDataClock {
public:
    /** Construct a TickerDataClock referring to a ticker_data_t */
    constexpr TickerDataClock(const ticker_data_t *ticker) : _ticker(ticker)
    {
    }

    /* duration is C++11 standard microseconds, so representation will be 64-bit signed integer */
    using duration = std::chrono::microseconds;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<TickerDataClock>;
    static const bool is_steady = false;

    /** Initialize a ticker and set the event handler
     *
     * @param handler A handler to be set
     */
    void set_handler(ticker_event_handler handler)
    {
        return ticker_set_handler(_ticker, handler);
    }

    /** Remove an event from the queue
     *
     * @param obj  The event object to be removed from the queue
     */
    void remove_event(ticker_event_t *obj)
    {
        ticker_remove_event(_ticker, obj);
    }

    /** Insert an event to the queue
     *
     * The event will be executed in timestamp - ticker_read_us() us.
     *
     * @note If an event is inserted with a timestamp less than the current
     * timestamp then the event will be scheduled immediately resulting in
     * an instant call to event handler.
     *
     * @param obj       The event object to be inserted to the queue
     * @param timestamp The event's timestamp
     * @param id        The event object
     */
    void insert_event(ticker_event_t *obj, time_point timestamp, uint32_t id)
    {
        ticker_insert_event_us(_ticker, obj, timestamp.time_since_epoch().count(), id);
    }

    /** Read the current (absolute) ticker's timestamp
     *
     * @warning Return an absolute timestamp counting from the initialization of the
     * ticker.
     *
     * @return The current timestamp
     */
    time_point now() const
    {
        return time_point(duration(ticker_read_us(_ticker)));
    }

    /** Read the next event's timestamp
     *
     * @param timestamp     The timestamp object.
     * @return 1 if timestamp is pending event, 0 if there's no event pending
     */
    int get_next_timestamp(time_point *timestamp) const
    {
        us_timestamp_t t;
        int ret = ticker_get_next_timestamp_us(_ticker, &t);
        *timestamp = time_point(duration(t));
        return ret;
    }

    /** Suspend this ticker
     *
     * When suspended reads will always return the same time and no
     * events will be dispatched. When suspended the common layer
     * will only ever call the interface function clear_interrupt()
     * and that is only if ticker_irq_handler is called.
     *
     */
    void suspend()
    {
        ticker_suspend(_ticker);
    }

    /** Resume this ticker
     *
     * When resumed the ticker will ignore any time that has passed
     * and continue counting up where it left off.
     */
    void resume()
    {
        ticker_resume(_ticker);
    }

    constexpr const ticker_data_t *operator->() const
    {
        return _ticker;
    }
    constexpr operator const ticker_data_t *() const
    {
        return _ticker;
    }

private:
    const ticker_data_t *const _ticker;
};

inline TickerDataClock::time_point get_time_point(const ticker_event_t &event)
{
    return TickerDataClock::time_point{TickerDataClock::duration{event.timestamp}};
}

/** @}*/

}
#endif /* MBED_TICKERDATACLOCK_H */
