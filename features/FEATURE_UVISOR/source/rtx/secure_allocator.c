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

#include "rt_TypeDef.h"
#include "rt_Memory.h"

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
    /* The internal rt_Memory MEMP structure must be placed AFTER table.page_origins[0] !!! */
    size_t offset = OFFSETOF(SecureAllocatorInternal, table.page_origins) + sizeof(((UvisorPageTable) {0}).page_origins);
    /* Create MEMP structure inside the memory. */
    if (rt_init_mem(mem + offset, bytes - offset)) {
        /* Abort if failed. */
        DPRINTF("secure_allocator_create_with_pool: MEMP allocator creation failed\n\n");
        return NULL;
    }
    /* Remember the MEMP pointer though. */
    allocator->table.page_origins[0] = mem + offset;
    DPRINTF("secure_allocator_create_with_pool: Created MEMP allocator %p with offset %d\n\n", mem + offset, offset);
    return allocator;
}

SecureAllocator secure_allocator_create_with_pages(
    size_t size,
    size_t maximum_malloc_size)
{
    const uint32_t page_size = uvisor_get_page_size();
    /* The rt_Memory allocator puts one MEMP structure at both the
     * beginning and end of the memory pool. */
    const size_t block_overhead = 2 * sizeof(MEMP);
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

    /* Initialize a MEMP structure in all pages. */
    for(size_t ii = 0; ii < page_count; ii++) {
        /* Add each page as a pool. */
        rt_init_mem(allocator->table.page_origins[ii], page_size);
        DPRINTF("secure_allocator_create_with_pages: Created MEMP allocator %p with offset %d\n", allocator->table.page_origins[ii], 0);
    }
    DPRINTF("\n");
    /* Aaaand across the line. */
    return (SecureAllocator) allocator;
}

int secure_allocator_destroy(
    SecureAllocator allocator)
{
    DPRINTF("secure_allocator_destroy: Destroying MEMP allocator at %p\n", table(allocator)->page_origins[0]);

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
        void * mem = rt_alloc_mem(table(allocator)->page_origins[index], size);
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
        size_t size = ((MEMP *) ((uint32_t) ptr - sizeof(MEMP)))->len;
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
        int ret = rt_free_mem(table(allocator)->page_origins[index], ptr);
        /* Return if free was successful. */
        if (ret == 0) {
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
