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
 * Title:       Kernel functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  OS Runtime Information
os_info_t os_Info __attribute__((section(".data.os"))) =
{ .os_id = os_KernelId, .version = os_CMSIS_RTX, .kernel.state = os_KernelInactive };

//  Library reference to irq_cm module
extern       uint8_t  os_irq_cm;
extern const uint8_t *os_irq_cm_lib_ref;
       const uint8_t* os_irq_cm_lib_ref = &os_irq_cm;


//  ==== Helper functions ====

/// Block Kernel (disable: thread switching, time tick, post ISR processing).
static void os_KernelBlock (void) {

  if (os_Info.tick_irqn >= 0) {
    os_ExtTick_DisableIRQ(os_Info.tick_irqn);
  }
  os_SysTimerDisable();
  os_Info.kernel.blocked = 1U;
  __DSB();
  if (os_Info.tick_irqn < 0) {
    os_Info.kernel.pendISR = os_GetPendSV_ST();
    os_ClrPendSV_ST();
  } else {
    os_Info.kernel.pendISR = os_GetPendSV();
    os_ClrPendSV();
  }
}

/// Unblock Kernel
static void os_KernelUnblock (void) {

  os_Info.kernel.blocked = 0U;
  __DSB();
  if (os_Info.kernel.pendSV != 0U) {
    os_Info.kernel.pendSV = 0U;
    os_SetPendSV();
  }
  if (os_Info.kernel.pendISR != 0U) {
    os_SetPendFlags(os_Info.kernel.pendISR);
  }
  if (os_Info.tick_irqn >= 0) {
    os_ExtTick_EnableIRQ(os_Info.tick_irqn);
  }
  os_SysTimerEnable();
}


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_0 (KernelInitialize,       osStatus_t)
SVC0_3 (KernelGetInfo,          osStatus_t, osVersion_t *, char *, uint32_t)
SVC0_0 (KernelStart,            osStatus_t)
SVC0_0 (KernelLock,             uint32_t)
SVC0_0N(KernelUnlock,           void)
SVC0_0 (KernelSuspend,          uint32_t)
SVC0_1N(KernelResume,           void, uint32_t)
SVC0_0 (KernelGetState,         osKernelState_t)
SVC0_0 (KernelGetTickCount,     uint64_t)
SVC0_0 (KernelGetTickFreq,      uint32_t)
SVC0_0 (KernelGetSysTimerCount, uint32_t)
SVC0_0 (KernelGetSysTimerFreq,  uint32_t)

