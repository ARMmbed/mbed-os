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
 * Title:       Semaphore functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Helper functions ====

/// Decrement Semaphore tokens.
/// \param[in]  semaphore       semaphore object.
/// \return 1 - success, 0 - failure.
static uint32_t os_SemaphoreTokenDecrement (os_semaphore_t *semaphore) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  uint32_t ret;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  if (semaphore->tokens != 0U) {
    semaphore->tokens--;
    ret = 1U;
  } else {
    ret = 0U;
  }

  if (primask == 0U) {
    __enable_irq();
  }
#else
  if (os_exc_dec16_nz(&semaphore->tokens) != 0U) {
    ret = 1U;
  } else {
    ret = 0U;
  }
#endif

  return ret;
}

/// Increment Semaphore tokens.
/// \param[in]  semaphore       semaphore object.
/// \return 1 - success, 0 - failure.
static uint32_t os_SemaphoreTokenIncrement (os_semaphore_t *semaphore) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  uint32_t ret;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  if (semaphore->tokens < semaphore->max_tokens) {
    semaphore->tokens++;
    ret = 1U;
  } else {
    ret = 0U;
  }

  if (primask == 0U) {
    __enable_irq();
  }
#else
  if (os_exc_inc16_lt(&semaphore->tokens, semaphore->max_tokens) < semaphore->max_tokens) {
    ret = 1U;
  } else {
    ret = 0U;
  }
#endif

  return ret;
}


//  ==== Library functions ====

/// Semaphore post ISR processing.
/// \param[in]  semaphore       semaphore object.
void os_SemaphorePostProcess (os_semaphore_t *semaphore) {
  os_thread_t *thread;

  if (semaphore->state == os_ObjectInactive) {
    return;
  }

  // Check if Thread is waiting for a token
  if (semaphore->thread_list != NULL) {
    // Try to acquire token
    if (os_SemaphoreTokenDecrement(semaphore) != 0U) {
      // Wakeup waiting Thread with highest Priority
      thread = os_ThreadListGet((os_object_t*)semaphore);
      os_ThreadWaitExit(thread, (uint32_t)osOK, false);
    }
  }
}


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_3(SemaphoreNew,      osSemaphoreId_t, uint32_t, uint32_t, const osSemaphoreAttr_t *)
SVC0_1(SemaphoreGetName,  const char *,    osSemaphoreId_t)
SVC0_2(SemaphoreAcquire,  osStatus_t,      osSemaphoreId_t, uint32_t)
SVC0_1(SemaphoreRelease,  osStatus_t,      osSemaphoreId_t)
SVC0_1(SemaphoreGetCount, uint32_t,        osSemaphoreId_t)
SVC0_1(SemaphoreDelete,   osStatus_t,      osSemaphoreId_t)

/// Create and Initialize a Semaphore object.
/// \note API identical to osSemaphoreNew
osSemaphoreId_t os_svcSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr) {
  os_semaphore_t *semaphore;
  uint8_t         flags;
  const char     *name;

  // Check parameters
  if ((max_count    == 0U)                     ||
      (max_count     > os_SemaphoreTokenLimit) ||
      (initial_count > max_count)) {
    return NULL;
  }

  // Process attributes
  if (attr != NULL) {
    name      = attr->name;
    semaphore = attr->cb_mem;
    if (semaphore != NULL) {
      if (((uint32_t)semaphore & 3U) || (attr->cb_size < sizeof(os_semaphore_t))) {
        return NULL;
      }
    } else {
      if (attr->cb_size != 0U) {
        return NULL;
      }
    }
  } else {
    name      = NULL;
    semaphore = NULL;
  }

  // Allocate object memory if not provided
  if (semaphore == NULL) {
    if (os_Info.mpi.semaphore != NULL) {
      semaphore = os_MemoryPoolAlloc(os_Info.mpi.semaphore);
    } else {
      semaphore = os_MemoryAlloc(os_Info.mem.common, sizeof(os_semaphore_t), 1U);
    }
    if (semaphore == NULL) {
      return NULL;
    }
    flags = os_FlagSystemObject;
  } else {
    flags = 0U;
  }

  // Initialize control block
  semaphore->id          = os_IdSemaphore;
  semaphore->state       = os_ObjectActive;
  semaphore->flags       = flags;
  semaphore->name        = name;
  semaphore->thread_list = NULL;
  semaphore->tokens      = (uint16_t)initial_count;
  semaphore->max_tokens  = (uint16_t)max_count;

  // Register post ISR processing function
  os_Info.post_process.semaphore = os_SemaphorePostProcess;

  return semaphore;
}

/// Get name of a Semaphore object.
/// \note API identical to osSemaphoreGetName
const char *os_svcSemaphoreGetName (osSemaphoreId_t semaphore_id) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;

  // Check parameters
  if ((semaphore == NULL) ||
      (semaphore->id != os_IdSemaphore)) {
    return NULL;
  }

  // Check object state
  if (semaphore->state == os_ObjectInactive) {
    return NULL;
  }

  return semaphore->name;
}

/// Acquire a Semaphore token or timeout if no tokens are available.
/// \note API identical to osSemaphoreAcquire
osStatus_t os_svcSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;

  // Check parameters
  if ((semaphore == NULL) ||
      (semaphore->id != os_IdSemaphore)) {
    return osErrorParameter;
  }

  // Check object state
  if (semaphore->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Try to acquire token
  if (os_SemaphoreTokenDecrement(semaphore) == 0U) {
    // No token available
    if (timeout != 0U) {
      // Suspend current Thread
      os_ThreadListPut((os_object_t*)semaphore, os_ThreadGetRunning());
      os_ThreadWaitEnter(os_ThreadWaitingSemaphore, timeout);
      return osErrorTimeout;
    } else {
      return osErrorResource;
    }
  }

  return osOK;
}

