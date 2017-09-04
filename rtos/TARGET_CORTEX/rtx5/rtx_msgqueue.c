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
 * Title:       Message Queue functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Helper functions ====

/// Put a Message into Queue sorted by Priority (Highest at Head).
/// \param[in]  mq              message queue object.
/// \param[in]  msg             message object.
static void MessageQueuePut (os_message_queue_t *mq, os_message_t *msg) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t      primask = __get_PRIMASK();
#endif
  os_message_t *prev, *next;

  if (mq->msg_last != NULL) {
    prev = mq->msg_last;
    next = NULL;
    while ((prev != NULL) && (prev->priority < msg->priority)) {
      next = prev;
      prev = prev->prev;
    }
    msg->prev = prev;
    msg->next = next;
    if (prev != NULL) {
      prev->next = msg;
    } else {
      mq->msg_first = msg;
    }
    if (next != NULL) {
      next->prev = msg;
    } else {
      mq->msg_last = msg;
    }
  } else {
    msg->prev = NULL;
    msg->next = NULL;
    mq->msg_first= msg;
    mq->msg_last = msg;
  }

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  mq->msg_count++;

  if (primask == 0U) {
    __enable_irq();
  }
#else
  atomic_inc32(&mq->msg_count);
#endif
}

/// Get a Message from Queue with Highest Priority.
/// \param[in]  mq              message queue object.
/// \return message object or NULL.
static os_message_t *MessageQueueGet (os_message_queue_t *mq) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t      primask = __get_PRIMASK();
#endif
  os_message_t *msg;
  uint32_t      count;
  uint8_t       flags;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  count = mq->msg_count;
  if (count != 0U) {
    mq->msg_count--;
  }

  if (primask == 0U) {
    __enable_irq();
  }
#else
  count = atomic_dec32_nz(&mq->msg_count);
#endif

  if (count == 0U) {
    return NULL;
  }

  msg = mq->msg_first;

  while (msg != NULL) {
#if (__EXCLUSIVE_ACCESS == 0U)
    __disable_irq();

    flags = msg->flags;
    msg->flags = 1U;

    if (primask == 0U) {
      __enable_irq();
    }
#else
    flags = atomic_wr8(&msg->flags, 1U);
#endif
    if (flags == 0U) {
      break;
    }
    msg = msg->next;
  }

  return msg;
}

/// Remove a Message from Queue
/// \param[in]  mq              message queue object.
/// \param[in]  msg             message object.
static void MessageQueueRemove (os_message_queue_t *mq, os_message_t *msg) {

  if (msg->prev != NULL) {
    msg->prev->next = msg->next;
  } else {
    mq->msg_first = msg->next;
  }
  if (msg->next != NULL) {
    msg->next->prev = msg->prev;
  } else {
    mq->msg_last = msg->prev;
  }
}


//  ==== Library functions ====

