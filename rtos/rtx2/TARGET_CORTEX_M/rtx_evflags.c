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
static int32_t os_EventFlagsSet (os_event_flags_t *ef, int32_t flags) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  int32_t  event_flags;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  ef->event_flags |= flags;
  event_flags = ef->event_flags;

  if (primask == 0U) {
    __enable_irq();
  }
#else
  event_flags = (int32_t)os_exc_set32((uint32_t *)&ef->event_flags, (uint32_t)flags);
#endif

  return event_flags;
}

/// Clear Event Flags.
/// \param[in]  ef              event flags object.
/// \param[in]  flags           specifies the flags to clear.
/// \return event flags before clearing.
static int32_t os_EventFlagsClear (os_event_flags_t *ef, int32_t flags) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  int32_t  event_flags;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  event_flags = ef->event_flags;
  ef->event_flags &= ~flags;

  if (primask == 0U) {
    __enable_irq();
  }
#else
  event_flags = (int32_t)os_exc_clr32((uint32_t *)&ef->event_flags, (uint32_t)flags);
#endif

  return event_flags;
}

/// Check Event Flags.
/// \param[in]  ef              event flags object.
/// \param[in]  flags           specifies the flags to check.
/// \param[in]  options         specifies flags options (osFlagsXxxx).
/// \return event flags before clearing or 0 if specified flags have not been set.
static int32_t os_EventFlagsCheck (os_event_flags_t *ef, int32_t flags, uint32_t options) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask;
#endif
  int32_t  event_flags;

  if ((options & osFlagsNoClear) == 0U) {
#if (__EXCLUSIVE_ACCESS == 0U)
    primask = __get_PRIMASK();
    __disable_irq();

    event_flags = ef->event_flags;
    if ((((options & osFlagsWaitAll) != 0U) && ((event_flags & flags) != flags)) ||
        (((options & osFlagsWaitAll) == 0U) && ((event_flags & flags) == 0))) {
      event_flags = 0;
    } else {
      ef->event_flags &= ~flags;
    }

    if (primask == 0U) {
      __enable_irq();
    }
#else
    if ((options & osFlagsWaitAll) != 0U) {
      event_flags = (int32_t)os_exc_chk32_all((uint32_t *)&ef->event_flags, (uint32_t)flags);
    } else {
      event_flags = (int32_t)os_exc_chk32_any((uint32_t *)&ef->event_flags, (uint32_t)flags);
    }
#endif
  } else {
    event_flags = ef->event_flags;
    if ((((options & osFlagsWaitAll) != 0U) && ((event_flags & flags) != flags)) ||
        (((options & osFlagsWaitAll) == 0U) && ((event_flags & flags) == 0))) {
      event_flags = 0;
    }
  }

  return event_flags;
}


//  ==== Library functions ====

/// Event Flags post ISR processing.
/// \param[in]  ef              event flags object.
void os_EventFlagsPostProcess (os_event_flags_t *ef) {
  os_thread_t *thread;
  os_thread_t *thread_next;
  int32_t      event_flags;

  if (ef->state == os_ObjectInactive) {
    return;
  }

  // Check if Threads are waiting for Event Flags
  thread = ef->thread_list;
  while (thread != NULL) {
    thread_next = thread->thread_next;
    event_flags = os_EventFlagsCheck(ef, thread->wait_flags, thread->flags_options);
    if (event_flags > 0) {
      os_ThreadListRemove(thread);
      os_ThreadWaitExit(thread, (uint32_t)event_flags, false);
    }
    thread = thread_next;
  }
}


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_1(EventFlagsNew,     osEventFlagsId_t, const osEventFlagsAttr_t *)
SVC0_1(EventFlagsGetName, const char *,     osEventFlagsId_t)
SVC0_2(EventFlagsSet,     int32_t,          osEventFlagsId_t, int32_t)
SVC0_2(EventFlagsClear,   int32_t,          osEventFlagsId_t, int32_t)
SVC0_1(EventFlagsGet,     int32_t,          osEventFlagsId_t)
SVC0_4(EventFlagsWait,    int32_t,          osEventFlagsId_t, int32_t, uint32_t, uint32_t)
SVC0_1(EventFlagsDelete,  osStatus_t,       osEventFlagsId_t)

