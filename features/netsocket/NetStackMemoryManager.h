/*
 * Copyright (c) 2018 ARM Limited
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

#ifndef NET_STACK_MEMORY_MANAGER_H
#define NET_STACK_MEMORY_MANAGER_H

/**
 * Network Stack interface memory manager
 *
 * This interface provides abstraction for memory modules used in different IP stacks (often to accommodate zero
 * copy). NetStack interface is required to accept output packets and provide received data using this stack-
 * independent API. This header should be implemented for each IP stack, so that we keep EMAC module independent.
 *
 * NetStack memory interface uses memory buffer chains to store data. Data passed in either direction
 * may be either contiguous (a single-buffer chain), or may consist of multiple buffers.
 * Chaining of the buffers is made using singly-linked list. The NetStack data-passing APIs do not specify
 * alignment or structure of the chain in either direction.
 *
 * Memory buffers can be allocated either from heap or from memory pools. Heap buffers are always contiguous.
 * Memory pool buffers may be either contiguous or chained depending on allocation size.
 *
 * On NetStack interface buffer chain ownership is transferred. EMAC must free buffer chain that it is given for
 * link output and the stack must free the buffer chain that it is given for link input.
 *
 */

#include "nsapi.h"

typedef void net_stack_mem_buf_t;          // Memory buffer

class NetStackMemoryManager {
public:

    /**
     * Allocates memory buffer from the heap
     *
     * Memory buffer allocated from heap is always contiguous and can be arbitrary size.
     *
     * @param size     Size of the memory to allocate in bytes
     * @param align    Memory alignment requirement in bytes
     * @return         Allocated memory buffer, or NULL in case of error
     */
    virtual net_stack_mem_buf_t *alloc_heap(uint32_t size, uint32_t align) = 0;

    /**
     * Allocates memory buffer chain from a pool
     *
     * Memory allocated from pool is contiguous if size is equal or less than
     * (aligned) allocation unit, otherwise may be chained. Will typically come from
     * fixed-size packet pool memory.
     *
     * @param  size    Total size of the memory to allocate in bytes
     * @param  align   Memory alignment requirement for each buffer in bytes
     * @return         Allocated memory buffer chain, or NULL in case of error
     */
    virtual net_stack_mem_buf_t *alloc_pool(uint32_t size, uint32_t align) = 0;

    /**
     * Get memory buffer pool allocation unit
     *
     * Returns the maximum size of contiguous memory that can be allocated from a pool.
     *
     * @param align    Memory alignment requirement in bytes
     * @return         Contiguous memory size
     */
    virtual uint32_t get_pool_alloc_unit(uint32_t align) const = 0;

    /**
     * Free memory buffer chain
     *
     * Frees all buffers from the chained list.
     *
     * @param buf      Memory buffer chain to be freed.
     */
    virtual void free(net_stack_mem_buf_t *buf) = 0;

    /**
     * Return total length of a memory buffer chain
     *
     * Returns a total length of this buffer and any following buffers in the chain.
     *
     * @param buf      Memory buffer chain
     * @return         Total length in bytes
     */
    virtual uint32_t get_total_len(const net_stack_mem_buf_t *buf) const = 0;

    /**
     * Copy a memory buffer chain
     *
     * Copies data from one buffer chain to another. Copy operation does not adjust the lengths
     * of the copied-to memory buffer chain, so chain total lengths must be the same.
     *
     * @param to_buf    Memory buffer chain to copy to
     * @param from_buf  Memory buffer chain to copy from
     */
    virtual void copy(net_stack_mem_buf_t *to_buf, const net_stack_mem_buf_t *from_buf) = 0;

    /**
     * Copy to a memory buffer chain
     *
     * Copies data to a buffer chain. Copy operation does not adjust the lengths
     * of the copied-to memory buffer chain, so chain total length must match the
     * copied length.
     *
     * @param to_buf    Memory buffer chain to copy to
     * @param ptr       Pointer to data
     * @param len       Data length
     */
    virtual void copy_to_buf(net_stack_mem_buf_t *to_buf, const void *ptr, uint32_t len);

    /**
     * Copy from a memory buffer chain
     *
     * Copies data from a memory buffer chain.
     *
     * @param len       Data length
     * @param ptr       Pointer to data
     * @param from_buf  Memory buffer chain to copy from
     * @return          Length of the data that was copied
     */
    virtual uint32_t copy_from_buf(void *ptr, uint32_t len, const net_stack_mem_buf_t *from_buf) const;

    /**
     * Concatenate two memory buffer chains
     *
     * Concatenates buffer chain to end of the other buffer chain. Concatenated-to buffer total length
     * is adjusted accordingly. cat_buf must point to the start of a the chain. After concatenation
     * to_buf's chain now owns those buffers, and they will be freed when the to_buf chain is freed.
     *
     * @param to_buf   Memory buffer chain to concatenate to
     * @param cat_buf  Memory buffer chain to concatenate
     */
    virtual void cat(net_stack_mem_buf_t *to_buf, net_stack_mem_buf_t *cat_buf) = 0;

    /**
     * Returns the next buffer
     *
     * Returns the next buffer from the memory buffer chain.
     *
     * @param buf      Memory buffer
     * @return         The next memory buffer, or NULL if last
     */
    virtual net_stack_mem_buf_t *get_next(const net_stack_mem_buf_t *buf) const = 0;

    /**
     * Return pointer to the payload of the buffer
     *
     * @param buf      Memory buffer
     * @return         Pointer to the payload
     */
    virtual void *get_ptr(const net_stack_mem_buf_t *buf) const = 0;

    /**
     * Return payload size of the buffer
     *
     * @param buf      Memory buffer
     * @return         Size in bytes
     */
    virtual uint32_t get_len(const net_stack_mem_buf_t *buf) const = 0;

    /**
     * Sets the payload size of the buffer
     *
     * The allocated payload size will not change. It is not permitted
     * to change the length of a buffer that is not the first (or only) in a chain.
     *
     * @param buf      Memory buffer
     * @param len      Payload size, must be less or equal to the allocated size
     */
    virtual void set_len(net_stack_mem_buf_t *buf, uint32_t len) = 0;
};

#endif /* NET_STACK_MEMORY_MANAGER_H */
