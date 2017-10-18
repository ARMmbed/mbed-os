/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
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

/* This file can be compiled externally to provide the page allocator algorithm
 * for devices NOT supported by uVisor. For this purpose this file is copied as
 * is into the target build folder and compiled by the target build system. */
#if defined(UVISOR_PRESENT) && (UVISOR_PRESENT == 1)

#include <uvisor.h>
#include "page_allocator.h"
#include "page_allocator_faults.h"
#include "vmpu_unpriv_access.h"
#include "vmpu.h"
#include "halt.h"
#include "context.h"

/* Since the page table memory is provided by the user, all accesses to it
 * are depriviledged! */
#define page_table_read  vmpu_unpriv_uint32_read
#define page_table_write vmpu_unpriv_uint32_write
/* For uVisor in supported mode the page allocator is always called through a
 * SVC call, which automagically serializes access to it. */
#define UVISOR_PAGE_ALLOCATOR_MUTEX_AQUIRE  {}
#define UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE {}
#define UVISOR_PAGE_UNUSED UVISOR_BOX_ID_INVALID

#endif /* defined(UVISOR_PRESENT) && (UVISOR_PRESENT == 1) */

#include "page_allocator_config.h"

/* Contains the page usage mapped by owner. */
uint32_t g_page_owner_map[UVISOR_MAX_BOXES][UVISOR_PAGE_MAP_COUNT];
/* Contains total page usage. */
uint32_t g_page_usage_map[UVISOR_PAGE_MAP_COUNT];
/* Contains the configured page size. */
uint32_t g_page_size;
/* Points to the beginning of the page heap. */
const void * g_page_heap_start;
/* Points to the end of the page heap. */
const void * g_page_heap_end;
/* Contains the number of free pages. */
uint8_t g_page_count_free;
/* Contains the total number of available pages. */
uint8_t g_page_count_total;
/* Contains the shift of the page owner mask. */
uint8_t g_page_map_shift;
/* Contains the rounded up page end address for ARMv7-M MPU region alignment. */
uint32_t g_page_head_end_rounded;

/* Helper function maps pointer to page id, or UVISOR_PAGE_UNUSED. */
uint8_t page_allocator_get_page_from_address(uint32_t address)
{
    /* Range check the returned pointer. */
    if (address < (uint32_t) g_page_heap_start || address >= (uint32_t) g_page_heap_end) {
        return UVISOR_PAGE_UNUSED;
    }
    /* Compute the index for the pointer. */
    return (address - (uint32_t) g_page_heap_start) / g_page_size;
}

