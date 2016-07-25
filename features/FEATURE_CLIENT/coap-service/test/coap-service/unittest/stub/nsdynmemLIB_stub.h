/*
 * Copyright (c) 2015 ARM Limited. All Rights Reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __NSDYNMEMLIB_STUB_H__
#define __NSDYNMEMLIB_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
    uint8_t returnCounter;
    void* expectedPointer;
} nsdynmemlib_stub_data_t;

extern nsdynmemlib_stub_data_t nsdynmemlib_stub;


void *ns_dyn_mem_alloc(int16_t alloc_size);
void *ns_dyn_mem_temporary_alloc(int16_t alloc_size);
void ns_dyn_mem_free(void *block);

#ifdef __cplusplus
}
#endif

#endif
