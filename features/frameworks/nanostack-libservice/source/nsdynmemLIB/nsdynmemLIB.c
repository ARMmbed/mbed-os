/*
 * Copyright (c) 2014-2019 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdint.h>
#include <string.h>
#undef NSDYNMEM_TRACKER_ENABLED
#include "nsdynmemLIB.h"
#include "platform/arm_hal_interrupt.h"
#include <stdlib.h>
#include "ns_list.h"

#ifndef STANDARD_MALLOC
typedef enum mem_stat_update_t {
    DEV_HEAP_ALLOC_OK,
    DEV_HEAP_ALLOC_FAIL,
    DEV_HEAP_FREE,
} mem_stat_update_t;

typedef struct {
    ns_list_link_t link;
} hole_t;

typedef int ns_mem_word_size_t; // internal signed heap block size type

// Amount of memory regions
#define REGION_COUNT 3

/* struct for book keeping variables */
struct ns_mem_book {
    ns_mem_word_size_t     *heap_main[REGION_COUNT];
    ns_mem_word_size_t     *heap_main_end[REGION_COUNT];
    mem_stat_t *mem_stat_info_ptr;
    void (*heap_failure_callback)(heap_fail_t);
    NS_LIST_HEAD(hole_t, link) holes_list;
    ns_mem_heap_size_t heap_size;
    ns_mem_heap_size_t temporary_alloc_heap_limit;   /* Amount of reserved heap temporary alloc can't exceed */
};

static mem_stat_t default_stats;
static ns_mem_book_t *default_book; // heap pointer for original "ns_" API use

// size of a hole_t in our word units
#define HOLE_T_SIZE ((ns_mem_word_size_t) ((sizeof(hole_t) + sizeof(ns_mem_word_size_t) - 1) / sizeof(ns_mem_word_size_t)))

#define TEMPORARY_ALLOC_FREE_HEAP_THRESHOLD 5  /* temporary allocations must leave 5% of the heap free */

static NS_INLINE hole_t *hole_from_block_start(ns_mem_word_size_t *start)
{
    return (hole_t *)(start + 1);
}

static NS_INLINE ns_mem_word_size_t *block_start_from_hole(hole_t *start)
{
    return ((ns_mem_word_size_t *)start) - 1;
}

static void heap_failure(ns_mem_book_t *book, heap_fail_t reason)
{
    if (book->heap_failure_callback) {
        book->heap_failure_callback(reason);
    }
}

static int ns_dyn_mem_region_find(ns_mem_book_t *book, ns_mem_word_size_t *block_ptr, ns_mem_word_size_t size)
{
    int index;
    for (index = 0; index < REGION_COUNT; index++) {
        if (book->heap_main[index] != 0) {
            if ((block_ptr >= book->heap_main[index]) &&
                    (block_ptr < book->heap_main_end[index]) &&
                    ((block_ptr + size) < book->heap_main_end[index])) {
                return index;
            }
        }
    }

    return -1;
}

static int ns_dyn_mem_region_save(ns_mem_book_t *book, ns_mem_word_size_t *region_start_ptr, ns_mem_word_size_t region_size)
{
    for (int i = 1; i < REGION_COUNT; i++) {
        if (book->heap_main[i] == 0) {
            book->heap_main[i] = region_start_ptr;
            book->heap_main_end[i] = book->heap_main[i] + region_size;
            return 0;
        }
    }

    return -1;
}


#endif //STANDARD_MALLOC

void ns_dyn_mem_init(void *heap, ns_mem_heap_size_t h_size,
                     void (*passed_fptr)(heap_fail_t), mem_stat_t *info_ptr)
{
    default_book = ns_mem_init(heap, h_size, passed_fptr, info_ptr);
}

int ns_dyn_mem_region_add(void *region_ptr, ns_mem_heap_size_t region_size)
{
    return ns_mem_region_add(default_book, region_ptr, region_size);
}

