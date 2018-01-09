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
#ifndef W_RTOS_H
#define W_RTOS_H

#include "wiced_result.h"
#include "wiced_utilities.h"
#include "wwd_rtos.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

#define WICED_HARDWARE_IO_WORKER_THREAD             ((wiced_worker_thread_t*)&wiced_hardware_io_worker_thread)
#define WICED_NETWORKING_WORKER_THREAD              ((wiced_worker_thread_t*)&wiced_networking_worker_thread )

#define WICED_PRIORITY_TO_NATIVE_PRIORITY(priority) (2 - ((int8_t)priority / 2))
#define WICED_END_OF_THREAD(thread)                 malloc_leak_check( &(thread).handle, LEAK_CHECK_THREAD); (void)(thread)
#define WICED_END_OF_CURRENT_THREAD( )              malloc_leak_check( NULL, LEAK_CHECK_THREAD)
#define WICED_END_OF_CURRENT_THREAD_NO_LEAK_CHECK( )

#define WICED_TO_MALLOC_THREAD( x )                 ((malloc_thread_handle) &((x)->handle ))

#define WICED_GET_THREAD_HANDLE( thread )           (&(( thread )->handle ))

#define WICED_GET_QUEUE_HANDLE( queue )             (&(( queue )->handle ))

/******************************************************
 *                    Constants
 ******************************************************/

/* Configuration of Built-in Worker Threads
 *
 * 1. wiced_hardware_io_worker_thread is designed to handle deferred execution of quick, non-blocking hardware I/O operations.
 *    - priority         : higher than that of wiced_networking_worker_thread
 *    - stack size       : small. Consequently, no printf is allowed here.
 *    - event queue size : the events are quick; therefore, large queue isn't required.
 *
 * 2. wiced_networking_worker_thread is designed to handle deferred execution of networking operations
 *    - priority         : lower to allow wiced_hardware_io_worker_thread to preempt and run
 *    - stack size       : considerably larger than that of wiced_hardware_io_worker_thread because of the networking functions.
 *    - event queue size : larger than that of wiced_hardware_io_worker_thread because networking operation may block
 */
#ifndef HARDWARE_IO_WORKER_THREAD_STACK_SIZE
#ifdef DEBUG
#define HARDWARE_IO_WORKER_THREAD_STACK_SIZE     (768) /* debug builds can use larger stack for example because of compiled-in asserts, switched off optimisation, etc */
#else
#define HARDWARE_IO_WORKER_THREAD_STACK_SIZE     (512)
#endif
#endif
#define HARDWARE_IO_WORKER_THREAD_QUEUE_SIZE      (10)

#ifndef NETWORKING_WORKER_THREAD_STACK_SIZE
#define NETWORKING_WORKER_THREAD_STACK_SIZE   (6*1024)
#endif
#define NETWORKING_WORKER_THREAD_QUEUE_SIZE       (15)

#ifndef TIMER_WORKER_THREAD_STACK_SIZE
#define TIMER_WORKER_THREAD_STACK_SIZE          (1024)
#endif

#ifndef TIMER_WORKER_THREAD_QUEUE_SIZE
#define TIMER_WORKER_THREAD_QUEUE_SIZE            (20)
#endif

#define RTOS_NAME                     "mbed"
#define RTOS_VERSION                  "???"

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

typedef struct { unsigned buffer[16]; } wiced_event_flags_t;

typedef host_semaphore_type_t wiced_semaphore_t;

typedef struct { unsigned buffer[16]; } wiced_mutex_t;

typedef void (*timer_handler_t)( void* arg );

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct
{
    host_thread_type_t handle;
    void*              stack;
} wiced_thread_t;

typedef struct
{
    host_queue_type_t handle;
    void*             buffer;
} wiced_queue_t;

typedef struct
{
    wiced_thread_t        thread;
    wiced_queue_t         event_queue;
} wiced_worker_thread_t;

typedef wiced_result_t (*event_handler_t)( void* arg );

typedef struct
{
    int             id;
    uint32_t        time_ms;
    timer_handler_t function;
    void*           arg;
} wiced_timer_t;

typedef struct
{
    event_handler_t        function;
    void*                  arg;
    wiced_timer_t          timer;
    wiced_worker_thread_t* thread;
} wiced_timed_event_t;

/******************************************************
 *                 Global Variables
 ******************************************************/

extern wiced_worker_thread_t wiced_hardware_io_worker_thread;
extern wiced_worker_thread_t wiced_networking_worker_thread;

/******************************************************
 *               Function Declarations
 ******************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
