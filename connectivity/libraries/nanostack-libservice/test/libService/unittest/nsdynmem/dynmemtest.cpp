/*
 * Copyright (c) 2015-2018 ARM Limited. All rights reserved.
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
#include "CppUTest/TestHarness.h"
#include "nsdynmemLIB.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error_callback.h"

// hardcoded amount of regions, keep in sync with nsdynmemlib "REGION_COUNT"
#define NS_MEM_REGION_CNT  (3)
// size of nsdynmemlib book keeping data ns_mem_book_t
#define NS_MEM_BOOK_SIZE (64 + (NS_MEM_REGION_CNT-1)*2*sizeof(ns_mem_heap_size_t))
#define NS_MEM_BOOK_SIZE_WITH_HOLE (NS_MEM_BOOK_SIZE + 2*sizeof(ns_mem_heap_size_t))

int ret_val;

TEST_GROUP(dynmem)
{
    void setup() {
        reset_heap_error();
    }

    void teardown() {
    }
};

TEST(dynmem, init)
{
    uint16_t size = 1000;
    uint8_t *heap = (uint8_t *)malloc(size);
    CHECK(NULL != heap);
    mem_stat_t info;
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(info.heap_sector_size == (size - NS_MEM_BOOK_SIZE));
    CHECK(!heap_have_failed());
    CHECK(ns_dyn_mem_get_mem_stat() == &info);
    free(heap);
}

TEST(dynmem, different_sizes)
{
    reset_heap_error();
    for (uint16_t size = 1000; size < 32768; size++) {
        mem_stat_t info;
        uint8_t *heap = (uint8_t *)malloc(size);
        ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
        CHECK(info.heap_sector_size >= (size - NS_MEM_BOOK_SIZE_WITH_HOLE));
        CHECK(!heap_have_failed());
        CHECK(ns_dyn_mem_alloc(10));
        free(heap);
    }
}

TEST(dynmem, diff_alignment)
{
    uint16_t size = 1000;
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *ptr = heap;
    CHECK(NULL != heap);
    reset_heap_error();
    for (int i = 0; i < 16; i++) {
        ptr++;
        size--;
        ns_dyn_mem_init(ptr, size, &heap_fail_callback, &info);
        CHECK(info.heap_sector_size >= (size - NS_MEM_BOOK_SIZE_WITH_HOLE));
        CHECK(!heap_have_failed());
    }
    free(heap);
}

TEST(dynmem, heap_add_region_api)
{
#if (NS_MEM_REGION_CNT  > 1)
    uint16_t size = 1000;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *heap2add = (uint8_t *)malloc(size);
    uint8_t *heap2add2 = (uint8_t *)malloc(size);

    mem_stat_t info;

    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(info.heap_sector_size == (size - NS_MEM_BOOK_SIZE))

    // param error, return <0
    ret_val = ns_dyn_mem_region_add(NULL, size);
    CHECK(0 != ret_val);

    // param error, return <0
    ret_val = ns_dyn_mem_region_add(heap2add, 0);
    CHECK(0 != ret_val);

    // param error, return <0
    ret_val = ns_dyn_mem_region_add(heap2add, 8);
    CHECK(0 != ret_val);

    // All OK - success, 1 reserved for bookkeeping
    ret_val = ns_dyn_mem_region_add(heap2add, size);
    CHECK(0 == ret_val);

    CHECK(!heap_have_failed());
    CHECK(ns_dyn_mem_get_mem_stat() == &info);
    CHECK(info.heap_sector_size == (2 * size - NS_MEM_BOOK_SIZE))

    // All OK - add more memory again success
    ret_val = ns_dyn_mem_region_add(heap2add2, size);
    CHECK(0 == ret_val);
    CHECK(info.heap_sector_size == (3 * size - NS_MEM_BOOK_SIZE))

    free(heap);
    free(heap2add);
    free(heap2add2);
#endif
}

TEST(dynmem, heap_add_region)
{
#if (NS_MEM_REGION_CNT  > 1 && NS_MEM_REGION_CNT < 4)
    uint16_t size = 200;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *heap_add1 = (uint8_t *)malloc(size);
    uint8_t *heap_add2 = (uint8_t *)malloc(size);
    uint8_t *heap_add3 = (uint8_t *)malloc(size);
    void *p[size * 4];

    mem_stat_t info;

    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(info.heap_sector_size == (size - NS_MEM_BOOK_SIZE))

    ret_val = ns_dyn_mem_region_add(heap_add1, size);
    CHECK(0 == ret_val);

    // region already added, therefore fails
    ret_val = ns_dyn_mem_region_add(heap_add1, size);
    CHECK(0 != ret_val);

    ret_val = ns_dyn_mem_region_add(heap_add3, size);
    CHECK(0 == ret_val);

    // There is room for 2 additional regions , therfore fails
    ret_val = ns_dyn_mem_region_add(heap_add2, size);
    CHECK(0 != ret_val);

    CHECK(info.heap_sector_size == (3 * size - NS_MEM_BOOK_SIZE))

    CHECK(!heap_have_failed());
    int block_size = 10;

    int i;
    for (i = 0; i < size * 3; i++) {
        p[i] = ns_dyn_mem_alloc(block_size);
        if (p[i]) {
            memset(p[i], 0xb0, block_size);
        } else {
            break;
        }
    }
    CHECK(!heap_have_failed());
    CHECK(info.heap_alloc_fail_cnt == 1);
    CHECK(info.heap_sector_alloc_cnt == i);
    CHECK(info.heap_sector_allocated_bytes == info.heap_sector_allocated_bytes_max);

    for (; i >= 0; i--) {
        ns_dyn_mem_free(p[i]);
    }
    CHECK(!heap_have_failed());
    CHECK(info.heap_sector_alloc_cnt == 0);

    free(heap);
    free(heap_add1);
    free(heap_add2);
    free(heap_add3);
#endif
}

TEST(dynmem, heap_add_region_randomized)
{
    /**
     * Test:
     *  - multiple regions
     *  - regions are not from continous
     *  - all memory allocated from heap
     *  - blocks are deallocated in random order
     */
