/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cmsis_compiler.h"
#include "rtx_os.h"
#include "rtx_evr.h"
#include "mbed_rtx.h"
#include "mbed_error.h"
#include "mbed_interface.h"
#include "RTX_Config.h"
#include "rtos/source/rtos_handlers.h"
#include "rtos/source/rtos_idle.h"

#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"              // Keil::Compiler:Event Recorder
// Used from rtx_evr.c
#define EvtRtxThreadExit               EventID(EventLevelAPI, 0xF2U, 0x19U)
#define EvtRtxThreadTerminate          EventID(EventLevelAPI, 0xF2U, 0x1AU)
#endif

static void (*terminate_hook)(osThreadId_t id);

static void thread_terminate_hook(osThreadId_t id)
{
    if (terminate_hook) {
        terminate_hook(id);
    }
}

void rtos_attach_thread_terminate_hook(void (*fptr)(osThreadId_t id))
{
    terminate_hook = fptr;
}

__NO_RETURN void osRtxIdleThread(void *argument)
{
    rtos_idle_loop();
}

__NO_RETURN uint32_t osRtxErrorNotify(uint32_t code, void *object_id)
{
    switch (code) {
        case osRtxErrorStackUnderflow:
            // Stack underflow detected for thread (thread_id=object_id)
            // Note: "overflow" is printed instead of "underflow" due to end user familiarity with overflow errors
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_STACK_OVERFLOW), "CMSIS-RTOS error: Stack overflow", code);
            break;
        case osRtxErrorISRQueueOverflow:
            // ISR Queue overflow detected when inserting object (object_id)
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_ISR_QUEUE_OVERFLOW), "CMSIS-RTOS error: ISR Queue overflow", code);
            break;
        case osRtxErrorTimerQueueOverflow:
            // User Timer Callback Queue overflow detected for timer (timer_id=object_id)
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_TIMER_QUEUE_OVERFLOW), "CMSIS-RTOS error: User Timer Callback Queue overflow", code);
            break;
        case osRtxErrorClibSpace:
            // Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_CLIB_SPACE_UNAVAILABLE), "CMSIS-RTOS error: STD C/C++ library libspace not available", code);
            break;
        case osRtxErrorClibMutex:
            // Standard C/C++ library mutex initialization failed
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_CLIB_MUTEX_INIT_FAILURE), "CMSIS-RTOS error: STD C/C++ library mutex initialization failed", code);
            break;
        default:
            //Unknown error flagged from kernel
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_UNKNOWN), "CMSIS-RTOS error: Unknown", code);
            break;
    }
}

#if defined(MBED_TRAP_ERRORS_ENABLED) && MBED_TRAP_ERRORS_ENABLED

static const char *error_msg(int32_t status)
{
    switch (status) {
        case osError:
            return "Unspecified RTOS error";
        case osErrorTimeout:
            return "Operation not completed within the timeout period";
        case osErrorResource:
            return "Resource not available";
        case osErrorParameter:
            return "Parameter error";
        case osErrorNoMemory:
            return "System is out of memory";
        case osErrorISR:
            return "Not allowed in ISR context";
        default:
            return "Unknown";
    }
}

static void trap_rtx_error(unsigned int error_value, int32_t rtx_status, mbed_error_status_t error_status)
{
    // Attempts to get the console for the first time while printing an error
    // may well cause a mutex error; in general let RTX calls fail during
    // an error condition.
    if (mbed_get_error_in_progress()) {
        return;
    }
    MBED_ERROR1(error_status, error_msg(rtx_status), error_value);
}

void EvrRtxKernelError(int32_t status)
{
    trap_rtx_error(status, status, MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_RTOS_EVENT));
}

void EvrRtxThreadError(osThreadId_t thread_id, int32_t status)
{
    trap_rtx_error((unsigned int) thread_id, status, MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_RTOS_THREAD_EVENT));
}

void EvrRtxTimerError(osTimerId_t timer_id, int32_t status)
{
    trap_rtx_error((unsigned int) timer_id, status, MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_RTOS_TIMER_EVENT));
}

void EvrRtxEventFlagsError(osEventFlagsId_t ef_id, int32_t status)
{
    trap_rtx_error((unsigned int) ef_id, status, MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_RTOS_EVENT_FLAGS_EVENT));
}

void EvrRtxMutexError(osMutexId_t mutex_id, int32_t status)
{
    trap_rtx_error((unsigned int) mutex_id, status, MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_RTOS_MUTEX_EVENT));
}

void EvrRtxSemaphoreError(osSemaphoreId_t semaphore_id, int32_t status)
{
    // Ignore semaphore overflow, the count will saturate with a returned error
    if (status == osRtxErrorSemaphoreCountLimit) {
        return;
    }

    trap_rtx_error((unsigned int) semaphore_id, status, MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_RTOS_SEMAPHORE_EVENT));
}

void EvrRtxMemoryPoolError(osMemoryPoolId_t mp_id, int32_t status)
{
    trap_rtx_error((unsigned int) mp_id, status, MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_RTOS_MEMORY_POOL_EVENT));
}

void EvrRtxMessageQueueError(osMessageQueueId_t mq_id, int32_t status)
{
    trap_rtx_error((unsigned int) mq_id, status, MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_RTOS_MESSAGE_QUEUE_EVENT));
}

#endif

// RTX hook which gets called when a thread terminates, using the event function to call hook
void EvrRtxThreadExit(void)
{
    osThreadId_t thread_id = osThreadGetId();
    thread_terminate_hook(thread_id);
#if (!defined(EVR_RTX_DISABLE) && (OS_EVR_THREAD != 0) && !defined(EVR_RTX_THREAD_EXIT_DISABLE) && defined(RTE_Compiler_EventRecorder))
    EventRecord2(EvtRtxThreadExit, 0U, 0U);
#endif
}

void EvrRtxThreadTerminate(osThreadId_t thread_id)
{
    thread_terminate_hook(thread_id);
#if (!defined(EVR_RTX_DISABLE) && (OS_EVR_THREAD != 0) && !defined(EVR_RTX_THREAD_TERMINATE_DISABLE) && defined(RTE_Compiler_EventRecorder))
    EventRecord2(EvtRtxThreadTerminate, (uint32_t)thread_id, 0U);
#endif
}
