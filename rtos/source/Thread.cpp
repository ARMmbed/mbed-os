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
#include "rtos/ThisThread.h"
#include "rtos_idle.h"
#include "rtos_handlers.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_error.h"

#if MBED_CONF_RTOS_PRESENT

#define ALIGN_UP(pos, align) ((pos) % (align) ? (pos) +  ((align) - (pos) % (align)) : (pos))
MBED_STATIC_ASSERT(ALIGN_UP(0, 8) == 0, "ALIGN_UP macro error");
MBED_STATIC_ASSERT(ALIGN_UP(1, 8) == 8, "ALIGN_UP macro error");

#define ALIGN_DOWN(pos, align) ((pos) - ((pos) % (align)))
MBED_STATIC_ASSERT(ALIGN_DOWN(7, 8) == 0, "ALIGN_DOWN macro error");
MBED_STATIC_ASSERT(ALIGN_DOWN(8, 8) == 8, "ALIGN_DOWN macro error");

namespace rtos {

#ifndef MBED_TZ_DEFAULT_ACCESS
#define MBED_TZ_DEFAULT_ACCESS   0
#endif

void Thread::constructor(uint32_t tz_module, osPriority priority,
                         uint32_t stack_size, unsigned char *stack_mem, const char *name)
{

    const uintptr_t unaligned_mem = reinterpret_cast<uintptr_t>(stack_mem);
    const uintptr_t aligned_mem = ALIGN_UP(unaligned_mem, 8);
    const uint32_t offset = aligned_mem - unaligned_mem;
    const uint32_t aligned_size = ALIGN_DOWN(stack_size - offset, 8);

    memset(&_obj_mem, 0, sizeof(_obj_mem));
    _tid = nullptr;
    _dynamic_stack = (stack_mem == nullptr);
    _finished = false;
    memset(&_attr, 0, sizeof(_attr));
    _attr.priority = priority;
    _attr.stack_size = aligned_size;
    _attr.name = name ? name : "application_unnamed_thread";
    _attr.stack_mem = reinterpret_cast<uint32_t *>(aligned_mem);
    _attr.tz_module = tz_module;
}

void Thread::constructor(osPriority priority,
                         uint32_t stack_size, unsigned char *stack_mem, const char *name)
{
    constructor(MBED_TZ_DEFAULT_ACCESS, priority, stack_size, stack_mem, name);
}

void Thread::constructor(mbed::Callback<void()> task,
                         osPriority priority, uint32_t stack_size, unsigned char *stack_mem, const char *name)
{
    constructor(MBED_TZ_DEFAULT_ACCESS, priority, stack_size, stack_mem, name);

    switch (start(task)) {
        case osErrorResource:
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_OUT_OF_RESOURCES), "OS ran out of threads!\n", task);
            break;
        case osErrorParameter:
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_ALREADY_IN_USE), "Thread already running!\n", task);
            break;
        case osErrorNoMemory:
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_OUT_OF_MEMORY), "Error allocating the stack memory\n", task);
        default:
            break;
    }
}

osStatus Thread::start(mbed::Callback<void()> task)
{
    _mutex.lock();

    if ((_tid != 0) || _finished) {
        _mutex.unlock();
        return osErrorParameter;
    }

    if (_attr.stack_mem == nullptr) {
        _attr.stack_mem = new uint32_t[_attr.stack_size / sizeof(uint32_t)];
        MBED_ASSERT(_attr.stack_mem != nullptr);
    }

    //Fill the stack with a magic word for maximum usage checking
    for (uint32_t i = 0; i < (_attr.stack_size / sizeof(uint32_t)); i++) {
        ((uint32_t *)_attr.stack_mem)[i] = osRtxStackMagicWord;
    }

    _attr.cb_size = sizeof(_obj_mem);
    _attr.cb_mem = &_obj_mem;
    _task = task;
    _tid = osThreadNew(Thread::_thunk, this, &_attr);
    if (_tid == nullptr) {
        if (_dynamic_stack) {
            // Cast before deallocation as delete[] does not accept void*
            delete[] static_cast<uint32_t *>(_attr.stack_mem);
            _attr.stack_mem = nullptr;
        }
        _mutex.unlock();
        _join_sem.release();
        return osErrorResource;
    }

    _mutex.unlock();
    return osOK;
}

