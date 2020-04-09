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
#ifndef MBED_WAIT_API_H
#define MBED_WAIT_API_H

#include "platform/mbed_toolchain.h"
#include "platform/mbed_atomic.h"
#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup platform-public-api */
/** @{*/

/**
 * \defgroup platform_wait_api wait_api functions
 * @{
 */

/** Generic wait functions.
 *
 * These provide simple NOP type wait capabilities.
 *
 * Example:
 * @code
 * #include "mbed.h"
 *
 * // Blinking rate in milliseconds
 * #define BLINKING_RATE_MS   500
 * DigitalOut  led(LED2);
 * InterruptIn button(SW2);
 *
 * void blink_led() {
 *     led = 1;
 *     wait_us(BLINKING_RATE_MS * 1000);
 *     led = 0;
 * }
 *
 * int main() {
 *     button.fall(&blink_led);
 *     while(1) {
 *         // Do nothing
 *     }
 * }
 * @endcode
 */

/** Waits a number of microseconds.
 *
 *  @param us the whole number of microseconds to wait
 *
 *  @note
 *    This function always spins to get the exact number of microseconds.
 *    This will affect power and multithread performance. Therefore, spinning for
 *    millisecond wait is not recommended, and ThisThread::sleep_for should
 *    be used instead.
 *
 *  @note You may call this function from ISR context, but large delays may
 *    impact system stability - interrupt handlers should take less than
 *    50us.
 */
void wait_us(int us);

/** Waits a number of nanoseconds.
 *
 * This function spins the CPU to produce a small delay. It should normally
 * only be used for delays of 10us (10000ns) or less. As it is calculated
 * based on the expected execution time of a software loop, it may well run
 * slower than requested based on activity from other threads and interrupts.
 * If greater precision is required, this can be called from inside a critical
 * section.
 *
 *  @param ns the number of nanoseconds to wait
 *
 *  @note
 *    wait_us() will likely give more precise time than wait_ns for large-enough
 *    delays, as it is based on a timer, but its set-up time may be excessive
 *    for the smallest microsecond counts, at which point wait_ns() is better.
 *
 *  @note
 *    Any delay larger than a millisecond (1000000ns) is liable to cause
 *    overflow in the internal loop calculation. You shouldn't normally be
 *    using this for such large delays anyway in real code, but be aware if
 *    calibrating. Make repeated calls for longer test runs.
 *
 *  @note You may call this function from ISR context.
 *
 */
void wait_ns(unsigned int ns);

/* Optimize if we know the rate */
#if DEVICE_USTICKER && defined US_TICKER_PERIOD_NUM
void _wait_us_ticks(uint32_t ticks);
void _wait_us_generic(unsigned int us);

/* Further optimization if we know us_ticker is always running */
#if MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT
#define _us_ticker_is_initialized true
#else
extern bool _us_ticker_initialized;
#define _us_ticker_is_initialized core_util_atomic_load_bool(&_us_ticker_initialized)
#endif

#if US_TICKER_PERIOD_DEN == 1 && (US_TICKER_MASK * US_TICKER_PERIOD_NUM) >= 0xFFFFFFFF
/* Ticker is wide and slow enough to have full 32-bit range - can always use it directly */
#define _us_is_small_enough(us) true
#else
/* Threshold is determined by specification of us_ticker_api.h - smallest possible
 * time range for the us_ticker is 16-bit 8MHz, which gives 8192us. This also leaves
 * headroom for the multiplication in 32 bits.
 */
#define _us_is_small_enough(us) ((us) < 8192)
#endif

/* Speed optimisation for small wait_us. Care taken to preserve binary compatibility */
inline void _wait_us_inline(unsigned int us)
{
    /* Threshold is determined by specification of us_ticker_api.h - smallest possible
     * time range for the us_ticker is 16-bit 8MHz, which gives 8192us. This also leaves
     * headroom for the multiplication in 32 bits.
     */
    if (_us_is_small_enough(us) && _us_ticker_is_initialized) {
        const uint32_t ticks = ((us * US_TICKER_PERIOD_DEN) + US_TICKER_PERIOD_NUM - 1) / US_TICKER_PERIOD_NUM;
        _wait_us_ticks(ticks);
    } else {
        _wait_us_generic(us);
    }
}

#define wait_us(us) _wait_us_inline(us)
#endif // Known-rate, initialised timer

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
/** @}*/
