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
 * Title:       Thread functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"
#include "rt_OsEventObserver.h"

//  ==== Helper functions ====

/// Set Thread Flags.
/// \param[in]  thread          thread object.
/// \param[in]  flags           specifies the flags to set.
/// \return thread flags after setting.
static uint32_t ThreadFlagsSet (os_thread_t *thread, uint32_t flags) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  uint32_t thread_flags;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  thread->thread_flags |= flags;
  thread_flags = thread->thread_flags;

  if (primask == 0U) {
    __enable_irq();
  }
#else
  thread_flags = atomic_set32(&thread->thread_flags, flags);
#endif

  return thread_flags;
}

/// Clear Thread Flags.
/// \param[in]  thread          thread object.
/// \param[in]  flags           specifies the flags to clear.
/// \return thread flags before clearing.
static uint32_t ThreadFlagsClear (os_thread_t *thread, uint32_t flags) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  uint32_t thread_flags;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  thread_flags = thread->thread_flags;
  thread->thread_flags &= ~flags;

  if (primask == 0U) {
    __enable_irq();
  }
#else
  thread_flags = atomic_clr32(&thread->thread_flags, flags);
#endif

  return thread_flags;
}

/// Check Thread Flags.
/// \param[in]  thread          thread object.
/// \param[in]  flags           specifies the flags to check.
/// \param[in]  options         specifies flags options (osFlagsXxxx).
/// \return thread flags before clearing or 0 if specified flags have not been set.
static uint32_t ThreadFlagsCheck (os_thread_t *thread, uint32_t flags, uint32_t options) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask;
#endif
  uint32_t thread_flags;

  if ((options & osFlagsNoClear) == 0U) {
#if (__EXCLUSIVE_ACCESS == 0U)
    primask = __get_PRIMASK();
    __disable_irq();

    thread_flags = thread->thread_flags;
    if ((((options & osFlagsWaitAll) != 0U) && ((thread_flags & flags) != flags)) ||
        (((options & osFlagsWaitAll) == 0U) && ((thread_flags & flags) == 0U))) {
      thread_flags = 0U;
    } else {
      thread->thread_flags &= ~flags;
    }

    if (primask == 0U) {
      __enable_irq();
    }
#else
    if ((options & osFlagsWaitAll) != 0U) {
      thread_flags = atomic_chk32_all(&thread->thread_flags, flags);
    } else {
      thread_flags = atomic_chk32_any(&thread->thread_flags, flags);
    }
#endif
  } else {
    thread_flags = thread->thread_flags;
    if ((((options & osFlagsWaitAll) != 0U) && ((thread_flags & flags) != flags)) ||
        (((options & osFlagsWaitAll) == 0U) && ((thread_flags & flags) == 0U))) {
      thread_flags = 0U;
    }
  }

  return thread_flags;
}


//  ==== Library functions ====

/// Put a Thread into specified Object list sorted by Priority (Highest at Head).
/// \param[in]  object          generic object.
/// \param[in]  thread          thread object.
void osRtxThreadListPut (volatile os_object_t *object, os_thread_t *thread) {
  os_thread_t *prev, *next;
  int32_t      priority;

  if (thread == NULL) {
    return;
  }

  priority = thread->priority;

  prev = (os_thread_t *)(uint32_t)object;
  next = object->thread_list;
  while ((next != NULL) && (next->priority >= priority)) {
    prev = next;
    next = next->thread_next;
  }
  thread->thread_prev = prev;
  thread->thread_next = next;
  prev->thread_next = thread;
  if (next != NULL) {
    next->thread_prev = thread;
  }
}

/// Get a Thread with Highest Priority from specified Object list and remove it.
/// \param[in]  object          generic object.
/// \return thread object. 
os_thread_t *osRtxThreadListGet (volatile os_object_t *object) {
  os_thread_t *thread;

  thread = object->thread_list;
  if (thread != NULL) {
    object->thread_list = thread->thread_next;
    if (thread->thread_next != NULL) {
      thread->thread_next->thread_prev = (os_thread_t *)(uint32_t)object;
    }
    thread->thread_prev = NULL;
  }

  return thread;
}

/// Retrieve Thread list root.
/// \param[in]  thread          thread object.
void *osRtxThreadListRoot (os_thread_t *thread) {

  while ((thread != NULL) && (thread->id == osRtxIdThread)) {
    thread = thread->thread_prev;
  }
  return ((void *)thread);
}

/// Re-sort a Thread in linked Object list by Priority (Highest at Head).
/// \param[in]  thread          thread object.
void osRtxThreadListSort (os_thread_t *thread) {
  os_object_t *object;
  os_thread_t *thread0;

  // Search for object
  thread0 = thread;
  while (thread0->id == osRtxIdThread) {
    thread0 = thread0->thread_prev;
    if (thread0 == NULL) { 
      return;
    }
  }
  object = (os_object_t *)thread0;

  osRtxThreadListRemove(thread);
  osRtxThreadListPut(object, thread);
}

/// Remove a Thread from linked Object list.
/// \param[in]  thread          thread object.
void osRtxThreadListRemove (os_thread_t *thread) {

  if (thread->thread_prev != NULL) {
    thread->thread_prev->thread_next = thread->thread_next;
    if (thread->thread_next != NULL) {
      thread->thread_next->thread_prev = thread->thread_prev;
    }
    thread->thread_prev = NULL;
  }
}

/// Unlink a Thread from specified linked list.
/// \param[in]  thread          thread object.
void osRtxThreadListUnlink (os_thread_t **thread_list, os_thread_t *thread) {

  if (thread->thread_next != NULL) {
    thread->thread_next->thread_prev = thread->thread_prev;
  }
  if (thread->thread_prev != NULL) {
    thread->thread_prev->thread_next = thread->thread_next;
    thread->thread_prev = NULL;
  } else {
    *thread_list = thread->thread_next;
  }
}

/// Mark a Thread as Ready and put it into Ready list (sorted by Priority).
/// \param[in]  thread          thread object.
void osRtxThreadReadyPut (os_thread_t *thread) {

  thread->state = osRtxThreadReady;
  osRtxThreadListPut(&osRtxInfo.thread.ready, thread);
}

