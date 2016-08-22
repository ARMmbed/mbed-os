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
#include "mbed_mem_trace.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifndef MBED_MEM_TRACING_ENABLED
  #error [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

/******************************************************************************/
/* Helper functions and data structures                                       */
/******************************************************************************/

// This structure keeps data about the various memory allocation operations,
// as traced by 'test_trace_cb' below.
#define TEST_MAX_MEMORY_OPS             10
// Trace results for all possible operations
typedef struct {
    uint8_t op;
    void *res;
    union {
        struct {
            size_t arg_size;
        } malloc_info;
        struct {
            void *arg_ptr;
            size_t arg_size;
        } realloc_info;
        struct {
            size_t arg_nmemb;
            size_t arg_size;
        } calloc_info;
        struct {
            void *arg_ptr;
        } free_info;
    };
} mem_trace_data_t;
// Memory operation statistics
typedef struct {
    mem_trace_data_t op_data[TEST_MAX_MEMORY_OPS];
    uint32_t total_ops;
    bool invalid_op, overflow;
} stats_t;
static stats_t stats;

// Clear all the memory statistics
static void test_clear_stats() {
    memset(&stats, 0, sizeof(stats));
}

// Memory tracer callback that records each operation in "stats" (above)
extern "C" void test_trace_cb(uint8_t op, void *res, void *caller, ...) {
    va_list va;
    mem_trace_data_t *pmem = stats.op_data + stats.total_ops;

    if (stats.total_ops >= TEST_MAX_MEMORY_OPS) {
        stats.overflow = true;
        return;
    }
    va_start(va, caller);
    pmem->op = op;
    pmem->res = res;
    switch(op) {
        case MBED_MEM_TRACE_MALLOC:
            pmem->malloc_info.arg_size = va_arg(va, size_t);
            break;

        case MBED_MEM_TRACE_REALLOC:
            pmem->realloc_info.arg_ptr = va_arg(va, void *);
            pmem->realloc_info.arg_size = va_arg(va, size_t);
            break;

        case MBED_MEM_TRACE_CALLOC:
            pmem->calloc_info.arg_nmemb = va_arg(va, size_t);
            pmem->calloc_info.arg_size = va_arg(va, size_t);
            break;

        case MBED_MEM_TRACE_FREE:
            pmem->free_info.arg_ptr = va_arg(va, void *);
            break;

        default:
            stats.invalid_op = true;
    }
    stats.total_ops ++;
    va_end(va);
}

// Generic sanity checks for the tracer
static void check_sanity(uint32_t expected_ops) {
    TEST_ASSERT_FALSE(stats.overflow);
    TEST_ASSERT_FALSE(stats.invalid_op);
    TEST_ASSERT_EQUAL_UINT32(stats.total_ops, expected_ops);
}

// Check a "malloc" operation
static void check_malloc_op(const mem_trace_data_t *p, void *expected_res, size_t expected_arg_size) {
    TEST_ASSERT_EQUAL_UINT8(p->op, MBED_MEM_TRACE_MALLOC);
    TEST_ASSERT_EQUAL_PTR(p->res, expected_res);
    TEST_ASSERT_EQUAL_UINT32(p->malloc_info.arg_size, expected_arg_size);
}

// Check a "free" operation
static void check_free_op(const mem_trace_data_t *p, void *expected_arg_ptr) {
    TEST_ASSERT_EQUAL_UINT8(p->op, MBED_MEM_TRACE_FREE);
    TEST_ASSERT_EQUAL_PTR(p->free_info.arg_ptr, expected_arg_ptr);
}

// Check a "realloc" operation
static void check_realloc_op(const mem_trace_data_t *p, void *expected_res, void *expected_arg_ptr, size_t expected_arg_size) {
    TEST_ASSERT_EQUAL_UINT8(p->op, MBED_MEM_TRACE_REALLOC);
    TEST_ASSERT_EQUAL_PTR(p->res, expected_res);
    TEST_ASSERT_EQUAL_UINT32(p->realloc_info.arg_ptr, expected_arg_ptr);
    TEST_ASSERT_EQUAL_UINT32(p->realloc_info.arg_size, expected_arg_size);
}

// Check a "calloc" operation
static void check_calloc_op(const mem_trace_data_t *p, void *expected_res, size_t expected_arg_nmemb, size_t expected_arg_size) {
    TEST_ASSERT_EQUAL_UINT8(p->op, MBED_MEM_TRACE_CALLOC);
    TEST_ASSERT_EQUAL_PTR(p->res, expected_res);
    TEST_ASSERT_EQUAL_UINT32(p->calloc_info.arg_nmemb, expected_arg_nmemb);
    TEST_ASSERT_EQUAL_UINT32(p->calloc_info.arg_size, expected_arg_size);
}

/******************************************************************************/
/* Tests                                                                      */
/******************************************************************************/

