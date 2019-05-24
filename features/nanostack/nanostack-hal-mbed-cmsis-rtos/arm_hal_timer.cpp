/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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

// Include before mbed.h to properly get UINT*_C()
#include "ns_types.h"

#include "platform/SingletonPtr.h"
#include "platform/arm_hal_timer.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/mbed_assert.h"
#include "Timeout.h"
#include "Timer.h"
#include "Ticker.h"
#include "events/Event.h"
#include "events/mbed_shared_queues.h"

using namespace mbed;
using namespace events;

static SingletonPtr<Timer> timer;
static SingletonPtr<Timeout> timeout;

// If critical sections are implemented using mutexes, timers must be called in thread context, and
// we use the high-priority event queue for this.
// If critical sections disable interrupts, we can call timers directly from interrupt. Avoiding the
// event queue can save ~1600B of RAM if the rest of the system is not using the event queue either.
// Caveats of this tunable are listed on arm_hal_interrupt.c.
#if !MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
static EventQueue *equeue;
#endif

static uint32_t due;
static void (*arm_hal_callback)(void);

#if defined(NS_EVENTLOOP_USE_TICK_TIMER)

#if MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
static SingletonPtr<Ticker> tick_ticker;
#endif

static int tick_timer_id;
static void (*tick_timer_cb)(void);

int8_t platform_tick_timer_register(void (*tick_timer_cb_handler)(void))
{
#if !MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
    equeue = mbed_highprio_event_queue();
    MBED_ASSERT(equeue != NULL);
#endif

    tick_timer_cb = tick_timer_cb_handler;
    return 0;
}

int8_t platform_tick_timer_start(uint32_t period_ms)
{
    int8_t retval = -1;
    if (tick_timer_cb && tick_timer_id == 0) {
#if !MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
        tick_timer_id = equeue->call_every(period_ms, tick_timer_cb);
        if (tick_timer_id != 0) {
            retval = 0;
        }
#else
        tick_ticker->attach_us(tick_timer_cb, period_ms * 1000);
        tick_timer_id = 1;
        retval = 0;
#endif
    }
    return retval;
}

int8_t platform_tick_timer_stop(void)
{
    int8_t retval = -1;
    if (tick_timer_id != 0) {
#if !MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
        equeue->cancel(tick_timer_id);
#else
        tick_ticker->detach();
#endif
        tick_timer_id = 0;
        retval = 0;
    }
    return retval;
}
#endif // NS_EVENTLOOP_USE_TICK_TIMER


// Called once at boot
void platform_timer_enable(void)
{
#if !MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
    equeue = mbed_highprio_event_queue();
    MBED_ASSERT(equeue != NULL);
#endif
    timer->start();
    // Prime the SingletonPtr - can't construct from IRQ/critical section
    timeout.get();
}

// Actually cancels a timer, not the opposite of enable
void platform_timer_disable(void)
{
    timeout->detach();
}

// Not called while running, fortunately
void platform_timer_set_cb(void (*new_fp)(void))
{
    arm_hal_callback = new_fp;
}

static void timer_callback(void)
{
    due = 0;

#if MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
    // Callback is interrupt safe so it can be called directly without
    // bouncing via event queue thread.
    arm_hal_callback();
#else
    equeue->call(arm_hal_callback);
#endif
}

// This is called from inside platform_enter_critical - IRQs can't happen
void platform_timer_start(uint16_t slots)
{
    timer->reset();
    due = slots * UINT32_C(50);
    timeout->attach_us(timer_callback, due);
}

// This is called from inside platform_enter_critical - IRQs can't happen
uint16_t platform_timer_get_remaining_slots(void)
{
    uint32_t elapsed = timer->read_us();
    if (elapsed < due) {
        return (uint16_t)((due - elapsed) / 50);
    } else {
        return 0;
    }
}