void page_allocator_init(void * const heap_start, void * const heap_end, const uint32_t * const page_size)
{
    if (!page_size || !vmpu_public_flash_addr((uint32_t) page_size)) {
        HALT_ERROR(SANITY_CHECK_FAILED,
            "Page size pointer (0x%08x) is not in flash memory.\n",
            (unsigned int) page_size);
    }
    if (!heap_start || !vmpu_public_sram_addr((uint32_t) heap_start)) {
        HALT_ERROR(SANITY_CHECK_FAILED,
            "Page heap start pointer (0x%08x) is not in sram memory.\n",
            (unsigned int) heap_start);
    }
    if (!heap_end || !vmpu_public_sram_addr((uint32_t) heap_end)) {
        HALT_ERROR(SANITY_CHECK_FAILED,
            "Page heap end pointer (0x%08x) is not in sram memory.\n",
            (unsigned int) heap_end);
    }
    if (heap_end < heap_start) {
        HALT_ERROR(SANITY_CHECK_FAILED,
            "Page heap end pointer (0x%08x) is smaller than heap start pointer (0x%08x).\n",
            (unsigned int) heap_end,
            (unsigned int) heap_start);
    }
    if ((*page_size < UVISOR_PAGE_SIZE_MINIMUM) || !vmpu_is_region_size_valid(*page_size)) {
        HALT_ERROR(SANITY_CHECK_FAILED,
            "Page size (%uB) not supported by this platform!\n",
            *page_size);
    }

    uint32_t start = vmpu_round_up_region((uint32_t) heap_start, *page_size);
    if (start == 0) {
        HALT_ERROR(SANITY_CHECK_FAILED,
            "Page heap start address 0x%08x cannot be aligned with page size %uB!\n",
            (unsigned int) heap_start,
            (unsigned int) *page_size);
    }

    g_page_size = *page_size;
    /* This is the page heap start address. */
    g_page_heap_start = (void *) start;

    /* How many pages can we fit in here? */
    if (start > (0xFFFFFFFFUL - g_page_size) || (start + g_page_size) > (uint32_t) heap_end) {
        g_page_count_total = 0;
    } else {
        g_page_count_total = ((uint32_t) heap_end - start) / g_page_size;
    }
    /* Clamp page count to table size. */
    if (g_page_count_total > UVISOR_PAGE_MAX_COUNT) {
        DPRINTF("uvisor_page_init: Clamping available page count from %u to %u!\n", g_page_count_total, UVISOR_PAGE_MAX_COUNT);
        /* Move the heap start address forward so that the last clamped page is located nearest to the heap end. */
        g_page_heap_start += (g_page_count_total - UVISOR_PAGE_MAX_COUNT) * g_page_size;
        /* Clamp the page count. */
        g_page_count_total = UVISOR_PAGE_MAX_COUNT;
    }
    g_page_count_free = g_page_count_total;
    /* Remember the end of the heap. */
    g_page_heap_end = g_page_heap_start + g_page_count_total * g_page_size;

    g_page_head_end_rounded = vmpu_round_up_region((uint32_t) g_page_heap_end, g_page_size * 8);
    /* Compute the page map shift.
     * This initial shift fully left aligns the page map. */
    g_page_map_shift = UVISOR_PAGE_MAP_COUNT * 32 - g_page_count_total;
    g_page_map_shift -= (g_page_head_end_rounded - (uint32_t) g_page_heap_end) / g_page_size;

    DPRINTF(
        "page heap: [0x%08x, 0x%08x] %ukB -> %u %ukB pages\n",
            (unsigned int) g_page_heap_start,
            (unsigned int) g_page_heap_end,
            (unsigned int) (g_page_count_free * g_page_size / 1024),
            (unsigned int) g_page_count_total,
            (unsigned int) (g_page_size / 1024)
    );

    /* Force a reset of owner and usage page maps. */
    memset(g_page_owner_map, 0, sizeof(g_page_owner_map));
    memset(g_page_usage_map, 0, sizeof(g_page_usage_map));
}

int page_allocator_malloc(UvisorPageTable * const table)
{
    UVISOR_PAGE_ALLOCATOR_MUTEX_AQUIRE;
    uint32_t pages_required = page_table_read((uint32_t) &(table->page_count));
    uint32_t page_size = page_table_read((uint32_t) &(table->page_size));
    /* Check if the user even wants any pages. */
    if (pages_required == 0) {
        DPRINTF("uvisor_page_malloc: FAIL: No pages requested!\n\n");
        UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
        return UVISOR_ERROR_PAGE_INVALID_PAGE_COUNT;
    }
    /* Check if we can fulfill the requested page size. */
    if (page_size != g_page_size) {
        DPRINTF("uvisor_page_malloc: FAIL: Requested page size %uB is not the configured page size %uB!\n\n", page_size, g_page_size);
        UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
        return UVISOR_ERROR_PAGE_INVALID_PAGE_SIZE;
    }
    /* Check if we have enough pages available. */
    if (pages_required > g_page_count_free) {
        DPRINTF("uvisor_page_malloc: FAIL: Cannot serve %u pages with only %u free pages!\n\n", pages_required, g_page_count_free);
        UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
        return UVISOR_ERROR_PAGE_OUT_OF_MEMORY;
    }

    /* Get the calling box id. */
    const page_owner_t box_id = g_active_box;
    DPRINTF("uvisor_page_malloc: Requesting %u pages with size %uB for box %u\n", pages_required, page_size, box_id);

    /* Update the free pages count. */
    g_page_count_free -= pages_required;
    /* Point to the first entry in the table. */
    void * * page_table = &(table->page_origins[0]);

    /* Iterate through the page table and find the empty pages. */
    uint32_t page = 0;
    for (; (page < g_page_count_total) && pages_required; page++) {
        /* If the page is unused, map_get returns zero. */
        if (!page_allocator_map_get(g_page_usage_map, page)) {
            /* Remember this page as used. */
            page_allocator_map_set(g_page_usage_map, page);
            /* Pages of box 0 are accessible to all other boxes! */
            if (box_id == 0) {
                uint32_t ii = 0;
                for (; ii < UVISOR_MAX_BOXES; ii++) {
                    page_allocator_map_set(g_page_owner_map[ii], page);
                }
            } else {
                /* Otherwise, remember ownership only for active box. */
                page_allocator_map_set(g_page_owner_map[box_id], page);
            }
            /* Reset the fault count for this page. */
            page_allocator_reset_faults(page);
            /* Get the pointer to the page. */
            void * ptr = (void *) g_page_heap_start + page * g_page_size;
            /* Zero the entire page before handing it out. */
            memset(ptr, 0, g_page_size);
            /* Write the pages address to the table in the first page. */
            page_table_write((uint32_t) page_table, (uint32_t) ptr);
            page_table++;
            /* One less page required. */
            pages_required--;
            DPRINTF("uvisor_page_malloc: Found an empty page 0x%08x entry at index %u\n", (unsigned int) ptr, page);
        }
    }
    DPRINTF("uvisor_page_malloc: %u free pages remaining.\n\n", g_page_count_free);

    UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
    return UVISOR_ERROR_PAGE_OK;
}

