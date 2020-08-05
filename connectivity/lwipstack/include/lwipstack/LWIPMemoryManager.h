/* Copyright (c) 2017 ARM Limited
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

#ifndef LWIP_MEMORY_MANAGER_H
#define LWIP_MEMORY_MANAGER_H

#include "EMACMemoryManager.h"


class LWIPMemoryManager final : public EMACMemoryManager {
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
    net_stack_mem_buf_t *alloc_heap(uint32_t size, uint32_t align) override;

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
    net_stack_mem_buf_t *alloc_pool(uint32_t size, uint32_t align) override;

    /**
     * Get memory buffer pool allocation unit
     *
     * Returns the maximum size of contiguous memory that can be allocated from a pool.
     *
     * @param align    Memory alignment requirement in bytes
     * @return         Contiguous memory size
     */
    uint32_t get_pool_alloc_unit(uint32_t align) const override;

    /**
     * Free memory buffer chain
     *
     * If memory buffer is chained must point to the start of the chain. Frees all buffers
     * from the chained list.
     *
     * @param buf      Memory buffer chain to be freed.
     */
    void free(net_stack_mem_buf_t *buf) override;

    /**
     * Return total length of a memory buffer chain
     *
     * Returns a total length of this buffer and any following buffers in the chain.
     *
     * @param buf      Memory buffer chain
     * @return         Total length in bytes
     */
    uint32_t get_total_len(const net_stack_mem_buf_t *buf) const override;

    /**
     * Copy a memory buffer chain
     *
     * Copies data from one buffer chain to another. Copy operation does not adjust the lengths
     * of the copied-to memory buffer chain, so chain total lengths must be the same.
     *
     * @param to_buf    Memory buffer chain to copy to
     * @param from_buf  Memory buffer chain to copy from
     */
    void copy(net_stack_mem_buf_t *to_buf, const net_stack_mem_buf_t *from_buf) override;

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
    void copy_to_buf(net_stack_mem_buf_t *to_buf, const void *ptr, uint32_t len) override;

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
    uint32_t copy_from_buf(void *ptr, uint32_t len, const net_stack_mem_buf_t *from_buf) const override;

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
    void cat(net_stack_mem_buf_t *to_buf, net_stack_mem_buf_t *cat_buf) override;

    /**
     * Returns the next buffer
     *
     * Returns the next buffer from the memory buffer chain.
     *
     * @param buf      Memory buffer
     * @return         The next memory buffer, or NULL if last
     */
    net_stack_mem_buf_t *get_next(const net_stack_mem_buf_t *buf) const override;

    /**
     * Return pointer to the payload of the buffer
     *
     * @param buf      Memory buffer
     * @return         Pointer to the payload
     */
    void *get_ptr(const net_stack_mem_buf_t *buf) const override;

    /**
     * Return payload size of the buffer
     *
     * @param buf      Memory buffer
     * @return         Size in bytes
     */
    uint32_t get_len(const net_stack_mem_buf_t *buf) const override;

    /**
     * Sets the payload size of the buffer
     *
     * The allocated payload size will not change. It is not permitted
     * to change the length of a buffer that is not the first (or only) in a chain.
     *
     * @param buf      Memory buffer
     * @param len      Payload size, must be less or equal allocated size
     */
    void set_len(net_stack_mem_buf_t *buf, uint32_t len) override;

private:

    /**
     * Returns a total memory alignment size
     *
     * Calculates the total memory alignment size for a memory buffer chain.
     * Used internally on pool allocation.
     *
     * @param  size    Size of the memory to allocate in bytes
     * @param  align   Memory alignment requirement for each buffer in bytes
     * @return         Total alignment needed in bytes
     */
    uint32_t count_total_align(uint32_t size, uint32_t align);

    /**
     * Aligns a memory buffer chain
     *
     * Aligns a memory buffer chain and updates lengths and total lengths
     * accordingly. There needs to be enough overhead to do the alignment
     * for all buffers.
     *
     * @param pbuf     Memory buffer
     * @param align    Memory alignment requirement for each buffer in bytes
     */
    void align_memory(struct pbuf *pbuf, uint32_t align);

    /**
     * Sets total lengths of a memory buffer chain
     *
     * Sets total length fields for a memory buffer chain based on buffer
     * length fields. All total lengths are calculated again.
     *
     * @param pbuf     Memory buffer
     */
    void set_total_len(struct pbuf *pbuf);
};

#endif /* LWIP_MEMORY_MANAGER_H */
