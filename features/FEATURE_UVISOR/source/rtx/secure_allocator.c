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

#include "rtx_lib.h"

/* uVisor uses rtx_memory instead of implementing its own dynamic,
 * non-fixed-size memory allocator. To do this, uVisor creates multiple
 * non-fixed-size allocator pools (one per page) and allocates memory from
 * these pools. uVisor must manage the memory for these pools' control blocks,
 * so it must know the size of these control blocks. */

/* The following memory pool control block structs are copied from
 * rtx_memory.c, so that uVisor can manage the memory for these control blocks
 * within pages. */
typedef struct mem_head_s {
  uint32_t size;                // Memory Pool size
  uint32_t used;                // Used Memory
} mem_head_t;

//  Memory Block Header structure
typedef struct mem_block_s {
  struct mem_block_s *next;     // Next Memory Block in list
  uint32_t            info;     // Info: length = <31:2>:'00', type = <1:0>
} mem_block_t;
/* End copy */

#include "secure_allocator.h"
#include "uvisor-lib/uvisor-lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Use printf with caution inside malloc: printf may allocate memory itself,
   so using printf in malloc may lead to recursive calls! */
#define DPRINTF(...) {}

/* offsetof is a gcc built-in function, this is the manual implementation */
#define OFFSETOF(type, member) ((uint32_t) (&(((type *)(0))->member)))

/* Internal structure currently only contains the page table. */
typedef struct {
    UvisorPageTable table;
} SecureAllocatorInternal;

static inline UvisorPageTable * table(SecureAllocator allocator) {
    return &(((SecureAllocatorInternal *) allocator)->table);
}

SecureAllocator secure_allocator_create_with_pool(
    void * mem,
    size_t bytes)
{
    SecureAllocatorInternal * allocator = mem;
    /* Signal that this is non-page allocated memory. */
    allocator->table.page_size = bytes;
    allocator->table.page_count = 0;
    /* The internal rtx_Memory memory pool structure must be placed AFTER
     * table.page_origins[0] !!! */
    size_t offset = OFFSETOF(SecureAllocatorInternal, table.page_origins) + sizeof(((UvisorPageTable) {0}).page_origins);
    uintptr_t page_origin = (uintptr_t) mem + offset;

    /* Align page_origin to a multiple of 8 (because RTX requries 8-byte
     * alignment of the origin). */
    page_origin = (page_origin + (0x8 - 1)) & -0x8;
    offset = page_origin - (uintptr_t) mem;
    size_t size = bytes - offset;
    /* Align size to a multiple of 8 (because RTX requires 8-byte alignment of
     * the size) */
    size &= -0x8;

    /* Create pool allocator structure inside the memory. */
    if (!osRtxMemoryInit((void *) page_origin, size)) {
        /* Abort if failed. */
        DPRINTF("secure_allocator_create_with_pool: pool allocator %p with offset %d creation failed (size %u bytes)\n\n", page_origin, offset, bytes - offset);
        return NULL;
    }
    /* Remember the pool allocator pointer though. */
    allocator->table.page_origins[0] = (void *) page_origin;
    DPRINTF("secure_allocator_create_with_pool: Created pool allocator %p with offset %d\n\n", page_origin, offset);
    return allocator;
}

SecureAllocator secure_allocator_create_with_pages(
    size_t size,
    size_t maximum_malloc_size)
{
    const uint32_t page_size = uvisor_get_page_size();
    /* The rtx_Memory allocator puts one pool allocator structure at both the
     * beginning and end of the memory pool. */
    const size_t block_overhead = 2 * sizeof(mem_block_t);
    const size_t page_size_with_overhead = page_size + block_overhead;
    /* Calculate the integer part of required the page count. */
    size_t page_count = size / page_size_with_overhead;
    /* Add another page if the remainder is not zero. */
    if (size - page_count * page_size_with_overhead) {
        page_count++;
    }
    DPRINTF("secure_allocator_create_with_pages: Requesting %u pages for at least %uB\n", page_count, size);

    /* Compute the maximum allocation within our blocks. */
    size_t maximum_allocation_size = page_size - block_overhead;
    /* If the required maximum allocation is larger than we can provide, abort. */
    if (maximum_malloc_size > maximum_allocation_size) {
        DPRINTF("secure_allocator_create_with_pages: Maximum allocation request %uB is larger then available %uB\n\n", maximum_malloc_size, maximum_allocation_size);
        return NULL;
    }

    /* Compute the required memory size for the page table. */
    size_t allocator_type_size = sizeof(SecureAllocatorInternal);
    /* Add size for each additional page. */
    allocator_type_size += (page_count - 1) * sizeof(((UvisorPageTable) {0}).page_origins);
    /* Allocate this much memory. */
    SecureAllocatorInternal * const allocator = malloc(allocator_type_size);
    /* If malloc failed, abort. */
    if (allocator == NULL) {
        DPRINTF("secure_allocator_create_with_pages: SecureAllocatorInternal failed to be allocated!\n\n");
        return NULL;
    }

    /* Prepare the page table. */
    allocator->table.page_size = page_size;
    allocator->table.page_count = page_count;
    /* Get me some pages. */
    if (uvisor_page_malloc((UvisorPageTable *) &(allocator->table))) {
        free(allocator);
        DPRINTF("secure_allocator_create_with_pages: Not enough free pages available!\n\n");
        return NULL;
    }

    /* Initialize a memory pool structure in all pages. */
    for(size_t ii = 0; ii < page_count; ii++) {
        /* Add each page as a pool. */
        osStatus_t status = osRtxMemoryInit(allocator->table.page_origins[ii], page_size);
        if (status == osOK) {
          DPRINTF("secure_allocator_create_with_pages: Created memory pool allocator %p with offset %d page %u\n", allocator->table.page_origins[ii], 0, ii);
        } else {
          DPRINTF("secure_allocator_create_with_pages: Failed creating memory pool allocator %p with offset %d page %u\n", allocator->table.page_origins[ii], 0, ii);
        }
    }
    DPRINTF("\n");
    /* Aaaand across the line. */
    return (SecureAllocator) allocator;
}