int page_allocator_free(const UvisorPageTable * const table)
{
    UVISOR_PAGE_ALLOCATOR_MUTEX_AQUIRE;
    if (g_page_count_free == g_page_count_total) {
        DPRINTF("uvisor_page_free: FAIL: There are no pages to free!\n\n");
        UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
        return UVISOR_ERROR_PAGE_INVALID_PAGE_TABLE;
    }
    uint32_t page_count = page_table_read((uint32_t) &(table->page_count));
    uint32_t page_size = page_table_read((uint32_t) &(table->page_size));
    if (page_size != g_page_size) {
        DPRINTF("uvisor_page_free: FAIL: Requested page size %uB is not the configured page size %uB!\n\n", page_size, g_page_size);
        UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
        return UVISOR_ERROR_PAGE_INVALID_PAGE_SIZE;
    }
    if (page_count == 0) {
        DPRINTF("uvisor_page_free: FAIL: Pointer table is empty!\n\n");
        UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
        return UVISOR_ERROR_PAGE_INVALID_PAGE_COUNT;
    }
    if (page_count > (unsigned) (g_page_count_total - g_page_count_free)) {
        DPRINTF("uvisor_page_free: FAIL: Pointer table too large!\n\n");
        UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
        return UVISOR_ERROR_PAGE_INVALID_PAGE_TABLE;
    }

    /* Get the calling box id. */
    const page_owner_t box_id = g_active_box;
    /* Iterate over the table and validate each pointer. */
    void * const * page_table = &(table->page_origins[0]);

    int table_size = page_count;
    for (; table_size > 0; page_table++, table_size--) {
        void * page = (void *) page_table_read((uint32_t) page_table);
        /* Compute the index for the pointer. */
        uint8_t page_index = page_allocator_get_page_from_address((uint32_t) page);
        /* Range check the returned pointer. */
        if (page_index == UVISOR_PAGE_UNUSED) {
            DPRINTF("uvisor_page_free: FAIL: Pointer 0x%08x does not belong to any page!\n\n", (unsigned int) page);
            UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
            return UVISOR_ERROR_PAGE_INVALID_PAGE_ORIGIN;
        }
        /* Check if the page belongs to the caller. */
        if (page_allocator_map_get(g_page_owner_map[box_id], page_index)) {
            /* Clear the owner and usage page maps for this page. */
            page_allocator_map_clear(g_page_usage_map, page_index);
            /* If the page was owned by box 0, we need to remove it from all other boxes! */
            if (box_id == 0) {
                uint32_t ii = 0;
                for (; ii < UVISOR_MAX_BOXES; ii++) {
                    page_allocator_map_clear(g_page_owner_map[ii], page_index);
                }
            } else {
                /* Otherwise, only remove for the active box. */
                page_allocator_map_clear(g_page_owner_map[box_id], page_index);
            }
            g_page_count_free++;
            DPRINTF("uvisor_page_free: Freeing page at index %u\n", page_index);
        }
        else {
            /* Abort if the page doesn't belong to the caller. */
            if (!page_allocator_map_get(g_page_usage_map, page_index)) {
                DPRINTF("uvisor_page_free: FAIL: Page %u is not allocated!\n\n", page_index);
            } else {
                DPRINTF("uvisor_page_free: FAIL: Page %u is not owned by box %u!\n\n", page_index, box_id);
            }
            UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
            return UVISOR_ERROR_PAGE_INVALID_PAGE_OWNER;
        }
    }

    DPRINTF("uvisor_page_free: %u free pages available.\n\n", g_page_count_free);
    UVISOR_PAGE_ALLOCATOR_MUTEX_RELEASE;
    return UVISOR_ERROR_PAGE_OK;
}