/// Insert a Thread into the Delay list sorted by Delay (Lowest at Head).
/// \param[in]  thread          thread object.
/// \param[in]  delay           delay value.
void osRtxThreadDelayInsert (os_thread_t *thread, uint32_t delay) {
  os_thread_t *prev, *next;

  if (delay == osWaitForever) {
    prev = NULL;
    next = osRtxInfo.thread.wait_list;
    while (next != NULL)  {
      prev = next;
      next = next->delay_next;
    }
    thread->delay = delay;
    thread->delay_prev = prev;
    thread->delay_next = next;
    if (prev != NULL) {
      prev->delay_next = thread;
    } else {
      osRtxInfo.thread.wait_list = thread;
    }
    if (next != NULL) {
      next->delay_prev = thread;
    }
  } else {
    prev = NULL;
    next = osRtxInfo.thread.delay_list;
    while ((next != NULL) && (next->delay <= delay)) {
      delay -= next->delay;
      prev = next;
      next = next->delay_next;
    }
    thread->delay = delay;
    thread->delay_prev = prev;
    thread->delay_next = next;
    if (prev != NULL) {
      prev->delay_next = thread;
    } else {
      osRtxInfo.thread.delay_list = thread;
    }
    if (next != NULL) {
      next->delay -= delay;
      next->delay_prev = thread;
    }
  }
}

/// Remove a Thread from the Delay list.
/// \param[in]  thread          thread object.
void osRtxThreadDelayRemove (os_thread_t *thread) {

  if (thread->delay == osWaitForever) {
    if ((thread->delay_prev == NULL) && (osRtxInfo.thread.wait_list != thread)) {
      return;
    }
    if (thread->delay_next != NULL) {
      thread->delay_next->delay_prev = thread->delay_prev;
    }
    if (thread->delay_prev != NULL) {
      thread->delay_prev->delay_next = thread->delay_next;
      thread->delay_prev = NULL;
    } else {
      osRtxInfo.thread.wait_list = thread->delay_next;
    }
  } else {
    if ((thread->delay_prev == NULL) && (osRtxInfo.thread.delay_list != thread)) {
      return;
    }
    if (thread->delay_next != NULL) {
      thread->delay_next->delay += thread->delay;
      thread->delay_next->delay_prev = thread->delay_prev;
    }
    if (thread->delay_prev != NULL) {
      thread->delay_prev->delay_next = thread->delay_next;
      thread->delay_prev = NULL;
    } else {
      osRtxInfo.thread.delay_list = thread->delay_next;
    }
  }
}

/// Process Thread Delay Tick (executed each System Tick).
void osRtxThreadDelayTick (void) {
  os_thread_t *thread;

  thread = osRtxInfo.thread.delay_list;
  if (thread == NULL) {
    return;
  }

  thread->delay--;

  if (thread->delay == 0U) {
    do {
      switch (thread->state) {
        case osRtxThreadWaitingDelay:
          EvrRtxThreadDelayCompleted();
          break;
        case osRtxThreadWaitingThreadFlags:
          EvrRtxThreadFlagsWaitTimeout();
          break;
        case osRtxThreadWaitingEventFlags:
          EvrRtxEventFlagsWaitTimeout((osEventFlagsId_t)osRtxThreadListRoot(thread));
          break;
        case osRtxThreadWaitingMutex:
          EvrRtxMutexAcquireTimeout((osMutexId_t)osRtxThreadListRoot(thread));
          break;
        case osRtxThreadWaitingSemaphore:
          EvrRtxSemaphoreAcquireTimeout((osSemaphoreId_t)osRtxThreadListRoot(thread));
          break;
        case osRtxThreadWaitingMemoryPool:
          EvrRtxMemoryPoolAllocTimeout((osMemoryPoolId_t)osRtxThreadListRoot(thread));
          break;
        case osRtxThreadWaitingMessageGet:
          EvrRtxMessageQueueGetTimeout((osMessageQueueId_t)osRtxThreadListRoot(thread));
          break;
        case osRtxThreadWaitingMessagePut:
          EvrRtxMessageQueuePutTimeout((osMessageQueueId_t)osRtxThreadListRoot(thread));
          break;
        default:
          break;
      }
      EvrRtxThreadUnblocked(thread, (osRtxThreadRegPtr(thread))[0]);
      osRtxThreadListRemove(thread);
      osRtxThreadReadyPut(thread);
      thread = thread->delay_next;
    } while ((thread != NULL) && (thread->delay == 0U));
    if (thread != NULL) {
      thread->delay_prev = NULL;
    }
    osRtxInfo.thread.delay_list = thread;
  }
}

/// Get pointer to Thread registers (R0..R3)
/// \param[in]  thread          thread object.
/// \return pointer to registers R0-R3.
uint32_t *osRtxThreadRegPtr (os_thread_t *thread) {
  return ((uint32_t *)(thread->sp + STACK_OFFSET_R0(thread->stack_frame)));
}

/// Block running Thread execution and register it as Ready to Run.
/// \param[in]  thread          running thread object.
void osRtxThreadBlock (os_thread_t *thread) {
  os_thread_t *prev, *next;
  int32_t      priority;

  thread->state = osRtxThreadReady;

  priority = thread->priority;

  prev = (os_thread_t *)(uint32_t)&osRtxInfo.thread.ready;
  next = prev->thread_next;

  while ((next != NULL) && (next->priority > priority)) {
    prev = next;
    next = next->thread_next;
  }
  thread->thread_prev = prev;
  thread->thread_next = next;
  prev->thread_next = thread;
  if (next != NULL) {
    next->thread_prev = thread;
  }
}

/// Switch to specified Thread.
/// \param[in]  thread          thread object.
void osRtxThreadSwitch (os_thread_t *thread) {

  thread->state = osRtxThreadRunning;
  osRtxInfo.thread.run.next = thread;
  osRtxThreadStackCheck();
  EvrRtxThreadSwitch(thread);
}

/// Notify the OS event observer of an imminent thread switch.
void thread_switch_helper(void) {
  if (osEventObs && osEventObs->thread_switch) {
    osEventObs->thread_switch(osRtxInfo.thread.run.next->context);
  }
}

/// Dispatch specified Thread or Ready Thread with Highest Priority.
/// \param[in]  thread          thread object or NULL.
void osRtxThreadDispatch (os_thread_t *thread) {
  uint8_t      kernel_state;
  os_thread_t *thread_running;

  kernel_state   = osRtxKernelGetState();
  thread_running = osRtxThreadGetRunning();
#if (__ARM_ARCH_7A__ != 0U)
  // On Cortex-A PendSV_Handler is executed before final context switch.
  if ((thread_running != NULL) && (thread_running->state != osRtxThreadRunning)) {
    thread_running = osRtxInfo.thread.run.next;
  }
#endif

  if (thread == NULL) {
    thread = osRtxInfo.thread.ready.thread_list;
    if ((kernel_state == osRtxKernelRunning) &&
        (thread_running != NULL) && (thread != NULL) && 
        (thread->priority > thread_running->priority)) {
      // Preempt running Thread
      osRtxThreadListRemove(thread);
      osRtxThreadBlock(thread_running);
      osRtxThreadSwitch(thread);
    }
  } else {
    if ((kernel_state == osRtxKernelRunning) &&
        (thread_running != NULL) &&
        (thread->priority > thread_running->priority)) {
      // Preempt running Thread
      osRtxThreadBlock(thread_running);
      osRtxThreadSwitch(thread);
    } else {
      // Put Thread into Ready list
      osRtxThreadReadyPut(thread);
    }
  }
}

