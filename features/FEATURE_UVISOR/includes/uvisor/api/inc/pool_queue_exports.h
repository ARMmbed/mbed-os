/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef UVISOR_POOL_QUEUE_EXPORTS_H
#define UVISOR_POOL_QUEUE_EXPORTS_H

#include "api/inc/magic_exports.h"
#include "api/inc/uvisor_exports.h"
#include "api/inc/uvisor_semaphore_exports.h"
#include "api/inc/uvisor_spinlock_exports.h"
#include <stdint.h>
#include <stddef.h>

#define UVISOR_POOL_QUEUE_NON_BLOCKING (0)
#define UVISOR_POOL_QUEUE_BLOCKING (1)

#define UVISOR_POOL_SLOT_INVALID     ((uint8_t) 0xFFU)
#define UVISOR_POOL_SLOT_IS_DEQUEUED ((uint8_t) 0xFEU)
#define UVISOR_POOL_SLOT_IS_FREE     ((uint8_t) 0xFDU)
#define UVISOR_POOL_MAX_VALID        ((uint8_t) 0xFCU)

typedef uint8_t uvisor_pool_slot_t;

typedef struct uvisor_pool_queue_entry {
    union {
        struct {
            /* The next slot in the queue */
            uvisor_pool_slot_t next;

            /* The previous slot in the queue */
            uvisor_pool_slot_t prev;
        } queued;
        struct {
            /* If the slot is free, the next available slot in the free list */
            uvisor_pool_slot_t next;

            /* If the slot is free or dequeued */
            uvisor_pool_slot_t state;
        } dequeued;
    };
} uvisor_pool_queue_entry_t;

/* These are assumed to only be statically allocated, so the management array
 * in in-place. */
typedef struct uvisor_pool {
    /* Magic that identifies this as a uvisor_pool type. */
    uint32_t magic;

    /* The array holds slots of data. */
    void const * array;

    /* The distance between elements in the array. */
    size_t stride;

    /* The maximum number of elements that could be in the array. */
    uvisor_pool_slot_t num;

    /* Whether or not the queue can block callers who want to allocate slots
     * from the pool. If non-zero, when no slots is available in the pool,
     * callers will be blocked up to their timeout amount of time before giving
     * up. */
    int blocking;

    /* The number of items currently allocated from the pool. For testing and
     * debug purposes only. */
    uvisor_pool_slot_t num_allocated;

    /* The first free slot. */
    uvisor_pool_slot_t first_free;

    /* The semaphore is used to block allocations when the pool is full. */
    UvisorSemaphore semaphore;

    /* The spinlock serializes updates to the management array. */
    UvisorSpinlock spinlock;

    /* This must be at the end so we can allocate memory for pools by
     * allocating enough room for the size of the pool appended by an array of
     * entries. */
    uvisor_pool_queue_entry_t management_array[];
} uvisor_pool_t;

typedef struct uvisor_pool_queue {
    /* Magic that identifies this as a uvisor_pool_queue type. */
    uint32_t magic;

    /* The first allocated slot */
    uvisor_pool_slot_t head;

    /* The last allocated slot */
    uvisor_pool_slot_t tail;

    uvisor_pool_t * pool;
} uvisor_pool_queue_t;

/* Intialize a pool.
 * Return 0 on success, non-zero otherwise. */
UVISOR_EXTERN int uvisor_pool_init(uvisor_pool_t * pool, void * array, size_t stride, size_t num, int blocking);

/* Initialize a pool queue.
 * Return 0 on success, non-zero otherwise. */
UVISOR_EXTERN int uvisor_pool_queue_init(uvisor_pool_queue_t * pool_queue, uvisor_pool_t * pool, void * array, size_t stride, size_t num, int blocking);

/* Allocate a slot from the pool. If the pool has no more slots available,
 * block up to the specified length of time in milliseconds. No blocking will
 * occur if the timeout is zero or the pool was initialized as non-blocking.
 * This doesn't put anything in the slot for you. It's up to you to do that.
 * Return the index of the allocated slot, or UVISOR_POOL_SLOT_INVALID if
 * timed out waiting for an available slot. This function will spin until the
 * spin lock serializing access to the pool can be taken. */
UVISOR_EXTERN uvisor_pool_slot_t uvisor_pool_allocate(uvisor_pool_t * pool, uint32_t timeout_ms);
/* Attempt to allocate a slot. This function will fail if the spin lock
 * serializing access to the pool can not be taken. */
