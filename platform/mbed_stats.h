
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
#include "hal/ticker_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MBED_ALL_STATS_ENABLED
#define MBED_SYS_STATS_ENABLED      1
#define MBED_STACK_STATS_ENABLED    1
#define MBED_CPU_STATS_ENABLED      1
#define MBED_HEAP_STATS_ENABLED     1
#define MBED_THREAD_STATS_ENABLED   1
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

/**
 * struct mbed_stats_cpu_t definition
 */
typedef struct {
    us_timestamp_t uptime;            /**< Time since system is up and running */
    us_timestamp_t idle_time;         /**< Time spent in idle thread since system is up and running */
    us_timestamp_t sleep_time;        /**< Time spent in sleep since system is up and running */
    us_timestamp_t deep_sleep_time;   /**< Time spent in deep sleep since system is up and running */
} mbed_stats_cpu_t;

/**
 *  Fill the passed in CPU stat structure with CPU statistics.
 *
 *  @param stats    A pointer to the mbed_stats_cpu_t structure to fill
 */
void mbed_stats_cpu_get(mbed_stats_cpu_t *stats);

/**
 * struct mbed_stats_thread_t definition
 */
typedef struct {
    uint32_t id;                /**< Thread Object Identifier */
    uint32_t state;             /**< Thread Object State */
    uint32_t priority;          /**< Thread Priority */
    uint32_t stack_size;        /**< Thread Stack Size */
    uint32_t stack_space;       /**< Thread remaining stack size */
    const char   *name;         /**< Thread Object name */
} mbed_stats_thread_t;

/**
 *  Fill the passed array of stat structures with the thread stats for each available thread.
 *
 *  @param stats    A pointer to an array of mbed_stats_thread_t structures to fill
 *  @param count    The number of mbed_stats_thread_t structures in the provided array
 *  @return         The number of mbed_stats_thread_t structures that have been filled,
 *                  this is equal to the number of threads on the system.
 */
size_t mbed_stats_thread_get_each(mbed_stats_thread_t *stats, size_t count);

/**
 * enum mbed_compiler_id_t definition
 */
typedef enum {
    ARM = 1,                    /**< ARM */
    GCC_ARM,                    /**< GNU ARM */
    IAR                         /**< IAR */
} mbed_compiler_id_t;

/**
 * struct mbed_stats_sys_t definition
 */
typedef struct {
    uint32_t os_version;                /**< Mbed OS Version (Release only) */
    uint32_t cpu_id;                    /**< CPUID Register data (Cortex-M only supported) */
    mbed_compiler_id_t compiler_id;     /**< Compiler ID \ref mbed_compiler_id_t */
    uint32_t compiler_version;          /**< Compiler version */
} mbed_stats_sys_t;

/**
 *  Fill the passed in sys stat structure with system stats.
 *
 *  @param stats    A pointer to the mbed_stats_sys_t structure to fill
 */
void mbed_stats_sys_get(mbed_stats_sys_t *stats);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/

/** @}*/
