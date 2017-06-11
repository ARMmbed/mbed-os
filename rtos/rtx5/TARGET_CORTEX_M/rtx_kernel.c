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
 * Title:       Kernel functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"
#include "rt_OsEventObserver.h"


//  OS Runtime Information
osRtxInfo_t osRtxInfo __attribute__((section(".data.os"))) =
{ .os_id = osRtxKernelId, .version = osRtxVersionKernel, .kernel.state = osRtxKernelInactive };


//  ==== Helper functions ====

/// Block Kernel (disable: thread switching, time tick, post ISR processing).
static void KernelBlock (void) {

  if (osRtxInfo.tick_irqn >= 0) {
    ExtTick_DisableIRQ(osRtxInfo.tick_irqn);
  }
  osRtxSysTimerDisable();
  osRtxInfo.kernel.blocked = 1U;
  __DSB();
  if (osRtxInfo.tick_irqn < 0) {
    osRtxInfo.kernel.pendISR = GetPendSV_ST();
    ClrPendSV_ST();
  } else {
    osRtxInfo.kernel.pendISR = GetPendSV();
    ClrPendSV();
  }
}

/// Unblock Kernel
static void KernelUnblock (void) {

  osRtxInfo.kernel.blocked = 0U;
  __DSB();
  if (osRtxInfo.kernel.pendSV != 0U) {
    osRtxInfo.kernel.pendSV = 0U;
    SetPendSV();
  }
  if (osRtxInfo.kernel.pendISR != 0U) {
    SetPendFlags(osRtxInfo.kernel.pendISR);
  }
  if (osRtxInfo.tick_irqn >= 0) {
    ExtTick_EnableIRQ(osRtxInfo.tick_irqn);
  }
  osRtxSysTimerEnable();
}


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_0M(KernelInitialize,       osStatus_t)
SVC0_3 (KernelGetInfo,          osStatus_t, osVersion_t *, char *, uint32_t)
SVC0_0M(KernelStart,            osStatus_t)
SVC0_0 (KernelLock,             int32_t)
SVC0_0 (KernelUnlock,           int32_t)
SVC0_1 (KernelRestoreLock,      int32_t, int32_t)
SVC0_0 (KernelSuspend,          uint32_t)
SVC0_1N(KernelResume,           void, uint32_t)
SVC0_0 (KernelGetState,         osKernelState_t)
SVC0_0D(KernelGetTickCount,     uint64_t)
SVC0_0 (KernelGetTickFreq,      uint32_t)
SVC0_0 (KernelGetSysTimerCount, uint32_t)
SVC0_0 (KernelGetSysTimerFreq,  uint32_t)

