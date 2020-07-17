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


/**
 * \file nsdynmemLIB.h
 * \brief Dynamical Memory API for library model
 * nsdynmemlib provides access to one default heap, along with the ability to use extra user heaps.
 * ns_dyn_mem_alloc/free always access the default heap initialised by ns_dyn_mem_init.
 * ns_mem_alloc/free access a user heap initialised by ns_mem_init. User heaps are identified by a book-keeping pointer.
 */

#ifndef NSDYNMEMLIB_H_
#define NSDYNMEMLIB_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

// Added to maintain backward compatibility with older implementation of ns_dyn_mem APIs
#define NSDYNMEMLIB_API_VERSION 3

typedef size_t ns_mem_block_size_t; //external interface unsigned heap block size type
typedef size_t ns_mem_heap_size_t; //total heap size type.

/*!
 * \enum heap_fail_t
 * \brief Dynamically heap system failure call back event types.
 */
typedef enum {
    NS_DYN_MEM_NULL_FREE,               /**< ns_dyn_mem_free(), NULL pointer free [obsolete - no longer faulted]  */
    NS_DYN_MEM_DOUBLE_FREE,                     /**< ns_dyn_mem_free(), Possible double pointer free */
    NS_DYN_MEM_ALLOCATE_SIZE_NOT_VALID, /**< Allocate size is 0 or smaller or size is bigger than max heap size  */
    NS_DYN_MEM_POINTER_NOT_VALID,       /**< ns_dyn_mem_free(), try to free pointer which not at heap sector */
    NS_DYN_MEM_HEAP_SECTOR_CORRUPTED,   /**< Heap system detect sector corruption */
    NS_DYN_MEM_HEAP_SECTOR_UNITIALIZED /**< ns_dyn_mem_free(), ns_dyn_mem_temporary_alloc() or ns_dyn_mem_alloc() called before ns_dyn_mem_init() */
} heap_fail_t;

/**
 * /struct mem_stat_t
 * /brief Struct for Memory stats Buffer structure
 */
typedef struct mem_stat_t {
    /*Heap stats*/
    ns_mem_heap_size_t heap_sector_size;                   /**< Heap total Sector len. */
    ns_mem_heap_size_t heap_sector_alloc_cnt;              /**< Reserved Heap sector cnt. */
    ns_mem_heap_size_t heap_sector_allocated_bytes;        /**< Reserved Heap data in bytes. */
    ns_mem_heap_size_t heap_sector_allocated_bytes_max;    /**< Reserved Heap data in bytes max value. */
    uint32_t heap_alloc_total_bytes;            /**< Total Heap allocated bytes. */
    uint32_t heap_alloc_fail_cnt;               /**< Counter for Heap allocation fail. */
} mem_stat_t;


typedef struct ns_mem_book ns_mem_book_t;

/**
  * \brief Init and set Dynamical heap pointer and length.
  *
  * \param heap_ptr Pointer to dynamically heap buffer
  * \param h_size size of the heap buffer
  * \param passed_fptr pointer to heap error callback function
  * \param info_ptr Pointer to mem_stat_t for memory statistics. Can be NULL.
  *
  * \return None
  */
extern void ns_dyn_mem_init(void *heap_ptr, ns_mem_heap_size_t h_size, void (*passed_fptr)(heap_fail_t), mem_stat_t *info_ptr);

/**
  * \brief Add memory region to initialized heap.
  *
  * This method adds memory region to already initialized heap.
  * Method will reset temporary heap threshold to the default value.
  *
  * \param region_ptr Pointer to memory region to be added
  * \param region_size size of the region buffer
  * \return 0 on success, <0 in case of errors.
  */
extern int ns_dyn_mem_region_add(void *region_ptr, ns_mem_heap_size_t region_size);

/**
  * \brief Free allocated memory.
  *
  * \param heap_ptr Pointer to allocated memory
  *
  * \return 0, Free OK
  * \return <0, Free Fail
  */
extern void ns_dyn_mem_free(void *heap_ptr);

/**
  * \brief Allocate temporary data.
  *
  * Space allocate started from beginning of the heap sector
  *
  * \param alloc_size Allocated data size
  *
  * \return 0, Allocate Fail
  * \return >0, Pointer to allocated data sector.
  */
extern void *ns_dyn_mem_temporary_alloc(ns_mem_block_size_t alloc_size);

/**
  * \brief Allocate long period data.
  *
  * Space allocate started from end of the heap sector
  *
  * \param alloc_size Allocated data size
  *
  * \return 0, Allocate Fail
  * \return >0, Pointer to allocated data sector.
  */
extern void *ns_dyn_mem_alloc(ns_mem_block_size_t alloc_size);

