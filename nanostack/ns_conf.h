/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#ifndef _NS_CONF_H_
#define _NS_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file ns_conf.h
 * \brief Nanostack configuration API.
 */

/**
 * \brief Set threshold for memory garbage collection.
 *
 *  Nanostack heap usage is monitored in regular intervals. If too much memory has been used then garbage collection (GC)
 *  is triggered. GC has two adjustable thresholds: HIGH and CRITICAL. HIGH threshold is lower one and once exceeded
 *  a GC will try to release memory that is used for caching. When CRITTICAL threshold is exceeded them GC will try to release
 *  memory more aggressiveliy.
 *
 *  Nanostack memory monitoring can only work if memory statistics are enabled in nsdynmemLIB.

 *
 * \param percentage_high       Percentage of total heap when garbage collection is first time triggered
 * \param percentage_critical   Percentage of total heap when critical garbage collection is triggered
 *
 * \return 0 in success, negative value in case of error.
 *
 */
int ns_conf_gc_threshold_set(uint8_t percentage_high, uint8_t percentage_critical);

/**
 * \brief Limit amount of incoming packets if system does not have enough free memory.
 * Memory statistics must been initialized in nsdynmemLIB to get this feature working.
 *
 * \param free_heap_percentage Percentage of free heap that must be available when packet arrives to MAC layer.
 * \return 0 in case of success, <0 otherwise.
 */
int ns_conf_packet_ingress_rate_limit_by_mem(uint8_t free_heap_percentage);

#ifdef __cplusplus
}
#endif

#endif /* _NS_CONF_H_ */
