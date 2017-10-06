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

#include "cmsis_os2.h"
#include "uvisor-lib/uvisor-lib.h"

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <reent.h>

/*
 * These are the C standard memory functions:
 * - void *calloc(size_t nmemb, size_t size);
 * - void free(void *ptr);
 * - void *malloc(size_t size);
 * - void *realloc(void *ptr, size_t size);
*/

/* Use printf with caution inside malloc: printf may allocate memory itself,
   so using printf in malloc may lead to recursive calls! */
#define DPRINTF(...) {};

extern RtxBoxIndex * const __uvisor_ps;

/** @retval 0 The kernel is not initialized.
 *  @retval 1 The kernel is initialized.. */
static int is_kernel_initialized()
{
    /* TODO: Bare-bone boxes must not call any RTX2 functions for now.
     * Each box should instead provide `heap_lock` and `heap_unlock` functions
     * as part of the box context. These would just be empty for boxes without
     * the need for heap locking. */
    if (__uvisor_ps->index.box_id_self != 0) {
        return 0;
    }

    static uint8_t kernel_running = 0;
    if (kernel_running) {
        return 1;
    }
    if (osKernelGetState() == osKernelRunning) {
        kernel_running = 1;
        return 1;
    }
    return 0;
}

static int init_allocator()
{
    int ret = 0;
    if (__uvisor_ps == NULL) {
#if defined(UVISOR_PRESENT) && (UVISOR_PRESENT == 1)
        return -1;
#else
        extern void secure_malloc_init(void);
        secure_malloc_init();
#endif
    }

    if ((__uvisor_ps->mutex_id == NULL) && is_kernel_initialized()) {
        /* Point the mutex attr to the data. */
        __uvisor_ps->mutex_attr.name = "uvisor_malloc_mutex";
        __uvisor_ps->mutex_attr.attr_bits = 0; /* Non-recursive */
        __uvisor_ps->mutex_attr.cb_mem = &__uvisor_ps->mutex_data;
        __uvisor_ps->mutex_attr.cb_size = sizeof(__uvisor_ps->mutex_data);

        /* Create mutex if not already done. */
        __uvisor_ps->mutex_id = osMutexNew(&__uvisor_ps->mutex_attr);
        /* Mutex failed to be created. */
        if (__uvisor_ps->mutex_id == NULL) {
            return -1;
        }
    }

    if (__uvisor_ps->index.active_heap == NULL) {
        /* We need to initialize the process heap. */
        if ((void *) __uvisor_ps->index.bss.address_of.heap != NULL) {
            /* Lock the mutex during initialization. */
            int kernel_initialized = is_kernel_initialized();
            if (kernel_initialized) {
                osMutexAcquire(__uvisor_ps->mutex_id, osWaitForever);
            }
            /* Initialize the process heap. */
            SecureAllocator allocator = secure_allocator_create_with_pool(
                (void *) __uvisor_ps->index.bss.address_of.heap,
                __uvisor_ps->index.box_heap_size);
            /* Set the allocator. */
            ret = allocator ? 0 : -1;
            __uvisor_ps->index.active_heap = allocator;
            /* Release the mutex. */
            if (kernel_initialized) {
                osMutexRelease(__uvisor_ps->mutex_id);
            }
        }
        else {
            DPRINTF("uvisor_allocator: No process heap available!\n");
            ret = -1;
        }
    }
    return ret;
}

typedef enum {
    MEMOP_MALLOC,
    MEMOP_MEMALIGN,
    MEMOP_CALLOC,
    MEMOP_REALLOC,
    MEMOP_FREE
} MemoryOperation;


static void * memory(MemoryOperation operation, uint32_t * args)
{
    /* Buffer the return value. */
    void * ret = NULL;
    /* Initialize allocator. */
    if (init_allocator()) {
        return NULL;
    }
    /* Check if we need to aquire the mutex. */
    int mutexed = is_kernel_initialized();
    void * allocator = __uvisor_ps->index.active_heap;

    /* Aquire the mutex if required.
     * TODO: Mutex use is very coarse here. It may be sufficient to guard
     * the `rt_alloc_mem` and `rt_free_mem` functions in `uvisor_allocator.c`.
     * However, it is simpler to do it here for now. */
    if (mutexed) {
        osMutexAcquire(__uvisor_ps->mutex_id, osWaitForever);
    }
    /* Perform the required operation. */
    switch(operation)
    {
        case MEMOP_MALLOC:
            ret = secure_malloc(allocator, (size_t) args[0]);
            break;
        case MEMOP_MEMALIGN:
            ret = secure_aligned_alloc(allocator, (size_t) args[0], (size_t) args[1]);
            break;
        case MEMOP_CALLOC:
            ret = secure_calloc(allocator, (size_t) args[0], (size_t) args[1]);
            break;
        case MEMOP_REALLOC:
            ret = secure_realloc(allocator, (void *) args[0], (size_t) args[1]);
            break;
        case MEMOP_FREE:
            secure_free(allocator, (void *) args[0]);
            break;
        default:
            break;
    }
    /* Release the mutex if required. */
    if (mutexed) {
        osMutexRelease(__uvisor_ps->mutex_id);
    }
    return ret;
}

/* Wrapped memory management functions. */
#if defined (__GNUC__)

void * __wrap__malloc_r(struct _reent * r, size_t size) {
    (void) r;
    return memory(MEMOP_MALLOC, (uint32_t *) &size);
}
void * __wrap__memalign_r(struct _reent * r, size_t alignment, size_t bytes) {
    (void) r;
    uint32_t args[2] = {(uint32_t) alignment, (uint32_t) bytes};
    return memory(MEMOP_MEMALIGN, args);
}
void * __wrap__calloc_r(struct _reent * r, size_t nmemb, size_t size) {
    (void) r;
    uint32_t args[2] = {(uint32_t) nmemb, (uint32_t) size};
    return memory(MEMOP_CALLOC, args);
}
void * __wrap__realloc_r(struct _reent * r, void * ptr, size_t size) {
    (void) r;
    uint32_t args[2] = {(uint32_t) ptr, (uint32_t) size};
    return memory(MEMOP_REALLOC, args);
}
void __wrap__free_r(struct _reent * r, void * ptr) {
    (void) r;
    memory(MEMOP_FREE, (uint32_t *) &ptr);
}

#elif defined (__CC_ARM)
/* TODO: Find out how to do function wrapping for ARMCC. See microlib libc. */
#   warning "Using uVisor allocator is not available for ARMCC. Falling back to default allocator."
#elif defined (__ICCARM__)
/* TODO: Find out how to do function wrapping for IARCC. */
#   warning "Using uVisor allocator is not available for IARCC. Falling back to default allocator."
#endif