/// Create and Initialize an Event Flags object.
/// \note API identical to osEventFlagsNew
osEventFlagsId_t os_svcEventFlagsNew (const osEventFlagsAttr_t *attr) {
  os_event_flags_t *ef;
  uint8_t           flags;
  const char       *name;

  // Process attributes
  if (attr != NULL) {
    name = attr->name;
    ef   = attr->cb_mem;
    if (ef != NULL) {
      if (((uint32_t)ef & 3U) || (attr->cb_size < sizeof(os_event_flags_t))) {
        return NULL;
      }
    } else {
      if (attr->cb_size != 0U) {
        return NULL;
      }
    }
  } else {
    name = NULL;
    ef   = NULL;
  }

  // Allocate object memory if not provided
  if (ef == NULL) {
    if (os_Info.mpi.event_flags != NULL) {
      ef = os_MemoryPoolAlloc(os_Info.mpi.event_flags);
    } else {
      ef = os_MemoryAlloc(os_Info.mem.common, sizeof(os_event_flags_t), 1U);
    }
    if (ef == NULL) {
      return NULL;
    }
    flags = os_FlagSystemObject;
  } else {
    flags = 0U;
  }

  // Initialize control block
  ef->id          = os_IdEventFlags;
  ef->state       = os_ObjectActive;
  ef->flags       = flags;
  ef->name        = name;
  ef->thread_list = NULL;
  ef->event_flags = 0;

  // Register post ISR processing function
  os_Info.post_process.event_flags = os_EventFlagsPostProcess;

  return ef;
}

/// Get name of an Event Flags object.
/// \note API identical to osEventFlagsGetName
const char *os_svcEventFlagsGetName (osEventFlagsId_t ef_id) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;

  // Check parameters
  if ((ef == NULL) ||
      (ef->id != os_IdEventFlags)) {
    return NULL;
  }

  // Check object state
  if (ef->state == os_ObjectInactive) {
    return NULL;
  }

  return ef->name;
}

/// Set the specified Event Flags.
/// \note API identical to osEventFlagsSet
int32_t os_svcEventFlagsSet (osEventFlagsId_t ef_id, int32_t flags) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  os_thread_t      *thread;
  os_thread_t      *thread_next;
  int32_t           event_flags;
  int32_t           event_flags0;

  // Check parameters
  if ((ef == NULL) ||
      (ef->id != os_IdEventFlags)) {
    return osErrorParameter;
  }
  if ((uint32_t)flags & ~((1U << os_EventFlagsLimit) - 1U)) {
    return osErrorParameter;
  }

  // Check object state
  if (ef->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Set Event Flags
  event_flags = os_EventFlagsSet(ef, flags);

  // Check if Threads are waiting for Event Flags
  thread = ef->thread_list;
  while (thread != NULL) {
    thread_next = thread->thread_next;
    event_flags0 = os_EventFlagsCheck(ef, thread->wait_flags, thread->flags_options);
    if (event_flags0 > 0) {
      if ((thread->flags_options & osFlagsNoClear) == 0U) {
        event_flags = event_flags0 & ~thread->wait_flags;
      } else {
        event_flags = event_flags0;
      }
      os_ThreadListRemove(thread);
      os_ThreadWaitExit(thread, (uint32_t)event_flags0, false);
    }
    thread = thread_next;
  }
  os_ThreadDispatch(NULL);

  return event_flags;
}

/// Clear the specified Event Flags.
/// \note API identical to osEventFlagsClear
int32_t os_svcEventFlagsClear (osEventFlagsId_t ef_id, int32_t flags) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;

  // Check parameters
  if ((ef == NULL) ||
      (ef->id != os_IdEventFlags)) {
    return osErrorParameter;
  }
  if ((uint32_t)flags & ~((1U << os_EventFlagsLimit) - 1U)) {
    return osErrorParameter;
  }

  // Check object state
  if (ef->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Clear Event Flags
  return os_EventFlagsClear(ef, flags);
}

/// Get the current Event Flags.
/// \note API identical to osEventFlagsGet
int32_t os_svcEventFlagsGet (osEventFlagsId_t ef_id) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;

  // Check parameters
  if ((ef == NULL) ||
      (ef->id != os_IdEventFlags)) {
    return 0;
  }

  // Check object state
  if (ef->state == os_ObjectInactive) {
    return 0;
  }

  return ef->event_flags;
}

/// Wait for one or more Event Flags to become signaled.
/// \note API identical to osEventFlagsWait
int32_t os_svcEventFlagsWait (osEventFlagsId_t ef_id, int32_t flags, uint32_t options, uint32_t timeout) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  os_thread_t      *running_thread;
  int32_t           event_flags;

  running_thread = os_ThreadGetRunning();
  if (running_thread == NULL) {
    return osError;
  }

  // Check parameters
  if ((ef == NULL) ||
      (ef->id != os_IdEventFlags)) {
    return osErrorParameter;
  }
  if ((uint32_t)flags & ~((1U << os_EventFlagsLimit) - 1U)) {
    return osErrorParameter;
  }

  // Check object state
  if (ef->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Check Event Flags
  event_flags = os_EventFlagsCheck(ef, flags, options);
  if (event_flags > 0) {
    return event_flags;
  }

  // Check if timeout is specified
  if (timeout != 0U) {
    // Store waiting flags and options
    running_thread->wait_flags = flags;
    running_thread->flags_options = (uint8_t)options;
    // Suspend current Thread
    os_ThreadListPut((os_object_t*)ef, running_thread);
    os_ThreadWaitEnter(os_ThreadWaitingEventFlags, timeout);
    return osErrorTimeout;
  }

  return osErrorResource;
}

