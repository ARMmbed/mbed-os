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
#include "platform/mbed_sleep.h"
#include "TimerEvent.h"
#include "lp_ticker_api.h"
#include "core_cm.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include <new>
#include "rtx_os.h"
extern "C" {
#include "rtx_lib.h"
}

using namespace mbed;

#ifdef MBED_TICKLESS

#if (defined(NO_SYSTICK))
/**
 * Return an IRQ number that can be used in the absence of SysTick
 *
 * @return Free IRQ number that can be used
 */
extern "C" IRQn_Type mbed_get_m0_tick_irqn(void);
#endif

class RtosTimer : private TimerEvent {
public:
    RtosTimer(): TimerEvent(get_lp_ticker_data()), _start_time(0), _tick(0) {
        _start_time = ticker_read_us(_ticker_data);
#if (defined(NO_SYSTICK))
        NVIC_SetVector(mbed_get_m0_tick_irqn(), (uint32_t)SysTick_Handler);
        NVIC_SetPriority(mbed_get_m0_tick_irqn(), 0xFF); /* RTOS requires lowest priority */
        NVIC_EnableIRQ(mbed_get_m0_tick_irqn());
#else
        // Ensure SysTick has the correct priority as it is still used
        // to trigger software interrupts on each tick. The period does
        // not matter since it will never start counting.
        SysTick_Setup(16);
#endif
    };

    /**
     * Schedule an os tick to fire
     *
     * @param delta Tick to fire at relative to current tick
     */
    void schedule_tick(uint32_t delta=1) {
        insert_absolute(_start_time + (_tick + delta) * 1000000 /  OS_TICK_FREQ);
    }


    /**
     * Prevent any scheduled ticks from triggering
     */
    void cancel_tick() {
        remove();
    }

    /**
     * Get the current tick count
     *
     * @return The number of ticks since boot. This should match RTX's tick count
     */
    uint32_t get_tick() {
        return _tick & 0xFFFFFFFF;
    }

    /**
     * Update the internal tick count
     *
     * @return The number of ticks incremented
     */
    uint32_t update_tick() {
        uint64_t new_tick = ticker_read_us(_ticker_data) * OS_TICK_FREQ / 1000000;
        if (new_tick > _tick) {
            // Don't update to the current tick. Instead, update to the
            // previous tick and let the SysTick handler increment it
            // to the current value. This allows scheduling restart
            // successfully after the OS is resumed.
            new_tick--;
        }
        uint32_t elapsed_ticks = new_tick - _tick;
        _tick = new_tick;
        return elapsed_ticks;
    }

    /**
     * Get the time
     *
     * @return Current time in microseconds
     */
    us_timestamp_t get_time() {
        return ticker_read_us(_ticker_data);
    }

    ~RtosTimer() {

    };

protected:

    void handler() {
#if (defined(NO_SYSTICK))
        NVIC_SetPendingIRQ(mbed_get_m0_tick_irqn());
#else
        SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
#endif
        _tick++;
    }

    us_timestamp_t _start_time;
    uint64_t _tick;
};

static RtosTimer *os_timer;
static uint64_t os_timer_data[sizeof(RtosTimer) / 8];

/// Setup System Timer.
int32_t osRtxSysTimerSetup (void)
{
    // Do not use SingletonPtr since this relies on the RTOS
    if (NULL == os_timer) {
        os_timer = new (os_timer_data) RtosTimer();
    }
    return -1;
}

/// Enable System Timer.
void osRtxSysTimerEnable (void)
{
    // set to fire interrupt on next tick
    os_timer->schedule_tick();
}

/// Disable System Timer.
void osRtxSysTimerDisable (void)
{
    os_timer->cancel_tick();
}

/// Acknowledge System Timer IRQ.
void osRtxSysTimerAckIRQ (void)
{
    os_timer->schedule_tick();
}

/// Get System Timer count.
uint32_t osRtxSysTimerGetCount (void)
{
    return os_timer->get_time() & 0xFFFFFFFF;
}

static void default_idle_hook(void)
{
    uint32_t elapsed_ticks = 0;

    core_util_critical_section_enter();
    uint32_t ticks_to_sleep = svcRtxKernelSuspend();
    MBED_ASSERT(os_timer->get_tick() == svcRtxKernelGetTickCount());
    if (ticks_to_sleep) {
        os_timer->schedule_tick(ticks_to_sleep);

        sleep_manager_lock_deep_sleep();
        sleep();
        sleep_manager_unlock_deep_sleep();

        os_timer->cancel_tick();
        // calculate how long we slept
        elapsed_ticks = os_timer->update_tick();
    }
    svcRtxKernelResume(elapsed_ticks);
    core_util_critical_section_exit();
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

#endif // MBED_TICKLESS

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
