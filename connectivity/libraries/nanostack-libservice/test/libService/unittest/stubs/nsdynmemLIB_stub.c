/*
 * Copyright (c) 2016, 2018 ARM Limited. All rights reserved.
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

#include "nsdynmemLIB_stub.h"
#include <stdint.h>
#include <string.h>
#include <nsdynmemLIB.h>
#include "platform/arm_hal_interrupt.h"
#ifdef STANDARD_MALLOC
#include <stdlib.h>
#endif

nsdynmemlib_stub_data_t nsdynmemlib_stub;

void ns_dyn_mem_init(void *heap, ns_mem_heap_size_t h_size, void (*passed_fptr)(heap_fail_t), mem_stat_t *info_ptr)
{
}

void *ns_dyn_mem_alloc(ns_mem_block_size_t alloc_size)
{
    if (nsdynmemlib_stub.returnCounter > 0) {
        nsdynmemlib_stub.returnCounter--;
        return malloc(alloc_size);
    } else {
        return (nsdynmemlib_stub.expectedPointer);
    }
}

void *ns_dyn_mem_temporary_alloc(ns_mem_block_size_t alloc_size)
{
    if (nsdynmemlib_stub.returnCounter > 0) {
        nsdynmemlib_stub.returnCounter--;
        return malloc(alloc_size);
    } else {
        return (nsdynmemlib_stub.expectedPointer);
    }
}

void ns_dyn_mem_free(void *block)
{
    free(block);
}
