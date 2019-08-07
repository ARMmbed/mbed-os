/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#include "rtos/EventFlags.h"
#include "rtos/ThisThread.h"
#include "mbed_os_timer.h"
#include <string.h>
#include "platform/mbed_error.h"
#include "platform/mbed_assert.h"

namespace rtos {

EventFlags::EventFlags()
{
    constructor("application_unnamed_event_flags");
}

EventFlags::EventFlags(const char *name)
{
    constructor(name);
}

void EventFlags::constructor(const char *name)
{
#if MBED_CONF_RTOS_PRESENT
    osEventFlagsAttr_t attr = { 0 };
    attr.name = name;
    attr.cb_mem = &_obj_mem;
    attr.cb_size = sizeof(_obj_mem);
    _id = osEventFlagsNew(&attr);
    MBED_ASSERT(_id);
#else
    _flags = 0;
#endif
}

uint32_t EventFlags::set(uint32_t flags)
{
#if MBED_CONF_RTOS_PRESENT
    return osEventFlagsSet(_id, flags);
#else
    return core_util_atomic_fetch_or_u32(&_flags, flags) | flags;
#endif
}

uint32_t EventFlags::clear(uint32_t flags)
{
#if MBED_CONF_RTOS_PRESENT
    return osEventFlagsClear(_id, flags);
#else
    return core_util_atomic_fetch_and_u32(&_flags, ~flags);
#endif
}

uint32_t EventFlags::get() const
{
#if MBED_CONF_RTOS_PRESENT
    return osEventFlagsGet(_id);
#else
    return core_util_atomic_load_u32(&_flags);
#endif
}

uint32_t EventFlags::wait_all(uint32_t flags, uint32_t millisec, bool clear)
{
    return wait(flags, osFlagsWaitAll, millisec, clear);
}

uint32_t EventFlags::wait_any(uint32_t flags, uint32_t millisec, bool clear)
{
    return wait(flags, osFlagsWaitAny, millisec, clear);
}

EventFlags::~EventFlags()
{
#if MBED_CONF_RTOS_PRESENT
    osEventFlagsDelete(_id);
#endif
}

uint32_t EventFlags::wait(uint32_t flags, uint32_t opt, uint32_t millisec, bool clear)
{
    if (clear == false) {
        opt |= osFlagsNoClear;
    }

#if MBED_CONF_RTOS_PRESENT
    return osEventFlagsWait(_id, flags, opt, millisec);
#else
    rtos::internal::flags_check_capture check;
    check.flags = &_flags;
    check.options = opt;
    check.flags_wanted = flags;
    check.result = 0;
    check.match = false;
    mbed::internal::do_timed_sleep_relative_or_forever(millisec, rtos::internal::non_rtos_check_flags, &check);
    if (check.match) {
        return check.result;
    } else if (millisec == 0) {
        return osErrorResource;
    } else {
        return osErrorTimeout;
    }
#endif
}

}