const mem_stat_t *ns_dyn_mem_get_mem_stat(void)
{
#ifndef STANDARD_MALLOC
    return ns_mem_get_mem_stat(default_book);
#else
    return NULL;
#endif
}

ns_mem_book_t *ns_mem_init(void *heap, ns_mem_heap_size_t h_size,
                           void (*passed_fptr)(heap_fail_t),
                           mem_stat_t *info_ptr)
{
#ifndef STANDARD_MALLOC
    ns_mem_book_t *book;

    ns_mem_word_size_t *ptr;
    ns_mem_word_size_t temp_int;
    /* Do memory alignment */
    temp_int = ((uintptr_t)heap % sizeof(ns_mem_word_size_t));
    if (temp_int) {
        heap = (uint8_t *) heap + (sizeof(ns_mem_word_size_t) - temp_int);
        h_size -= (sizeof(ns_mem_word_size_t) - temp_int);
    }

    /* Make correction for total length also */
    temp_int = (h_size % sizeof(ns_mem_word_size_t));
    if (temp_int) {
        h_size -= (sizeof(ns_mem_word_size_t) - temp_int);
    }

    book = heap;
    memset(book->heap_main, 0, REGION_COUNT * sizeof(ns_mem_word_size_t *));
    memset(book->heap_main_end, 0, REGION_COUNT * sizeof(ns_mem_word_size_t *));

    book->heap_main[0] = (ns_mem_word_size_t *) & (book[1]); // SET Heap Pointer
    book->heap_size = h_size - sizeof(ns_mem_book_t); //Set Heap Size
    temp_int = (book->heap_size / sizeof(ns_mem_word_size_t));
    temp_int -= 2;
    ptr = book->heap_main[0];
    *ptr = -(temp_int);
    ptr += (temp_int + 1);
    *ptr = -(temp_int);
    book->heap_main_end[0] = ptr;

    ns_list_init(&book->holes_list);
    ns_list_add_to_start(&book->holes_list, hole_from_block_start(book->heap_main[0]));

    if (info_ptr) {
        book->mem_stat_info_ptr = info_ptr;
    } else {
        book->mem_stat_info_ptr = &default_stats;
    }
    //RESET Memory by Heap Len
    memset(book->mem_stat_info_ptr, 0, sizeof(mem_stat_t));
    book->mem_stat_info_ptr->heap_sector_size = book->heap_size;
    book->temporary_alloc_heap_limit = book->heap_size / 100 * (100 - TEMPORARY_ALLOC_FREE_HEAP_THRESHOLD);
#endif
    //There really is no support to standard malloc in this library anymore
    book->heap_failure_callback = passed_fptr;

    return book;
}