/// Initialize the RTOS Kernel.
/// \note API identical to osKernelInitialize
osStatus_t os_svcKernelInitialize (void) {

  if (os_Info.kernel.state == os_KernelReady) {
    return osOK;
  }
  if (os_Info.kernel.state != osKernelInactive) {
    return osError;
  }

  // Initialize os_Info
  memset(&os_Info.kernel, 0, sizeof(os_Info) - offsetof(os_info_t, kernel));

  if (os_Config.thread_stack_size < (64U + 8U)) {
    return osError;
  }

  if ((os_Config.isr_queue.data == NULL) || (os_Config.isr_queue.max == 0U)) {
    return osError;
  }
  os_Info.isr_queue.data = os_Config.isr_queue.data;
  os_Info.isr_queue.max  = os_Config.isr_queue.max;

  os_Info.thread.robin.timeout = os_Config.robin_timeout;

  // Initialize Memory Pools (Variable Block Size)
  if (os_MemoryInit(os_Config.mem.common_addr, os_Config.mem.common_size) != 0U) {
    os_Info.mem.common = os_Config.mem.common_addr;
  }
  if (os_MemoryInit(os_Config.mem.stack_addr, os_Config.mem.stack_size) != 0U) {
    os_Info.mem.stack = os_Config.mem.stack_addr;
  } else {
    os_Info.mem.stack = os_Info.mem.common;
  }
  if (os_MemoryInit(os_Config.mem.mp_data_addr, os_Config.mem.mp_data_size) != 0U) {
    os_Info.mem.mp_data = os_Config.mem.mp_data_addr;
  } else {
    os_Info.mem.mp_data = os_Info.mem.common;
  }
  if (os_MemoryInit(os_Config.mem.mq_data_addr, os_Config.mem.mq_data_size) != 0U) {
    os_Info.mem.mq_data = os_Config.mem.mq_data_addr;
  } else {
    os_Info.mem.mq_data = os_Info.mem.common;
  }

  // Initialize Memory Pools (Fixed Block Size)
  if ((os_Config.mpi.stack != NULL) &&
      (os_MemoryPoolInit(os_Config.mpi.stack,
                         os_Config.mpi.stack->max_blocks,
                         os_Config.mpi.stack->block_size,
                         os_Config.mpi.stack->block_base) != 0U)) {
    os_Info.mpi.stack = os_Config.mpi.stack;
  }
  if ((os_Config.mpi.thread != NULL) &&
      (os_MemoryPoolInit(os_Config.mpi.thread,
                         os_Config.mpi.thread->max_blocks,
                         os_Config.mpi.thread->block_size,
                         os_Config.mpi.thread->block_base) != 0U)) {
    os_Info.mpi.thread = os_Config.mpi.thread;
  }
  if ((os_Config.mpi.timer != NULL) &&
      (os_MemoryPoolInit(os_Config.mpi.timer,
                         os_Config.mpi.timer->max_blocks,
                         os_Config.mpi.timer->block_size,
                         os_Config.mpi.timer->block_base) != 0U)) {
    os_Info.mpi.timer = os_Config.mpi.timer;
  }
  if ((os_Config.mpi.event_flags != NULL) &&
      (os_MemoryPoolInit(os_Config.mpi.event_flags,
                         os_Config.mpi.event_flags->max_blocks,
                         os_Config.mpi.event_flags->block_size,
                         os_Config.mpi.event_flags->block_base) != 0U)) {
    os_Info.mpi.event_flags = os_Config.mpi.event_flags;
  }
  if ((os_Config.mpi.mutex != NULL) &&
      (os_MemoryPoolInit(os_Config.mpi.mutex,
                         os_Config.mpi.mutex->max_blocks,
                         os_Config.mpi.mutex->block_size,
                         os_Config.mpi.mutex->block_base) != 0U)) {
    os_Info.mpi.mutex = os_Config.mpi.mutex;
  }
  if ((os_Config.mpi.semaphore != NULL) &&
      (os_MemoryPoolInit(os_Config.mpi.semaphore,
                         os_Config.mpi.semaphore->max_blocks,
                         os_Config.mpi.semaphore->block_size,
                         os_Config.mpi.semaphore->block_base) != 0U)) {
    os_Info.mpi.semaphore = os_Config.mpi.semaphore;
  }
  if ((os_Config.mpi.memory_pool != NULL) &&
      (os_MemoryPoolInit(os_Config.mpi.memory_pool,
                         os_Config.mpi.memory_pool->max_blocks,
                         os_Config.mpi.memory_pool->block_size,
                         os_Config.mpi.memory_pool->block_base) != 0U)) {
    os_Info.mpi.memory_pool = os_Config.mpi.memory_pool;
  }
  if ((os_Config.mpi.message_queue != NULL) &&
      (os_MemoryPoolInit(os_Config.mpi.message_queue,
                         os_Config.mpi.message_queue->max_blocks,
                         os_Config.mpi.message_queue->block_size,
                         os_Config.mpi.message_queue->block_base) != 0U)) {
    os_Info.mpi.message_queue = os_Config.mpi.message_queue;
  }

#if (__DOMAIN_NS == 1U)
  // Initialize Secure Process Stack
  if (TZ_InitContextSystem_S() == 0U) {
    return osError;
  }
#endif

  // Create Idle Thread
  os_Info.thread.idle = (os_thread_t *)(os_svcThreadNew(
                                          os_IdleThread,
                                          NULL,
                                          os_Config.idle_thread_attr));
  if (os_Info.thread.idle == NULL) {
    return osError;
  }

  // Initialize SVC and PendSV System Service Calls
  os_SVC_Initialize();

  os_Info.kernel.state = os_KernelReady;

  return osOK;
}

///  Get RTOS Kernel Information.
/// \note API identical to osKernelGetInfo
osStatus_t os_svcKernelGetInfo (osVersion_t *version, char *id_buf, uint32_t id_size) {

  if (version != NULL) {
    version->api    = os_CMSIS_API;
    version->kernel = os_CMSIS_RTX;
  }

  if ((id_buf != NULL) && (id_size != 0U)) {
    if (id_size > sizeof(os_KernelId)) {
      id_size = sizeof(os_KernelId);
    }
    memcpy(id_buf, os_KernelId, id_size);
  }

  return osOK;
}

/// Get the current RTOS Kernel state.
/// \note API identical to osKernelGetState
osKernelState_t os_svcKernelGetState (void) {
  return ((osKernelState_t)(os_Info.kernel.state));
}

