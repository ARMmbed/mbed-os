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
 * \file nsdynmem_tracker.h
 * \brief Dynamical Memory Tracker definitions to override the default NS dynamic memory functionality
 * Provides tracking and tracing of dynamic memory blocks
 */

#ifndef NSDYNMEM_TRACKER_H_
#define NSDYNMEM_TRACKER_H_
#ifdef __cplusplus
extern "C" {
#endif

#if NSDYNMEM_TRACKER_ENABLED==1

#define ns_dyn_mem_free(block) ns_dyn_mem_tracker_dyn_mem_free(block, __func__, __LINE__)
#define ns_dyn_mem_temporary_alloc(alloc_size) ns_dyn_mem_tracker_dyn_mem_temporary_alloc(alloc_size, __func__, __LINE__)
#define ns_dyn_mem_alloc(alloc_size) ns_dyn_mem_tracker_dyn_mem_alloc(alloc_size, __func__, __LINE__)

void *ns_dyn_mem_tracker_dyn_mem_alloc(ns_mem_heap_size_t alloc_size, const char *function, uint32_t line);
void *ns_dyn_mem_tracker_dyn_mem_temporary_alloc(ns_mem_heap_size_t alloc_size, const char *function, uint32_t line);
void ns_dyn_mem_tracker_dyn_mem_free(void *block, const char *function, uint32_t line);

#endif

#ifdef __cplusplus
}
#endif
#endif /* NSDYNMEM_TRACKER_H_ */


