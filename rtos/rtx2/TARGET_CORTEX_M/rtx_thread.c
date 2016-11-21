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
 * Title:       Thread functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Helper functions ====

/// Set Thread Flags.
/// \param[in]  thread          thread object.
/// \param[in]  flags           specifies the flags to set.
/// \return thread flags after setting.
static int32_t os_ThreadFlagsSet (os_thread_t *thread, int32_t flags) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  int32_t  thread_flags;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  thread->thread_flags |= flags;
  thread_flags = thread->thread_flags;

  if (primask == 0U) {
    __enable_irq();
  }
#else
  thread_flags = (int32_t)os_exc_set32((uint32_t *)&thread->thread_flags, (uint32_t)flags);
#endif

  return thread_flags;
}

/// Clear Thread Flags.
/// \param[in]  thread          thread object.
/// \param[in]  flags           specifies the flags to clear.
/// \return thread flags before clearing.
static int32_t os_ThreadFlagsClear (os_thread_t *thread, int32_t flags) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  int32_t  thread_flags;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  thread_flags = thread->thread_flags;
  thread->thread_flags &= ~flags;

  if (primask == 0U) {
    __enable_irq();
  }
#else
  thread_flags = (int32_t)os_exc_clr32((uint32_t *)&thread->thread_flags, (uint32_t)flags);
#endif

  return thread_flags;
}

/// Check Thread Flags.
/// \param[in]  thread          thread object.
/// \param[in]  flags           specifies the flags to check.
/// \param[in]  options         specifies flags options (osFlagsXxxx).
/// \return thread flags before clearing or 0 if specified flags have not been set.
static int32_t os_ThreadFlagsCheck (os_thread_t *thread, int32_t flags, uint32_t options) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask;
#endif
  int32_t  thread_flags;

  if ((options & osFlagsNoClear) == 0U) {
#if (__EXCLUSIVE_ACCESS == 0U)
    primask = __get_PRIMASK();
    __disable_irq();

    thread_flags = thread->thread_flags;
    if ((((options & osFlagsWaitAll) != 0U) && ((thread_flags & flags) != flags)) ||
        (((options & osFlagsWaitAll) == 0U) && ((thread_flags & flags) == 0))) {
      thread_flags = 0;
    } else {
      thread->thread_flags &= ~flags;
    }

    if (primask == 0U) {
      __enable_irq();
    }
#else
    if ((options & osFlagsWaitAll) != 0U) {
      thread_flags = (int32_t)os_exc_chk32_all((uint32_t *)&thread->thread_flags, (uint32_t)flags);
    } else {
      thread_flags = (int32_t)os_exc_chk32_any((uint32_t *)&thread->thread_flags, (uint32_t)flags);
    }
#endif
  } else {
    thread_flags = thread->thread_flags;
    if ((((options & osFlagsWaitAll) != 0U) && ((thread_flags & flags) != flags)) ||
        (((options & osFlagsWaitAll) == 0U) && ((thread_flags & flags) == 0))) {
      thread_flags = 0;
    }
  }

  return thread_flags;
}


//  ==== Library functions ====

