/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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

#ifndef NS_HAL_INIT_H_
#define NS_HAL_INIT_H_

#include <stddef.h>
#include "nsdynmemLIB.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialise core Nanostack HAL components.
 *
 * Calls after the first do nothing. So "major" users should make sure
 * they call this first with a "large" heap size, before anyone
 * requests a smaller one.
 *
 * Parameters are as for ns_dyn_mem_init (but note that nsdynmemlib
 * currently limits heap size to 16-bit, so be wary of passing large
 * sizes.
 *
 * If heap is NULL, h_size will be allocated from the malloc() heap,
 * else the passed-in pointer will be used.
 */
void ns_hal_init(void *heap, size_t h_size, void (*passed_fptr)(heap_fail_t), mem_stat_t *info_ptr);

#ifdef __cplusplus
}
#endif

#endif /* NS_HAL_INIT_H_ */
