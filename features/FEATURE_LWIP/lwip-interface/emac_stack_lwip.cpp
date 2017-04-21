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

#if DEVICE_EMAC

#include "emac_stack_mem.h"
#include "pbuf.h"

emac_stack_mem_t *emac_stack_mem_alloc(emac_stack_t* stack, uint32_t size, uint32_t align)
{

    struct pbuf *pbuf = pbuf_alloc(PBUF_RAW, size + align, PBUF_RAM);
    if (pbuf == NULL) {
        return NULL;
    }

    if (align) {
        uint32_t remainder = (uint32_t)pbuf->payload % align;
        uint32_t offset = align - remainder;
        if (offset >= align) {
            offset = align;
        }

        pbuf->payload = (void*)((char*)pbuf->payload + offset);
        pbuf->tot_len -= offset;
        pbuf->len -= offset;
    }

    return (emac_stack_mem_t*)pbuf;
}

void emac_stack_mem_free(emac_stack_t* stack, emac_stack_mem_t *mem)
{
    pbuf_free((struct pbuf*)mem);
}

void emac_stack_mem_copy(emac_stack_t* stack, emac_stack_mem_t *to, emac_stack_mem_t *from)
{
    pbuf_copy((struct pbuf*)to, (struct pbuf*)from);
}

void *emac_stack_mem_ptr(emac_stack_t* stack, emac_stack_mem_t *mem)
{
    return ((struct pbuf*)mem)->payload;
}

uint32_t emac_stack_mem_len(emac_stack_t* stack, emac_stack_mem_t *mem)
{
    return ((struct pbuf*)mem)->len;
}

void emac_stack_mem_set_len(emac_stack_t* stack, emac_stack_mem_t *mem, uint32_t len)
{
    struct pbuf *pbuf = (struct pbuf*)mem;

    pbuf->len = len;
}

emac_stack_mem_t *emac_stack_mem_chain_dequeue(emac_stack_t* stack, emac_stack_mem_chain_t **chain)
{
    struct pbuf **list = (struct pbuf**)chain;
    struct pbuf *head = *list;
    *list = (*list)->next;

    return (emac_stack_mem_t *)head;
}

uint32_t emac_stack_mem_chain_len(emac_stack_t* stack, emac_stack_mem_chain_t *chain)
{
    return ((struct pbuf*)chain)->tot_len;
}

void emac_stack_mem_ref(emac_stack_t* stack, emac_stack_mem_t *mem)
{
    pbuf_ref((struct pbuf*)mem);
}

#endif /* DEVICE_EMAC */
