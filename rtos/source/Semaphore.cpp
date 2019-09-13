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
#include "rtos/Semaphore.h"
#include "rtos/Kernel.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_error.h"
#include "platform/source/mbed_os_timer.h"

#include <string.h>

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

#if MBED_CONF_RTOS_PRESENT
/* To sidestep deprecation warnings */
int32_t Semaphore::_wait(uint32_t millisec)
{
    osStatus_t stat = osSemaphoreAcquire(_id, millisec);
    switch (stat) {
        case osOK:
            return osSemaphoreGetCount(_id) + 1;
        case osErrorTimeout:
        case osErrorResource:
            return 0;
        case osErrorParameter:
        default:
            return -1;
    }
}
#endif

int32_t Semaphore::wait(uint32_t millisec)
{
#if MBED_CONF_RTOS_PRESENT
    return _wait(millisec);
#else
    sem_wait_capture capture = { this, false };
    mbed::internal::do_timed_sleep_relative_or_forever(millisec, semaphore_available, &capture);
    if (capture.acquired) {
        return core_util_atomic_load_s32(&_count) + 1;
    } else {
        return 0;
    }
#endif
}

int32_t Semaphore::wait_until(uint64_t millisec)
{
#if MBED_CONF_RTOS_PRESENT
    uint64_t now = Kernel::get_ms_count();

    if (now >= millisec) {
        return _wait(0);
    } else if (millisec - now >= osWaitForever) {
        // API permits early return
        return _wait(osWaitForever - 1);
    } else {
        return _wait(millisec - now);
    }
#else
    sem_wait_capture capture = { this, false };
    mbed::internal::do_timed_sleep_absolute(millisec, semaphore_available, &capture);
    if (capture.acquired) {
        return core_util_atomic_load_s32(&_count) + 1;
    } else {
        return 0;
    }
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
#if MBED_CONF_RTOS_PRESENT
    osStatus_t status = osSemaphoreAcquire(_id, millisec);
    if (status == osOK) {
        return true;
    }

    bool success = (status == osErrorResource && millisec == 0) ||
                   (status == osErrorTimeout && millisec != osWaitForever);

    if (!success) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_SEMAPHORE_LOCK_FAILED), "Semaphore acquire failed", status);
    }
    return false;
#else
    sem_wait_capture capture = { this, false };
    mbed::internal::do_timed_sleep_relative_or_forever(millisec, semaphore_available, &capture);
    return capture.acquired;
#endif
}

bool Semaphore::try_acquire_until(uint64_t millisec)
{
#if MBED_CONF_RTOS_PRESENT
    uint64_t now = Kernel::get_ms_count();

    if (now >= millisec) {
        return try_acquire();
    } else if (millisec - now >= osWaitForever) {
        // API permits early return
        return try_acquire_for(osWaitForever - 1);
    } else {
        return try_acquire_for(millisec - now);
    }
#else
    sem_wait_capture capture = { this, false };
    mbed::internal::do_timed_sleep_absolute(millisec, semaphore_available, &capture);
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
#endif
    return osOK;
}

Semaphore::~Semaphore()
{
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreDelete(_id);
#endif
}

}
