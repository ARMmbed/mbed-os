/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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
#ifndef MBED_THREAD_H
#define MBED_THREAD_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup mbed_thread Mbed Thread
 * \ingroup platform-public-api
 * @{
 */

/** Generic thread functions.
 *
 * These are C versions of functions provided in C++ via rtos::Thread and rtos::ThisThread
 */

/** Read the current RTOS kernel millisecond tick count.
     The tick count corresponds to the tick count the RTOS uses for timing
     purposes. It increments monotonically from 0 at boot, so it effectively
     never wraps. If the underlying RTOS only provides a 32-bit tick count,
     this method expands it to 64 bits.
     @return  RTOS kernel current tick count
     @note Mbed OS always uses millisecond RTOS ticks, and this could only wrap
           after half a billion years.
     @note In a non-RTOS build, this computes an equivalent time in milliseconds,
           based on a HAL timer. The time may be referenced as 0 on first call.
     @note You cannot call this function from ISR context.
    @note The equivalent functionality is accessible in C++ via rtos::Kernel::get_ms_count.
 */
uint64_t get_ms_count(void);

/** Sleep for a specified time period in millisec:
  @param   millisec  time delay value
  @note You cannot call this function from ISR context.
  @note The equivalent functionality is accessible in C++ via rtos::ThisThread::sleep_for.
*/
void thread_sleep_for(uint32_t millisec);

/** Sleep until a specified time in millisec
  The specified time is according to Kernel::get_ms_count().
  @param   millisec absolute time in millisec
  @note You cannot call this function from ISR context.
  @note if millisec is equal to or lower than the current tick count, this
        returns immediately.
  @note The equivalent functionality is accessible in C++ via ThisThread::sleep_until.
*/
void thread_sleep_until(uint64_t millisec);

/** @}*/

#ifdef __cplusplus
}
#endif


#endif //MBED_THREAD_H