/// Exit Thread wait state.
/// \param[in]  thread          thread object.
/// \param[in]  ret_val         return value.
/// \param[in]  dispatch        dispatch flag.
void osRtxThreadWaitExit (os_thread_t *thread, uint32_t ret_val, bool dispatch) {
  uint32_t *reg;

  EvrRtxThreadUnblocked(thread, ret_val);

  reg = osRtxThreadRegPtr(thread);
  reg[0] = ret_val;

  osRtxThreadDelayRemove(thread);
  if (dispatch) {
    osRtxThreadDispatch(thread);
  } else {
    osRtxThreadReadyPut(thread);
  }
}

/// Enter Thread wait state.
/// \param[in]  state           new thread state.
/// \param[in]  timeout         timeout.
/// \return true - success, false - failure.
bool osRtxThreadWaitEnter (uint8_t state, uint32_t timeout) {
  os_thread_t *thread;

  thread = osRtxThreadGetRunning();
  if (thread == NULL) {
    return false;
  }

  if (osRtxKernelGetState() != osRtxKernelRunning) {
    osRtxThreadListRemove(thread);
    return false;
  }

  if (osRtxInfo.thread.ready.thread_list == NULL) {
    return false;
  }

  EvrRtxThreadBlocked(thread, timeout);

  thread->state = state;
  osRtxThreadDelayInsert(thread, timeout);
  thread = osRtxThreadListGet(&osRtxInfo.thread.ready);
  osRtxThreadSwitch(thread);

  return true;
}

/// Check current running Thread Stack.
__WEAK void osRtxThreadStackCheck (void) {
  os_thread_t *thread;

  thread = osRtxThreadGetRunning();
  if (thread != NULL) {
    if ((thread->sp <= (uint32_t)thread->stack_mem) ||
        (*((uint32_t *)thread->stack_mem) != osRtxStackMagicWord)) {
      osRtxErrorNotify(osRtxErrorStackUnderflow, thread);
    }
  }
}

/// Thread post ISR processing.
/// \param[in]  thread          thread object.
void osRtxThreadPostProcess (os_thread_t *thread) {
  uint32_t thread_flags;

  if ((thread->state == osRtxThreadInactive) ||
      (thread->state == osRtxThreadTerminated)) {
    return;
  }

  // Check if Thread is waiting for Thread Flags
  if (thread->state == osRtxThreadWaitingThreadFlags) {
    thread_flags = ThreadFlagsCheck(thread, thread->wait_flags, thread->flags_options);
    if (thread_flags != 0U) {
      osRtxThreadWaitExit(thread, thread_flags, false);
      EvrRtxThreadFlagsWaitCompleted(thread->wait_flags, thread->flags_options, thread_flags);
    }
  }
}


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_4M(ThreadNew,           osThreadId_t,    osThreadFunc_t, void *, const osThreadAttr_t *, void *)
SVC0_1 (ThreadGetName,       const char *,    osThreadId_t)
SVC0_0 (ThreadGetId,         osThreadId_t)
SVC0_1 (ThreadGetState,      osThreadState_t, osThreadId_t)
SVC0_1 (ThreadGetStackSize,  uint32_t, osThreadId_t)
SVC0_1 (ThreadGetStackSpace, uint32_t, osThreadId_t)
SVC0_2 (ThreadSetPriority,   osStatus_t,      osThreadId_t, osPriority_t)
SVC0_1 (ThreadGetPriority,   osPriority_t,    osThreadId_t)
SVC0_0 (ThreadYield,         osStatus_t)
SVC0_1 (ThreadSuspend,       osStatus_t,      osThreadId_t)
SVC0_1 (ThreadResume,        osStatus_t,      osThreadId_t)
SVC0_1 (ThreadDetach,        osStatus_t,      osThreadId_t)
SVC0_1 (ThreadJoin,          osStatus_t,      osThreadId_t)
SVC0_0N(ThreadExit,          void)
SVC0_1 (ThreadTerminate,     osStatus_t,      osThreadId_t)
SVC0_0 (ThreadGetCount,      uint32_t)
SVC0_2 (ThreadEnumerate,     uint32_t,        osThreadId_t *, uint32_t)
SVC0_2 (ThreadFlagsSet,      uint32_t,        osThreadId_t, uint32_t)
SVC0_1 (ThreadFlagsClear,    uint32_t,        uint32_t)
SVC0_0 (ThreadFlagsGet,      uint32_t)
SVC0_3 (ThreadFlagsWait,     uint32_t,        uint32_t, uint32_t, uint32_t)

