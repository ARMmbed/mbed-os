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
 * Title:       Event Flags functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Helper functions ====

/// Set Event Flags.
/// \param[in]  ef              event flags object.
/// \param[in]  flags           specifies the flags to set.
/// \return event flags after setting.
static uint32_t EventFlagsSet (os_event_flags_t *ef, uint32_t flags) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  uint32_t event_flags;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  ef->event_flags |= flags;
  event_flags = ef->event_flags;

  if (primask == 0U) {
    __enable_irq();
  }
#else
  event_flags = atomic_set32(&ef->event_flags, flags);
#endif

  return event_flags;
}

/// Clear Event Flags.
/// \param[in]  ef              event flags object.
/// \param[in]  flags           specifies the flags to clear.
/// \return event flags before clearing.
static uint32_t EventFlagsClear (os_event_flags_t *ef, uint32_t flags) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  uint32_t event_flags;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  event_flags = ef->event_flags;
  ef->event_flags &= ~flags;

  if (primask == 0U) {
    __enable_irq();
  }
#else
  event_flags = atomic_clr32(&ef->event_flags, flags);
#endif

  return event_flags;
}

/// Check Event Flags.
/// \param[in]  ef              event flags object.
/// \param[in]  flags           specifies the flags to check.
/// \param[in]  options         specifies flags options (osFlagsXxxx).
/// \return event flags before clearing or 0 if specified flags have not been set.
static uint32_t EventFlagsCheck (os_event_flags_t *ef, uint32_t flags, uint32_t options) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask;
#endif
  uint32_t event_flags;

  if ((options & osFlagsNoClear) == 0U) {
#if (__EXCLUSIVE_ACCESS == 0U)
    primask = __get_PRIMASK();
    __disable_irq();

    event_flags = ef->event_flags;
    if ((((options & osFlagsWaitAll) != 0U) && ((event_flags & flags) != flags)) ||
        (((options & osFlagsWaitAll) == 0U) && ((event_flags & flags) == 0U))) {
      event_flags = 0U;
    } else {
      ef->event_flags &= ~flags;
    }

    if (primask == 0U) {
      __enable_irq();
    }
#else
    if ((options & osFlagsWaitAll) != 0U) {
      event_flags = atomic_chk32_all(&ef->event_flags, flags);
    } else {
      event_flags = atomic_chk32_any(&ef->event_flags, flags);
    }
#endif
  } else {
    event_flags = ef->event_flags;
    if ((((options & osFlagsWaitAll) != 0U) && ((event_flags & flags) != flags)) ||
        (((options & osFlagsWaitAll) == 0U) && ((event_flags & flags) == 0U))) {
      event_flags = 0U;
    }
  }

  return event_flags;
}


//  ==== Library functions ====

/// Event Flags post ISR processing.
/// \param[in]  ef              event flags object.
void osRtxEventFlagsPostProcess (os_event_flags_t *ef) {
  os_thread_t *thread;
  os_thread_t *thread_next;
  uint32_t     event_flags;

  if (ef->state == osRtxObjectInactive) {
    return;
  }

  // Check if Threads are waiting for Event Flags
  thread = ef->thread_list;
  while (thread != NULL) {
    thread_next = thread->thread_next;
    event_flags = EventFlagsCheck(ef, thread->wait_flags, thread->flags_options);
    if (event_flags != 0U) {
      osRtxThreadListRemove(thread);
      osRtxThreadWaitExit(thread, event_flags, false);
      EvrRtxEventFlagsWaitCompleted(ef, thread->wait_flags, thread->flags_options, event_flags);
    }
    thread = thread_next;
  }
}


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_1M(EventFlagsNew,     osEventFlagsId_t, const osEventFlagsAttr_t *)
SVC0_1 (EventFlagsGetName, const char *,     osEventFlagsId_t)
SVC0_2 (EventFlagsSet,     uint32_t,         osEventFlagsId_t, uint32_t)
SVC0_2 (EventFlagsClear,   uint32_t,         osEventFlagsId_t, uint32_t)
SVC0_1 (EventFlagsGet,     uint32_t,         osEventFlagsId_t)
SVC0_4 (EventFlagsWait,    uint32_t,         osEventFlagsId_t, uint32_t, uint32_t, uint32_t)
SVC0_1 (EventFlagsDelete,  osStatus_t,       osEventFlagsId_t)

