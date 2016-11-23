/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * $Revision:   V5.0.0
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       RTX Configuration
 *
 * -----------------------------------------------------------------------------
 */
 
//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#include "mbed_rtx.h"

 
// <h>System Configuration
// =======================
 
//     <o>Global Dynamic Memory size [bytes] <0-1073741824:8>
//     <i> Defines the combined global dynamic memory size.
//     <i> Default: 4096

#ifndef OS_DYNAMIC_MEM_SIZE
#define OS_DYNAMIC_MEM_SIZE         4096
#endif

//   <o>Kernel Tick Frequency [Hz] <1-1000000>
//   <i> Defines base time unit for delays and timeouts.
//   <i> Default: 1000 (1ms tick)
#ifndef OS_TICK_FREQ
#define OS_TICK_FREQ                1000
#endif

//   <e>Round-Robin Thread switching
//   <i> Enables Round-Robin Thread switching.
#ifndef OS_ROBIN_ENABLE
#define OS_ROBIN_ENABLE             1
#endif

//     <o>Round-Robin Timeout <1-1000>
//     <i> Defines how many ticks a thread will execute before a thread switch.
//     <i> Default: 5
#ifndef OS_ROBIN_TIMEOUT
#define OS_ROBIN_TIMEOUT            5
#endif

//   </e>
 
//   <o>ISR FIFO Queue 
//      <4=>  4 entries    <8=>   8 entries   <12=>  12 entries   <16=>  16 entries
//     <24=> 24 entries   <32=>  32 entries   <48=>  48 entries   <64=>  64 entries
//     <96=> 96 entries  <128=> 128 entries  <196=> 196 entries  <256=> 256 entries
//   <i> RTOS Functions called from ISR store requests to this buffer.
//   <i> Default: 16 entries
#ifndef OS_ISR_FIFO_QUEUE
#define OS_ISR_FIFO_QUEUE           16
#endif

// </h>
 
// <h>Thread Configuration
// =======================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_THREAD_OBJ_MEM
#define OS_THREAD_OBJ_MEM           0
#endif

//     <o>Number of user Threads (total) <1-1000>
//     <i> Defines maximum number of user threads that can be active at the same time.
//     <i> Applies to user threads with system provided memory for control blocks.
#ifndef OS_THREAD_NUM
#error "target doesn not define OS_THREAD_NUM or no target defined"
#endif
 
//     <o>Number of user Threads with user-provided Stack size <0-1000>
//     <i> Defines maximum number of user threads with user-provided stack size.
//     <i> Default: 0
#ifndef OS_THREAD_USER_STACK_NUM
#define OS_THREAD_USER_STACK_NUM    0
#endif

//     <o>Total Stack size [bytes] for user Threads with user-provided Stack size <0-1073741824:8>
//     <i> Defines the combined stack size for user threads with user-provided stack size.
//     <i> Default: 0
#ifndef OS_THREAD_USER_STACK_SIZE
#define OS_THREAD_USER_STACK_SIZE   0
#endif

//   </e>
 
//   <o>Default Thread Stack size [bytes] <96-1073741824:8>
//   <i> Defines stack size for threads with zero stack size specified.
//   <i> Default: 200
#ifndef OS_STACK_SIZE
#define OS_STACK_SIZE               200
#endif

//   <o>Idle Thread Stack size [bytes] <72-1073741824:8>
//   <i> Defines stack size for Idle thread.
//   <i> Default: 200
#ifndef OS_IDLE_THREAD_STACK_SIZE
#define OS_IDLE_THREAD_STACK_SIZE   200
#endif
//   <q>Stack overrun checking
//   <i> Enable stack overrun checks at thread switch.
//   <i> Enabling this option increases slightly the execution time of a thread switch.
#ifndef OS_STACK_CHECK
#define OS_STACK_CHECK              1
#endif

//   <q>Stack usage watermark
//   <i> Initialize thread stack with watermark pattern for analyzing stack usage.
//   <i> Enabling this option increases significantly the execution time of thread creation.
#ifndef OS_STACK_WATERMARK
#define OS_STACK_WATERMARK          0
#endif

//   <o>Processor mode for Thread execution 
//     <0=> Unprivileged mode 
//     <1=> Privileged mode
//   <i> Default: Privileged mode
#ifndef OS_PRIVILEGE_MODE
#define OS_PRIVILEGE_MODE           1
#endif

// </h>
 
// <h>Timer Configuration
// ======================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_TIMER_OBJ_MEM
#define OS_TIMER_OBJ_MEM            0
#endif

//     <o>Number of Timer objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_TIMER_NUM
#define OS_TIMER_NUM                1
#endif

//   </e>
 
//   <o>Timer Thread Priority
//      <8=> Low
//     <16=> Below Normal  <24=> Normal  <32=> Above Normal
//     <40=> High
//     <48=> Realtime
//   <i> Defines priority for timer thread
//   <i> Default: High
#ifndef OS_TIMER_THREAD_PRIO
#define OS_TIMER_THREAD_PRIO        40
#endif

