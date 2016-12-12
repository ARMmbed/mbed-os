/***************************************************************************//**
 * @file buffer_pool_allocator.h
 * @brief This is a simple memory allocator that uses a build time defined pool
 *   of constant sized buffers. It's a very simple allocator, but one that can
 *   be easily used in any application.
 *
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef BUFFER_POOL_ALLOCATOR_H__
#define BUFFER_POOL_ALLOCATOR_H__

// Get the standard include types
#include <stdint.h>

/**
 * Allocate a buffer with at least the number of bytes specified. If there is
 * not enough space then this function will return NULL.
 * @param size The number of bytes to allocate for this buffer
 * @return Returns a handle to a buffer at least size bytes long or NULL if no
 *   buffer could be allocated.
 */
void* memoryAllocate(uint32_t size);

/**
 * Free the buffer pointed to by handle. This will only decrement the reference
 * counter for this buffer. The memory is not freed until the reference counter
 * reaches zero.
 * @param handle The handle to free. Must match the value returned by
 *   the memoryAllocate() function.
 */
void memoryFree(void *handle);

/**
 * Take a memory handle and get the data pointer associated with it. This will
 * return NULL if passed an invalid or unallocated handle.
 * @param handle The handle to get the pointer for. Must match the value
 *   returned by the memoryAllocate() function.
 */
void *memoryPtrFromHandle(void *handle);

/**
 * Increment the reference counter on the memory pointed to by handle. After
 * doing this there will have to be an additional call to memoryFree() to
 * release the memory.
 * @param handle The handle to the object which needs its reference count
 *   increased. Must match the value returned by the memoryAllocate() function.
 */
void memoryTakeReference(void *handle);

#endif // BUFFER_POOL_ALLOCATOR_H__
