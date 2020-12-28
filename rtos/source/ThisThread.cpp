/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
 * SPDX-License-Identifier: MIT
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

#define __STDC_LIMIT_MACROS
#include "rtos/ThisThread.h"

#include "platform/mbed_toolchain.h"
#include "rtos/Kernel.h"
#include "platform/CriticalSectionLock.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_critical.h"
#include "platform/internal/mbed_os_timer.h"

using std::milli;
using std::chrono::duration;
using rtos::Kernel::Clock;
using rtos::Kernel::wait_for_u32_max;
using rtos::Kernel::wait_for_u32_forever;

#if !MBED_CONF_RTOS_PRESENT
/* If the RTOS is not present, we call mbed_thread.cpp to do the work */
/* If the RTOS is present, mbed_thread.cpp calls us to do the work */
#include "platform/mbed_thread.h"

static uint32_t thread_flags;

/* For the flags to be useful, need a way of setting them, but there's only the main
 * thread, and that has no Thread object, so Thread class is not provided. Implement
 * one CMSIS-RTOS function to provide access.
 */
extern "C"
uint32_t osThreadFlagsSet(osThreadId_t /*thread_id*/, uint32_t flags)
{
    return core_util_atomic_fetch_or_u32(&thread_flags, flags) | flags;
}
#endif

