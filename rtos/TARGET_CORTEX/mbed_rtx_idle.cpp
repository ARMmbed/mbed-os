/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
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

#include "rtos/rtos_idle.h"
#include "platform/mbed_power_mgmt.h"
#include "TimerEvent.h"
#include "lp_ticker_api.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include <new>
#include "rtx_os.h"
extern "C" {
#include "rtx_lib.h"
}

using namespace mbed;

#if (defined(MBED_TICKLESS) && defined(DEVICE_LPTICKER))

#include "rtos/TARGET_CORTEX/SysTimer.h"

static rtos::internal::SysTimer *os_timer;
static uint64_t os_timer_data[sizeof(rtos::internal::SysTimer) / 8];

/// Enable System Timer.
void OS_Tick_Enable (void)
{
    // Do not use SingletonPtr since this relies on the RTOS
    if (NULL == os_timer) {
        os_timer = new (os_timer_data) rtos::internal::SysTimer();
        os_timer->setup_irq();
    }

    // set to fire interrupt on next tick
    os_timer->schedule_tick();
}

/// Disable System Timer.
void OS_Tick_Disable (void)
{
    os_timer->cancel_tick();
}

/// Acknowledge System Timer IRQ.
void OS_Tick_AcknowledgeIRQ (void)
{
    os_timer->schedule_tick();
}

/// Get System Timer count.
uint32_t OS_Tick_GetCount (void)
{
    return os_timer->get_time() & 0xFFFFFFFF;
}

// Get OS Tick IRQ number.
int32_t  OS_Tick_GetIRQn (void) {
  return -1;
}

// Get OS Tick overflow status.
uint32_t OS_Tick_GetOverflow (void) {
  return 0;
}

// Get OS Tick interval.
uint32_t OS_Tick_GetInterval (void) {
  return 1000;
}

static void default_idle_hook(void)
{
    uint32_t ticks_to_sleep = osKernelSuspend();
    os_timer->suspend(ticks_to_sleep);

    bool event_pending = false;
    while (!os_timer->suspend_time_passed() && !event_pending) {

        core_util_critical_section_enter();
        if (osRtxInfo.kernel.pendSV) {
            event_pending = true;
        } else {
            sleep();
        }
        core_util_critical_section_exit();

        // Ensure interrupts get a chance to fire
        __ISB();
    }
    osKernelResume(os_timer->resume());
}

#elif defined(FEATURE_UVISOR)

static void default_idle_hook(void)
{
    /* uVisor can't sleep. See <https://github.com/ARMmbed/uvisor/issues/420>
     * for details. */
}

#else

static void default_idle_hook(void)
{
    // critical section to complete sleep with locked deepsleep
    core_util_critical_section_enter();
    sleep_manager_lock_deep_sleep();
    sleep();
    sleep_manager_unlock_deep_sleep();
    core_util_critical_section_exit();
}

#endif // (defined(MBED_TICKLESS) && defined(DEVICE_LPTICKER))

static void (*idle_hook_fptr)(void) = &default_idle_hook;

void rtos_attach_idle_hook(void (*fptr)(void))
{
    //Attach the specified idle hook, or the default idle hook in case of a NULL pointer
    if (fptr != NULL) {
        idle_hook_fptr = fptr;
    } else {
        idle_hook_fptr = default_idle_hook;
    }
}

extern "C" void rtos_idle_loop(void)
{
    //Continuously call the idle hook function pointer
    while (1) {
        idle_hook_fptr();
    }
}