/// Message Queue post ISR processing.
/// \param[in]  msg             message object.
void osRtxMessageQueuePostProcess (os_message_t *msg) {
  os_message_queue_t *mq;
  os_thread_t        *thread;
  uint32_t           *reg;
  void              **ptr;

  if (msg->state == osRtxObjectInactive) {
    return;
  }

  if (msg->flags != 0U) {
    // Remove Message
    ptr = (void *)((uint8_t *)msg + sizeof(os_message_t));
    mq = *ptr;
    if (mq->state == osRtxObjectInactive) {
      return;
    }
    MessageQueueRemove(mq, msg);
    // Free memory
    msg->state = osRtxObjectInactive;
    osRtxMemoryPoolFree(&mq->mp_info, msg);
    // Check if Thread is waiting to send a Message
    if ((mq->thread_list != NULL) && (mq->thread_list->state == osRtxThreadWaitingMessagePut)) {
      // Try to allocate memory
      msg = osRtxMemoryPoolAlloc(&mq->mp_info);
      if (msg != NULL) {
        // Wakeup waiting Thread with highest Priority
        thread = osRtxThreadListGet((os_object_t*)mq);
        osRtxThreadWaitExit(thread, (uint32_t)osOK, false);
        // Copy Message (R2: const void *msg_ptr, R3: uint8_t msg_prio)
        reg = osRtxThreadRegPtr(thread);
        memcpy((uint8_t *)msg + sizeof(os_message_t), (void *)reg[2], mq->msg_size);
        // Store Message into Queue
        msg->id       = osRtxIdMessage;
        msg->state    = osRtxObjectActive;
        msg->flags    = 0U;
        msg->priority = (uint8_t)reg[3];
        MessageQueuePut(mq, msg);
        EvrRtxMessageQueueInserted(mq, (void *)reg[2]);
      }
    }
  } else {
    // New Message
    mq = (void *)msg->next;
    if (mq->state == osRtxObjectInactive) {
      return;
    }
    // Check if Thread is waiting to receive a Message
    if ((mq->thread_list != NULL) && (mq->thread_list->state == osRtxThreadWaitingMessageGet)) {
      EvrRtxMessageQueueInserted(mq, (void *)msg->prev);
      // Wakeup waiting Thread with highest Priority
      thread = osRtxThreadListGet((os_object_t*)mq);
      osRtxThreadWaitExit(thread, (uint32_t)osOK, false);
      // Copy Message (R2: void *msg_ptr, R3: uint8_t *msg_prio)
      reg = osRtxThreadRegPtr(thread);
      memcpy((void *)reg[2], (uint8_t *)msg + sizeof(os_message_t), mq->msg_size);
      if (reg[3] != 0U) {
        *((uint8_t *)reg[3]) = msg->priority;
      }
      EvrRtxMessageQueueRetrieved(mq, (void *)reg[2]);
      // Free memory
      msg->state = osRtxObjectInactive;
      osRtxMemoryPoolFree(&mq->mp_info, msg);
    } else {
      EvrRtxMessageQueueInserted(mq, (void *)msg->prev);
      MessageQueuePut(mq, msg);
    }
  }
}


//  ==== Service Calls ====

SVC0_3M(MessageQueueNew,         osMessageQueueId_t, uint32_t, uint32_t, const osMessageQueueAttr_t *)
SVC0_1 (MessageQueueGetName,     const char *,       osMessageQueueId_t)
SVC0_4 (MessageQueuePut,         osStatus_t,         osMessageQueueId_t, const void *, uint8_t,   uint32_t)
SVC0_4 (MessageQueueGet,         osStatus_t,         osMessageQueueId_t,       void *, uint8_t *, uint32_t)
SVC0_1 (MessageQueueGetCapacity, uint32_t,           osMessageQueueId_t)
SVC0_1 (MessageQueueGetMsgSize,  uint32_t,           osMessageQueueId_t)
SVC0_1 (MessageQueueGetCount,    uint32_t,           osMessageQueueId_t)
SVC0_1 (MessageQueueGetSpace,    uint32_t,           osMessageQueueId_t)
SVC0_1 (MessageQueueReset,       osStatus_t,         osMessageQueueId_t)
SVC0_1 (MessageQueueDelete,      osStatus_t,         osMessageQueueId_t)

