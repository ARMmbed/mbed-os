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
 * Title:       Mutex functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Library functions ====

/// Release Mutex list when owner Thread terminates.
/// \param[in]  mutex           mutex object.
/// \return 1 - success, 0 - failure.
void osRtxMutexOwnerRelease (os_mutex_t *mutex_list) {
  os_mutex_t  *mutex;
  os_thread_t *thread;

  mutex = mutex_list;
  while (mutex) {
    mutex_list = mutex->owner_next;
    // Check if Mutex is Robust
    if (mutex->attr & osMutexRobust) {
      // Clear Lock counter
      mutex->lock = 0U;
      EvrRtxMutexReleased(mutex, 0U);
      // Check if Thread is waiting for a Mutex
      if (mutex->thread_list != NULL) {
        // Wakeup waiting Thread with highest Priority
        thread = osRtxThreadListGet((os_object_t*)mutex);
        osRtxThreadWaitExit(thread, (uint32_t)osOK, false);
        // Thread is the new Mutex owner
        mutex->owner_thread = thread;
        mutex->owner_next   = thread->mutex_list;
        mutex->owner_prev   = NULL;
        thread->mutex_list  = mutex;
        mutex->lock = 1U;
        EvrRtxMutexAcquired(mutex, 1U);
      }
    }
    mutex = mutex_list;
  }
}


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_1M(MutexNew,      osMutexId_t,  const osMutexAttr_t *)
SVC0_1 (MutexGetName,  const char *, osMutexId_t)
SVC0_2 (MutexAcquire,  osStatus_t,   osMutexId_t, uint32_t)
SVC0_1 (MutexRelease,  osStatus_t,   osMutexId_t)
SVC0_1 (MutexGetOwner, osThreadId_t, osMutexId_t)
SVC0_1 (MutexDelete,   osStatus_t,   osMutexId_t)

