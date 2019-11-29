/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "platform/mbed_mem_trace.h"
#include "platform/mbed_stats.h"
#include "platform/mbed_toolchain.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* There are two memory tracers in mbed OS:

- the first can be used to detect the maximum heap usage at runtime. It is
  activated by setting the configuration option MBED_HEAP_STATS_ENABLED to true.
- the second can be used to trace each memory call by automatically invoking
  a callback on each memory operation (see hal/api/mbed_mem_trace.h). It is
  activated by setting the configuration option MBED_MEM_TRACING_ENABLED to true.

Both tracers can be activated and deactivated in any combination. If both tracers
are active, the second one (MBED_MEM_TRACING_ENABLED) will trace the first one's
(MBED_HEAP_STATS_ENABLED) memory calls.*/

/******************************************************************************/
/* Implementation of the runtime max heap usage checker                       */
/******************************************************************************/

typedef struct {
    uint32_t size;
    uint32_t signature;
} alloc_info_t;

#if MBED_HEAP_STATS_ENABLED
#define MBED_HEAP_STATS_SIGNATURE       (0xdeadbeef)

static SingletonPtr<PlatformMutex> malloc_stats_mutex;
static mbed_stats_heap_t heap_stats = {0, 0, 0, 0, 0, 0, 0};

typedef struct  {
    size_t size;
} mbed_heap_overhead_t;

#define MALLOC_HEADER_SIZE          (sizeof(mbed_heap_overhead_t))
#define MALLOC_HEADER_PTR(p)        (mbed_heap_overhead_t *)((char *)(p) - MALLOC_HEADER_SIZE)
#define MALLOC_HEAP_TOTAL_SIZE(p)   (((p)->size) & (~0x1))
#endif

void mbed_stats_heap_get(mbed_stats_heap_t *stats)
{
#if MBED_HEAP_STATS_ENABLED
    extern uint32_t mbed_heap_size;
    heap_stats.reserved_size = mbed_heap_size;

    malloc_stats_mutex->lock();
    memcpy(stats, &heap_stats, sizeof(mbed_stats_heap_t));
    malloc_stats_mutex->unlock();
#else
    memset(stats, 0, sizeof(mbed_stats_heap_t));
#endif
}

/******************************************************************************/
/* GCC memory allocation wrappers                                             */
/******************************************************************************/

#if defined(TOOLCHAIN_GCC)

extern "C" {
    void *__real__malloc_r(struct _reent *r, size_t size);
    void *__real__memalign_r(struct _reent *r, size_t alignment, size_t bytes);
    void *__real__realloc_r(struct _reent *r, void *ptr, size_t size);
    void __real__free_r(struct _reent *r, void *ptr);
    void *__real__calloc_r(struct _reent *r, size_t nmemb, size_t size);
    void *malloc_wrapper(struct _reent *r, size_t size, void *caller);
    void free_wrapper(struct _reent *r, void *ptr, void *caller);
}


extern "C" void *__wrap__malloc_r(struct _reent *r, size_t size)
{
    return malloc_wrapper(r, size, MBED_CALLER_ADDR());
}

extern "C" void *malloc_wrapper(struct _reent *r, size_t size, void *caller)
{
    void *ptr = NULL;
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_lock();
#endif
#if MBED_HEAP_STATS_ENABLED
    malloc_stats_mutex->lock();
    alloc_info_t *alloc_info = (alloc_info_t *)__real__malloc_r(r, size + sizeof(alloc_info_t));
    if (alloc_info != NULL) {
        alloc_info->size = size;
        alloc_info->signature = MBED_HEAP_STATS_SIGNATURE;
        ptr = (void *)(alloc_info + 1);
        heap_stats.current_size += size;
        heap_stats.total_size += size;
        heap_stats.alloc_cnt += 1;
        if (heap_stats.current_size > heap_stats.max_size) {
            heap_stats.max_size = heap_stats.current_size;
        }
        heap_stats.overhead_size += MALLOC_HEAP_TOTAL_SIZE(MALLOC_HEADER_PTR(alloc_info)) - size;
    } else {
        heap_stats.alloc_fail_cnt += 1;
    }
    malloc_stats_mutex->unlock();
#else // #if MBED_HEAP_STATS_ENABLED
    ptr = __real__malloc_r(r, size);
#endif // #if MBED_HEAP_STATS_ENABLED
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_malloc(ptr, size, caller);
    mbed_mem_trace_unlock();
#endif // #if MBED_MEM_TRACING_ENABLED
    return ptr;
}

