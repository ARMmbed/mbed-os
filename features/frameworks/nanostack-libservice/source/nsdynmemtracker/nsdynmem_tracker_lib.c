/*
 * Copyright (c) 2020 ARM Limited. All rights reserved.
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

#include <stdlib.h>
#include <string.h>
#include "ns_types.h"
#include "nsdynmem_tracker_lib.h"
#include "platform/arm_hal_interrupt.h"
#include "nsdynmemLIB.h"

#if NSDYNMEM_TRACKER_ENABLED==1

static int8_t ns_dyn_mem_tracker_lib_find_free_index(ns_dyn_mem_tracker_lib_conf_t *conf, uint16_t *index);
static int8_t ns_dyn_mem_tracker_lib_find_caller_index(ns_dyn_mem_tracker_lib_conf_t *conf, void *caller_addr, uint16_t *caller_index);
static int8_t ns_dyn_mem_tracker_lib_find_block_index(ns_dyn_mem_tracker_lib_conf_t *conf, void *block, uint16_t *block_index);
static int8_t ns_dyn_mem_tracker_lib_ext_find_free_index(ns_dyn_mem_tracker_lib_conf_t *conf, uint32_t start_index, uint32_t *free_index);
static void ns_dyn_mem_tracker_lib_permanent_printed_value_set(ns_dyn_mem_tracker_lib_conf_t *conf, void *caller_addr, bool new_value);
static int8_t ns_dyn_mem_tracker_lib_ext_find_block_index(ns_dyn_mem_tracker_lib_conf_t *conf, void *block, uint32_t start_index, uint32_t *block_index);

int8_t ns_dyn_mem_tracker_lib_alloc(ns_dyn_mem_tracker_lib_conf_t *conf, void *caller_addr, const char *function, uint32_t line, void *block, uint32_t alloc_size)
{
    // Allocation failed
    if (block == NULL) {
        return 0;
    }

    platform_enter_critical();

    // If dynamic memory blocks are not set, calls allocator
    if (conf->mem_blocks == NULL) {
        conf->mem_blocks = conf->alloc_mem_blocks(conf->mem_blocks, &conf->mem_blocks_count);
        if (conf->mem_blocks == NULL) {
            platform_exit_critical();
            return -1;
        }
    }

    uint16_t caller_index = 0;
    if (ns_dyn_mem_tracker_lib_find_caller_index(conf, caller_addr, &caller_index) >= 0) {
        if (conf->ext_mem_blocks == NULL) {
            conf->ext_mem_blocks = conf->ext_alloc_mem_blocks(conf->ext_mem_blocks, &conf->ext_mem_blocks_count);
            if (conf->ext_mem_blocks == NULL) {
                platform_exit_critical();
                return -1;
            }
        }

        uint32_t free_index = 0;
        uint32_t start_index = 0;
        if (conf->block_index_hash != NULL) {
            start_index = conf->block_index_hash(block, conf->ext_mem_blocks_count);
        }
        if (ns_dyn_mem_tracker_lib_ext_find_free_index(conf, start_index, &free_index) < 0) {
            conf->ext_mem_blocks = conf->ext_alloc_mem_blocks(conf->ext_mem_blocks, &conf->ext_mem_blocks_count);
            if (conf->ext_mem_blocks == NULL) {
                platform_exit_critical();
                return -1;
            }
            if (conf->block_index_hash != NULL) {
                start_index = conf->block_index_hash(block, conf->ext_mem_blocks_count);
            }
            if (ns_dyn_mem_tracker_lib_ext_find_free_index(conf, start_index, &free_index) < 0) {
                platform_exit_critical();
                return -1;
            }
        }

        // Updates memory blocks array entry
        conf->mem_blocks[caller_index].ref_count++;
        conf->mem_blocks[caller_index].total_size += alloc_size;
        conf->mem_blocks[caller_index].lifetime = 0;
        conf->mem_blocks[caller_index].permanent = false;
        conf->mem_blocks[caller_index].permanent_printed = false;

        conf->ext_mem_blocks[free_index].block = block;
        conf->ext_mem_blocks[free_index].caller_addr = caller_addr;
        conf->ext_mem_blocks[free_index].size = alloc_size;

        conf->allocated_memory += alloc_size;

        platform_exit_critical();
        return 0;
    }

    uint16_t free_index = 0;
    if (ns_dyn_mem_tracker_lib_find_free_index(conf, &free_index) < 0) {
        conf->mem_blocks = conf->alloc_mem_blocks(conf->mem_blocks, &conf->mem_blocks_count);
        if (conf->mem_blocks == NULL || (ns_dyn_mem_tracker_lib_find_free_index(conf, &free_index) < 0)) {
            platform_exit_critical();
            return -1;
        }
    }

    conf->mem_blocks[free_index].block = block;
    conf->mem_blocks[free_index].caller_addr = caller_addr;
    conf->mem_blocks[free_index].size = alloc_size;
    conf->mem_blocks[free_index].total_size = alloc_size;
    conf->mem_blocks[free_index].lifetime = 0;
    conf->mem_blocks[free_index].ref_count = 1;
    conf->mem_blocks[free_index].function = function;
    conf->mem_blocks[free_index].line = line;
    conf->mem_blocks[free_index].permanent = false;
    conf->mem_blocks[free_index].permanent_printed = false;

    if (free_index > conf->last_mem_block_index) {
        conf->last_mem_block_index = free_index;
    }

    conf->allocated_memory += alloc_size;

    platform_exit_critical();

    return 0;
}

int8_t ns_dyn_mem_tracker_lib_free(ns_dyn_mem_tracker_lib_conf_t *conf, void *caller_addr, const char *function, uint32_t line, void *block)
{
    (void) function;
    (void) line;
    (void) caller_addr;

    // No memory block or no allocations made
    if (block == NULL || conf->mem_blocks == NULL) {
        return 0;
    }

    platform_enter_critical();

    uint16_t block_index = 0;
    if (ns_dyn_mem_tracker_lib_find_block_index(conf, block, &block_index) >= 0) {
        // If last block for allocator clears the allocator
        if (conf->mem_blocks[block_index].ref_count <= 1) {
            conf->mem_blocks[block_index].ref_count = 0;
            conf->mem_blocks[block_index].caller_addr = NULL;
            conf->mem_blocks[block_index].total_size = 0;
            conf->mem_blocks[block_index].function = NULL;
            conf->mem_blocks[block_index].line = 0;
        } else {
            // Other blocks exists
            conf->mem_blocks[block_index].ref_count--;
            conf->mem_blocks[block_index].total_size -= conf->mem_blocks[block_index].size;
        }

        conf->allocated_memory -= conf->mem_blocks[block_index].size;

        // Clears block specific fields
        conf->mem_blocks[block_index].block = NULL;
        conf->mem_blocks[block_index].size = 0;
        // Resets lifetime and permanent settings
        conf->mem_blocks[block_index].lifetime = 0;
        conf->mem_blocks[block_index].permanent = false;
        conf->mem_blocks[block_index].permanent_printed = false;

        platform_exit_critical();
        return 0;
    }

    if (conf->ext_mem_blocks == NULL) {
        platform_exit_critical();
        return -1;
    }

    uint32_t ext_block_index = 0;
    uint32_t start_index = 0;
    if (conf->block_index_hash != NULL) {
        start_index = conf->block_index_hash(block, conf->ext_mem_blocks_count);
    }
    if (ns_dyn_mem_tracker_lib_ext_find_block_index(conf, block, start_index, &ext_block_index) < 0) {
        platform_exit_critical();
        return -1;
    }

    void *ext_caller_addr = conf->ext_mem_blocks[ext_block_index].caller_addr;

    uint16_t caller_index;
    if (ns_dyn_mem_tracker_lib_find_caller_index(conf, ext_caller_addr, &caller_index) < 0) {
        platform_exit_critical();
        return -1;
    }

    conf->mem_blocks[caller_index].ref_count--;
    conf->mem_blocks[caller_index].total_size -= conf->ext_mem_blocks[ext_block_index].size;

    conf->allocated_memory -= conf->ext_mem_blocks[ext_block_index].size;

    // Clears extended block
    conf->ext_mem_blocks[ext_block_index].block = NULL;
    conf->ext_mem_blocks[ext_block_index].caller_addr = NULL;
    conf->ext_mem_blocks[ext_block_index].size = 0;

    // Resets lifetime and permanent settings
    conf->mem_blocks[block_index].lifetime = 0;
    conf->mem_blocks[block_index].permanent = false;
    conf->mem_blocks[block_index].permanent_printed = false;

    // If last block for allocator clears the allocator
    if (conf->mem_blocks[block_index].ref_count == 0) {
        conf->mem_blocks[block_index].block = NULL;
        conf->mem_blocks[block_index].caller_addr = NULL;
        conf->mem_blocks[block_index].size = 0;
        conf->mem_blocks[block_index].total_size = 0;
        conf->mem_blocks[block_index].function = NULL;
        conf->mem_blocks[block_index].line = 0;
    }

    platform_exit_critical();

    return 0;
}

void ns_dyn_mem_tracker_lib_step(ns_dyn_mem_tracker_lib_conf_t *conf)
{
    platform_enter_critical();

    if (conf->mem_blocks_count != 0) {
        for (uint32_t index = 0; index <= conf->last_mem_block_index; index++) {
            if (conf->mem_blocks[index].block != NULL) {
                conf->mem_blocks[index].lifetime++;
            }
        }
    }

    platform_exit_critical();
}

int8_t ns_dyn_mem_tracker_lib_allocator_lists_update(ns_dyn_mem_tracker_lib_conf_t *conf)
{
    platform_enter_critical();

    ns_dyn_mem_tracker_lib_mem_blocks_t *blocks = conf->mem_blocks;
    ns_dyn_mem_tracker_lib_allocators_t *top_allocators = conf->top_allocators;
    ns_dyn_mem_tracker_lib_allocators_t *permanent_allocators = conf->permanent_allocators;
    ns_dyn_mem_tracker_lib_allocators_t *to_permanent_allocators = conf->to_permanent_allocators;

    uint16_t top_allocators_count = conf->top_allocators_count;
    uint16_t permanent_allocators_count = conf->permanent_allocators_count;
    uint16_t to_permanent_allocators_count = conf->to_permanent_allocators_count;

    memset(top_allocators, 0, top_allocators_count * sizeof(ns_dyn_mem_tracker_lib_allocators_t));
    memset(permanent_allocators, 0, permanent_allocators_count * sizeof(ns_dyn_mem_tracker_lib_allocators_t));
    memset(to_permanent_allocators, 0, to_permanent_allocators_count * sizeof(ns_dyn_mem_tracker_lib_allocators_t));

    // Maximum set as permanent in one go
    uint8_t to_permanent_count = 0;

    // Maximum to print of permanent entries
    uint8_t permanent_count = 0;

    for (uint32_t index = 0; index <= conf->last_mem_block_index; index++) {
        if (blocks[index].block != NULL) {
            void *caller_addr = blocks[index].caller_addr;

            // Checks if caller address has already been counted
            bool next = false;
            for (uint32_t list_index = 0; list_index < top_allocators_count; list_index++) {
                if (top_allocators[list_index].caller_addr == caller_addr) {
                    next = true;
                    break;
                }
            }
            if (next) {
                // Already on list, continue
                continue;
            }

            // Checks whether all reference are marked permanent
            if (blocks[index].permanent) {
                if (!blocks[index].permanent_printed && permanent_count < permanent_allocators_count) {
                    permanent_allocators[permanent_count].caller_addr = caller_addr;
                    permanent_allocators[permanent_count].alloc_count = blocks[index].ref_count;
                    permanent_allocators[permanent_count].total_memory = blocks[index].total_size;
                    permanent_allocators[permanent_count].min_lifetime = blocks[index].lifetime;
                    permanent_allocators[permanent_count].function = blocks[index].function;
                    permanent_allocators[permanent_count].line = blocks[index].line;

                    permanent_count++;
                    blocks[index].permanent_printed = true;
                }
                continue;
            } else {
                // Checks whether lifetime threshold has been reached, traces and skips
                if (blocks[index].lifetime > conf->to_permanent_steps_count && to_permanent_count < to_permanent_allocators_count) {
                    blocks[index].permanent = true;

                    to_permanent_allocators[to_permanent_count].caller_addr = caller_addr;
                    to_permanent_allocators[to_permanent_count].alloc_count = blocks[index].ref_count;
                    to_permanent_allocators[to_permanent_count].total_memory = blocks[index].total_size;
                    to_permanent_allocators[to_permanent_count].min_lifetime = blocks[index].lifetime;
                    to_permanent_allocators[to_permanent_count].function = blocks[index].function;
                    to_permanent_allocators[to_permanent_count].line = blocks[index].line;

                    to_permanent_count++;
                    continue;
                }
            }

            // Add to list if allocation count is larger than entry on the list
            for (uint16_t list_index = 0; list_index < top_allocators_count; list_index++) {
                if (blocks[index].ref_count >= top_allocators[list_index].alloc_count) {
                    if (list_index != (top_allocators_count - 1)) {
                        uint8_t index_count = (top_allocators_count - list_index - 1);
                        uint32_t size = index_count * sizeof(ns_dyn_mem_tracker_lib_allocators_t);
                        memmove(&top_allocators[list_index + 1], &top_allocators[list_index], size);
                    }
                    top_allocators[list_index].caller_addr = caller_addr;
                    top_allocators[list_index].alloc_count = blocks[index].ref_count;
                    top_allocators[list_index].total_memory = blocks[index].total_size;
                    top_allocators[list_index].min_lifetime = blocks[index].lifetime;
                    top_allocators[list_index].function = blocks[index].function;
                    top_allocators[list_index].line = blocks[index].line;
                    break;
                }
            }
        }
    }

    if (permanent_count < permanent_allocators_count) {
        ns_dyn_mem_tracker_lib_permanent_printed_value_set(conf, NULL, false);
    }

    platform_exit_critical();

    return 0;
}

void ns_dyn_mem_tracker_lib_max_snap_shot_update(ns_dyn_mem_tracker_lib_conf_t *conf)
{
    platform_enter_critical();

    ns_dyn_mem_tracker_lib_mem_blocks_t *blocks = conf->mem_blocks;
    ns_dyn_mem_tracker_lib_allocators_t *max_snap_shot_allocators = conf->max_snap_shot_allocators;

    uint16_t max_snap_shot_allocators_count = conf->max_snap_shot_allocators_count;

    memset(max_snap_shot_allocators, 0, max_snap_shot_allocators_count * sizeof(ns_dyn_mem_tracker_lib_allocators_t));

    for (uint32_t index = 0; index <= conf->last_mem_block_index; index++) {
        if (blocks[index].block != NULL) {
            void *caller_addr = blocks[index].caller_addr;

            // Checks if caller address has already been counted
            bool next = false;
            for (uint16_t list_index = 0; list_index < max_snap_shot_allocators_count; list_index++) {
                if (max_snap_shot_allocators[list_index].caller_addr == caller_addr) {
                    next = true;
                    break;
                }
            }
            if (next) {
                // Already on list, continue
                continue;
            }

            // Add to list if allocation count is larger than entry on the list
            for (uint16_t list_index = 0; list_index < max_snap_shot_allocators_count; list_index++) {
                if (blocks[index].total_size >= max_snap_shot_allocators[list_index].total_memory) {
                    if (list_index != (max_snap_shot_allocators_count - 1)) {
                        uint8_t index_count = (max_snap_shot_allocators_count - list_index - 1);
                        uint32_t size = index_count * sizeof(ns_dyn_mem_tracker_lib_allocators_t);
                        memmove(&max_snap_shot_allocators[list_index + 1], &max_snap_shot_allocators[list_index], size);
                    }
                    max_snap_shot_allocators[list_index].caller_addr = blocks[index].caller_addr;
                    max_snap_shot_allocators[list_index].alloc_count = blocks[index].ref_count;
                    max_snap_shot_allocators[list_index].total_memory = blocks[index].total_size;
                    max_snap_shot_allocators[list_index].min_lifetime = blocks[index].lifetime;
                    max_snap_shot_allocators[list_index].function = blocks[index].function;
                    max_snap_shot_allocators[list_index].line = blocks[index].line;
                    break;
                }
            }
        }
    }

    platform_exit_critical();
}

static void ns_dyn_mem_tracker_lib_permanent_printed_value_set(ns_dyn_mem_tracker_lib_conf_t *conf, void *caller_addr, bool new_value)
{
    /* Search for all the references to the caller address from the allocation info and
       set block permanent value */
    for (uint16_t search_index = 0; search_index <= conf->last_mem_block_index; search_index++) {
        if (caller_addr == NULL || conf->mem_blocks[search_index].caller_addr == caller_addr) {
            conf->mem_blocks[search_index].permanent_printed = new_value;
        }
    }
}