/// Initialize the RTOS Kernel.
/// \note API identical to osKernelInitialize
osStatus_t svcRtxKernelInitialize (void) {

  if (osRtxInfo.kernel.state == osRtxKernelReady) {
    EvrRtxKernelInitializeCompleted();
    return osOK;
  }
  if (osRtxInfo.kernel.state != osKernelInactive) {
    EvrRtxKernelError(osError);
    return osError;
  }

  // Initialize osRtxInfo
  memset(&osRtxInfo.kernel, 0, sizeof(osRtxInfo) - offsetof(osRtxInfo_t, kernel));

  if (osRtxConfig.thread_stack_size < (64U + 8U)) {
    EvrRtxKernelError(osRtxErrorInvalidThreadStack);
    return osError;
  }

  if ((osRtxConfig.isr_queue.data == NULL) || (osRtxConfig.isr_queue.max == 0U)) {
    EvrRtxKernelError(osError);
    return osError;
  }
  osRtxInfo.isr_queue.data = osRtxConfig.isr_queue.data;
  osRtxInfo.isr_queue.max  = osRtxConfig.isr_queue.max;

  osRtxInfo.thread.robin.timeout = osRtxConfig.robin_timeout;

  // Initialize Memory Pools (Variable Block Size)
  if (osRtxMemoryInit(osRtxConfig.mem.common_addr, osRtxConfig.mem.common_size) != 0U) {
    osRtxInfo.mem.common = osRtxConfig.mem.common_addr;
  }
  if (osRtxMemoryInit(osRtxConfig.mem.stack_addr, osRtxConfig.mem.stack_size) != 0U) {
    osRtxInfo.mem.stack = osRtxConfig.mem.stack_addr;
  } else {
    osRtxInfo.mem.stack = osRtxInfo.mem.common;
  }
  if (osRtxMemoryInit(osRtxConfig.mem.mp_data_addr, osRtxConfig.mem.mp_data_size) != 0U) {
    osRtxInfo.mem.mp_data = osRtxConfig.mem.mp_data_addr;
  } else {
    osRtxInfo.mem.mp_data = osRtxInfo.mem.common;
  }
  if (osRtxMemoryInit(osRtxConfig.mem.mq_data_addr, osRtxConfig.mem.mq_data_size) != 0U) {
    osRtxInfo.mem.mq_data = osRtxConfig.mem.mq_data_addr;
  } else {
    osRtxInfo.mem.mq_data = osRtxInfo.mem.common;
  }

  // Initialize Memory Pools (Fixed Block Size)
  if ((osRtxConfig.mpi.stack != NULL) &&
      (osRtxMemoryPoolInit(osRtxConfig.mpi.stack,
                           osRtxConfig.mpi.stack->max_blocks,
                           osRtxConfig.mpi.stack->block_size,
                           osRtxConfig.mpi.stack->block_base) != 0U)) {
    osRtxInfo.mpi.stack = osRtxConfig.mpi.stack;
  }
  if ((osRtxConfig.mpi.thread != NULL) &&
      (osRtxMemoryPoolInit(osRtxConfig.mpi.thread,
                           osRtxConfig.mpi.thread->max_blocks,
                           osRtxConfig.mpi.thread->block_size,
                           osRtxConfig.mpi.thread->block_base) != 0U)) {
    osRtxInfo.mpi.thread = osRtxConfig.mpi.thread;
  }
  if ((osRtxConfig.mpi.timer != NULL) &&
      (osRtxMemoryPoolInit(osRtxConfig.mpi.timer,
                           osRtxConfig.mpi.timer->max_blocks,
                           osRtxConfig.mpi.timer->block_size,
                           osRtxConfig.mpi.timer->block_base) != 0U)) {
    osRtxInfo.mpi.timer = osRtxConfig.mpi.timer;
  }
  if ((osRtxConfig.mpi.event_flags != NULL) &&
      (osRtxMemoryPoolInit(osRtxConfig.mpi.event_flags,
                           osRtxConfig.mpi.event_flags->max_blocks,
                           osRtxConfig.mpi.event_flags->block_size,
                           osRtxConfig.mpi.event_flags->block_base) != 0U)) {
    osRtxInfo.mpi.event_flags = osRtxConfig.mpi.event_flags;
  }
  if ((osRtxConfig.mpi.mutex != NULL) &&
      (osRtxMemoryPoolInit(osRtxConfig.mpi.mutex,
                           osRtxConfig.mpi.mutex->max_blocks,
                           osRtxConfig.mpi.mutex->block_size,
                           osRtxConfig.mpi.mutex->block_base) != 0U)) {
    osRtxInfo.mpi.mutex = osRtxConfig.mpi.mutex;
  }
  if ((osRtxConfig.mpi.semaphore != NULL) &&
      (osRtxMemoryPoolInit(osRtxConfig.mpi.semaphore,
                           osRtxConfig.mpi.semaphore->max_blocks,
                           osRtxConfig.mpi.semaphore->block_size,
                           osRtxConfig.mpi.semaphore->block_base) != 0U)) {
    osRtxInfo.mpi.semaphore = osRtxConfig.mpi.semaphore;
  }
  if ((osRtxConfig.mpi.memory_pool != NULL) &&
      (osRtxMemoryPoolInit(osRtxConfig.mpi.memory_pool,
                           osRtxConfig.mpi.memory_pool->max_blocks,
                           osRtxConfig.mpi.memory_pool->block_size,
                           osRtxConfig.mpi.memory_pool->block_base) != 0U)) {
    osRtxInfo.mpi.memory_pool = osRtxConfig.mpi.memory_pool;
  }
  if ((osRtxConfig.mpi.message_queue != NULL) &&
      (osRtxMemoryPoolInit(osRtxConfig.mpi.message_queue,
                           osRtxConfig.mpi.message_queue->max_blocks,
                           osRtxConfig.mpi.message_queue->block_size,
                           osRtxConfig.mpi.message_queue->block_base) != 0U)) {
    osRtxInfo.mpi.message_queue = osRtxConfig.mpi.message_queue;
  }

#if (__DOMAIN_NS == 1U)
  // Initialize Secure Process Stack
  if (TZ_InitContextSystem_S() == 0U) {
    EvrRtxKernelError(osRtxErrorTZ_InitContext_S);
    return osError;
  }
#endif

  // Initialize SVC and PendSV System Service Calls
  SVC_Initialize();

  osRtxInfo.kernel.state = osRtxKernelReady;

  EvrRtxKernelInitializeCompleted();

  return osOK;
}