extern "C" void *__wrap__realloc_r(struct _reent *r, void *ptr, size_t size)
{
    void *new_ptr = NULL;
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_lock();
#endif
#if MBED_HEAP_STATS_ENABLED
    // Implement realloc_r with malloc and free.
    // The function realloc_r can't be used here directly since
    // it can call into __wrap__malloc_r (returns ptr + 4) or
    // resize memory directly (returns ptr + 0).

    // Note - no lock needed since malloc and free are thread safe

    // Get old size
    uint32_t old_size = 0;
    if (ptr != NULL) {
        alloc_info_t *alloc_info = ((alloc_info_t *)ptr) - 1;
        old_size = alloc_info->size;
    }

    // Allocate space
    if (size != 0) {
        new_ptr = malloc(size);
    }

    // If the new buffer has been allocated copy the data to it
    // and free the old buffer
    if (new_ptr != NULL) {
        uint32_t copy_size = (old_size < size) ? old_size : size;
        memcpy(new_ptr, (void *)ptr, copy_size);
        free(ptr);
    }
#else // #if MBED_HEAP_STATS_ENABLED
    new_ptr = __real__realloc_r(r, ptr, size);
#endif // #if MBED_HEAP_STATS_ENABLED
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_realloc(new_ptr, ptr, size, MBED_CALLER_ADDR());
    mbed_mem_trace_unlock();
#endif // #if MBED_MEM_TRACING_ENABLED
    return new_ptr;
}

extern "C" void __wrap__free_r(struct _reent *r, void *ptr)
{
    free_wrapper(r, ptr, MBED_CALLER_ADDR());
}

extern "C" void free_wrapper(struct _reent *r, void *ptr, void *caller)
{
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_lock();
#endif
#if MBED_HEAP_STATS_ENABLED
    malloc_stats_mutex->lock();
    alloc_info_t *alloc_info = NULL;
    if (ptr != NULL) {
        alloc_info = ((alloc_info_t *)ptr) - 1;
        if (MBED_HEAP_STATS_SIGNATURE == alloc_info->signature) {
            size_t user_size = alloc_info->size;
            size_t alloc_size = MALLOC_HEAP_TOTAL_SIZE(MALLOC_HEADER_PTR(alloc_info));
            alloc_info->signature = 0x0;
            heap_stats.current_size -= user_size;
            heap_stats.alloc_cnt -= 1;
            heap_stats.overhead_size -= (alloc_size - user_size);
            __real__free_r(r, (void *)alloc_info);
        } else {
            __real__free_r(r, ptr);
        }
    }

    malloc_stats_mutex->unlock();
#else // #if MBED_HEAP_STATS_ENABLED
    __real__free_r(r, ptr);
#endif // #if MBED_HEAP_STATS_ENABLED
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_free(ptr, caller);
    mbed_mem_trace_unlock();
#endif // #if MBED_MEM_TRACING_ENABLED
}

extern "C" void *__wrap__calloc_r(struct _reent *r, size_t nmemb, size_t size)
{
    void *ptr = NULL;
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_lock();
#endif
#if MBED_HEAP_STATS_ENABLED
    // Note - no lock needed since malloc is thread safe

    ptr = malloc(nmemb * size);
    if (ptr != NULL) {
        memset(ptr, 0, nmemb * size);
    }
#else // #if MBED_HEAP_STATS_ENABLED
    ptr = __real__calloc_r(r, nmemb, size);
#endif // #if MBED_HEAP_STATS_ENABLED
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_calloc(ptr, nmemb, size, MBED_CALLER_ADDR());
    mbed_mem_trace_unlock();
#endif // #if MBED_MEM_TRACING_ENABLED
    return ptr;
}

