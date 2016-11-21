/** \addtogroup rtos */
/** @{*/
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
 * Project:     CMSIS-RTOS RTX
 * Title:       RTX Library definitions
 *
 * -----------------------------------------------------------------------------
 */

#ifndef RTX_LIB_H_
#define RTX_LIB_H_

#include <string.h>
#include <stdbool.h>
#include "core_cm.h"                    // Cortex-M definitions
#include "tz_context.h"                 // TrustZone Context API
#include "cmsis_os2.h"                  // CMSIS RTOS API
#include "rtx_os.h"                     // RTX OS definitions


//  ==== Inline functions ====

// Kernel Inline functions
__STATIC_INLINE uint8_t      os_KernelGetState   (void) { return os_Info.kernel.state; }

// Thread Inline functions
__STATIC_INLINE os_thread_t *os_ThreadGetRunning (void) { return os_Info.thread.run.curr; }
__STATIC_INLINE void         os_ThreadSetRunning (os_thread_t *thread) { os_Info.thread.run.curr = thread; }


//  ==== Library functions ====

// Thread Library functions
extern void         os_ThreadListPut     (volatile os_object_t *object, os_thread_t *thread);
extern os_thread_t *os_ThreadListGet     (volatile os_object_t *object);
extern void         os_ThreadListSort    (os_thread_t  *thread);
extern void         os_ThreadListRemove  (os_thread_t  *thread);
extern void         os_ThreadListUnlink  (os_thread_t **thread_list, os_thread_t *thread);
extern void         os_ThreadReadyPut    (os_thread_t  *thread);
extern void         os_ThreadDelayInsert (os_thread_t  *thread, uint32_t delay);
extern void         os_ThreadDelayRemove (os_thread_t  *thread);
extern void         os_ThreadDelayTick   (void);
extern uint32_t    *os_ThreadRegPtr      (os_thread_t  *thread);
extern void         os_ThreadBlock       (os_thread_t  *thread);
extern void         os_ThreadSwitch      (os_thread_t  *thread);
extern void         os_ThreadDispatch    (os_thread_t  *thread);
extern void         os_ThreadWaitExit    (os_thread_t  *thread, uint32_t ret_val, bool dispatch);
extern bool         os_ThreadWaitEnter   (uint8_t state, uint32_t timeout);
extern void         os_ThreadStackCheck  (void);

// Timer Library functions
extern void os_TimerTick   (void);
extern void os_TimerThread (void *argument);

// Mutex Library functions
extern void  os_MutexOwnerRelease (os_mutex_t *mutex_list);

// Memory Heap Library functions
extern uint32_t os_MemoryInit (void *mem, uint32_t size);
extern void    *os_MemoryAlloc(void *mem, uint32_t size, uint32_t type);
extern uint32_t os_MemoryFree (void *mem, void *block);

// Memory Pool Library functions
extern uint32_t   os_MemoryPoolInit  (os_mp_info_t *mp_info, uint32_t blocks, uint32_t block_size, void *block_mem);
extern void      *os_MemoryPoolAlloc (os_mp_info_t *mp_info);
extern osStatus_t os_MemoryPoolFree  (os_mp_info_t *mp_info, void *block);

// System Library functions
extern void  os_Tick_Handler   (void);
extern void  os_PendSV_Handler (void);
extern void  os_PostProcess    (os_object_t *object);

// Post ISR processing functions
extern void  os_ThreadPostProcess       (os_thread_t      *thread);
extern void  os_EventFlagsPostProcess   (os_event_flags_t *ef);
extern void  os_SemaphorePostProcess    (os_semaphore_t   *semaphore);
extern void  os_MemoryPoolPostProcess   (os_memory_pool_t *mp);
extern void  os_MessageQueuePostProcess (os_message_t     *msg);


//  ==== Service Calls ====

// Kernel Service Calls
extern osStatus_t       os_svcKernelInitialize       (void);
extern osStatus_t       os_svcKernelGetInfo          (osVersion_t *version, char *id_buf, uint32_t id_size);
extern osKernelState_t  os_svcKernelGetState         (void);
extern osStatus_t       os_svcKernelStart            (void);
extern uint32_t         os_svcKernelLock             (void);
extern void             os_svcKernelUnlock           (void);
extern uint32_t         os_svcKernelSuspend          (void);
extern void             os_svcKernelResume           (uint32_t sleep_ticks);
extern uint64_t         os_svcKernelGetTickCount     (void);
extern uint32_t         os_svcKernelGetTickFreq      (void);
extern uint32_t         os_svcKernelGetSysTimerCount (void);
extern uint32_t         os_svcKernelGetSysTimerFreq  (void);

// Thread Service Calls
extern osThreadId_t     os_svcThreadNew          (os_thread_func_t func, void *argument, const osThreadAttr_t *attr);
extern const char *     os_svcThreadGetName      (osThreadId_t thread_id);
extern osThreadId_t     os_svcThreadGetId        (void);
extern osThreadState_t  os_svcThreadGetState     (osThreadId_t thread_id);
extern uint32_t         os_svcThreadGetStackSize (osThreadId_t thread_id);
extern uint32_t         os_svcThreadGetStackSpace(osThreadId_t thread_id);
extern osStatus_t       os_svcThreadSetPriority  (osThreadId_t thread_id, osPriority_t priority);
extern osPriority_t     os_svcThreadGetPriority  (osThreadId_t thread_id);
extern osStatus_t       os_svcThreadYield        (void);
extern osStatus_t       os_svcThreadSuspend      (osThreadId_t thread_id);
extern osStatus_t       os_svcThreadResume       (osThreadId_t thread_id);
extern osStatus_t       os_svcThreadDetach       (osThreadId_t thread_id);
extern osStatus_t       os_svcThreadJoin         (osThreadId_t thread_id);
extern void             os_svcThreadExit         (void);
extern osStatus_t       os_svcThreadTerminate    (osThreadId_t thread_id);
extern uint32_t         os_svcThreadGetCount     (void);
extern uint32_t         os_svcThreadEnumerate    (osThreadId_t *thread_array, uint32_t array_items);
extern int32_t          os_svcThreadFlagsSet     (osThreadId_t thread_id, int32_t flags);
extern int32_t          os_svcThreadFlagsClear   (int32_t flags);
extern int32_t          os_svcThreadFlagsGet     (void);
extern int32_t          os_svcThreadFlagsWait    (int32_t flags, uint32_t options, uint32_t timeout);

