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
#include "Thread.h"

#include "mbed_error.h"
#include "rtos_idle.h"

// rt_tid2ptcb is an internal function which we exposed to get TCB for thread id
#undef NULL  //Workaround for conflicting macros in rt_TypeDef.h and stdio.h
#include "rt_TypeDef.h"

extern "C" P_TCB rt_tid2ptcb(osThreadId thread_id);

namespace rtos {

Thread::Thread(void (*task)(void const *argument), void *argument,
        osPriority priority, uint32_t stack_size, unsigned char *stack_pointer) {
#if defined(__MBED_CMSIS_RTOS_CA9) || defined(__MBED_CMSIS_RTOS_CM)
    _thread_def.pthread = task;
    _thread_def.tpriority = priority;
    _thread_def.stacksize = stack_size;
    if (stack_pointer != NULL) {
        _thread_def.stack_pointer = (uint32_t*)stack_pointer;
        _dynamic_stack = false;
    } else {
        _thread_def.stack_pointer = new uint32_t[stack_size/sizeof(uint32_t)];
        if (_thread_def.stack_pointer == NULL)
            error("Error allocating the stack memory\n");
        _dynamic_stack = true;
    }
    
    //Fill the stack with a magic word for maximum usage checking
    for (uint32_t i = 0; i < (stack_size / sizeof(uint32_t)); i++) {
        _thread_def.stack_pointer[i] = 0xE25A2EA5;
    }
#endif
    _tid = osThreadCreate(&_thread_def, argument);
}

osStatus Thread::terminate() {
    return osThreadTerminate(_tid);
}

osStatus Thread::set_priority(osPriority priority) {
    return osThreadSetPriority(_tid, priority);
}

osPriority Thread::get_priority() {
    return osThreadGetPriority(_tid);
}

int32_t Thread::signal_set(int32_t signals) {
    return osSignalSet(_tid, signals);
}

int32_t Thread::signal_clr(int32_t signals) {
    return osSignalClear(_tid, signals);
}

Thread::State Thread::get_state() {
#if !defined(__MBED_CMSIS_RTOS_CA9) && !defined(__MBED_CMSIS_RTOS_CM)
#ifdef CMSIS_OS_RTX
    return ((State)_thread_def.tcb.state);
#endif
#else
    uint8_t status;
    status = osThreadGetState(_tid);
    return ((State)status);
#endif
}

uint32_t Thread::stack_size() {
#ifndef __MBED_CMSIS_RTOS_CA9
#if defined(CMSIS_OS_RTX) && !defined(__MBED_CMSIS_RTOS_CM)
    return _thread_def.tcb.priv_stack;
#else
    P_TCB tcb = rt_tid2ptcb(_tid);
    return tcb->priv_stack;
#endif
#else
    return 0;
#endif
}

uint32_t Thread::free_stack() {
#ifndef __MBED_CMSIS_RTOS_CA9
#if defined(CMSIS_OS_RTX) && !defined(__MBED_CMSIS_RTOS_CM)
    uint32_t bottom = (uint32_t)_thread_def.tcb.stack;
    return _thread_def.tcb.tsk_stack - bottom;
#else
    P_TCB tcb = rt_tid2ptcb(_tid);
    uint32_t bottom = (uint32_t)tcb->stack;
    return tcb->tsk_stack - bottom;
#endif
#else
    return 0;
#endif
}

uint32_t Thread::used_stack() {
#ifndef __MBED_CMSIS_RTOS_CA9
#if defined(CMSIS_OS_RTX) && !defined(__MBED_CMSIS_RTOS_CM)
    uint32_t top = (uint32_t)_thread_def.tcb.stack + _thread_def.tcb.priv_stack;
    return top - _thread_def.tcb.tsk_stack;
#else
    P_TCB tcb = rt_tid2ptcb(_tid);
    uint32_t top = (uint32_t)tcb->stack + tcb->priv_stack;
    return top - tcb->tsk_stack;
#endif
#else
    return 0;
#endif
}

uint32_t Thread::max_stack() {
#ifndef __MBED_CMSIS_RTOS_CA9
#if defined(CMSIS_OS_RTX) && !defined(__MBED_CMSIS_RTOS_CM)
    uint32_t high_mark = 0;
    while (_thread_def.tcb.stack[high_mark] == 0xE25A2EA5)
        high_mark++;
    return _thread_def.tcb.priv_stack - (high_mark * 4);
#else
    P_TCB tcb = rt_tid2ptcb(_tid);
    uint32_t high_mark = 0;
    while (tcb->stack[high_mark] == 0xE25A2EA5)
        high_mark++;
    return tcb->priv_stack - (high_mark * 4);
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

Thread::~Thread() {
    terminate();
#ifdef __MBED_CMSIS_RTOS_CM
    if (_dynamic_stack) {
        delete[] (_thread_def.stack_pointer);
    }
#endif
}

}
