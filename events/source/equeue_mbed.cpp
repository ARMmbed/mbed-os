/*
 * Implementation for the mbed library
 * https://github.com/mbedmicro/mbed
 *
 * Copyright (c) 2016-2019 ARM Limited
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
#include "events/internal/equeue_platform.h"

#if defined(EQUEUE_PLATFORM_MBED)

#include <stdbool.h>
#include <string.h>
#include "cmsis.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_power_mgmt.h"
#include "drivers/Timer.h"
#include "drivers/Ticker.h"
#include "drivers/Timeout.h"
#include "drivers/LowPowerTimeout.h"
#include "drivers/LowPowerTicker.h"
#include "drivers/LowPowerTimer.h"

using namespace mbed;

// Ticker operations
#if MBED_CONF_RTOS_API_PRESENT

#include "rtos/Kernel.h"
#include "platform/source/mbed_os_timer.h"

void equeue_tick_init()
{
#if defined MBED_TICKLESS || !MBED_CONF_RTOS_PRESENT
    mbed::internal::init_os_timer();
#endif
}

unsigned equeue_tick()
{
#if defined MBED_TICKLESS || !MBED_CONF_RTOS_PRESENT
    // It is not safe to call get_ms_count from ISRs, both
    // because documentation says so, and because it will give
    // a stale value from the RTOS if the interrupt has woken
    // us out of sleep - the RTOS will not have updated its
    // ticks yet.
    if (core_util_is_isr_active()) {
        // And the documentation further says that this
        // should not be called from critical sections, for
        // performance reasons, but I don't have a good
        // current alternative!
        return mbed::internal::os_timer->get_time() / 1000;
    } else {
        return rtos::Kernel::get_ms_count();
    }
#else
    // And this is the legacy behaviour - if running in
    // non-tickless mode, this works fine, despite Mbed OS
    // documentation saying no. (Most recent CMSIS-RTOS
    // permits `ososKernelGetTickCount` from IRQ, and our
    // `rtos::Kernel` wrapper copes too).
    return rtos::Kernel::get_ms_count();
#endif
}

#else

#if MBED_CONF_EVENTS_USE_LOWPOWER_TIMER_TICKER

#define ALIAS_TIMER      LowPowerTimer
#define ALIAS_TICKER     LowPowerTicker
#define ALIAS_TIMEOUT    LowPowerTimeout
#else
#define ALIAS_TIMER      Timer
#define ALIAS_TICKER     Ticker
#define ALIAS_TIMEOUT    Timeout
#endif

static volatile unsigned equeue_minutes = 0;
static unsigned equeue_timer[
     (sizeof(ALIAS_TIMER) + sizeof(unsigned) - 1) / sizeof(unsigned)];
static unsigned equeue_ticker[
     (sizeof(ALIAS_TICKER) + sizeof(unsigned) - 1) / sizeof(unsigned)];

static void equeue_tick_update()
{
    equeue_minutes += reinterpret_cast<ALIAS_TIMER *>(equeue_timer)->read_ms();
    reinterpret_cast<ALIAS_TIMER *>(equeue_timer)->reset();
}

void equeue_tick_init()
{
    MBED_STATIC_ASSERT(sizeof(equeue_timer) >= sizeof(ALIAS_TIMER),
                       "The equeue_timer buffer must fit the class Timer");
    MBED_STATIC_ASSERT(sizeof(equeue_ticker) >= sizeof(ALIAS_TICKER),
                       "The equeue_ticker buffer must fit the class Ticker");
    ALIAS_TIMER *timer = new (equeue_timer) ALIAS_TIMER;
    ALIAS_TICKER *ticker = new (equeue_ticker) ALIAS_TICKER;

    equeue_minutes = 0;
    timer->start();
    ticker->attach_us(equeue_tick_update, 1000 << 16);
}

unsigned equeue_tick()
{
    unsigned minutes;
    unsigned ms;

    do {
        minutes = equeue_minutes;
        ms = reinterpret_cast<ALIAS_TIMER *>(equeue_timer)->read_ms();
    } while (minutes != equeue_minutes);

    return minutes + ms;
}

#endif

// Mutex operations
int equeue_mutex_create(equeue_mutex_t *m)
{
    return 0;
}
void equeue_mutex_destroy(equeue_mutex_t *m) { }

void equeue_mutex_lock(equeue_mutex_t *m)
{
    core_util_critical_section_enter();
}

void equeue_mutex_unlock(equeue_mutex_t *m)
{
    core_util_critical_section_exit();
}


// Semaphore operations
#ifdef MBED_CONF_RTOS_API_PRESENT

#include "rtos/EventFlags.h"

MBED_STATIC_ASSERT(sizeof(equeue_sema_t) == sizeof(rtos::EventFlags), "equeue_sema_t / rtos::EventFlags mismatch");

int equeue_sema_create(equeue_sema_t *s)
{
    new (s) rtos::EventFlags("equeue");
    return 0;
}

void equeue_sema_destroy(equeue_sema_t *s)
{
    rtos::EventFlags *ef = reinterpret_cast<rtos::EventFlags *>(s);
    ef->~EventFlags();
}

void equeue_sema_signal(equeue_sema_t *s)
{
    rtos::EventFlags *ef = reinterpret_cast<rtos::EventFlags *>(s);
    ef->set(1);
}

bool equeue_sema_wait(equeue_sema_t *s, int ms)
{
    if (ms < 0) {
        ms = osWaitForever;
    }

    rtos::EventFlags *ef = reinterpret_cast<rtos::EventFlags *>(s);
    return ef->wait_any(1, ms) == 1;
}

#else

// Semaphore operations
int equeue_sema_create(equeue_sema_t *s)
{
    *s = 0;
    return 0;
}

void equeue_sema_destroy(equeue_sema_t *s)
{
}

void equeue_sema_signal(equeue_sema_t *s)
{
    *s = 1;
}

static void equeue_sema_timeout(equeue_sema_t *s)
{
    *s = -1;
}

bool equeue_sema_wait(equeue_sema_t *s, int ms)
{
    ALIAS_TIMEOUT timeout;
    if (ms > 0) {
        timeout.attach_us(callback(equeue_sema_timeout, s), (us_timestamp_t)ms * 1000);
    }

    core_util_critical_section_enter();
    while (!*s && ms != 0) {
        sleep();
        core_util_critical_section_exit();
        __ISB();
        core_util_critical_section_enter();
    }

    int signal = *s;
    *s = 0;
    core_util_critical_section_exit();

    return (signal > 0);
}

#endif

#endif