/// Create and Initialize an Event Flags object.
/// \note API identical to osEventFlagsNew
osEventFlagsId_t svcRtxEventFlagsNew (const osEventFlagsAttr_t *attr) {
  os_event_flags_t *ef;
  uint8_t           flags;
  const char       *name;

  // Process attributes
  if (attr != NULL) {
    name = attr->name;
    ef   = attr->cb_mem;
    if (ef != NULL) {
      if (((uint32_t)ef & 3U) || (attr->cb_size < sizeof(os_event_flags_t))) {
        EvrRtxEventFlagsError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    } else {
      if (attr->cb_size != 0U) {
        EvrRtxEventFlagsError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    }
  } else {
    name = NULL;
    ef   = NULL;
  }

  // Allocate object memory if not provided
  if (ef == NULL) {
    if (osRtxInfo.mpi.event_flags != NULL) {
      ef = osRtxMemoryPoolAlloc(osRtxInfo.mpi.event_flags);
    } else {
      ef = osRtxMemoryAlloc(osRtxInfo.mem.common, sizeof(os_event_flags_t), 1U);
    }
    if (ef == NULL) {
      EvrRtxEventFlagsError(NULL, osErrorNoMemory);
      return NULL;
    }
    flags = osRtxFlagSystemObject;
  } else {
    flags = 0U;
  }

  // Initialize control block
  ef->id          = osRtxIdEventFlags;
  ef->state       = osRtxObjectActive;
  ef->flags       = flags;
  ef->name        = name;
  ef->thread_list = NULL;
  ef->event_flags = 0U;

  // Register post ISR processing function
  osRtxInfo.post_process.event_flags = osRtxEventFlagsPostProcess;

  EvrRtxEventFlagsCreated(ef);

  return ef;
}

/// Get name of an Event Flags object.
/// \note API identical to osEventFlagsGetName
const char *svcRtxEventFlagsGetName (osEventFlagsId_t ef_id) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;

  // Check parameters
  if ((ef == NULL) || (ef->id != osRtxIdEventFlags)) {
    EvrRtxEventFlagsGetName(ef, NULL);
    return NULL;
  }

  // Check object state
  if (ef->state == osRtxObjectInactive) {
    EvrRtxEventFlagsGetName(ef, NULL);
    return NULL;
  }

  EvrRtxEventFlagsGetName(ef, ef->name);

  return ef->name;
}

/// Set the specified Event Flags.
/// \note API identical to osEventFlagsSet
uint32_t svcRtxEventFlagsSet (osEventFlagsId_t ef_id, uint32_t flags) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  os_thread_t      *thread;
  os_thread_t      *thread_next;
  uint32_t          event_flags;
  uint32_t          event_flags0;

  // Check parameters
  if ((ef == NULL) || (ef->id != osRtxIdEventFlags) ||
      (flags & ~((1U << osRtxEventFlagsLimit) - 1U))) {
    EvrRtxEventFlagsError(ef, osErrorParameter);
    return ((uint32_t)osErrorParameter);
  }

  // Check object state
  if (ef->state == osRtxObjectInactive) {
    EvrRtxEventFlagsError(ef, osErrorResource);
    return ((uint32_t)osErrorResource);
  }

  // Set Event Flags
  event_flags = EventFlagsSet(ef, flags);

  // Check if Threads are waiting for Event Flags
  thread = ef->thread_list;
  while (thread != NULL) {
    thread_next = thread->thread_next;
    event_flags0 = EventFlagsCheck(ef, thread->wait_flags, thread->flags_options);
    if (event_flags0 != 0U) {
      if ((thread->flags_options & osFlagsNoClear) == 0U) {
        event_flags = event_flags0 & ~thread->wait_flags;
      } else {
        event_flags = event_flags0;
      }
      osRtxThreadListRemove(thread);
      osRtxThreadWaitExit(thread, event_flags0, false);
      EvrRtxEventFlagsWaitCompleted(ef, thread->wait_flags, thread->flags_options, event_flags0);
    }
    thread = thread_next;
  }
  osRtxThreadDispatch(NULL);

  EvrRtxEventFlagsSetDone(ef, event_flags);

  return event_flags;
}