/// Create a thread and add it to Active Threads.
/// \note API identical to osThreadContextNew
osThreadId_t svcRtxThreadNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr, void *context) {
  os_thread_t  *thread;
  uint32_t      attr_bits;
  void         *stack_mem;
  uint32_t      stack_size;
  osPriority_t  priority;
  uint8_t       flags;
  const char   *name;
  uint32_t     *ptr;
  uint32_t      n;
#if (__DOMAIN_NS == 1U)
  TZ_ModuleId_t tz_module;
  TZ_MemoryId_t tz_memory;
#endif

  // Check parameters
  if (func == NULL) {
    EvrRtxThreadError(NULL, osErrorParameter);
    return NULL;
  }

  // Process attributes
  if (attr != NULL) {
    name       = attr->name;
    attr_bits  = attr->attr_bits;
    thread     = attr->cb_mem;
    stack_mem  = attr->stack_mem;
    stack_size = attr->stack_size;
    priority   = attr->priority;
#if (__DOMAIN_NS == 1U)
    tz_module  = attr->tz_module;
#endif
    if (thread != NULL) {
      if (((uint32_t)thread & 3U) || (attr->cb_size < sizeof(os_thread_t))) {
        EvrRtxThreadError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    } else {
      if (attr->cb_size != 0U) {
        EvrRtxThreadError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    }
    if (stack_mem != NULL) {
      if (((uint32_t)stack_mem & 7U) || (stack_size == 0U)) {
        EvrRtxThreadError(NULL, osRtxErrorInvalidThreadStack);
        return NULL;
      }
    }
    if (priority == osPriorityNone) {
      priority = osPriorityNormal;
    } else {
      if ((priority < osPriorityIdle) || (priority > osPriorityISR)) {
        EvrRtxThreadError(NULL, osRtxErrorInvalidPriority);
        return NULL;
      }
    }
  } else {
    name       = NULL;
    attr_bits  = 0U;
    thread     = NULL;
    stack_mem  = NULL;
    stack_size = 0U;
    priority   = osPriorityNormal;
#if (__DOMAIN_NS == 1U)
    tz_module  = 0U;
#endif
  }

  // Check stack size
  if ((stack_size != 0U) && ((stack_size & 7U) || (stack_size < (64U + 8U)))) {
    EvrRtxThreadError(NULL, osRtxErrorInvalidThreadStack);
    return NULL;
  }

  // Allocate object memory if not provided
  if (thread == NULL) {
    if (osRtxInfo.mpi.thread != NULL) {
      thread = osRtxMemoryPoolAlloc(osRtxInfo.mpi.thread);
    } else {
      thread = osRtxMemoryAlloc(osRtxInfo.mem.common, sizeof(os_thread_t), 1U);
    }
    if (thread == NULL) {
      EvrRtxThreadError(NULL, osErrorNoMemory);
      return NULL;
    }
    flags = osRtxFlagSystemObject;
  } else {
    flags = 0U;
  }

  // Allocate stack memory if not provided
  if (stack_mem == NULL) {
    if (stack_size == 0U) {
      stack_size = osRtxConfig.thread_stack_size;
      if (osRtxInfo.mpi.stack != NULL) {
        stack_mem = osRtxMemoryPoolAlloc(osRtxInfo.mpi.stack);
        if (stack_mem != NULL) {
          flags |= osRtxThreadFlagDefStack;
        }
      } else {
        stack_mem = osRtxMemoryAlloc(osRtxInfo.mem.stack, stack_size, 0U);
      }
    } else {
      stack_mem = osRtxMemoryAlloc(osRtxInfo.mem.stack, stack_size, 0U);
    }
    if (stack_mem == NULL) {
      EvrRtxThreadError(NULL, osErrorNoMemory);
      if (flags & osRtxFlagSystemObject) {
        if (osRtxInfo.mpi.thread != NULL) {
          osRtxMemoryPoolFree(osRtxInfo.mpi.thread, thread);
        } else {
          osRtxMemoryFree(osRtxInfo.mem.common, thread);
        }
      }
      return NULL;
    }
    flags |= osRtxFlagSystemMemory;
  }

#if (__DOMAIN_NS == 1U)
  // Allocate secure process stack
  if (tz_module != 0U) {
    tz_memory = TZ_AllocModuleContext_S(tz_module);
    if (tz_memory == 0U) {
      EvrRtxThreadError(NULL, osRtxErrorTZ_AllocContext_S);
      if (flags & osRtxFlagSystemMemory) {
        if (flags & osRtxThreadFlagDefStack) {
          osRtxMemoryPoolFree(osRtxInfo.mpi.stack, thread->stack_mem);
        } else {
          osRtxMemoryFree(osRtxInfo.mem.stack, thread->stack_mem);
        }
      }
      if (flags & osRtxFlagSystemObject) {
        if (osRtxInfo.mpi.thread != NULL) {
          osRtxMemoryPoolFree(osRtxInfo.mpi.thread, thread);
        } else {
          osRtxMemoryFree(osRtxInfo.mem.common, thread);
        }
      }
      return NULL;
    }
  } else {
    tz_memory = 0U;
  }
#endif

  // Initialize control block
  thread->id            = osRtxIdThread;
  thread->state         = osRtxThreadReady;
  thread->flags         = flags;
  thread->attr          = (uint8_t)attr_bits;
  thread->name          = name;
  thread->thread_next   = NULL;
  thread->thread_prev   = NULL;
  thread->delay_next    = NULL;
  thread->delay_prev    = NULL;
  thread->thread_join   = NULL;
  thread->delay         = 0U;
  thread->priority      = (int8_t)priority;
  thread->priority_base = (int8_t)priority;
  thread->stack_frame   = STACK_FRAME_INIT;
  thread->flags_options = 0U;
  thread->wait_flags    = 0U;
  thread->thread_flags  = 0U;
  thread->mutex_list    = NULL;
  thread->stack_mem     = stack_mem;
  thread->stack_size    = stack_size;
  thread->sp            = (uint32_t)stack_mem + stack_size - 64U;
  thread->thread_addr   = (uint32_t)func;
#if (__DOMAIN_NS == 1U)
  thread->tz_memory     = tz_memory;
#endif

  // Initialize stack
   ptr   = (uint32_t *)stack_mem;
  *ptr++ = osRtxStackMagicWord;
  if (osRtxConfig.flags & osRtxConfigStackWatermark) {
    for (n = (stack_size/4U) - (16U + 1U); n; n--) {
      *ptr++ = osRtxStackFillPattern;
    }
  } else {
    ptr = (uint32_t *)thread->sp;
  }
  for (n = 13U; n; n--) {
    *ptr++ = 0U;                        // R4..R11, R0..R3, R12
  }
  *ptr++   = (uint32_t)osThreadExit;    // LR
  *ptr++   = (uint32_t)func;            // PC
  *ptr++   = xPSR_INIT(
              (osRtxConfig.flags & osRtxConfigPrivilegedMode),
              ((uint32_t)func & 1U)
             );                         // xPSR
  *(ptr-8) = (uint32_t)argument;        // R0

  // Register post ISR processing function
  osRtxInfo.post_process.thread = osRtxThreadPostProcess;

  EvrRtxThreadCreated(thread);

  /* Notify the OS event observer of a new thread. */
  if (osEventObs && osEventObs->thread_create) {
    thread->context = osEventObs->thread_create((int)thread, context);
  } else {
    thread->context = context;
  }

  osRtxThreadDispatch(thread);

  return thread;
}

/// Get name of a thread.
/// \note API identical to osThreadGetName
const char *svcRtxThreadGetName (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread)) {
    EvrRtxThreadGetName(thread, NULL);
    return NULL;
  }

  // Check object state
  if (thread->state == osRtxObjectInactive) {
    EvrRtxThreadGetName(thread, NULL);
    return NULL;
  }

  EvrRtxThreadGetName(thread, thread->name);

  return thread->name;
}

/// Return the thread ID of the current running thread.
/// \note API identical to osThreadGetId
osThreadId_t svcRtxThreadGetId (void) {
  os_thread_t *thread;

  thread = osRtxThreadGetRunning();
  EvrRtxThreadGetId(thread);
  return thread;
}