/// Create and Initialize a Message Queue object.
/// \note API identical to osMessageQueueNew
osMessageQueueId_t svcRtxMessageQueueNew (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr) {
  os_message_queue_t *mq;
  void               *mq_mem;
  uint32_t            mq_size;
  uint32_t            block_size;
  uint32_t            size;
  uint8_t             flags;
  const char         *name;

  // Check parameters
  if ((msg_count == 0U) || (msg_size  == 0U)) {
    EvrRtxMessageQueueError(NULL, osErrorParameter);
    return NULL;
  }
  msg_size = (msg_size + 3U) & ~3UL;
  block_size = msg_size + sizeof(os_message_t);
  if ((__CLZ(msg_count) + __CLZ(block_size)) < 32) {
    EvrRtxMessageQueueError(NULL, osErrorParameter);
    return NULL;
  }

  size = msg_count * block_size;

  // Process attributes
  if (attr != NULL) {
    name    = attr->name;
    mq      = attr->cb_mem;
    mq_mem  = attr->mq_mem;
    mq_size = attr->mq_size;
    if (mq != NULL) {
      if (((uint32_t)mq & 3U) || (attr->cb_size < sizeof(os_message_queue_t))) {
        EvrRtxMessageQueueError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    } else {
      if (attr->cb_size != 0U) {
        EvrRtxMessageQueueError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    }
    if (mq_mem != NULL) {
      if (((uint32_t)mq_mem & 3U) || (mq_size < size)) {
        EvrRtxMessageQueueError(NULL, osRtxErrorInvalidDataMemory);
        return NULL;
      }
    } else {
      if (mq_size != 0U) {
        EvrRtxMessageQueueError(NULL, osRtxErrorInvalidDataMemory);
        return NULL;
      }
    }
  } else {
    name   = NULL;
    mq     = NULL;
    mq_mem = NULL;
  }

  // Allocate object memory if not provided
  if (mq == NULL) {
    if (osRtxInfo.mpi.message_queue != NULL) {
      mq = osRtxMemoryPoolAlloc(osRtxInfo.mpi.message_queue);
    } else {
      mq = osRtxMemoryAlloc(osRtxInfo.mem.common, sizeof(os_message_queue_t), 1U);
    }
    if (mq == NULL) {
      EvrRtxMessageQueueError(NULL, osErrorNoMemory);
      return NULL;
    }
    flags = osRtxFlagSystemObject;
  } else {
    flags = 0U;
  }

  // Allocate data memory if not provided
  if (mq_mem == NULL) {
    mq_mem = osRtxMemoryAlloc(osRtxInfo.mem.mq_data, size, 0U);
    if (mq_mem == NULL) {
      EvrRtxMessageQueueError(NULL, osErrorNoMemory);
      if (flags & osRtxFlagSystemObject) {
        if (osRtxInfo.mpi.message_queue != NULL) {
          osRtxMemoryPoolFree(osRtxInfo.mpi.message_queue, mq);
        } else {
          osRtxMemoryFree(osRtxInfo.mem.common, mq);
        }
      }
      return NULL;
    }
    memset(mq_mem, 0, size);
    flags |= osRtxFlagSystemMemory;
  }

  // Initialize control block
  mq->id          = osRtxIdMessageQueue;
  mq->state       = osRtxObjectActive;
  mq->flags       = flags;
  mq->name        = name;
  mq->thread_list = NULL;
  mq->msg_size    = msg_size;
  mq->msg_count   = 0U;
  mq->msg_first   = NULL;
  mq->msg_last    = NULL;
  osRtxMemoryPoolInit(&mq->mp_info, msg_count, block_size, mq_mem);

  // Register post ISR processing function
  osRtxInfo.post_process.message_queue = osRtxMessageQueuePostProcess;

  EvrRtxMessageQueueCreated(mq);

  return mq;
}

/// Get name of a Message Queue object.
/// \note API identical to osMessageQueueGetName
const char *svcRtxMessageQueueGetName (osMessageQueueId_t mq_id) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue)) {
    EvrRtxMessageQueueGetName(mq, NULL);
    return NULL;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueGetName(mq, NULL);
    return NULL;
  }

  EvrRtxMessageQueueGetName(mq, mq->name);

  return mq->name;
}