#if (NS_MEM_REGION_CNT  > 1)
    uint32_t size = 200000;
    uint8_t *heap_ptr[NS_MEM_REGION_CNT];  // heap memory regions
    void *ptrs[size * NS_MEM_REGION_CNT / 4] = {0}; // allocated memory pointers
    mem_stat_t info;
    uint8_t *gap_between_regions = NULL;

    for (int cnt = 0; cnt < NS_MEM_REGION_CNT; cnt++) {
        heap_ptr[cnt] = (uint8_t *)malloc(size);
        if (gap_between_regions) {
            free(gap_between_regions);
        }
        gap_between_regions = (uint8_t *)malloc(size / 3);
    }
    free(gap_between_regions);

    reset_heap_error();
    ns_dyn_mem_init(heap_ptr[0], size, &heap_fail_callback, &info);
    CHECK(info.heap_sector_size == (size - NS_MEM_BOOK_SIZE))

    for (int cnt = NS_MEM_REGION_CNT - 1; cnt > 0; cnt--) {
        ret_val = ns_dyn_mem_region_add(heap_ptr[cnt], size);
        CHECK(0 == ret_val);
    }

    CHECK(info.heap_sector_size == (NS_MEM_REGION_CNT * size - NS_MEM_BOOK_SIZE))

    CHECK(!heap_have_failed());

    srand(time(NULL));
    int block_size;
    int i;
    for (i = 0; i < size * NS_MEM_REGION_CNT; i++) {
        // allocate huge amount of small blocks until allocation fails
        block_size = (rand() % 4) + 1;
        ptrs[i] = ns_dyn_mem_alloc(block_size);
        if (ptrs[i]) {
            memset(ptrs[i], 0xb0, block_size);
        } else {
            break;
        }
    }
    CHECK(!heap_have_failed());
    CHECK(info.heap_alloc_fail_cnt == 1);
    CHECK(info.heap_sector_alloc_cnt == i);
    CHECK(info.heap_sector_allocated_bytes == info.heap_sector_allocated_bytes_max);

    // free allocated memmory blocks in random order
    int block_id;
    do {
        block_id = (rand() % i);
        if (ptrs[block_id] != NULL) {
            ns_dyn_mem_free(ptrs[block_id]);
            ptrs[block_id] = NULL;
        }
    } while (info.heap_sector_alloc_cnt != 0);


    CHECK(!heap_have_failed());
    CHECK(info.heap_sector_alloc_cnt == 0);
    CHECK(info.heap_sector_allocated_bytes == 0);

    for (int cnt = 0; cnt < NS_MEM_REGION_CNT; cnt++) {
        free(heap_ptr[cnt]);
    }
