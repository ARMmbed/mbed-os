/*
 * Copyright (c) 2015-2016, ARM Limited, All Rights Reserved
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
#include "uvisor-lib/uvisor-lib.h"

#if !(defined(UVISOR_PRESENT) && (UVISOR_PRESENT == 1))

#include "cmsis.h"

/* This is the fallback implementation for using the page allocator from uVisor
 * inside an OS as a normal function.
 * Be aware that the page allocator is not re-entrant, so the OS must provide a
 * mutex implementation to enable thread-safety!
 */
#define DPRINTF(...) {}
#define g_active_box 0
#define vmpu_is_box_id_valid(...) 0
#define vmpu_public_flash_addr(...) 1
#define vmpu_sram_addr(...) 1
#define vmpu_public_sram_addr(...) 1
#define HALT_ERROR(id, ...) {}
#define UVISOR_PAGE_ALLOCATOR_MUTEX_AQUIRE  page_allocator_mutex_aquire()
#define UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE osMutexRelease(g_page_allocator_mutex_id)
#define page_allocator_reset_faults(...) {}

/* Forward declaration of the page allocator API. */
int page_allocator_malloc(UvisorPageTable * const table);
int page_allocator_free(const UvisorPageTable * const table);

int uvisor_page_malloc(UvisorPageTable *const table)
{
    return page_allocator_malloc(table);
}

int uvisor_page_free(const UvisorPageTable *const table)
{
    return page_allocator_free(table);
}

/* Implement mutex for page allocator. */
static osMutexId_t g_page_allocator_mutex_id = NULL;
static osRtxMutex_t g_page_allocator_mutex_data;
static osMutexDef_t g_page_allocator_mutex_attr = {
  .name = "uvisor_malloc_mutex",
  .attr_bits = 0, /* Non-recursive */
  .cb_mem = &g_page_allocator_mutex_data,
  .cb_size = sizeof(g_page_allocator_mutex_data)
};

static void page_allocator_mutex_aquire()
{
    if (g_page_allocator_mutex_id == NULL) {
        /* Create mutex if not already done. */
        g_page_allocator_mutex_id = osMutexNew(&g_page_allocator_mutex_attr);
        if (g_page_allocator_mutex_id == NULL) {
            /* Mutex failed to be created. */
            return;
        }
    }

    osMutexAcquire(g_page_allocator_mutex_id, osWaitForever);
}

/* Alignment of MPU regions is not required anymore, however we still require
 * a 32B alignment, to have some page size granularity. */
static inline int vmpu_is_region_size_valid(uint32_t size)
{
    return ((size & ~31) == size);
}
static inline uint32_t vmpu_round_up_region(uint32_t addr, uint32_t size)
{
    if (!vmpu_is_region_size_valid(size)) {
        return 0;
    }
    const uint32_t mask = size - 1;
    /* Adding the mask can overflow. */
    const uint32_t rounded_addr = addr + mask;
    /* Check for overflow. */
    if (rounded_addr < addr) {
        /* This means the address was too large to align. */
        return 0;
    }
    /* Mask the rounded address to get the aligned address. */
    return (rounded_addr & ~mask);
}
static inline uint32_t page_table_read(uint32_t addr)
{
    return *((uint32_t *) addr);
}
static inline void page_table_write(uint32_t addr, uint32_t data)
{
    *((uint32_t *) addr) = data;
}

/* Include the original page allocator source directly. */
#include "../page_allocator.c_inc"

#endif