/// Put a Message into a Queue or timeout if Queue is full.
/// \note API identical to osMessageQueuePut
osStatus_t svcRtxMessageQueuePut (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;
  os_message_t       *msg;
  os_thread_t        *thread;
  uint32_t           *reg;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue) || (msg_ptr == NULL)) {
    EvrRtxMessageQueueError(mq, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueError(mq, osErrorResource);
    return osErrorResource;
  }

  // Check if Thread is waiting to receive a Message
  if ((mq->thread_list != NULL) && (mq->thread_list->state == osRtxThreadWaitingMessageGet)) {
    EvrRtxMessageQueueInserted(mq, msg_ptr);
    // Wakeup waiting Thread with highest Priority
    thread = osRtxThreadListGet((os_object_t*)mq);
    osRtxThreadWaitExit(thread, (uint32_t)osOK, true);
    // Copy Message (R2: void *msg_ptr, R3: uint8_t *msg_prio)
    reg = osRtxThreadRegPtr(thread);
    memcpy((void *)reg[2], msg_ptr, mq->msg_size);
    if (reg[3] != 0U) {
      *((uint8_t *)reg[3]) = msg_prio;
    }
    EvrRtxMessageQueueRetrieved(mq, (void *)reg[2]);
    return osOK;
  }

  // Try to allocate memory
  msg = osRtxMemoryPoolAlloc(&mq->mp_info);
  if (msg != NULL) {
    // Copy Message
    memcpy((uint8_t *)msg + sizeof(os_message_t), msg_ptr, mq->msg_size);
    // Put Message into Queue
    msg->id       = osRtxIdMessage;
    msg->state    = osRtxObjectActive;
    msg->flags    = 0U;
    msg->priority = msg_prio;
    MessageQueuePut(mq, msg);
  } else {
    // No memory available
    if (timeout != 0U) {
      EvrRtxMessageQueuePutPending(mq, msg_ptr, timeout);
      // Suspend current Thread
      osRtxThreadListPut((os_object_t*)mq, osRtxThreadGetRunning());
      osRtxThreadWaitEnter(osRtxThreadWaitingMessagePut, timeout);
      // Save arguments (R2: const void *msg_ptr, R3: uint8_t msg_prio)
      reg = (uint32_t *)(__get_PSP());
      reg[2] = (uint32_t)msg_ptr;
      reg[3] = (uint32_t)msg_prio;
      return osErrorTimeout;
    } else {
      EvrRtxMessageQueueNotInserted(mq, msg_ptr);
      return osErrorResource;
    }
  }

  EvrRtxMessageQueueInserted(mq, msg_ptr);

  return osOK;
}

/// Get a Message from a Queue or timeout if Queue is empty.
/// \note API identical to osMessageQueueGet
osStatus_t svcRtxMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;
  os_message_t       *msg;
  os_thread_t        *thread;
  uint32_t           *reg;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue) || (msg_ptr == NULL)) {
    EvrRtxMessageQueueError(mq, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueError(mq, osErrorResource);
    return osErrorResource;
  }

  // Get Message from Queue
  msg = MessageQueueGet(mq);
  if (msg != NULL) {
    MessageQueueRemove(mq, msg);
    // Copy Message
    memcpy(msg_ptr, (uint8_t *)msg + sizeof(os_message_t), mq->msg_size);
    if (msg_prio != NULL) {
      *msg_prio = msg->priority;
    }
    EvrRtxMessageQueueRetrieved(mq, msg_ptr);
    // Free memory
    msg->state = osRtxObjectInactive;
    osRtxMemoryPoolFree(&mq->mp_info, msg);
  } else {
    // No Message available
    if (timeout != 0U) {
      EvrRtxMessageQueueGetPending(mq, msg_ptr, timeout);
      // Suspend current Thread
      osRtxThreadListPut((os_object_t*)mq, osRtxThreadGetRunning());
      osRtxThreadWaitEnter(osRtxThreadWaitingMessageGet, timeout);
      // Save arguments (R2: void *msg_ptr, R3: uint8_t *msg_prio)
      reg = (uint32_t *)(__get_PSP());
      reg[2] = (uint32_t)msg_ptr;
      reg[3] = (uint32_t)msg_prio;
      return osErrorTimeout;
    } else {
      EvrRtxMessageQueueNotRetrieved(mq, msg_ptr);
      return osErrorResource;
    }
  }

  // Check if Thread is waiting to send a Message
  if ((mq->thread_list != NULL) && (mq->thread_list->state == osRtxThreadWaitingMessagePut)) {
    // Try to allocate memory
    msg = osRtxMemoryPoolAlloc(&mq->mp_info);
    if (msg != NULL) {
      // Wakeup waiting Thread with highest Priority
      thread = osRtxThreadListGet((os_object_t*)mq);
      osRtxThreadWaitExit(thread, (uint32_t)osOK, true);
      // Copy Message (R2: const void *msg_ptr, R3: uint8_t msg_prio)
      reg = osRtxThreadRegPtr(thread);
      memcpy((uint8_t *)msg + sizeof(os_message_t), (void *)reg[2], mq->msg_size);
      // Store Message into Queue
      msg->id       = osRtxIdMessage;
      msg->state    = osRtxObjectActive;
      msg->flags    = 0U;
      msg->priority = (uint8_t)reg[3];
      MessageQueuePut(mq, msg);
      EvrRtxMessageQueueInserted(mq, (void *)reg[2]);
    }
  }

  return osOK;
}