#endif
}

TEST(dynmem, ns_dyn_mem_alloc)
{
    uint16_t size = 1000;
    mem_stat_t info;
    void *p[size];
    uint8_t *heap = (uint8_t *)malloc(size);
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    int block = 1;

    int i;
    for (i = 0; i < size; i++) {
        p[i] = ns_dyn_mem_alloc(block);

        if (p[i]) {
            memset(p[i], 0xb0, block);
        } else {
            break;
        }
    }
    CHECK(!heap_have_failed());
    CHECK(info.heap_alloc_fail_cnt == 1);
    CHECK(info.heap_sector_alloc_cnt == i);
    CHECK(info.heap_sector_allocated_bytes == info.heap_sector_allocated_bytes_max);

    for (; i >= 0; i--) {
        ns_dyn_mem_free(p[i]);
    }
    CHECK(!heap_have_failed());
    CHECK(info.heap_sector_alloc_cnt == 0);
    free(heap);
}

TEST(dynmem, ns_dyn_mem_temporary_alloc)
{
    uint16_t size = 1000;
    mem_stat_t info;
    void *p[size];
    uint8_t *heap = (uint8_t *)malloc(size);
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    int block = 1;

    int i;
    for (i = 0; i < size; i++) {
        p[i] = ns_dyn_mem_temporary_alloc(block);
        if (!p[i]) {
            break;
        }
    }
    CHECK(!heap_have_failed());
    CHECK(info.heap_alloc_fail_cnt == 1);
    CHECK(info.heap_sector_alloc_cnt == i);
    CHECK(info.heap_sector_allocated_bytes == info.heap_sector_allocated_bytes_max);

    for (; i >= 0; i--) {
        ns_dyn_mem_free(p[i]);
    }
    CHECK(!heap_have_failed());
    CHECK(info.heap_sector_alloc_cnt == 0);
    free(heap);
}