/// Get current thread state of a thread.
/// \note API identical to osThreadGetState
osThreadState_t svcRtxThreadGetState (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread)) {
    EvrRtxThreadGetState(thread, osThreadError);
    return osThreadError;
  }

  EvrRtxThreadGetState(thread, (osThreadState_t)(thread->state & osRtxThreadStateMask));

  return ((osThreadState_t)(thread->state & osRtxThreadStateMask));
}

/// Get stack size of a thread.
/// \note API identical to osThreadGetStackSize
uint32_t svcRtxThreadGetStackSize (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread)) {
    EvrRtxThreadGetStackSize(thread, 0U);
    return 0U;
  }

  // Check object state
  if (thread->state == osRtxObjectInactive) {
    EvrRtxThreadGetStackSize(thread, 0U);
    return 0U;
  }

  EvrRtxThreadGetStackSize(thread, thread->stack_size);

  return thread->stack_size;
}

/// Get available stack space of a thread based on stack watermark recording during execution.
/// \note API identical to osThreadGetStackSpace
uint32_t svcRtxThreadGetStackSpace (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;
  uint32_t    *stack;
  uint32_t     space;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread)) {
    EvrRtxThreadGetStackSpace(thread, 0U);
    return 0U;
  }

  // Check object state
  if (thread->state == osRtxObjectInactive) {
    EvrRtxThreadGetStackSpace(thread, 0U);
    return 0U;
  }

  if ((osRtxConfig.flags & osRtxConfigStackWatermark) == 0U) {
    EvrRtxThreadGetStackSpace(thread, 0U);
    return 0U;
  }

  stack = thread->stack_mem;
  if (*stack++ != osRtxStackMagicWord) {
    EvrRtxThreadGetStackSpace(thread, 0U);
    return 0U;
  }
  for (space = 4U; space < thread->stack_size; space += 4U) {
    if (*stack++ != osRtxStackFillPattern) {
      break;
    }
  }

  EvrRtxThreadGetStackSpace(thread, space);

  return space;
}

/// Change priority of a thread.
/// \note API identical to osThreadSetPriority
osStatus_t svcRtxThreadSetPriority (osThreadId_t thread_id, osPriority_t priority) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread) ||
      (priority < osPriorityIdle) || (priority > osPriorityISR)) {
    EvrRtxThreadError(thread, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if ((thread->state == osRtxThreadInactive) ||
      (thread->state == osRtxThreadTerminated)) {
    EvrRtxThreadError(thread, osErrorResource);
    return osErrorResource;
  }

  if (thread->priority   != (int8_t)priority) {
    thread->priority      = (int8_t)priority;
    thread->priority_base = (int8_t)priority;
    osRtxThreadListSort(thread);
    osRtxThreadDispatch(NULL);
  }

  return osOK;
}

/// Get current priority of a thread.
/// \note API identical to osThreadGetPriority
osPriority_t svcRtxThreadGetPriority (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread)) {
    EvrRtxThreadGetPriority(thread, osPriorityError);
    return osPriorityError;
  }

  // Check object state
  if ((thread->state == osRtxThreadInactive) ||
      (thread->state == osRtxThreadTerminated)) {
    EvrRtxThreadGetPriority(thread, osPriorityError);
    return osPriorityError;
  }

  EvrRtxThreadGetPriority(thread, (osPriority_t)thread->priority);

  return ((osPriority_t)thread->priority);
}

/// Pass control to next thread that is in state READY.
/// \note API identical to osThreadYield
osStatus_t svcRtxThreadYield (void) {
  uint8_t      kernel_state;
  os_thread_t *thread_running;
  os_thread_t *thread_ready;

  kernel_state   = osRtxKernelGetState();
  thread_running = osRtxThreadGetRunning();
  thread_ready   = osRtxInfo.thread.ready.thread_list;
  if ((kernel_state == osRtxKernelRunning) &&
      (thread_ready != NULL) && (thread_running != NULL) &&
      (thread_ready->priority == thread_running->priority)) {
    osRtxThreadListRemove(thread_ready);
    osRtxThreadReadyPut(thread_running);
    osRtxThreadSwitch(thread_ready);
  }

  return osOK;
}

/// Suspend execution of a thread.
/// \note API identical to osThreadSuspend
osStatus_t svcRtxThreadSuspend (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread)) {
    EvrRtxThreadError(thread, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  switch (thread->state & osRtxThreadStateMask) {
    case osRtxThreadRunning:
      if ((osRtxKernelGetState() != osRtxKernelRunning) ||
          (osRtxInfo.thread.ready.thread_list == NULL)) {
        EvrRtxThreadError(thread, osErrorResource);
        return osErrorResource;
      }
      break;
    case osRtxThreadReady:
      osRtxThreadListRemove(thread);
      break;
    case osRtxThreadBlocked:
      osRtxThreadListRemove(thread);
      osRtxThreadDelayRemove(thread);
      break;
    case osRtxThreadInactive:
    case osRtxThreadTerminated:
    default:
      EvrRtxThreadError(thread, osErrorResource);
      return osErrorResource;
  }

  EvrRtxThreadSuspended(thread);

  if (thread->state == osRtxThreadRunning) {
    osRtxThreadSwitch(osRtxThreadListGet(&osRtxInfo.thread.ready));
  }

  // Update Thread State and put it into Delay list
  thread->state = osRtxThreadBlocked;
  thread->thread_prev = NULL;
  thread->thread_next = NULL;
  osRtxThreadDelayInsert(thread, osWaitForever);

  return osOK;
}

/// Resume execution of a thread.
/// \note API identical to osThreadResume
osStatus_t svcRtxThreadResume (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread)) {
    EvrRtxThreadError(thread, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if ((thread->state & osRtxThreadStateMask) != osRtxThreadBlocked) {
    EvrRtxThreadError(thread, osErrorResource);
    return osErrorResource;
  }

  EvrRtxThreadResumed(thread);

  // Wakeup Thread
  osRtxThreadListRemove(thread);
  osRtxThreadDelayRemove(thread);
  osRtxThreadDispatch(thread);

  return osOK;
}

/// Free Thread resources.
/// \param[in]  thread          thread object.
static void osRtxThreadFree (os_thread_t *thread) {

  // Mark object as inactive
  thread->state = osRtxThreadInactive;

#if (__DOMAIN_NS == 1U)
  // Free secure process stack
  if (thread->tz_memory != 0U) {
    TZ_FreeModuleContext_S(thread->tz_memory);
  }
#endif

  // Free stack memory
  if (thread->flags & osRtxFlagSystemMemory) {
    if (thread->flags & osRtxThreadFlagDefStack) {
      osRtxMemoryPoolFree(osRtxInfo.mpi.stack, thread->stack_mem);
    } else {
      osRtxMemoryFree(osRtxInfo.mem.stack, thread->stack_mem);
    }
  }

  // Free object memory
  if (thread->flags & osRtxFlagSystemObject) {
    if (osRtxInfo.mpi.thread != NULL) {
      osRtxMemoryPoolFree(osRtxInfo.mpi.thread, thread);
    } else {
      osRtxMemoryFree(osRtxInfo.mem.common, thread);
    }
  }
}

/// Detach a thread (thread storage can be reclaimed when thread terminates).
/// \note API identical to osThreadDetach
osStatus_t svcRtxThreadDetach (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread)) {
    EvrRtxThreadError(thread, osErrorParameter);
    return osErrorParameter;
  }

  // Check object attributes
  if ((thread->attr & osThreadJoinable) == 0U) {
    EvrRtxThreadError(thread, osRtxErrorThreadNotJoinable);
    return osErrorResource;
  }

  // Check object state
  if (thread->state == osRtxThreadInactive) {
    EvrRtxThreadError(thread, osErrorResource);
    return osErrorResource;
  }

  if (thread->state == osRtxThreadTerminated) {
    osRtxThreadListUnlink(&osRtxInfo.thread.terminate_list, thread);
    osRtxThreadFree(thread);
  } else {
    thread->attr &= ~osThreadJoinable;
  }

  EvrRtxThreadDetached(thread);

  return osOK;
}

