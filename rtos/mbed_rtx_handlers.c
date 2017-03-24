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
    error("CMSIS-RTOS error status: 0x%X, task ID: 0x%X\n\r", code, tid);

    /* That shouldn't be reached */
    for (;;) {}
}
