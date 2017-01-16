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
 * Title:       Memory Pool functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Library functions ====

/// Initialize Memory Pool.
/// \param[in]  mp_info         memory pool info.
/// \param[in]  block_count     maximum number of memory blocks in memory pool.
/// \param[in]  block_size      size of a memory block in bytes.
/// \param[in]  block_mem       pointer to memory for block storage.
/// \return 1 - success, 0 - failure.
uint32_t osRtxMemoryPoolInit (os_mp_info_t *mp_info, uint32_t block_count, uint32_t block_size, void *block_mem) {
  void *block;

  // Check parameters
  if ((mp_info == NULL) || (block_count == 0U) || (block_size  == 0U) || (block_mem   == NULL)) {
    return 0U;
  }

  // Initialize information structure
  mp_info->max_blocks  = block_count;
  mp_info->used_blocks = 0U;
  mp_info->block_size  = block_size;
  mp_info->block_base  = block_mem;
  mp_info->block_free  = block_mem;
  mp_info->block_lim   = (uint8_t *)block_mem + (block_count * block_size);

  EvrRtxMemoryBlockInit(mp_info, block_count, block_size, block_mem);

  // Link all free blocks
  while (--block_count) {
    block = (uint8_t *)block_mem + block_size;
    *((void **)block_mem) = block;
    block_mem = block;
  }
  *((void **)block_mem) = NULL;

  return 1U;
}

/// Allocate a memory block from a Memory Pool.
/// \param[in]  mp_info         memory pool info.
/// \return address of the allocated memory block or NULL in case of no memory is available.
void *osRtxMemoryPoolAlloc (os_mp_info_t *mp_info) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  void *block;

  if (mp_info == NULL) {
    EvrRtxMemoryBlockAlloc(NULL, NULL);
    return NULL;
  }

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  if (mp_info->used_blocks < mp_info->max_blocks) {
    mp_info->used_blocks++;
    block = mp_info->block_free;
    if (block != NULL) {
      mp_info->block_free = *((void **)block);
    }
  } else {
    block = NULL;
  }

  if (primask == 0U) {
    __enable_irq();
  }
#else
  if (atomic_inc32_lt(&mp_info->used_blocks, mp_info->max_blocks) < mp_info->max_blocks) {
    block = atomic_link_get(&mp_info->block_free);
  } else {
    block = NULL;
  }
#endif

  EvrRtxMemoryBlockAlloc(mp_info, block);

  return block;
}

/// Return an allocated memory block back to a Memory Pool.
/// \param[in]  mp_info         memory pool info.
/// \param[in]  block           address of the allocated memory block to be returned to the memory pool.
/// \return status code that indicates the execution status of the function.
osStatus_t osRtxMemoryPoolFree (os_mp_info_t *mp_info, void *block) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#endif
  osStatus_t status;

  if ((mp_info == NULL) || (block < mp_info->block_base) || (block >= mp_info->block_lim)) {
    EvrRtxMemoryBlockFree(mp_info, block, osErrorParameter);
    return osErrorParameter;
  }

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  if (mp_info->used_blocks != 0U) {
    mp_info->used_blocks--;
    *((void **)block) = mp_info->block_free;
    mp_info->block_free = block;
    status = osOK;
  } else {
    status = osErrorResource;
  }

  if (primask == 0U) {
    __enable_irq();
  }
#else
  if (atomic_dec32_nz(&mp_info->used_blocks) != 0U) {
    atomic_link_put(&mp_info->block_free, block);
    status = osOK;
  } else {
    status = osErrorResource;
  }
#endif

  EvrRtxMemoryBlockFree(mp_info, block, status);

  return status;
}

