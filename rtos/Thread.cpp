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

// rt_tid2ptcb is an internal function which we exposed to get TCB for thread id
#undef NULL  //Workaround for conflicting macros in rt_TypeDef.h and stdio.h
#include "rt_TypeDef.h"

extern "C" P_TCB rt_tid2ptcb(osThreadId thread_id);


static void (*terminate_hook)(osThreadId id) = 0;
extern "C" void thread_terminate_hook(osThreadId id)
{
    if (terminate_hook != (void (*)(osThreadId))NULL) {
        terminate_hook(id);
    }
}

namespace rtos {

void Thread::constructor(osPriority priority,
        uint32_t stack_size, unsigned char *stack_pointer) {
    _tid = 0;
    _dynamic_stack = (stack_pointer == NULL);

#if defined(__MBED_CMSIS_RTOS_CA9) || defined(__MBED_CMSIS_RTOS_CM)
    _thread_def.tpriority = priority;
    _thread_def.stacksize = stack_size;
    _thread_def.stack_pointer = (uint32_t*)stack_pointer;
#endif
}

void Thread::constructor(Callback<void()> task,
        osPriority priority, uint32_t stack_size, unsigned char *stack_pointer) {
    constructor(priority, stack_size, stack_pointer);

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

osStatus Thread::start(Callback<void()> task) {
    _mutex.lock();

    if (_tid != 0) {
        _mutex.unlock();
        return osErrorParameter;
    }

#if defined(__MBED_CMSIS_RTOS_CA9) || defined(__MBED_CMSIS_RTOS_CM)
    _thread_def.pthread = Thread::_thunk;
    if (_thread_def.stack_pointer == NULL) {
        _thread_def.stack_pointer = new uint32_t[_thread_def.stacksize/sizeof(uint32_t)];
        MBED_ASSERT(_thread_def.stack_pointer != NULL);
    }

    //Fill the stack with a magic word for maximum usage checking
    for (uint32_t i = 0; i < (_thread_def.stacksize / sizeof(uint32_t)); i++) {
        _thread_def.stack_pointer[i] = 0xE25A2EA5;
    }
#endif
    _task = task;
    _tid = osThreadCreate(&_thread_def, this);
    if (_tid == NULL) {
        if (_dynamic_stack) {
            delete[] (_thread_def.stack_pointer);
            _thread_def.stack_pointer = (uint32_t*)NULL;
        }
        _mutex.unlock();
        _join_sem.release();
        return osErrorResource;
    }

    _mutex.unlock();
    return osOK;
}

osStatus Thread::terminate() {
    osStatus ret;
    _mutex.lock();

    // Set the Thread's tid to NULL and
    // release the semaphore before terminating
    // since this thread could be terminating itself
    osThreadId local_id = _tid;
    _join_sem.release();
    _tid = (osThreadId)NULL;

    ret = osThreadTerminate(local_id);

    _mutex.unlock();
    return ret;
}

osStatus Thread::join() {
    int32_t ret = _join_sem.wait();
    if (ret < 0) {
        return osErrorOS;
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

osStatus Thread::set_priority(osPriority priority) {
    osStatus ret;
    _mutex.lock();

    ret = osThreadSetPriority(_tid, priority);

    _mutex.unlock();
    return ret;
}

osPriority Thread::get_priority() {
    osPriority ret;
    _mutex.lock();

    ret = osThreadGetPriority(_tid);

    _mutex.unlock();
    return ret;
}

int32_t Thread::signal_set(int32_t signals) {
    // osSignalSet is thread safe as long as the underlying
    // thread does not get terminated or return from main
    return osSignalSet(_tid, signals);
}

int32_t Thread::signal_clr(int32_t signals) {
    // osSignalClear is thread safe as long as the underlying
    // thread does not get terminated or return from main
    return osSignalClear(_tid, signals);
}

Thread::State Thread::get_state() {
#if !defined(__MBED_CMSIS_RTOS_CA9) && !defined(__MBED_CMSIS_RTOS_CM)
#ifdef CMSIS_OS_RTX
    State status = Deleted;
    _mutex.lock();

    if (_tid != NULL) {
        status = (State)_thread_def.tcb.state;
    }

    _mutex.unlock();
    return status;
#endif
#else
    State status = Deleted;
    _mutex.lock();

    if (_tid != NULL) {
        status = (State)osThreadGetState(_tid);
    }

    _mutex.unlock();
    return status;
#endif
}

uint32_t Thread::stack_size() {
#ifndef __MBED_CMSIS_RTOS_CA9
#if defined(CMSIS_OS_RTX) && !defined(__MBED_CMSIS_RTOS_CM)
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        size = _thread_def.tcb.priv_stack;
    }

    _mutex.unlock();
    return size;
#else
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        P_TCB tcb = rt_tid2ptcb(_tid);
        size = tcb->priv_stack;
    }

    _mutex.unlock();
    return size;
#endif
#else
    return 0;
#endif
}

uint32_t Thread::free_stack() {
#ifndef __MBED_CMSIS_RTOS_CA9
#if defined(CMSIS_OS_RTX) && !defined(__MBED_CMSIS_RTOS_CM)
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        uint32_t bottom = (uint32_t)_thread_def.tcb.stack;
        size = _thread_def.tcb.tsk_stack - bottom;
    }

    _mutex.unlock();
    return size;
#else
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        P_TCB tcb = rt_tid2ptcb(_tid);
        uint32_t bottom = (uint32_t)tcb->stack;
        size = tcb->tsk_stack - bottom;
    }

