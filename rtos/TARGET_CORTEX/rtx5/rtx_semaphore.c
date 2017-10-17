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
 * Title:       Semaphore functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Helper functions ====

/// Decrement Semaphore tokens.
/// \param[in]  semaphore       semaphore object.
/// \return 1 - success, 0 - failure.
static uint32_t SemaphoreTokenDecrement (os_semaphore_t *semaphore) {
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
  if (atomic_dec16_nz(&semaphore->tokens) != 0U) {
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
static uint32_t SemaphoreTokenIncrement (os_semaphore_t *semaphore) {
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
  if (atomic_inc16_lt(&semaphore->tokens, semaphore->max_tokens) < semaphore->max_tokens) {
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
void osRtxSemaphorePostProcess (os_semaphore_t *semaphore) {
  os_thread_t *thread;

  if (semaphore->state == osRtxObjectInactive) {
    return;
  }

  // Check if Thread is waiting for a token
  if (semaphore->thread_list != NULL) {
    // Try to acquire token
    if (SemaphoreTokenDecrement(semaphore) != 0U) {
      // Wakeup waiting Thread with highest Priority
      thread = osRtxThreadListGet((os_object_t*)semaphore);
      osRtxThreadWaitExit(thread, (uint32_t)osOK, false);
      EvrRtxSemaphoreAcquired(semaphore);
    }
  }
}


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_3M(SemaphoreNew,      osSemaphoreId_t, uint32_t, uint32_t, const osSemaphoreAttr_t *)
SVC0_1 (SemaphoreGetName,  const char *,    osSemaphoreId_t)
SVC0_2 (SemaphoreAcquire,  osStatus_t,      osSemaphoreId_t, uint32_t)
SVC0_1 (SemaphoreRelease,  osStatus_t,      osSemaphoreId_t)
SVC0_1 (SemaphoreGetCount, uint32_t,        osSemaphoreId_t)
SVC0_1 (SemaphoreDelete,   osStatus_t,      osSemaphoreId_t)

/// Create and Initialize a Semaphore object.
/// \note API identical to osSemaphoreNew
osSemaphoreId_t svcRtxSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr) {
  os_semaphore_t *semaphore;
  uint8_t         flags;
  const char     *name;

  // Check parameters
  if ((max_count == 0U) || (max_count > osRtxSemaphoreTokenLimit) || (initial_count > max_count)) {
    EvrRtxSemaphoreError(NULL, osErrorParameter);
    return NULL;
  }

  // Process attributes
  if (attr != NULL) {
    name      = attr->name;
    semaphore = attr->cb_mem;
    if (semaphore != NULL) {
      if (((uint32_t)semaphore & 3U) || (attr->cb_size < sizeof(os_semaphore_t))) {
        EvrRtxSemaphoreError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    } else {
      if (attr->cb_size != 0U) {
        EvrRtxSemaphoreError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    }
  } else {
    name      = NULL;
    semaphore = NULL;
  }

  // Allocate object memory if not provided
  if (semaphore == NULL) {
    if (osRtxInfo.mpi.semaphore != NULL) {
      semaphore = osRtxMemoryPoolAlloc(osRtxInfo.mpi.semaphore);
    } else {
      semaphore = osRtxMemoryAlloc(osRtxInfo.mem.common, sizeof(os_semaphore_t), 1U);
    }
    if (semaphore == NULL) {
      EvrRtxSemaphoreError(NULL, osErrorNoMemory);
      return NULL;
    }
    flags = osRtxFlagSystemObject;
  } else {
    flags = 0U;
  }

  // Initialize control block
  semaphore->id          = osRtxIdSemaphore;
  semaphore->state       = osRtxObjectActive;
  semaphore->flags       = flags;
  semaphore->name        = name;
  semaphore->thread_list = NULL;
  semaphore->tokens      = (uint16_t)initial_count;
  semaphore->max_tokens  = (uint16_t)max_count;

  // Register post ISR processing function
  osRtxInfo.post_process.semaphore = osRtxSemaphorePostProcess;

  EvrRtxSemaphoreCreated(semaphore);

  return semaphore;
}

/// Get name of a Semaphore object.
/// \note API identical to osSemaphoreGetName
const char *svcRtxSemaphoreGetName (osSemaphoreId_t semaphore_id) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;

  // Check parameters
  if ((semaphore == NULL) || (semaphore->id != osRtxIdSemaphore)) {
    EvrRtxSemaphoreGetName(semaphore, NULL);
    return NULL;
  }

  // Check object state
  if (semaphore->state == osRtxObjectInactive) {
    EvrRtxSemaphoreGetName(semaphore, NULL);
    return NULL;
  }

  EvrRtxSemaphoreGetName(semaphore, semaphore->name);

  return semaphore->name;
}

/// Acquire a Semaphore token or timeout if no tokens are available.
/// \note API identical to osSemaphoreAcquire
osStatus_t svcRtxSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;

  // Check parameters
  if ((semaphore == NULL) || (semaphore->id != osRtxIdSemaphore)) {
    EvrRtxSemaphoreError(semaphore, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (semaphore->state == osRtxObjectInactive) {
    EvrRtxSemaphoreError(semaphore, osErrorResource);
    return osErrorResource;
  }

  // Try to acquire token
  if (SemaphoreTokenDecrement(semaphore) == 0U) {
    // No token available
    if (timeout != 0U) {
      EvrRtxSemaphoreAcquirePending(semaphore, timeout);
      // Suspend current Thread
      osRtxThreadListPut((os_object_t*)semaphore, osRtxThreadGetRunning());
      osRtxThreadWaitEnter(osRtxThreadWaitingSemaphore, timeout);
      return osErrorTimeout;
    } else {
      EvrRtxSemaphoreNotAcquired(semaphore);
      return osErrorResource;
    }
  }

  EvrRtxSemaphoreAcquired(semaphore);

  return osOK;
}

/// Release a Semaphore token that was acquired by osSemaphoreAcquire.
/// \note API identical to osSemaphoreRelease
osStatus_t svcRtxSemaphoreRelease (osSemaphoreId_t semaphore_id) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;
  os_thread_t    *thread;

  // Check parameters
  if ((semaphore == NULL) || (semaphore->id != osRtxIdSemaphore)) {
    EvrRtxSemaphoreError(semaphore, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (semaphore->state == osRtxObjectInactive) {
    EvrRtxSemaphoreError(semaphore, osErrorResource);
    return osErrorResource;
  }

  // Check if Thread is waiting for a token
  if (semaphore->thread_list != NULL) {
    EvrRtxSemaphoreReleased(semaphore);
    // Wakeup waiting Thread with highest Priority
    thread = osRtxThreadListGet((os_object_t*)semaphore);
    osRtxThreadWaitExit(thread, (uint32_t)osOK, true);
    EvrRtxSemaphoreAcquired(semaphore);
  } else {
    // Try to release token
    if (SemaphoreTokenIncrement(semaphore) == 0U) {
      EvrRtxSemaphoreError(semaphore, osRtxErrorSemaphoreCountLimit);
      return osErrorResource;
    }
    EvrRtxSemaphoreReleased(semaphore);
  }

  return osOK;
}

/// Get current Semaphore token count.
/// \note API identical to osSemaphoreGetCount
uint32_t svcRtxSemaphoreGetCount (osSemaphoreId_t semaphore_id) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;

  // Check parameters
  if ((semaphore == NULL) || (semaphore->id != osRtxIdSemaphore)) {
    EvrRtxSemaphoreGetCount(semaphore, 0U);
    return 0U;
  }

  // Check object state
  if (semaphore->state == osRtxObjectInactive) {
    EvrRtxSemaphoreGetCount(semaphore, 0U);
    return 0U;
  }

  EvrRtxSemaphoreGetCount(semaphore, semaphore->tokens);

  return semaphore->tokens;
}

/// Delete a Semaphore object.
/// \note API identical to osSemaphoreDelete
osStatus_t svcRtxSemaphoreDelete (osSemaphoreId_t semaphore_id) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;
  os_thread_t    *thread;

  // Check parameters
  if ((semaphore == NULL) || (semaphore->id != osRtxIdSemaphore)) {
    EvrRtxSemaphoreError(semaphore, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (semaphore->state == osRtxObjectInactive) {
    EvrRtxSemaphoreError(semaphore, osErrorResource);
    return osErrorResource;
  }

  // Mark object as inactive
  semaphore->state = osRtxObjectInactive;

  // Unblock waiting threads
  if (semaphore->thread_list != NULL) {
    do {
      thread = osRtxThreadListGet((os_object_t*)semaphore);
      osRtxThreadWaitExit(thread, (uint32_t)osErrorResource, false);
    } while (semaphore->thread_list != NULL);
    osRtxThreadDispatch(NULL);
  }

  // Free object memory
  if (semaphore->flags & osRtxFlagSystemObject) {
    if (osRtxInfo.mpi.semaphore != NULL) {
      osRtxMemoryPoolFree(osRtxInfo.mpi.semaphore, semaphore);
    } else {
      osRtxMemoryFree(osRtxInfo.mem.common, semaphore);
    }
  }

  EvrRtxSemaphoreDestroyed(semaphore);

  return osOK;
}


//  ==== ISR Calls ====

/// Acquire a Semaphore token or timeout if no tokens are available.
/// \note API identical to osSemaphoreAcquire
__STATIC_INLINE
osStatus_t isrRtxSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;

  // Check parameters
  if ((semaphore == NULL) || (semaphore->id != osRtxIdSemaphore) || (timeout != 0U)) {
    EvrRtxSemaphoreError(semaphore, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (semaphore->state == osRtxObjectInactive) {
    EvrRtxSemaphoreError(semaphore, osErrorResource);
    return osErrorResource;
  }

  // Try to acquire token
  if (SemaphoreTokenDecrement(semaphore) == 0U) {
    // No token available
    EvrRtxSemaphoreNotAcquired(semaphore);
    return osErrorResource;
  }

  EvrRtxSemaphoreAcquired(semaphore);

  return osOK;
}

/// Release a Semaphore token that was acquired by osSemaphoreAcquire.
/// \note API identical to osSemaphoreRelease
__STATIC_INLINE
osStatus_t isrRtxSemaphoreRelease (osSemaphoreId_t semaphore_id) {
  os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;

  // Check parameters
  if ((semaphore == NULL) || (semaphore->id != osRtxIdSemaphore)) {
    EvrRtxSemaphoreError(semaphore, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (semaphore->state == osRtxObjectInactive) {
    EvrRtxSemaphoreError(semaphore, osErrorResource);
    return osErrorResource;
  }

  // Try to release token
  if (SemaphoreTokenIncrement(semaphore) != 0U) {
    // Register post ISR processing
    osRtxPostProcess((os_object_t *)semaphore);
  } else {
    EvrRtxSemaphoreError(semaphore, osRtxErrorSemaphoreCountLimit);
    return osErrorResource;
  }

  EvrRtxSemaphoreReleased(semaphore);

  return osOK;
}


//  ==== Public API ====

/// Create and Initialize a Semaphore object.
osSemaphoreId_t osSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr) {
  EvrRtxSemaphoreNew(max_count, initial_count, attr);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxSemaphoreError(NULL, osErrorISR);
    return NULL;
  }
  return __svcSemaphoreNew(max_count, initial_count, attr);
}

/// Get name of a Semaphore object.
const char *osSemaphoreGetName (osSemaphoreId_t semaphore_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxSemaphoreGetName(semaphore_id, NULL);
    return NULL;
  }
  return __svcSemaphoreGetName(semaphore_id);
}

/// Acquire a Semaphore token or timeout if no tokens are available.
osStatus_t osSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout) {
  EvrRtxSemaphoreAcquire(semaphore_id, timeout);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return isrRtxSemaphoreAcquire(semaphore_id, timeout);
  } else {
    return  __svcSemaphoreAcquire(semaphore_id, timeout);
  }
}

/// Release a Semaphore token that was acquired by osSemaphoreAcquire.
osStatus_t osSemaphoreRelease (osSemaphoreId_t semaphore_id) {
  EvrRtxSemaphoreRelease(semaphore_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return isrRtxSemaphoreRelease(semaphore_id);
  } else {
    return  __svcSemaphoreRelease(semaphore_id);
  }
}

/// Get current Semaphore token count.
uint32_t osSemaphoreGetCount (osSemaphoreId_t semaphore_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxSemaphoreGetCount(semaphore_id);
  } else {
    return  __svcSemaphoreGetCount(semaphore_id);
  }
}

/// Delete a Semaphore object.
osStatus_t osSemaphoreDelete (osSemaphoreId_t semaphore_id) {
  EvrRtxSemaphoreDelete(semaphore_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxSemaphoreError(semaphore_id, osErrorISR);
    return osErrorISR;
  }
  return __svcSemaphoreDelete(semaphore_id);
}
