/*
 * Copyright (c) 2014-2016 ARM Limited. All rights reserved.
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
 *
 */
#ifndef NSDYNMEMLIB_H_
#define NSDYNMEMLIB_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

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
    int16_t heap_sector_size;                   /**< Heap total Sector len. */
    int16_t heap_sector_alloc_cnt;              /**< Reserved Heap sector cnt. */
    int16_t heap_sector_allocated_bytes;        /**< Reserved Heap data in bytes. */
    int16_t heap_sector_allocated_bytes_max;    /**< Reserved Heap data in bytes max value. */
    uint32_t heap_alloc_total_bytes;            /**< Total Heap allocated bytes. */
    uint32_t heap_alloc_fail_cnt;               /**< Counter for Heap allocation fail. */
} mem_stat_t;

/**
  * \brief Init and set Dynamical heap pointer and length.
  *
  * \param heap_ptr Pointer to dynamically heap buffer
  * \param heap_size size of the heap buffer
  * \return None
  */
extern void ns_dyn_mem_init(uint8_t *heap, uint16_t h_size, void (*passed_fptr)(heap_fail_t), mem_stat_t *info_ptr);


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
extern void *ns_dyn_mem_temporary_alloc(int16_t alloc_size);
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
extern void *ns_dyn_mem_alloc(int16_t alloc_size);

/**
  * \brief Get pointer to the current mem_stat_t set via ns_dyn_mem_init.
  *
  * Get pointer to the statistics information, if one is set during the
  * initialization. This may be useful for statistics collection purposes.
  *
  * Note: the caller may not modify the returned structure.
  *
  * \return NULL, no mem_stat_t was given on initialization
  * \return !=0, Pointer to mem_stat_t.
  */
extern const mem_stat_t *ns_dyn_mem_get_mem_stat(void);

#ifdef __cplusplus
}
#endif
#endif /* NSDYNMEMLIB_H_ */

