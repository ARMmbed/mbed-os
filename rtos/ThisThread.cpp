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

#define __STDC_LIMIT_MACROS
#include "rtos/ThisThread.h"

#include "rtos/Kernel.h"
#include "rtos/rtos_idle.h"
#include "platform/mbed_assert.h"

namespace rtos {

uint32_t ThisThread::flags_clear(uint32_t flags)
{
    flags = osThreadFlagsClear(flags);
    MBED_ASSERT(!(flags & osFlagsError));
    return flags;
}

uint32_t ThisThread::flags_get()
{
    return osThreadFlagsGet();
}

static uint32_t flags_wait_for(uint32_t flags, uint32_t millisec, bool clear, uint32_t options)
{
    if (!clear) {
        options |= osFlagsNoClear;
    }
    flags = osThreadFlagsWait(flags, options, millisec);
    if (flags & osFlagsError) {
        MBED_ASSERT((flags == osFlagsErrorTimeout && millisec != osWaitForever) ||
                    (flags == osFlagsErrorResource && millisec == 0));
        flags = ThisThread::flags_get();
    }

    return flags;
}

static uint32_t flags_wait_until(uint32_t flags, uint64_t millisec, bool clear, uint32_t options)
{
    uint64_t now = Kernel::get_ms_count();

    uint32_t delay;
    if (now >= millisec) {
        delay = 0;
    } else if (millisec - now >= osWaitForever) {
        // Documentation permits early return for big offsets
        delay = osWaitForever - 1;
    } else {
        delay = millisec - now;
    }
    return flags_wait_for(flags, delay, clear, options);
}

uint32_t ThisThread::flags_wait_all(uint32_t flags, bool clear)
{
    return flags_wait_for(flags, osWaitForever, clear, osFlagsWaitAll);
}

uint32_t ThisThread::flags_wait_all_for(uint32_t flags, uint32_t millisec, bool clear)
{
    return flags_wait_for(flags, millisec, clear, osFlagsWaitAll);
}

uint32_t ThisThread::flags_wait_all_until(uint32_t flags, uint64_t millisec, bool clear)
{
    return flags_wait_until(flags, millisec, clear, osFlagsWaitAll);
}

uint32_t ThisThread::flags_wait_any(uint32_t flags, bool clear)
{
    return flags_wait_for(flags, osWaitForever, clear, osFlagsWaitAny);
}

uint32_t ThisThread::flags_wait_any_for(uint32_t flags, uint32_t millisec, bool clear)
{
    return flags_wait_for(flags, millisec, clear, osFlagsWaitAny);
}

uint32_t ThisThread::flags_wait_any_until(uint32_t flags, uint64_t millisec, bool clear)
{
    return flags_wait_until(flags, millisec, clear, osFlagsWaitAny);
}

void ThisThread::sleep_for(uint32_t millisec)
{
    osStatus_t status = osDelay(millisec);
    MBED_ASSERT(status == osOK);
}

void ThisThread::sleep_until(uint64_t millisec)
{
    // CMSIS-RTOS 2.1.0 had 64-bit time and osDelayUntil, but that's been revoked.
    // Limit ourselves to manual implementation assuming a >=32-bit osDelay.

    // 64-bit time doesn't wrap (for half a billion years, at last)
    // make the effort to loop for unlimited sleep, as it doesn't cost much
    uint64_t now;

    while ((now = Kernel::get_ms_count()) < millisec) {
        if (millisec - now > UINT32_MAX) {
            sleep_for(UINT32_MAX);
            continue;
        } else {
            sleep_for(millisec - now);
            break;
        }
    }
}

void ThisThread::yield()
{
    osThreadYield();
}

osThreadId_t ThisThread::get_id()
{
    return osThreadGetId();
}

const char *get_name()
{
    osThreadId_t id = osThreadGetId();
    if (id == NULL) {
        return NULL;
    }
    return osThreadGetName(id);
}

}
