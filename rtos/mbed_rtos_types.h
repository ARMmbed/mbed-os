/* mbed Microcontroller Library
 * Copyright (c) 2006-2019, ARM Limited, All Rights Reserved
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
#ifndef RTOS_TYPES_H_
#define RTOS_TYPES_H_

#if MBED_CONF_RTOS_PRESENT || defined(DOXYGEN_ONLY) || defined(UNITTEST)
#include "cmsis_os2.h"
#else

#ifdef __cplusplus
extern "C" {
#endif

/* Minimal definitions for bare metal form of RTOS */

// Timeout value.
#define osWaitForever         0xFFFFFFFFU ///< Wait forever timeout value.

// Flags options (\ref osThreadFlagsWait and \ref osEventFlagsWait).
#define osFlagsWaitAny        0x00000000U ///< Wait for any flag (default).
#define osFlagsWaitAll        0x00000001U ///< Wait for all flags.
#define osFlagsNoClear        0x00000002U ///< Do not clear flags which have been specified to wait for.

// Flags errors (returned by osThreadFlagsXxxx and osEventFlagsXxxx).
#define osFlagsError          0x80000000U ///< Error indicator.
#define osFlagsErrorUnknown   0xFFFFFFFFU ///< osError (-1).
#define osFlagsErrorTimeout   0xFFFFFFFEU ///< osErrorTimeout (-2).
#define osFlagsErrorResource  0xFFFFFFFDU ///< osErrorResource (-3).
#define osFlagsErrorParameter 0xFFFFFFFCU ///< osErrorParameter (-4).
#define osFlagsErrorISR       0xFFFFFFFAU ///< osErrorISR (-6).

// Status code values returned by CMSIS-RTOS functions.
typedef enum {
    osOK                    =  0,         ///< Operation completed successfully.
    osError                 = -1,         ///< Unspecified RTOS error: run-time error but no other error message fits.
    osErrorTimeout          = -2,         ///< Operation not completed within the timeout period.
    osErrorResource         = -3,         ///< Resource not available.
    osErrorParameter        = -4,         ///< Parameter error.
    osErrorNoMemory         = -5,         ///< System is out of memory: it was impossible to allocate or reserve memory for the operation.
    osErrorISR              = -6,         ///< Not allowed in ISR context: the function cannot be called from interrupt service routines.
    osStatusReserved        = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} osStatus_t;


// \details Thread ID identifies the thread.
typedef void *osThreadId_t;

// Set the specified Thread Flags of a thread.
// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
// \param[in]     flags         specifies the flags of the thread that shall be set.
// \return thread flags after setting or error code if highest bit set.
uint32_t osThreadFlagsSet(osThreadId_t thread_id, uint32_t flags);

#ifdef __cplusplus
}
#endif

#endif



#endif /* RTOS_TYPES_H_ */
