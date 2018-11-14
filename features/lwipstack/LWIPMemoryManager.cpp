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

#include "pbuf.h"
#include "LWIPMemoryManager.h"

net_stack_mem_buf_t *LWIPMemoryManager::alloc_heap(uint32_t size, uint32_t align)
{
    struct pbuf *pbuf = pbuf_alloc(PBUF_RAW, size + align, PBUF_RAM);
    if (pbuf == NULL) {
        return NULL;
    }

    align_memory(pbuf, align);

    return static_cast<net_stack_mem_buf_t *>(pbuf);
}

net_stack_mem_buf_t *LWIPMemoryManager::alloc_pool(uint32_t size, uint32_t align)
{
    uint32_t total_align = count_total_align(size, align);

    struct pbuf *pbuf = pbuf_alloc(PBUF_RAW, size + total_align, PBUF_POOL);
    if (pbuf == NULL) {
        return NULL;
    }

    align_memory(pbuf, align);

    return static_cast<net_stack_mem_buf_t *>(pbuf);
}

uint32_t LWIPMemoryManager::get_pool_alloc_unit(uint32_t align) const
{
    uint32_t alloc_unit = LWIP_MEM_ALIGN_SIZE(PBUF_POOL_BUFSIZE) - align;
    return alloc_unit;
}

void LWIPMemoryManager::free(net_stack_mem_buf_t *buf)
{
    pbuf_free(static_cast<struct pbuf *>(buf));
}

uint32_t LWIPMemoryManager::get_total_len(const net_stack_mem_buf_t *buf) const
{
    return (static_cast<const struct pbuf *>(buf))->tot_len;
}

void LWIPMemoryManager::copy(net_stack_mem_buf_t *to_buf, const net_stack_mem_buf_t *from_buf)
{
    pbuf_copy(static_cast<struct pbuf *>(to_buf), static_cast<const struct pbuf *>(from_buf));
}

void LWIPMemoryManager::copy_to_buf(net_stack_mem_buf_t *to_buf, const void *ptr, uint32_t len)
{
    pbuf_take(static_cast<struct pbuf *>(to_buf), ptr, len);
}

uint32_t LWIPMemoryManager::copy_from_buf(void *ptr, uint32_t len, const net_stack_mem_buf_t *from_buf) const
{
    return pbuf_copy_partial(static_cast<const struct pbuf *>(from_buf), ptr, len, 0);
}

void LWIPMemoryManager::cat(net_stack_mem_buf_t *to_buf, net_stack_mem_buf_t *cat_buf)
{
    pbuf_cat(static_cast<struct pbuf *>(to_buf), static_cast<struct pbuf *>(cat_buf));
}

net_stack_mem_buf_t *LWIPMemoryManager::get_next(const net_stack_mem_buf_t *buf) const
{
    if (!buf) {
        return NULL;
    }
    struct pbuf *next = (static_cast<const struct pbuf *>(buf))->next;
    return static_cast<net_stack_mem_buf_t *>(next);
}

void *LWIPMemoryManager::get_ptr(const net_stack_mem_buf_t *buf) const
{
    return (static_cast<const struct pbuf *>(buf))->payload;
}

uint32_t LWIPMemoryManager::get_len(const net_stack_mem_buf_t *buf) const
{
    return (static_cast<const struct pbuf *>(buf))->len;
}

void LWIPMemoryManager::set_len(net_stack_mem_buf_t *buf, uint32_t len)
{
    struct pbuf *pbuf = static_cast<struct pbuf *>(buf);
    pbuf->len = len;
    set_total_len(pbuf);
}

uint32_t LWIPMemoryManager::count_total_align(uint32_t size, uint32_t align)
{
    uint32_t buffers = size / get_pool_alloc_unit(align);
    if (size % get_pool_alloc_unit(align) != 0) {
        buffers++;
    }
    return buffers * align;
}

void LWIPMemoryManager::align_memory(struct pbuf *pbuf, uint32_t align)
{
    if (!align) {
        return;
    }

    struct pbuf *pbuf_start = pbuf;

    while (pbuf) {
        uint32_t remainder = reinterpret_cast<uint32_t>(pbuf->payload) % align;
        if (remainder) {
            uint32_t offset = align - remainder;
            if (offset >= align) {
                offset = align;
            }
            pbuf->payload = static_cast<char *>(pbuf->payload) + offset;
        }
        pbuf->len -= align;
        pbuf = pbuf->next;
    }

    // Correct total lengths
    set_total_len(pbuf_start);
}

void LWIPMemoryManager::set_total_len(struct pbuf *pbuf)
{
    if (!pbuf->next) {
        pbuf->tot_len = pbuf->len;
        return;
    }

    uint32_t total_len;
    struct pbuf *pbuf_tailing;

    while (pbuf) {
        total_len = pbuf->len;

        pbuf_tailing = pbuf->next;
        while (pbuf_tailing) {
            total_len += pbuf_tailing->len;
            pbuf_tailing = pbuf_tailing->next;
        }

        pbuf->tot_len = total_len;
        pbuf = pbuf->next;
    }
}