/// Wait for specified thread to terminate.
/// \note API identical to osThreadJoin
osStatus_t svcRtxThreadJoin (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread)) {
    EvrRtxThreadError(thread, osErrorParameter);
    return osErrorParameter;
  }

  // Check object attributes
  if ((thread->attr & osThreadJoinable) == 0U) {
    EvrRtxThreadError(thread, osRtxErrorThreadNotJoinable);
    return osErrorResource;
  }

  // Check object state
  if ((thread->state == osRtxThreadInactive) ||
      (thread->state == osRtxThreadRunning)) {
    EvrRtxThreadError(thread, osErrorResource);
    return osErrorResource;
  }

  if (thread->state == osRtxThreadTerminated) {
    osRtxThreadListUnlink(&osRtxInfo.thread.terminate_list, thread);
    osRtxThreadFree(thread);
  } else {
    EvrRtxThreadJoinPending(thread);
    // Suspend current Thread
    if (osRtxThreadWaitEnter(osRtxThreadWaitingJoin, osWaitForever)) {
      thread->thread_join = osRtxThreadGetRunning();
    }
    return osErrorResource;
  }

  EvrRtxThreadJoined(thread);

  return osOK;
}

/// Terminate execution of current running thread.
/// \note API identical to osThreadExit
void svcRtxThreadExit (void) {
  os_thread_t *thread;

  thread = osRtxThreadGetRunning();
  if (thread == NULL) {
    return;
  }

  // Release owned Mutexes
  osRtxMutexOwnerRelease(thread->mutex_list);

  // Wakeup Thread waiting to Join
  if (thread->thread_join != NULL) {
    osRtxThreadWaitExit(thread->thread_join, (uint32_t)osOK, false);
    EvrRtxThreadJoined(thread->thread_join);
  }

  // Switch to next Ready Thread
  if ((osRtxKernelGetState() != osRtxKernelRunning) ||
      (osRtxInfo.thread.ready.thread_list == NULL)) {
    return;
  }
  thread->sp = __get_PSP();
  osRtxThreadSwitch(osRtxThreadListGet(&osRtxInfo.thread.ready));
  osRtxThreadSetRunning(NULL);

  if (((thread->attr & osThreadJoinable) == 0U) || (thread->thread_join != NULL)) {
    osRtxThreadFree(thread);
  } else {
    // Update Thread State and put it into Terminate Thread list
    thread->state = osRtxThreadTerminated;
    thread->thread_prev = NULL;
    thread->thread_next = osRtxInfo.thread.terminate_list;
    if (osRtxInfo.thread.terminate_list != NULL) {
      osRtxInfo.thread.terminate_list->thread_prev = thread;
    }
    osRtxInfo.thread.terminate_list = thread;
  }

  EvrRtxThreadDestroyed(thread);
}

