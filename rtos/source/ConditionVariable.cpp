/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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
#include "rtos/ConditionVariable.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"

#include "platform/mbed_error.h"
#include "platform/mbed_assert.h"

#if MBED_CONF_RTOS_PRESENT

namespace rtos {

ConditionVariable::Waiter::Waiter(): sem(0), prev(nullptr), next(nullptr), in_list(false)
{
    // No initialization to do
}

ConditionVariable::ConditionVariable(Mutex &mutex): _mutex(mutex), _wait_list(nullptr)
{
    // No initialization to do
}

void ConditionVariable::wait()
{
    wait_for(osWaitForever);
}

bool ConditionVariable::wait_for(uint32_t millisec)
{
    Waiter current_thread;
    MBED_ASSERT(_mutex.get_owner() == ThisThread::get_id());
    MBED_ASSERT(_mutex._count == 1);
    _add_wait_list(&_wait_list, &current_thread);

    _mutex.unlock();

    bool timeout = !current_thread.sem.try_acquire_for(millisec);

    _mutex.lock();

    if (current_thread.in_list) {
        _remove_wait_list(&_wait_list, &current_thread);
    }

    return timeout;
}

bool ConditionVariable::wait_until(uint64_t millisec)
{
    uint64_t now = Kernel::get_ms_count();

    if (now >= millisec) {
        // Time has already passed - standard behaviour is to
        // treat as a "try".
        return wait_for(0);
    } else if (millisec - now >= osWaitForever) {
        // Exceeds maximum delay of underlying wait_for -
        // spuriously wake after 49 days, indicating no timeout.
        wait_for(osWaitForever - 1);
        return false;
    } else {
        return wait_for(millisec - now);
    }
}

void ConditionVariable::notify_one()
{
    MBED_ASSERT(_mutex.get_owner() == ThisThread::get_id());
    if (_wait_list != nullptr) {
        _wait_list->sem.release();
        _remove_wait_list(&_wait_list, _wait_list);
    }
}

void ConditionVariable::notify_all()
{
    MBED_ASSERT(_mutex.get_owner() == ThisThread::get_id());
    while (_wait_list != nullptr) {
        _wait_list->sem.release();
        _remove_wait_list(&_wait_list, _wait_list);
    }
}

void ConditionVariable::_add_wait_list(Waiter **wait_list, Waiter *waiter)
{
    if (nullptr == *wait_list) {
        // Nothing in the list so add it directly.
        // Update prev and next pointer to reference self
        *wait_list = waiter;
        waiter->next = waiter;
        waiter->prev = waiter;
    } else {
        // Add after the last element
        Waiter *first = *wait_list;
        Waiter *last = (*wait_list)->prev;

        // Update new entry
        waiter->next = first;
        waiter->prev = last;

        // Insert into the list
        first->prev = waiter;
        last->next = waiter;
    }
    waiter->in_list = true;
}

void ConditionVariable::_remove_wait_list(Waiter **wait_list, Waiter *waiter)
{
    Waiter *prev = waiter->prev;
    Waiter *next = waiter->next;

    // Remove from list
    prev->next = waiter->next;
    next->prev = waiter->prev;
    *wait_list = waiter->next;

    if (*wait_list == waiter) {
        // This was the last element in the list
        *wait_list = nullptr;
    }

    // Invalidate pointers
    waiter->next = nullptr;
    waiter->prev = nullptr;
    waiter->in_list = false;
}

ConditionVariable::~ConditionVariable()
{
    MBED_ASSERT(nullptr == _wait_list);
}

}

#endif
