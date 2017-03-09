/*
 * Copyright (c) 2015-2016, ARM Limited, All Rights Reserved
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
#include "uvisor-lib/uvisor-lib.h"

#if !(defined(UVISOR_PRESENT) && (UVISOR_PRESENT == 1))

#include <string.h> /* for memset */

/* Forward declaration of the page allocator API. */
extern void page_allocator_init(void * const heap_start, void * const heap_end, const uint32_t * const page_size);

extern uint32_t __end__[];      /* __heap_start */
extern uint32_t __HeapLimit[];  /* __heap_end   */

extern uint32_t __StackLimit[];   /* bottom of stack */

/* There is only one box index for box 0. */
RtxBoxIndex * __uvisor_ps UVISOR_ALIGN(4);

static void box_index_init(void *box_bss, uint32_t heap_size)
{
    const uint32_t index_size = sizeof(RtxBoxIndex);
    /* Adjust size for overhead of box index */
    heap_size -= index_size;

    /* The box index is at the beginning of the bss section */
    RtxBoxIndex *const indexOS = box_bss;
    /* Zero the _entire_ index, so that we know to initialize the mutex on
     * first use! */
    memset(box_bss, 0, index_size);
    /* Initialize user context */
    indexOS->index.ctx = NULL;
    /* Initialize box heap */
    indexOS->index.box_heap = box_bss + index_size;
    indexOS->index.box_heap_size = heap_size;
    /* Active heap pointer is NULL */
    indexOS->index.active_heap = NULL;

    /* There is no box config for unsupported! */
    indexOS->index.config = NULL;

    /* Set the index */
    __uvisor_ps = indexOS;
}

void secure_malloc_init(void)
{
    /* get the public heap size from the linker script */
    uint32_t heap_size = ((uint32_t) __HeapLimit -
                          (uint32_t) __end__);
    /* Public heap size is aligned to page boundaries n*UVISOR_PAGE_SIZE */
    uint32_t heap_start = (uint32_t) __StackLimit - heap_size;
    /* align the start address of the public heap to a page boundary */
    heap_start &= ~(UVISOR_PAGE_SIZE - 1);
    /* adjust the heap size to the new heap start address */
    heap_size = (uint32_t) __StackLimit - heap_start;

    /* page heap now extends from the previous public heap start address
     * to the new public heap start address */
    extern uint32_t __uvisor_page_size;
    page_allocator_init(__end__, (void *) heap_start, &__uvisor_page_size);
    box_index_init((void *) heap_start, heap_size);
}

#endif
