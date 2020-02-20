/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "rtos/Kernel.h"
#include "rtos_idle.h"
#include "rtos_handlers.h"
#include "platform/mbed_critical.h"
#include "platform/source/mbed_os_timer.h"

#if !MBED_CONF_RTOS_PRESENT
/* If the RTOS is not present, we call mbed_thread.cpp to do the work */
/* If the RTOS is present, mbed_thread.cpp calls us to do the work */
#include "platform/mbed_thread.h"
#endif

namespace rtos {

uint64_t Kernel::get_ms_count()
{
#if MBED_CONF_RTOS_PRESENT
    // CMSIS-RTOS 2.1.0 and 2.1.1 differ in the time type. We assume
    // our header at least matches the implementation, so we don't try looking
    // at the run-time version report. (There's no compile-time version report)

    // 2.1.0 uint64_t osKernelGetTickCount(void), not documented as callable from ISR (but RTX does allow)
    // 2.1.1 uint32_t osKernelGetTickCount(void), callable from ISR
    // 2.1.x who knows? We assume could go back to uint64_t
    if (sizeof osKernelGetTickCount() == sizeof(uint64_t)) {
        return osKernelGetTickCount();
    } else { /* assume 32-bit */
        // Based on suggestion in CMSIS-RTOS 2.1.1 docs, but with reentrancy
        // protection for the tick memory. We use critical section rather than a
        // mutex, as hopefully this method can be callable from interrupt later -
        // only thing currently preventing it is that pre CMSIS RTOS 2.1.1, it's
        // not defined as safe.
        // We assume this is called multiple times per 32-bit wrap period (49 days).
        static uint32_t tick_h, tick_l;

        core_util_critical_section_enter();
        // The 2.1.1 API says this is legal from an ISR - we assume this means
        // it's also legal with interrupts disabled. RTX implementation kind
        // of conflates the two.
        uint32_t tick32 = osKernelGetTickCount();
        if (tick32 < tick_l) {
            tick_h++;
        }
        tick_l = tick32;
        uint64_t ret = ((uint64_t) tick_h << 32) | tick_l;
        core_util_critical_section_exit();
        return ret;
    }
#else
    return ::get_ms_count();
#endif
}

#if MBED_CONF_RTOS_PRESENT
void Kernel::attach_idle_hook(void (*fptr)(void))
{
    rtos_attach_idle_hook(fptr);
}

void Kernel::attach_thread_terminate_hook(void (*fptr)(osThreadId_t id))
{
    rtos_attach_thread_terminate_hook(fptr);
}
#endif

}
