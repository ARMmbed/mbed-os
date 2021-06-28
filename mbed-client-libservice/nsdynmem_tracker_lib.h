/*
 * Copyright (c) 2020, Pelion and affiliates.
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


/**
 * \file nsdynmem_tracker_lib.h
 * \brief Dynamical Memory Tracker library API
 * Provides tracking and tracing of dynamic memory blocks
 */

#ifndef NSDYNMEM_TRACKER_LIB_H_
#define NSDYNMEM_TRACKER_LIB_H_
#ifdef __cplusplus
extern "C" {
#endif

#if NSDYNMEM_TRACKER_ENABLED==1

// Memory block structure with caller information
typedef struct ns_dyn_mem_tracker_lib_mem_blocks_s {
    void *block;                   /**< Allocated memory block */
    void *caller_addr;             /**< Caller address */
    uint32_t size;                 /**< Allocation size */
    uint32_t total_size;           /**< Total allocation size for all allocations */
    uint32_t lifetime;             /**< Memory block lifetime in steps (e.g. seconds) */
    uint32_t ref_count;            /**< Reference count */
    const char *function;          /**< Caller function */
    uint16_t line;                 /**< Caller line in module */
    bool permanent : 1;            /**< Permanent memory block */
    bool permanent_printed : 1;    /**< Permanent memory block printed */
} ns_dyn_mem_tracker_lib_mem_blocks_t;

// Extended memory block structure that is used if same caller allocates multiple memory blocks
typedef struct ns_dyn_mem_tracker_lib_mem_blocks_ext_s {
    void *block;                   /**< Allocated memory block */
    void *caller_addr;             /**< Caller address */
    uint32_t size;                 /**< Allocation size */
} ns_dyn_mem_tracker_lib_mem_blocks_ext_t;

// Allocator information structure
typedef struct ns_dyn_mem_tracker_lib_allocators_s {
    void *caller_addr;             /**< Caller address */
    uint32_t alloc_count;          /**< Number of allocations */
    uint32_t total_memory;         /**< Total memory used by allocations */
    uint32_t min_lifetime;         /**< Shortest lifetime among the allocations */
    const char *function;          /**< Function name string */
    uint16_t line;                 /**< Module line */
} ns_dyn_mem_tracker_lib_allocators_t;

// Memory block array allocator / array size increase allocator
typedef ns_dyn_mem_tracker_lib_mem_blocks_t *ns_dyn_mem_tracker_lib_alloc_mem_blocks(ns_dyn_mem_tracker_lib_mem_blocks_t *blocks, uint16_t *mem_blocks_count);
// Extended memory block array allocator / array size increase allocator
typedef ns_dyn_mem_tracker_lib_mem_blocks_ext_t *ns_dyn_mem_tracker_lib_alloc_mem_blocks_ext(ns_dyn_mem_tracker_lib_mem_blocks_ext_t *blocks, uint32_t *mem_blocks_count);
// Extended memory block array index hash function to get memory block (allocation/search start) index from block address
typedef uint32_t ns_dyn_mem_tracker_lib_mem_block_index_hash(void *block, uint32_t ext_mem_blocks_count);

typedef struct ns_dyn_mem_tracker_lib_conf_s {
    ns_dyn_mem_tracker_lib_mem_blocks_t *mem_blocks;                    /**< Memory blocks array, if NULL calls allocator on init */
    ns_dyn_mem_tracker_lib_mem_blocks_ext_t *ext_mem_blocks;            /**< Extended memory blocks array, if NULL calls allocator on init */
    ns_dyn_mem_tracker_lib_allocators_t *top_allocators;                /**< Top allocators array */
    ns_dyn_mem_tracker_lib_allocators_t *permanent_allocators;          /**< Permanent allocators */
    ns_dyn_mem_tracker_lib_allocators_t *to_permanent_allocators;       /**< To permanent allocators */
    ns_dyn_mem_tracker_lib_allocators_t *max_snap_shot_allocators;      /**< Snap shot of maximum memory used by allocators */
    ns_dyn_mem_tracker_lib_alloc_mem_blocks *alloc_mem_blocks;          /**< Memory block array allocator / array size increase allocator */
    ns_dyn_mem_tracker_lib_alloc_mem_blocks_ext *ext_alloc_mem_blocks;  /**< Extended memory block array allocator / array size increase allocator */
    ns_dyn_mem_tracker_lib_mem_block_index_hash *block_index_hash;      /**< Hash function to get memory block index from block address */
    uint32_t allocated_memory;                                          /**< Currently allocated memory */
    uint16_t mem_blocks_count;                                          /**< Number of entries in memory blocks array */
    uint32_t ext_mem_blocks_count;                                      /**< Number of entries in extended memory blocks array */
    uint16_t last_mem_block_index;                                      /**< Last memory block in memory blocks array */
    uint16_t top_allocators_count;                                      /**< Top allocators array count */
    uint16_t permanent_allocators_count;                                /**< Permanent allocators array count */
    uint16_t to_permanent_allocators_count;                             /**< To permanent allocators array count */
    uint16_t max_snap_shot_allocators_count;                            /**< Snap shot of maximum memory used by allocators array count */
    uint16_t to_permanent_steps_count;                                  /**< How many steps before moving block to permanent allocators list */
} ns_dyn_mem_tracker_lib_conf_t;

int8_t ns_dyn_mem_tracker_lib_alloc(ns_dyn_mem_tracker_lib_conf_t *conf, void *caller_addr, const char *function, uint32_t line, void *block, uint32_t alloc_size);
int8_t ns_dyn_mem_tracker_lib_free(ns_dyn_mem_tracker_lib_conf_t *conf, void *caller_addr, const char *function, uint32_t line, void *block);
void ns_dyn_mem_tracker_lib_step(ns_dyn_mem_tracker_lib_conf_t *conf);
int8_t ns_dyn_mem_tracker_lib_allocator_lists_update(ns_dyn_mem_tracker_lib_conf_t *conf);
void ns_dyn_mem_tracker_lib_max_snap_shot_update(ns_dyn_mem_tracker_lib_conf_t *conf);

#endif

#ifdef __cplusplus
}
#endif
#endif /* NSDYNMEM_TRACKER_LIB_H_ */