int ns_mem_region_add(ns_mem_book_t *book, void *region_ptr, ns_mem_heap_size_t region_size)
{
#ifndef STANDARD_MALLOC
    if (!book || !region_ptr || region_size < 3 * sizeof(ns_mem_word_size_t)) {
        return -1;
    }

    ns_mem_word_size_t *block_ptr;
    ns_mem_word_size_t temp_int;

    /* Do memory alignment */
    temp_int = ((uintptr_t)region_ptr % sizeof(ns_mem_word_size_t));
    if (temp_int) {
        region_ptr = (uint8_t *) region_ptr + (sizeof(ns_mem_word_size_t) - temp_int);
        region_size -= (sizeof(ns_mem_word_size_t) - temp_int);
    }

    /* Make correction for total length */
    temp_int = (region_size % sizeof(ns_mem_word_size_t));
    if (temp_int) {
        region_size -= (sizeof(ns_mem_word_size_t) - temp_int);
    }

    // Create hole from new heap memory
    temp_int = (region_size / sizeof(ns_mem_word_size_t));
    temp_int -= 2;
    block_ptr = region_ptr;
    *block_ptr = -(temp_int);
    block_ptr += (temp_int + 1);    // now block_ptr points to end of block
    *block_ptr = -(temp_int);

    // find place for the new hole from the holes list
    hole_t *hole_to_add = hole_from_block_start(region_ptr);
    hole_t *previous_hole = NULL;
    ns_list_foreach(hole_t, hole_in_list_ptr, &book->holes_list) {
        if (hole_in_list_ptr < hole_to_add) {
            previous_hole = hole_in_list_ptr;
        } else if (hole_in_list_ptr == hole_to_add) {
            // trying to add memory block that is already in the list!
            return -2;
        }
    }

    // save region
    if (ns_dyn_mem_region_save(book, region_ptr, (region_size / (sizeof(ns_mem_word_size_t))) - 1) != 0) {
        return -3;
    }

    // Add new hole to the list
    if (previous_hole) {
        ns_list_add_after(&book->holes_list, previous_hole, hole_to_add);
    } else {
        ns_list_add_to_start(&book->holes_list, hole_to_add);
    }

    // adjust total heap size with new hole
    book->heap_size += region_size;

    book->mem_stat_info_ptr->heap_sector_size = book->heap_size;

    // adjust temporary allocation limits to match new heap
    book->temporary_alloc_heap_limit = book->heap_size / 100 * (100 - TEMPORARY_ALLOC_FREE_HEAP_THRESHOLD);

    return 0;
#else
    (void) book;
    (void) region_ptr;
    (void) region_size;

    return -1;
#endif
}

const mem_stat_t *ns_mem_get_mem_stat(ns_mem_book_t *heap)
{
#ifndef STANDARD_MALLOC
    return heap->mem_stat_info_ptr;
#else
    return NULL;
#endif
}

int ns_mem_set_temporary_alloc_free_heap_threshold(ns_mem_book_t *book, uint8_t free_heap_percentage, ns_mem_heap_size_t free_heap_amount)
{
#ifndef STANDARD_MALLOC
    ns_mem_heap_size_t heap_limit = 0;

    if (!book) {
        // no book
        return -1;
    }

    if (free_heap_amount && free_heap_amount < book->heap_size / 2) {
        heap_limit = book->heap_size - free_heap_amount;
    }

    if (!free_heap_amount && free_heap_percentage && free_heap_percentage < 50) {
        heap_limit = book->heap_size / 100 * (100 - free_heap_percentage);
    }

    if (free_heap_amount == 0 && free_heap_percentage == 0) {
        // feature disabled, allow whole heap to be reserved by temporary allo
        heap_limit = book->heap_size;
    }

    if (heap_limit == 0) {
        // illegal heap parameters
        return -2;
    }

    book->temporary_alloc_heap_limit = heap_limit;

    return 0;
#else
    return -3;
#endif
}

extern int ns_dyn_mem_set_temporary_alloc_free_heap_threshold(uint8_t free_heap_percentage, ns_mem_heap_size_t free_heap_amount)
{
    return ns_mem_set_temporary_alloc_free_heap_threshold(default_book, free_heap_percentage, free_heap_amount);
}

#ifndef STANDARD_MALLOC
static void dev_stat_update(mem_stat_t *mem_stat_info_ptr, mem_stat_update_t type, ns_mem_block_size_t size)
{

    switch (type) {
        case DEV_HEAP_ALLOC_OK:
            mem_stat_info_ptr->heap_sector_alloc_cnt++;
            mem_stat_info_ptr->heap_sector_allocated_bytes += size;
            if (mem_stat_info_ptr->heap_sector_allocated_bytes_max < mem_stat_info_ptr->heap_sector_allocated_bytes) {
                mem_stat_info_ptr->heap_sector_allocated_bytes_max = mem_stat_info_ptr->heap_sector_allocated_bytes;
            }
            mem_stat_info_ptr->heap_alloc_total_bytes += size;
            break;
        case DEV_HEAP_ALLOC_FAIL:
            mem_stat_info_ptr->heap_alloc_fail_cnt++;
            break;
        case DEV_HEAP_FREE:
            mem_stat_info_ptr->heap_sector_alloc_cnt--;
            mem_stat_info_ptr->heap_sector_allocated_bytes -= size;
            break;
    }

}

