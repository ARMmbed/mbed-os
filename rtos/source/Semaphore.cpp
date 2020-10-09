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
#include "rtos/Semaphore.h"
#include "rtos/Kernel.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_error.h"
#include "platform/internal/mbed_os_timer.h"

#include <string.h>

using namespace std::chrono_literals;
using std::chrono::duration;
using std::milli;

namespace rtos {

Semaphore::Semaphore(int32_t count)
{
    constructor(count, 0xffff);
}

Semaphore::Semaphore(int32_t count, uint16_t max_count)
{
    constructor(count, max_count);
}

void Semaphore::constructor(int32_t count, uint16_t max_count)
{
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreAttr_t attr = { 0 };
    attr.cb_mem = &_obj_mem;
    attr.cb_size = sizeof(_obj_mem);
    _id = osSemaphoreNew(max_count, count, &attr);
    MBED_ASSERT(_id != nullptr);
#else
    _count = count;
    _max_count = max_count;
#endif
}

#if !MBED_CONF_RTOS_PRESENT
struct sem_wait_capture {
    Semaphore *sem;
    bool acquired;
};

bool Semaphore::semaphore_available(void *handle)
{
    sem_wait_capture *wait = static_cast<sem_wait_capture *>(handle);
    return wait->acquired = wait->sem->try_acquire();
}
#endif

bool Semaphore::try_acquire()
{
#if MBED_CONF_RTOS_PRESENT
    osStatus_t status = osSemaphoreAcquire(_id, 0);
    if (status != osOK && status != osErrorResource) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_SEMAPHORE_LOCK_FAILED), "Semaphore acquire failed", status);
    }
    return status == osOK;
#else
    int32_t old_count = core_util_atomic_load_s32(&_count);
    do {
        if (old_count == 0) {
            return false;
        }
    } while (!core_util_atomic_cas_s32(&_count, &old_count, old_count - 1));
    return true;
#endif
}

void Semaphore::acquire()
{
#if MBED_CONF_RTOS_PRESENT
    osStatus_t status = osSemaphoreAcquire(_id, osWaitForever);
    if (status != osOK) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_SEMAPHORE_LOCK_FAILED), "Semaphore acquire failed", status);
    }
#else
    sem_wait_capture capture = { this, false };
    mbed::internal::do_untimed_sleep(semaphore_available, &capture);
    if (!capture.acquired) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_SEMAPHORE_LOCK_FAILED), "Semaphore acquire failed");
    }
#endif
}

bool Semaphore::try_acquire_for(uint32_t millisec)
{
    return try_acquire_for(duration<uint32_t, milli>(millisec));
}

bool Semaphore::try_acquire_for(Kernel::Clock::duration_u32 rel_time)
{
#if MBED_CONF_RTOS_PRESENT
    osStatus_t status = osSemaphoreAcquire(_id, rel_time.count());
    if (status == osOK) {
        return true;
    }

    bool success = (status == osErrorResource && rel_time == rel_time.zero()) ||
                   (status == osErrorTimeout);

    if (!success) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_SEMAPHORE_LOCK_FAILED), "Semaphore acquire failed", status);
    }
    return false;
#else
    sem_wait_capture capture = { this, false };
    mbed::internal::do_timed_sleep_relative_or_forever(rel_time, semaphore_available, &capture);
    return capture.acquired;
#endif
}

bool Semaphore::try_acquire_until(uint64_t millisec)
{
    return try_acquire_until(Kernel::Clock::time_point(duration<uint64_t, milli>(millisec)));
}

bool Semaphore::try_acquire_until(Kernel::Clock::time_point abs_time)
{
#if MBED_CONF_RTOS_PRESENT
    Kernel::Clock::time_point now = Kernel::Clock::now();

    if (now >= abs_time) {
        return try_acquire();
    } else if (abs_time - now > Kernel::wait_for_u32_max) {
        // API permits early return
        return try_acquire_for(Kernel::wait_for_u32_max);
    } else {
        return try_acquire_for(abs_time - now);
    }
#else
    sem_wait_capture capture = { this, false };
    mbed::internal::do_timed_sleep_absolute(abs_time, semaphore_available, &capture);
    return capture.acquired;
#endif
}

osStatus Semaphore::release(void)
{
#if MBED_CONF_RTOS_PRESENT
    return osSemaphoreRelease(_id);
#else
    int32_t old_count = core_util_atomic_load_s32(&_count);
    do {
        if (old_count == _max_count) {
            return osErrorResource;
        }
    } while (!core_util_atomic_cas_s32(&_count, &old_count, old_count + 1));

    return osOK;
#endif // MBED_CONF_RTOS_PRESENT
}

Semaphore::~Semaphore()
{
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreDelete(_id);
#endif
}

}