//   <o>Timer Thread Stack size [bytes] <0-1073741824:8>
//   <i> Defines stack size for Timer thread.
//   <i> May be set to 0 when timers are not used.
//   <i> Default: 200
#ifndef OS_TIMER_THREAD_STACK_SIZE
#define OS_TIMER_THREAD_STACK_SIZE  200
#endif

//   <o>Timer Callback Queue entries <0-256>
//   <i> Number of concurrent active timer callback functions.
//   <i> May be set to 0 when timers are not used.
//   <i> Default: 4
#ifndef OS_TIMER_CB_QUEUE
#define OS_TIMER_CB_QUEUE           4
#endif

// </h>
 
// <h>Event Flags Configuration
// ============================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_EVFLAGS_OBJ_MEM
#define OS_EVFLAGS_OBJ_MEM          0
#endif
//     <o>Number of Event Flags objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_EVFLAGS_NUM
#define OS_EVFLAGS_NUM              1
#endif

//   </e>
 
// </h>
 
// <h>Mutex Configuration
// ======================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_MUTEX_OBJ_MEM
#define OS_MUTEX_OBJ_MEM            0
#endif

//     <o>Number of Mutex objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_MUTEX_NUM
#define OS_MUTEX_NUM                1
#endif

//   </e>
 
// </h>
 
// <h>Semaphore Configuration
// ==========================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_SEMAPHORE_OBJ_MEM
#define OS_SEMAPHORE_OBJ_MEM        0
#endif

//     <o>Number of Semaphore objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_SEMAPHORE_NUM
#define OS_SEMAPHORE_NUM            1
#endif

//   </e>
 
// </h>
 
// <h>Memory Pool Configuration
// ============================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_MEMPOOL_OBJ_MEM
#define OS_MEMPOOL_OBJ_MEM          0
#endif

//     <o>Number of Memory Pool objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_MEMPOOL_NUM
#define OS_MEMPOOL_NUM              1
#endif

//     <o>Data Storage Memory size [bytes] <0-1073741824:8>
//     <i> Defines the combined data storage memory size.
//     <i> Applies to objects with system provided memory for data storage.
//     <i> Default: 0
#ifndef OS_MEMPOOL_DATA_SIZE
#define OS_MEMPOOL_DATA_SIZE        0
#endif

//   </e>
 
// </h>
 
// <h>Message Queue Configuration
// ==============================
 
//   <e>Object specific Memory allocation
//   <i> Enables object specific memory allocation.
#ifndef OS_MSGQUEUE_OBJ_MEM
#define OS_MSGQUEUE_OBJ_MEM         0
#endif

//     <o>Number of Message Queue objects <1-1000>
//     <i> Defines maximum number of objects that can be active at the same time.
//     <i> Applies to objects with system provided memory for control blocks.
#ifndef OS_MSGQUEUE_NUM
#define OS_MSGQUEUE_NUM             1
#endif

//     <o>Data Storage Memory size [bytes] <0-1073741824:8>
//     <i> Defines the combined data storage memory size.
//     <i> Applies to objects with system provided memory for data storage.
//     <i> Default: 0
#ifndef OS_MSGQUEUE_DATA_SIZE
#define OS_MSGQUEUE_DATA_SIZE       0
#endif

//   </e>
 
// </h>
 
// Number of Threads which use standard C/C++ library libspace
// (when thread specific memory allocation is not used).
#if (OS_THREAD_OBJ_MEM == 0)
#define OS_THREAD_LIBSPACE_NUM      4
#else
#define OS_THREAD_LIBSPACE_NUM      OS_THREAD_NUM
#endif
 
//------------- <<< end of configuration section >>> ---------------------------
 
#include "rtx_os.h"

extern void rtos_idle_loop(void);

// OS Idle Thread
__NO_RETURN void os_IdleThread (void *argument) {
  (void)argument;

  rtos_idle_loop();
}
 
// OS Error Callback function
uint32_t os_Error (uint32_t code, void *object_id) {
  (void)object_id;

  switch (code) {
    case os_ErrorStackUnderflow:
      // Stack underflow detected for thread (thread_id=object_id)
      break;
    case os_ErrorISRQueueOverflow:
      // ISR Queue overflow detected when inserting object (object_id)
      break;
    case os_ErrorTimerQueueOverflow:
      // User Timer Callback Queue overflow detected for timer (timer_id=object_id)
      break;
    case os_ErrorClibSpace:
      // Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM
      break;
    case os_ErrorClibMutex:
      // Standard C/C++ library mutex initialization failed
      break;
    default:
      break;
  }

  osThreadId_t tid = osThreadGetId();
  error("CMSIS-RTOS error status: 0x%8X, task ID: 0x%08X\n\r", code, tid);
}
 
#include "rtx_config.h"