static ns_mem_word_size_t convert_allocation_size(ns_mem_book_t *book, ns_mem_block_size_t requested_bytes)
{
    if (book->heap_main[0] == 0) {
        heap_failure(book, NS_DYN_MEM_HEAP_SECTOR_UNITIALIZED);
    } else if (requested_bytes < 1) {
        heap_failure(book, NS_DYN_MEM_ALLOCATE_SIZE_NOT_VALID);
    } else if (requested_bytes > (book->heap_size - 2 * sizeof(ns_mem_word_size_t))) {
        heap_failure(book, NS_DYN_MEM_ALLOCATE_SIZE_NOT_VALID);
    }
    return (requested_bytes + sizeof(ns_mem_word_size_t) - 1) / sizeof(ns_mem_word_size_t);
}

// Checks that block length indicators are valid
// Block has format: Size of data area [1 word] | data area [abs(size) words]| Size of data area [1 word]
// If Size is negative it means area is unallocated
static int8_t ns_mem_block_validate(ns_mem_word_size_t *block_start)
{
    int8_t ret_val = -1;
    ns_mem_word_size_t *end = block_start;
    ns_mem_word_size_t size_start = *end;
    end += (1 + abs(size_start));
    if (size_start != 0 && size_start == *end) {
        ret_val = 0;
    }
    return ret_val;
}
#endif

// For direction, use 1 for direction up and -1 for down
static void *ns_mem_internal_alloc(ns_mem_book_t *book, const ns_mem_block_size_t alloc_size, int direction)
{
#ifndef STANDARD_MALLOC
    if (!book) {
        /* We can not do anything except return NULL because we can't find book
           keeping block */
        return NULL;
    }

    if (direction == 1) {
        if (book->mem_stat_info_ptr->heap_sector_allocated_bytes > book->temporary_alloc_heap_limit) {
            /* Not enough heap for temporary memory allocation */
            dev_stat_update(book->mem_stat_info_ptr, DEV_HEAP_ALLOC_FAIL, 0);
            return NULL;
        }
    }

    ns_mem_word_size_t *block_ptr = NULL;

    platform_enter_critical();

    ns_mem_word_size_t data_size = convert_allocation_size(book, alloc_size);
    if (!data_size) {
        goto done;
    }

    // ns_list_foreach, either forwards or backwards, result to ptr
    for (hole_t *cur_hole = direction > 0 ? ns_list_get_first(&book->holes_list)
                            : ns_list_get_last(&book->holes_list);
            cur_hole;
            cur_hole = direction > 0 ? ns_list_get_next(&book->holes_list, cur_hole)
                       : ns_list_get_previous(&book->holes_list, cur_hole)
        ) {
        ns_mem_word_size_t *p = block_start_from_hole(cur_hole);
        if (ns_mem_block_validate(p) != 0 || *p >= 0) {
            //Validation failed, or this supposed hole has positive (allocated) size
            heap_failure(book, NS_DYN_MEM_HEAP_SECTOR_CORRUPTED);
            break;
        }
        if (-*p >= data_size) {
            // Found a big enough block
            block_ptr = p;
            break;
        }
    }

    if (!block_ptr) {
        goto done;
    }

    // Separate declaration from initialization to keep IAR happy as the gotos skip this block.
    ns_mem_word_size_t block_data_size;
    block_data_size = -*block_ptr;
    if (block_data_size >= (data_size + 2 + HOLE_T_SIZE)) {
        ns_mem_word_size_t hole_size = block_data_size - data_size - 2;
        ns_mem_word_size_t *hole_ptr;
        //There is enough room for a new hole so create it first
        if (direction > 0) {
            hole_ptr = block_ptr + 1 + data_size + 1;
            // Hole will be left at end of area.
            // Would like to just replace this block_ptr with new descriptor, but
            // they could overlap, so ns_list_replace might fail
            //ns_list_replace(&holes_list, block_ptr, hole_from_block_start(hole_ptr));
            hole_t *before = ns_list_get_previous(&book->holes_list, hole_from_block_start(block_ptr));
            ns_list_remove(&book->holes_list, hole_from_block_start(block_ptr));
            if (before) {
                ns_list_add_after(&book->holes_list, before, hole_from_block_start(hole_ptr));
            } else {
                ns_list_add_to_start(&book->holes_list, hole_from_block_start(hole_ptr));
            }
        } else {
            hole_ptr = block_ptr;
            // Hole remains at start of area - keep existing descriptor in place.
            block_ptr += 1 + hole_size + 1;
        }

        hole_ptr[0] = -hole_size;
        hole_ptr[1 + hole_size] = -hole_size;
    } else {
        // Not enough room for a left-over hole, so use the whole block
        data_size = block_data_size;
        ns_list_remove(&book->holes_list, hole_from_block_start(block_ptr));
    }
    block_ptr[0] = data_size;
    block_ptr[1 + data_size] = data_size;

done:


    if (block_ptr) {
        //Update Allocate OK
        dev_stat_update(book->mem_stat_info_ptr, DEV_HEAP_ALLOC_OK, (data_size + 2) * sizeof(ns_mem_word_size_t));
    } else {
        //Update Allocate Fail, second parameter is used for stats
        dev_stat_update(book->mem_stat_info_ptr, DEV_HEAP_ALLOC_FAIL, 0);
    }
    platform_exit_critical();

    return block_ptr ? block_ptr + 1 : NULL;
#else
    void *retval = NULL;
    if (alloc_size) {
        platform_enter_critical();
        retval = malloc(alloc_size);
        platform_exit_critical();
    }
    return retval;
#endif
}