extern "C" void *__wrap__memalign_r(struct _reent *r, size_t alignment, size_t bytes)
{
    return __real__memalign_r(r, alignment, bytes);
}


/******************************************************************************/
/* ARMCC / IAR memory allocation wrappers                                     */
/******************************************************************************/

#elif defined(TOOLCHAIN_ARM) || defined(__ICCARM__)

#if defined(TOOLCHAIN_ARM)
#define SUPER_MALLOC    $Super$$malloc
#define SUB_MALLOC      $Sub$$malloc
#define SUPER_REALLOC   $Super$$realloc
#define SUB_REALLOC     $Sub$$realloc
#define SUPER_CALLOC    $Super$$calloc
#define SUB_CALLOC      $Sub$$calloc
#define SUPER_FREE      $Super$$free
#define SUB_FREE        $Sub$$free
#elif defined(__ICCARM__)
#define SUPER_MALLOC    $Super$$__iar_dlmalloc
#define SUB_MALLOC      $Sub$$__iar_dlmalloc
#define SUPER_REALLOC   $Super$$__iar_dlrealloc
#define SUB_REALLOC     $Sub$$__iar_dlrealloc
#define SUPER_CALLOC    $Super$$__iar_dlcalloc
#define SUB_CALLOC      $Sub$$__iar_dlcalloc
#define SUPER_FREE      $Super$$__iar_dlfree
#define SUB_FREE        $Sub$$__iar_dlfree
#endif

/* Enable hooking of memory function only if tracing is also enabled */
#if defined(MBED_MEM_TRACING_ENABLED) || defined(MBED_HEAP_STATS_ENABLED)

extern "C" {
    void *SUPER_MALLOC(size_t size);
    void *SUPER_REALLOC(void *ptr, size_t size);
    void *SUPER_CALLOC(size_t nmemb, size_t size);
    void SUPER_FREE(void *ptr);
    void *malloc_wrapper(size_t size, void *caller);
    void free_wrapper(void *ptr, void *caller);
}

extern "C" void *SUB_MALLOC(size_t size)
{
    return malloc_wrapper(size, MBED_CALLER_ADDR());
}

extern "C" void *malloc_wrapper(size_t size, void *caller)
{
    void *ptr = NULL;
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_lock();
#endif
#if MBED_HEAP_STATS_ENABLED
    malloc_stats_mutex->lock();
    alloc_info_t *alloc_info = (alloc_info_t *)SUPER_MALLOC(size + sizeof(alloc_info_t));
    if (alloc_info != NULL) {
        alloc_info->size = size;
        alloc_info->signature = MBED_HEAP_STATS_SIGNATURE;
        ptr = (void *)(alloc_info + 1);
        heap_stats.current_size += size;
        heap_stats.total_size += size;
        heap_stats.alloc_cnt += 1;
        if (heap_stats.current_size > heap_stats.max_size) {
            heap_stats.max_size = heap_stats.current_size;
        }
        heap_stats.overhead_size += MALLOC_HEAP_TOTAL_SIZE(MALLOC_HEADER_PTR(alloc_info)) - size;
    } else {
        heap_stats.alloc_fail_cnt += 1;
    }
    malloc_stats_mutex->unlock();
#else // #if MBED_HEAP_STATS_ENABLED
    ptr = SUPER_MALLOC(size);
#endif // #if MBED_HEAP_STATS_ENABLED
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_malloc(ptr, size, caller);
    mbed_mem_trace_unlock();
#endif // #if MBED_MEM_TRACING_ENABLED
    return ptr;
}