namespace rtos {

uint32_t ThisThread::flags_clear(uint32_t flags)
{
#if MBED_CONF_RTOS_PRESENT
    flags = osThreadFlagsClear(flags);
    MBED_ASSERT(!(flags & osFlagsError));
#else
    flags = core_util_atomic_fetch_and_u32(&thread_flags, ~flags);
#endif
    return flags;
}

uint32_t ThisThread::flags_get()
{
#if MBED_CONF_RTOS_PRESENT
    return osThreadFlagsGet();
#else
    return core_util_atomic_load_u32(&thread_flags);
#endif
}

#if !MBED_CONF_RTOS_PRESENT
namespace internal {
bool non_rtos_check_flags(void *handle)
{
    flags_check_capture *check = static_cast<flags_check_capture *>(handle);
    uint32_t cur_flags = core_util_atomic_load_u32(check->flags);
    uint32_t set_flags;
    do {
        set_flags = cur_flags & check->flags_wanted;
        check->result = set_flags;
        if ((check->options & osFlagsWaitAll) ? set_flags == check->flags_wanted : set_flags != 0) {
            if (check->options & osFlagsNoClear) {
                break;
            }
        } else {
            return false;
        }
    } while (!core_util_atomic_cas_u32(check->flags, &cur_flags, cur_flags & ~set_flags));
    check->match = true;
    return true;
}
}
#endif

static uint32_t flags_wait_for(uint32_t flags, Clock::duration_u32 rel_time, bool clear, uint32_t options)
{
    if (!clear) {
        options |= osFlagsNoClear;
    }
#if MBED_CONF_RTOS_PRESENT
    flags = osThreadFlagsWait(flags, options, rel_time.count());
    if (flags & osFlagsError) {
        MBED_ASSERT((flags == osFlagsErrorTimeout && rel_time != wait_for_u32_forever) ||
                    (flags == osFlagsErrorResource && rel_time == rel_time.zero()));
        flags = ThisThread::flags_get();
    }
#else
    rtos::internal::flags_check_capture check;
    check.flags = &thread_flags;
    check.options = options;
    check.flags_wanted = flags;
    check.result = 0;
    mbed::internal::do_timed_sleep_relative_or_forever(rel_time, rtos::internal::non_rtos_check_flags, &check);
    flags = check.result;
#endif

    return flags;
}

static uint32_t flags_wait(uint32_t flags, bool clear, uint32_t options)
{
#if MBED_CONF_RTOS_PRESENT
    return flags_wait_for(flags, wait_for_u32_forever, clear, options);
#else
    /* Avoids pulling in timer if not used */
    if (!clear) {
        options |= osFlagsNoClear;
    }
    rtos::internal::flags_check_capture check;
    check.flags = &thread_flags;
    check.options = options;
    check.flags_wanted = flags;
    check.result = 0;
    mbed::internal::do_untimed_sleep(rtos::internal::non_rtos_check_flags, &check);
    flags = check.result;

    return flags;
#endif
}

static uint32_t flags_wait_until(uint32_t flags, Clock::time_point abs_time, bool clear, uint32_t options)
{
    Clock::time_point now = Clock::now();

    Clock::duration_u32 rel_time;
    if (now >= abs_time) {
        rel_time = rel_time.zero();
    } else if (abs_time - now > wait_for_u32_max) {
        // Documentation permits early return for big offsets
        rel_time = wait_for_u32_max;
    } else {
        rel_time = abs_time - now;
    }
    return flags_wait_for(flags, rel_time, clear, options);
}

uint32_t ThisThread::flags_wait_all(uint32_t flags, bool clear)
{
    return flags_wait(flags, clear, osFlagsWaitAll);
}

uint32_t ThisThread::flags_wait_all_for(uint32_t flags, uint32_t millisec, bool clear)
{
    return flags_wait_all_for(flags, duration<uint32_t, milli>(millisec), clear);
}

uint32_t ThisThread::flags_wait_all_for(uint32_t flags, Clock::duration_u32 rel_time, bool clear)
{
    return flags_wait_for(flags, rel_time, clear, osFlagsWaitAll);
}

uint32_t ThisThread::flags_wait_all_until(uint32_t flags, uint64_t millisec, bool clear)
{
    return flags_wait_all_until(flags, Clock::time_point(duration<uint64_t, milli>(millisec)), clear);
}

uint32_t ThisThread::flags_wait_all_until(uint32_t flags, Clock::time_point abs_time, bool clear)
{
    return flags_wait_until(flags, abs_time, clear, osFlagsWaitAll);
}

uint32_t ThisThread::flags_wait_any(uint32_t flags, bool clear)
{
    return flags_wait(flags, clear, osFlagsWaitAny);
}

uint32_t ThisThread::flags_wait_any_for(uint32_t flags, uint32_t millisec, bool clear)
{
    return flags_wait_any_for(flags, duration<uint32_t, milli>(millisec), clear);
}

uint32_t ThisThread::flags_wait_any_for(uint32_t flags, Clock::duration_u32 rel_time, bool clear)
{
    return flags_wait_for(flags, rel_time, clear, osFlagsWaitAny);
}

uint32_t ThisThread::flags_wait_any_until(uint32_t flags, uint64_t millisec, bool clear)
{
    return flags_wait_any_until(flags, Clock::time_point(duration<uint64_t, milli>(millisec)), clear);
}

uint32_t ThisThread::flags_wait_any_until(uint32_t flags, Clock::time_point abs_time, bool clear)
{
    return flags_wait_until(flags, abs_time, clear, osFlagsWaitAny);
}

void ThisThread::sleep_for(uint32_t millisec)
{
    ThisThread::sleep_for(duration<uint32_t, std::milli>(millisec));
}

void ThisThread::sleep_for(Clock::duration_u32 rel_time)
{
#if MBED_CONF_RTOS_PRESENT
    osStatus_t status = osDelay(rel_time.count());
    MBED_ASSERT(status == osOK);
    (void) status;
#else
    thread_sleep_for(rel_time.count());
#endif
}

void ThisThread::sleep_until(uint64_t millisec)
{
    ThisThread::sleep_until(Clock::time_point(duration<uint64_t, milli>(millisec)));
}

void ThisThread::sleep_until(Clock::time_point abs_time)
{
#if MBED_CONF_RTOS_PRESENT
    Clock::time_point now;

    while ((now = Clock::now()) < abs_time) {
        if (abs_time - now > wait_for_u32_max) {
            osStatus_t status = osDelay(wait_for_u32_max.count());
            MBED_ASSERT(status == osOK);
            (void) status;
            continue;
        } else {
            osStatus_t status = osDelay((abs_time - now).count());
            MBED_ASSERT(status == osOK);
            (void) status;
            break;
        }
    }
#else
    thread_sleep_until(abs_time.time_since_epoch().count());
#endif
}

void ThisThread::yield()
{
#if MBED_CONF_RTOS_PRESENT
    osThreadYield();
#else
    asm("yield");
#endif
}

osThreadId_t ThisThread::get_id()
{
#if MBED_CONF_RTOS_PRESENT
    return osThreadGetId();
#else
    return (osThreadId_t) 1; // dummy non-0 value
#endif
}

const char *ThisThread::get_name()
{
#if MBED_CONF_RTOS_PRESENT
    osThreadId_t id = osThreadGetId();
    if (id == nullptr) {
        return nullptr;
    }
    return osThreadGetName(id);
#else
    return nullptr;
#endif
}

}
