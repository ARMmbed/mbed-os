/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include "nsdynmemLIB.h"
#include <string.h>
#include "mbed_assert.h"
#include "NanostackMemoryManager.h"

struct ns_stack_mem_t
{
    ns_stack_mem_t *next;
    void *payload;
    uint32_t len;
    uint8_t mem[];
};

emac_mem_buf_t *NanostackMemoryManager::alloc_heap(uint32_t size, uint32_t align)
{
    ns_stack_mem_t *buf = static_cast<ns_stack_mem_t *>(ns_dyn_mem_temporary_alloc(sizeof(ns_stack_mem_t) + size + align));
    if (buf == NULL) {
        return NULL;
    }

    buf->next = NULL;
    buf->payload = buf->mem;
    buf->len = size;

    if (align) {
        uint32_t remainder = reinterpret_cast<uint32_t>(buf->payload) % align;
        if (remainder) {
            uint32_t offset = align - remainder;
            if (offset >= align) {
                offset = align;
            }

            buf->payload = static_cast<char *>(buf->payload) + offset;
        }
    }

    return static_cast<emac_mem_buf_t *>(buf);
}

emac_mem_buf_t *NanostackMemoryManager::alloc_pool(uint32_t size, uint32_t align)
{
    return alloc_heap(size, align);
}

uint32_t NanostackMemoryManager::get_pool_alloc_unit(uint32_t align) const
{
    return 1536; // arbitrary nicely-aligned number big enough for Ethernet
}

void NanostackMemoryManager::free(emac_mem_buf_t *mem)
{
    ns_dyn_mem_free(mem);
}

uint32_t NanostackMemoryManager::get_total_len(const emac_mem_buf_t *buf) const
{
    const ns_stack_mem_t *mem = static_cast<const ns_stack_mem_t *>(buf);
    uint32_t total = 0;

    while (mem) {
        total += mem->len;
        mem = mem->next;
    }
    return total;
}

void NanostackMemoryManager::copy(emac_mem_buf_t *to, const emac_mem_buf_t *from)
{
    ns_stack_mem_t *to_mem = static_cast<ns_stack_mem_t *>(to);
    const ns_stack_mem_t *from_mem = static_cast<const ns_stack_mem_t *>(from);
    MBED_ASSERT(get_total_len(to) >= get_total_len(from));

    uint32_t to_offset = 0;
    uint32_t from_offset = 0;
    while (from_mem) {
        uint32_t to_avail = to_mem->len - to_offset;
        uint32_t from_avail = from_mem->len - from_offset;
        uint32_t chunk = to_avail < from_avail ? to_avail : from_avail;
        uint8_t *to_ptr = static_cast<uint8_t *>(to_mem->payload) + to_offset;
        const uint8_t *from_ptr = static_cast<const uint8_t *>(from_mem->payload) + from_offset;
        memcpy(to_ptr, from_ptr, chunk);
        to_offset += chunk;
        if (to_offset == to_mem->len) {
            to_mem = to_mem->next;
            to_offset = 0;
        }
        from_offset += chunk;
        if (from_offset == from_mem->len) {
            from_mem = from_mem->next;
            from_offset = 0;
        }
    }
}

void NanostackMemoryManager::cat(emac_mem_buf_t *to_buf, emac_mem_buf_t *cat_buf)
{
    ns_stack_mem_t *to_mem = static_cast<ns_stack_mem_t *>(to_buf);
    ns_stack_mem_t *cat_mem = static_cast<ns_stack_mem_t *>(cat_buf);

    while (to_mem->next) {
        to_mem = to_mem->next;
    }

    to_mem->next = cat_mem;
}

emac_mem_buf_t *NanostackMemoryManager::get_next(const emac_mem_buf_t *buf) const
{
    return static_cast<const ns_stack_mem_t *>(buf)->next;
}

void *NanostackMemoryManager::get_ptr(const emac_mem_buf_t *buf) const
{
    return static_cast<const ns_stack_mem_t *>(buf)->payload;
}

uint32_t NanostackMemoryManager::get_len(const emac_mem_buf_t *buf) const
{
    return static_cast<const ns_stack_mem_t *>(buf)->len;
}

void NanostackMemoryManager::set_len(emac_mem_buf_t *buf, uint32_t len)
{
    ns_stack_mem_t *mem = static_cast<ns_stack_mem_t *>(buf);

    mem->len = len;
}