/// Put a Thread into specified Object list sorted by Priority (Highest at Head).
/// \param[in]  object          generic object.
/// \param[in]  thread          thread object.
void os_ThreadListPut (volatile os_object_t *object, os_thread_t *thread) {
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
os_thread_t *os_ThreadListGet (volatile os_object_t *object) {
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

/// Re-sort a Thread in linked Object list by Priority (Highest at Head).
/// \param[in]  thread          thread object.
void os_ThreadListSort (os_thread_t *thread) {
  os_object_t *object;
  os_thread_t *thread0;

  // Search for object
  thread0 = thread;
  while (thread0->id == os_IdThread) {
    thread0 = thread0->thread_prev;
    if (thread0 == NULL) { 
      return;
    }
  }
  object = (os_object_t *)thread0;

  os_ThreadListRemove(thread);
  os_ThreadListPut(object, thread);
}

/// Remove a Thread from linked Object list.
/// \param[in]  thread          thread object.
void os_ThreadListRemove (os_thread_t *thread) {

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
void os_ThreadListUnlink (os_thread_t **thread_list, os_thread_t *thread) {

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
void os_ThreadReadyPut (os_thread_t *thread) {

  thread->state = os_ThreadReady;
  os_ThreadListPut(&os_Info.thread.ready, thread);
}

/// Insert a Thread into the Delay list sorted by Delay (Lowest at Head).
/// \param[in]  thread          thread object.
/// \param[in]  delay           delay value.
void os_ThreadDelayInsert (os_thread_t *thread, uint32_t delay) {
  os_thread_t *prev, *next;

  if (delay == osWaitForever) {
    prev = NULL;
    next = os_Info.thread.wait_list;
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
      os_Info.thread.wait_list = thread;
    }
    if (next != NULL) {
      next->delay_prev = thread;
    }
  } else {
    prev = NULL;
    next = os_Info.thread.delay_list;
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
      os_Info.thread.delay_list = thread;
    }
    if (next != NULL) {
      next->delay -= delay;
      next->delay_prev = thread;
    }
  }
}

/// Remove a Thread from the Delay list.
/// \param[in]  thread          thread object.
void os_ThreadDelayRemove (os_thread_t *thread) {

  if (thread->delay == osWaitForever) {
    if ((thread->delay_prev == NULL) && (os_Info.thread.wait_list != thread)) {
      return;
    }
    if (thread->delay_next != NULL) {
      thread->delay_next->delay_prev = thread->delay_prev;
    }
    if (thread->delay_prev != NULL) {
      thread->delay_prev->delay_next = thread->delay_next;
      thread->delay_prev = NULL;
    } else {
      os_Info.thread.wait_list = thread->delay_next;
    }
  } else {
    if ((thread->delay_prev == NULL) && (os_Info.thread.delay_list != thread)) {
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
      os_Info.thread.delay_list = thread->delay_next;
    }
  }
}

/// Process Thread Delay Tick (executed each System Tick).
void os_ThreadDelayTick (void) {
  os_thread_t *thread;

  thread = os_Info.thread.delay_list;
  if (thread == NULL) {
    return;
  }

  thread->delay--;

  if (thread->delay == 0U) {
    do {
      os_ThreadListRemove(thread);
      os_ThreadReadyPut(thread);
      thread = thread->delay_next;
    } while ((thread != NULL) && (thread->delay == 0U));
    if (thread != NULL) {
      thread->delay_prev = NULL;
    }
    os_Info.thread.delay_list = thread;
  }
}

/// Get pointer to Thread registers (R0..R3)
/// \param[in]  thread          thread object.
/// \return pointer to registers R0-R3.
uint32_t *os_ThreadRegPtr (os_thread_t *thread) {

#if (__FPU_USED == 1U)
  if (IS_EXTENDED_STACK_FRAME(thread->stack_frame)) {
    // Extended Stack Frame: S16-S31, R4-R11, R0-R3, R12, LR, PC, xPSR, S0-S15, FPSCR
    return ((uint32_t *)(thread->sp + (16U+8U)*4U));
  } else {
    // Basic Stack Frame:             R4-R11, R0-R3, R12, LR, PC, xPSR
    return ((uint32_t *)(thread->sp +      8U *4U));
  }
#else
  // Stack Frame: R4-R11, R0-R3, R12, LR, PC, xPSR
  return ((uint32_t *)(thread->sp + 8U*4U));
#endif
}

/// Block running Thread execution and register it as Ready to Run.
/// \param[in]  thread          running thread object.
void os_ThreadBlock (os_thread_t *thread) {
  os_thread_t *prev, *next;
  int32_t      priority;

  thread->state = os_ThreadReady;

  priority = thread->priority;

  prev = (os_thread_t *)(uint32_t)&os_Info.thread.ready;
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
void os_ThreadSwitch (os_thread_t *thread) {

  thread->state = os_ThreadRunning;
  os_Info.thread.run.next = thread;
  os_ThreadStackCheck();
}

/// Dispatch specified Thread or Ready Thread with Highest Priority.
/// \param[in]  thread          thread object or NULL.
void os_ThreadDispatch (os_thread_t *thread) {
  uint8_t      kernel_state;
  os_thread_t *thread_running;

  kernel_state   = os_KernelGetState();
  thread_running = os_ThreadGetRunning();

  if (thread == NULL) {
    thread = os_Info.thread.ready.thread_list;
    if ((kernel_state == os_KernelRunning) &&
        (thread_running != NULL) && (thread != NULL) && 
        (thread->priority > thread_running->priority)) {
      // Preempt running Thread
      os_ThreadListRemove(thread);
      os_ThreadBlock(thread_running);
      os_ThreadSwitch(thread);
    }
  } else {
    if ((kernel_state == os_KernelRunning) &&
        (thread_running != NULL) &&
        (thread->priority > thread_running->priority)) {
      // Preempt running Thread
      os_ThreadBlock(thread_running);
      os_ThreadSwitch(thread);
    } else {
      // Put Thread into Ready list
      os_ThreadReadyPut(thread);
    }
  }
}

/// Exit Thread wait state.
/// \param[in]  thread          thread object.
/// \param[in]  ret_val         return value.
/// \param[in]  dispatch        dispatch flag.
void os_ThreadWaitExit (os_thread_t *thread, uint32_t ret_val, bool dispatch) {
  uint32_t *reg;

  reg = os_ThreadRegPtr(thread);
  reg[0] = ret_val;

  os_ThreadDelayRemove(thread);
  if (dispatch) {
    os_ThreadDispatch(thread);
  } else {
    os_ThreadReadyPut(thread);
  }
}

/// Enter Thread wait state.
/// \param[in]  state           new thread state.
/// \param[in]  timeout         timeout.
/// \return true - success, false - failure.
bool os_ThreadWaitEnter (uint8_t state, uint32_t timeout) {
  os_thread_t *thread;

  thread = os_ThreadGetRunning();
  if (thread == NULL) {
    return false;
  }

  if (os_KernelGetState() != os_KernelRunning) {
    os_ThreadListRemove(thread);
    return false;
  }

  if (os_Info.thread.ready.thread_list == NULL) {
    return false;
  }

  thread->state = state;
  os_ThreadDelayInsert(thread, timeout);
  thread = os_ThreadListGet(&os_Info.thread.ready);
  os_ThreadSwitch(thread);

  return true;
}

/// Check current running Thread Stack.
__WEAK void os_ThreadStackCheck (void) {
  os_thread_t *thread;

  thread = os_ThreadGetRunning();
  if (thread != NULL) {
    if ((thread->sp <= (uint32_t)thread->stack_mem) ||
        (*((uint32_t *)thread->stack_mem) != os_StackMagicWord)) {
      os_Error(os_ErrorStackUnderflow, thread);
    }
  }
}

/// Thread post ISR processing.
/// \param[in]  thread          thread object.
void os_ThreadPostProcess (os_thread_t *thread) {
  int32_t thread_flags;

  if ((thread->state == os_ThreadInactive) ||
      (thread->state == os_ThreadTerminated)) {
    return;
  }

  // Check if Thread is waiting for Thread Flags
  if (thread->state == os_ThreadWaitingThreadFlags) {
    thread_flags = os_ThreadFlagsCheck(thread, thread->wait_flags, thread->flags_options);
    if (thread_flags > 0) {
      os_ThreadWaitExit(thread, (uint32_t)thread_flags, false);
    }
  }
}


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_3 (ThreadNew,           osThreadId_t,    os_thread_func_t, void *, const osThreadAttr_t *)
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
SVC0_2 (ThreadFlagsSet,      int32_t,         osThreadId_t, int32_t)
SVC0_1 (ThreadFlagsClear,    int32_t,         int32_t)
SVC0_0 (ThreadFlagsGet,      int32_t)
SVC0_3 (ThreadFlagsWait,     int32_t,         int32_t, uint32_t, uint32_t)

/// Create a thread and add it to Active Threads.
/// \note API identical to osThreadNew
osThreadId_t os_svcThreadNew (os_thread_func_t func, void *argument, const osThreadAttr_t *attr) {
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
        return NULL;
      }
    } else {
      if (attr->cb_size != 0U) {
        return NULL;
      }
    }
    if (stack_mem != NULL) {
      if (((uint32_t)stack_mem & 7U) || (stack_size == 0U)) {
        return NULL;
      }
    }
    if (priority == osPriorityNone) {
      priority = osPriorityNormal;
    } else {
      if ((priority < osPriorityIdle) || (priority > osPriorityISR)) {
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
    return NULL;
  }

  // Allocate object memory if not provided
  if (thread == NULL) {
    if (os_Info.mpi.thread != NULL) {
      thread = os_MemoryPoolAlloc(os_Info.mpi.thread);
    } else {
      thread = os_MemoryAlloc(os_Info.mem.common, sizeof(os_thread_t), 1U);
    }
    if (thread == NULL) {
      return NULL;
    }
    flags = os_FlagSystemObject;
  } else {
    flags = 0U;
  }

  // Allocate stack memory if not provided
  if (stack_mem == NULL) {
    if (stack_size == 0U) {
      stack_size = os_Config.thread_stack_size;
      if (os_Info.mpi.stack != NULL) {
        stack_mem = os_MemoryPoolAlloc(os_Info.mpi.stack);
        if (stack_mem != NULL) {
          flags |= os_ThreadFlagDefStack;
        }
      }
    }
    if (stack_mem == NULL) {
      stack_mem = os_MemoryAlloc(os_Info.mem.stack, stack_size, 0U);
    }
    if (stack_mem == NULL) {
      if (flags & os_FlagSystemObject) {
        if (os_Info.mpi.thread != NULL) {
          os_MemoryPoolFree(os_Info.mpi.thread, thread);
        } else {
          os_MemoryFree(os_Info.mem.common, thread);
        }
      }
      return NULL;
    }
    flags |= os_FlagSystemMemory;
  }

#if (__DOMAIN_NS == 1U)
  // Allocate secure process stack
  if (tz_module != 0U) {
    tz_memory = TZ_AllocModuleContext_S(tz_module);
    if (tz_memory == 0U) {
      if (flags & os_FlagSystemMemory) {
        if (flags & os_ThreadFlagDefStack) {
          os_MemoryPoolFree(os_Info.mpi.stack, thread->stack_mem);
        } else {
          os_MemoryFree(os_Info.mem.stack, thread->stack_mem);
        }
      }
      if (flags & os_FlagSystemObject) {
        if (os_Info.mpi.thread != NULL) {
          os_MemoryPoolFree(os_Info.mpi.thread, thread);
        } else {
          os_MemoryFree(os_Info.mem.common, thread);
        }
      }
      return NULL;
    }
  } else {
    tz_memory = 0U;
  }
#endif

  // Initialize control block
  thread->id            = os_IdThread;
  thread->state         = os_ThreadReady;
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
  thread->wait_flags    = 0;
  thread->thread_flags  = 0;
  thread->mutex_list    = NULL;
  thread->stack_mem     = stack_mem;
  thread->stack_size    = stack_size;
  thread->sp            = (uint32_t)stack_mem + stack_size - 64U;
#if (__DOMAIN_NS == 1U)
  thread->tz_memory     = tz_memory;
#endif

  // Initialize stack
   ptr   = (uint32_t *)stack_mem;
  *ptr++ = os_StackMagicWord;
  if (os_Config.flags & os_ConfigStackWatermark) {
    for (n = (stack_size/4U) - (16U + 1U); n; n--) {
      *ptr++ = os_StackFillPattern;
    }
  } else {
    ptr = (uint32_t *)thread->sp;
  }
  for (n = 13U; n; n--) {
    *ptr++ = 0U;                        // R4..R11, R0..R3, R12
  }
  *ptr++   = (uint32_t)osThreadExit;    // LR
  *ptr++   = (uint32_t)func;            // PC
  *ptr++   = XPSR_INITIAL_VALUE;        // xPSR
  *(ptr-8) = (uint32_t)argument;        // R0

  // Register post ISR processing function
  os_Info.post_process.thread = os_ThreadPostProcess;

  os_ThreadDispatch(thread);

  return thread;
}

/// Get name of a thread.
/// \note API identical to osThreadGetName
const char *os_svcThreadGetName (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return NULL;
  }

  // Check object state
  if (thread->state == os_ObjectInactive) {
    return NULL;
  }

  return thread->name;
}

/// Return the thread ID of the current running thread.
/// \note API identical to osThreadGetId
osThreadId_t os_svcThreadGetId (void) {
  os_thread_t *thread;

  thread = os_ThreadGetRunning();
  return thread;
}

/// Get current thread state of a thread.
/// \note API identical to osThreadGetState
osThreadState_t os_svcThreadGetState (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return osThreadError;
  }

  return ((osThreadState_t)(thread->state & os_ThreadStateMask));
}