///  Get RTOS Kernel Information.
/// \note API identical to osKernelGetInfo
osStatus_t svcRtxKernelGetInfo (osVersion_t *version, char *id_buf, uint32_t id_size) {

  if (version != NULL) {
    version->api    = osRtxVersionAPI;
    version->kernel = osRtxVersionKernel;
  }

  if ((id_buf != NULL) && (id_size != 0U)) {
    if (id_size > sizeof(osRtxKernelId)) {
      id_size = sizeof(osRtxKernelId);
    }
    memcpy(id_buf, osRtxKernelId, id_size);
  }

  EvrRtxKernelInfoRetrieved(version, id_buf);

  return osOK;
}

/// Get the current RTOS Kernel state.
/// \note API identical to osKernelGetState
osKernelState_t svcRtxKernelGetState (void) {
  EvrRtxKernelGetState((osKernelState_t)(osRtxInfo.kernel.state));
  return ((osKernelState_t)(osRtxInfo.kernel.state));
}

/// Start the RTOS Kernel scheduler.
/// \note API identical to osKernelStart
osStatus_t svcRtxKernelStart (void) {
  os_thread_t *thread;

  if (osRtxInfo.kernel.state != osRtxKernelReady) {
    EvrRtxKernelError(osRtxErrorKernelNotReady);
    return osError;
  }

  // Create Idle Thread
  if (osRtxInfo.thread.idle == NULL) {
    osRtxInfo.thread.idle = svcRtxThreadNew(osRtxIdleThread, NULL, osRtxConfig.idle_thread_attr, NULL);
    if (osRtxInfo.thread.idle == NULL) {
      EvrRtxKernelError(osError);
      return osError;
    }
  }

  // Create Timer Thread
  if (osRtxConfig.timer_mq_mcnt != 0U) {
    if (osRtxInfo.timer.thread == NULL) {
      osRtxInfo.timer.thread = svcRtxThreadNew(osRtxTimerThread, NULL, osRtxConfig.timer_thread_attr, NULL);
      if (osRtxInfo.timer.thread == NULL) {
        EvrRtxKernelError(osError);
        return osError;
      }
    }
  }

  // Switch to Ready Thread with highest Priority
  thread = osRtxThreadListGet(&osRtxInfo.thread.ready);
  if (thread == NULL) {
    EvrRtxKernelError(osError);
    return osError;
  }
  osRtxThreadSwitch(thread);

  if ((osRtxConfig.flags & osRtxConfigPrivilegedMode) != 0U) {
    // Privileged Thread mode & PSP
    __set_CONTROL(0x02U);
  } else {
    // Unprivileged Thread mode & PSP
    __set_CONTROL(0x03U);
  }

  osRtxInfo.kernel.sys_freq = SystemCoreClock;

  // Setup and Enable System Timer
  osRtxInfo.tick_irqn = osRtxSysTimerSetup();
  if (osRtxInfo.tick_irqn >= 0) {
    ExtTick_SetupIRQ (osRtxInfo.tick_irqn);
    ExtTick_EnableIRQ(osRtxInfo.tick_irqn);
  }
  osRtxSysTimerEnable();

  osRtxInfo.kernel.state = osRtxKernelRunning;

  EvrRtxKernelStarted();

  return osOK;
}

/// Lock the RTOS Kernel scheduler.
/// \note API identical to osKernelLock
int32_t svcRtxKernelLock (void) {

  if (osRtxInfo.kernel.state == osRtxKernelLocked) {
    EvrRtxKernelLocked(1);
    return 1;
  }
  if (osRtxInfo.kernel.state == osRtxKernelRunning) {
    osRtxInfo.kernel.state = osRtxKernelLocked;
    EvrRtxKernelLocked(0);
    return 0;
  }

  EvrRtxKernelError(osError);
  return osError;
}
 
