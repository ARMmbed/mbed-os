/*
 * Copyright (c) 2006-2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "platform/mbed_power_mgmt.h"
#include "platform/CriticalSectionLock.h"
#include "platform/internal/SysTimer.h"
#include "platform/internal/mbed_os_timer.h"
#include "us_ticker_api.h"
#include "lp_ticker_api.h"
#include "mbed_critical.h"
#include "mbed_error.h"
#include <new>

/* This provides the marshalling point for a system global SysTimer, which
 * is used to provide:
 * - timed sleeps (for default idle hook in RTOS tickless mode, or non-RTOS sleeps)
 * - regular ticks for RTOS
 * - absolute system timing (directly for non-RTOS, or indirectly via RTOS tick count)
 */

namespace mbed {
namespace internal {

OsTimer *os_timer;

namespace {
uint64_t os_timer_data[(sizeof(OsTimer) + 7) / 8];
}

OsTimer *init_os_timer()
{
    // Do not use SingletonPtr since this relies on the RTOS.
    // Locking not required as it will be first called during
    // OS init, or else we're a non-RTOS single-threaded setup.
    if (!os_timer) {
#if DEVICE_LPTICKER && !MBED_CONF_TARGET_TICKLESS_FROM_US_TICKER
        os_timer = new (os_timer_data) OsTimer(get_lp_ticker_data());
#elif DEVICE_USTICKER
        os_timer = new (os_timer_data) OsTimer(get_us_ticker_data());
#else
        MBED_ERROR(
            MBED_MAKE_ERROR(
                MBED_MODULE_PLATFORM,
                MBED_ERROR_CODE_CONFIG_UNSUPPORTED),
            "OS timer not available");
#endif
    }

    return os_timer;
}

/* These traits classes are designed to permit chunks of code to be
 * omitted - in particular eliminating timers. However, we don't want
 * to cause template bloat, so don't have too many traits variants.
 */

/* Optionally timed operation, with optional predicate */
struct timed_predicate_op {
    timed_predicate_op(OsClock::time_point t) : wake_time(t), orig_predicate(NULL), orig_handle(NULL)
    {
        init_os_timer();
    }

    timed_predicate_op(OsClock::time_point t, bool (*wake_predicate)(void *), void *wake_predicate_handle) : wake_time(t), orig_predicate(wake_predicate), orig_handle(wake_predicate_handle)
    {
        init_os_timer();
    }

    ~timed_predicate_op()
    {
        // Make sure wake timer is cancelled. (It may or may not be, depending on
        // why we woke).
        os_timer->cancel_wake();
    }

    bool wake_condition() const
    {
        return (orig_predicate && orig_predicate(orig_handle)) || os_timer->wake_time_passed();
    }

    void sleep_prepare()
    {
        if (wake_time != wake_time.max()) {
            OsClock::set_wake_time(wake_time);
        }
    }

    bool sleep_prepared()
    {
        return wake_time == wake_time.max() || os_timer->wake_time_set();
    }

private:
    OsClock::time_point wake_time;
    bool (*orig_predicate)(void *);
    void *orig_handle;
};

/* Untimed operation with predicate */
struct untimed_op {
    untimed_op(bool (*wake_predicate)(void *), void *wake_predicate_handle) : orig_predicate(wake_predicate), orig_handle(wake_predicate_handle)
    {
    }

    bool wake_condition() const
    {
        return orig_predicate(orig_handle);
    }

    void sleep_prepare()
    {
    }