/// Memory Pool post ISR processing.
/// \param[in]  mp              memory pool object.
void osRtxMemoryPoolPostProcess (os_memory_pool_t *mp) {
  void        *block;
  os_thread_t *thread;

  if (mp->state == osRtxObjectInactive) {
    return;
  }

  // Check if Thread is waiting to allocate memory
  if (mp->thread_list != NULL) {
    // Allocate memory
    block = osRtxMemoryPoolAlloc(&mp->mp_info);
    if (block != NULL) {
      // Wakeup waiting Thread with highest Priority
      thread = osRtxThreadListGet((os_object_t*)mp);
      osRtxThreadWaitExit(thread, (uint32_t)block, false);
      EvrRtxMemoryPoolAllocated(mp, block);
    }
  }
}


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_3M(MemoryPoolNew,          osMemoryPoolId_t, uint32_t, uint32_t, const osMemoryPoolAttr_t *)
SVC0_1 (MemoryPoolGetName,      const char *,     osMemoryPoolId_t)
SVC0_2 (MemoryPoolAlloc,        void *,           osMemoryPoolId_t, uint32_t)
SVC0_2 (MemoryPoolFree,         osStatus_t,       osMemoryPoolId_t, void *)
SVC0_1 (MemoryPoolGetCapacity,  uint32_t,         osMemoryPoolId_t)
SVC0_1 (MemoryPoolGetBlockSize, uint32_t,         osMemoryPoolId_t)
SVC0_1 (MemoryPoolGetCount,     uint32_t,         osMemoryPoolId_t)
SVC0_1 (MemoryPoolGetSpace,     uint32_t,         osMemoryPoolId_t)
SVC0_1 (MemoryPoolDelete,       osStatus_t,       osMemoryPoolId_t)

