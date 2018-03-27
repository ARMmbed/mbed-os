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
#ifndef MBED_SYS_TIMER_H
#define MBED_SYS_TIMER_H

#if defined(DEVICE_LPTICKER) || defined(DOXYGEN_ONLY)

#include "platform/NonCopyable.h"
#include "drivers/TimerEvent.h"

namespace rtos {
namespace internal {

/**
 * @cond RTOS_INTERNAL
 *
 * @addtogroup rtos
 * @{
 *
 * @defgroup rtos_SysTimer SysTimer class
 * @{
 */

/**
 * The SysTimer class is used exclusively by RTX idle loop in TICKLESS mode.
 *
 * @note SysTimer is not the part of Mbed RTOS API.
 */
class SysTimer: private mbed::TimerEvent, private mbed::NonCopyable<SysTimer> {
public:

    SysTimer();
    SysTimer(const ticker_data_t *data);
    virtual ~SysTimer();

    /**
     * Enable an IRQ/SysTick with the correct priority.
     */
    static void setup_irq();

    /**
     * Set wakeup time and schedule a wakeup event after delta ticks
     *
     * After suspend has been called the function suspend_time_passed
     * can be used to determine if the suspend time has passed.
     *
     * @param delta Ticks to remain suspended
     */
    void suspend(uint32_t delta);

    /**
     * Check if the suspend time has passed
     *
     * @return true if the specified number of ticks has passed otherwise false
     */
    bool suspend_time_passed();

    /**
     * Exit suspend mode and return elapsed ticks
     *
     * Due to a scheduling issue, the number of ticks returned is decremented
     * by 1 so that a handler can be called and update to the current value.
     * This allows scheduling restart successfully after the OS is resumed.
     *
     * @return the number of elapsed ticks minus 1
     */
    uint32_t resume();

    /**
     * Schedule an os tick to fire
     *
     * @param delta Tick to fire at relative to current tick
     *
     * @warning If a tick is already scheduled it needs to be cancelled first!
     */
    void schedule_tick(uint32_t delta = 1);

    /**
     * Prevent any scheduled ticks from triggering
     */
    void cancel_tick();

    /** Get the current tick count
     *
     * @return The number of ticks since timer creation. For the os_timer this
     * should match RTX's tick count (the number of ticks since boot).
     */
    uint32_t get_tick();

    /**
     * Get the time
     *
     * @return Current time in microseconds
     */
    us_timestamp_t get_time();

protected:
    virtual void handler();
    void _increment_tick();
    static void _set_irq_pending();
    us_timestamp_t _start_time;
    uint64_t _tick;
    bool _suspend_time_passed;
    bool _suspended;
};

/**
 * @}
 * @}
 * @endcond
 */

}
}

#endif

#endif