int secure_allocator_destroy(
    SecureAllocator allocator)
{
    DPRINTF("secure_allocator_destroy: Destroying memory pool allocator at %p\n", table(allocator)->page_origins[0]);

    /* Check if we are working on statically allocated memory. */
    SecureAllocatorInternal * alloc = (SecureAllocatorInternal * const) allocator;
    if (alloc->table.page_count == 0) {
        DPRINTF("secure_allocator_destroy: %p is not page-backed memory, not freeing!\n", allocator);
        return -1;
    }

    /* Free all pages. */
    if (uvisor_page_free(&(alloc->table))) {
        DPRINTF("secure_allocator_destroy: Unable to free pages!\n\n");
        return -1;
    }

    /* Free the allocator structure. */
    free(allocator);

    DPRINTF("\n");
    return 0;
}

void * secure_malloc(
    SecureAllocator allocator,
    size_t size)
{
    size_t index = 0;
    do {
        /* Search in this page. */
        void * mem = osRtxMemoryAlloc(table(allocator)->page_origins[index], size, 0);
        /* Return if we found something. */
        if (mem) {
            DPRINTF("secure_malloc: Found %4uB in page %u at %p\n", size, index, mem);
            return mem;
        }
        /* Otherwise, go to the next page. */
        index++;
    } /* Continue search if more pages are available. */
    while (index < table(allocator)->page_count);

    DPRINTF("secure_malloc: Out of memory in allocator %p \n", allocator);
    /* We found nothing. */
    return NULL;
}

void * secure_aligned_alloc(
    SecureAllocator allocator,
    size_t alignment,
    size_t size)
{
    /* Alignment must be a power of two! */
    if (alignment & ((1UL << ((31UL - __builtin_clz(alignment)) - 1)))) {
        return NULL;
    }
    /* TODO: THIS IS A NAIVE IMPLEMENTATION, which wastes much memory. */
    void * ptr = secure_malloc(allocator, size + alignment - 1);
    if (ptr == NULL) {
        return NULL;
    }
    return (void *) (((uint32_t) ptr + alignment - 1) & ~(alignment - 1));
}

void * secure_calloc(
    SecureAllocator allocator,
    size_t nmemb,
    size_t size)
{
    if ((uint64_t) nmemb * size > SIZE_MAX) {
        /* (size * nmemb) has overflowed. */
        return NULL;
    }
    void * ptr = secure_malloc(allocator, size * nmemb);
    if (ptr == NULL) {
        return NULL;
    }
    memset(ptr, 0, size * nmemb);
    return ptr;
}

void * secure_realloc(
    SecureAllocator allocator,
    void * ptr,
    size_t new_size)
{
    /* TODO: THIS IS A NAIVE IMPLEMENTATION, which always allocates new
       memory, and copies the memory, then frees the old memory. */

    /* Allocate new memory. */
    void * new_ptr = secure_malloc(allocator, new_size);
    /* If memory allocation failed, abort. */
    if (new_ptr == NULL) {
        return NULL;
    }

    /* Passing NULL as ptr is legal, realloc acts as malloc then. */
    if (ptr) {
        /* Get the size of the ptr memory. */
        size_t size = ((mem_block_t *) ((uint32_t) ptr - sizeof(mem_block_t)))->info & ~0x3;
        /* Copy the memory to the new location, min(new_size, size). */
        memcpy(new_ptr, ptr, new_size < size ? new_size : size);
        /* Free the previous memory. */
        secure_free(allocator, ptr);
    }
    return new_ptr;
}

void secure_free(
    SecureAllocator allocator,
    void * ptr)
{
    size_t index = 0;
    do {
        /* Search in this page. */
        int ret = osRtxMemoryFree(table(allocator)->page_origins[index], ptr);
        /* Return if free was successful. */
        if (ret == 1) {
            DPRINTF("secure_free: Freed %p in page %u.\n", ptr, index);
            return;
        }
        /* Otherwise, go to the next page. */
        index++;
    } /* Continue search if more pages are available. */
    while (index < table(allocator)->page_count);

    DPRINTF("secure_free: %p not found in allocator %p!\n", ptr, allocator);
    /* We found nothing. */
    return;
}
