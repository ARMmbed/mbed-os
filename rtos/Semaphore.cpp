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
#include "platform/mbed_error.h"

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
    osSemaphoreAttr_t attr = { 0 };
    attr.cb_mem = &_obj_mem;
    attr.cb_size = sizeof(_obj_mem);
    _id = osSemaphoreNew(max_count, count, &attr);
    MBED_ASSERT(_id != NULL);
}

bool Semaphore::try_acquire()
{
    osStatus_t status = osSemaphoreAcquire(_id, 0);
    if (status != osOK && status != osErrorResource) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_SEMAPHORE_LOCK_FAILED), "Semaphore acquire failed", status);
    }
    return status == osOK;
}

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

int32_t Semaphore::wait(uint32_t millisec)
{
    return _wait(millisec);
}

int32_t Semaphore::wait_until(uint64_t millisec)
{
    uint64_t now = Kernel::get_ms_count();

    if (now >= millisec) {
        return _wait(0);
    } else if (millisec - now >= osWaitForever) {
        // API permits early return
        return _wait(osWaitForever - 1);
    } else {
        return _wait(millisec - now);
    }
}

void Semaphore::acquire()
{
    osStatus_t status = osSemaphoreAcquire(_id, osWaitForever);
    if (status != osOK) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_SEMAPHORE_LOCK_FAILED), "Semaphore acquire failed", status);
    }
}

bool Semaphore::try_acquire_for(uint32_t millisec)
{
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
}

bool Semaphore::try_acquire_until(uint64_t millisec)
{
    uint64_t now = Kernel::get_ms_count();

    if (now >= millisec) {
        return try_acquire();
    } else if (millisec - now >= osWaitForever) {
        // API permits early return
        return try_acquire_for(osWaitForever - 1);
    } else {
        return try_acquire_for(millisec - now);
    }
}

osStatus Semaphore::release(void)
{
    return osSemaphoreRelease(_id);
}

Semaphore::~Semaphore()
{
    osSemaphoreDelete(_id);
}

}