/// Release a Semaphore token that was acquired by osSemaphoreAcquire.
/// \note API identical to osSemaphoreRelease
osStatus_t os_svcSemaphoreRelease (osSemaphoreId_t semaphore_id) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;
  os_thread_t    *thread;

  // Check parameters
  if ((semaphore == NULL) ||
      (semaphore->id != os_IdSemaphore)) {
    return osErrorParameter;
  }

  // Check object state
  if (semaphore->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Check if Thread is waiting for a token
  if (semaphore->thread_list != NULL) {
    // Wakeup waiting Thread with highest Priority
    thread = os_ThreadListGet((os_object_t*)semaphore);
    os_ThreadWaitExit(thread, (uint32_t)osOK, true);
  } else {
    // Try to release token
    if (os_SemaphoreTokenIncrement(semaphore) == 0U) {
      return osErrorResource;
    }
  }

  return osOK;
}

/// Get current Semaphore token count.
/// \note API identical to osSemaphoreGetCount
uint32_t os_svcSemaphoreGetCount (osSemaphoreId_t semaphore_id) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;

  // Check parameters
  if ((semaphore == NULL) ||
      (semaphore->id != os_IdSemaphore)) {
    return 0U;
  }

  // Check object state
  if (semaphore->state == os_ObjectInactive) {
    return 0U;
  }

  return semaphore->tokens;
}

/// Delete a Semaphore object.
/// \note API identical to osSemaphoreDelete
osStatus_t os_svcSemaphoreDelete (osSemaphoreId_t semaphore_id) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;
  os_thread_t    *thread;

  // Check parameters
  if ((semaphore == NULL) ||
      (semaphore->id != os_IdSemaphore)) {
    return osErrorParameter;
  }

  // Check object state
  if (semaphore->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Mark object as inactive
  semaphore->state = os_ObjectInactive;

  // Unblock waiting threads
  if (semaphore->thread_list != NULL) {
    do {
      thread = os_ThreadListGet((os_object_t*)semaphore);
      os_ThreadWaitExit(thread, (uint32_t)osErrorResource, false);
    } while (semaphore->thread_list != NULL);
    os_ThreadDispatch(NULL);
  }

  // Free object memory
  if (semaphore->flags & os_FlagSystemObject) {
    if (os_Info.mpi.semaphore != NULL) {
      os_MemoryPoolFree(os_Info.mpi.semaphore, semaphore);
    } else {
      os_MemoryFree(os_Info.mem.common, semaphore);
    }
  }

  return osOK;
}


//  ==== ISR Calls ====

/// Acquire a Semaphore token or timeout if no tokens are available.
/// \note API identical to osSemaphoreAcquire
__STATIC_INLINE
osStatus_t os_isrSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;

  // Check parameters
  if ((semaphore == NULL) ||
      (semaphore->id != os_IdSemaphore)) {
    return osErrorParameter;
  }
  if (timeout != 0U) {
    return osErrorParameter;
  }

  // Check object state
  if (semaphore->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Try to acquire token
  if (os_SemaphoreTokenDecrement(semaphore) == 0U) {
    // No token available
    return osErrorResource;
  }

  return osOK;
}

/// Release a Semaphore token that was acquired by osSemaphoreAcquire.
/// \note API identical to osSemaphoreRelease
__STATIC_INLINE
osStatus_t os_isrSemaphoreRelease (osSemaphoreId_t semaphore_id) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;

  // Check parameters
  if ((semaphore == NULL) ||
      (semaphore->id != os_IdSemaphore)) {
    return osErrorParameter;
  }

  // Check object state
  if (semaphore->state == os_ObjectInactive) {
    return osErrorResource;
  }

  // Try to release token
  if (os_SemaphoreTokenIncrement(semaphore) != 0U) {
    // Register post ISR processing
    os_PostProcess((os_object_t *)semaphore);
  } else {
    return osErrorResource;
  }

  return osOK;
}


//  ==== Public API ====

/// Create and Initialize a Semaphore object.
osSemaphoreId_t osSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return NULL;
  }
  if ((os_KernelGetState() == os_KernelReady) && IS_PRIVILEGED()) {
    // Kernel Ready (not running) and in Privileged mode
    return os_svcSemaphoreNew(max_count, initial_count, attr);
  } else {
    return  __svcSemaphoreNew(max_count, initial_count, attr);
  }
}

/// Get name of a Semaphore object.
const char *osSemaphoreGetName (osSemaphoreId_t semaphore_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return NULL;
  }
  return  __svcSemaphoreGetName(semaphore_id);
}

/// Acquire a Semaphore token or timeout if no tokens are available.
osStatus_t osSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return os_isrSemaphoreAcquire(semaphore_id, timeout);
  } else {
    return  __svcSemaphoreAcquire(semaphore_id, timeout);
  }
}

/// Release a Semaphore token that was acquired by osSemaphoreAcquire.
osStatus_t osSemaphoreRelease (osSemaphoreId_t semaphore_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return os_isrSemaphoreRelease(semaphore_id);
  } else {
    return  __svcSemaphoreRelease(semaphore_id);
  }
}

/// Get current Semaphore token count.
uint32_t osSemaphoreGetCount (osSemaphoreId_t semaphore_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return os_svcSemaphoreGetCount(semaphore_id);
  } else {
    return  __svcSemaphoreGetCount(semaphore_id);
  }
}

/// Delete a Semaphore object.
osStatus_t osSemaphoreDelete (osSemaphoreId_t semaphore_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return osErrorISR;
  }
  return __svcSemaphoreDelete(semaphore_id);
}