/// Clear the specified Event Flags.
/// \note API identical to osEventFlagsClear
uint32_t svcRtxEventFlagsClear (osEventFlagsId_t ef_id, uint32_t flags) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  uint32_t          event_flags;

  // Check parameters
  if ((ef == NULL) || (ef->id != osRtxIdEventFlags) ||
      (flags & ~((1U << osRtxEventFlagsLimit) - 1U))) {
    EvrRtxEventFlagsError(ef, osErrorParameter);
    return ((uint32_t)osErrorParameter);
  }

  // Check object state
  if (ef->state == osRtxObjectInactive) {
    EvrRtxEventFlagsError(ef, osErrorResource);
    return ((uint32_t)osErrorResource);
  }

  // Clear Event Flags
  event_flags = EventFlagsClear(ef, flags);

  EvrRtxEventFlagsClearDone(ef, event_flags);
  
  return event_flags;
}

/// Get the current Event Flags.
/// \note API identical to osEventFlagsGet
uint32_t svcRtxEventFlagsGet (osEventFlagsId_t ef_id) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;

  // Check parameters
  if ((ef == NULL) || (ef->id != osRtxIdEventFlags)) {
    EvrRtxEventFlagsGet(ef, 0U);
    return 0U;
  }

  // Check object state
  if (ef->state == osRtxObjectInactive) {
    EvrRtxEventFlagsGet(ef, 0U);
    return 0U;
  }

  EvrRtxEventFlagsGet(ef, ef->event_flags);

  return ef->event_flags;
}

/// Wait for one or more Event Flags to become signaled.
/// \note API identical to osEventFlagsWait
uint32_t svcRtxEventFlagsWait (osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  os_thread_t      *running_thread;
  uint32_t          event_flags;

  running_thread = osRtxThreadGetRunning();
  if (running_thread == NULL) {
    EvrRtxEventFlagsError(ef, osRtxErrorKernelNotRunning);
    return ((uint32_t)osError);
  }

  // Check parameters
  if ((ef == NULL) || (ef->id != osRtxIdEventFlags) ||
      (flags & ~((1U << osRtxEventFlagsLimit) - 1U))) {
    EvrRtxEventFlagsError(ef, osErrorParameter);
    return ((uint32_t)osErrorParameter);
  }

  // Check object state
  if (ef->state == osRtxObjectInactive) {
    EvrRtxEventFlagsError(ef, osErrorResource);
    return ((uint32_t)osErrorResource);
  }

  // Check Event Flags
  event_flags = EventFlagsCheck(ef, flags, options);
  if (event_flags != 0U) {
    EvrRtxEventFlagsWaitCompleted(ef, flags, options, event_flags);
    return event_flags;
  }

  // Check if timeout is specified
  if (timeout != 0U) {
    EvrRtxEventFlagsWaitPending(ef, flags, options, timeout);
    // Store waiting flags and options
    running_thread->wait_flags = flags;
    running_thread->flags_options = (uint8_t)options;
    // Suspend current Thread
    osRtxThreadListPut((os_object_t*)ef, running_thread);
    osRtxThreadWaitEnter(osRtxThreadWaitingEventFlags, timeout);
    return ((uint32_t)osErrorTimeout);
  }

  EvrRtxEventFlagsWaitNotCompleted(ef, flags, options);

  return ((uint32_t)osErrorResource);
}

/// Delete an Event Flags object.
/// \note API identical to osEventFlagsDelete
osStatus_t svcRtxEventFlagsDelete (osEventFlagsId_t ef_id) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  os_thread_t      *thread;

  // Check parameters
  if ((ef == NULL) || (ef->id != osRtxIdEventFlags)) {
    EvrRtxEventFlagsError(ef, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (ef->state == osRtxObjectInactive) {
    EvrRtxEventFlagsError(ef, osErrorResource);
    return osErrorResource;
  }

  // Mark object as inactive
  ef->state = osRtxObjectInactive;

  // Unblock waiting threads
  if (ef->thread_list != NULL) {
    do {
      thread = osRtxThreadListGet((os_object_t*)ef);
      osRtxThreadWaitExit(thread, (uint32_t)osErrorResource, false);
    } while (ef->thread_list != NULL);
    osRtxThreadDispatch(NULL);
  }

  // Free object memory
  if (ef->flags & osRtxFlagSystemObject) {
    if (osRtxInfo.mpi.event_flags != NULL) {
      osRtxMemoryPoolFree(osRtxInfo.mpi.event_flags, ef);
    } else {
      osRtxMemoryFree(osRtxInfo.mem.common, ef);
    }
  }

  EvrRtxEventFlagsDestroyed(ef);

  return osOK;
}


//  ==== ISR Calls ====

/// Set the specified Event Flags.
/// \note API identical to osEventFlagsSet
__STATIC_INLINE
uint32_t isrRtxEventFlagsSet (osEventFlagsId_t ef_id, uint32_t flags) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  uint32_t          event_flags;

  // Check parameters
  if ((ef == NULL) || (ef->id != osRtxIdEventFlags) ||
      (flags & ~((1U << osRtxEventFlagsLimit) - 1U))) {
    EvrRtxEventFlagsError(ef, osErrorParameter);
    return ((uint32_t)osErrorParameter);
  }

  // Check object state
  if (ef->state == osRtxObjectInactive) {
    EvrRtxEventFlagsError(ef, osErrorResource);
    return ((uint32_t)osErrorResource);
  }

  // Set Event Flags
  event_flags = EventFlagsSet(ef, flags);

  // Register post ISR processing
  osRtxPostProcess((os_object_t *)ef);

  EvrRtxEventFlagsSetDone(ef, event_flags);

  return event_flags;
}

