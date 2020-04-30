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
#ifndef MBED_TIMEOUT_H
#define MBED_TIMEOUT_H

#include "drivers/HighResClock.h"
#include "drivers/Ticker.h"

namespace mbed {
/**
 * \defgroup drivers_Timeout Timeout class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/** A Timeout is used to call a function at a point in the future
 *
 * You can use as many separate Timeout objects as you require.
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Blink until timeout.
 *
 * #include "mbed.h"
 *
 * Timeout timeout;
 * DigitalOut led(LED1);
 *
 * int on = 1;
 *
 * void attimeout() {
 *     on = 0;
 * }
 *
 * int main() {
 *     timeout.attach(&attimeout, 5);
 *     while(on) {
 *         led = !led;
 *         ThisThread::sleep_for(200);
 *     }
 * }
 * @endcode
 */
class TimeoutBase : public TickerBase {
public:
    /** Return time remaining until callback
     *
     * @return time remaining until callback is due
     *
     * @note if the callback is overdue, or has already run, the returned value will be negative
     */
    std::chrono::microseconds remaining_time() const
    {
        return scheduled_time() - _ticker_data.now();
    }

#if !defined(DOXYGEN_ONLY)
protected:
    using TickerBase::TickerBase;
    ~TimeoutBase() = default;

    void handler() final;

    /** Return scheduled callback time
     *
     * @return scheduled callback time
     *
     * @note if the callback is overdue, or has already run, the returned value will be in the past
     */
    TickerDataClock::time_point scheduled_time() const
    {
        return get_time_point(event);
    }
#endif
};

class Timeout : public TimeoutBase {
public:
    Timeout();

    /** Clock to use with attach_absolute, guaranteeing running only while attached or manually locked */
    using clock = HighResClock;

    /** @copydoc TimeoutBase::scheduled_time() */
    HighResClock::time_point scheduled_time() const
    {
        /* Massage from virtual TickerDataClock::time_point used internally to true HighResClock::time_point */
        return HighResClock::time_point{TimeoutBase::scheduled_time().time_since_epoch()};
    }

    /** Attach a function to be called by the Timeout, specifying the absolute time
     *
     *  @param func pointer to the function to be called
     *  @param abs_time the absolute time for the call, referenced to HighResClock
     *
     *  @note setting @a abs_time to a time in the past means the event will be scheduled immediately
     *  resulting in an instant call to the function.
     */
    template <class F>
    void attach_absolute(F &&func, HighResClock::time_point abs_time)
    {
        /* Massage from true HighResClock::time_point to virtual TickerDataClock::time_point used internally */
        TimeoutBase::attach_absolute(std::forward<F>(func), TickerDataClock::time_point{abs_time.time_since_epoch()});
    }
};

/** @}*/

} // namespace mbed

#endif