/// Get maximum number of messages in a Message Queue.
/// \note API identical to osMessageGetCapacity
uint32_t svcRtxMessageQueueGetCapacity (osMessageQueueId_t mq_id) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue)) {
    EvrRtxMessageQueueGetCapacity(mq, 0U);
    return 0U;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueGetCapacity(mq, 0U);
    return 0U;
  }

  EvrRtxMessageQueueGetCapacity(mq, mq->mp_info.max_blocks);

  return mq->mp_info.max_blocks;
}

/// Get maximum message size in a Memory Pool.
/// \note API identical to osMessageGetMsgSize
uint32_t svcRtxMessageQueueGetMsgSize (osMessageQueueId_t mq_id) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue)) {
    EvrRtxMessageQueueGetMsgSize(mq, 0U);
    return 0U;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueGetMsgSize(mq, 0U);
    return 0U;
  }

  EvrRtxMessageQueueGetMsgSize(mq, mq->msg_size);

  return mq->msg_size;
}

/// Get number of queued messages in a Message Queue.
/// \note API identical to osMessageGetCount
uint32_t svcRtxMessageQueueGetCount (osMessageQueueId_t mq_id) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue)) {
    EvrRtxMessageQueueGetCount(mq, 0U);
    return 0U;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueGetCount(mq, 0U);
    return 0U;
  }

  EvrRtxMessageQueueGetCount(mq, mq->msg_count);

  return mq->msg_count;
}

/// Get number of available slots for messages in a Message Queue.
/// \note API identical to osMessageGetSpace
uint32_t svcRtxMessageQueueGetSpace (osMessageQueueId_t mq_id) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue)) {
    EvrRtxMessageQueueGetSpace(mq, 0U);
    return 0U;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueGetSpace(mq, 0U);
    return 0U;
  }

  EvrRtxMessageQueueGetSpace(mq, mq->mp_info.max_blocks - mq->msg_count);

  return (mq->mp_info.max_blocks - mq->msg_count);
}

/// Reset a Message Queue to initial empty state.
/// \note API identical to osMessageQueueReset
osStatus_t svcRtxMessageQueueReset (osMessageQueueId_t mq_id) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;
  os_message_t       *msg;
  os_thread_t        *thread;
  uint32_t           *reg;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue)) {
    EvrRtxMessageQueueError(mq, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueError(mq, osErrorResource);
    return osErrorResource;
  }

  // Remove Messages from Queue
  for (;;) {
    // Get Message from Queue
    msg = MessageQueueGet(mq);
    if (msg == NULL) {
      break;
    }
    MessageQueueRemove(mq, msg);
    EvrRtxMessageQueueRetrieved(mq, NULL);
    // Free memory
    msg->state = osRtxObjectInactive;
    osRtxMemoryPoolFree(&mq->mp_info, msg);
  }

  // Check if Threads are waiting to send Messages
  if ((mq->thread_list != NULL) && (mq->thread_list->state == osRtxThreadWaitingMessagePut)) {
    do {
      // Try to allocate memory
      msg = osRtxMemoryPoolAlloc(&mq->mp_info);
      if (msg != NULL) {
        // Wakeup waiting Thread with highest Priority
        thread = osRtxThreadListGet((os_object_t*)mq);
        osRtxThreadWaitExit(thread, (uint32_t)osOK, false);
        // Copy Message (R2: const void *msg_ptr, R3: uint8_t msg_prio)
        reg = osRtxThreadRegPtr(thread);
        memcpy((uint8_t *)msg + sizeof(os_message_t), (void *)reg[2], mq->msg_size);
        // Store Message into Queue
        msg->id       = osRtxIdMessage;
        msg->state    = osRtxObjectActive;
        msg->flags    = 0U;
        msg->priority = (uint8_t)reg[3];
        MessageQueuePut(mq, msg);
        EvrRtxMessageQueueInserted(mq, (void *)reg[2]);
      }
    } while ((msg != NULL) && (mq->thread_list != NULL));
    osRtxThreadDispatch(NULL);
  }

  EvrRtxMessageQueueResetDone(mq);

  return osOK;
}

