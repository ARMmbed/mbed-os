/*
 * Copyright (c) 2013-2017, ARM Limited, All Rights Reserved
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

using utest::v1::Case;


/******************************************************************************/
/* Helper functions and data structures                                       */
/******************************************************************************/

#define THREAD_STACK_SIZE 384
#define NUM_TEST_THREADS  3

template<osPriority PRIORITY, uint32_t STACK_SIZE>
class TestThread : public Thread {
    uint8_t stack[STACK_SIZE];
public:
    TestThread() : Thread(PRIORITY, STACK_SIZE, stack) { }
};

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
static void test_clear_stats()
{
    memset(&stats, 0, sizeof(stats));
}

// Memory tracer callback that records each operation in "stats" (above)
extern "C" void test_trace_cb(uint8_t op, void *res, void *caller, ...)
{
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
static void check_sanity(uint32_t expected_ops)
{
    TEST_ASSERT_FALSE(stats.overflow);
    TEST_ASSERT_FALSE(stats.invalid_op);
    TEST_ASSERT_EQUAL_UINT32(expected_ops, stats.total_ops);
}

// Check a "malloc" operation
static void check_malloc_op(const mem_trace_data_t *p, void *expected_res, size_t expected_arg_size)
{
    TEST_ASSERT_EQUAL_UINT8(MBED_MEM_TRACE_MALLOC, p->op);
    TEST_ASSERT_EQUAL_PTR(expected_res, p->res);
    TEST_ASSERT_EQUAL_UINT32(expected_arg_size, p->malloc_info.arg_size);
}

// Check a "free" operation
static void check_free_op(const mem_trace_data_t *p, void *expected_arg_ptr)
{
    TEST_ASSERT_EQUAL_UINT8(MBED_MEM_TRACE_FREE, p->op);
    TEST_ASSERT_EQUAL_PTR(expected_arg_ptr, p->free_info.arg_ptr);
}

// Check a "realloc" operation
static void check_realloc_op(const mem_trace_data_t *p, void *expected_res, void *expected_arg_ptr, size_t expected_arg_size)
{
    TEST_ASSERT_EQUAL_UINT8(MBED_MEM_TRACE_REALLOC, p->op);
    TEST_ASSERT_EQUAL_PTR(expected_res, p->res);
    TEST_ASSERT_EQUAL_UINT32(expected_arg_ptr, p->realloc_info.arg_ptr);
    TEST_ASSERT_EQUAL_UINT32(expected_arg_size, p->realloc_info.arg_size);
}

// Check a "calloc" operation
static void check_calloc_op(const mem_trace_data_t *p, void *expected_res, size_t expected_arg_nmemb, size_t expected_arg_size)
{
    TEST_ASSERT_EQUAL_UINT8(MBED_MEM_TRACE_CALLOC, p->op);
    TEST_ASSERT_EQUAL_PTR(expected_res, p->res);
    TEST_ASSERT_EQUAL_UINT32(expected_arg_nmemb, p->calloc_info.arg_nmemb);
    TEST_ASSERT_EQUAL_UINT32(expected_arg_size, p->calloc_info.arg_size);
}

// Memory tracer callback to test thread safety
extern "C" void test_trace_cb_multithread(uint8_t op, void *res, void *caller, ...)
{
    volatile  static int trace_guard = 0;
    trace_guard++;
    TEST_ASSERT_TRUE_MESSAGE(trace_guard == 1, "Race condition occurred !!!!");
    trace_guard--;
}

// Thread function
void malloc_free(volatile bool *thread_continue)
{
    const size_t block_size = 126;

    while(*thread_continue) {
        void *p = malloc(block_size);
        TEST_ASSERT_NOT_EQUAL(p, NULL);
        free(p);
    }
}


/** Test single malloc/free tracing
 *
 *  Given a memory trace mechanism
 *  When perform single memory allocation/deallocation using malloc/free
 *  Then tracing matches the operations
 *
 */
static void test_case_single_malloc_free()
{
    const uint32_t num_op = 2;
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
    check_sanity(num_op);
    check_malloc_op(pmem++, p, block_size);
    check_free_op(pmem, p);
}


/** Test all memory operations (malloc, realloc, free, calloc) tracing
 *
 *  Given a memory trace mechanism
 *  When perform all memory operations
 *  Then tracing matches the operations
 *
 */
static void test_case_all_memory_ops()
{
    const uint32_t num_op = 5;
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
    TEST_ASSERT_NOT_EQUAL(p_calloc, NULL);

    // Free the realloc() pointer first, then the calloc() one
    free(p_realloc);
    free(p_calloc);

    // Stop tracing
    mbed_mem_trace_set_callback(NULL);

    // Check tracer result
    check_sanity(num_op);
    check_malloc_op(pmem++, p_malloc, malloc_size);
    check_realloc_op(pmem++, p_realloc, p_malloc, realloc_size);
    check_calloc_op(pmem++, p_calloc, nmemb, size);
    check_free_op(pmem++, p_realloc);
    check_free_op(pmem, p_calloc);
}


/** Test that tracing is off when using a NULL callback
 *
 *  Given a memory trace mechanism
 *  When tracing is turned off
 *  Then performed memory operations doesn't report any tracing
 *
 */
static void test_case_trace_off()
{
    const uint32_t num_op = 0;
    const size_t malloc_size = 10;

    test_clear_stats();
    // We don't want any tracing
    mbed_mem_trace_set_callback(NULL);

    // Allocate a buffer and free it
    void *p_malloc = malloc(malloc_size);
    TEST_ASSERT_NOT_EQUAL(p_malloc, NULL);
    free(p_malloc);

    // Check that we didn't trace anything
    check_sanity(num_op);
}


/** Test partial tracing
 *
 *  Given a memory trace mechanism
 *  When perform memory operations while tracing is on then off and on again
 *  Then tracing report only part of operations
 *
 */
static void test_case_partial_trace()
{
    const uint32_t num_op = 2;
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
    check_sanity(num_op);
    check_malloc_op(pmem++, p_malloc_1, malloc_size_1);
    check_free_op(pmem, p_malloc_2);
}


/** Test new/delete tracing
 *
 *  Given a memory trace mechanism
 *  When memory allocation/deallocation is performed using new/delete
 *  Then tracing matches the operations
 *
 */
static void test_case_new_delete()
{
    const uint32_t num_op = 4;
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
    check_sanity(num_op);
    check_malloc_op(pmem++, p_int, sizeof(int));
    check_malloc_op(pmem++, p_int_array, 10 * sizeof(int));
    check_free_op(pmem++, p_int);
    check_free_op(pmem, p_int_array);
}


/** Test tracing thread safety
 *
 *  Given a memory trace mechanism and multiple threads are started in parallel
 *  When each of the threads perform memory allocation/deallocation (thus uses memory trace mechanisms)
 *  Then tracing is protected against simultaneous multithreaded access
 *
 */
static void test_case_multithread_malloc_free()
{
    const uint32_t wait_time_us = 10000;
    volatile bool threads_continue;
    TestThread<osPriorityNormal, THREAD_STACK_SIZE> threads[NUM_TEST_THREADS];

    mbed_mem_trace_set_callback(test_trace_cb_multithread);

    threads_continue = true;
    for (int i = 0; i < NUM_TEST_THREADS; i++) {
        threads[i].start(callback(malloc_free, &threads_continue));
    }

    Thread::wait(wait_time_us);
    threads_continue = false;

    for (int i = 0; i < NUM_TEST_THREADS; i++) {
        threads[i].join();
    }

    mbed_mem_trace_set_callback(NULL);
}



static Case cases[] =
{
    Case("Test single malloc/free trace", test_case_single_malloc_free),
    Case("Test all memory operations trace", test_case_all_memory_ops),
    Case("Test trace off", test_case_trace_off),
    Case("Test partial trace", test_case_partial_trace),
    Case("Test new/delete trace", test_case_new_delete),
    Case("Test multithreaded trace", test_case_multithread_malloc_free)
};

static utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

static utest::v1::Specification specification(greentea_test_setup, cases, utest::v1::greentea_test_teardown_handler);

int main()
{
    // Disable stdout buffering to prevent any unwanted allocations
    setvbuf(stdout, NULL, _IONBF, 0);

    return !utest::v1::Harness::run(specification);
}