/// Create and Initialize a Mutex object.
/// \note API identical to osMutexNew
osMutexId_t svcRtxMutexNew (const osMutexAttr_t *attr) {
  os_mutex_t *mutex;
  uint32_t    attr_bits;
  uint8_t     flags;
  const char *name;

  // Process attributes
  if (attr != NULL) {
    name      = attr->name;
    attr_bits = attr->attr_bits;
    mutex     = attr->cb_mem;
    if (mutex != NULL) {
      if (((uint32_t)mutex & 3U) || (attr->cb_size < sizeof(os_mutex_t))) {
        EvrRtxMutexError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    } else {
      if (attr->cb_size != 0U) {
        EvrRtxMutexError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    }
  } else {
    name      = NULL;
    attr_bits = 0U;
    mutex     = NULL;
  }

  // Allocate object memory if not provided
  if (mutex == NULL) {
    if (osRtxInfo.mpi.mutex != NULL) {
      mutex = osRtxMemoryPoolAlloc(osRtxInfo.mpi.mutex);
    } else {
      mutex = osRtxMemoryAlloc(osRtxInfo.mem.common, sizeof(os_mutex_t), 1U);
    }
    if (mutex == NULL) {
      EvrRtxMutexError(NULL, osErrorNoMemory);
      return NULL;
    }
    flags = osRtxFlagSystemObject;
  } else {
    flags = 0U;
  }

  // Initialize control block
  mutex->id           = osRtxIdMutex;
  mutex->state        = osRtxObjectActive;
  mutex->flags        = flags;
  mutex->attr         = (uint8_t)attr_bits;
  mutex->name         = name;
  mutex->thread_list  = NULL;
  mutex->owner_thread = NULL;
  mutex->owner_prev   = NULL;
  mutex->owner_next   = NULL;
  mutex->lock         = 0U;

  EvrRtxMutexCreated(mutex);

  return mutex;
}

/// Get name of a Mutex object.
/// \note API identical to osMutexGetName
const char *svcRtxMutexGetName (osMutexId_t mutex_id) {
  os_mutex_t *mutex = (os_mutex_t *)mutex_id;

  // Check parameters
  if ((mutex == NULL) || (mutex->id != osRtxIdMutex)) {
    EvrRtxMutexGetName(mutex, NULL);
    return NULL;
  }

  // Check object state
  if (mutex->state == osRtxObjectInactive) {
    EvrRtxMutexGetName(mutex, NULL);
    return NULL;
  }

  EvrRtxMutexGetName(mutex, mutex->name);

  return mutex->name;
}

/// Acquire a Mutex or timeout if it is locked.
/// \note API identical to osMutexAcquire
osStatus_t svcRtxMutexAcquire (osMutexId_t mutex_id, uint32_t timeout) {
  os_mutex_t  *mutex = (os_mutex_t *)mutex_id;
  os_thread_t *runnig_thread;

  runnig_thread = osRtxThreadGetRunning();
  if (runnig_thread == NULL) {
    EvrRtxMutexError(mutex, osRtxErrorKernelNotRunning);
    return osError;
  }

  // Check parameters
  if ((mutex == NULL) || (mutex->id != osRtxIdMutex)) {
    EvrRtxMutexError(mutex, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mutex->state == osRtxObjectInactive) {
    EvrRtxMutexError(mutex, osErrorResource);
    return osErrorResource;
  }

  // Check if Mutex is not locked
  if (mutex->lock == 0U) {
    // Acquire Mutex
    mutex->owner_thread = runnig_thread;
    mutex->owner_next   = runnig_thread->mutex_list;
    mutex->owner_prev   = NULL;
    runnig_thread->mutex_list = mutex;
    mutex->lock = 1U;
    EvrRtxMutexAcquired(mutex, mutex->lock);
    return osOK;
  }

  // Check if Mutex is recursive and running Thread is the owner
  if ((mutex->attr & osMutexRecursive) && (mutex->owner_thread == runnig_thread)) {
    // Increment lock counter
    if (mutex->lock == osRtxMutexLockLimit) {
      EvrRtxMutexError(mutex, osRtxErrorMutexLockLimit);
      return osErrorResource;
    }
    mutex->lock++;
    EvrRtxMutexAcquired(mutex, mutex->lock);
    return osOK;
  }

  // Check if timeout is specified
  if (timeout != 0U) {
    // Check if Priority inheritance protocol is enabled
    if (mutex->attr & osMutexPrioInherit) {
      // Raise priority of owner Thread if lower than priority of running Thread
      if (mutex->owner_thread->priority < runnig_thread->priority) {
        mutex->owner_thread->priority = runnig_thread->priority;
        osRtxThreadListSort(mutex->owner_thread);
      }
    }
    EvrRtxMutexAcquirePending(mutex, timeout);
    // Suspend current Thread
    osRtxThreadListPut((os_object_t*)mutex, runnig_thread);
    osRtxThreadWaitEnter(osRtxThreadWaitingMutex, timeout);
    return osErrorTimeout;
  }

  // Mutex was not acquired
  EvrRtxMutexNotAcquired(mutex);

  return osErrorResource;
}

/// Release a Mutex that was acquired by osMutexAcquire.
/// \note API identical to osMutexRelease
osStatus_t svcRtxMutexRelease (osMutexId_t mutex_id) {
  os_mutex_t  *mutex = (os_mutex_t *)mutex_id;
  os_mutex_t  *mutex0;
  os_thread_t *thread;
  os_thread_t *runnig_thread;
  int8_t       priority;

  runnig_thread = osRtxThreadGetRunning();
  if (runnig_thread == NULL) {
    EvrRtxMutexError(mutex, osRtxErrorKernelNotRunning);
    return osError;
  }

  // Check parameters
  if ((mutex == NULL) || (mutex->id != osRtxIdMutex)) {
    EvrRtxMutexError(mutex, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mutex->state == osRtxObjectInactive) {
    EvrRtxMutexError(mutex, osErrorResource);
    return osErrorResource;
  }

  // Check if running Thread is not the owner
  if (mutex->owner_thread != runnig_thread) {
    EvrRtxMutexError(mutex, osRtxErrorMutexNotOwned);
    return osErrorResource;
  }

  // Check if Mutex is not locked
  if (mutex->lock == 0U) {
    EvrRtxMutexError(mutex, osRtxErrorMutexNotLocked);
    return osErrorResource;
  }

  // Decrement Lock counter
  mutex->lock--;
  EvrRtxMutexReleased(mutex, mutex->lock);

  // Check Lock counter
  if (mutex->lock != 0U) {
    return osOK;
  }

  // Remove Mutex from Thread owner list
  if (mutex->owner_next != NULL) {
    mutex->owner_next->owner_prev = mutex->owner_prev;
  }
  if (mutex->owner_prev != NULL) {
    mutex->owner_prev->owner_next = mutex->owner_next;
  } else {
    runnig_thread->mutex_list = mutex->owner_next;
  }

  // Restore running Thread priority
  if (mutex->attr & osMutexPrioInherit) {
    priority = runnig_thread->priority_base;
    mutex0   = runnig_thread->mutex_list;
    while (mutex0) {
      // Mutexes owned by running Thread
      if ((mutex0->thread_list != NULL) && (mutex0->thread_list->priority > priority)) {
        // Higher priority Thread is waiting for Mutex
        priority = mutex0->thread_list->priority;
      }
      mutex0 = mutex0->owner_next;
    }
    runnig_thread->priority = priority;
  }

  // Check if Thread is waiting for a Mutex
  if (mutex->thread_list != NULL) {
    // Wakeup waiting Thread with highest Priority
    thread = osRtxThreadListGet((os_object_t*)mutex);
    osRtxThreadWaitExit(thread, (uint32_t)osOK, false);
    // Thread is the new Mutex owner
    mutex->owner_thread = thread;
    mutex->owner_next   = thread->mutex_list;
    mutex->owner_prev   = NULL;
    thread->mutex_list  = mutex;
    mutex->lock = 1U;
    EvrRtxMutexAcquired(mutex, 1U);
  }

  osRtxThreadDispatch(NULL);

  return osOK;
}

/// Get Thread which owns a Mutex object.
/// \note API identical to osMutexGetOwner
osThreadId_t svcRtxMutexGetOwner (osMutexId_t mutex_id) {
  os_mutex_t *mutex = (os_mutex_t *)mutex_id;

  // Check parameters
  if ((mutex == NULL) || (mutex->id != osRtxIdMutex)) {
    EvrRtxMutexGetOwner(mutex, NULL);
    return NULL;
  }

  // Check object state
  if (mutex->state == osRtxObjectInactive) {
    EvrRtxMutexGetOwner(mutex, NULL);
    return NULL;
  }

  // Check if Mutex is not locked
  if (mutex->lock == 0U) {
    EvrRtxMutexGetOwner(mutex, NULL);
    return NULL;
  }

  EvrRtxMutexGetOwner(mutex, mutex->owner_thread);

  return mutex->owner_thread;
}

/// Delete a Mutex object.
/// \note API identical to osMutexDelete
osStatus_t svcRtxMutexDelete (osMutexId_t mutex_id) {
  os_mutex_t  *mutex = (os_mutex_t *)mutex_id;
  os_mutex_t  *mutex0;
  os_thread_t *thread;
  int8_t       priority;

  // Check parameters
  if ((mutex == NULL) || (mutex->id != osRtxIdMutex)) {
    EvrRtxMutexError(mutex, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mutex->state == osRtxObjectInactive) {
    EvrRtxMutexError(mutex, osErrorResource);
    return osErrorResource;
  }

  // Mark object as inactive
  mutex->state = osRtxObjectInactive;

  // Check if Mutex is locked
  if (mutex->lock != 0U) {

    thread = mutex->owner_thread;

    // Remove Mutex from Thread owner list
    if (mutex->owner_next != NULL) {
      mutex->owner_next->owner_prev = mutex->owner_prev;
    }
    if (mutex->owner_prev != NULL) {
      mutex->owner_prev->owner_next = mutex->owner_next;
    } else {
      thread->mutex_list = mutex->owner_next;
    }

    // Restore owner Thread priority
    if (mutex->attr & osMutexPrioInherit) {
      priority = thread->priority_base;
      mutex0   = thread->mutex_list;
      while (mutex0) {
        // Mutexes owned by running Thread
        if ((mutex0->thread_list != NULL) && (mutex0->thread_list->priority > priority)) {
          // Higher priority Thread is waiting for Mutex
          priority = mutex0->thread_list->priority;
        }
        mutex0 = mutex0->owner_next;
      }
      if (thread->priority != priority) {
        thread->priority = priority;
        osRtxThreadListSort(thread);
      }
    }

    // Unblock waiting threads
    if (mutex->thread_list != NULL) {
      do {
        thread = osRtxThreadListGet((os_object_t*)mutex);
        osRtxThreadWaitExit(thread, (uint32_t)osErrorResource, false);
      } while (mutex->thread_list != NULL);
    }

    osRtxThreadDispatch(NULL);
  }

  // Free object memory
  if (mutex->flags & osRtxFlagSystemObject) {
    if (osRtxInfo.mpi.mutex != NULL) {
      osRtxMemoryPoolFree(osRtxInfo.mpi.mutex, mutex);
    } else {
      osRtxMemoryFree(osRtxInfo.mem.common, mutex);
    }
  }

  EvrRtxMutexDestroyed(mutex);

  return osOK;
}


//  ==== Public API ====

/// Create and Initialize a Mutex object.
osMutexId_t osMutexNew (const osMutexAttr_t *attr) {
  EvrRtxMutexNew(attr);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMutexError(NULL, osErrorISR);
    return NULL;
  }
  return __svcMutexNew(attr);
}

/// Get name of a Mutex object.
const char *osMutexGetName (osMutexId_t mutex_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMutexGetName(mutex_id, NULL);
    return NULL;
  }
  return __svcMutexGetName(mutex_id);
}

/// Acquire a Mutex or timeout if it is locked.
osStatus_t osMutexAcquire (osMutexId_t mutex_id, uint32_t timeout) {
  EvrRtxMutexAcquire(mutex_id, timeout);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMutexError(mutex_id, osErrorISR);
    return osErrorISR;
  }
  return __svcMutexAcquire(mutex_id, timeout);
}

/// Release a Mutex that was acquired by \ref osMutexAcquire.
osStatus_t osMutexRelease (osMutexId_t mutex_id) {
  EvrRtxMutexRelease(mutex_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMutexError(mutex_id, osErrorISR);
    return osErrorISR;
  }
  return __svcMutexRelease(mutex_id);
}

/// Get Thread which owns a Mutex object.
osThreadId_t osMutexGetOwner (osMutexId_t mutex_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMutexGetOwner(mutex_id, NULL);
    return NULL;
  }
  return __svcMutexGetOwner(mutex_id);
}

/// Delete a Mutex object.
osStatus_t osMutexDelete (osMutexId_t mutex_id) {
  EvrRtxMutexDelete(mutex_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMutexError(mutex_id, osErrorISR);
    return osErrorISR;
  }
  return __svcMutexDelete(mutex_id);
}