/// Get stack size of a thread.
/// \note API identical to osThreadGetStackSize
uint32_t os_svcThreadGetStackSize (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return 0U;
  }

  // Check object state
  if (thread->state == os_ObjectInactive) {
    return 0U;
  }

  return thread->stack_size;
}

/// Get available stack space of a thread based on stack watermark recording during execution.
/// \note API identical to osThreadGetStackSpace
uint32_t os_svcThreadGetStackSpace (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;
  uint32_t    *stack;
  uint32_t     space;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return 0U;
  }

  // Check object state
  if (thread->state == os_ObjectInactive) {
    return 0U;
  }

  if ((os_Config.flags & os_ConfigStackWatermark) == 0U) {
    return 0U;
  }

  stack = thread->stack_mem;
  if (*stack++ != os_StackMagicWord) {
    return 0U;
  }
  for (space = 4U; space < thread->stack_size; space += 4U) {
    if (*stack++ != os_StackFillPattern) {
      break;
    }
  }

  return space;
}

/// Change priority of a thread.
/// \note API identical to osThreadSetPriority
osStatus_t os_svcThreadSetPriority (osThreadId_t thread_id, osPriority_t priority) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return osErrorParameter;
  }
  if ((priority < osPriorityIdle) || (priority > osPriorityISR)) {
    return osErrorParameter;
  }

  // Check object state
  if ((thread->state == os_ThreadInactive) ||
      (thread->state == os_ThreadTerminated)) {
    return osErrorResource;
  }

  if (thread->priority   != (int8_t)priority) {
    thread->priority      = (int8_t)priority;
    thread->priority_base = (int8_t)priority;
    os_ThreadListSort(thread);
    os_ThreadDispatch(NULL);
  }

  return osOK;
}