/// Delete a Message Queue object.
/// \note API identical to osMessageQueueDelete
osStatus_t svcRtxMessageQueueDelete (osMessageQueueId_t mq_id) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;
  os_thread_t        *thread;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue)) {
    EvrRtxMessageQueueError(mq, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueError(mq, osErrorResource);
    return osErrorResource;
  }

  // Mark object as inactive
  mq->state = osRtxObjectInactive;

  // Unblock waiting threads
  if (mq->thread_list != NULL) {
    do {
      thread = osRtxThreadListGet((os_object_t*)mq);
      osRtxThreadWaitExit(thread, (uint32_t)osErrorResource, false);
    } while (mq->thread_list != NULL);
    osRtxThreadDispatch(NULL);
  }

  // Free data memory
  if (mq->flags & osRtxFlagSystemMemory) {
    osRtxMemoryFree(osRtxInfo.mem.mq_data, mq->mp_info.block_base);
  }

  // Free object memory
  if (mq->flags & osRtxFlagSystemObject) {
    if (osRtxInfo.mpi.message_queue != NULL) {
      osRtxMemoryPoolFree(osRtxInfo.mpi.message_queue, mq);
    } else {
      osRtxMemoryFree(osRtxInfo.mem.common, mq);
    }
  }

  EvrRtxMessageQueueDestroyed(mq);

  return osOK;
}


//  ==== ISR Calls ====

/// Put a Message into a Queue or timeout if Queue is full.
/// \note API identical to osMessageQueuePut
__STATIC_INLINE
osStatus_t isrRtxMessageQueuePut (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;
  os_message_t       *msg;
  const void        **ptr;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue) || (msg_ptr == NULL) || (timeout != 0U)) {
    EvrRtxMessageQueueError(mq, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueError(mq, osErrorResource);
    return osErrorResource;
  }

  // Try to allocate memory
  msg = osRtxMemoryPoolAlloc(&mq->mp_info);
  if (msg != NULL) {
    // Copy Message
    memcpy((uint8_t *)msg + sizeof(os_message_t), msg_ptr, mq->msg_size);
    msg->id       = osRtxIdMessage;
    msg->state    = osRtxObjectActive;
    msg->flags    = 0U;
    msg->priority = msg_prio;
    // Register post ISR processing
     ptr = (void *)&msg->prev;
    *ptr = msg_ptr;
     ptr = (void *)&msg->next;
    *ptr = mq;
    osRtxPostProcess((os_object_t *)msg);
  } else {
    // No memory available
    EvrRtxMessageQueueNotInserted(mq, msg_ptr);
    return osErrorResource;
  }

  EvrRtxMessageQueueInsertPending(mq, msg_ptr);

  return osOK;
}

