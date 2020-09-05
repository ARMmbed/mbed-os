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
#ifndef MBED_TIMER_H
#define MBED_TIMER_H

#include "platform/platform.h"
#include "drivers/TickerDataClock.h"
#include "platform/NonCopyable.h"

namespace mbed {

class CriticalSectionLock;

/**
 * \defgroup drivers_Timer Timer class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/** A general purpose timer
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Count the time to toggle an LED
 *
 * #include "mbed.h"
 *
 * Timer timer;
 * DigitalOut led(LED1);
 * int begin, end;
 *
 * int main() {
 *     timer.start();
 *     begin = timer.read_us();
 *     led = !led;
 *     end = timer.read_us();
 *     printf("Toggle the led takes %d us", end - begin);
 * }
 * @endcode
 */
class TimerBase {

public:
    /** Start the timer
     */
    void start();

    /** Stop the timer
     */
    void stop();

    /** Reset the timer to 0.
     *
     * If it was already running, it will continue
     */
    void reset();

    /** Get the time passed in seconds
     *
     *  @returns    Time passed in seconds
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Floating point operators should normally be avoided for code size. If really needed, you can use `duration<float>{elapsed_time()}.count()`")
    float read() const;

    /** Get the time passed in milliseconds
     *
     *  @returns    Time passed in milliseconds
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Use the Chrono-based elapsed_time method.  If integer milliseconds are needed, you can use `duration_cast<milliseconds>(elapsed_time()).count()`")
    int read_ms() const;

    /** Get the time passed in microseconds
     *
     *  @returns    Time passed in microseconds
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Use the Chrono-based elapsed_time method.  If integer microseconds are needed, you can use `elapsed_time().count()`")
    int read_us() const;

    /** An operator shorthand for read()
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Floating point operators should normally be avoided for code size. If really needed, you can use `duration<float>{elapsed_time()}.count()`")
    operator float() const;

    /** Get in a high resolution type the time passed in microseconds.
     *  Returns a 64 bit integer.
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Use the Chrono-based elapsed_time method.  If integer microseconds are needed, you can use `elapsed_time().count()`")
    us_timestamp_t read_high_resolution_us() const;

    /** Get in a high resolution type the time passed in microseconds.
     *  Returns a 64 bit integer chrono duration.
     */
    std::chrono::microseconds elapsed_time() const;

#if !defined(DOXYGEN_ONLY)
protected:
    TimerBase(const ticker_data_t *data);
    TimerBase(const ticker_data_t *data, bool lock_deepsleep);
    TimerBase(const TimerBase &t);
    TimerBase(TimerBase &&t);
    ~TimerBase();

    const TimerBase &operator=(const TimerBase &) = delete;

    std::chrono::microseconds slicetime() const;
    TickerDataClock::time_point _start{};   // the start time of the latest slice
    std::chrono::microseconds _time{};    // any accumulated time from previous slices
    TickerDataClock _ticker_data;
    bool _lock_deepsleep;    // flag that indicates if deep sleep should be disabled
    bool _running = false;   // whether the timer is running

private:
    // Copy storage while a lock is held
    TimerBase(const TimerBase &t, const CriticalSectionLock &) : TimerBase(t, false) {}
    // Copy storage only - used by delegating constructors
    TimerBase(const TimerBase &t, bool) : _start(t._start), _time(t._time), _ticker_data(t._ticker_data), _lock_deepsleep(t._lock_deepsleep), _running(t._running) {}
};
#endif

class Timer : public TimerBase {
public:
    Timer();
};
/** @}*/

} // namespace mbed

#endif
