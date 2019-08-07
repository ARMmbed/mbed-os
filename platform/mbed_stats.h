
/** \ingroup mbed-os-public */
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_stats stats functions
 * @{
 */
/* mbed Microcontroller Library
 * Copyright (c) 2016-2019 ARM Limited
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
#ifndef MBED_STATS_H
#define MBED_STATS_H
#include <stdint.h>
#include <stddef.h>
#include "hal/ticker_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MBED_ALL_STATS_ENABLED

#ifndef MBED_SYS_STATS_ENABLED
#define MBED_SYS_STATS_ENABLED      1
#endif
#ifndef MBED_STACK_STATS_ENABLED
#define MBED_STACK_STATS_ENABLED    1
#endif
#ifndef MBED_CPU_STATS_ENABLED
#define MBED_CPU_STATS_ENABLED      1
#endif
#ifndef MBED_HEAP_STATS_ENABLED
#define MBED_HEAP_STATS_ENABLED     1
#endif
#ifndef MBED_THREAD_STATS_ENABLED
#define MBED_THREAD_STATS_ENABLED   1
#endif

#endif // MBED_ALL_STATS_ENABLED

/** Maximum memory regions reported by mbed-os memory statistics */
#define MBED_MAX_MEM_REGIONS     4

/**
 * struct mbed_stats_heap_t definition
 */
typedef struct {
    uint32_t current_size;      /**< Bytes currently allocated on the heap */
    uint32_t max_size;          /**< Maximum bytes allocated on the heap at one time since reset */
    uint32_t total_size;        /**< Cumulative sum of bytes allocated on the heap that have not been freed */
    uint32_t reserved_size;     /**< Current number of bytes reserved for the heap */
    uint32_t alloc_cnt;         /**< Current number of allocations that have not been freed since reset */
    uint32_t alloc_fail_cnt;    /**< Number of failed allocations since reset */
    uint32_t overhead_size;     /**< Number of bytes used to store heap statistics. This overhead takes up space on the heap, reducing the available heap space */
} mbed_stats_heap_t;

/**
 *  Fill the passed in heap stat structure with the heap statistics.
 *
 *  @param stats    A pointer to the mbed_stats_heap_t structure to fill
 */
void mbed_stats_heap_get(mbed_stats_heap_t *stats);

/**
 * struct mbed_stats_stack_t definition
 */
typedef struct {
    uint32_t thread_id;         /**< Identifier for the thread that owns the stack or 0 if representing accumulated statistics */
    uint32_t max_size;          /**< Maximum number of bytes used on the stack since the thread was started */
    uint32_t reserved_size;     /**< Current number of bytes reserved for the stack */
    uint32_t stack_cnt;         /**< The number of stacks represented in the accumulated statistics or 1 if representing a single stack */
} mbed_stats_stack_t;

/**
 *  Fill the passed in structure with stack statistics accumulated for all threads. The thread_id will be 0
 *  and stack_cnt will represent number of threads.
 *
 *  @param stats    A pointer to the mbed_stats_stack_t structure to fill
 */
void mbed_stats_stack_get(mbed_stats_stack_t *stats);

/**
 *  Fill the passed array of structures with the stack statistics for each available thread.
 *
 *  @param stats    A pointer to an array of mbed_stats_stack_t structures to fill
 *  @param count    The number of mbed_stats_stack_t structures in the provided array
 *  @return         The number of mbed_stats_stack_t structures that have been filled.
 *                  If the number of stacks on the system is less than or equal to count, it will equal the number of stacks on the system.
 *                  If the number of stacks on the system is greater than count, it will equal count.
 */
size_t mbed_stats_stack_get_each(mbed_stats_stack_t *stats, size_t count);

/**
 * struct mbed_stats_cpu_t definition
 */
typedef struct {
    us_timestamp_t uptime;            /**< Time since the system has started */
    us_timestamp_t idle_time;         /**< Time spent in the idle thread since the system has started */
    us_timestamp_t sleep_time;        /**< Time spent in sleep since the system has started */
    us_timestamp_t deep_sleep_time;   /**< Time spent in deep sleep since the system has started */
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
    uint32_t id;                /**< ID of the thread */
    uint32_t state;             /**< State of the thread */
    uint32_t priority;          /**< Priority of the thread (higher number indicates higher priority) */
    uint32_t stack_size;        /**< Current number of bytes reserved for the stack */
    uint32_t stack_space;       /**< Current number of free bytes remaining on the stack */
    const char   *name;         /**< Name of the thread */
} mbed_stats_thread_t;

/**
 *  Fill the passed array of stat structures with the thread statistics for each available thread.
 *
 *  @param stats    A pointer to an array of mbed_stats_thread_t structures to fill
 *  @param count    The number of mbed_stats_thread_t structures in the provided array
 *  @return         The number of mbed_stats_thread_t structures that have been filled.
 *                  If the number of threads on the system is less than or equal to count, it will equal the number of threads on the system.
 *                  If the number of threads on the system is greater than count, it will equal count.
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
    uint32_t os_version;                        /**< Mbed OS version (populated only for tagged releases) */
    uint32_t cpu_id;                            /**< CPUID register data (Cortex-M only supported) */
    mbed_compiler_id_t compiler_id;             /**< Compiler ID \ref mbed_compiler_id_t */
    uint32_t compiler_version;                  /**< Compiler version */
    uint32_t ram_start[MBED_MAX_MEM_REGIONS];   /**< Start addresses of all internal RAM memories */
    uint32_t ram_size[MBED_MAX_MEM_REGIONS];    /**< Size of all internal RAM memories in target */
    uint32_t rom_start[MBED_MAX_MEM_REGIONS];   /**< Start addresses of all internal ROM memories */
    uint32_t rom_size[MBED_MAX_MEM_REGIONS];    /**< Size of all internal ROM memories in target */
} mbed_stats_sys_t;

/**
 *  Fill the passed in system stat structure with system statistics.
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
