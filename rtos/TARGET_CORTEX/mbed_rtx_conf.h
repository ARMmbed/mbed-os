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
#ifndef MBED_RTX_CONF_H
#define MBED_RTX_CONF_H

#include "mbed_rtx.h"

/** Any access to RTX5 specific data structures used in common code should be wrapped in ifdef MBED_OS_BACKEND_RTX5 */
#define MBED_OS_BACKEND_RTX5

/** The thread's stack size can be configured by the application, if not explicitly specified it'll default to 4K */
#ifndef MBED_CONF_APP_THREAD_STACK_SIZE
#define MBED_CONF_APP_THREAD_STACK_SIZE 4096
#endif

#define OS_STACK_SIZE               MBED_CONF_APP_THREAD_STACK_SIZE

/** The timer thread's stack size can be configured by the application, if not explicitly specified, it'll default to 768 */
#ifndef MBED_CONF_APP_TIMER_THREAD_STACK_SIZE
#define MBED_CONF_APP_TIMER_THREAD_STACK_SIZE   768
#endif

#define OS_TIMER_THREAD_STACK_SIZE  MBED_CONF_APP_TIMER_THREAD_STACK_SIZE

/** The idle thread's stack size can be configured by the application, if not explicitly specified, it'll default to 512 */
#ifndef MBED_CONF_APP_IDLE_THREAD_STACK_SIZE
#define MBED_CONF_APP_IDLE_THREAD_STACK_SIZE    512
#endif

#define OS_IDLE_THREAD_STACK_SIZE   MBED_CONF_APP_IDLE_THREAD_STACK_SIZE

#define OS_DYNAMIC_MEM_SIZE         0

#if defined(OS_TICK_FREQ) && (OS_TICK_FREQ != 1000)
#error "OS Tickrate must be 1000 for system timing"
#endif

#if !defined(OS_STACK_WATERMARK) && (defined(MBED_STACK_STATS_ENABLED) || defined(MBED_ALL_STATS_ENABLED))
#define OS_STACK_WATERMARK          1
#endif

#if !defined(OS_STACK_WATERMARK) && defined(MBED_THREAD_STATS_ENABLED)
#define OS_STACK_WATERMARK          1
#endif

/* Run threads unprivileged when uVisor is enabled. */
#if defined(FEATURE_UVISOR) && defined(TARGET_UVISOR_SUPPORTED)
# define OS_PRIVILEGE_MODE           0
#endif

#define OS_IDLE_THREAD_TZ_MOD_ID     1
#define OS_TIMER_THREAD_TZ_MOD_ID    1


// Don't adopt default multi-thread support for ARM/ARMC6 toolchains from RTX code base.
// Provide Mbed-specific instead.
#define RTX_NO_MULTITHREAD_CLIB

#define OS_IDLE_THREAD_NAME         "idle_thread"
#define OS_TIMER_THREAD_NAME        "timer_thread"

#endif /* MBED_RTX_CONF_H */
