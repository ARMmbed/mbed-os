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

#if MBED_TICKLESS

#include "hal/us_ticker_api.h"
#include "hal/lp_ticker_api.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
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

#define US_IN_TICK          (1000000 / OS_TICK_FREQ)
MBED_STATIC_ASSERT(1000000 % OS_TICK_FREQ == 0, "OS_TICK_FREQ must be a divisor of 1000000 for correct tick calculations");

#if (defined(NO_SYSTICK))
/**
 * Return an IRQ number that can be used in the absence of SysTick
 *
 * @return Free IRQ number that can be used
 */
extern "C" IRQn_Type mbed_get_m0_tick_irqn(void);
#endif

#if defined(TARGET_CORTEX_A)
extern "C" IRQn_ID_t mbed_get_a9_tick_irqn(void);
#endif

namespace rtos {
namespace internal {

SysTimer::SysTimer() :
#if DEVICE_LPTICKER
    TimerEvent(get_lp_ticker_data()),
#else
    TimerEvent(get_us_ticker_data()),
#endif
    _time_us(0), _tick(0)
{
    _time_us = ticker_read_us(_ticker_data);
    _suspend_time_passed = true;
    _suspended = false;
}

SysTimer::SysTimer(const ticker_data_t *data) :
    TimerEvent(data), _time_us(0), _tick(0)
{
    _time_us = ticker_read_us(_ticker_data);
    _suspend_time_passed = true;
    _suspended = false;
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
    // Remove ensures serialized access to SysTimer by stopping timer interrupt
    remove();

    _suspend_time_passed = false;
    _suspended = true;

    schedule_tick(ticks);
}

bool SysTimer::suspend_time_passed()
{
    return _suspend_time_passed;
}

uint32_t SysTimer::resume()
{
    // Remove ensures serialized access to SysTimer by stopping timer interrupt
    remove();

    _suspended = false;
    _suspend_time_passed = true;

    uint64_t elapsed_ticks = (ticker_read_us(_ticker_data) - _time_us) / US_IN_TICK;
    if (elapsed_ticks > 0) {
        // Don't update to the current tick. Instead, update to the
        // previous tick and let the SysTick handler increment it
        // to the current value. This allows scheduling restart
        // successfully after the OS is resumed.
        elapsed_ticks--;
    }
    _time_us += elapsed_ticks * US_IN_TICK;
    _tick += elapsed_ticks;

    return elapsed_ticks;
}

void SysTimer::schedule_tick(uint32_t delta)
{
    core_util_critical_section_enter();

    insert_absolute(_time_us + delta * US_IN_TICK);

    core_util_critical_section_exit();
}

void SysTimer::cancel_tick()
{
    // Underlying call is interrupt safe

    remove();
}

uint32_t SysTimer::get_tick()
{
    return _tick & 0xFFFFFFFF;
}

us_timestamp_t SysTimer::get_time()
{
    // Underlying call is interrupt safe

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
    _time_us += US_IN_TICK;
}

void SysTimer::handler()
{
    core_util_critical_section_enter();

    if (_suspended) {
        _suspend_time_passed = true;
    } else {
        _set_irq_pending();
        _increment_tick();
        schedule_tick();
    }

    core_util_critical_section_exit();
}

}
}

#endif // MBED_TICKLESS
