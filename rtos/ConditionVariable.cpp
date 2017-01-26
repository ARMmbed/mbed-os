/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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
#include "rtos/Semaphore.h"
#include "rtos/Thread.h"

#include "mbed_error.h"
#include "mbed_assert.h"

namespace rtos {

#define RESUME_SIGNAL      (1 << 15)

struct Waiter {
    Waiter();
    Semaphore sem;
    Waiter *prev;
    Waiter *next;
    bool in_list;
};

Waiter::Waiter(): sem(0), prev(NULL), next(NULL), in_list(false)
{
    // No initialization to do
}

ConditionVariable::ConditionVariable(Mutex &mutex): _mutex(mutex), _wait_list(NULL)
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
    MBED_ASSERT(_mutex.get_owner() == Thread::gettid());
    MBED_ASSERT(_mutex._count == 1);
    _add_wait_list(&current_thread);

    _mutex.unlock();

    int32_t sem_count = current_thread.sem.wait(millisec);
    bool timeout = (sem_count > 0) ? false : true;

    _mutex.lock();

    if (current_thread.in_list) {
        _remove_wait_list(&current_thread);
    }

    return timeout;
}

void ConditionVariable::notify_one()
{
    MBED_ASSERT(_mutex.get_owner() == Thread::gettid());
    if (_wait_list != NULL) {
        _wait_list->sem.release();
        _remove_wait_list(_wait_list);
    }
}

void ConditionVariable::notify_all()
{
    MBED_ASSERT(_mutex.get_owner() == Thread::gettid());
    while (_wait_list != NULL) {
        _wait_list->sem.release();
        _remove_wait_list(_wait_list);
    }
}

void ConditionVariable::_add_wait_list(Waiter * waiter)
{
    if (NULL == _wait_list) {
        // Nothing in the list so add it directly.
        // Update prev pointer to reference self
        _wait_list = waiter;
        waiter->prev = waiter;
    } else {
        // Add after the last element
        Waiter *last = _wait_list->prev;
        last->next = waiter;
        waiter->prev = last;
        _wait_list->prev = waiter;
    }
    waiter->in_list = true;
}

void ConditionVariable::_remove_wait_list(Waiter * waiter)
{
    // Remove this element from the start of the list
    Waiter * next = waiter->next;
    if (waiter == _wait_list) {
        _wait_list = next;
    }
    if (next != NULL) {
        next = waiter->prev;
    }
    Waiter * prev = waiter->prev;
    if (prev != NULL) {
        prev = waiter->next;
    }
    waiter->next = NULL;
    waiter->prev = NULL;
    waiter->in_list = false;
}

ConditionVariable::~ConditionVariable()
{
    MBED_ASSERT(NULL == _wait_list);
}

}