extern "C" void *SUB_REALLOC(void *ptr, size_t size)
{
    void *new_ptr = NULL;
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_lock();
#endif
#if MBED_HEAP_STATS_ENABLED
    // Note - no lock needed since malloc and free are thread safe

    // Get old size
    uint32_t old_size = 0;
    if (ptr != NULL) {
        alloc_info_t *alloc_info = ((alloc_info_t *)ptr) - 1;
        old_size = alloc_info->size;
    }

    // Allocate space
    if (size != 0) {
        new_ptr = malloc(size);
    }

    // If the new buffer has been allocated copy the data to it
    // and free the old buffer
    if ((new_ptr != NULL) && (ptr != NULL)) {
        uint32_t copy_size = (old_size < size) ? old_size : size;
        memcpy(new_ptr, (void *)ptr, copy_size);
        free(ptr);
    }

    {
        volatile uint8_t dummy = 0;
        if (dummy != 0) { // always false
            // this code will never be executed
            // it's just to tell the compiler/linker to preserve SUB_REALLOC symbol
            // when LTO enabled
            SUPER_REALLOC(NULL, 0);
        }
    }
#else // #if MBED_HEAP_STATS_ENABLED
    new_ptr = SUPER_REALLOC(ptr, size);
#endif // #if MBED_HEAP_STATS_ENABLED
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_realloc(new_ptr, ptr, size, MBED_CALLER_ADDR());
    mbed_mem_trace_unlock();
#endif // #if MBED_MEM_TRACING_ENABLED
    return new_ptr;
}

extern "C" void *SUB_CALLOC(size_t nmemb, size_t size)
{
    void *ptr = NULL;
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_lock();
#endif
#if MBED_HEAP_STATS_ENABLED
    // Note - no lock needed since malloc is thread safe
    ptr = malloc(nmemb * size);
    if (ptr != NULL) {
        memset(ptr, 0, nmemb * size);
    }

    {
        volatile uint8_t dummy = 0;
        if (dummy != 0) { // always false
            // this code will never be executed
            // it's just to tell the compiler/linker to preserve SUB_CALLOC symbol
            // when LTO enabled
            SUPER_CALLOC(NULL, 0);
        }
    }
#else // #if MBED_HEAP_STATS_ENABLED
    ptr = SUPER_CALLOC(nmemb, size);
#endif // #if MBED_HEAP_STATS_ENABLED
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_calloc(ptr, nmemb, size, MBED_CALLER_ADDR());
    mbed_mem_trace_unlock();
#endif // #if MBED_MEM_TRACING_ENABLED
    return ptr;
}

extern "C" void SUB_FREE(void *ptr)
{
    free_wrapper(ptr, MBED_CALLER_ADDR());
}

extern "C" void free_wrapper(void *ptr, void *caller)
{
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_lock();
#endif
#if MBED_HEAP_STATS_ENABLED
    malloc_stats_mutex->lock();
    alloc_info_t *alloc_info = NULL;
    if (ptr != NULL) {
        alloc_info = ((alloc_info_t *)ptr) - 1;
        if (MBED_HEAP_STATS_SIGNATURE == alloc_info->signature) {
            size_t user_size = alloc_info->size;
            size_t alloc_size = MALLOC_HEAP_TOTAL_SIZE(MALLOC_HEADER_PTR(alloc_info));
            alloc_info->signature = 0x0;
            heap_stats.current_size -= user_size;
            heap_stats.alloc_cnt -= 1;
            heap_stats.overhead_size -= (alloc_size - user_size);
            SUPER_FREE((void *)alloc_info);
        } else {
            SUPER_FREE(ptr);
        }
    }

    malloc_stats_mutex->unlock();
#else // #if MBED_HEAP_STATS_ENABLED
    SUPER_FREE(ptr);
#endif // #if MBED_HEAP_STATS_ENABLED
#if MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_free(ptr, caller);
    mbed_mem_trace_unlock();
#endif // #if MBED_MEM_TRACING_ENABLED
}

#endif // #if defined(MBED_MEM_TRACING_ENABLED) || defined(MBED_HEAP_STATS_ENABLED)

/******************************************************************************/
/* Allocation wrappers for other toolchains are not supported yet             */
/******************************************************************************/

#else

#if MBED_MEM_TRACING_ENABLED
#error Memory tracing is not supported with the current toolchain.
#endif

#if MBED_HEAP_STATS_ENABLED
#error Heap statistics are not supported with the current toolchain.
#endif

#endif // #if defined(TOOLCHAIN_GCC)