/// Get current priority of a thread.
/// \note API identical to osThreadGetPriority
osPriority_t os_svcThreadGetPriority (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return osPriorityError;
  }

  // Check object state
  if ((thread->state == os_ThreadInactive) ||
      (thread->state == os_ThreadTerminated)) {
    return osPriorityError;
  }

  return ((osPriority_t)thread->priority);
}

/// Pass control to next thread that is in state READY.
/// \note API identical to osThreadYield
osStatus_t os_svcThreadYield (void) {
  uint8_t      kernel_state;
  os_thread_t *thread_running;
  os_thread_t *thread_ready;

  kernel_state   = os_KernelGetState();
  thread_running = os_ThreadGetRunning();
  thread_ready   = os_Info.thread.ready.thread_list;
  if ((kernel_state == os_KernelRunning) &&
      (thread_ready != NULL) && (thread_running != NULL) &&
      (thread_ready->priority == thread_running->priority)) {
    os_ThreadListRemove(thread_ready);
    os_ThreadReadyPut(thread_running);
    os_ThreadSwitch(thread_ready);
  }

  return osOK;
}

/// Suspend execution of a thread.
/// \note API identical to osThreadSuspend
osStatus_t os_svcThreadSuspend (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return osErrorParameter;
  }

  // Check object state
  switch (thread->state & os_ThreadStateMask) {
    case os_ThreadRunning:
      if ((os_KernelGetState() != os_KernelRunning) ||
          (os_Info.thread.ready.thread_list == NULL)) {
        return osErrorResource;
      }
      os_ThreadSwitch(os_ThreadListGet(&os_Info.thread.ready));
      break;
    case os_ThreadReady:
      os_ThreadListRemove(thread);
      break;
    case os_ThreadBlocked:
      os_ThreadListRemove(thread);
      os_ThreadDelayRemove(thread);
      break;
    case os_ThreadInactive:
    case os_ThreadTerminated:
    default:
      return osErrorResource;
  }

  // Update Thread State and put it into Delay list
  thread->state = os_ThreadBlocked;
  thread->thread_prev = NULL;
  thread->thread_next = NULL;
  os_ThreadDelayInsert(thread, osWaitForever);

  return osOK;
}