osStatus Thread::terminate()
{
    osStatus_t ret = osOK;
    _mutex.lock();

    // Set the Thread's tid to nullptr and
    // release the semaphore before terminating
    // since this thread could be terminating itself
    osThreadId_t local_id = _tid;
    _join_sem.release();
    _tid = nullptr;
    if (!_finished) {
        _finished = true;
        // if local_id == 0 Thread was not started in first place
        // and does not have to be terminated
        if (local_id != 0) {
            ret = osThreadTerminate(local_id);
        }
    }
    _mutex.unlock();
    return ret;
}

osStatus Thread::join()
{
    _join_sem.acquire();

    // The semaphore has been released so this thread is being
    // terminated or has been terminated. Once the mutex has
    // been locked it is ensured that the thread is deleted.
    _mutex.lock();
    MBED_ASSERT(nullptr == _tid);
    _mutex.unlock();

    // Release sem so any other threads joining this thread wake up
    _join_sem.release();
    return osOK;
}

osStatus Thread::set_priority(osPriority priority)
{
    osStatus_t ret;
    _mutex.lock();

    ret = osThreadSetPriority(_tid, priority);

    _mutex.unlock();
    return ret;
}

osPriority Thread::get_priority() const
{
    osPriority_t ret;
    _mutex.lock();

    ret = osThreadGetPriority(_tid);

    _mutex.unlock();
    return ret;
}

uint32_t Thread::flags_set(uint32_t flags)
{
    flags = osThreadFlagsSet(_tid, flags);
    MBED_ASSERT(!(flags & osFlagsError));
    return flags;
}

int32_t Thread::signal_set(int32_t flags)
{
    return osThreadFlagsSet(_tid, flags);
}

Thread::State Thread::get_state() const
{
    uint8_t state = osThreadTerminated;

    _mutex.lock();

    if (_tid != nullptr) {
#if defined(MBED_OS_BACKEND_RTX5)
        state = _obj_mem.state;
#else
        state = osThreadGetState(_tid);
#endif
    }

    _mutex.unlock();

    State user_state;

    switch (state) {
        case osThreadInactive:
            user_state = Inactive;
            break;
        case osThreadReady:
            user_state = Ready;
            break;
        case osThreadRunning:
            user_state = Running;
            break;
#if defined(MBED_OS_BACKEND_RTX5)
        case osRtxThreadWaitingDelay:
            user_state = WaitingDelay;
            break;
        case osRtxThreadWaitingJoin:
            user_state = WaitingJoin;
            break;
        case osRtxThreadWaitingThreadFlags:
            user_state = WaitingThreadFlag;
            break;
        case osRtxThreadWaitingEventFlags:
            user_state = WaitingEventFlag;
            break;
        case osRtxThreadWaitingMutex:
            user_state = WaitingMutex;
            break;
        case osRtxThreadWaitingSemaphore:
            user_state = WaitingSemaphore;
            break;
        case osRtxThreadWaitingMemoryPool:
            user_state = WaitingMemoryPool;
            break;
        case osRtxThreadWaitingMessageGet:
            user_state = WaitingMessageGet;
            break;
        case osRtxThreadWaitingMessagePut:
            user_state = WaitingMessagePut;
            break;
#endif
        case osThreadTerminated:
        default:
            user_state = Deleted;
            break;
    }

    return user_state;
}

uint32_t Thread::stack_size() const
{
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != nullptr) {
        size = osThreadGetStackSize(_tid);
    }

    _mutex.unlock();
    return size;
}