UVISOR_EXTERN uvisor_pool_slot_t uvisor_pool_try_allocate(uvisor_pool_t * pool);

/* Enqueue the specified slot into the queue. */
UVISOR_EXTERN void uvisor_pool_queue_enqueue(uvisor_pool_queue_t * pool_queue, uvisor_pool_slot_t slot);
UVISOR_EXTERN int uvisor_pool_queue_try_enqueue(uvisor_pool_queue_t * pool_queue, uvisor_pool_slot_t slot);

/* Free the specified slot back into the pool. Invalid slots are ignored.
 * Return the slot that was freed, or UVISOR_POOL_SLOT_IS_FREE if the slot was
 * already freed, or UVISOR_POOL_SLOT_INVALID if the slot being requested to
 * free is outside the range of the queue. */
UVISOR_EXTERN uvisor_pool_slot_t uvisor_pool_free(uvisor_pool_t * pool, uvisor_pool_slot_t slot);
UVISOR_EXTERN uvisor_pool_slot_t uvisor_pool_try_free(uvisor_pool_t * pool, uvisor_pool_slot_t slot);

/* Remove the specified slot from the queue. This function does not free the
 * specified slot back into the pool. Return the slot that was dequeued, or
 * UVISOR_POOL_SLOT_IS_DEQUEUED if the slot was already dequeued, or
 * UVISOR_POOL_SLOT_INVALID if the slot being requested to dequeue is outside
 * the range of the queue. */
UVISOR_EXTERN uvisor_pool_slot_t uvisor_pool_queue_dequeue(uvisor_pool_queue_t * pool_queue, uvisor_pool_slot_t slot);

/* Remove the first slot from the queue. This function does not free the
 * specified slot back into the pool. Return the slot that was dequeued or
 * UVISOR_POOL_SLOT_INVALID if the slot being requested to dequeue is outside
 * the range of the queue. */
UVISOR_EXTERN uvisor_pool_slot_t uvisor_pool_queue_dequeue_first(uvisor_pool_queue_t * pool_queue);
UVISOR_EXTERN uvisor_pool_slot_t uvisor_pool_queue_try_dequeue_first(uvisor_pool_queue_t * pool_queue);

/* Find the first (in queue order) slot that the supplied query function
 * returns non-zero for. The query function is provided with `context` on every
 * invocation. This allows query functions to access additional data without
 * having to use global variables. `uvisor_pool_queue_find_first` is reentrant. */
typedef int (*TQueryFN_Ptr)(uvisor_pool_slot_t slot, void * context);
UVISOR_EXTERN uvisor_pool_slot_t uvisor_pool_queue_find_first(uvisor_pool_queue_t * pool_queue,
                                                              TQueryFN_Ptr query_fn, void * context);

/* Inline helper function to make allocating slots for pool queues easier and
 * better encapsulated (clients don't need to pull the pool out of the pool
 * queue, or even realize pool_queue is implemented with a pool) */
static inline uvisor_pool_slot_t uvisor_pool_queue_allocate(uvisor_pool_queue_t * pool_queue, uint32_t timeout_ms)
{
    return uvisor_pool_allocate(pool_queue->pool, timeout_ms);
}

static inline uvisor_pool_slot_t uvisor_pool_queue_try_allocate(uvisor_pool_queue_t * pool_queue)
{
    return uvisor_pool_try_allocate(pool_queue->pool);
}

/* Inline helper function to make freeing slots for pool queues easier and
 * better encapsulated (clients don't need to pull the pool out of the pool
 * queue, or even realize pool_queue is implemented with a pool) */
static inline uvisor_pool_slot_t uvisor_pool_queue_free(uvisor_pool_queue_t * pool_queue, uvisor_pool_slot_t slot)
{
    return uvisor_pool_free(pool_queue->pool, slot);
}

static inline uvisor_pool_slot_t uvisor_pool_queue_try_free(uvisor_pool_queue_t * pool_queue, uvisor_pool_slot_t slot)
{
    return uvisor_pool_try_free(pool_queue->pool, slot);
}

/* Return a pointer to the specified slot within the pool. */
static inline void * uvisor_pool_pointer_to(uvisor_pool_t * pool, uvisor_pool_slot_t slot)
{
    if (slot >= pool->num) {
        return NULL;
    }
    return (uint8_t *) pool->array + pool->stride * slot;
}

#endif
