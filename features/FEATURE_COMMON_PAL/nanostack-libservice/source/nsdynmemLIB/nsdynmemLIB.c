/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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
#include "nsdynmemLIB.h"
#include "platform/arm_hal_interrupt.h"
#include <stdlib.h>
#include "ns_list.h"

void (*heap_failure_callback)(heap_fail_t);

#ifndef STANDARD_MALLOC
static int *heap_main = 0;
static int *heap_main_end = 0;
static uint16_t heap_size = 0;

typedef enum mem_stat_update_t {
    DEV_HEAP_ALLOC_OK,
    DEV_HEAP_ALLOC_FAIL,
    DEV_HEAP_FREE,
} mem_stat_update_t;


static mem_stat_t *mem_stat_info_ptr = 0;

typedef struct {
    ns_list_link_t link;
} hole_t;

static NS_LIST_DEFINE(holes_list, hole_t, link);

// size of a hole_t in our word units
#define HOLE_T_SIZE ((sizeof(hole_t) + sizeof(int) - 1) / sizeof(int))

static NS_INLINE hole_t *hole_from_block_start(int *start)
{
    return (hole_t *)(start + 1);
}

static NS_INLINE int *block_start_from_hole(hole_t *start)
{
    return ((int *)start) - 1;
}


static void heap_failure(heap_fail_t reason)
{
    if (heap_failure_callback) {
        heap_failure_callback(reason);
    }
}

#endif

void ns_dyn_mem_init(uint8_t *heap, uint16_t h_size, void (*passed_fptr)(heap_fail_t), mem_stat_t *info_ptr)
{
#ifndef STANDARD_MALLOC
    int *ptr;
    int temp_int;
    /* Do memory alignment */
    temp_int = ((uintptr_t)heap % sizeof(int));
    if (temp_int) {
        heap += (sizeof(int) - temp_int);
        h_size -= (sizeof(int) - temp_int);
    }

    /* Make correction for total length also */
    temp_int = (h_size % sizeof(int));
    if (temp_int) {
        h_size -= (sizeof(int) - temp_int);
    }
    heap_main = (int *)heap; // SET Heap Pointer
    heap_size = h_size; //Set Heap Size
    temp_int = (h_size / sizeof(int));
    temp_int -= 2;
    ptr = heap_main;
    *ptr = -(temp_int);
    ptr += (temp_int + 1);
    *ptr = -(temp_int);
    heap_main_end = ptr;

    ns_list_init(&holes_list);
    ns_list_add_to_start(&holes_list, hole_from_block_start(heap_main));

    //RESET Memory by Hea Len
    if (info_ptr) {
        mem_stat_info_ptr = info_ptr;
        memset(mem_stat_info_ptr, 0, sizeof(mem_stat_t));
        mem_stat_info_ptr->heap_sector_size = heap_size;
    }
#endif
    heap_failure_callback = passed_fptr;
}

const mem_stat_t *ns_dyn_mem_get_mem_stat(void)
{
#ifndef STANDARD_MALLOC
    return mem_stat_info_ptr;
#else
    return NULL;
#endif
}

