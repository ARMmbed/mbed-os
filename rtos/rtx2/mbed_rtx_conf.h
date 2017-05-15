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

#ifndef OS_STACK_SIZE
#ifndef MBED_SMALL_TARGET
#define OS_STACK_SIZE               4096
#else
#define OS_STACK_SIZE               2048
#endif
#endif

#define OS_TIMER_THREAD_STACK_SIZE 768
#define OS_IDLE_THREAD_STACK_SIZE  256

#define OS_DYNAMIC_MEM_SIZE         0

#if defined(__CC_ARM)
#define OS_MUTEX_OBJ_MEM            1
#define OS_MUTEX_NUM                6
#endif

#if !defined(OS_STACK_WATERMARK) && (defined(MBED_STACK_STATS_ENABLED) && MBED_STACK_STATS_ENABLED)
#define OS_STACK_WATERMARK          1
#endif

/* Run threads unprivileged when uVisor is enabled. */
#if defined(FEATURE_UVISOR) && defined(TARGET_UVISOR_SUPPORTED)
# define OS_PRIVILEGE_MODE           0
#endif

#endif /* MBED_RTX_CONF_H */
