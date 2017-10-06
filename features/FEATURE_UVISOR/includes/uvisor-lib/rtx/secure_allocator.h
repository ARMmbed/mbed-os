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
#ifndef __SECURE_ALLOCATOR_H__
#define __SECURE_ALLOCATOR_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Contains the allocator data and backing page table. */
typedef void * SecureAllocator;

/** Create an allocator in-place in an existing pool without using pages.
 * Use this to turn statically allocated memory into a heap.
 * Or allocate a large piece of memory and then turn that into a heap.
 *
 * @param mem   Pointer to the origin of the memory pool
 * @param bytes Length of the memory pool in bytes
 * @returns the allocator or `NULL` on failure
 */
SecureAllocator secure_allocator_create_with_pool(
    void * mem,
    size_t bytes);

/** Create an allocator using pages from the page heap.
 * Use this to request secure dynamic memory for your process.
 * Note that this memory is not guaranteed to be consecutive, therefore you
 * must specify the maximum allocation size that you plan to use in this
 * allocator. This function will then compute the number and size of required
 * pages and request them from the secure page heap.
 *
 * @param total_size          The minimal total size of the heap
 * @param maximum_malloc_size The largest size to be allocated in one chunk
 * @returns the allocator or `NULL` on failure (out of memory,
 *          maximum malloc size cannot be fulfilled)
 */
SecureAllocator secure_allocator_create_with_pages(
    size_t total_size,
    size_t maximum_malloc_size);

/** Destroy the allocator and free the backing pages.
 * An attempt to destroy a memory-pool backed allocator will fail and return
 * with an error code.
 *
 * @retval 0  Allocator successfully destroyed.
 * @retval -1 Allocator is static (memory-pool), or freeing memory pages failed.
 */
int secure_allocator_destroy(
    SecureAllocator allocator);

/** Drop-in for `malloc`. */
void * secure_malloc(
    SecureAllocator allocator,
    size_t size);

/** Drop-in for `aligned_alloc`. */
void * secure_aligned_alloc(
    SecureAllocator allocator,
    size_t alignment,
    size_t size);

/** Drop-in for `calloc`. */
void * secure_calloc(
    SecureAllocator allocator,
    size_t nmemb,
    size_t size);

/** Drop-in for `realloc`. */
void * secure_realloc(
    SecureAllocator allocator,
    void * ptr,
    size_t size);

/** Drop-in for `free`. */
void secure_free(
    SecureAllocator allocator,
    void * ptr);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif  /* __SECURE_ALLOCATOR_H__ */