#ifndef STANDARD_MALLOC
void dev_stat_update(mem_stat_update_t type, int16_t size)
{
    if (mem_stat_info_ptr) {
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
}

static int convert_allocation_size(int16_t requested_bytes)
{
    if (heap_main == 0) {
        heap_failure(NS_DYN_MEM_HEAP_SECTOR_UNITIALIZED);
    } else if (requested_bytes < 1) {
        heap_failure(NS_DYN_MEM_ALLOCATE_SIZE_NOT_VALID);
    } else if ((size_t)requested_bytes > (heap_size - 2 * sizeof(int)) ) {
        heap_failure(NS_DYN_MEM_ALLOCATE_SIZE_NOT_VALID);
    }
    return (requested_bytes + sizeof(int) - 1) / sizeof(int);
}

// Checks that block length indicators are valid
// Block has format: Size of data area [1 word] | data area [abs(size) words]| Size of data area [1 word]
// If Size is negative it means area is unallocated
static int8_t ns_block_validate(int *block_start)
{
    int8_t ret_val = -1;
    int *end = block_start;
    int size_start = *end;
    end += (1 + abs(size_start));
    if (size_start != 0 && size_start == *end) {
        ret_val = 0;
    }
    return ret_val;
}
#endif

// For direction, use 1 for direction up and -1 for down
static void *ns_dyn_mem_internal_alloc(const int16_t alloc_size, int direction)
{
#ifndef STANDARD_MALLOC
    int *block_ptr = NULL;

    platform_enter_critical();

    int data_size = convert_allocation_size(alloc_size);
    if (!data_size) {
        goto done;
    }

    // ns_list_foreach, either forwards or backwards, result to ptr
    for (hole_t *cur_hole = direction > 0 ? ns_list_get_first(&holes_list)
                                          : ns_list_get_last(&holes_list);
         cur_hole;
         cur_hole = direction > 0 ? ns_list_get_next(&holes_list, cur_hole)
                                  : ns_list_get_previous(&holes_list, cur_hole)
        ) {
        int *p = block_start_from_hole(cur_hole);
        if (ns_block_validate(p) != 0 || *p >= 0) {
            //Validation failed, or this supposed hole has positive (allocated) size
            heap_failure(NS_DYN_MEM_HEAP_SECTOR_CORRUPTED);
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

    size_t block_data_size = -*block_ptr;
    if (block_data_size >= (data_size + 2 + HOLE_T_SIZE)) {
        int hole_size = block_data_size - data_size - 2;
        int *hole_ptr;
        //There is enough room for a new hole so create it first
        if ( direction > 0 ) {
            hole_ptr = block_ptr + 1 + data_size + 1;
            // Hole will be left at end of area.
            // Would like to just replace this block_ptr with new descriptor, but
            // they could overlap, so ns_list_replace might fail
            //ns_list_replace(&holes_list, block_ptr, hole_from_block_start(hole_ptr));
            hole_t *before = ns_list_get_previous(&holes_list, hole_from_block_start(block_ptr));
            ns_list_remove(&holes_list, hole_from_block_start(block_ptr));
            if (before) {
                ns_list_add_after(&holes_list, before, hole_from_block_start(hole_ptr));
            } else {
                ns_list_add_to_start(&holes_list, hole_from_block_start(hole_ptr));
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
        ns_list_remove(&holes_list, hole_from_block_start(block_ptr));
    }
    block_ptr[0] = data_size;
    block_ptr[1 + data_size] = data_size;

 done:
    if (mem_stat_info_ptr) {
        if (block_ptr) {
            //Update Allocate OK
            dev_stat_update(DEV_HEAP_ALLOC_OK, (data_size + 2) * sizeof(int));

        } else {
            //Update Allocate Fail, second parameter is not used for stats
            dev_stat_update(DEV_HEAP_ALLOC_FAIL, 0);
        }
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

void *ns_dyn_mem_alloc(int16_t alloc_size)
{
    return ns_dyn_mem_internal_alloc(alloc_size, -1);
}

void *ns_dyn_mem_temporary_alloc(int16_t alloc_size)
{
    return ns_dyn_mem_internal_alloc(alloc_size, 1);
}

#ifndef STANDARD_MALLOC
static void ns_free_and_merge_with_adjacent_blocks(int * const cur_block, int data_size)
{
    // Theory of operation: Block is always in form | Len | Data | Len |
    // So we need to check length of previous (if current not heap start)
    // and next (if current not heap end) blocks. Negative length means
    // free memory so we can merge freed block with those.

    hole_t *existing_start = NULL;
    hole_t *existing_end = NULL;
    int *start = cur_block;
    int *end = cur_block + data_size + 1;
    //invalidate current block
    *start = -data_size;
    *end = -data_size;
    size_t merged_data_size = data_size;

    if (start != heap_main) {
        if (*(start - 1) < 0) {
            int *block_end = start - 1;
            size_t block_size = 1 + (-*block_end) + 1;
            merged_data_size += block_size;
            start -= block_size;
            if (*start != *block_end) {
                heap_failure(NS_DYN_MEM_HEAP_SECTOR_CORRUPTED);
            }
            if (block_size >= 1 + HOLE_T_SIZE + 1) {
                existing_start = hole_from_block_start(start);
            }
        }
    }

    if (end != heap_main_end) {
        if (*(end + 1) < 0) {
            int *block_start = end + 1;
            size_t block_size = 1 + (-*block_start) + 1;
            merged_data_size += block_size;
            end += block_size;
            if (*end != *block_start) {
                heap_failure(NS_DYN_MEM_HEAP_SECTOR_CORRUPTED);
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
        before = ns_list_get_next(&holes_list, existing_end);
        ns_list_remove(&holes_list, existing_end);
    }
    if (existing_start) {
        // Extending hole described by "existing_start" upwards.
        // No need to modify that descriptor - it remains at the bottom
        // of the merged block to describe it.
    } else {
        // Didn't find adjacent descriptors, but may still
        // be merging with small blocks without descriptors.
        if ( merged_data_size >= HOLE_T_SIZE ) {
            // Locate hole position in list, if we don't already know
            // from merging with the block above.
            if (!existing_end) {
                ns_list_foreach(hole_t, ptr, &holes_list) {
                    if (ptr > to_add) {
                        before = ptr;
                        break;
                    }
                }
            }
            if (before) {
                ns_list_add_before(&holes_list, before, to_add);
            } else {
                ns_list_add_to_end(&holes_list, to_add);
            }

        }
    }
    *start = -merged_data_size;
    *end = -merged_data_size;
}
#endif

void ns_dyn_mem_free(void *block)
{
#ifndef STANDARD_MALLOC
    int *ptr = block;
    int size;

    if (!block) {
        return;
    }

    if (!heap_main) {
        heap_failure(NS_DYN_MEM_HEAP_SECTOR_UNITIALIZED);
        return;
    }

    platform_enter_critical();
    ptr --;
    //Read Current Size
    size = *ptr;
    if (size < 0) {
        heap_failure(NS_DYN_MEM_DOUBLE_FREE);
    } else if (ptr < heap_main || ptr >= heap_main_end) {
        heap_failure(NS_DYN_MEM_POINTER_NOT_VALID);
    } else if ((ptr + size) >= heap_main_end) {
        heap_failure(NS_DYN_MEM_POINTER_NOT_VALID);
    } else {
        if (ns_block_validate(ptr) != 0) {
            heap_failure(NS_DYN_MEM_HEAP_SECTOR_CORRUPTED);
        } else {
            ns_free_and_merge_with_adjacent_blocks(ptr, size);
            if (mem_stat_info_ptr) {
                //Update Free Counter
                dev_stat_update(DEV_HEAP_FREE, (size + 2) * sizeof(int));
            }
        }
    }
    platform_exit_critical();
#else
    platform_enter_critical();
    free(block);
    platform_exit_critical();
#endif
}