/// Start the RTOS Kernel scheduler.
/// \note API identical to osKernelStart
osStatus_t os_svcKernelStart (void) {
  os_thread_t *thread;

  if (os_Info.kernel.state != os_KernelReady) {
    return osError;
  }

  // Switch to Ready Thread with highest Priority
  thread = os_ThreadListGet(&os_Info.thread.ready);
  if (thread == NULL) {
    return osError;
  }
  os_ThreadSwitch(thread);

  if ((os_Config.flags & os_ConfigPrivilegedMode) != 0U) {
    // Privileged Thread mode & PSP
    __set_CONTROL(0x02U);
  } else {
    // Unprivileged Thread mode & PSP
    __set_CONTROL(0x03U);
  }
  __DSB();
  __ISB();

  os_Info.kernel.sys_freq = SystemCoreClock;

  // Setup and Enable System Timer
  os_Info.tick_irqn = os_SysTimerSetup();
  if (os_Info.tick_irqn >= 0) {
    os_ExtTick_EnableIRQ(os_Info.tick_irqn);
  }
  os_SysTimerEnable();

  os_Info.kernel.state = os_KernelRunning;

  return osOK;
}

/// Lock the RTOS Kernel scheduler.
/// \note API identical to osKernelLock
uint32_t os_svcKernelLock (void) {

  if (os_Info.kernel.state == osKernelRunning) {
    os_Info.kernel.state = os_KernelLocked;
    return 1U;
  }

  return 0U;
}
 
/// Unlock the RTOS Kernel scheduler.
/// \note API identical to osKernelUnlock
void os_svcKernelUnlock (void) {

  if (os_Info.kernel.state == os_KernelLocked) {
    os_Info.kernel.state = osKernelRunning;
  }
}

/// Suspend the RTOS Kernel scheduler.
/// \note API identical to osKernelSuspend
uint32_t os_svcKernelSuspend (void) {
  os_thread_t *thread;
  os_timer_t  *timer;
  uint32_t     delay;

  if (os_Info.kernel.state != os_KernelRunning) {
    return 0U;
  }

  os_KernelBlock();

  delay = osWaitForever;

  // Check Thread Delay list
  thread = os_Info.thread.delay_list;
  if (thread != NULL) {
    delay = thread->delay;
  }

  // Check Active Timer list
  timer = os_Info.timer.list;
  if (timer != NULL) {
    if (timer->tick < delay) {
      delay = timer->tick;
    }
  }

  os_Info.kernel.state = os_KernelSuspended;

  return delay;
}

/// Resume the RTOS Kernel scheduler.
/// \note API identical to osKernelResume
void os_svcKernelResume (uint32_t sleep_ticks) {
  os_thread_t *thread;
  os_timer_t  *timer;
  uint32_t     delay;

  if (os_Info.kernel.state != os_KernelSuspended) {
    return;
  }

  // Process Thread Delay list
  thread = os_Info.thread.delay_list;
  if (thread != NULL) {
    delay = sleep_ticks;
    if (delay >= thread->delay) {
        delay -= thread->delay;
      os_Info.kernel.tick += thread->delay;
      thread->delay = 1U;
      do {
        os_ThreadDelayTick();
        if (delay == 0U) { 
          break;
        }
        delay--;
        os_Info.kernel.tick++;
      } while (os_Info.thread.delay_list != NULL);
    } else {
      thread->delay -= delay;
      os_Info.kernel.tick += delay;
    }
  } else {
    os_Info.kernel.tick += sleep_ticks;
  }

  // Process Active Timer list
  timer = os_Info.timer.list;
  if (timer != NULL) {
    if (sleep_ticks >= timer->tick) {
        sleep_ticks -= timer->tick;
      timer->tick = 1U;
      do {
        os_TimerTick();
        if (sleep_ticks == 0U) {
          break;
        }
        sleep_ticks--;
      } while (os_Info.timer.list != NULL);
    } else {
      timer->tick -= sleep_ticks;
    }
  }

  os_Info.kernel.state = os_KernelRunning;

  os_ThreadDispatch(NULL);

  os_KernelUnblock();
}

/// Get the RTOS kernel tick count.
/// \note API identical to osKernelGetTickCount
uint64_t os_svcKernelGetTickCount (void) {
  return os_Info.kernel.tick;
}

/// Get the RTOS kernel tick frequency.
/// \note API identical to osKernelGetTickFreq
uint32_t os_svcKernelGetTickFreq (void) {
  return os_Config.tick_freq;
}

/// Get the RTOS kernel system timer count.
/// \note API identical to osKernelGetSysTimerCount
uint32_t os_svcKernelGetSysTimerCount (void) {
  return os_SysTimerGetCount();
}

/// Get the RTOS kernel system timer frequency.
/// \note API identical to osKernelGetSysTimerFreq
uint32_t os_svcKernelGetSysTimerFreq (void) {
  return os_SysTimerGetFreq();
}


