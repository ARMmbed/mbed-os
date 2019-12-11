/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

#include "rtos/source/rtos_idle.h"
#include "platform/mbed_power_mgmt.h"
#include "platform/source/mbed_os_timer.h"
#include "TimerEvent.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include <new>
#include "rtx_os.h"

/* Everything in rtx_lib.h, and provided by this file, has C linkage */
extern "C" {
#include "rtx_lib.h"

    using mbed::internal::os_timer;
    using mbed::internal::OsTimer;

#ifdef MBED_TICKLESS

#if MBED_CONF_TARGET_TICKLESS_FROM_US_TICKER && !DEVICE_USTICKER
#error Microsecond ticker required when MBED_CONF_TARGET_TICKLESS_FROM_US_TICKER is true
#endif

#if !MBED_CONF_TARGET_TICKLESS_FROM_US_TICKER && !DEVICE_LPTICKER
#error Low power ticker required when MBED_CONF_TARGET_TICKLESS_FROM_US_TICKER is false
#endif

    // Setup OS Tick timer to generate periodic RTOS Kernel Ticks
    int32_t OS_Tick_Setup(uint32_t freq, IRQHandler_t handler)
    {
        MBED_ASSERT(freq == 1000);

#ifdef TARGET_CORTEX_A
        IRQn_ID_t irq = OsTimer::get_irq_number();

        IRQ_SetPriority(irq, 0xFF);
        IRQ_SetHandler(irq, handler);
        IRQ_EnableIRQ(irq);
#else
        IRQn_Type irq = OsTimer::get_irq_number();

        NVIC_SetPriority(irq, 0xFF);
        NVIC_SetVector(irq, (uint32_t)handler);
        MBED_ASSERT(handler == (IRQHandler_t)NVIC_GetVector(irq));

        if (irq >= 0) {
            NVIC_EnableIRQ(irq);
        }
#endif

        return 0;
    }

    // Enable System Timer.
    void OS_Tick_Enable(void)
    {
        // set to fire interrupt on next tick
        mbed::internal::init_os_timer()->start_tick();
    }

    // Disable System Timer.
    void OS_Tick_Disable(void)
    {
        os_timer->cancel_tick();
    }

    // Acknowledge System Timer IRQ.
    void OS_Tick_AcknowledgeIRQ(void)
    {
        os_timer->acknowledge_tick();
    }

    // Get System Timer count.
    uint32_t OS_Tick_GetCount(void)
    {
        return (uint32_t) os_timer->get_time_since_tick();
    }

    // Get OS Tick IRQ number.
    int32_t  OS_Tick_GetIRQn(void)
    {
        return os_timer->get_irq_number();
    }

    // Get OS Tick overflow status.
    uint32_t OS_Tick_GetOverflow(void)
    {
        // No need to indicate overflow - we let OS_Tick_GetCount overflow above
        // OS_Tick_GetInterval.
        return 0;
    }

    // Get OS Tick timer clock frequency
    uint32_t OS_Tick_GetClock(void)
    {
        return 1000000;
    }

    // Get OS Tick interval.
    uint32_t OS_Tick_GetInterval(void)
    {
        return 1000;
    }

    static bool rtos_event_pending(void *)
    {
        return core_util_atomic_load_u8(&osRtxInfo.kernel.pendSV);
    }

    static void default_idle_hook(void)
    {
        uint32_t ticks_to_sleep = osKernelSuspend();
        // osKernelSuspend will call OS_Tick_Disable, cancelling the tick, which frees
        // up the os timer for the timed sleep
        uint64_t ticks_slept = mbed::internal::do_timed_sleep_relative(ticks_to_sleep, rtos_event_pending);
        MBED_ASSERT(ticks_slept < osWaitForever);
        osKernelResume((uint32_t) ticks_slept);
    }


#else // MBED_TICKLESS

    static void default_idle_hook(void)
    {
        // critical section to complete sleep with locked deepsleep
        core_util_critical_section_enter();
        sleep_manager_lock_deep_sleep();
        sleep();
        sleep_manager_unlock_deep_sleep();
        core_util_critical_section_exit();
    }

#endif // MBED_TICKLESS

    static void (*idle_hook_fptr)(void) = &default_idle_hook;

    void rtos_attach_idle_hook(void (*fptr)(void))
    {
        //Attach the specified idle hook, or the default idle hook in case of a null pointer
        if (fptr != nullptr) {
            idle_hook_fptr = fptr;
        } else {
            idle_hook_fptr = default_idle_hook;
        }
    }

    MBED_NORETURN void rtos_idle_loop(void)
    {
        //Continuously call the idle hook function pointer
        while (1) {
            idle_hook_fptr();
        }
    }

} // extern "C"