/// Resume execution of a thread.
/// \note API identical to osThreadResume
osStatus_t os_svcThreadResume (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return osErrorParameter;
  }

  // Check object state
  if ((thread->state & os_ThreadStateMask) != os_ThreadBlocked) {
    return osErrorResource;
  }

  // Wakeup Thread
  os_ThreadListRemove(thread);
  os_ThreadDelayRemove(thread);
  os_ThreadDispatch(thread);

  return osOK;
}

/// Free Thread resources.
/// \param[in]  thread          thread object.
static void os_ThreadFree (os_thread_t *thread) {

  // Mark object as inactive
  thread->state = os_ThreadInactive;

#if (__DOMAIN_NS == 1U)
  // Free secure process stack
  if (thread->tz_memory != 0U) {
    TZ_FreeModuleContext_S(thread->tz_memory);
  }
#endif

  // Free stack memory
  if (thread->flags & os_FlagSystemMemory) {
    if (thread->flags & os_ThreadFlagDefStack) {
      os_MemoryPoolFree(os_Info.mpi.stack, thread->stack_mem);
    } else {
      os_MemoryFree(os_Info.mem.stack, thread->stack_mem);
    }
  }

  // Free object memory
  if (thread->flags & os_FlagSystemObject) {
    if (os_Info.mpi.thread != NULL) {
      os_MemoryPoolFree(os_Info.mpi.thread, thread);
    } else {
      os_MemoryFree(os_Info.mem.common, thread);
    }
  }
}

/// Detach a thread (thread storage can be reclaimed when thread terminates).
/// \note API identical to osThreadDetach
osStatus_t os_svcThreadDetach (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return osErrorParameter;
  }

  if ((thread->attr & osThreadJoinable) == 0U) {
    return osErrorResource;
  }

  // Check object state
  if (thread->state == os_ThreadInactive) {
    return osErrorResource;
  }

  if (thread->state == os_ThreadTerminated) {
    os_ThreadListUnlink(&os_Info.thread.terminate_list, thread);
    os_ThreadFree(thread);
  } else {
    thread->attr &= ~osThreadJoinable;
  }

  return osOK;
}