/// Terminate execution of a thread.
/// \note API identical to osThreadTerminate
osStatus_t svcRtxThreadTerminate (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread)) {
    EvrRtxThreadError(thread, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  switch (thread->state & osRtxThreadStateMask) {
    case osRtxThreadRunning:
      break;
    case osRtxThreadReady:
      osRtxThreadListRemove(thread);
      break;
    case osRtxThreadBlocked:
      osRtxThreadListRemove(thread);
      osRtxThreadDelayRemove(thread);
      break;
    case osRtxThreadInactive:
    case osRtxThreadTerminated:
    default:
      EvrRtxThreadError(thread, osErrorResource);
      return osErrorResource;
  }

  if (osEventObs && osEventObs->thread_destroy) {
    osEventObs->thread_destroy(thread->context);
  }

  // Release owned Mutexes
  osRtxMutexOwnerRelease(thread->mutex_list);

  // Wakeup Thread waiting to Join
  if (thread->thread_join != NULL) {
    osRtxThreadWaitExit(thread->thread_join, (uint32_t)osOK, false);
    EvrRtxThreadJoined(thread->thread_join);
  }

  // Switch to next Ready Thread when terminating running Thread
  if (thread->state == osRtxThreadRunning) {
    if ((osRtxKernelGetState() != osRtxKernelRunning) ||
        (osRtxInfo.thread.ready.thread_list == NULL)) {
      EvrRtxThreadError(thread, osErrorResource);
      return osErrorResource;
    }
    thread->sp = __get_PSP();
    osRtxThreadSwitch(osRtxThreadListGet(&osRtxInfo.thread.ready));
    osRtxThreadSetRunning(NULL);
  } else {
    osRtxThreadDispatch(NULL);
  }

  if (((thread->attr & osThreadJoinable) == 0U) || (thread->thread_join != NULL)) {
    osRtxThreadFree(thread);
  } else {
    // Update Thread State and put it into Terminate Thread list
    thread->state = osRtxThreadTerminated;
    thread->thread_prev = NULL;
    thread->thread_next = osRtxInfo.thread.terminate_list;
    if (osRtxInfo.thread.terminate_list != NULL) {
      osRtxInfo.thread.terminate_list->thread_prev = thread;
    }
    osRtxInfo.thread.terminate_list = thread;
  }

  EvrRtxThreadDestroyed(thread);

  return osOK;
}

/// Get number of active threads.
/// \note API identical to osThreadGetCount
uint32_t svcRtxThreadGetCount (void) {
  os_thread_t *thread;
  uint32_t     count;

  // Running Thread
  count = 1U;

  // Ready List
  for (thread = osRtxInfo.thread.ready.thread_list;
       (thread != NULL); thread = thread->thread_next, count++) {};

  // Delay List
  for (thread = osRtxInfo.thread.delay_list;
       (thread != NULL); thread = thread->delay_next,  count++) {};

  // Wait List
  for (thread = osRtxInfo.thread.wait_list;
       (thread != NULL); thread = thread->delay_next,  count++) {};

  EvrRtxThreadGetCount(count);

  return count;
}

/// Enumerate active threads.
/// \note API identical to osThreadEnumerate
uint32_t svcRtxThreadEnumerate (osThreadId_t *thread_array, uint32_t array_items) {
  os_thread_t *thread;
  uint32_t     count;

  // Check parameters
  if ((thread_array == NULL) || (array_items == 0U)) {
    EvrRtxThreadEnumerate(thread_array, array_items, 0U);
    return 0U;
  }

  // Running Thread
  *thread_array++ = osRtxThreadGetRunning();
  count = 1U;

  // Ready List
  for (thread = osRtxInfo.thread.ready.thread_list;
       (thread != NULL) && (count < array_items); thread = thread->thread_next, count++) {
    *thread_array++ = thread;
  }

  // Delay List
  for (thread = osRtxInfo.thread.delay_list;
       (thread != NULL) && (count < array_items); thread = thread->delay_next,  count++) {
    *thread_array++ = thread;
  }

  // Wait List
  for (thread = osRtxInfo.thread.wait_list;
       (thread != NULL) && (count < array_items); thread = thread->delay_next,  count++) {
    *thread_array++ = thread;
  }

  EvrRtxThreadEnumerate(thread_array - count, array_items, count);

  return count;
}

/// Set the specified Thread Flags of a thread.
/// \note API identical to osThreadFlagsSet
uint32_t svcRtxThreadFlagsSet (osThreadId_t thread_id, uint32_t flags) {
  os_thread_t *thread = (os_thread_t *)thread_id;
  uint32_t     thread_flags;
  uint32_t     thread_flags0;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread) ||
      (flags & ~((1U << osRtxThreadFlagsLimit) - 1U))) {
    EvrRtxThreadError(thread, osErrorParameter);
    return ((uint32_t)osErrorParameter);
  }

  // Check object state
  if ((thread->state == osRtxThreadInactive) ||
      (thread->state == osRtxThreadTerminated)) {
    EvrRtxThreadError(thread, osErrorResource);
    return ((uint32_t)osErrorResource);
  }

  // Set Thread Flags
  thread_flags = ThreadFlagsSet(thread, flags);

  // Check if Thread is waiting for Thread Flags
  if (thread->state == osRtxThreadWaitingThreadFlags) {
    thread_flags0 = ThreadFlagsCheck(thread, thread->wait_flags, thread->flags_options);
    if (thread_flags0 != 0U) {
      if ((thread->flags_options & osFlagsNoClear) == 0U) {
        thread_flags = thread_flags0 & ~thread->wait_flags;
      } else {
        thread_flags = thread_flags0;
      }
      osRtxThreadWaitExit(thread, thread_flags0, true);
      EvrRtxThreadFlagsWaitCompleted(thread->wait_flags, thread->flags_options, thread_flags0);
    }
  }

  EvrRtxThreadFlagsSetDone(thread, thread_flags);

  return thread_flags;
}

/// Clear the specified Thread Flags of current running thread.
/// \note API identical to osThreadFlagsClear
uint32_t svcRtxThreadFlagsClear (uint32_t flags) {
  os_thread_t *thread;
  uint32_t     thread_flags;

  thread = osRtxThreadGetRunning();
  if (thread == NULL) {
    EvrRtxThreadError(NULL, osRtxErrorKernelNotRunning);
    return ((uint32_t)osError);
  }

  // Check parameters
  if (flags & ~((1U << osRtxThreadFlagsLimit) - 1U)) {
    EvrRtxThreadError(thread, osErrorParameter);
    return ((uint32_t)osErrorParameter);
  }

  // Check object state
  if ((thread->state == osRtxThreadInactive) ||
      (thread->state == osRtxThreadTerminated)) {
    EvrRtxThreadError(thread, osErrorResource);
    return ((uint32_t)osErrorResource);
  }

  // Clear Thread Flags
  thread_flags = ThreadFlagsClear(thread, flags);

  EvrRtxThreadFlagsClearDone(thread_flags);

  return thread_flags;
}

/// Get the current Thread Flags of current running thread.
/// \note API identical to osThreadFlagsGet
uint32_t svcRtxThreadFlagsGet (void) {
  os_thread_t *thread;

  thread = osRtxThreadGetRunning();
  if (thread == NULL) {
    EvrRtxThreadFlagsGet(0U);
    return 0U;
  }

  // Check object state
  if ((thread->state == osRtxThreadInactive) ||
      (thread->state == osRtxThreadTerminated)) {
    EvrRtxThreadFlagsGet(0U);
    return 0U;
  }

  EvrRtxThreadFlagsGet(thread->thread_flags);

  return thread->thread_flags;
}

/// Wait for one or more Thread Flags of the current running thread to become signaled.
/// \note API identical to osThreadFlagsWait
uint32_t svcRtxThreadFlagsWait (uint32_t flags, uint32_t options, uint32_t timeout) {
  os_thread_t *thread;
  uint32_t     thread_flags;

  thread = osRtxThreadGetRunning();
  if (thread == NULL) {
    EvrRtxThreadError(NULL, osRtxErrorKernelNotRunning);
    return ((uint32_t)osError);
  }

  // Check parameters
  if (flags & ~((1U << osRtxThreadFlagsLimit) - 1U)) {
    EvrRtxThreadError(thread, osErrorParameter);
    return ((uint32_t)osErrorParameter);
  }

  // Check Thread Flags
  thread_flags = ThreadFlagsCheck(thread, flags, options);
  if (thread_flags != 0U) {
    EvrRtxThreadFlagsWaitCompleted(flags, options, thread_flags);
    return thread_flags;
  }

  // Check if timeout is specified
  if (timeout != 0U) {
    // Store waiting flags and options
    EvrRtxThreadFlagsWaitPending(flags, options, timeout);
    thread->wait_flags = flags;
    thread->flags_options = (uint8_t)options;
    // Suspend current Thread
    osRtxThreadWaitEnter(osRtxThreadWaitingThreadFlags, timeout);
    return ((uint32_t)osErrorTimeout);
  }

  EvrRtxThreadFlagsWaitNotCompleted(flags, options);

  return ((uint32_t)osErrorResource);
}


