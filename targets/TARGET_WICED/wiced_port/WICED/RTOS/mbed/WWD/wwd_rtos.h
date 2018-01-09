/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef W_WWD_RTOS_H
#define W_WWD_RTOS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"

#define RTOS_HIGHER_PRIORTIY_THAN(x)     (x < osPriorityHigh ? x+1 : osPriorityHigh)
#define RTOS_LOWER_PRIORTIY_THAN(x)      (x > osPriorityLow  ? x-1 : osPriorityLow)
#define RTOS_LOWEST_PRIORITY             (osPriorityLow)
#define RTOS_HIGHEST_PRIORITY            (osPriorityHigh)
#define RTOS_DEFAULT_THREAD_PRIORITY     (osPriorityNormal)

#define RTOS_USE_DYNAMIC_THREAD_STACK

#ifndef WWD_LOGGING_STDOUT_ENABLE
#ifdef DEBUG
#define WWD_THREAD_STACK_SIZE        (732 + 1400)   /* Stack checking requires a larger stack */
#else /* ifdef DEBUG */
#define WWD_THREAD_STACK_SIZE        (544 + 1400)
#endif /* ifdef DEBUG */
#else /* if WWD_LOGGING_STDOUT_ENABLE */
#define WWD_THREAD_STACK_SIZE        (544 + 4096 + 1400) /* WWD_LOG uses printf and requires a minimum of 4K stack */
#endif /* WWD_LOGGING_STDOUT_ENABLE */

typedef struct { unsigned buffer[64]; } host_thread_type_t;     /** definition of a thread handle */
typedef struct { unsigned buffer[16]; } host_semaphore_type_t;  /** definition of a semaphore */
typedef struct { unsigned buffer[16]; } host_queue_type_t;      /** definition of a message queue */

typedef struct
{
    unsigned char info;    /* not supported yet */
} host_rtos_thread_config_type_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WWD_RTOS_H_ */