/// Get a Message from a Queue or timeout if Queue is empty.
/// \note API identical to osMessageQueueGet
__STATIC_INLINE
osStatus_t isrRtxMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout) {
  os_message_queue_t *mq = (os_message_queue_t *)mq_id;
  os_message_t       *msg;
  void              **ptr;

  // Check parameters
  if ((mq == NULL) || (mq->id != osRtxIdMessageQueue) || (msg_ptr == NULL) || (timeout != 0U)) {
    EvrRtxMessageQueueError(mq, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (mq->state == osRtxObjectInactive) {
    EvrRtxMessageQueueError(mq, osErrorResource);
    return osErrorResource;
  }

  // Get Message from Queue
  msg = MessageQueueGet(mq);
  if (msg != NULL) {
    // Copy Message
    memcpy(msg_ptr, (uint8_t *)msg + sizeof(os_message_t), mq->msg_size);
    if (msg_prio != NULL) {
      *msg_prio = msg->priority;
    }
    EvrRtxMessageQueueRetrieved(mq, msg_ptr);
    // Register post ISR processing
     ptr = (void *)((uint8_t *)msg + sizeof(os_message_t));
    *ptr = mq;
    osRtxPostProcess((os_object_t *)msg);
  } else {
    // No Message available
    EvrRtxMessageQueueNotRetrieved(mq, msg_ptr);
    return osErrorResource;
  }

  return osOK;
}


//  ==== Public API ====

/// Create and Initialize a Message Queue object.
osMessageQueueId_t osMessageQueueNew (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr) {
  EvrRtxMessageQueueNew(msg_count, msg_size, attr);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMessageQueueError(NULL, osErrorISR);
    return NULL;
  }
  return __svcMessageQueueNew(msg_count, msg_size, attr);
}

/// Get name of a Message Queue object.
const char *osMessageQueueGetName (osMessageQueueId_t mq_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMessageQueueGetName(mq_id, NULL);
    return NULL;
  }
  return __svcMessageQueueGetName(mq_id);
}

/// Put a Message into a Queue or timeout if Queue is full.
osStatus_t osMessageQueuePut (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout) {
  EvrRtxMessageQueuePut(mq_id, msg_ptr, msg_prio, timeout);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return isrRtxMessageQueuePut(mq_id, msg_ptr, msg_prio, timeout);
  } else {
    return  __svcMessageQueuePut(mq_id, msg_ptr, msg_prio, timeout);
  }
}

/// Get a Message from a Queue or timeout if Queue is empty.
osStatus_t osMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout) {
  EvrRtxMessageQueueGet(mq_id, msg_ptr, msg_prio, timeout);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return isrRtxMessageQueueGet(mq_id, msg_ptr, msg_prio, timeout);
  } else {
    return  __svcMessageQueueGet(mq_id, msg_ptr, msg_prio, timeout);
  }
}

/// Get maximum number of messages in a Message Queue.
uint32_t osMessageQueueGetCapacity (osMessageQueueId_t mq_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxMessageQueueGetCapacity(mq_id);
  } else {
    return  __svcMessageQueueGetCapacity(mq_id);
  }
}

/// Get maximum message size in a Memory Pool.
uint32_t osMessageQueueGetMsgSize (osMessageQueueId_t mq_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxMessageQueueGetMsgSize(mq_id);
  } else {
    return  __svcMessageQueueGetMsgSize(mq_id);
  }
}

/// Get number of queued messages in a Message Queue.
uint32_t osMessageQueueGetCount (osMessageQueueId_t mq_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxMessageQueueGetCount(mq_id);
  } else {
    return  __svcMessageQueueGetCount(mq_id);
  }
}

/// Get number of available slots for messages in a Message Queue.
uint32_t osMessageQueueGetSpace (osMessageQueueId_t mq_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    return svcRtxMessageQueueGetSpace(mq_id);
  } else {
    return  __svcMessageQueueGetSpace(mq_id);
  }
}

/// Reset a Message Queue to initial empty state.
osStatus_t osMessageQueueReset (osMessageQueueId_t mq_id) {
  EvrRtxMessageQueueReset(mq_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMessageQueueError(mq_id, osErrorISR);
    return osErrorISR;
  }
  return __svcMessageQueueReset(mq_id);
}

/// Delete a Message Queue object.
osStatus_t osMessageQueueDelete (osMessageQueueId_t mq_id) {
  EvrRtxMessageQueueDelete(mq_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxMessageQueueError(mq_id, osErrorISR);
    return osErrorISR;
  }
  return __svcMessageQueueDelete(mq_id);
}