void *ns_mem_alloc(ns_mem_book_t *heap, ns_mem_block_size_t alloc_size)
{
    return ns_mem_internal_alloc(heap, alloc_size, -1);
}

void *ns_mem_temporary_alloc(ns_mem_book_t *heap, ns_mem_block_size_t alloc_size)
{
    return ns_mem_internal_alloc(heap, alloc_size, 1);
}

void *ns_dyn_mem_alloc(ns_mem_block_size_t alloc_size)
{
    return ns_mem_alloc(default_book, alloc_size);
}

void *ns_dyn_mem_temporary_alloc(ns_mem_block_size_t alloc_size)
{
    return ns_mem_temporary_alloc(default_book, alloc_size);
}

#ifndef STANDARD_MALLOC
static void ns_mem_free_and_merge_with_adjacent_blocks(ns_mem_book_t *book, ns_mem_word_size_t *cur_block, ns_mem_word_size_t data_size)
{
    // Theory of operation: Block is always in form | Len | Data | Len |
    // So we need to check length of previous (if current not heap start)
    // and next (if current not heap end) blocks. Negative length means
    // free memory so we can merge freed block with those.

    hole_t *existing_start = NULL;
    hole_t *existing_end = NULL;
    ns_mem_word_size_t *start = cur_block;
    ns_mem_word_size_t *end = cur_block + data_size + 1;
    ns_mem_word_size_t *region_start;
    ns_mem_word_size_t *region_end;

    int region_index = ns_dyn_mem_region_find(book, cur_block, data_size);
    if (region_index >= 0) {
        region_start = book->heap_main[region_index];
        region_end = book->heap_main_end[region_index];
    } else {
        heap_failure(book, NS_DYN_MEM_HEAP_SECTOR_CORRUPTED);
        // can't find region for the block, return
        return;
    }

    //invalidate current block
    *start = -data_size;
    *end = -data_size;
    ns_mem_word_size_t merged_data_size = data_size;

    if (start != region_start) {
        if (*(start - 1) < 0) {
            ns_mem_word_size_t *block_end = start - 1;
            ns_mem_word_size_t block_size = 1 + (-*block_end) + 1;
            merged_data_size += block_size;
            start -= block_size;
            if (*start != *block_end) {
                heap_failure(book, NS_DYN_MEM_HEAP_SECTOR_CORRUPTED);
            }
            if (block_size >= 1 + HOLE_T_SIZE + 1) {
                existing_start = hole_from_block_start(start);
            }
        }
    }

    if (end != region_end) {
        if (*(end + 1) < 0) {
            ns_mem_word_size_t *block_start = end + 1;
            ns_mem_word_size_t block_size = 1 + (-*block_start) + 1;
            merged_data_size += block_size;
            end += block_size;
            if (*end != *block_start) {
                heap_failure(book, NS_DYN_MEM_HEAP_SECTOR_CORRUPTED);
            }
            if (block_size >= 1 + HOLE_T_SIZE + 1) {
                existing_end = hole_from_block_start(block_start);
            }
        }
    }

    hole_t *to_add = hole_from_block_start(start);
    hole_t *before = NULL;
    if (existing_end) {
        // Extending hole described by "existing_end" downwards.
        // Will replace with descriptor at bottom of merged block.
        // (Can't use ns_list_replace, because of danger of overlap)
        // Optimisation - note our position for insertion below.
        before = ns_list_get_next(&book->holes_list, existing_end);
        ns_list_remove(&book->holes_list, existing_end);
    }
    if (existing_start) {
        // Extending hole described by "existing_start" upwards.
        // No need to modify that descriptor - it remains at the bottom
        // of the merged block to describe it.
    } else {
        // Didn't find adjacent descriptors, but may still
        // be merging with small blocks without descriptors.
        if (merged_data_size >= HOLE_T_SIZE) {
            // Locate hole position in list, if we don't already know
            // from merging with the block above.
            if (!existing_end) {
                ns_list_foreach(hole_t, ptr, &book->holes_list) {
                    if (ptr > to_add) {
                        before = ptr;
                        break;
                    }
                }
            }
            if (before) {
                ns_list_add_before(&book->holes_list, before, to_add);
            } else {
                ns_list_add_to_end(&book->holes_list, to_add);
            }

        }
    }
    *start = -merged_data_size;
    *end = -merged_data_size;
}
#endif

