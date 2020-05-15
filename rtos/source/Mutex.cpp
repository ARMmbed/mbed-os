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
#include "rtos/Mutex.h"
#include "rtos/Kernel.h"

#include <string.h>
#include "platform/mbed_error.h"
#include "platform/mbed_assert.h"

#if MBED_CONF_RTOS_PRESENT

using namespace std::chrono_literals;
using std::milli;
using std::chrono::duration;

namespace rtos {

Mutex::Mutex()
{
    constructor();
}

Mutex::Mutex(const char *name)
{
    constructor(name);
}

void Mutex::constructor(const char *name)
{
    _count = 0;
    osMutexAttr_t attr =
    { 0 };
    attr.name = name ? name : "application_unnamed_mutex";
    attr.cb_mem = &_obj_mem;
    attr.cb_size = sizeof(_obj_mem);
    attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;
    _id = osMutexNew(&attr);
    // To permit certain cases where a device may get constructed in
    // by the attempt to print an error in a fatal shutdown, let a
    // mutex construction error pass.
    MBED_ASSERT(_id || mbed_get_error_in_progress());
}

void Mutex::lock(void)
{
    osStatus status = osMutexAcquire(_id, osWaitForever);
    if (osOK == status) {
        _count++;
    }

    if (status != osOK && !mbed_get_error_in_progress()) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_MUTEX_LOCK_FAILED), "Mutex lock failed", status);
    }
}

bool Mutex::trylock()
{
    return trylock_for(0s);
}

bool Mutex::trylock_for(uint32_t millisec)
{
    return trylock_for(duration<uint32_t, milli>(millisec));
}

bool Mutex::trylock_for(Kernel::Clock::duration_u32 rel_time)
{
    osStatus status = osMutexAcquire(_id, rel_time.count());
    if (status == osOK) {
        _count++;
        return true;
    }

    bool success = (status == osOK ||
                    (status == osErrorResource && rel_time == rel_time.zero()) ||
                    (status == osErrorTimeout && rel_time <= Kernel::wait_for_u32_max));

    if (!success && !mbed_get_error_in_progress()) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_MUTEX_LOCK_FAILED), "Mutex lock failed", status);
    }

    return false;
}

bool Mutex::trylock_until(uint64_t millisec)
{
    return trylock_until(Kernel::Clock::time_point(duration<uint64_t, milli>(millisec)));
}

bool Mutex::trylock_until(Kernel::Clock::time_point abs_time)
{
    Kernel::Clock::time_point now = Kernel::Clock::now();

    if (now >= abs_time) {
        return trylock();
    } else if (abs_time - now > Kernel::wait_for_u32_max) {
        // API permits early return
        return trylock_for(Kernel::wait_for_u32_max);
    } else {
        return trylock_for(abs_time - now);
    }
}

void Mutex::unlock()
{
    // Count must be adjusted inside the lock. This would leave it incorrect
    // on failure, but it only is used for an assert in ConditionVariable,
    // and a mutex release failure means MBED_ERROR anyway.
    _count--;

    osStatus status = osMutexRelease(_id);

    if (status != osOK && !mbed_get_error_in_progress()) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_MUTEX_UNLOCK_FAILED), "Mutex unlock failed", status);
    }
}

osThreadId Mutex::get_owner()
{
    return osMutexGetOwner(_id);
}

Mutex::~Mutex()
{
    osMutexDelete(_id);
}

}

#endif