uint32_t Thread::free_stack() const
{
    uint32_t size = 0;
    _mutex.lock();

#if defined(MBED_OS_BACKEND_RTX5)
    if (_tid != nullptr) {
        mbed_rtos_storage_thread_t *thread = (mbed_rtos_storage_thread_t *)_tid;
        size = (uint32_t)thread->sp - (uint32_t)thread->stack_mem;
    }
#endif

    _mutex.unlock();
    return size;
}

uint32_t Thread::used_stack() const
{
    uint32_t size = 0;
    _mutex.lock();

#if defined(MBED_OS_BACKEND_RTX5)
    if (_tid != nullptr) {
        mbed_rtos_storage_thread_t *thread = (mbed_rtos_storage_thread_t *)_tid;
        size = ((uint32_t)thread->stack_mem + thread->stack_size) - thread->sp;
    }
#endif

    _mutex.unlock();
    return size;
}

uint32_t Thread::max_stack() const
{
    uint32_t size = 0;
    _mutex.lock();

    if (_tid != nullptr) {
#if defined(MBED_OS_BACKEND_RTX5)
        mbed_rtos_storage_thread_t *thread = (mbed_rtos_storage_thread_t *)_tid;
        uint32_t high_mark = 0;
        while ((((uint32_t *)(thread->stack_mem))[high_mark] == osRtxStackMagicWord) || (((uint32_t *)(thread->stack_mem))[high_mark] == osRtxStackFillPattern)) {
            high_mark++;
        }
        size = thread->stack_size - (high_mark * sizeof(uint32_t));
#else
        size = osThreadGetStackSize(_tid) - osThreadGetStackSpace(_tid);
#endif
    }

    _mutex.unlock();
    return size;
}

const char *Thread::get_name() const
{
    return _attr.name;
}

osThreadId_t Thread::get_id() const
{
    return _tid;
}

int32_t Thread::signal_clr(int32_t flags)
{
    return osThreadFlagsClear(flags);
}

osEvent Thread::signal_wait(int32_t signals, uint32_t millisec)
{
    uint32_t res;
    osEvent evt;
    uint32_t options = osFlagsWaitAll;
    if (signals == 0) {
        options = osFlagsWaitAny;
        signals = 0x7FFFFFFF;
    }
    res = osThreadFlagsWait(signals, options, millisec);
    if (res & osFlagsError) {
        switch (res) {
            case osFlagsErrorISR:
                evt.status = osErrorISR;
                break;
            case osFlagsErrorResource:
                evt.status = osOK;
                break;
            case osFlagsErrorTimeout:
                evt.status = (osStatus)osEventTimeout;
                break;
            case osFlagsErrorParameter:
            default:
                evt.status = (osStatus)osErrorValue;
                break;
        }
    } else {
        evt.status = (osStatus)osEventSignal;
        evt.value.signals = res;
    }

    return evt;
}

osStatus Thread::wait(uint32_t millisec)
{
    ThisThread::sleep_for(millisec);
    return osOK;
}

osStatus Thread::wait_until(uint64_t millisec)
{
    ThisThread::sleep_until(millisec);
    return osOK;
}

osStatus Thread::yield()
{
    return osThreadYield();
}

osThreadId Thread::gettid()
{
    return osThreadGetId();
}

void Thread::attach_idle_hook(void (*fptr)(void))
{
    rtos_attach_idle_hook(fptr);
}

void Thread::attach_terminate_hook(void (*fptr)(osThreadId_t id))
{
    rtos_attach_thread_terminate_hook(fptr);
}

Thread::~Thread()
{
    // terminate is thread safe
    terminate();
    if (_dynamic_stack) {
        // Cast before deallocation as delete[] does not accept void*
        delete[] static_cast<uint32_t *>(_attr.stack_mem);
        _attr.stack_mem = nullptr;
    }
}

void Thread::_thunk(void *thread_ptr)
{
    Thread *t = (Thread *)thread_ptr;
    t->_task();
    t->_mutex.lock();
    t->_tid = nullptr;
    t->_finished = true;
    t->_join_sem.release();
    // rtos will release the mutex automatically
}

}

#endif
