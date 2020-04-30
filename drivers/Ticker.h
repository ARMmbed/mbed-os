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
#ifndef MBED_TICKER_H
#define MBED_TICKER_H

#include <chrono>
#include <mstd_utility>
#include "drivers/TickerDataClock.h"
#include "drivers/TimerEvent.h"
#include "platform/Callback.h"
#include "platform/mbed_toolchain.h"
#include "platform/NonCopyable.h"
#include "hal/lp_ticker_api.h"

namespace mbed {

/**
 * \defgroup drivers_Ticker Ticker class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/** A Ticker is used to call a function at a recurring interval
 *
 *  You can use as many separate Ticker objects as you require.
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Toggle the blinking LED after 5 seconds
 *
 * #include "mbed.h"
 * using namespace std::chrono;
 *
 * Ticker timer;
 * DigitalOut led1(LED1);
 * DigitalOut led2(LED2);
 *
 * int flip = 0;
 *
 * void attime() {
 *     flip = !flip;
 * }
 *
 * int main() {
 *     timer.attach(&attime, 5us);
 *     while(1) {
 *         if(flip == 0) {
 *             led1 = !led1;
 *         } else {
 *             led2 = !led2;
 *         }
 *         ThisThread::sleep_for(200ms);
 *     }
 * }
 * @endcode
 */
class TickerBase : public TimerEvent, private NonCopyable<TickerBase> {
public:
    /** Attach a function to be called by the Ticker, specifying the interval in seconds
     *
     *  The method forwards its arguments to attach_us() rather than copying them which
     *  may not be trivial depending on the callback copied.
     *  The function is forcibly inlined to not use floating-point operations. This is
     *  possible given attach_us() expects an integer value for the callback interval.
     *  @param func pointer to the function to be called
     *  @param t the time between calls in seconds
     *  @deprecated Pass a chrono duration, not a float second count. For example use `10ms` rather than `0.01f`.
     */
#if defined(__ICCARM__)
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Pass a chrono duration, not a float second count. For example use `10ms` rather than `0.01f`.")
    MBED_FORCEINLINE template <typename F>
#else
    template <typename F> MBED_FORCEINLINE
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Pass a chrono duration, not a float second count. For example use `10ms` rather than `0.01f`.")
#endif
    void attach(F &&func, float t)
    {
        auto float_interval = std::chrono::duration<float>(t);
        attach(std::forward<F>(func), std::chrono::duration_cast<std::chrono::microseconds>(float_interval));
    }

    /** Attach a function to be called by the Ticker, specifying the interval in microseconds
     *
     *  @param func pointer to the function to be called
     *  @param t the time between calls in micro-seconds
     *
     *  @note setting @a t to a value shorter than it takes to process the ticker callback
     *  causes the system to hang. Ticker callback is called constantly with no time
     *  for threads scheduling.
     *  @deprecated Pass a chrono duration, not an integer microsecond count. For example use `10ms` rather than `10000`.
     *
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Pass a chrono duration, not an integer microsecond count. For example use `10ms` rather than `10000`.")
    void attach_us(Callback<void()> func, us_timestamp_t t);

    /** Attach a function to be called by the Ticker, specifying the interval in microseconds
     *
     *  @param func pointer to the function to be called
     *  @param t the time between calls in micro-seconds
     *
     *  @note setting @a t to a value shorter than it takes to process the ticker callback
     *  causes the system to hang. Ticker callback is called constantly with no time
     *  for threads scheduling.
     *
     */
    void attach(Callback<void()> func, std::chrono::microseconds t);

    /** Detach the function
     */
    void detach();

#if !defined(DOXYGEN_ONLY)
protected:
    TickerBase(const ticker_data_t *data);
    TickerBase(const ticker_data_t *data, bool lock_deepsleep);

    ~TickerBase()
    {
        detach();
    }

    /** Attach a function to be called by the Ticker, specifying the absolute call time
     *
     *  If used, handler must be overridden, as TickerBase::handler would attempt
     *  to reschedule. This is done by `TimeoutBase` (used by `Timeout` and `LowPowerTimeout`).
     *
     *  @param func pointer to the function to be called
     *  @param abs_time the time for the call
     *
     *  @note setting @a abs_time to a time in the past means the event will be scheduled immediately
     *  resulting in an instant call to the function.
     */
    void attach_absolute(Callback<void()> func, TickerDataClock::time_point abs_time);

    void handler() override;
    std::chrono::microseconds  _delay{0};  /**< Time delay (in microseconds) for resetting the multishot callback. */
    Callback<void()>    _function;  /**< Callback. */
    bool          _lock_deepsleep;  /**< Flag which indicates if deep sleep should be disabled. */
#endif
private:
    void setup(std::chrono::microseconds t);
    void setup_absolute(TickerDataClock::time_point t);
};

class Ticker : public TickerBase {
public:
    Ticker();
};
/** @}*/

} // namespace mbed

#endif