// Delay Service Calls
extern osStatus_t       os_svcDelay      (uint32_t ticks);
extern osStatus_t       os_svcDelayUntil (uint32_t ticks_l, uint32_t ticks_h);

// Timer Service Calls
extern osTimerId_t      os_svcTimerNew       (os_timer_func_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr);
extern const char *     os_svcTimerGetName   (osTimerId_t timer_id);
extern osStatus_t       os_svcTimerStart     (osTimerId_t timer_id, uint32_t ticks);
extern osStatus_t       os_svcTimerStop      (osTimerId_t timer_id);
extern uint32_t         os_svcTimerIsRunning (osTimerId_t timer_id);
extern osStatus_t       os_svcTimerDelete    (osTimerId_t timer_id);

// Event Flags Service Calls
extern osEventFlagsId_t os_svcEventFlagsNew     (const osEventFlagsAttr_t *attr);
extern const char *     os_svcEventFlagsGetName (osEventFlagsId_t ef_id);
extern int32_t          os_svcEventFlagsSet     (osEventFlagsId_t ef_id, int32_t flags);
extern int32_t          os_svcEventFlagsClear   (osEventFlagsId_t ef_id, int32_t flags);
extern int32_t          os_svcEventFlagsGet     (osEventFlagsId_t ef_id);
extern int32_t          os_svcEventFlagsWait    (osEventFlagsId_t ef_id, int32_t flags, uint32_t options, uint32_t timeout);
extern osStatus_t       os_svcEventFlagsDelete  (osEventFlagsId_t ef_id);

// Mutex Service Calls
extern osMutexId_t      os_svcMutexNew      (const osMutexAttr_t *attr);
extern const char *     os_svcMutexGetName  (osMutexId_t mutex_id);
extern osStatus_t       os_svcMutexAcquire  (osMutexId_t mutex_id, uint32_t timeout);
extern osStatus_t       os_svcMutexRelease  (osMutexId_t mutex_id);
extern osThreadId_t     os_svcMutexGetOwner (osMutexId_t mutex_id);
extern osStatus_t       os_svcMutexDelete   (osMutexId_t mutex_id);

// Semaphore Service Calls
extern osSemaphoreId_t  os_svcSemaphoreNew     (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr);
extern const char *     os_svcSemaphoreGetName (osSemaphoreId_t semaphore_id);
extern osStatus_t       os_svcSemaphoreRelease (osSemaphoreId_t semaphore_id);
extern osStatus_t       os_svcSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout);
extern uint32_t         os_svcSemaphoreGetCount(osSemaphoreId_t semaphore_id);
extern osStatus_t       os_svcSemaphoreDelete  (osSemaphoreId_t semaphore_id);

// Memory Pool Service Calls
extern osMemoryPoolId_t os_svcMemoryPoolNew          (uint32_t blocks, uint32_t block_size, const osMemoryPoolAttr_t *attr);
extern const char *     os_svcMemoryPoolGetName      (osMemoryPoolId_t mp_id);
extern void *           os_svcMemoryPoolAlloc        (osMemoryPoolId_t mp_id, uint32_t timeout);
extern osStatus_t       os_svcMemoryPoolFree         (osMemoryPoolId_t mp_id, void *block);
extern uint32_t         os_svcMemoryPoolGetCapacity  (osMemoryPoolId_t mp_id);
extern uint32_t         os_svcMemoryPoolGetBlockSize (osMemoryPoolId_t mp_id);
extern uint32_t         os_svcMemoryPoolGetCount     (osMemoryPoolId_t mp_id);
extern uint32_t         os_svcMemoryPoolGetSpace     (osMemoryPoolId_t mp_id);
extern osStatus_t       os_svcMemoryPoolDelete       (osMemoryPoolId_t mp_id);

// Message Queue Service Calls
extern osMessageQueueId_t os_svcMessageQueueNew         (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr);
extern const char *       os_svcMessageQueueGetName     (osMessageQueueId_t mq_id);
extern osStatus_t         os_svcMessageQueuePut         (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t  msg_prio, uint32_t timeout);
extern osStatus_t         os_svcMessageQueueGet         (osMessageQueueId_t mq_id,       void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);
extern uint32_t           os_svcMessageQueueGetCapacity (osMessageQueueId_t mq_id);
extern uint32_t           os_svcMessageQueueGetMsgSize  (osMessageQueueId_t mq_id);
extern uint32_t           os_svcMessageQueueGetCount    (osMessageQueueId_t mq_id);
extern uint32_t           os_svcMessageQueueGetSpace    (osMessageQueueId_t mq_id);
extern osStatus_t         os_svcMessageQueueReset       (osMessageQueueId_t mq_id);
extern osStatus_t         os_svcMessageQueueDelete      (osMessageQueueId_t mq_id);


#endif  // RTX_LIB_H_

/** @}*/