TEST(dynmem, ns_dyn_mem_temporary_alloc_with_heap_threshold)
{
    uint16_t size = 1000;
    mem_stat_t info;
    void *p1, *p2;
    int ret_val;
    uint8_t *heap = (uint8_t *)malloc(size);
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());

    // test1: temporary alloc will fail if there is less than 5% heap free
    p1 = ns_dyn_mem_temporary_alloc((size - NS_MEM_BOOK_SIZE_WITH_HOLE) * 0.96);
    CHECK(!heap_have_failed());
    CHECK(p1);
    p2 = ns_dyn_mem_temporary_alloc((size - NS_MEM_BOOK_SIZE_WITH_HOLE) * 0.02);
    CHECK(p2 == NULL);
    CHECK(!heap_have_failed());
    CHECK(info.heap_alloc_fail_cnt == 1);

    // Test2, disable threshold feature and try p2 allocation again
    ns_dyn_mem_set_temporary_alloc_free_heap_threshold(0, 0);
    p2 = ns_dyn_mem_temporary_alloc((size - NS_MEM_BOOK_SIZE_WITH_HOLE) * 0.02);
    CHECK(!heap_have_failed());
    CHECK(p2);
    ns_dyn_mem_free(p1);
    ns_dyn_mem_free(p2);
    CHECK(info.heap_alloc_fail_cnt == 1);
    CHECK(info.heap_sector_alloc_cnt == 0);

    // Test3, enable feature by free heap percentage
    ns_dyn_mem_set_temporary_alloc_free_heap_threshold(40, 0);
    p1 = ns_dyn_mem_temporary_alloc((size - NS_MEM_BOOK_SIZE_WITH_HOLE) * 0.65);
    CHECK(p1);
    p2 = ns_dyn_mem_temporary_alloc((size - NS_MEM_BOOK_SIZE_WITH_HOLE) * 0.10);
    CHECK(p2 == NULL);
    ns_dyn_mem_free(p1);
    CHECK(!heap_have_failed());
    CHECK(info.heap_alloc_fail_cnt == 2);
    CHECK(info.heap_sector_alloc_cnt == 0);

    // Test4, enable feature by free heap amount
    ns_dyn_mem_set_temporary_alloc_free_heap_threshold(0, 200);
    p1 = ns_dyn_mem_temporary_alloc(size - NS_MEM_BOOK_SIZE_WITH_HOLE - 100);
    CHECK(p1);
    p2 = ns_dyn_mem_temporary_alloc(1);
    CHECK(p2 == NULL);
    ns_dyn_mem_free(p1);

    // Test5, illegal API parameters
    ret_val = ns_dyn_mem_set_temporary_alloc_free_heap_threshold(0, size / 2);
    CHECK(ret_val == -2);
    ret_val = ns_dyn_mem_set_temporary_alloc_free_heap_threshold(0, size * 2);
    CHECK(ret_val == -2);
    ret_val = ns_dyn_mem_set_temporary_alloc_free_heap_threshold(51, 0);
    CHECK(ret_val == -2);
    ret_val = ns_dyn_mem_set_temporary_alloc_free_heap_threshold(255, 0);
    CHECK(ret_val == -2);

    CHECK(!heap_have_failed());
    CHECK(info.heap_alloc_fail_cnt == 3);
    CHECK(info.heap_sector_alloc_cnt == 0);
    free(heap);

    // Test6, feature is disabled if info is not set
    heap = (uint8_t *)malloc(size);
    CHECK(NULL != heap);
    ns_dyn_mem_init(heap, size, &heap_fail_callback, NULL);
    ret_val = ns_dyn_mem_set_temporary_alloc_free_heap_threshold(0, 0);
    CHECK(ret_val == -1);
    CHECK(!heap_have_failed());
    free(heap);
}

TEST(dynmem, test_both_allocs_with_hole_usage)
{
    uint16_t size = NS_MEM_BOOK_SIZE_WITH_HOLE + 15 + 5;
    mem_stat_t info;
    void *p[size];
    uint8_t *heap = (uint8_t *)malloc(size);
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());

    void *ptr = ns_dyn_mem_alloc(15);
    void *ptr2 = ns_dyn_mem_alloc(4);

    ns_dyn_mem_free(ptr);
    ns_dyn_mem_free(ptr2);
    CHECK(info.heap_sector_allocated_bytes == 0);

    void *ptr3 = ns_dyn_mem_temporary_alloc(15);
    void *ptr4 = ns_dyn_mem_temporary_alloc(5);

    ns_dyn_mem_free(ptr3);
    ns_dyn_mem_free(ptr4);


    CHECK(info.heap_sector_allocated_bytes == 0);

    free(heap);
}

TEST(dynmem, test_temp_alloc_with_skipping_hole)
{
    uint16_t size = 1000;
    mem_stat_t info;
    void *p[size];
    uint8_t *heap = (uint8_t *)malloc(size);
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());

    void *ptr1 = ns_dyn_mem_temporary_alloc(15);
    void *ptr2 = ns_dyn_mem_temporary_alloc(5);

    ns_dyn_mem_free(ptr1);
    void *ptr3 = ns_dyn_mem_temporary_alloc(35);
    ns_dyn_mem_free(ptr2);
    ns_dyn_mem_free(ptr3);


    CHECK(info.heap_sector_allocated_bytes == 0);

    free(heap);
}