static int8_t ns_dyn_mem_tracker_lib_find_free_index(ns_dyn_mem_tracker_lib_conf_t *conf, uint16_t *free_index)
{
    for (uint16_t index = 0; index < conf->mem_blocks_count; index++) {
        if (conf->mem_blocks[index].caller_addr == NULL) {
            *free_index = index;
            return 0;
        }
    }
    return -1;
}

static int8_t ns_dyn_mem_tracker_lib_find_caller_index(ns_dyn_mem_tracker_lib_conf_t *conf, void *caller_addr, uint16_t *caller_index)
{
    for (uint16_t index = 0; index <= conf->last_mem_block_index; index++) {
        if (conf->mem_blocks[index].caller_addr == caller_addr) {
            *caller_index = index;
            return 0;
        }
    }
    return -1;
}

static int8_t ns_dyn_mem_tracker_lib_find_block_index(ns_dyn_mem_tracker_lib_conf_t *conf, void *block, uint16_t *block_index)
{
    for (uint16_t index = 0; index <= conf->last_mem_block_index; index++) {
        if (conf->mem_blocks[index].block == block) {
            *block_index = index;
            return 0;
        }
    }
    return -1;
}

static int8_t ns_dyn_mem_tracker_lib_ext_find_free_index(ns_dyn_mem_tracker_lib_conf_t *conf, uint32_t start_index, uint32_t *free_index)
{
    for (uint32_t index = start_index; index < conf->ext_mem_blocks_count; index++) {
        if (conf->ext_mem_blocks[index].caller_addr == NULL) {
            *free_index = index;
            return 0;
        }
    }

    if (start_index == 0) {
        return -1;
    }

    for (uint32_t index = 0; index < start_index; index++) {
        if (conf->ext_mem_blocks[index].caller_addr == NULL) {
            *free_index = index;
            return 0;
        }
    }

    return -1;
}

static int8_t ns_dyn_mem_tracker_lib_ext_find_block_index(ns_dyn_mem_tracker_lib_conf_t *conf, void *block, uint32_t start_index, uint32_t *block_index)
{
    for (uint32_t index = start_index; index < conf->ext_mem_blocks_count; index++) {
        if (conf->ext_mem_blocks[index].block == block) {
            *block_index = index;
            return 0;
        }
    }

    if (start_index == 0) {
        return -1;
    }

    for (uint32_t index = 0; index < start_index; index++) {
        if (conf->ext_mem_blocks[index].block == block) {
            *block_index = index;
            return 0;
        }
    }

    return -1;
}

#endif