//  ==== ISR Calls ====

/// Set the specified Thread Flags of a thread.
/// \note API identical to osThreadFlagsSet
__STATIC_INLINE
uint32_t isrRtxThreadFlagsSet (osThreadId_t thread_id, uint32_t flags) {
  os_thread_t *thread = (os_thread_t *)thread_id;
  uint32_t     thread_flags;

  // Check parameters
  if ((thread == NULL) || (thread->id != osRtxIdThread) ||
      (flags & ~((1U << osRtxThreadFlagsLimit) - 1U))) {
    EvrRtxThreadError(thread, osErrorParameter);
    return ((uint32_t)osErrorParameter);
  }

  // Check object state
  if ((thread->state == osRtxThreadInactive) ||
      (thread->state == osRtxThreadTerminated)) {
    EvrRtxThreadError(thread, osErrorResource);
    return ((uint32_t)osErrorResource);
  }

  // Set Thread Flags
  thread_flags = ThreadFlagsSet(thread, flags);

  // Register post ISR processing
  osRtxPostProcess((os_object_t *)thread);

  EvrRtxThreadFlagsSetDone(thread, thread_flags);

  return thread_flags;
}


//  ==== Public API ====

/// Create a thread and add it to Active Threads.
osThreadId_t osThreadNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr) {
  return osThreadContextNew(func, argument, attr, NULL);
}

osThreadId_t osThreadContextNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr, void *context) {
  EvrRtxThreadNew(func, argument, attr);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(NULL, osErrorISR);
    return NULL;
  }
  return __svcThreadNew(func, argument, attr, context);
}

/// Get name of a thread.
const char *osThreadGetName (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadGetName(thread_id, NULL);
    return NULL;
  }
  return __svcThreadGetName(thread_id);
}

/// Return the thread ID of the current running thread.
osThreadId_t osThreadGetId (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadGetId(NULL);
    return NULL;
  }
  return __svcThreadGetId();
}

/// Get current thread state of a thread.
osThreadState_t osThreadGetState (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadGetState(thread_id, osThreadError);
    return osThreadError;
  }
  return __svcThreadGetState(thread_id);
}

/// Get stack size of a thread.
uint32_t osThreadGetStackSize (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadGetStackSize(thread_id, 0U);
    return 0U;
  }
  return __svcThreadGetStackSize(thread_id);
}

/// Get available stack space of a thread based on stack watermark recording during execution.
uint32_t osThreadGetStackSpace (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadGetStackSpace(thread_id, 0U);
    return 0U;
  }
  return __svcThreadGetStackSpace(thread_id);
}

/// Change priority of a thread.
osStatus_t osThreadSetPriority (osThreadId_t thread_id, osPriority_t priority) {
  EvrRtxThreadSetPriority(thread_id, priority);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(thread_id, osErrorISR);
    return osErrorISR;
  }
  return __svcThreadSetPriority(thread_id, priority);
}

/// Get current priority of a thread.
osPriority_t osThreadGetPriority (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadGetPriority(thread_id, osPriorityError);
    return osPriorityError;
  }
  return __svcThreadGetPriority(thread_id);
}

/// Pass control to next thread that is in state READY.
osStatus_t osThreadYield (void) {
  EvrRtxThreadYield();
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(NULL, osErrorISR);
    return osErrorISR;
  }
  return __svcThreadYield();
}

/// Suspend execution of a thread.
osStatus_t osThreadSuspend (osThreadId_t thread_id) {
  EvrRtxThreadSuspend(thread_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(thread_id, osErrorISR);
    return osErrorISR;
  }
  return __svcThreadSuspend(thread_id);
}

/// Resume execution of a thread.
osStatus_t osThreadResume (osThreadId_t thread_id) {
  EvrRtxThreadResume(thread_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(thread_id, osErrorISR);
    return osErrorISR;
  }
  return __svcThreadResume(thread_id);
}

/// Detach a thread (thread storage can be reclaimed when thread terminates).
osStatus_t osThreadDetach (osThreadId_t thread_id) {
  EvrRtxThreadDetach(thread_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(thread_id, osErrorISR);
    return osErrorISR;
  }
  return __svcThreadDetach(thread_id);
}

/// Wait for specified thread to terminate.
osStatus_t osThreadJoin (osThreadId_t thread_id) {
  EvrRtxThreadJoin(thread_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(thread_id, osErrorISR);
    return osErrorISR;
  }
  return __svcThreadJoin(thread_id);
}

/// Terminate execution of current running thread.
__NO_RETURN void osThreadExit (void) {
  EvrRtxThreadExit();
  __svcThreadExit();
  EvrRtxThreadError(NULL, osError);
  for (;;);
}

/// Terminate execution of a thread.
osStatus_t osThreadTerminate (osThreadId_t thread_id) {
  EvrRtxThreadTerminate(thread_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(thread_id, osErrorISR);
    return osErrorISR;
  }
  return __svcThreadTerminate(thread_id);
}

/// Get number of active threads.
uint32_t osThreadGetCount (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadGetCount(0U);
    return 0U;
  }
  return __svcThreadGetCount();
}

/// Enumerate active threads.
uint32_t osThreadEnumerate (osThreadId_t *thread_array, uint32_t array_items) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadEnumerate(thread_array, array_items, 0U);
    return 0U;
  }
  return __svcThreadEnumerate(thread_array, array_items);
}

/// Set the specified Thread Flags of a thread.
uint32_t osThreadFlagsSet (osThreadId_t thread_id, uint32_t flags) {
  EvrRtxThreadFlagsSet(thread_id, flags);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return isrRtxThreadFlagsSet(thread_id, flags);
  } else {
    return  __svcThreadFlagsSet(thread_id, flags);
  }
}

/// Clear the specified Thread Flags of current running thread.
uint32_t osThreadFlagsClear (uint32_t flags) {
  EvrRtxThreadFlagsClear(flags);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(NULL, osErrorISR);
    return ((uint32_t)osErrorISR);
  }
  return __svcThreadFlagsClear(flags);
}

/// Get the current Thread Flags of current running thread.
uint32_t osThreadFlagsGet (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadFlagsGet(0U);
    return 0U;
  }
  return __svcThreadFlagsGet();
}

/// Wait for one or more Thread Flags of the current running thread to become signaled.
uint32_t osThreadFlagsWait (uint32_t flags, uint32_t options, uint32_t timeout) {
  EvrRtxThreadFlagsWait(flags, options, timeout);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(NULL, osErrorISR);
    return ((uint32_t)osErrorISR);
  }
  return __svcThreadFlagsWait(flags, options, timeout);
}