    bool sleep_prepared()
    {
        return true;
    }

private:
    bool (*orig_predicate)(void *);
    void *orig_handle;
};

/* We require that this is called from thread context, outside a critical section,
 * and the kernel already suspended if an RTOS, meaning we don't have to worry
 * about any potential threading issues.
 *
 * The wake predicate will be called from both outside and inside a critical
 * section, so appropriate atomic care must be taken.
 */
template <class OpT>
void do_sleep_operation(OpT &op)
{
    // We assume the ticker is not already in use - without RTOS, it
    // is never used, with RTOS, it will have been disabled with OS_Tick_Disable
    while (!op.wake_condition()) {
        // Set (or re-set) the wake time - outside a critical section, as
        // it could take long enough to cause UART data loss on some platforms.
        op.sleep_prepare();

        // If no target sleep function, nothing else to do - just keep
        // rechecking the wake condition.
#if DEVICE_SLEEP
        // Now we need to enter the critical section for the race-free sleep
        {
            CriticalSectionLock lock;

            // Recheck wake conditions before starting sleep, avoiding race
            if (op.wake_condition()) {
                break;
            }

            // It's possible that an intermediate wake interrupt occurred
            // between "set_wake_time" and the critical lock - only sleep
            // if we see that the timer is armed or we don't need it. Otherwise,
            // we go round to set the timer again.
            if (op.sleep_prepared()) {
                // Enter HAL sleep (normal or deep)
                sleep();
            }
        }

        // Ensure interrupts get a chance to fire, which allows new result from
        // wake_predicate() and wake_time_passed()
        __ISB();
#endif
    }
}

/* We require that this is called from thread context, outside a critical section,
 * and the kernel already suspended if an RTOS, meaning we don't have to worry
 * about any potential threading issues.
 *
 * The wake predicate will be called from both outside and inside a critical
 * section, so appropriate atomic care must be taken.
 */
OsClock::time_point do_timed_sleep_absolute(OsClock::time_point wake_time, bool (*wake_predicate)(void *), void *wake_predicate_handle)
{
    {
        timed_predicate_op op(wake_time, wake_predicate, wake_predicate_handle);
        do_sleep_operation(op);
    }

    return OsClock::now_with_init_done();
}


#if MBED_CONF_RTOS_PRESENT
/* The 32-bit limit is part of the API - we will always wake within 2^32 ticks */
/* This version is tuned for RTOS use, where the RTOS needs to know the time spent sleeping */
/* Note that unlike do_timed_sleep_relative_or_forever it does not do a tick update on entry; */
/* it assumes the caller has been using the ticker, and is passing a delay relative to the */
/* time point of the ticks it has acknowledged. */
OsClock::duration_u32 do_timed_sleep_relative_to_acknowledged_ticks(OsClock::duration_u32 wake_delay, bool (*wake_predicate)(void *), void *wake_predicate_handle)
{
    // When running with RTOS, the requested delay will be based on the kernel's tick count.
    // If it missed a tick as entering idle, we should reflect that by basing the start time
    // on its current idea of time - OsClock::acknowledged_ticks().
    // Example: OS acknowledged tick count = 100, our reported tick count = 101, requested delay = 50
    // We need to schedule wake for tick 150, report 50 ticks back to our caller, and
    // clear the unacknowledged tick count.
    OsClock::time_point sleep_start = OsClock::acknowledged_ticks();

    OsClock::time_point sleep_finish = do_timed_sleep_absolute(sleep_start + wake_delay, wake_predicate, wake_predicate_handle);

    return sleep_finish - sleep_start;
}

#else

void do_untimed_sleep(bool (*wake_predicate)(void *), void *wake_predicate_handle)
{
    untimed_op op(wake_predicate, wake_predicate_handle);

    do_sleep_operation(op);
}

/* max() delay is treated as "wait forever" */
/* This version is tuned for non-RTOS use, where we must update the tick count, */
/* don't need to return sleep time, and waiting forever is possible */
void do_timed_sleep_relative_or_forever(OsClock::duration_u32 wake_delay, bool (*wake_predicate)(void *), void *wake_predicate_handle)
{
    // Special-case 0 delay, to save multiple callers having to do it. Just call the predicate once.
    if (wake_delay == wake_delay.zero()) {
        if (wake_predicate) {
            wake_predicate(wake_predicate_handle);
        }
        return;
    }

    OsClock::time_point wake_time;
    if (wake_delay == OsClock::duration_u32::max()) {
        wake_time = OsClock::time_point::max();
    } else {
        wake_time = OsClock::now() + wake_delay;
    }
    /* Always use timed_predicate_op here to save pulling in two templates */
    timed_predicate_op op(wake_time, wake_predicate, wake_predicate_handle);
    do_sleep_operation(op);
}

#endif

} // namespace internal
} // namespace mbed
