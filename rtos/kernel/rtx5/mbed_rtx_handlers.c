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
#include "mbed_rtx.h"
#include "mbed_error.h"

extern void rtos_idle_loop(void);

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
        error("CMSIS-RTOS error: Stack underflow (status: 0x%X, task ID: 0x%X, task name: %s)\n\r",
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
