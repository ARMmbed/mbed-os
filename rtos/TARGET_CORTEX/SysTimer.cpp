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

#if DEVICE_LOWPOWERTIMER

#include "hal/lp_ticker_api.h"
#include "rtx_core_cm.h"
extern "C" {
#include "rtx_lib.h"
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
}

void SysTimer::setup_irq()
{
#if (defined(NO_SYSTICK))
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

void SysTimer::schedule_tick(uint32_t delta)
{
    insert_absolute(_start_time + (_tick + delta) * 1000000ULL / OS_TICK_FREQ);
}

void SysTimer::cancel_tick()
{
    remove();
}

uint32_t SysTimer::get_tick()
{
    return _tick & 0xFFFFFFFF;
}

uint32_t SysTimer::update_tick()
{
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
    return elapsed_ticks;
}

us_timestamp_t SysTimer::get_time()
{
    return ticker_read_us(_ticker_data);
}

SysTimer::~SysTimer()
{
}

void SysTimer::set_irq_pending()
{
#if (defined(NO_SYSTICK))
    NVIC_SetPendingIRQ(mbed_get_m0_tick_irqn());
#else
    SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
#endif
}

void SysTimer::increment_tick()
{
    _tick++;
}

void SysTimer::handler()
{
    set_irq_pending();
    increment_tick();
}

}
}

#endif
