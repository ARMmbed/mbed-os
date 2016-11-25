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
#include "rtos/Thread.h"

#include "mbed.h"
#include "rtos/rtos_idle.h"

static void (*terminate_hook)(osThreadId_t id) = 0;
extern "C" void thread_terminate_hook(osThreadId_t id)
{
    if (terminate_hook != (void (*)(osThreadId_t))NULL) {
        terminate_hook(id);
    }
}

namespace rtos {

void Thread::constructor(osPriority_t priority,
        uint32_t stack_size, unsigned char *stack_mem) {
    _tid = 0;
    _dynamic_stack = (stack_mem == NULL);

#if defined(__MBED_CMSIS_RTOS_CA9) || defined(__MBED_CMSIS_RTOS_CM)
    _thread_attr.priority = priority;
    _thread_attr.stack_size = stack_size;
    _thread_attr.stack_mem = (uint32_t*)stack_mem;
#endif
}

void Thread::constructor(Callback<void()> task,
        osPriority_t priority, uint32_t stack_size, unsigned char *stack_mem) {
    constructor(priority, stack_size, stack_mem);

    switch (start(task)) {
        case osErrorResource:
            error("OS ran out of threads!\n");
            break;
        case osErrorParameter:
            error("Thread already running!\n");
            break;
        case osErrorNoMemory:
            error("Error allocating the stack memory\n");
        default:
            break;
    }
}

osStatus_t Thread::start(Callback<void()> task) {
    _mutex.lock();

    if (_tid != 0) {
        _mutex.unlock();
        return osErrorParameter;
    }

#if defined(__MBED_CMSIS_RTOS_CA9) || defined(__MBED_CMSIS_RTOS_CM)
    if (_thread_attr.stack_mem == NULL) {
        _thread_attr.stack_mem = new uint32_t[_thread_attr.stack_size/sizeof(uint32_t)];
        MBED_ASSERT(_thread_attr.stack_mem != NULL);
    }

    //Fill the stack with a magic word for maximum usage checking
    for (uint32_t i = 0; i < (_thread_attr.stack_size / sizeof(uint32_t)); i++) {
        ((uint32_t *)_thread_attr.stack_mem)[i] = 0xE25A2EA5;
    }

    _thread_attr.cb_size = sizeof(_ob_mem);
    _thread_attr.cb_mem = _ob_mem;
#endif
    _task = task;
    _tid = osThreadNew(Thread::_thunk, this, &_thread_attr);
    if (_tid == NULL) {
        if (_dynamic_stack) {
            delete[] (uint32_t *)(_thread_attr.stack_mem);
            _thread_attr.stack_mem = (uint32_t*)NULL;
        }
        _mutex.unlock();
        _join_sem.release();
        return osErrorResource;
    }

    _mutex.unlock();
    return osOK;
}

osStatus_t Thread::terminate() {
    osStatus_t ret;
    _mutex.lock();

    // Set the Thread's tid to NULL and
    // release the semaphore before terminating
    // since this thread could be terminating itself
    osThreadId_t local_id = _tid;
    _join_sem.release();
    _tid = (osThreadId_t)NULL;
    _mutex.unlock();

    ret = osThreadTerminate(local_id);

    return ret;
}

osStatus_t Thread::join() {
    int32_t ret = _join_sem.wait();
    if (ret < 0) {
        return osError;
    }

    // The semaphore has been released so this thread is being
    // terminated or has been terminated. Once the mutex has
    // been locked it is ensured that the thread is deleted.
    _mutex.lock();
    MBED_ASSERT(NULL == _tid);
    _mutex.unlock();

    // Release sem so any other threads joining this thread wake up
    _join_sem.release();
    return osOK;
}

osStatus_t Thread::set_priority(osPriority_t priority) {
    osStatus_t ret;
    _mutex.lock();

    ret = osThreadSetPriority(_tid, priority);

    _mutex.unlock();
    return ret;
}

osPriority_t Thread::get_priority() {
    osPriority_t ret;
    _mutex.lock();

    ret = osThreadGetPriority(_tid);

    _mutex.unlock();
    return ret;
}

int32_t Thread::signal_set(int32_t flags) {
    return osThreadFlagsSet(_tid, flags);
}

int32_t Thread::signal_clr(int32_t flags) {
    return osThreadFlagsClear(flags);
}

osThreadState_t Thread::get_state() {
    osThreadState_t status = osThreadError;
    _mutex.lock();

    if (_tid != NULL) {
        status = osThreadGetState(_tid);
    }

    _mutex.unlock();
    return status;
}

uint32_t Thread::stack_size() {
#ifndef __MBED_CMSIS_RTOS_CA9
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        os_thread_t *thread = (os_thread_t *)_tid;
        size = thread->stack_size;
    }

    _mutex.unlock();
    return size;
#else
    return 0;
#endif
}

uint32_t Thread::free_stack() {
#ifndef __MBED_CMSIS_RTOS_CA9
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        os_thread_t *thread = (os_thread_t *)_tid;
        size = (uint32_t)thread->stack_mem - thread->sp;
    }

    _mutex.unlock();
    return size;
#else
    return 0;
#endif
}

uint32_t Thread::used_stack() {
#ifndef __MBED_CMSIS_RTOS_CA9
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        os_thread_t *thread = (os_thread_t *)_tid;
        size = ((uint32_t)thread->stack_mem + thread->stack_size) - thread->sp;
    }

    _mutex.unlock();
    return size;
#else
    return 0;
#endif
}

uint32_t Thread::max_stack() {
#ifndef __MBED_CMSIS_RTOS_CA9
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        os_thread_t *thread = (os_thread_t *)_tid;
        uint32_t high_mark = 0;
        while (((uint32_t *)(thread->stack_mem))[high_mark] == 0xE25A2EA5)
            high_mark++;
        size = ((uint32_t)thread->stack_mem + thread->stack_size) - (high_mark * 4);
    }

    _mutex.unlock();
    return size;
#else
    return 0;
#endif
}

int32_t Thread::signal_wait(int32_t flags, uint32_t millisec) {
    return osThreadFlagsWait(flags, 0, millisec);
}

osStatus_t Thread::wait(uint32_t millisec) {
    return osDelay(millisec);
}

osStatus_t Thread::yield() {
    return osThreadYield();
}

osThreadId_t Thread::gettid() {
    return osThreadGetId();
}

void Thread::attach_idle_hook(void (*fptr)(void)) {
    rtos_attach_idle_hook(fptr);
}

void Thread::attach_terminate_hook(void (*fptr)(osThreadId_t id)) {
    terminate_hook = fptr;
}

Thread::~Thread() {
    // terminate is thread safe
    terminate();
#ifdef __MBED_CMSIS_RTOS_CM
    if (_dynamic_stack) {
        delete[] (uint32_t*)(_thread_attr.stack_mem);
        _thread_attr.stack_mem = (uint32_t*)NULL;
    }
#endif
}

void Thread::_thunk(void * thread_ptr)
{
    Thread *t = (Thread*)thread_ptr;
    t->_task();
    t->_mutex.lock();
    t->_tid = (osThreadId_t)NULL;
    t->_mutex.unlock();
    t->_join_sem.release();
}

}