/// Unlock the RTOS Kernel scheduler.
/// \note API identical to osKernelUnlock
int32_t svcRtxKernelUnlock (void) {

  if (osRtxInfo.kernel.state == osRtxKernelLocked) {
    osRtxInfo.kernel.state = osRtxKernelRunning;
    EvrRtxKernelUnlocked(1);
    return 1;
  }
  if (osRtxInfo.kernel.state == osRtxKernelRunning) {
    EvrRtxKernelUnlocked(0);
    return 0;
  }

  EvrRtxKernelError(osError);
  return osError;
}

/// Restore the RTOS Kernel scheduler lock state.
/// \note API identical to osKernelRestoreLock
int32_t svcRtxKernelRestoreLock (int32_t lock) {

  if ((osRtxInfo.kernel.state == osRtxKernelRunning) || 
      (osRtxInfo.kernel.state == osRtxKernelLocked)) {
    switch (lock) {
      case 1:
        osRtxInfo.kernel.state = osRtxKernelLocked;
        EvrRtxKernelLockRestored(1);
        return 1;
      case 0:
        osRtxInfo.kernel.state = osRtxKernelRunning;
        EvrRtxKernelLockRestored(0);
        return 0;
      default:
        break;
    }
  }

  EvrRtxKernelError(osError);
  return osError;
}

/// Suspend the RTOS Kernel scheduler.
/// \note API identical to osKernelSuspend
uint32_t svcRtxKernelSuspend (void) {
  os_thread_t *thread;
  os_timer_t  *timer;
  uint32_t     delay;

  if (osRtxInfo.kernel.state != osRtxKernelRunning) {
    EvrRtxKernelError(osRtxErrorKernelNotRunning);
    return 0U;
  }

  KernelBlock();

  delay = osWaitForever;

  // Check Thread Delay list
  thread = osRtxInfo.thread.delay_list;
  if (thread != NULL) {
    delay = thread->delay;
  }

  // Check Active Timer list
  timer = osRtxInfo.timer.list;
  if (timer != NULL) {
    if (timer->tick < delay) {
      delay = timer->tick;
    }
  }

  osRtxInfo.kernel.state = osRtxKernelSuspended;

  EvrRtxKernelSuspended(delay);

  return delay;
}

/// Resume the RTOS Kernel scheduler.
/// \note API identical to osKernelResume
void svcRtxKernelResume (uint32_t sleep_ticks) {
  os_thread_t *thread;
  os_timer_t  *timer;
  uint32_t     delay;

  if (osRtxInfo.kernel.state != osRtxKernelSuspended) {
    EvrRtxKernelResumed();
    return;
  }

  // Process Thread Delay list
  thread = osRtxInfo.thread.delay_list;
  if (thread != NULL) {
    delay = sleep_ticks;
    if (delay >= thread->delay) {
        delay -= thread->delay;
      osRtxInfo.kernel.tick += thread->delay;
      thread->delay = 1U;
      do {
        osRtxThreadDelayTick();
        if (delay == 0U) { 
          break;
        }
        delay--;
        osRtxInfo.kernel.tick++;
      } while (osRtxInfo.thread.delay_list != NULL);
    } else {
      thread->delay -= delay;
      osRtxInfo.kernel.tick += delay;
    }
  } else {
    osRtxInfo.kernel.tick += sleep_ticks;
  }

  // Process Active Timer list
  timer = osRtxInfo.timer.list;
  if (timer != NULL) {
    if (sleep_ticks >= timer->tick) {
        sleep_ticks -= timer->tick;
      timer->tick = 1U;
      do {
        osRtxInfo.timer.tick();
        if (sleep_ticks == 0U) {
          break;
        }
        sleep_ticks--;
      } while (osRtxInfo.timer.list != NULL);
    } else {
      timer->tick -= sleep_ticks;
    }
  }

  osRtxInfo.kernel.state = osRtxKernelRunning;

  osRtxThreadDispatch(NULL);

  KernelUnblock();

  EvrRtxKernelResumed();
}

/// Get the RTOS kernel tick count.
/// \note API identical to osKernelGetTickCount
uint64_t svcRtxKernelGetTickCount (void) {
  EvrRtxKernelGetTickCount(osRtxInfo.kernel.tick);
  return osRtxInfo.kernel.tick;
}

/// Get the RTOS kernel tick frequency.
/// \note API identical to osKernelGetTickFreq
uint32_t svcRtxKernelGetTickFreq (void) {
  EvrRtxKernelGetTickFreq(osRtxConfig.tick_freq);
  return osRtxConfig.tick_freq;
}

