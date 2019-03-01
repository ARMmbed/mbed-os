
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_wait_api wait_api functions
 * @{
 */

/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifdef __cplusplus
extern "C" {
#endif

/** Generic wait functions.
 *
 * These provide simple NOP type wait capabilities.
 *
 * Example:
 * @code
 * #include "mbed.h"
 *
 * DigitalOut heartbeat(LED1);
 *
 * int main() {
 *     while (1) {
 *         heartbeat = 1;
 *         wait(0.5);
 *         heartbeat = 0;
 *         wait(0.5);
 *     }
 * }
 * @endcode
 */

/** Waits for a number of seconds, with microsecond resolution (within
 *  the accuracy of single precision floating point).
 *
 *  @param s number of seconds to wait
 *
 *  @note
 *    If the RTOS is present, this function spins to get the exact number of microseconds for
 *    microsecond precision up to 10 milliseconds. If delay is larger than 10 milliseconds and not in ISR, it is the same as
 *    `wait_ms`. We recommend `wait_us` and `wait_ms` over `wait`.
 */
void wait(float s);

/** Waits a number of milliseconds.
 *
 *  @param ms the whole number of milliseconds to wait
 *
 *  @note
 *    If the RTOS is present, it calls ThisThread::sleep_for(), which is same as CMSIS osDelay().
 *    You can't call this from interrupts, and it doesn't lock hardware sleep.
 */
void wait_ms(int ms);

/** Waits a number of microseconds.
 *
 *  @param us the whole number of microseconds to wait
 *
 *  @note
 *    This function always spins to get the exact number of microseconds.
 *    This will affect power and multithread performance. Therefore, spinning for
 *    millisecond wait is not recommended, and wait_ms() should
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

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
/** @}*/