TEST(dynmem, zero_allocate)
{
    uint16_t size = 1000;
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *ptr = heap;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    ns_dyn_mem_alloc(0);
    CHECK(heap_have_failed());
    CHECK(NS_DYN_MEM_ALLOCATE_SIZE_NOT_VALID == current_heap_error);
    free(heap);
}

TEST(dynmem, too_big)
{
    uint16_t size = 1000;
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *ptr = heap;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    ns_dyn_mem_alloc(size);
    CHECK(heap_have_failed());
    CHECK(NS_DYN_MEM_ALLOCATE_SIZE_NOT_VALID == current_heap_error);
    free(heap);
}

TEST(dynmem, corrupted_memory)
{
    uint16_t size = 1000;
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *ptr = heap;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    int *pt = (int *)ns_dyn_mem_alloc(8);
    CHECK(!heap_have_failed());
    pt -= 2;
    *pt = 0;
    ns_dyn_mem_alloc(8);
    CHECK(NS_DYN_MEM_HEAP_SECTOR_CORRUPTED == current_heap_error);
    free(heap);
}

TEST(dynmem, no_big_enough_sector)
{
    uint16_t size = NS_MEM_BOOK_SIZE_WITH_HOLE + (5 * 8);
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *ptr = heap;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    int *pt = (int *)ns_dyn_mem_alloc(8);
    pt = (int *)ns_dyn_mem_alloc(8);
    ns_dyn_mem_alloc(8);
    ns_dyn_mem_temporary_alloc(8);
    ns_dyn_mem_temporary_alloc(8);

    ns_dyn_mem_free(pt);

    pt = (int *)ns_dyn_mem_temporary_alloc(32);
    CHECK(NULL == pt);
    free(heap);
}

TEST(dynmem, diff_sizes)
{
    uint16_t size = 1000;
    mem_stat_t info;
    void *p;
    uint8_t *heap = (uint8_t *)malloc(size);
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    int i;
    for (i = 1; i < (size - NS_MEM_BOOK_SIZE_WITH_HOLE); i++) {
        p = ns_dyn_mem_temporary_alloc(i);
        CHECK(p);
        ns_dyn_mem_free(p);
        CHECK(!heap_have_failed());
    }
    CHECK(!heap_have_failed());
    CHECK(info.heap_sector_alloc_cnt == 0);
    free(heap);
}

TEST(dynmem, double_free)
{
    uint16_t size = 1000;
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    void *p;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    p = ns_dyn_mem_alloc(100);
    CHECK(p);
    ns_dyn_mem_free(p);
    CHECK(!heap_have_failed());
    ns_dyn_mem_free(p);
    CHECK(heap_have_failed());
    CHECK(NS_DYN_MEM_DOUBLE_FREE == current_heap_error);
    free(heap);
}

TEST(dynmem, middle_free)
{
    uint16_t size = 1000;
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    void *p[3];
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    for (int i = 0; i < 3; i++) {
        p[i] = ns_dyn_mem_temporary_alloc(100);
        CHECK(p);
    }
    ns_dyn_mem_free(p[1]);
    CHECK(!heap_have_failed());
    ns_dyn_mem_free(p[0]);
    CHECK(!heap_have_failed());
    ns_dyn_mem_free(p[2]);
    CHECK(!heap_have_failed());
    free(heap);
}

TEST(dynmem, over_by_one)
{
    uint16_t size = 1000;
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *p;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    p = (uint8_t *)ns_dyn_mem_alloc(100);
    CHECK(p);
    p[100] = 0xff;
    ns_dyn_mem_free(p);
    CHECK(heap_have_failed());
    CHECK(NS_DYN_MEM_HEAP_SECTOR_CORRUPTED == current_heap_error);
    free(heap);
}

