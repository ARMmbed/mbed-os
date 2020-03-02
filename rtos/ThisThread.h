/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef THIS_THREAD_H
#define THIS_THREAD_H

#include <stdint.h>
#include "rtos/mbed_rtos_types.h"

namespace rtos {
/** \addtogroup rtos-public-api */
/** @{*/

/**
 * \defgroup rtos_ThisThread ThisThread namespace
 * @{
 */

/** The ThisThread namespace allows controlling the current thread.
 *
 *  Example:
 *  @code
 *  #include "mbed.h"
 *  #include "rtos.h"
 *
 *  Thread thread;
 *  DigitalOut led1(LED1);
 *
 *  #define STOP_FLAG 1
 *
 *  // Blink function toggles the led in a long running loop
 *  void blink(DigitalOut *led) {
 *      while (!ThisThread::flags_wait_any_for(STOP_FLAG, 1000)) {
 *          *led = !*led;
 *      }
 *  }
 *
 *  // Spawns a thread to run blink for 5 seconds
 *  int main() {
 *      thread.start(callback(blink, &led1));
 *      ThisThread::sleep_for(5000);
 *      thread.signal_set(STOP_FLAG);
 *      thread.join();
 *  }
 *  @endcode
 *
 */
namespace ThisThread {
/** Clears the specified Thread Flags of the currently running thread.
  @param   flags  specifies the flags of the thread that should be cleared.
  @return  thread flags before clearing.
  @note You cannot call this function from ISR context.
  @see Thread::flags_set
*/
uint32_t flags_clear(uint32_t flags);

/** Returns the Thread Flags currently set for the currently running thread.
  @return  current thread flags or 0 if not in a valid thread.
  @note You cannot call this function from ISR context.
  @see Thread::flags_set
*/
uint32_t flags_get();

/** Wait for all of the specified Thread Flags to become signaled for the current thread.
  @param   flags     specifies the flags to wait for
  @param   clear     whether to clear the specified flags after waiting for them. (default: true)
  @return  actual thread flags before clearing, which will satisfy the wait
  @note You cannot call this function from ISR context.
  @see Thread::flags_set
*/
uint32_t flags_wait_all(uint32_t flags, bool clear = true);

/** Wait for any of the specified Thread Flags to become signaled for the current thread.
  @param   flags     specifies the flags to wait for
  @param   clear     whether to clear the specified flags after waiting for them. (default: true)
  @return  actual thread flags before clearing, which will satisfy the wait
  @note You cannot call this function from ISR context.
  @see Thread::flags_set
*/
uint32_t flags_wait_any(uint32_t flags, bool clear = true);

/** Wait for all of the specified Thread Flags to become signaled for the current thread.
  @param   flags     specifies the flags to wait for
  @param   millisec  timeout value.
  @param   clear     whether to clear the specified flags after waiting for them. (default: true)
  @return  actual thread flags before clearing, which may not satisfy the wait
  @note You cannot call this function from ISR context.
  @see Thread::flags_set
*/
uint32_t flags_wait_all_for(uint32_t flags, uint32_t millisec, bool clear = true);

/** Wait for all of the specified Thread Flags to become signaled for the current thread.
  @param   flags     specifies the flags to wait for
  @param   millisec  absolute timeout time, referenced to Kernel::get_ms_count()
  @param   clear     whether to clear the specified flags after waiting for them. (default: true)
  @return  actual thread flags before clearing, which may not satisfy the wait
  @note You cannot call this function from ISR context.
  @note the underlying RTOS may have a limit to the maximum wait time
        due to internal 32-bit computations, but this is guaranteed to work if the
        wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
        the wait will time out earlier than specified.
  @see Thread::flags_set
*/
uint32_t flags_wait_all_until(uint32_t flags, uint64_t millisec, bool clear = true);

/** Wait for any of the specified Thread Flags to become signaled for the current thread.
  @param   flags     specifies the flags to wait for
  @param   millisec  timeout value.
  @param   clear     whether to clear the specified flags after waiting for them. (default: true)
  @return  actual thread flags before clearing, which may not satisfy the wait
  @note You cannot call this function from ISR context.
  @see Thread::flags_set
*/
uint32_t flags_wait_any_for(uint32_t flags, uint32_t millisec, bool clear = true);

/** Wait for any of the specified Thread Flags to become signaled for the current thread.
  @param   flags     specifies the flags to wait for
  @param   millisec  absolute timeout time, referenced to Kernel::get_ms_count()
  @param   clear     whether to clear the specified flags after waiting for them. (default: true)
  @return  actual thread flags before clearing, which may not satisfy the wait
  @note You cannot call this function from ISR context.
  @note the underlying RTOS may have a limit to the maximum wait time
        due to internal 32-bit computations, but this is guaranteed to work if the
          wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
          the wait will time out earlier than specified.
  @see Thread::flags_set
*/
uint32_t flags_wait_any_until(uint32_t flags, uint64_t millisec, bool clear = true);

/** Sleep for a specified time period in millisec:
  @param   millisec  time delay value
  @note You cannot call this function from ISR context.
  @note The equivalent functionality is accessible in C via thread_sleep_for.
*/
void sleep_for(uint32_t millisec);

/** Sleep until a specified time in millisec
  The specified time is according to Kernel::get_ms_count().
  @param   millisec absolute time in millisec
  @note You cannot call this function from ISR context.
  @note if millisec is equal to or lower than the current tick count, this
        returns immediately.
  @note The equivalent functionality is accessible in C via thread_sleep_until.
*/
void sleep_until(uint64_t millisec);

/** Pass control to next equal-priority thread that is in state READY.
    (Higher-priority READY threads would prevent us from running; this
    will not enable lower-priority threads to run, as we remain READY).
    @note You cannot call this function from ISR context.
*/
void yield();

/** Get the thread id of the current running thread.
  @return  thread ID for reference by other functions or nullptr in case of error or in ISR context.
  @note You may call this function from ISR context.
*/
osThreadId_t get_id();

/** Get the thread name of the current running thread.
  @return  thread name pointer or nullptr if thread has no name or in case of error.
  @note You cannot call this function from ISR context.
*/
const char *get_name();

};
/** @}*/
/** @}*/


namespace internal {
/** \addtogroup rtos-internal-api */
/** @{*/

struct flags_check_capture {
    uint32_t *flags;
    uint32_t options;
    uint32_t flags_wanted;
    uint32_t result;
    bool match;
};

bool non_rtos_check_flags(void *handle);

}
/** @}*/
}
#endif