/// Wait for specified thread to terminate.
/// \note API identical to osThreadJoin
osStatus_t os_svcThreadJoin (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return osErrorParameter;
  }

  if ((thread->attr & osThreadJoinable) == 0U) {
    return osErrorResource;
  }

  // Check object state
  if ((thread->state == os_ThreadInactive) ||
      (thread->state == os_ThreadRunning)) {
    return osErrorResource;
  }

  if (thread->state == os_ThreadTerminated) {
    os_ThreadListUnlink(&os_Info.thread.terminate_list, thread);
    os_ThreadFree(thread);
  } else {
    // Suspend current Thread
    if (os_ThreadWaitEnter(os_ThreadWaitingJoin, osWaitForever)) {
      thread->thread_join = os_ThreadGetRunning();
    }
    return osErrorResource;
  }

  return osOK;
}

/// Terminate execution of current running thread.
/// \note API identical to osThreadExit
void os_svcThreadExit (void) {
  os_thread_t *thread;

  thread = os_ThreadGetRunning();
  if (thread == NULL) {
    return;
  }

  // Release owned Mutexes
  os_MutexOwnerRelease(thread->mutex_list);

  // Wakeup Thread waiting to Join
  if (thread->thread_join != NULL) {
    os_ThreadWaitExit(thread->thread_join, (uint32_t)osOK, false);
  }

  // Switch to next Ready Thread
  if ((os_KernelGetState() != os_KernelRunning) ||
      (os_Info.thread.ready.thread_list == NULL)) {
    return;
  }
  thread->sp = __get_PSP();
  os_ThreadSwitch(os_ThreadListGet(&os_Info.thread.ready));
  os_ThreadSetRunning(NULL);

  if (((thread->attr & osThreadJoinable) == 0U) || (thread->thread_join != NULL)) {
    os_ThreadFree(thread);
  } else {
    // Update Thread State and put it into Terminate Thread list
    thread->state = os_ThreadTerminated;
    thread->thread_prev = NULL;
    thread->thread_next = os_Info.thread.terminate_list;
    os_Info.thread.terminate_list = thread;
  }
}

/// Terminate execution of a thread.
/// \note API identical to osThreadTerminate
osStatus_t os_svcThreadTerminate (osThreadId_t thread_id) {
  os_thread_t *thread = (os_thread_t *)thread_id;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return osErrorParameter;
  }

  // Check object state
  switch (thread->state & os_ThreadStateMask) {
    case os_ThreadRunning:
      break;
    case os_ThreadReady:
      os_ThreadListRemove(thread);
      break;
    case os_ThreadBlocked:
      os_ThreadListRemove(thread);
      os_ThreadDelayRemove(thread);
      break;
    case os_ThreadInactive:
    case os_ThreadTerminated:
    default:
      return osErrorResource;
  }

  // Release owned Mutexes
  os_MutexOwnerRelease(thread->mutex_list);

  // Wakeup Thread waiting to Join
  if (thread->thread_join != NULL) {
    os_ThreadWaitExit(thread->thread_join, (uint32_t)osOK, false);
  }

  // Switch to next Ready Thread when terminating running Thread
  if (thread->state == os_ThreadRunning) {
    if ((os_KernelGetState() != os_KernelRunning) ||
        (os_Info.thread.ready.thread_list == NULL)) {
      return osErrorResource;
    }
    thread->sp = __get_PSP();
    os_ThreadSwitch(os_ThreadListGet(&os_Info.thread.ready));
    os_ThreadSetRunning(NULL);
  } else {
    os_ThreadDispatch(NULL);
  }

  if (((thread->attr & osThreadJoinable) == 0U) || (thread->thread_join != NULL)) {
    os_ThreadFree(thread);
  } else {
    // Update Thread State and put it into Terminate Thread list
    thread->state = os_ThreadTerminated;
    thread->thread_prev = NULL;
    thread->thread_next = os_Info.thread.terminate_list;
    os_Info.thread.terminate_list = thread;
  }

  return osOK;
}

/// Get number of active threads.
/// \note API identical to osThreadGetCount
uint32_t os_svcThreadGetCount (void) {
  os_thread_t *thread;
  uint32_t     count;

  // Running Thread
  count = 1U;

  // Ready List
  for (thread = os_Info.thread.ready.thread_list;
       (thread != NULL); thread = thread->thread_next, count++);

  // Delay List
  for (thread = os_Info.thread.delay_list;
       (thread != NULL); thread = thread->delay_next,  count++);

  // Wait List
  for (thread = os_Info.thread.wait_list;
       (thread != NULL); thread = thread->delay_next,  count++);

  return count;
}

