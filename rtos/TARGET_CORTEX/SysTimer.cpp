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
#include "rtos/TARGET_CORTEX/SysTimer.h"

#if DEVICE_LPTICKER

#include "hal/lp_ticker_api.h"
#include "mbed_critical.h"
#if defined(TARGET_CORTEX_A)
#include "rtx_core_ca.h"
#else//Cortex-M
#include "rtx_core_cm.h"
#endif
extern "C" {
#include "rtx_lib.h"
#if defined(TARGET_CORTEX_A)
#include "irq_ctrl.h"
#endif
}

#if (defined(NO_SYSTICK))
/**
 * Return an IRQ number that can be used in the absence of SysTick
 *
 * @return Free IRQ number that can be used
 */
extern "C" IRQn_Type mbed_get_m0_tick_irqn(void);
#endif

namespace rtos {
namespace internal {

SysTimer::SysTimer() :
        TimerEvent(get_lp_ticker_data()), _start_time(0), _tick(0)
{
    _start_time = ticker_read_us(_ticker_data);
    _suspend_time_passed = true;
    _suspended = false;
}

SysTimer::SysTimer(const ticker_data_t *data) :
        TimerEvent(data), _start_time(0), _tick(0)
{
    _start_time = ticker_read_us(_ticker_data);
}

void SysTimer::setup_irq()
{
#if (defined(NO_SYSTICK) && !defined (TARGET_CORTEX_A))
    NVIC_SetVector(mbed_get_m0_tick_irqn(), (uint32_t)SysTick_Handler);
    NVIC_SetPriority(mbed_get_m0_tick_irqn(), 0xFF); /* RTOS requires lowest priority */
    NVIC_EnableIRQ(mbed_get_m0_tick_irqn());
#else
    // Ensure SysTick has the correct priority as it is still used
    // to trigger software interrupts on each tick. The period does
    // not matter since it will never start counting.
    OS_Tick_Setup(osRtxConfig.tick_freq, OS_TICK_HANDLER);
#endif
}

void SysTimer::suspend(uint32_t ticks)
{
    core_util_critical_section_enter();

    remove();
    schedule_tick(ticks);
    _suspend_time_passed = false;
    _suspended = true;

    core_util_critical_section_exit();
}

bool SysTimer::suspend_time_passed()
{
    return _suspend_time_passed;
}

uint32_t SysTimer::resume()
{
    core_util_critical_section_enter();

    _suspended = false;
    _suspend_time_passed = true;
    remove();

    uint64_t new_tick = (ticker_read_us(_ticker_data) - _start_time) * OS_TICK_FREQ / 1000000;
    if (new_tick > _tick) {
        // Don't update to the current tick. Instead, update to the
        // previous tick and let the SysTick handler increment it
        // to the current value. This allows scheduling restart
        // successfully after the OS is resumed.
        new_tick--;
    }
    uint32_t elapsed_ticks = new_tick - _tick;
    _tick = new_tick;

    core_util_critical_section_exit();
    return elapsed_ticks;
}

void SysTimer::schedule_tick(uint32_t delta)
{
    core_util_critical_section_enter();

    insert_absolute(_start_time + (_tick + delta) * 1000000ULL / OS_TICK_FREQ);

    core_util_critical_section_exit();
}

void SysTimer::cancel_tick()
{
    core_util_critical_section_enter();

    remove();

    core_util_critical_section_exit();
}

uint32_t SysTimer::get_tick()
{
    return _tick & 0xFFFFFFFF;
}

us_timestamp_t SysTimer::get_time()
{
    return ticker_read_us(_ticker_data);
}

SysTimer::~SysTimer()
{
}

void SysTimer::_set_irq_pending()
{
    // Protected function synchronized externally

#if (defined(NO_SYSTICK))
    NVIC_SetPendingIRQ(mbed_get_m0_tick_irqn());
#elif (TARGET_CORTEX_A)
    IRQ_SetPending(mbed_get_a9_tick_irqn());
#else
    SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
#endif
}

void SysTimer::_increment_tick()
{
    // Protected function synchronized externally

    _tick++;
}

void SysTimer::handler()
{
    core_util_critical_section_enter();

    if (_suspended) {
        _suspend_time_passed = true;
    } else {
        _set_irq_pending();
        _increment_tick();
    }

    core_util_critical_section_exit();
}

}
}

#endif