/**
  * \brief Get pointer to the current mem_stat_t set via ns_dyn_mem_init.
  *
  * Get pointer to the statistics information, if one is set during the
  * initialization. This may be useful for statistics collection purposes.
  *
  * Note: the caller may not modify the returned structure.
  *
  * \return NULL, no mem_stat_t was given on initialization
  * \return Pointer to mem_stat_t or NULL.
  */
extern const mem_stat_t *ns_dyn_mem_get_mem_stat(void);

/**
  * \brief Set amount of free heap that must be available for temporary memory allocation to succeed.
  *
  * Temporary memory allocation will fail if system does not have defined amount of heap free.
  *
  * Note: the caller must set mem_stat_t structure in initialization.
  *
  * \param free_heap_percentage percentage of total heap that must be free for temporary memory allocation. Set free_heap_amount to 0 when this percentage value.
  * \param free_heap_amount Amount of free heap that must be free for temporary memory allocation. This value takes preference over percentage parameter value.
  *
  * \return 0 on success, <0 otherwise
  */
extern int ns_dyn_mem_set_temporary_alloc_free_heap_threshold(uint8_t free_heap_percentage, ns_mem_heap_size_t free_heap_amount);

/**
  * \brief Init and set Dynamical heap pointer and length.
  *
  * \param heap_ptr Pointer to dynamically heap buffer.
  * \param h_size size of the heap buffer.
  * \param passed_fptr pointer to heap error callback function.
  * \param info_ptr Pointer to mem_stat_t for memory statistics. Can be NULL.
  * \return Pointer to ns_mem_book_t.
  */
extern ns_mem_book_t *ns_mem_init(void *heap_ptr, ns_mem_heap_size_t h_size, void (*passed_fptr)(heap_fail_t), mem_stat_t *info_ptr);

/**
  * \brief Add memory region to initialized heap.
  *
  * This method adds memory region to already initialized heap.
  * Method will reset temporary heap threshold to the default value.
  *
  * \param book Address of book keeping structure.
  * \param region_ptr Pointer to memory region buffer.
  * \param region_size size of the memory region to add
  *
  * \return 0 on success, <0 in case of errors.
  */
extern int ns_mem_region_add(ns_mem_book_t *book, void *region_ptr, ns_mem_heap_size_t region_size);

/**
  * \brief Free allocated memory.
  *
  * \param book Address of book keeping structure
  * \param heap_ptr Pointer to allocated memory
  *
  * \return 0, Free OK
  * \return <0, Free Fail
  */
extern void ns_mem_free(ns_mem_book_t *book, void *heap_ptr);

/**
  * \brief Allocate temporary data.
  *
  * Space allocate started from beginning of the heap sector
  *
  * \param book Address of book keeping structure
  * \param alloc_size Allocated data size
  *
  * \return 0, Allocate Fail
  * \return >0, Pointer to allocated data sector.
  */
extern void *ns_mem_temporary_alloc(ns_mem_book_t *book, ns_mem_block_size_t alloc_size);

/**
  * \brief Allocate long period data.
  *
  * Space allocate started from end of the heap sector
  *
  * \param book Address of book keeping structure
  * \param alloc_size Allocated data size
  *
  * \return 0, Allocate Fail
  * \return >0, Pointer to allocated data sector.
  */
extern void *ns_mem_alloc(ns_mem_book_t *book, ns_mem_block_size_t alloc_size);

/**
  * \brief Get pointer to the current mem_stat_t set via ns_mem_init.
  *
  * Get pointer to the statistics information, if one is set during the
  * initialization. This may be useful for statistics collection purposes.
  *
  * Note: the caller may not modify the returned structure.
  *
  * \param book Address of book keeping structure
  *
  * \return NULL, no mem_stat_t was given on initialization
  * \return !=0, Pointer to mem_stat_t.
  */
extern const mem_stat_t *ns_mem_get_mem_stat(ns_mem_book_t *book);

/**
  * \brief Set amount of free heap that must be available for temporary memory allocation to succeed.
  *
  * Temporary memory allocation will fail if system does not have defined amount of heap free.
  *
  * Note: the caller must set mem_stat_t structure in initialization.
  *
  * \param book Address of book keeping structure
  * \param free_heap_percentage percentage of total heap that must be free for temporary memory allocation. Set free_heap_amount to 0 when using percentage value.
  * \param free_heap_amount Amount of free heap that must be free for temporary memory allocation. This value takes preference over the percentage parameter value.
  *
  * \return 0 on success, <0 otherwise
  */
extern int ns_mem_set_temporary_alloc_free_heap_threshold(ns_mem_book_t *book, uint8_t free_heap_percentage, ns_mem_heap_size_t free_heap_amount);

#ifdef __cplusplus
}
#endif
#endif /* NSDYNMEMLIB_H_ */