/// Enumerate active threads.
/// \note API identical to osThreadEnumerate
uint32_t os_svcThreadEnumerate (osThreadId_t *thread_array, uint32_t array_items) {
  os_thread_t *thread;
  uint32_t     count;

  // Check parameters
  if ((thread_array == NULL) || (array_items == 0U)) {
    return 0U;
  }

  // Running Thread
  *thread_array++ = os_ThreadGetRunning();
  count = 1U;

  // Ready List
  for (thread = os_Info.thread.ready.thread_list;
       (thread != NULL) && (count < array_items); thread = thread->thread_next, count++) {
    *thread_array++ = thread;
  }

  // Delay List
  for (thread = os_Info.thread.delay_list;
       (thread != NULL) && (count < array_items); thread = thread->delay_next,  count++) {
    *thread_array++ = thread;
  }

  // Wait List
  for (thread = os_Info.thread.wait_list;
       (thread != NULL) && (count < array_items); thread = thread->delay_next,  count++) {
    *thread_array++ = thread;
  }

  return count;
}

/// Set the specified Thread Flags of a thread.
/// \note API identical to osThreadFlagsSet
int32_t os_svcThreadFlagsSet (osThreadId_t thread_id, int32_t flags) {
  os_thread_t *thread = (os_thread_t *)thread_id;
  int32_t      thread_flags;
  int32_t      thread_flags0;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return osErrorParameter;
  }
  if ((uint32_t)flags & ~((1U << os_ThreadFlagsLimit) - 1U)) {
    return osErrorParameter;
  }

  // Check object state
  if ((thread->state == os_ThreadInactive) ||
      (thread->state == os_ThreadTerminated)) {
    return osErrorResource;
  }

  // Set Thread Flags
  thread_flags = os_ThreadFlagsSet(thread, flags);

  // Check if Thread is waiting for Thread Flags
  if (thread->state == os_ThreadWaitingThreadFlags) {
    thread_flags0 = os_ThreadFlagsCheck(thread, thread->wait_flags, thread->flags_options);
    if (thread_flags0 > 0) {
      if ((thread->flags_options & osFlagsNoClear) == 0U) {
        thread_flags = thread_flags0 & ~thread->wait_flags;
      } else {
        thread_flags = thread_flags0;
      }
      os_ThreadWaitExit(thread, (uint32_t)thread_flags0, true);
    }
  }

  return thread_flags;
}

/// Clear the specified Thread Flags of current running thread.
/// \note API identical to osThreadFlagsClear
int32_t os_svcThreadFlagsClear (int32_t flags) {
  os_thread_t *thread;

  thread = os_ThreadGetRunning();
  if (thread == NULL) {
    return osError;
  }

  // Check parameters
  if ((uint32_t)flags & ~((1U << os_ThreadFlagsLimit) - 1U)) {
    return osErrorParameter;
  }

  // Check object state
  if ((thread->state == os_ThreadInactive) ||
      (thread->state == os_ThreadTerminated)) {
    return osErrorResource;
  }

  // Clear Thread Flags
  return os_ThreadFlagsClear(thread, flags);
}

/// Get the current Thread Flags of current running thread.
/// \note API identical to osThreadFlagsGet
int32_t os_svcThreadFlagsGet (void) {
  os_thread_t *thread;

  thread = os_ThreadGetRunning();
  if (thread == NULL) {
    return 0;
  }

  // Check object state
  if ((thread->state == os_ThreadInactive) ||
      (thread->state == os_ThreadTerminated)) {
    return 0;
  }

  return thread->thread_flags;
}

/// Wait for one or more Thread Flags of the current running thread to become signaled.
/// \note API identical to osThreadFlagsWait
int32_t os_svcThreadFlagsWait (int32_t flags, uint32_t options, uint32_t timeout) {
  os_thread_t *thread;
  int32_t      thread_flags;

  thread = os_ThreadGetRunning();
  if (thread == NULL) {
    return osError;
  }

  // Check parameters
  if ((uint32_t)flags & ~((1U << os_ThreadFlagsLimit) - 1U)) {
    return osErrorParameter;
  }

  // Check Thread Flags
  thread_flags = os_ThreadFlagsCheck(thread, flags, options);
  if (thread_flags > 0) {
    return thread_flags;
  }

  // Check if timeout is specified
  if (timeout != 0U) {
    // Store waiting flags and options
    thread->wait_flags = flags;
    thread->flags_options = (uint8_t)options;
    // Suspend current Thread
    os_ThreadWaitEnter(os_ThreadWaitingThreadFlags, timeout);
    return osErrorTimeout;
  }

  return osErrorResource;
}


//  ==== ISR Calls ====