TEST(dynmem, not_from_this_heap)
{
    uint16_t size = 1000;
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *p;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    p = (uint8_t *)ns_dyn_mem_alloc(100);
    CHECK(p);
    ns_dyn_mem_free(&heap[-1]);
    CHECK(heap_have_failed());
    CHECK(NS_DYN_MEM_POINTER_NOT_VALID == current_heap_error);
    reset_heap_error();
    ns_dyn_mem_free(&heap[1001]);
    CHECK(heap_have_failed());
    CHECK(NS_DYN_MEM_POINTER_NOT_VALID == current_heap_error);
    free(heap);
}

TEST(dynmem, free_on_empty_heap)
{
    uint16_t size = 1000;
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *p;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    ns_dyn_mem_free(&heap[1]);
    CHECK(heap_have_failed());
    CHECK(NS_DYN_MEM_POINTER_NOT_VALID == current_heap_error);
    free(heap);
}

TEST(dynmem, not_negative_stats)
{
    uint16_t size = 1000;
    mem_stat_t info;
    uint8_t *heap = (uint8_t *)malloc(size);
    void *p;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, &info);
    CHECK(!heap_have_failed());
    CHECK(info.heap_sector_allocated_bytes == 0);
    ns_dyn_mem_alloc(8);
    p = ns_dyn_mem_alloc(8);
    ns_dyn_mem_alloc(8);
    CHECK(info.heap_sector_allocated_bytes >= 24);
    int16_t last_value = info.heap_sector_allocated_bytes;
    ns_dyn_mem_free(p);
    CHECK(info.heap_sector_allocated_bytes >= 16);
    CHECK(info.heap_sector_allocated_bytes < last_value);
    last_value = info.heap_sector_allocated_bytes;
    for (int i = 0; i < 10; i++) {
        p = ns_dyn_mem_alloc(1);
        ns_dyn_mem_free(p);
    }
    CHECK(info.heap_sector_allocated_bytes == last_value);
    free(heap);
}

TEST(dynmem, test_invalid_pointer_freed)
{
    uint16_t size = 1000;
    uint8_t *heap = (uint8_t *)malloc(size);
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, NULL);
    int *ptr = (int *)ns_dyn_mem_alloc(4);
    ptr--;
    *ptr = 16;
    ptr++;
    ns_dyn_mem_free(ptr);
    CHECK(NS_DYN_MEM_POINTER_NOT_VALID == current_heap_error);

    free(heap);
}

TEST(dynmem, test_merge_corrupted_previous_block)
{
    uint16_t size = 1000;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *p;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, NULL);
    CHECK(!heap_have_failed());

    int *ptr = (int *)ns_dyn_mem_alloc(4);
    int *ptr2 = (int *)ns_dyn_mem_alloc(4);
    ns_dyn_mem_free(ptr);
    ptr = ptr2 - 2;
    *ptr = -2;
    ns_dyn_mem_free(ptr2);

    CHECK(NS_DYN_MEM_HEAP_SECTOR_CORRUPTED == current_heap_error);

    free(heap);
}

TEST(dynmem, test_free_corrupted_next_block)
{
    uint16_t size = 1000;
    uint8_t *heap = (uint8_t *)malloc(size);
    uint8_t *p;
    CHECK(NULL != heap);
    reset_heap_error();
    ns_dyn_mem_init(heap, size, &heap_fail_callback, NULL);
    CHECK(!heap_have_failed());

    int *ptr = (int *)ns_dyn_mem_temporary_alloc(4);
    int *ptr2 = (int *)ns_dyn_mem_temporary_alloc(4);
    ns_dyn_mem_free(ptr);
    ptr = ptr2 + 2;
    *ptr = -2;
    ns_dyn_mem_free(ptr2);

    CHECK(NS_DYN_MEM_HEAP_SECTOR_CORRUPTED == current_heap_error);

    free(heap);
}

//NOTE! This test must be last!
TEST(dynmem, uninitialized_test)
{
    void *p = ns_dyn_mem_alloc(4);
    ns_dyn_mem_free(p);
    CHECK(p == NULL);
}