/// Wait for one or more Event Flags to become signaled.
/// \note API identical to osEventFlagsWait
__STATIC_INLINE
uint32_t isrRtxEventFlagsWait (osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  uint32_t          event_flags;

  // Check parameters
  if ((ef == NULL) || (ef->id != osRtxIdEventFlags) || (timeout != 0U) ||
      (flags & ~((1U << osRtxEventFlagsLimit) - 1U))) {
    EvrRtxEventFlagsError(ef, osErrorParameter);
    return ((uint32_t)osErrorParameter);
  }

  // Check object state
  if (ef->state == osRtxObjectInactive) {
    EvrRtxEventFlagsError(ef, osErrorResource);
    return ((uint32_t)osErrorResource);
  }

  // Check Event Flags
  event_flags = EventFlagsCheck(ef, flags, options);
  if (event_flags != 0U) {
    EvrRtxEventFlagsWaitCompleted(ef, flags, options, event_flags);
    return ((uint32_t)event_flags);
  }

  EvrRtxEventFlagsWaitNotCompleted(ef, flags, options);

  return ((uint32_t)osErrorResource);
}


//  ==== Public API ====

/// Create and Initialize an Event Flags object.
osEventFlagsId_t osEventFlagsNew (const osEventFlagsAttr_t *attr) {
  EvrRtxEventFlagsNew(attr);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxEventFlagsError(NULL, osErrorISR);
    return NULL;
  }
  return __svcEventFlagsNew(attr);
}

/// Get name of an Event Flags object.
const char *osEventFlagsGetName (osEventFlagsId_t ef_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxEventFlagsGetName(ef_id, NULL);
    return NULL;
  }
  return __svcEventFlagsGetName(ef_id);
}

/// Set the specified Event Flags.
uint32_t osEventFlagsSet (osEventFlagsId_t ef_id, uint32_t flags) {
  EvrRtxEventFlagsSet(ef_id, flags);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return isrRtxEventFlagsSet(ef_id, flags);
  } else {
    return  __svcEventFlagsSet(ef_id, flags);
  }
}

/// Clear the specified Event Flags.
uint32_t osEventFlagsClear (osEventFlagsId_t ef_id, uint32_t flags) {
  EvrRtxEventFlagsClear(ef_id, flags);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxEventFlagsClear(ef_id, flags);
  } else {
    return  __svcEventFlagsClear(ef_id, flags);
  }
}

/// Get the current Event Flags.
uint32_t osEventFlagsGet (osEventFlagsId_t ef_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxEventFlagsGet(ef_id);
  } else {
    return  __svcEventFlagsGet(ef_id);
  }
}

/// Wait for one or more Event Flags to become signaled.
uint32_t osEventFlagsWait (osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout) {
  EvrRtxEventFlagsWait(ef_id, flags, options, timeout);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return isrRtxEventFlagsWait(ef_id, flags, options, timeout);
  } else {
    return  __svcEventFlagsWait(ef_id, flags, options, timeout);
  }
}

/// Delete an Event Flags object.
osStatus_t osEventFlagsDelete (osEventFlagsId_t ef_id) {
  EvrRtxEventFlagsDelete(ef_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxEventFlagsError(ef_id, osErrorISR);
    return osErrorISR;
  }
  return __svcEventFlagsDelete(ef_id);
}