static bool pointer_address_validate(ns_mem_book_t *book, ns_mem_word_size_t *ptr, ns_mem_word_size_t size)
{

    if (ns_dyn_mem_region_find(book, ptr, size) >= 0) {
        return true;
    }

    return false;
}

void ns_mem_free(ns_mem_book_t *book, void *block)
{
#ifndef STANDARD_MALLOC

    if (!block) {
        return;
    }

    ns_mem_word_size_t *ptr = block;
    ns_mem_word_size_t size;

    platform_enter_critical();
    ptr --;
    //Read Current Size
    size = *ptr;
    if (!pointer_address_validate(book, ptr, size)) {
        heap_failure(book, NS_DYN_MEM_POINTER_NOT_VALID);
    } else if (size < 0) {
        heap_failure(book, NS_DYN_MEM_DOUBLE_FREE);
    } else {
        if (ns_mem_block_validate(ptr) != 0) {
            heap_failure(book, NS_DYN_MEM_HEAP_SECTOR_CORRUPTED);
        } else {
            ns_mem_free_and_merge_with_adjacent_blocks(book, ptr, size);
            //Update Free Counter
            dev_stat_update(book->mem_stat_info_ptr, DEV_HEAP_FREE, (size + 2) * sizeof(ns_mem_word_size_t));
        }
    }

    platform_exit_critical();
#else
    platform_enter_critical();
    free(block);
    platform_exit_critical();
#endif
}

void ns_dyn_mem_free(void *block)
{
    ns_mem_free(default_book, block);
}
