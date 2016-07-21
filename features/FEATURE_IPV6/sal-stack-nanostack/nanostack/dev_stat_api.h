/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */
/**
 * \file dev_stat_api.h
 * \brief 6LowPAN library device statistics API
 *
 * - dev_stat_internal_init(), Enable stats update.
 * - dev_stat_get_longest_heap_sector(), Get longest free sector.
 * - dev_stat_get_runtime_seconds(), Get system runtime in seconds.
 *
 */
#ifndef _DEV_STAT_API_H
#define _DEV_STAT_API_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * /struct dev_stat_t
 * /brief Struct for device stats buffer structure.
 */
typedef struct dev_stat_t {
    /*Heap stats*/
    int16_t heap_sector_size;               /**< Heap total sector length. */
    int16_t heap_sector_alloc_cnt;      /**< Reserved heap sector count. */
    int16_t heap_sector_allocated_bytes;    /**< Reserved heap data in bytes. */
    int16_t heap_sector_allocated_bytes_max;    /**< Reserved heap data in bytes max value. */
    uint32_t heap_alloc_total_bytes;        /**< Total heap allocated bytes. */
    uint32_t heap_alloc_fail_cnt;           /**< Counter for heap allocation fail. */
} dev_stat_t;

/**
  *  \brief Enable device stats collection.
  *
  * \param info_ptr Static pointer where the stack updates device stat.
  *
  * \return 0, Init OK.
  * \return -1, Null pointer parameter detected.
  *
  */
extern int8_t dev_stat_internal_init(dev_stat_t *info_ptr);
/**
  *  \brief Get current longest free sector.
  *
  *
  * \return Signed 16-bit length for longest sector size.
  *
  */
extern int16_t dev_stat_get_longest_heap_sector(void);
/**
  *  \brief Get system runtime in seconds.
  *
  *
  * \return Signed 16-bit length for longest sector size.
  *
  */
extern uint32_t dev_stat_get_runtime_seconds(void);
#ifdef __cplusplus
}
#endif
#endif