/// Delete an Event Flags object.
/// \note API identical to osEventFlagsDelete
osStatus_t os_svcEventFlagsDelete (osEventFlagsId_t ef_id) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  os_thread_t      *thread;

  // Check parameters
  if ((ef == NULL) ||
      (ef->id != os_IdEventFlags)) {
    return osErrorParameter;
  }

  // Check object state
  if (ef->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Mark object as inactive
  ef->state = os_ObjectInactive;

  // Unblock waiting threads
  if (ef->thread_list != NULL) {
    do {
      thread = os_ThreadListGet((os_object_t*)ef);
      os_ThreadWaitExit(thread, (uint32_t)osErrorResource, false);
    } while (ef->thread_list != NULL);
    os_ThreadDispatch(NULL);
  }

  // Free object memory
  if (ef->flags & os_FlagSystemObject) {
    if (os_Info.mpi.event_flags != NULL) {
      os_MemoryPoolFree(os_Info.mpi.event_flags, ef);
    } else {
      os_MemoryFree(os_Info.mem.common, ef);
    }
  }

  return osOK;
}


//  ==== ISR Calls ====

/// Set the specified Event Flags.
/// \note API identical to osEventFlagsSet
__STATIC_INLINE
int32_t os_isrEventFlagsSet (osEventFlagsId_t ef_id, int32_t flags) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  int32_t           event_flags;

  // Check parameters
  if ((ef == NULL) ||
      (ef->id != os_IdEventFlags)) {
    return osErrorParameter;
  }
  if ((uint32_t)flags & ~((1U << os_EventFlagsLimit) - 1U)) {
    return osErrorParameter;
  }

  // Check object state
  if (ef->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Set Event Flags
  event_flags = os_EventFlagsSet(ef, flags);

  // Register post ISR processing
  os_PostProcess((os_object_t *)ef);

  return event_flags;
}

/// Wait for one or more Event Flags to become signaled.
/// \note API identical to osEventFlagsWait
__STATIC_INLINE
int32_t os_isrEventFlagsWait (osEventFlagsId_t ef_id, int32_t flags, uint32_t options, uint32_t timeout) {
  os_event_flags_t *ef = (os_event_flags_t *)ef_id;
  int32_t           event_flags;

  // Check parameters
  if ((ef == NULL) ||
      (ef->id != os_IdEventFlags)) {
    return osErrorParameter;
  }
  if ((uint32_t)flags & ~((1U << os_EventFlagsLimit) - 1U)) {
    return osErrorParameter;
  }
  if (timeout != 0U) {
    return osErrorParameter;
  }

  // Check object state
  if (ef->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Check Event Flags
  event_flags = os_EventFlagsCheck(ef, flags, options);
  if (event_flags > 0) {
    return event_flags;
  }

  return osErrorResource;
}


//  ==== Public API ====

/// Create and Initialize an Event Flags object.
osEventFlagsId_t osEventFlagsNew (const osEventFlagsAttr_t *attr) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return NULL;
  }
  if ((os_KernelGetState() == os_KernelReady) && IS_PRIVILEGED()) {
    // Kernel Ready (not running) and in Privileged mode
    return os_svcEventFlagsNew(attr);
  } else {
    return  __svcEventFlagsNew(attr);
  }
}

/// Get name of an Event Flags object.
const char *osEventFlagsGetName (osEventFlagsId_t ef_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return NULL;
  }
  return  __svcEventFlagsGetName(ef_id);
}

/// Set the specified Event Flags.
int32_t osEventFlagsSet (osEventFlagsId_t ef_id, int32_t flags) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return os_isrEventFlagsSet(ef_id, flags);
  } else {
    return  __svcEventFlagsSet(ef_id, flags);
  }
}

/// Clear the specified Event Flags.
int32_t osEventFlagsClear (osEventFlagsId_t ef_id, int32_t flags) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return os_svcEventFlagsClear(ef_id, flags);
  } else {
    return  __svcEventFlagsClear(ef_id, flags);
  }
}

/// Get the current Event Flags.
int32_t osEventFlagsGet (osEventFlagsId_t ef_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return os_svcEventFlagsGet(ef_id);
  } else {
    return  __svcEventFlagsGet(ef_id);
  }
}

/// Wait for one or more Event Flags to become signaled.
int32_t osEventFlagsWait (osEventFlagsId_t ef_id, int32_t flags, uint32_t options, uint32_t timeout) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return os_isrEventFlagsWait(ef_id, flags, options, timeout);
  } else {
    return  __svcEventFlagsWait(ef_id, flags, options, timeout);
  }
}

/// Delete an Event Flags object.
osStatus_t osEventFlagsDelete (osEventFlagsId_t ef_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return __svcEventFlagsDelete(ef_id);
}