    _mutex.unlock();
    return size;
#endif
#else
    return 0;
#endif
}

uint32_t Thread::used_stack() {
#ifndef __MBED_CMSIS_RTOS_CA9
#if defined(CMSIS_OS_RTX) && !defined(__MBED_CMSIS_RTOS_CM)
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        uint32_t top = (uint32_t)_thread_def.tcb.stack + _thread_def.tcb.priv_stack;
        size = top - _thread_def.tcb.tsk_stack;
    }

    _mutex.unlock();
    return size;
#else
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        P_TCB tcb = rt_tid2ptcb(_tid);
        uint32_t top = (uint32_t)tcb->stack + tcb->priv_stack;
        size =  top - tcb->tsk_stack;
    }

    _mutex.unlock();
    return size;
#endif
#else
    return 0;
#endif
}

uint32_t Thread::max_stack() {
#ifndef __MBED_CMSIS_RTOS_CA9
#if defined(CMSIS_OS_RTX) && !defined(__MBED_CMSIS_RTOS_CM)
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        uint32_t high_mark = 0;
        while (_thread_def.tcb.stack[high_mark] == 0xE25A2EA5)
            high_mark++;
        size = _thread_def.tcb.priv_stack - (high_mark * 4);
    }

    _mutex.unlock();
    return size;
#else
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != NULL) {
        P_TCB tcb = rt_tid2ptcb(_tid);
        uint32_t high_mark = 0;
        while (tcb->stack[high_mark] == 0xE25A2EA5)
            high_mark++;
        size = tcb->priv_stack - (high_mark * 4);
    }

    _mutex.unlock();
    return size;
#endif
#else
    return 0;
#endif
}

osEvent Thread::signal_wait(int32_t signals, uint32_t millisec) {
    return osSignalWait(signals, millisec);
}

osStatus Thread::wait(uint32_t millisec) {
    return osDelay(millisec);
}

osStatus Thread::yield() {
    return osThreadYield();
}

osThreadId Thread::gettid() {
    return osThreadGetId();
}

void Thread::attach_idle_hook(void (*fptr)(void)) {
    rtos_attach_idle_hook(fptr);
}

void Thread::attach_terminate_hook(void (*fptr)(osThreadId id)) {
    terminate_hook = fptr;
}

Thread::~Thread() {
    // terminate is thread safe
    terminate();
#ifdef __MBED_CMSIS_RTOS_CM
    if (_dynamic_stack) {
        delete[] (_thread_def.stack_pointer);
        _thread_def.stack_pointer = (uint32_t*)NULL;
    }
#endif
}

void Thread::_thunk(const void * thread_ptr)
{
    Thread *t = (Thread*)thread_ptr;
    t->_task();
    t->_mutex.lock();
    t->_tid = (osThreadId)NULL;
    t->_join_sem.release();
    // rtos will release the mutex automatically
}

}
