/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
#include "RTX_Config.h"

#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"              // Keil::Compiler:Event Recorder
// Used from rtx_evr.c
#define EvtRtxThreadExit               EventID(EventLevelAPI, 0xF2U, 0x19U)
#define EvtRtxThreadTerminate          EventID(EventLevelAPI, 0xF2U, 0x1AU)
#endif

extern void rtos_idle_loop(void);
extern void thread_terminate_hook(osThreadId_t id);

__NO_RETURN void osRtxIdleThread (void *argument)
{
    for (;;) {
      rtos_idle_loop();
    }
}

__NO_RETURN uint32_t osRtxErrorNotify (uint32_t code, void *object_id)
{
    osThreadId_t tid = osThreadGetId();

    switch (code) {
      case osRtxErrorStackUnderflow:
        // Stack underflow detected for thread (thread_id=object_id)
        // Note: "overflow" is printed instead of "underflow" due to end user familiarity with overflow errors
        error("CMSIS-RTOS error: Stack overflow (status: 0x%X, task ID: 0x%X, task name: %s)\n\r",
                code, object_id, osThreadGetName(object_id));
        break;
      case osRtxErrorISRQueueOverflow:
        // ISR Queue overflow detected when inserting object (object_id)
        error("CMSIS-RTOS error: ISR Queue overflow (status: 0x%X, task ID: 0x%X, object ID: 0x%X)\n\r",
                code, tid, object_id);
        break;
      case osRtxErrorTimerQueueOverflow:
        // User Timer Callback Queue overflow detected for timer (timer_id=object_id)
        error("CMSIS-RTOS error: User Timer Callback Queue overflow (status: 0x%X, task ID: 0x%X, timer ID: 0x%X)\n\r",
                code, tid, object_id);
        break;
      case osRtxErrorClibSpace:
        // Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM
        error("CMSIS-RTOS error: STD C/C++ library libspace not available (status: 0x%X, task ID: 0x%X)\n\r",
                code, tid);
        break;
      case osRtxErrorClibMutex:
        // Standard C/C++ library mutex initialization failed
        error("CMSIS-RTOS error: STD C/C++ library mutex initialization failed (status: 0x%X, task ID: 0x%X)\n\r",
                code, tid);
        break;
      default:
        error("CMSIS-RTOS error: Unknown (status: 0x%X, task ID: 0x%X)\n\r", code, tid);
        break;
    }

    /* That shouldn't be reached */
    for (;;) {}
}

#if defined(MBED_TRAP_ERRORS_ENABLED) && MBED_TRAP_ERRORS_ENABLED

static const char* error_msg(int32_t status)
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

void EvrRtxKernelError (int32_t status)
{
    error("Kernel error %i: %s\r\n", status, error_msg(status));
}

void EvrRtxThreadError (osThreadId_t thread_id, int32_t status)
{
    error("Thread %p error %i: %s\r\n", thread_id, status, error_msg(status));
}

void EvrRtxTimerError (osTimerId_t timer_id, int32_t status)
{
    error("Timer %p error %i: %s\r\n", timer_id, status, error_msg(status));
}

void EvrRtxEventFlagsError (osEventFlagsId_t ef_id, int32_t status)
{
    error("Event %p error %i: %s\r\n", ef_id, status, error_msg(status));
}

void EvrRtxMutexError (osMutexId_t mutex_id, int32_t status)
{
    error("Mutex %p error %i: %s\r\n", mutex_id, status, error_msg(status));
}

void EvrRtxSemaphoreError (osSemaphoreId_t semaphore_id, int32_t status)
{
    // Ignore semaphore overflow, the count will saturate with a returned error
    if (status == osRtxErrorSemaphoreCountLimit) {
        return;
    }

    error("Semaphore %p error %i\r\n", semaphore_id, status);
}

void EvrRtxMemoryPoolError (osMemoryPoolId_t mp_id, int32_t status)
{
    error("Memory Pool %p error %i\r\n", mp_id, status);
}

void EvrRtxMessageQueueError (osMessageQueueId_t mq_id, int32_t status)
{
    error("Message Queue %p error %i\r\n", mq_id, status);
}

#endif

// RTX hook which gets called when a thread terminates, using the event function to call hook
void EvrRtxThreadExit (void)
{
    osThreadId_t thread_id = osThreadGetId();
    thread_terminate_hook(thread_id);
#if (!defined(EVR_RTX_DISABLE) && (OS_EVR_THREAD != 0) && !defined(EVR_RTX_THREAD_EXIT_DISABLE) && defined(RTE_Compiler_EventRecorder))
    EventRecord2(EvtRtxThreadExit, 0U, 0U);
#endif
}

void EvrRtxThreadTerminate (osThreadId_t thread_id)
{
    thread_terminate_hook(thread_id);
#if (!defined(EVR_RTX_DISABLE) && (OS_EVR_THREAD != 0) && !defined(EVR_RTX_THREAD_TERMINATE_DISABLE) && defined(RTE_Compiler_EventRecorder))
    EventRecord2(EvtRtxThreadTerminate, (uint32_t)thread_id, 0U);
#endif
}
