/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "mbed_stats.h"
#include <stdlib.h>
#include <stdio.h>

#if !defined(MBED_HEAP_STATS_ENABLED)
  #error [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

#define ALLOCATION_SIZE_DEFAULT 564
#define ALLOCATION_SIZE_SMALL   124
#define ALLOCATION_SIZE_LARGE   700
#define ALLOCATION_SIZE_FAIL   (1024 * 1024 *1024)

typedef void* (*malloc_cb_t) (uint32_t size);

static void* thunk_malloc(uint32_t size);
static void* thunk_calloc_1(uint32_t size);
static void* thunk_calloc_4(uint32_t size);
static void* thunk_realloc(uint32_t size);

malloc_cb_t malloc_thunk_array[] = {
    thunk_malloc,
    thunk_calloc_1,
    thunk_calloc_4,
    thunk_realloc,
};

void test_case_malloc_free_size()
{
    mbed_stats_heap_t stats_start;
    mbed_stats_heap_t stats_current;
    void *data;

    mbed_stats_heap_get(&stats_start);

    for (uint32_t i = 0; i < sizeof(malloc_thunk_array) / sizeof(malloc_cb_t); i++) {

        // Allocate memory and assert size change
        data = malloc_thunk_array[i](ALLOCATION_SIZE_DEFAULT);
        TEST_ASSERT(data != NULL);
        mbed_stats_heap_get(&stats_current);
        uint32_t increase = stats_current.current_size - stats_start.current_size;
        TEST_ASSERT_EQUAL_UINT32(ALLOCATION_SIZE_DEFAULT, increase);
        TEST_ASSERT_EQUAL_UINT32(stats_start.total_size + ALLOCATION_SIZE_DEFAULT * (i + 1), stats_current.total_size);
        TEST_ASSERT_EQUAL_UINT32(stats_start.alloc_cnt + 1, stats_current.alloc_cnt);
        TEST_ASSERT_EQUAL_UINT32(stats_start.alloc_fail_cnt, stats_current.alloc_fail_cnt);

        // Free memory and assert back to starting size
        free(data);
        mbed_stats_heap_get(&stats_current);
        TEST_ASSERT_EQUAL_UINT32(stats_start.current_size, stats_current.current_size);
        TEST_ASSERT_EQUAL_UINT32(stats_start.alloc_cnt, stats_current.alloc_cnt);
        TEST_ASSERT_EQUAL_UINT32(stats_start.alloc_fail_cnt, stats_current.alloc_fail_cnt);
    }
}

void test_case_allocate_zero()
{
    mbed_stats_heap_t stats_start;
    mbed_stats_heap_t stats_current;
    void *data;

    mbed_stats_heap_get(&stats_start);

    for (uint32_t i = 0; i < sizeof(malloc_thunk_array) / sizeof(malloc_cb_t); i++) {

        // Allocate memory and assert size change
        data = malloc_thunk_array[i](0);
        // Return can be NULL
        mbed_stats_heap_get(&stats_current);
        TEST_ASSERT_EQUAL_UINT32(stats_start.current_size, stats_current.current_size);
        TEST_ASSERT_EQUAL_UINT32(stats_start.total_size, stats_current.total_size);
        TEST_ASSERT_EQUAL_UINT32(stats_start.alloc_fail_cnt, stats_current.alloc_fail_cnt);

        // Free memory and assert back to starting size
        free(data);
        mbed_stats_heap_get(&stats_current);
        TEST_ASSERT_EQUAL_UINT32(stats_start.current_size, stats_current.current_size);
        TEST_ASSERT_EQUAL_UINT32(stats_start.alloc_cnt, stats_current.alloc_cnt);
        TEST_ASSERT_EQUAL_UINT32(stats_start.alloc_fail_cnt, stats_current.alloc_fail_cnt);
    }
}

void test_case_allocate_fail()
{
    mbed_stats_heap_t stats_start;
    mbed_stats_heap_t stats_current;
    void *data;

    mbed_stats_heap_get(&stats_start);

    for (uint32_t i = 0; i < sizeof(malloc_thunk_array) / sizeof(malloc_cb_t); i++) {

        // Trigger a failure by trying to allocate a buffer that won't fit
        data = malloc_thunk_array[i](ALLOCATION_SIZE_FAIL);
        TEST_ASSERT(data == NULL);
        mbed_stats_heap_get(&stats_current);
        TEST_ASSERT_EQUAL_UINT32(stats_start.current_size, stats_current.current_size);
        TEST_ASSERT_EQUAL_UINT32(stats_start.total_size, stats_current.total_size);
        TEST_ASSERT_EQUAL_UINT32(stats_start.alloc_cnt, stats_current.alloc_cnt);
        TEST_ASSERT_EQUAL_UINT32(stats_start.alloc_fail_cnt + i + 1, stats_current.alloc_fail_cnt);
    }
}

static void* thunk_malloc(uint32_t size)
{
    return malloc(size);
}

static void* thunk_calloc_1(uint32_t size)
{
    return calloc(size / 1, 1);
}

static void* thunk_calloc_4(uint32_t size)
{
    return calloc(size / 4, 4);
}


static void* thunk_realloc(uint32_t size)
{
    return realloc(NULL, size);
}

void test_case_realloc_size()
{
    mbed_stats_heap_t stats_start;
    mbed_stats_heap_t stats_current;
    uint32_t increase;
    void *data;

    mbed_stats_heap_get(&stats_start);

    // Allocate memory and assert size change
    data = realloc(NULL, ALLOCATION_SIZE_DEFAULT);
    TEST_ASSERT(data != NULL);
    mbed_stats_heap_get(&stats_current);
    increase = stats_current.current_size - stats_start.current_size;
    TEST_ASSERT_EQUAL_UINT32(increase, ALLOCATION_SIZE_DEFAULT);

    // Decrease size and assert size change
    data = realloc(data, ALLOCATION_SIZE_SMALL);
    TEST_ASSERT(data != NULL);
    mbed_stats_heap_get(&stats_current);
    increase = stats_current.current_size - stats_start.current_size;
    TEST_ASSERT_EQUAL_UINT32(increase, ALLOCATION_SIZE_SMALL);

    // Increase size and assert size change
    data = realloc(data, ALLOCATION_SIZE_LARGE);
    TEST_ASSERT(data != NULL);
    mbed_stats_heap_get(&stats_current);
    increase = stats_current.current_size - stats_start.current_size;
    TEST_ASSERT_EQUAL_UINT32(increase, ALLOCATION_SIZE_LARGE);

    // Free memory and assert back to starting size
    free(data);
    mbed_stats_heap_get(&stats_current);
    TEST_ASSERT_EQUAL_UINT32(stats_start.current_size, stats_current.current_size);
}

Case cases[] = {
    Case("malloc and free size", test_case_malloc_free_size),
    Case("allocate size zero", test_case_allocate_zero),
    Case("allocation failure", test_case_allocate_fail),
    Case("realloc size", test_case_realloc_size),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