//  ==== Public API ====

/// Initialize the RTOS Kernel.
osStatus_t osKernelInitialize (void) {
  if (__get_IPSR() != 0U) {
    return osErrorISR;                          // Not allowed in ISR
  }
  if ((__get_CONTROL() & 1U) == 0U) {           // Privileged mode
    return os_svcKernelInitialize();
  } else {                                      // Unprivileged mode
    return  __svcKernelInitialize();
  }
}

///  Get RTOS Kernel Information.
osStatus_t osKernelGetInfo (osVersion_t *version, char *id_buf, uint32_t id_size) {
  if (__get_IPSR() != 0U) {
    return osErrorISR;                          // Not allowed in ISR
  }
  if ((__get_CONTROL() & 1U) == 0U) {           // Privileged mode
    return os_svcKernelGetInfo(version, id_buf, id_size);
  } else {                                      // Unprivileged mode
    return  __svcKernelGetInfo(version, id_buf, id_size);
  }
}

/// Get the current RTOS Kernel state.
osKernelState_t osKernelGetState (void) {
  if (__get_IPSR() != 0U) {
    return osKernelError;                       // Not allowed in ISR
  }
  if ((__get_CONTROL() & 1U) == 0U) {           // Privileged mode
    return os_svcKernelGetState();
  } else {
    return  __svcKernelGetState();
  }
}

/// Start the RTOS Kernel scheduler.
osStatus_t osKernelStart (void) {
  osStatus_t status;
  uint32_t   stack[8];

  if (__get_IPSR() != 0U) {
    return osErrorISR;                          // Not allowed in ISR
  }
  switch (__get_CONTROL() & 0x03U) {
    case 0x00U:                                 // Privileged Thread mode & MSP
#if ( (__ARM_ARCH_8M_MAIN__ == 1U) || \
     ((__ARM_ARCH_8M_BASE__ == 1U) && (__DOMAIN_NS == 0U)))
//    __set_PSPLIM((uint32_t)stack);
#endif
      __set_PSP((uint32_t)(stack + 8));         // Initial PSP
      __set_CONTROL(0x02U);                     // Set Privileged Thread mode & PSP
      __DSB();
      __ISB();
      status = __svcKernelStart();
      if (status != osOK) {
        __set_CONTROL(0x00U);                   // Restore Privileged Thread mode & MSP
      }
      return status;
    case 0x01U:                                 // Unprivileged Thread mode & MSP
      return osError;
    case 0x02U:                                 // Privileged Thread mode & PSP
    case 0x03U:                                 // Unprivileged Thread mode & PSP
      break;
  }
  return __svcKernelStart();
}

/// Lock the RTOS Kernel scheduler.
uint32_t osKernelLock (void) {
  if (__get_IPSR() != 0U) {
    return 0U;                                  // Not allowed in ISR
  }
  return __svcKernelLock();
}
 
/// Unlock the RTOS Kernel scheduler.
void osKernelUnlock (void) {
  if (__get_IPSR() != 0U) {
    return;                                     // Not allowed in ISR
  }
  __svcKernelUnlock();
}

/// Suspend the RTOS Kernel scheduler.
uint32_t osKernelSuspend (void) {
  if (__get_IPSR() != 0U) {
    return 0U;                                  // Not allowed in ISR
  }
  return __svcKernelSuspend();
}

/// Resume the RTOS Kernel scheduler.
void osKernelResume (uint32_t sleep_ticks) {
  if (__get_IPSR() != 0U) {
    return;                                     // Not allowed in ISR
  }
  __svcKernelResume(sleep_ticks);
}

/// Get the RTOS kernel tick count.
uint64_t osKernelGetTickCount (void) {
  if (__get_IPSR() != 0U) {
    return 0U;                                  // Not allowed in ISR
  }
  return __svcKernelGetTickCount();
}

/// Get the RTOS kernel tick frequency.
uint32_t osKernelGetTickFreq (void) {
  if (__get_IPSR() != 0U) {
    return 0U;                                  // Not allowed in ISR
  }
  return __svcKernelGetTickFreq();
}

/// Get the RTOS kernel system timer count.
uint32_t osKernelGetSysTimerCount (void) {
  if (__get_IPSR() != 0U) {
    return 0U;                                  // Not allowed in ISR
  }
  return __svcKernelGetSysTimerCount();
}

/// Get the RTOS kernel system timer frequency.
uint32_t osKernelGetSysTimerFreq (void) {
  if (__get_IPSR() != 0U) {
    return 0U;                                  // Not allowed in ISR
  }
  return __svcKernelGetSysTimerFreq();
}