/// Get the RTOS kernel system timer count.
/// \note API identical to osKernelGetSysTimerCount
uint32_t svcRtxKernelGetSysTimerCount (void) {
  uint32_t count = osRtxSysTimerGetCount();
  EvrRtxKernelGetSysTimerCount(count);
  return count;
}

/// Get the RTOS kernel system timer frequency.
/// \note API identical to osKernelGetSysTimerFreq
uint32_t svcRtxKernelGetSysTimerFreq (void) {
  uint32_t freq = osRtxSysTimerGetFreq();
  EvrRtxKernelGetSysTimerFreq(freq);
  return freq;
}


//  ==== Public API ====

/// Initialize the RTOS Kernel.
osStatus_t osKernelInitialize (void) {
  EvrRtxKernelInitialize();
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelError(osErrorISR);
    return osErrorISR;
  }
  return __svcKernelInitialize();
}

///  Get RTOS Kernel Information.
osStatus_t osKernelGetInfo (osVersion_t *version, char *id_buf, uint32_t id_size) {
  EvrRtxKernelGetInfo(version, id_buf, id_size);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelError(osErrorISR);
    return osErrorISR;
  }
  if (IS_PRIVILEGED()) {
    return svcRtxKernelGetInfo(version, id_buf, id_size);
  } else {
    return  __svcKernelGetInfo(version, id_buf, id_size);
  }
}

/// Get the current RTOS Kernel state.
osKernelState_t osKernelGetState (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelGetState(osKernelError);
    return osKernelError;
  }
  if (IS_PRIVILEGED()) {
    return svcRtxKernelGetState();
  } else {
    return  __svcKernelGetState();
  }
}

/// Start the RTOS Kernel scheduler.
osStatus_t osKernelStart (void) {
  EvrRtxKernelStart();
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelError(osErrorISR);
    return osErrorISR;
  }

  /* Call the pre-start event (from unprivileged mode) if the handler exists
   * and the kernel is not running. */
  /* FIXME osEventObs needs to be readable but not writable from unprivileged
   * code. */
  if (osKernelGetState() != osKernelRunning && osEventObs && osEventObs->pre_start) {
    osEventObs->pre_start();
  }

  return __svcKernelStart();
}

/// Lock the RTOS Kernel scheduler.
int32_t osKernelLock (void) {
  EvrRtxKernelLock();
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelError(osErrorISR);
    return osErrorISR;
  }
  return __svcKernelLock();
}
 
/// Unlock the RTOS Kernel scheduler.
int32_t osKernelUnlock (void) {
  EvrRtxKernelUnlock();
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelError(osErrorISR);
    return osErrorISR;
  }
  return __svcKernelUnlock();
}

/// Restore the RTOS Kernel scheduler lock state.
int32_t osKernelRestoreLock (int32_t lock) {
  EvrRtxKernelRestoreLock(lock);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelError(osErrorISR);
    return osErrorISR;
  }
  return __svcKernelRestoreLock(lock);
}

/// Suspend the RTOS Kernel scheduler.
uint32_t osKernelSuspend (void) {
  EvrRtxKernelSuspend();
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelError(osErrorISR);
    return 0U;
  }
  return __svcKernelSuspend();
}

/// Resume the RTOS Kernel scheduler.
void osKernelResume (uint32_t sleep_ticks) {
  EvrRtxKernelResume(sleep_ticks);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelError(osErrorISR);
    return;
  }
  __svcKernelResume(sleep_ticks);
}

/// Get the RTOS kernel tick count.
uint64_t osKernelGetTickCount (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelGetTickCount(0U);
    return  0U;
  } else {
    return  __svcKernelGetTickCount();
  }
}

/// Get the RTOS kernel tick frequency.
uint32_t osKernelGetTickFreq (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxKernelGetTickFreq(0U);
    return  0U;
  } else {
    return  __svcKernelGetTickFreq();
  }
}

/// Get the RTOS kernel system timer count.
uint32_t osKernelGetSysTimerCount (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxKernelGetSysTimerCount();
  } else {
    return  __svcKernelGetSysTimerCount();
  }
}

/// Get the RTOS kernel system timer frequency.
uint32_t osKernelGetSysTimerFreq (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxKernelGetSysTimerFreq();
  } else {
    return  __svcKernelGetSysTimerFreq();
  }
}
