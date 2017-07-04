/*
 * Copyright (c) 2013-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
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
#include "rtx_core_c.h"                 // Cortex core definitions
#if ((__ARM_ARCH_8M_BASE__ != 0) || (__ARM_ARCH_8M_MAIN__ != 0))
#include "tz_context.h"                 // TrustZone Context API
#endif
#include "os_tick.h"
#include "cmsis_os2.h"                  // CMSIS RTOS API
#include "rtx_os.h"                     // RTX OS definitions
#include "rtx_evr.h"                    // RTX Event Recorder definitions


//  ==== Library defines ====

#define os_thread_t         osRtxThread_t
#define os_timer_t          osRtxTimer_t
#define os_timer_finfo_t    osRtxTimerFinfo_t
#define os_event_flags_t    osRtxEventFlags_t
#define os_mutex_t          osRtxMutex_t
#define os_semaphore_t      osRtxSemaphore_t
#define os_mp_info_t        osRtxMpInfo_t
#define os_memory_pool_t    osRtxMemoryPool_t
#define os_message_t        osRtxMessage_t
#define os_message_queue_t  osRtxMessageQueue_t
#define os_object_t         osRtxObject_t

//  ==== Inline functions ====

// Kernel Inline functions
__STATIC_INLINE uint8_t      osRtxKernelGetState   (void) { return osRtxInfo.kernel.state; }

// Thread Inline functions
__STATIC_INLINE os_thread_t *osRtxThreadGetRunning (void) { return osRtxInfo.thread.run.curr; }
__STATIC_INLINE void         osRtxThreadSetRunning (os_thread_t *thread) { osRtxInfo.thread.run.curr = thread; }


//  ==== Library functions ====

// Thread Library functions
extern void         osRtxThreadListPut     (volatile os_object_t *object, os_thread_t *thread);
extern os_thread_t *osRtxThreadListGet     (volatile os_object_t *object);
extern void        *osRtxThreadListRoot    (os_thread_t  *thread);
extern void         osRtxThreadListSort    (os_thread_t  *thread);
extern void         osRtxThreadListRemove  (os_thread_t  *thread);
extern void         osRtxThreadListUnlink  (os_thread_t **thread_list, os_thread_t *thread);
extern void         osRtxThreadReadyPut    (os_thread_t  *thread);
extern void         osRtxThreadDelayInsert (os_thread_t  *thread, uint32_t delay);
extern void         osRtxThreadDelayRemove (os_thread_t  *thread);
extern void         osRtxThreadDelayTick   (void);
extern uint32_t    *osRtxThreadRegPtr      (os_thread_t  *thread);
extern void         osRtxThreadBlock       (os_thread_t  *thread);
extern void         osRtxThreadSwitch      (os_thread_t  *thread);
extern void         osRtxThreadDispatch    (os_thread_t  *thread);
extern void         osRtxThreadWaitExit    (os_thread_t  *thread, uint32_t ret_val, bool dispatch);
extern bool         osRtxThreadWaitEnter   (uint8_t state, uint32_t timeout);
extern void         osRtxThreadStackCheck  (void);

// Timer Library functions
extern void osRtxTimerTick   (void);
extern void osRtxTimerThread (void *argument);

// Mutex Library functions
extern void osRtxMutexOwnerRelease (os_mutex_t *mutex_list);

// Memory Heap Library functions
extern uint32_t osRtxMemoryInit (void *mem, uint32_t size);
extern void    *osRtxMemoryAlloc(void *mem, uint32_t size, uint32_t type);
extern uint32_t osRtxMemoryFree (void *mem, void *block);

// Memory Pool Library functions
extern uint32_t   osRtxMemoryPoolInit  (os_mp_info_t *mp_info, uint32_t blocks, uint32_t block_size, void *block_mem);
extern void      *osRtxMemoryPoolAlloc (os_mp_info_t *mp_info);
extern osStatus_t osRtxMemoryPoolFree  (os_mp_info_t *mp_info, void *block);

// System Library functions
extern void osRtxTick_Handler   (void);
extern void osRtxPendSV_Handler (void);
extern void osRtxPostProcess    (os_object_t *object);

// Post ISR processing functions
extern void osRtxThreadPostProcess       (os_thread_t      *thread);
extern void osRtxEventFlagsPostProcess   (os_event_flags_t *ef);
extern void osRtxSemaphorePostProcess    (os_semaphore_t   *semaphore);
extern void osRtxMemoryPoolPostProcess   (os_memory_pool_t *mp);
extern void osRtxMessageQueuePostProcess (os_message_t     *msg);


//  ==== Service Calls ====

// Kernel Service Calls
extern osStatus_t       svcRtxKernelInitialize       (void);
extern osStatus_t       svcRtxKernelGetInfo          (osVersion_t *version, char *id_buf, uint32_t id_size);
extern osKernelState_t  svcRtxKernelGetState         (void);
extern osStatus_t       svcRtxKernelStart            (void);
extern int32_t          svcRtxKernelLock             (void);
extern int32_t          svcRtxKernelUnlock           (void);
extern int32_t          svcRtxKernelRestoreLock      (int32_t lock);
extern uint32_t         svcRtxKernelSuspend          (void);
extern void             svcRtxKernelResume           (uint32_t sleep_ticks);
extern uint32_t         svcRtxKernelGetTickCount     (void);
extern uint32_t         svcRtxKernelGetTickFreq      (void);
extern uint32_t         svcRtxKernelGetSysTimerCount (void);
extern uint32_t         svcRtxKernelGetSysTimerFreq  (void);

// Thread Service Calls
extern osThreadId_t     svcRtxThreadNew          (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr);
extern const char *     svcRtxThreadGetName      (osThreadId_t thread_id);
extern osThreadId_t     svcRtxThreadGetId        (void);
extern osThreadState_t  svcRtxThreadGetState     (osThreadId_t thread_id);
extern uint32_t         svcRtxThreadGetStackSize (osThreadId_t thread_id);
extern uint32_t         svcRtxThreadGetStackSpace(osThreadId_t thread_id);
extern osStatus_t       svcRtxThreadSetPriority  (osThreadId_t thread_id, osPriority_t priority);
extern osPriority_t     svcRtxThreadGetPriority  (osThreadId_t thread_id);
extern osStatus_t       svcRtxThreadYield        (void);
extern osStatus_t       svcRtxThreadSuspend      (osThreadId_t thread_id);
extern osStatus_t       svcRtxThreadResume       (osThreadId_t thread_id);
extern osStatus_t       svcRtxThreadDetach       (osThreadId_t thread_id);
extern osStatus_t       svcRtxThreadJoin         (osThreadId_t thread_id);
extern void             svcRtxThreadExit         (void);
extern osStatus_t       svcRtxThreadTerminate    (osThreadId_t thread_id);
extern uint32_t         svcRtxThreadGetCount     (void);
extern uint32_t         svcRtxThreadEnumerate    (osThreadId_t *thread_array, uint32_t array_items);
extern uint32_t         svcRtxThreadFlagsSet     (osThreadId_t thread_id, uint32_t flags);
extern uint32_t         svcRtxThreadFlagsClear   (uint32_t flags);
extern uint32_t         svcRtxThreadFlagsGet     (void);
extern uint32_t         svcRtxThreadFlagsWait    (uint32_t flags, uint32_t options, uint32_t timeout);

// Delay Service Calls
extern osStatus_t       svcRtxDelay      (uint32_t ticks);
extern osStatus_t       svcRtxDelayUntil (uint32_t ticks);

// Timer Service Calls
extern osTimerId_t      svcRtxTimerNew       (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr);
extern const char *     svcRtxTimerGetName   (osTimerId_t timer_id);
extern osStatus_t       svcRtxTimerStart     (osTimerId_t timer_id, uint32_t ticks);
extern osStatus_t       svcRtxTimerStop      (osTimerId_t timer_id);
extern uint32_t         svcRtxTimerIsRunning (osTimerId_t timer_id);
extern osStatus_t       svcRtxTimerDelete    (osTimerId_t timer_id);

// Event Flags Service Calls
extern osEventFlagsId_t svcRtxEventFlagsNew     (const osEventFlagsAttr_t *attr);
extern const char *     svcRtxEventFlagsGetName (osEventFlagsId_t ef_id);
extern uint32_t         svcRtxEventFlagsSet     (osEventFlagsId_t ef_id, uint32_t flags);
extern uint32_t         svcRtxEventFlagsClear   (osEventFlagsId_t ef_id, uint32_t flags);
extern uint32_t         svcRtxEventFlagsGet     (osEventFlagsId_t ef_id);
extern uint32_t         svcRtxEventFlagsWait    (osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout);
extern osStatus_t       svcRtxEventFlagsDelete  (osEventFlagsId_t ef_id);

// Mutex Service Calls
extern osMutexId_t      svcRtxMutexNew      (const osMutexAttr_t *attr);
extern const char *     svcRtxMutexGetName  (osMutexId_t mutex_id);
extern osStatus_t       svcRtxMutexAcquire  (osMutexId_t mutex_id, uint32_t timeout);
extern osStatus_t       svcRtxMutexRelease  (osMutexId_t mutex_id);
extern osThreadId_t     svcRtxMutexGetOwner (osMutexId_t mutex_id);
extern osStatus_t       svcRtxMutexDelete   (osMutexId_t mutex_id);

// Semaphore Service Calls
extern osSemaphoreId_t  svcRtxSemaphoreNew     (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr);
extern const char *     svcRtxSemaphoreGetName (osSemaphoreId_t semaphore_id);
extern osStatus_t       svcRtxSemaphoreRelease (osSemaphoreId_t semaphore_id);
extern osStatus_t       svcRtxSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout);
extern uint32_t         svcRtxSemaphoreGetCount(osSemaphoreId_t semaphore_id);
extern osStatus_t       svcRtxSemaphoreDelete  (osSemaphoreId_t semaphore_id);

// Memory Pool Service Calls
extern osMemoryPoolId_t svcRtxMemoryPoolNew          (uint32_t blocks, uint32_t block_size, const osMemoryPoolAttr_t *attr);
extern const char *     svcRtxMemoryPoolGetName      (osMemoryPoolId_t mp_id);
extern void *           svcRtxMemoryPoolAlloc        (osMemoryPoolId_t mp_id, uint32_t timeout);
extern osStatus_t       svcRtxMemoryPoolFree         (osMemoryPoolId_t mp_id, void *block);
extern uint32_t         svcRtxMemoryPoolGetCapacity  (osMemoryPoolId_t mp_id);
extern uint32_t         svcRtxMemoryPoolGetBlockSize (osMemoryPoolId_t mp_id);
extern uint32_t         svcRtxMemoryPoolGetCount     (osMemoryPoolId_t mp_id);
extern uint32_t         svcRtxMemoryPoolGetSpace     (osMemoryPoolId_t mp_id);
extern osStatus_t       svcRtxMemoryPoolDelete       (osMemoryPoolId_t mp_id);

// Message Queue Service Calls
extern osMessageQueueId_t svcRtxMessageQueueNew         (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr);
extern const char *       svcRtxMessageQueueGetName     (osMessageQueueId_t mq_id);
extern osStatus_t         svcRtxMessageQueuePut         (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t  msg_prio, uint32_t timeout);
extern osStatus_t         svcRtxMessageQueueGet         (osMessageQueueId_t mq_id,       void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);
extern uint32_t           svcRtxMessageQueueGetCapacity (osMessageQueueId_t mq_id);
extern uint32_t           svcRtxMessageQueueGetMsgSize  (osMessageQueueId_t mq_id);
extern uint32_t           svcRtxMessageQueueGetCount    (osMessageQueueId_t mq_id);
extern uint32_t           svcRtxMessageQueueGetSpace    (osMessageQueueId_t mq_id);
extern osStatus_t         svcRtxMessageQueueReset       (osMessageQueueId_t mq_id);
extern osStatus_t         svcRtxMessageQueueDelete      (osMessageQueueId_t mq_id);


#endif  // RTX_LIB_H_
