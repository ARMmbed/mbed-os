/* mbed Microcontroller Library
 * Copyright (c) 2017-2019 ARM Limited
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
#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <chrono>
#include "rtos/mbed_rtos_types.h"
#include "platform/mbed_toolchain.h"
#if !MBED_CONF_RTOS_PRESENT
#include "platform/internal/mbed_os_timer.h"
#endif


namespace rtos {

/** \addtogroup rtos-public-api */
/** @{*/

/** Functions in the Kernel namespace control RTOS kernel information. */
namespace Kernel {

namespace impl {
/* Internal integer-returning function.
 *
 * ARM EABI means that `time_point`s do not get returned in registers, so
 * it's worth having the actual exteernal definition return an integer, and only
 * convert to `time_point` via the inline function `now()`.
 */
uint64_t get_tick_count();
}

/** Read the current RTOS kernel millisecond tick count.
     The tick count corresponds to the tick count the RTOS uses for timing
     purposes. It increments monotonically from 0 at boot, so it effectively
     never wraps. If the underlying RTOS only provides a 32-bit tick count,
     this method expands it to 64 bits.
     @return  RTOS kernel current tick count
     @note Mbed OS always uses millisecond RTOS ticks, and this could only wrap
           after half a billion years.
     @note You cannot call this function from ISR context.
     @deprecated Use `Kernel::Clock::now()` to get a chrono time_point instead of an integer millisecond count.
 */
MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Use `Kernel::Clock::now()` to get a chrono time_point instead of an integer millisecond count.")
uint64_t get_ms_count();

/** A C++11 chrono TrivialClock for the kernel millisecond tick count
 *
 * @note To fit better into the chrono framework, Kernel::Clock uses
 *       std::chrono::milliseconds as its representation, which makes it signed
 *       and at least 45 bits (so it will be int64_t or equivalent).
 */
struct Clock {
    Clock() = delete;
    /* Standard TrivialClock fields */
    using duration = std::chrono::milliseconds;
    using rep = duration::rep;
    using period = duration::period;
#if MBED_CONF_RTOS_PRESENT
    using time_point = std::chrono::time_point<Clock>;
#else
    /* In non-RTOS builds, the clock maps directly to the underlying clock, and must
     * indicate that here, so we can do implicit conversion internally.
     */
    using time_point = std::chrono::time_point<mbed::internal::OsClock, duration>;
#endif
    static constexpr bool is_steady = true;
    static time_point now()
    {
        return time_point(duration(impl::get_tick_count()));
    }
    /* Extension to make it easy to use 32-bit durations for some APIs, as we historically have,
     * for efficiency.
     */
    using duration_u32 = std::chrono::duration<uint32_t, period>;

    /** Lock the clock to ensure it stays running; dummy for API compatibility with HighResClock */
    static void lock()
    {
    }

    /** Unlock the clock, allowing it to stop during power saving; dummy for API compatibility with HighResClock */
    static void unlock()
    {
    }
};

/** Maximum duration for Kernel::Clock::duration_u32-based APIs
 *
 * @note As duration_u32-based APIs pass through straight to CMSIS-RTOS, they will
 *       interpret duration_u32(0xFFFFFFFF) as "wait forever". Indicate maximum
 *       wait time of 0xFFFFFFFE for these calls (which is ~49 days).
 */
constexpr Clock::duration_u32 wait_for_u32_max{osWaitForever - 1};

/** Magic "wait forever" constant for Kernel::Clock::duration_u32-based APIs
 *
 * Many duration_u32-based APIs treat duration_u32(0xFFFFFFFF) as "wait forever".
 */
constexpr Clock::duration_u32 wait_for_u32_forever{osWaitForever};

/** Attach a function to be called by the RTOS idle task.
  @param   fptr pointer to the function to be called

  @note You may call this function from ISR context.
  @note Bare metal profile: This API is not supported.
*/
void attach_idle_hook(void (*fptr)(void));

/** Attach a function to be called when a thread terminates.
  @param   fptr pointer to the function to be called

  @note You may call this function from ISR context.
  @note Bare metal profile: This API is not supported.
*/
void attach_thread_terminate_hook(void (*fptr)(osThreadId_t id));

} // namespace Kernel

/** @}*/

} // namespace rtos
#endif
