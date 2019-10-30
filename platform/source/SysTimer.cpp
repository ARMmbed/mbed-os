/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hal/us_ticker_api.h"
#include "hal/lp_ticker_api.h"
#include "mbed_atomic.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include "platform/mbed_power_mgmt.h"
#include "platform/CriticalSectionLock.h"
#include "platform/source/SysTimer.h"
extern "C" {
#if MBED_CONF_RTOS_PRESENT
#include "rtx_lib.h"
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

#if defined(TARGET_CORTEX_A)
extern "C" IRQn_ID_t mbed_get_a9_tick_irqn(void);
#endif

namespace mbed {
namespace internal {

template<uint32_t US_IN_TICK, bool IRQ>
SysTimer<US_IN_TICK, IRQ>::SysTimer() :
#if DEVICE_LPTICKER
    TimerEvent(get_lp_ticker_data()),
#else
    TimerEvent(get_us_ticker_data()),
#endif
    _time_us(ticker_read_us(_ticker_data)),
    _tick(0),
    _unacknowledged_ticks(0),
    _wake_time_set(false),
    _wake_time_passed(false),
    _wake_early(false),
    _ticking(false),
    _deep_sleep_locked(false)
{
}

template<uint32_t US_IN_TICK, bool IRQ>
SysTimer<US_IN_TICK, IRQ>::SysTimer(const ticker_data_t *data) :
    TimerEvent(data),
    _time_us(ticker_read_us(_ticker_data)),
    _tick(0),
    _unacknowledged_ticks(0),
    _wake_time_set(false),
    _wake_time_passed(false),
    _wake_early(false),
    _ticking(false),
    _deep_sleep_locked(false)
{
}

template<uint32_t US_IN_TICK, bool IRQ>
SysTimer<US_IN_TICK, IRQ>::~SysTimer()
{
    cancel_tick();
    cancel_wake();
}

template<uint32_t US_IN_TICK, bool IRQ>
void SysTimer<US_IN_TICK, IRQ>::set_wake_time(uint64_t at)
{
    // SysTimer must not be active - we must be in suspend state
    MBED_ASSERT(!_ticking);

    // There is a potential race here, when called from outside
    // a critical section. See function documentation for notes on
    // handling it.
    if (core_util_atomic_load_bool(&_wake_time_set)) {
        return;
    }

    // Analyse the timers
    if (update_and_get_tick() >= at) {
        _wake_time_passed = true;
        return;
    }

    uint64_t ticks_to_sleep = at - _tick;
    uint64_t wake_time = at * US_IN_TICK;

    /* Set this first, before attaching the interrupt that can unset it */
    _wake_time_set = true;
    _wake_time_passed = false;

    if (!_deep_sleep_locked && !_ticker_data->interface->runs_in_deep_sleep) {
        _deep_sleep_locked = true;
        sleep_manager_lock_deep_sleep();
    }
    /* Consider whether we will need early or precise wake-up */
    if (MBED_CONF_TARGET_DEEP_SLEEP_LATENCY > 0 &&
            ticks_to_sleep > MBED_CONF_TARGET_DEEP_SLEEP_LATENCY &&
            !_deep_sleep_locked) {
        /* If there is deep sleep latency, but we still have enough time,
         * and we haven't blocked deep sleep ourselves,
         * allow for that latency by requesting early wake-up.
         * Actual sleep may or may not be deep, depending on other actors.
         */
        _wake_early = true;
        insert_absolute(wake_time - MBED_CONF_TARGET_DEEP_SLEEP_LATENCY * US_IN_TICK);
    } else {
        /* Otherwise, set up to wake at the precise time.
         * If there is a deep sleep latency, ensure that we're holding the lock so the sleep
         * is shallow. (If there is no deep sleep latency, we're fine with it being deep).
         */
        _wake_early = false;
        if (MBED_CONF_TARGET_DEEP_SLEEP_LATENCY > 0 && !_deep_sleep_locked) {
            _deep_sleep_locked = true;
            sleep_manager_lock_deep_sleep();
        }
        insert_absolute(wake_time);
    }
}

template<uint32_t US_IN_TICK, bool IRQ>
void SysTimer<US_IN_TICK, IRQ>::cancel_wake()
{
    MBED_ASSERT(!_ticking);
    // Remove ensures serialized access to SysTimer by stopping timer interrupt
    remove();

    _wake_time_set = false;
    _wake_time_passed = false;

    if (_deep_sleep_locked) {
        _deep_sleep_locked = false;
        sleep_manager_unlock_deep_sleep();
    }
}

template<uint32_t US_IN_TICK, bool IRQ>
uint64_t SysTimer<US_IN_TICK, IRQ>::_elapsed_ticks() const
{
    uint64_t elapsed_us = ticker_read_us(_ticker_data) - _time_us;
    if (elapsed_us < US_IN_TICK) {
        return 0;
    } else if (elapsed_us < 2 * US_IN_TICK) {
        return 1;
    } else if (elapsed_us <= 0xFFFFFFFF) {
        // Fast common case avoiding 64-bit division
        return (uint32_t) elapsed_us / US_IN_TICK;
    } else {
        return elapsed_us / US_IN_TICK;
    }
}

template<uint32_t US_IN_TICK, bool IRQ>
void SysTimer<US_IN_TICK, IRQ>::start_tick()
{
    _ticking = true;
    if (_unacknowledged_ticks > 0) {
        _set_irq_pending();
    }
    _schedule_tick();
}

template<uint32_t US_IN_TICK, bool IRQ>
void SysTimer<US_IN_TICK, IRQ>::_schedule_tick()
{
    insert_absolute(_time_us + US_IN_TICK);
}

template<uint32_t US_IN_TICK, bool IRQ>
void SysTimer<US_IN_TICK, IRQ>::acknowledge_tick()
{
    // Try to avoid missed ticks if OS's IRQ level is not keeping
    // up with our handler.
    // 8-bit counter to save space, and also make sure it we don't
    // try TOO hard to resync if something goes really awry -
    // resync will reset if the count hits 256.
    if (core_util_atomic_decr_u8(&_unacknowledged_ticks, 1) > 0) {
        _set_irq_pending();
    }
}

template<uint32_t US_IN_TICK, bool IRQ>
void SysTimer<US_IN_TICK, IRQ>::cancel_tick()
{
    // Underlying call is interrupt safe

    remove();
    _ticking = false;

    _clear_irq_pending();
}

template<uint32_t US_IN_TICK, bool IRQ>
uint64_t SysTimer<US_IN_TICK, IRQ>::get_tick() const
{
    // Atomic is necessary as this can be called from any foreground context,
    // while IRQ can update it.
    return core_util_atomic_load_u64(&_tick);
}

template<uint32_t US_IN_TICK, bool IRQ>
uint64_t SysTimer<US_IN_TICK, IRQ>::update_and_get_tick()
{
    MBED_ASSERT(!_ticking && !_wake_time_set);
    // Can only be used when no interrupts are scheduled
    // Update counters to reflect elapsed time
    uint64_t elapsed_ticks = _elapsed_ticks();
    _unacknowledged_ticks = 0;
    _time_us += elapsed_ticks * US_IN_TICK;
    _tick += elapsed_ticks;

    return _tick;
}

template<uint32_t US_IN_TICK, bool IRQ>
us_timestamp_t SysTimer<US_IN_TICK, IRQ>::get_time() const
{
    // Underlying call is interrupt safe

    return ticker_read_us(_ticker_data);
}

template<uint32_t US_IN_TICK, bool IRQ>
us_timestamp_t SysTimer<US_IN_TICK, IRQ>::get_time_since_tick() const
{
    // Underlying call is interrupt safe, and _time_us is not updated by IRQ

    return get_time() - _time_us;
}

#if (defined(NO_SYSTICK))
template<uint32_t US_IN_TICK, bool IRQ>
IRQn_Type SysTimer<US_IN_TICK, IRQ>::get_irq_number()
{
    return mbed_get_m0_tick_irqn();
}
#elif (TARGET_CORTEX_M)
template<uint32_t US_IN_TICK, bool IRQ>
IRQn_Type SysTimer<US_IN_TICK, IRQ>::get_irq_number()
{
    return SysTick_IRQn;
}
#elif (TARGET_CORTEX_A)
template<uint32_t US_IN_TICK, bool IRQ>
IRQn_ID_t SysTimer<US_IN_TICK, IRQ>::get_irq_number()
{
    return mbed_get_a9_tick_irqn();
}
#endif

template<uint32_t US_IN_TICK, bool IRQ>
void SysTimer<US_IN_TICK, IRQ>::_set_irq_pending()
{
    // Protected function synchronized externally
    if (!IRQ) {
        return;
    }
#if (defined(NO_SYSTICK))
    NVIC_SetPendingIRQ(mbed_get_m0_tick_irqn());
#elif (TARGET_CORTEX_M)
    SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
#else
    IRQ_SetPending(mbed_get_a9_tick_irqn());
#endif
}

template<uint32_t US_IN_TICK, bool IRQ>
void SysTimer<US_IN_TICK, IRQ>::_clear_irq_pending()
{
    // Protected function synchronized externally
    if (!IRQ) {
        return;
    }
#if (defined(NO_SYSTICK))
    NVIC_ClearPendingIRQ(mbed_get_m0_tick_irqn());
#elif (TARGET_CORTEX_M)
    SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;
#else
    IRQ_ClearPending(mbed_get_a9_tick_irqn());
#endif
}

template<uint32_t US_IN_TICK, bool IRQ>
void SysTimer<US_IN_TICK, IRQ>::_increment_tick()
{
    // Protected function synchronized externally

    _tick++;
    _time_us += US_IN_TICK;
}

template<uint32_t US_IN_TICK, bool IRQ>
void SysTimer<US_IN_TICK, IRQ>::handler()
{
    /* To reduce IRQ latency problems, we do not re-arm in the interrupt handler */
    if (_wake_time_set) {
        _wake_time_set = false;
        if (!_wake_early) {
            _wake_time_passed = true;
        }
        /* If this was an early interrupt, user has the responsibility to check and
         * note the combination of (!set, !passed), and re-arm the wake timer if
         * necessary.
         */
    } else if (_ticking) {
        _unacknowledged_ticks++;
        _set_irq_pending();
        _increment_tick();
        // We do this now, rather than in acknowledgement, as we get it "for free"
        // here - because we're in the ticker handler, the programming gets deferred
        // until end of dispatch, and the ticker would likely be rescheduling
        // anyway after dispatch.

        _schedule_tick();
    }
}

#if MBED_CONF_RTOS_PRESENT
/* Whatever the OS wants (in case it isn't 1ms) */
MBED_STATIC_ASSERT(1000000 % OS_TICK_FREQ == 0, "OS_TICK_FREQ must be a divisor of 1000000 for correct tick calculations");
#define OS_TICK_US (1000000 / OS_TICK_FREQ)
#if OS_TICK_US != 1000
template class SysTimer<OS_TICK_US>;
#endif
#endif

/* Standard 1ms SysTimer */
template class SysTimer<1000>;

/* Standard 1ms SysTimer that doesn't set interrupts, used for Greentea tests */
template class SysTimer<1000, false>;

/* Slowed-down SysTimer that doesn't set interrupts, used for Greentea tests */
template class SysTimer<42000, false>;

} // namespace internal
} // namespace mbed