/// Set the specified Thread Flags of a thread.
/// \note API identical to osThreadFlagsSet
__STATIC_INLINE
int32_t os_isrThreadFlagsSet (osThreadId_t thread_id, int32_t flags) {
  os_thread_t *thread = (os_thread_t *)thread_id;
  int32_t      thread_flags;

  // Check parameters
  if ((thread == NULL) ||
      (thread->id != os_IdThread)) {
    return osErrorParameter;
  }
  if ((uint32_t)flags & ~((1U << os_ThreadFlagsLimit) - 1U)) {
    return osErrorParameter;
  }

  // Check object state
  if ((thread->state == os_ThreadInactive) ||
      (thread->state == os_ThreadTerminated)) {
    return osErrorResource;
  }

  // Set Thread Flags
  thread_flags = os_ThreadFlagsSet(thread, flags);

  // Register post ISR processing
  os_PostProcess((os_object_t *)thread);

  return thread_flags;
}


//  ==== Public API ====

/// Create a thread and add it to Active Threads.
osThreadId_t osThreadNew (os_thread_func_t func, void *argument, const osThreadAttr_t *attr) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return NULL;
  }
  if ((os_KernelGetState() == os_KernelReady) && IS_PRIVILEGED()) {
    // Kernel Ready (not running) and in Privileged mode
    return os_svcThreadNew(func, argument, attr);
  } else {
    return  __svcThreadNew(func, argument, attr);
  }
}

/// Get name of a thread.
const char *osThreadGetName (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return NULL;
  }
  return  __svcThreadGetName(thread_id);
}

/// Return the thread ID of the current running thread.
osThreadId_t osThreadGetId (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return NULL;
  }
  return __svcThreadGetId();
}

/// Get current thread state of a thread.
osThreadState_t osThreadGetState (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osThreadError;
  }
  return __svcThreadGetState(thread_id);
}

/// Get stack size of a thread.
uint32_t osThreadGetStackSize (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return 0U;
  }
  return __svcThreadGetStackSize(thread_id);
}

/// Get available stack space of a thread based on stack watermark recording during execution.
uint32_t osThreadGetStackSpace (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return 0U;
  }
  return __svcThreadGetStackSpace(thread_id);
}

/// Change priority of a thread.
osStatus_t osThreadSetPriority (osThreadId_t thread_id, osPriority_t priority) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return __svcThreadSetPriority(thread_id, priority);
}

/// Get current priority of a thread.
osPriority_t osThreadGetPriority (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osPriorityError;
  }
  return __svcThreadGetPriority(thread_id);
}

/// Pass control to next thread that is in state READY.
osStatus_t osThreadYield (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return __svcThreadYield();
}

/// Suspend execution of a thread.
osStatus_t osThreadSuspend (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return __svcThreadSuspend(thread_id);
}

/// Resume execution of a thread.
osStatus_t osThreadResume (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return __svcThreadResume(thread_id);
}

/// Detach a thread (thread storage can be reclaimed when thread terminates).
osStatus_t osThreadDetach (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return __svcThreadDetach(thread_id);
}

/// Wait for specified thread to terminate.
osStatus_t osThreadJoin (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return __svcThreadJoin(thread_id);
}

/// Terminate execution of current running thread.
__NO_RETURN void osThreadExit (void) {
  __svcThreadExit();
  for (;;);
}

/// Terminate execution of a thread.
osStatus_t osThreadTerminate (osThreadId_t thread_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return __svcThreadTerminate(thread_id);
}

/// Get number of active threads.
uint32_t osThreadGetCount (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return 0U;                                  // Not allowed in ISR
  }
  return __svcThreadGetCount();
}

/// Enumerate active threads.
uint32_t osThreadEnumerate (osThreadId_t *thread_array, uint32_t array_items) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return 0U;                                  // Not allowed in ISR
  }
  return __svcThreadEnumerate(thread_array, array_items);
}

/// Set the specified Thread Flags of a thread.
int32_t osThreadFlagsSet (osThreadId_t thread_id, int32_t flags) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return os_isrThreadFlagsSet(thread_id, flags);
  } else {
    return  __svcThreadFlagsSet(thread_id, flags);
  }
}

/// Clear the specified Thread Flags of current running thread.
int32_t osThreadFlagsClear (int32_t flags) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return  __svcThreadFlagsClear(flags);
}

/// Get the current Thread Flags of current running thread.
int32_t osThreadFlagsGet (void) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return 0;
  }                               
  return  __svcThreadFlagsGet();
}

/// Wait for one or more Thread Flags of the current running thread to become signaled.
int32_t osThreadFlagsWait (int32_t flags, uint32_t options, uint32_t timeout) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return  __svcThreadFlagsWait(flags, options, timeout);
}
