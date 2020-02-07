/* mbed Microcontroller Library
 * Copyright (c) 2006-2020, ARM Limited, All Rights Reserved
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

/** \addtogroup rtos-public-api */
/** @{*/

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

/// Priority values.
typedef enum {
    osPriorityNone          =  0,         ///< No priority (not initialized).
    osPriorityIdle          =  1,         ///< Reserved for Idle thread.
    osPriorityLow           =  8,         ///< Priority: low
    osPriorityLow1          =  8 + 1,     ///< Priority: low + 1
    osPriorityLow2          =  8 + 2,     ///< Priority: low + 2
    osPriorityLow3          =  8 + 3,     ///< Priority: low + 3
    osPriorityLow4          =  8 + 4,     ///< Priority: low + 4
    osPriorityLow5          =  8 + 5,     ///< Priority: low + 5
    osPriorityLow6          =  8 + 6,     ///< Priority: low + 6
    osPriorityLow7          =  8 + 7,     ///< Priority: low + 7
    osPriorityBelowNormal   = 16,         ///< Priority: below normal
    osPriorityBelowNormal1  = 16 + 1,     ///< Priority: below normal + 1
    osPriorityBelowNormal2  = 16 + 2,     ///< Priority: below normal + 2
    osPriorityBelowNormal3  = 16 + 3,     ///< Priority: below normal + 3
    osPriorityBelowNormal4  = 16 + 4,     ///< Priority: below normal + 4
    osPriorityBelowNormal5  = 16 + 5,     ///< Priority: below normal + 5
    osPriorityBelowNormal6  = 16 + 6,     ///< Priority: below normal + 6
    osPriorityBelowNormal7  = 16 + 7,     ///< Priority: below normal + 7
    osPriorityNormal        = 24,         ///< Priority: normal
    osPriorityNormal1       = 24 + 1,     ///< Priority: normal + 1
    osPriorityNormal2       = 24 + 2,     ///< Priority: normal + 2
    osPriorityNormal3       = 24 + 3,     ///< Priority: normal + 3
    osPriorityNormal4       = 24 + 4,     ///< Priority: normal + 4
    osPriorityNormal5       = 24 + 5,     ///< Priority: normal + 5
    osPriorityNormal6       = 24 + 6,     ///< Priority: normal + 6
    osPriorityNormal7       = 24 + 7,     ///< Priority: normal + 7
    osPriorityAboveNormal   = 32,         ///< Priority: above normal
    osPriorityAboveNormal1  = 32 + 1,     ///< Priority: above normal + 1
    osPriorityAboveNormal2  = 32 + 2,     ///< Priority: above normal + 2
    osPriorityAboveNormal3  = 32 + 3,     ///< Priority: above normal + 3
    osPriorityAboveNormal4  = 32 + 4,     ///< Priority: above normal + 4
    osPriorityAboveNormal5  = 32 + 5,     ///< Priority: above normal + 5
    osPriorityAboveNormal6  = 32 + 6,     ///< Priority: above normal + 6
    osPriorityAboveNormal7  = 32 + 7,     ///< Priority: above normal + 7
    osPriorityHigh          = 40,         ///< Priority: high
    osPriorityHigh1         = 40 + 1,     ///< Priority: high + 1
    osPriorityHigh2         = 40 + 2,     ///< Priority: high + 2
    osPriorityHigh3         = 40 + 3,     ///< Priority: high + 3
    osPriorityHigh4         = 40 + 4,     ///< Priority: high + 4
    osPriorityHigh5         = 40 + 5,     ///< Priority: high + 5
    osPriorityHigh6         = 40 + 6,     ///< Priority: high + 6
    osPriorityHigh7         = 40 + 7,     ///< Priority: high + 7
    osPriorityRealtime      = 48,         ///< Priority: realtime
    osPriorityRealtime1     = 48 + 1,     ///< Priority: realtime + 1
    osPriorityRealtime2     = 48 + 2,     ///< Priority: realtime + 2
    osPriorityRealtime3     = 48 + 3,     ///< Priority: realtime + 3
    osPriorityRealtime4     = 48 + 4,     ///< Priority: realtime + 4
    osPriorityRealtime5     = 48 + 5,     ///< Priority: realtime + 5
    osPriorityRealtime6     = 48 + 6,     ///< Priority: realtime + 6
    osPriorityRealtime7     = 48 + 7,     ///< Priority: realtime + 7
    osPriorityISR           = 56,         ///< Reserved for ISR deferred thread.
    osPriorityError         = -1,         ///< System cannot determine priority or illegal priority.
    osPriorityReserved      = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} osPriority_t;

// \details Thread ID identifies the thread.
typedef void *osThreadId_t;

// Set the specified Thread Flags of a thread.
// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
// \param[in]     flags         specifies the flags of the thread that shall be set.
// \return thread flags after setting or error code if highest bit set.
uint32_t osThreadFlagsSet(osThreadId_t thread_id, uint32_t flags);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif


#endif /* RTOS_TYPES_H_ */
