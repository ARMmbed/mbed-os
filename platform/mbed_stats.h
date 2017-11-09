
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_stats stats functions
 * @{
 */
/* mbed Microcontroller Library
 * Copyright (c) 2016-2016 ARM Limited
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
#ifndef MBED_STATS_H
#define MBED_STATS_H
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * struct mbed_stats_heap_t definition
 */
typedef struct {
    uint32_t current_size;      /**< Bytes allocated currently. */
    uint32_t max_size;          /**< Max bytes allocated at a given time. */
    uint32_t total_size;        /**< Cumulative sum of bytes ever allocated. */
    uint32_t reserved_size;     /**< Current number of bytes allocated for the heap. */
    uint32_t alloc_cnt;         /**< Current number of allocations. */
    uint32_t alloc_fail_cnt;    /**< Number of failed allocations. */
} mbed_stats_heap_t;

/**
 *  Fill the passed in heap stat structure with heap stats.
 *
 *  @param stats    A pointer to the mbed_stats_heap_t structure to fill
 */
void mbed_stats_heap_get(mbed_stats_heap_t *stats);

/**
 * struct mbed_stats_stack_t definition
 */
typedef struct {
    uint32_t thread_id;         /**< Identifier for thread that owns the stack or 0 if multiple threads. */
    uint32_t max_size;          /**< Maximum number of bytes used on the stack. */
    uint32_t reserved_size;     /**< Current number of bytes allocated for the stack. */
    uint32_t stack_cnt;         /**< Number of stacks stats accumulated in the structure. */
} mbed_stats_stack_t;

/**
 *  Fill the passed in structure with stack stats accumulated for all threads. The thread_id will be 0
 *  and stack_cnt will represent number of threads.
 *
 *  @param stats    A pointer to the mbed_stats_stack_t structure to fill
 */
void mbed_stats_stack_get(mbed_stats_stack_t *stats);

/**
 *  Fill the passed array of stat structures with the stack stats for each available thread.
 *
 *  @param stats    A pointer to an array of mbed_stats_stack_t structures to fill
 *  @param count    The number of mbed_stats_stack_t structures in the provided array
 *  @return         The number of mbed_stats_stack_t structures that have been filled,
 *                  this is equal to the number of stacks on the system.
 */
size_t mbed_stats_stack_get_each(mbed_stats_stack_t *stats, size_t count);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/

/** @}*/
