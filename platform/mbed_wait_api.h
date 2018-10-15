
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_wait_api wait_api functions
 * @{
 */

/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
 *    usec precision upto 10msec. If delay is larger then 10msec and not in ISR, it is same as
 *    `wait_ms`. `wait_us` and `wait_ms` are recommended over `wait`
 */
void wait(float s);

/** Waits a number of milliseconds.
 *
 *  @param ms the whole number of milliseconds to wait
 *
 *  @note
 *    If the RTOS is present, it is same as CMSIS osDelay()
 *    Not callable from interrupts, doesn't lock hardware sleep.
 */
void wait_ms(int ms);

/** Waits a number of microseconds.
 *
 *  @param us the whole number of microseconds to wait
 *
 *  @note
 *    This function always spins to get the exact number of microseconds.
 *    If RTOS is present, this will affect power (prevents deep sleep) and
 *    multithread performance, hence spinning for milli-sec wait is not recommended.
 */
void wait_us(int us);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
/** @}*/