// Allocate a single buffer, then free it. Check that tracing matches the operations.
static void test_case_single_malloc_free() {
    const size_t block_size = 126;
    const mem_trace_data_t *pmem = stats.op_data;

    test_clear_stats();
    mbed_mem_trace_set_callback(test_trace_cb);
    // Allocate a single memory block
    void *p = malloc(block_size);
    TEST_ASSERT_NOT_EQUAL(p, NULL);
    // Free the memory block
    free(p);
    // Stop tracing
    mbed_mem_trace_set_callback(NULL);
    // Check tracer result
    check_sanity(2);
    check_malloc_op(pmem ++, p, block_size);
    check_free_op(pmem, p);
}

// Test all memory operations (malloc, realloc, free, calloc)
static void test_case_all_memory_ops() {
    const size_t malloc_size = 40, realloc_size = 80, nmemb = 25, size = 10;
    const mem_trace_data_t *pmem = stats.op_data;

    test_clear_stats();
    mbed_mem_trace_set_callback(test_trace_cb);
    // Allocate a single memory block, the realloc it
    void *p_malloc = malloc(malloc_size);
    TEST_ASSERT_NOT_EQUAL(p_malloc, NULL);
    void *p_realloc = realloc(p_malloc, realloc_size);
    TEST_ASSERT_NOT_EQUAL(p_realloc, NULL);
    // Use calloc() now
    void *p_calloc = calloc(nmemb, size);
    //TEST_ASSERT_NOT_EQUAL(p_calloc, NULL);
    // Free the realloc() pointer first, then the calloc() one
    free(p_realloc);
    free(p_calloc);
    // Stop tracing
    mbed_mem_trace_set_callback(NULL);
    // Check tracer result
    check_sanity(6);
    check_malloc_op(pmem ++, p_malloc, malloc_size);
    check_realloc_op(pmem ++, p_realloc, p_malloc, realloc_size);
    // calloc() calls malloc() internally
    check_malloc_op(pmem ++, p_calloc, nmemb * size);
    check_calloc_op(pmem ++, p_calloc, nmemb, size);
    check_free_op(pmem ++, p_realloc);
    check_free_op(pmem, p_calloc);
}

// Test that tracing is off when using a NULL callback
static void test_case_trace_off() {
    const size_t malloc_size = 10;

    test_clear_stats();
    // We don't want any tracing
    mbed_mem_trace_set_callback(NULL);
    // Allocate a buffer and free it
    void *p_malloc = malloc(malloc_size);
    TEST_ASSERT_NOT_EQUAL(p_malloc, NULL);
    free(p_malloc);
    // Check that we didn't trace anything
    check_sanity(0);
}

// Test partial tracing (start tracing, stop tracing, restart later)
static void test_case_partial_trace() {
    const size_t malloc_size_1 = 20, malloc_size_2 = 30;
    const mem_trace_data_t *pmem = stats.op_data;

    test_clear_stats();
    // Start tracing
    mbed_mem_trace_set_callback(test_trace_cb);
    // Allocate a buffer
    void *p_malloc_1 = malloc(malloc_size_1);
    TEST_ASSERT_NOT_EQUAL(p_malloc_1, NULL);
    // Disable tracing before freeing the first buffer
    mbed_mem_trace_set_callback(NULL);
    free(p_malloc_1);
    // Allocate another buffer (still not traced)
    void *p_malloc_2 = malloc(malloc_size_2);
    TEST_ASSERT_NOT_EQUAL(p_malloc_2, NULL);
    // Re-enable tracing
    mbed_mem_trace_set_callback(test_trace_cb);
    // And free the second buffer (this operation should be tracer)
    free(p_malloc_2);
    // Stop tracing
    mbed_mem_trace_set_callback(NULL);
    // Check tracer result
    check_sanity(2);
    check_malloc_op(pmem ++, p_malloc_1, malloc_size_1);
    check_free_op(pmem, p_malloc_2);
}

// Test new/delete tracing
static void test_case_new_delete() {
    const mem_trace_data_t *pmem = stats.op_data;

    test_clear_stats();
    // Start tracing
    mbed_mem_trace_set_callback(test_trace_cb);
    // Test new, new[], delete and delete[]
    int *p_int = new int;
    int *p_int_array = new int[10];
    delete p_int;
    delete[] p_int_array;
    // Stop tracing
    mbed_mem_trace_set_callback(NULL);
    // Check tracer result
    check_sanity(4);
    check_malloc_op(pmem ++, p_int, sizeof(int));
    check_malloc_op(pmem ++, p_int_array, 10 * sizeof(int));
    check_free_op(pmem ++, p_int);
    check_free_op(pmem ++, p_int_array);
}

static Case cases[] = {
    Case("single malloc/free", test_case_single_malloc_free),
    Case("all memory operations", test_case_all_memory_ops),
    Case("trace off", test_case_trace_off),
    Case("partial trace", test_case_partial_trace),
    Case("test new/delete", test_case_new_delete)
};

static status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

static Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    // Disable stdout buffering to prevent any unwanted allocations
    setvbuf(stdout, NULL, _IONBF, 0);
    Harness::run(specification);
}