/// Create and Initialize a Memory Pool object.
/// \note API identical to osMemoryPoolNew
osMemoryPoolId_t svcRtxMemoryPoolNew (uint32_t block_count, uint32_t block_size, const osMemoryPoolAttr_t *attr) {
  os_memory_pool_t *mp;
  void             *mp_mem;
  uint32_t          mp_size;
  uint32_t          size;
  uint8_t           flags;
  const char       *name;

  // Check parameters
  if ((block_count == 0U) || (block_size  == 0U)) {
    EvrRtxMemoryPoolError(NULL, osErrorParameter);
    return NULL;
  }
  block_size = (block_size + 3U) & ~3UL;
  if ((__CLZ(block_count) + __CLZ(block_size)) < 32) {
    EvrRtxMemoryPoolError(NULL, osErrorParameter);
    return NULL;
  }

  size = block_count * block_size;

  // Process attributes
  if (attr != NULL) {
    name    = attr->name;
    mp      = attr->cb_mem;
    mp_mem  = attr->mp_mem;
    mp_size = attr->mp_size;
    if (mp != NULL) {
      if (((uint32_t)mp & 3U) || (attr->cb_size < sizeof(os_memory_pool_t))) {
        EvrRtxMemoryPoolError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    } else {
      if (attr->cb_size != 0U) {
        EvrRtxMemoryPoolError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    }
    if (mp_mem != NULL) {
      if (((uint32_t)mp_mem & 3U) || (mp_size < size)) {
        EvrRtxMemoryPoolError(NULL, osRtxErrorInvalidDataMemory);
        return NULL;
      }
    } else {
      if (mp_size != 0U) {
        EvrRtxMemoryPoolError(NULL, osRtxErrorInvalidDataMemory);
        return NULL;
      }
    }
  } else {
    name   = NULL;
    mp     = NULL;
    mp_mem = NULL;
  }

  // Allocate object memory if not provided
  if (mp == NULL) {
    if (osRtxInfo.mpi.memory_pool != NULL) {
      mp = osRtxMemoryPoolAlloc(osRtxInfo.mpi.memory_pool);
    } else {
      mp = osRtxMemoryAlloc(osRtxInfo.mem.common, sizeof(os_memory_pool_t), 1U);
    }
    if (mp == NULL) {
      EvrRtxMemoryPoolError(NULL, osErrorNoMemory);
      return NULL;
    }
    flags = osRtxFlagSystemObject;
  } else {
    flags = 0U;
  }

  // Allocate data memory if not provided
  if (mp_mem == NULL) {
    mp_mem = osRtxMemoryAlloc(osRtxInfo.mem.mp_data, size, 0U);
    if (mp_mem == NULL) {
      EvrRtxMemoryPoolError(NULL, osErrorNoMemory);
      if (flags & osRtxFlagSystemObject) {
        if (osRtxInfo.mpi.memory_pool != NULL) {
          osRtxMemoryPoolFree(osRtxInfo.mpi.memory_pool, mp);
        } else {
          osRtxMemoryFree(osRtxInfo.mem.common, mp);
        }
      }
      return NULL;
    }
    memset(mp_mem, 0, size);
    flags |= osRtxFlagSystemMemory;
  }

  // Initialize control block
  mp->id          = osRtxIdMemoryPool;
  mp->state       = osRtxObjectActive;
  mp->flags       = flags;
  mp->name        = name;
  mp->thread_list = NULL;
  osRtxMemoryPoolInit(&mp->mp_info, block_count, block_size, mp_mem);

  // Register post ISR processing function
  osRtxInfo.post_process.memory_pool = osRtxMemoryPoolPostProcess;

  EvrRtxMemoryPoolCreated(mp);

  return mp;
}

/// Get name of a Memory Pool object.
/// \note API identical to osMemoryPoolGetName
const char *svcRtxMemoryPoolGetName (osMemoryPoolId_t mp_id) {
  os_memory_pool_t *mp = (os_memory_pool_t *)mp_id;

  // Check parameters
  if ((mp == NULL) || (mp->id != osRtxIdMemoryPool)) {
    EvrRtxMemoryPoolGetName(mp, NULL);
    return NULL;
  }

  // Check object state
  if (mp->state == osRtxObjectInactive) {
    EvrRtxMemoryPoolGetName(mp, NULL);
    return NULL;
  }

  EvrRtxMemoryPoolGetName(mp, mp->name);

  return mp->name;
}

/// Allocate a memory block from a Memory Pool.
/// \note API identical to osMemoryPoolAlloc
void *svcRtxMemoryPoolAlloc (osMemoryPoolId_t mp_id, uint32_t timeout) {
  os_memory_pool_t *mp = (os_memory_pool_t *)mp_id;
  void             *block;

  // Check parameters
  if ((mp == NULL) || (mp->id != osRtxIdMemoryPool)) {
    EvrRtxMemoryPoolError(mp, osErrorParameter);
    return NULL;
  }

  // Check object state
  if (mp->state == osRtxObjectInactive) {
    EvrRtxMemoryPoolError(mp, osErrorResource);
    return NULL;
  }

  // Allocate memory
  block = osRtxMemoryPoolAlloc(&mp->mp_info);
  if (block == NULL) {
    // No memory available
    if (timeout != 0U) {
      EvrRtxMemoryPoolAllocPending(mp, timeout);
      // Suspend current Thread
      osRtxThreadListPut((os_object_t*)mp, osRtxThreadGetRunning());
      osRtxThreadWaitEnter(osRtxThreadWaitingMemoryPool, timeout);
    } else {
      EvrRtxMemoryPoolAllocFailed(mp);
    }
  } else {
    EvrRtxMemoryPoolAllocated(mp, block);
  }

  return block;
}

/// Return an allocated memory block back to a Memory Pool.
/// \note API identical to osMemoryPoolFree
osStatus_t svcRtxMemoryPoolFree (osMemoryPoolId_t mp_id, void *block) {
  os_memory_pool_t *mp = (os_memory_pool_t *)mp_id;
  os_thread_t      *thread;
  osStatus_t        status;

  // Check parameters
  if ((mp == NULL) || (mp->id != osRtxIdMemoryPool)) {
    EvrRtxMemoryPoolError(mp, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mp->state == osRtxObjectInactive) {
    EvrRtxMemoryPoolError(mp, osErrorResource);
    return osErrorResource;
  }

  // Free memory
  status = osRtxMemoryPoolFree(&mp->mp_info, block);
  if (status == osOK) {
    EvrRtxMemoryPoolDeallocated(mp, block);
    // Check if Thread is waiting to allocate memory
    if (mp->thread_list != NULL) {
      // Allocate memory
      block = osRtxMemoryPoolAlloc(&mp->mp_info);
      if (block != NULL) {
        // Wakeup waiting Thread with highest Priority
        thread = osRtxThreadListGet((os_object_t*)mp);
        osRtxThreadWaitExit(thread, (uint32_t)block, true);
        EvrRtxMemoryPoolAllocated(mp, block);
      }
    }
  } else {
    EvrRtxMemoryPoolFreeFailed(mp, block);
  }

  return status;
}

/// Get maximum number of memory blocks in a Memory Pool.
/// \note API identical to osMemoryPoolGetCapacity
uint32_t svcRtxMemoryPoolGetCapacity (osMemoryPoolId_t mp_id) {
  os_memory_pool_t *mp = (os_memory_pool_t *)mp_id;

  // Check parameters
  if ((mp == NULL) || (mp->id != osRtxIdMemoryPool)) {
    EvrRtxMemoryPoolGetCapacity(mp, 0U);
    return 0U;
  }

  // Check object state
  if (mp->state == osRtxObjectInactive) {
    EvrRtxMemoryPoolGetCapacity(mp, 0U);
    return 0U;
  }

  EvrRtxMemoryPoolGetCapacity(mp, mp->mp_info.max_blocks);

  return mp->mp_info.max_blocks;
}

/// Get memory block size in a Memory Pool.
/// \note API identical to osMemoryPoolGetBlockSize
uint32_t svcRtxMemoryPoolGetBlockSize (osMemoryPoolId_t mp_id) {
  os_memory_pool_t *mp = (os_memory_pool_t *)mp_id;

  // Check parameters
  if ((mp == NULL) || (mp->id != osRtxIdMemoryPool)) {
    EvrRtxMemoryPoolGetBlockSize(mp, 0U);
    return 0U;
  }

  // Check object state
  if (mp->state == osRtxObjectInactive) {
    EvrRtxMemoryPoolGetBlockSize(mp, 0U);
    return 0U;
  }

  EvrRtxMemoryPoolGetBlockSize(mp, mp->mp_info.block_size);

  return mp->mp_info.block_size;
}

/// Get number of memory blocks used in a Memory Pool.
/// \note API identical to osMemoryPoolGetCount
uint32_t svcRtxMemoryPoolGetCount (osMemoryPoolId_t mp_id) {
  os_memory_pool_t *mp = (os_memory_pool_t *)mp_id;

  // Check parameters
  if ((mp == NULL) || (mp->id != osRtxIdMemoryPool)) {
    EvrRtxMemoryPoolGetCount(mp, 0U);
    return 0U;
  }

  // Check object state
  if (mp->state == osRtxObjectInactive) {
    EvrRtxMemoryPoolGetCount(mp, 0U);
    return 0U;
  }

  EvrRtxMemoryPoolGetCount(mp, mp->mp_info.used_blocks);

  return mp->mp_info.used_blocks;
}

/// Get number of memory blocks available in a Memory Pool.
/// \note API identical to osMemoryPoolGetSpace
uint32_t svcRtxMemoryPoolGetSpace (osMemoryPoolId_t mp_id) {
  os_memory_pool_t *mp = (os_memory_pool_t *)mp_id;

  // Check parameters
  if ((mp == NULL) || (mp->id != osRtxIdMemoryPool)) {
    EvrRtxMemoryPoolGetSpace(mp, 0U);
    return 0U;
  }

  // Check object state
  if (mp->state == osRtxObjectInactive) {
    EvrRtxMemoryPoolGetSpace(mp, 0U);
    return 0U;
  }

  EvrRtxMemoryPoolGetSpace(mp, mp->mp_info.max_blocks - mp->mp_info.used_blocks);

  return (mp->mp_info.max_blocks - mp->mp_info.used_blocks);
}

/// Delete a Memory Pool object.
/// \note API identical to osMemoryPoolDelete
osStatus_t svcRtxMemoryPoolDelete (osMemoryPoolId_t mp_id) {
  os_memory_pool_t *mp = (os_memory_pool_t *)mp_id;
  os_thread_t      *thread;

  // Check parameters
  if ((mp == NULL) || (mp->id != osRtxIdMemoryPool)) {
    EvrRtxMemoryPoolError(mp, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mp->state == osRtxObjectInactive) {
    EvrRtxMemoryPoolError(mp, osErrorResource);
    return osErrorResource;
  }

  // Mark object as inactive
  mp->state = osRtxObjectInactive;

  // Unblock waiting threads
  if (mp->thread_list != NULL) {
    do {
      thread = osRtxThreadListGet((os_object_t*)mp);
      osRtxThreadWaitExit(thread, 0U, false);
    } while (mp->thread_list != NULL);
    osRtxThreadDispatch(NULL);
  }

  // Free data memory
  if (mp->flags & osRtxFlagSystemMemory) {
    osRtxMemoryFree(osRtxInfo.mem.mp_data, mp->mp_info.block_base);
  }

  // Free object memory
  if (mp->flags & osRtxFlagSystemObject) {
    if (osRtxInfo.mpi.memory_pool != NULL) {
      osRtxMemoryPoolFree(osRtxInfo.mpi.memory_pool, mp);
    } else {
      osRtxMemoryFree(osRtxInfo.mem.common, mp);
    }
  }

  EvrRtxMemoryPoolDestroyed(mp);

  return osOK;
}


//  ==== ISR Calls ====

/// Allocate a memory block from a Memory Pool.
/// \note API identical to osMemoryPoolAlloc
__STATIC_INLINE
void *isrRtxMemoryPoolAlloc (osMemoryPoolId_t mp_id, uint32_t timeout) {
  os_memory_pool_t *mp = (os_memory_pool_t *)mp_id;
  void             *block;

  // Check parameters
  if ((mp == NULL) || (mp->id != osRtxIdMemoryPool) || (timeout != 0U)) {
    EvrRtxMemoryPoolError(mp, osErrorParameter);
    return NULL;
  }

  // Check object state
  if (mp->state == osRtxObjectInactive) {
    EvrRtxMemoryPoolError(mp, osErrorResource);
    return NULL;
  }

  // Allocate memory
  block = osRtxMemoryPoolAlloc(&mp->mp_info);
  if (block == NULL) {
    EvrRtxMemoryPoolAllocFailed(mp);
  } else {
    EvrRtxMemoryPoolAllocated(mp, block);
  }

  return block;
}

/// Return an allocated memory block back to a Memory Pool.
/// \note API identical to osMemoryPoolFree
__STATIC_INLINE
osStatus_t isrRtxMemoryPoolFree (osMemoryPoolId_t mp_id, void *block) {
  os_memory_pool_t *mp = (os_memory_pool_t *)mp_id;
  osStatus_t        status;

  // Check parameters
  if ((mp == NULL) || (mp->id != osRtxIdMemoryPool)) {
    EvrRtxMemoryPoolError(mp, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mp->state == osRtxObjectInactive) {
    EvrRtxMemoryPoolError(mp, osErrorResource);
    return osErrorResource;
  }

  // Free memory
  status = osRtxMemoryPoolFree(&mp->mp_info, block);
  if (status == osOK) {
    // Register post ISR processing
    osRtxPostProcess((os_object_t *)mp);
    EvrRtxMemoryPoolDeallocated(mp, block);
  } else {
    EvrRtxMemoryPoolFreeFailed(mp, block);
  }

  return status;
}


//  ==== Public API ====

/// Create and Initialize a Memory Pool object.
osMemoryPoolId_t osMemoryPoolNew (uint32_t block_count, uint32_t block_size, const osMemoryPoolAttr_t *attr) {
  EvrRtxMemoryPoolNew(block_count, block_size, attr);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMemoryPoolError(NULL, osErrorISR);
    return NULL;
  }
  return __svcMemoryPoolNew(block_count, block_size, attr);
}

/// Get name of a Memory Pool object.
const char *osMemoryPoolGetName (osMemoryPoolId_t mp_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMemoryPoolGetName(mp_id, NULL);
    return NULL;
  }
  return __svcMemoryPoolGetName(mp_id);
}

/// Allocate a memory block from a Memory Pool.
void *osMemoryPoolAlloc (osMemoryPoolId_t mp_id, uint32_t timeout) {
  EvrRtxMemoryPoolAlloc(mp_id, timeout);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return isrRtxMemoryPoolAlloc(mp_id, timeout);
  } else {
    return  __svcMemoryPoolAlloc(mp_id, timeout);
  }
}

/// Return an allocated memory block back to a Memory Pool.
osStatus_t osMemoryPoolFree (osMemoryPoolId_t mp_id, void *block) {
  EvrRtxMemoryPoolFree(mp_id, block);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return isrRtxMemoryPoolFree(mp_id, block);
  } else {
    return  __svcMemoryPoolFree(mp_id, block);
  }
}

/// Get maximum number of memory blocks in a Memory Pool.
uint32_t osMemoryPoolGetCapacity (osMemoryPoolId_t mp_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxMemoryPoolGetCapacity(mp_id);
  } else {
    return  __svcMemoryPoolGetCapacity(mp_id);
  }
}

/// Get memory block size in a Memory Pool.
uint32_t osMemoryPoolGetBlockSize (osMemoryPoolId_t mp_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxMemoryPoolGetBlockSize(mp_id);
  } else {
    return  __svcMemoryPoolGetBlockSize(mp_id);
  }
}

/// Get number of memory blocks used in a Memory Pool.
uint32_t osMemoryPoolGetCount (osMemoryPoolId_t mp_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxMemoryPoolGetCount(mp_id);
  } else {
    return  __svcMemoryPoolGetCount(mp_id);
  }
}

/// Get number of memory blocks available in a Memory Pool.
uint32_t osMemoryPoolGetSpace (osMemoryPoolId_t mp_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxMemoryPoolGetSpace(mp_id);
  } else {
    return  __svcMemoryPoolGetSpace(mp_id);
  }
}

/// Delete a Memory Pool object.
osStatus_t osMemoryPoolDelete (osMemoryPoolId_t mp_id) {
  EvrRtxMemoryPoolDelete(mp_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMemoryPoolError(mp_id, osErrorISR);
    return osErrorISR;
  }
  return __svcMemoryPoolDelete(mp_id);
}
