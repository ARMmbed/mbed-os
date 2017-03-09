/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
  activated by defining the MBED_HEAP_STATS_ENABLED macro.
- the second can be used to trace each memory call by automatically invoking
  a callback on each memory operation (see hal/api/mbed_mem_trace.h). It is
  activated by defining the MBED_MEM_TRACING_ENABLED macro.

Both tracers can be activated and deactivated in any combination. If both tracers
are active, the second one (MBED_MEM_TRACING_ENABLED) will trace the first one's
(MBED_HEAP_STATS_ENABLED) memory calls.*/

/******************************************************************************/
/* Implementation of the runtime max heap usage checker                       */
/******************************************************************************/

/* Size must be a multiple of 8 to keep alignment */
typedef struct {
    uint32_t size;
    uint32_t pad;
} alloc_info_t;

#ifdef MBED_MEM_TRACING_ENABLED
static SingletonPtr<PlatformMutex> mem_trace_mutex;
#endif
#ifdef MBED_HEAP_STATS_ENABLED
static SingletonPtr<PlatformMutex> malloc_stats_mutex;
static mbed_stats_heap_t heap_stats = {0, 0, 0, 0, 0};
#endif

void mbed_stats_heap_get(mbed_stats_heap_t *stats)
{
#ifdef MBED_HEAP_STATS_ENABLED
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

#ifdef   FEATURE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif/* FEATURE_UVISOR */

extern "C" {
    void * __real__malloc_r(struct _reent * r, size_t size);
    void * __real__realloc_r(struct _reent * r, void * ptr, size_t size);
    void __real__free_r(struct _reent * r, void * ptr);
    void* __real__calloc_r(struct _reent * r, size_t nmemb, size_t size);
}

// TODO: memory tracing doesn't work with uVisor enabled.
#if !defined(FEATURE_UVISOR)

extern "C" void * __wrap__malloc_r(struct _reent * r, size_t size) {
    void *ptr = NULL;
#ifdef MBED_HEAP_STATS_ENABLED
    malloc_stats_mutex->lock();
    alloc_info_t *alloc_info = (alloc_info_t*)__real__malloc_r(r, size + sizeof(alloc_info_t));
    if (alloc_info != NULL) {
        alloc_info->size = size;
        ptr = (void*)(alloc_info + 1);
        heap_stats.current_size += size;
        heap_stats.total_size += size;
        heap_stats.alloc_cnt += 1;
        if (heap_stats.current_size > heap_stats.max_size) {
            heap_stats.max_size = heap_stats.current_size;
        }
    } else {
        heap_stats.alloc_fail_cnt += 1;
    }
    malloc_stats_mutex->unlock();
#else // #ifdef MBED_HEAP_STATS_ENABLED
    ptr = __real__malloc_r(r, size);
#endif // #ifdef MBED_HEAP_STATS_ENABLED
#ifdef MBED_MEM_TRACING_ENABLED
    mem_trace_mutex->lock();
    mbed_mem_trace_malloc(ptr, size, MBED_CALLER_ADDR());
    mem_trace_mutex->unlock();
#endif // #ifdef MBED_MEM_TRACING_ENABLED
    return ptr;
}

extern "C" void * __wrap__realloc_r(struct _reent * r, void * ptr, size_t size) {
    void *new_ptr = NULL;
#ifdef MBED_HEAP_STATS_ENABLED
    // Implement realloc_r with malloc and free.
    // The function realloc_r can't be used here directly since
    // it can call into __wrap__malloc_r (returns ptr + 4) or
    // resize memory directly (returns ptr + 0).

    // Note - no lock needed since malloc and free are thread safe

    // Get old size
    uint32_t old_size = 0;
    if (ptr != NULL) {
        alloc_info_t *alloc_info = ((alloc_info_t*)ptr) - 1;
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
        memcpy(new_ptr, (void*)ptr, copy_size);
        free(ptr);
    }
#else // #ifdef MBED_HEAP_STATS_ENABLED
    new_ptr = __real__realloc_r(r, ptr, size);
#endif // #ifdef MBED_HEAP_STATS_ENABLED
#ifdef MBED_MEM_TRACING_ENABLED
    mem_trace_mutex->lock();
    mbed_mem_trace_realloc(new_ptr, ptr, size, MBED_CALLER_ADDR());
    mem_trace_mutex->unlock();
#endif // #ifdef MBED_MEM_TRACING_ENABLED
    return new_ptr;
}

extern "C" void __wrap__free_r(struct _reent * r, void * ptr) {
#ifdef MBED_HEAP_STATS_ENABLED
    malloc_stats_mutex->lock();
    alloc_info_t *alloc_info = NULL;
    if (ptr != NULL) {
        alloc_info = ((alloc_info_t*)ptr) - 1;
        heap_stats.current_size -= alloc_info->size;
        heap_stats.alloc_cnt -= 1;
    }
    __real__free_r(r, (void*)alloc_info);
    malloc_stats_mutex->unlock();
#else // #ifdef MBED_HEAP_STATS_ENABLED
    __real__free_r(r, ptr);
#endif // #ifdef MBED_HEAP_STATS_ENABLED
#ifdef MBED_MEM_TRACING_ENABLED
    mem_trace_mutex->lock();
    mbed_mem_trace_free(ptr, MBED_CALLER_ADDR());
    mem_trace_mutex->unlock();
#endif // #ifdef MBED_MEM_TRACING_ENABLED
}

#endif // if !defined(FEATURE_UVISOR)

extern "C" void * __wrap__calloc_r(struct _reent * r, size_t nmemb, size_t size) {
    void *ptr = NULL;
#ifdef MBED_HEAP_STATS_ENABLED
    // Note - no lock needed since malloc is thread safe

    ptr = malloc(nmemb * size);
    if (ptr != NULL) {
        memset(ptr, 0, nmemb * size);
    }
#else // #ifdef MBED_HEAP_STATS_ENABLED
    ptr = __real__calloc_r(r, nmemb, size);
#endif // #ifdef MBED_HEAP_STATS_ENABLED
#ifdef MBED_MEM_TRACING_ENABLED
    mem_trace_mutex->lock();
    mbed_mem_trace_calloc(ptr, nmemb, size, MBED_CALLER_ADDR());
    mem_trace_mutex->unlock();
#endif // #ifdef MBED_MEM_TRACING_ENABLED
    return ptr;
}


/******************************************************************************/
/* ARMCC memory allocation wrappers                                           */
/******************************************************************************/

#elif defined(TOOLCHAIN_ARM) // #if defined(TOOLCHAIN_GCC)

/* Enable hooking of memory function only if tracing is also enabled */
#if defined(MBED_MEM_TRACING_ENABLED) || defined(MBED_HEAP_STATS_ENABLED)

extern "C" {
    void *$Super$$malloc(size_t size);
    void *$Super$$realloc(void *ptr, size_t size);
    void *$Super$$calloc(size_t nmemb, size_t size);
    void $Super$$free(void *ptr);
}

extern "C" void* $Sub$$malloc(size_t size) {
    void *ptr = NULL;
#ifdef MBED_HEAP_STATS_ENABLED
    malloc_stats_mutex->lock();
    alloc_info_t *alloc_info = (alloc_info_t*)$Super$$malloc(size + sizeof(alloc_info_t));
    if (alloc_info != NULL) {
        alloc_info->size = size;
        ptr = (void*)(alloc_info + 1);
        heap_stats.current_size += size;
        heap_stats.total_size += size;
        heap_stats.alloc_cnt += 1;
        if (heap_stats.current_size > heap_stats.max_size) {
            heap_stats.max_size = heap_stats.current_size;
        }
    } else {
        heap_stats.alloc_fail_cnt += 1;
    }
    malloc_stats_mutex->unlock();
#else // #ifdef MBED_HEAP_STATS_ENABLED
    ptr = $Super$$malloc(size);
#endif // #ifdef MBED_HEAP_STATS_ENABLED
#ifdef MBED_MEM_TRACING_ENABLED
    mem_trace_mutex->lock();
    mbed_mem_trace_malloc(ptr, size, MBED_CALLER_ADDR());
    mem_trace_mutex->unlock();
#endif // #ifdef MBED_MEM_TRACING_ENABLED
    return ptr;
}

extern "C" void* $Sub$$realloc(void *ptr, size_t size) {
    void *new_ptr = NULL;
#ifdef MBED_HEAP_STATS_ENABLED
    // Note - no lock needed since malloc and free are thread safe

    // Get old size
    uint32_t old_size = 0;
    if (ptr != NULL) {
        alloc_info_t *alloc_info = ((alloc_info_t*)ptr) - 1;
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
        memcpy(new_ptr, (void*)ptr, copy_size);
        free(ptr);
    }
#else // #ifdef MBED_HEAP_STATS_ENABLED
    new_ptr = $Super$$realloc(ptr, size);
#endif // #ifdef MBED_HEAP_STATS_ENABLED
#ifdef MBED_MEM_TRACING_ENABLED
    mem_trace_mutex->lock();
    mbed_mem_trace_realloc(new_ptr, ptr, size, MBED_CALLER_ADDR());
    mem_trace_mutex->unlock();
#endif // #ifdef MBED_MEM_TRACING_ENABLED
    return new_ptr;
}

extern "C" void *$Sub$$calloc(size_t nmemb, size_t size) {
    void *ptr = NULL;
#ifdef MBED_HEAP_STATS_ENABLED
    // Note - no lock needed since malloc is thread safe
    ptr = malloc(nmemb * size);
    if (ptr != NULL) {
        memset(ptr, 0, nmemb * size);
    }
#else // #ifdef MBED_HEAP_STATS_ENABLED
    ptr = $Super$$calloc(nmemb, size);
#endif // #ifdef MBED_HEAP_STATS_ENABLED
#ifdef MBED_MEM_TRACING_ENABLED
    mem_trace_mutex->lock();
    mbed_mem_trace_calloc(ptr, nmemb, size, MBED_CALLER_ADDR());
    mem_trace_mutex->unlock();
#endif // #ifdef MBED_MEM_TRACING_ENABLED
    return ptr;
}

extern "C" void $Sub$$free(void *ptr) {
#ifdef MBED_HEAP_STATS_ENABLED
    malloc_stats_mutex->lock();
    alloc_info_t *alloc_info = NULL;
    if (ptr != NULL) {
        alloc_info = ((alloc_info_t*)ptr) - 1;
        heap_stats.current_size -= alloc_info->size;
        heap_stats.alloc_cnt -= 1;
    }
    $Super$$free((void*)alloc_info);
    malloc_stats_mutex->unlock();
#else // #ifdef MBED_HEAP_STATS_ENABLED
    $Super$$free(ptr);
#endif // #ifdef MBED_HEAP_STATS_ENABLED
#ifdef MBED_MEM_TRACING_ENABLED
    mem_trace_mutex->lock();
    mbed_mem_trace_free(ptr, MBED_CALLER_ADDR());
    mem_trace_mutex->unlock();
#endif // #ifdef MBED_MEM_TRACING_ENABLED
}

#endif // #if defined(MBED_MEM_TRACING_ENABLED) || defined(MBED_HEAP_STATS_ENABLED)

/******************************************************************************/
/* Allocation wrappers for other toolchains are not supported yet             */
/******************************************************************************/

#else // #if defined(TOOLCHAIN_GCC)

#ifdef MBED_MEM_TRACING_ENABLED
#warning Memory tracing is not supported with the current toolchain.
#endif

#ifdef MBED_HEAP_STATS_ENABLED
#warning Heap statistics are not supported with the current toolchain.
#endif

#endif // #if defined(TOOLCHAIN_GCC)

