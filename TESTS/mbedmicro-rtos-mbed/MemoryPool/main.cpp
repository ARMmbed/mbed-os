/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] MemoryPool test cases require a RTOS to run.
#else

#define TEST_ASSERT_DURATION_WITHIN(delta, expected, actual) \
    do { \
        using ct = std::common_type_t<decltype(delta), decltype(expected), decltype(actual)>; \
        TEST_ASSERT_INT_WITHIN(ct(delta).count(), ct(expected).count(), ct(actual).count()); \
    } while (0)

#define TEST_ASSERT_TIME_POINT_WITHIN(delta, expected, actual) \
    do { \
        using ct_tp = std::common_type_t<decltype(expected), decltype(actual)>; \
        using ct = std::common_type_t<decltype(delta), ct_tp::duration>; \
        TEST_ASSERT_INT_WITHIN(ct(delta).count(), ct(expected.time_since_epoch()).count(), ct(actual.time_since_epoch()).count()); \
    } while (0)

using namespace utest::v1;
using namespace std::chrono;

#define THREAD_STACK_SIZE 512
#define TEST_TIMEOUT 50ms

/* Enum used to select block allocation method. */
typedef enum {
    ALLOC, CALLOC
} AllocType;

/* Structure for complex block type. */
typedef struct {
    int a;
    char b;
    int c;
} COMPLEX_TYPE;

/* Function to check if complex type object is cleared.*/
bool comp_is_cleared(COMPLEX_TYPE *object)
{
    if (object->a == 0 && object->b == 0 && object->c == 0) {
        return true;
    }

    return false;
}

/* Function to check if complex type object holds specified values.*/
bool comp_is_equal(COMPLEX_TYPE *object, int a, char b, int c)
{
    if (object->a == a && object->b == b && object->c == c) {
        return true;
    }

    return false;
}

/* Function to set complex type object fields.*/
void comp_set(COMPLEX_TYPE *object, int a, char b, int c)
{
    object->a = a;
    object->b = b;
    object->c = c;
}

/* Template for functional tests for try_alloc(), try_calloc() functions
 * of MemoryPool object.
 *
 * Given MemoryPool object of the specified type and queue size has
 * been successfully created.
 * When max number of blocks is allocated from the pool.
 * Then all allocations are successful.
 *
 * */
template<typename T, const uint32_t numOfEntries>
void test_mem_pool_alloc_success(AllocType atype)
{
    MemoryPool<T, numOfEntries> mem_pool;
    T *p_blocks[numOfEntries];
    uint32_t i;

    /* Test alloc()/calloc() methods - try to allocate max number of
       blocks. All allocations should be successful. */
    for (i = 0; i < numOfEntries; i++) {
        /* Allocate memory block. */
        if (atype == ALLOC) {
            p_blocks[i] = mem_pool.try_alloc();
        } else {
            p_blocks[i] = mem_pool.try_calloc();
        }

        /* Show that memory pool block has been allocated. */
        TEST_ASSERT_NOT_NULL(p_blocks[i]);

        /* Check if Calloc clears the block. */
        if (atype == CALLOC) {
            TEST_ASSERT_EQUAL(0, *p_blocks[i]);
        }

        /* Init fields. */
        *p_blocks[i] = (i + 5);
    }

    /* Check if blocks holds valid values. */
    for (i = 0; i < numOfEntries; i++) {
        TEST_ASSERT_EQUAL((i + 5), *p_blocks[i]);
    }
}

/* Template for functional tests for try_alloc(), try_calloc() functions
 * of MemoryPool object.
 *
 * Complex memory pool block type is used.
 *
 * Given MemoryPool object of the specified type and queue size has
 * been successfully created.
 * When max number of blocks is allocated from the pool.
 * Then all allocations are successful.
 *
 * */
template<typename T, const uint32_t numOfEntries>
void test_mem_pool_alloc_success_complex(AllocType atype)
{
    MemoryPool<T, numOfEntries> mem_pool;
    T *p_blocks[numOfEntries];
    uint32_t i;

    /* Test alloc()/calloc() methods - try to allocate max number of
       blocks. All allocations should be successful. */
    for (i = 0; i < numOfEntries; i++) {
        /* Allocate memory block. */
        if (atype == ALLOC) {
            p_blocks[i] = mem_pool.try_alloc();
        } else {
            p_blocks[i] = mem_pool.try_calloc();
        }

        /* Show that memory pool block has been allocated. */
        TEST_ASSERT_NOT_NULL(p_blocks[i]);

        /* Check if Calloc clears the block. */
        if (atype == CALLOC) {
            TEST_ASSERT_EQUAL(true, comp_is_cleared(p_blocks[i]));
        }

        /* Init fields. */
        comp_set(p_blocks[i], i + 1, i + 2, i + 3);
    }

    /* Check if blocks holds valid values. */
    for (i = 0; i < numOfEntries; i++) {
        TEST_ASSERT_EQUAL(true, comp_is_equal(p_blocks[i], i + 1, i + 2, i + 3));
    }
}

/* Template for functional tests for try_alloc(), try_calloc() functions
 * of MemoryPool object.
 *
 * Given MemoryPool has already max number of blocks allocated from the pool.
 * When next block is allocated.
 * Then allocation fails.
 *
 * */
template<typename T, const uint32_t numOfEntries>
void test_mem_pool_alloc_fail(AllocType atype)
{
    MemoryPool<T, numOfEntries> mem_pool;
    T *p_blocks[numOfEntries];
    T *p_extra_block;
    uint32_t i;

    /* Allocate all available blocks. */
    for (i = 0; i < numOfEntries; i++) {
        if (atype == ALLOC) {
            p_blocks[i] = mem_pool.try_alloc();
        } else {
            p_blocks[i] = mem_pool.try_calloc();
        }

        /* Show that memory pool block has been allocated. */
        TEST_ASSERT_NOT_NULL(p_blocks[i]);
    }

    /* There are no more blocks available. Try to allocate another block. */
    if (atype == ALLOC) {
        p_extra_block = mem_pool.try_alloc();
    } else {
        p_extra_block = mem_pool.try_calloc();
    }

    /* Show that memory pool block has NOT been allocated. */
    TEST_ASSERT_NULL(p_extra_block);
}

/* Template for functional tests for free() function
 * of MemoryPool object.
 *
 * Given MemoryPool has all blocks allocated.
 * When free operation is executed on the each allocated block.
 * Then each deallocation is successfully performed.
 *
 * */
template<typename T, const uint32_t numOfEntries>
void test_mem_pool_free_success(AllocType atype)
{
    MemoryPool<T, numOfEntries> mem_pool;
    T *p_blocks[numOfEntries];
    uint32_t i;
    osStatus status;

    /* Allocate all available blocks. */
    for (i = 0; i < numOfEntries; i++) {
        if (atype == ALLOC) {
            p_blocks[i] = mem_pool.try_alloc();
        } else {
            p_blocks[i] = mem_pool.try_calloc();
        }

        /* Show that memory pool block has been allocated. */
        TEST_ASSERT_NOT_NULL(p_blocks[i]);
    }

    /* Free all memory blocks. */
    for (i = 0; i < numOfEntries; i++) {
        status = mem_pool.free(p_blocks[i]);

        /* Check operation status. */
        TEST_ASSERT_EQUAL(osOK, status);
    }
}

/* Template for functional tests for try_alloc(), try_calloc() functions
 * of MemoryPool object.
 *
 * Basic memory pool block type is used.
 *
 * Given MemoryPool had all blocks allocated and one block has
 * been freed (last).
 * When next block is allocated.
 * Then allocation is successful.
 *
 * */
template<typename T, const uint32_t numOfEntries>
void test_mem_pool_free_realloc_last(AllocType atype)
{
    MemoryPool<T, numOfEntries> mem_pool;
    T *p_blocks[numOfEntries];
    uint32_t i;
    osStatus status;

    /* Allocate all available blocks. */
    for (i = 0; i < numOfEntries; i++) {
        if (atype == ALLOC) {
            p_blocks[i] = mem_pool.try_alloc();
        } else {
            p_blocks[i] = mem_pool.try_calloc();
        }

        /* Init block. */
        *p_blocks[i] = 0xAB;

        /* Show that memory pool block has been allocated. */
        TEST_ASSERT_NOT_NULL(p_blocks[i]);
    }

    /* Free the last block. */
    status = mem_pool.free(p_blocks[numOfEntries - 1]);

    /* Check status. */
    TEST_ASSERT_EQUAL(osOK, status);

    /* Try to allocate another block (one block is now available). */
    if (atype == ALLOC) {
        p_blocks[numOfEntries - 1] = mem_pool.try_alloc();
    } else {
        p_blocks[numOfEntries - 1] = mem_pool.try_calloc();
    }

    /* Show that memory pool block has been now allocated. */
    TEST_ASSERT_NOT_NULL(p_blocks[numOfEntries - 1]);

    /* Check if Calloc clears the block. */
    if (atype == CALLOC) {
        TEST_ASSERT_EQUAL(0, *p_blocks[numOfEntries - 1]);
    }
}

/* Template for functional tests for try_alloc(), try_calloc() functions
 * of MemoryPool object.
 *
 * Complex memory pool block type is used.
 *
 * Given MemoryPool had all blocks allocated and one block has
 * been freed (last).
 * When next block is allocated.
 * Then allocation is successful.
 *
 * */
template<typename T, const uint32_t numOfEntries>
void test_mem_pool_free_realloc_last_complex(AllocType atype)
{
    MemoryPool<T, numOfEntries> mem_pool;
    T *p_blocks[numOfEntries];
    uint32_t i;
    osStatus status;

    /* Allocate all available blocks. */
    for (i = 0; i < numOfEntries; i++) {
        if (atype == ALLOC) {
            p_blocks[i] = mem_pool.try_alloc();
        } else {
            p_blocks[i] = mem_pool.try_calloc();
        }

        /* Init block. */
        comp_set(p_blocks[i], i + 1, i + 2, i + 3);

        /* Show that memory pool block has been allocated. */
        TEST_ASSERT_NOT_NULL(p_blocks[i]);
    }

    /* Free the last block. */
    status = mem_pool.free(p_blocks[numOfEntries - 1]);

    /* Check status. */
    TEST_ASSERT_EQUAL(osOK, status);

    /* Try to allocate another block (one block is now available). */
    if (atype == ALLOC) {
        p_blocks[numOfEntries - 1] = mem_pool.try_alloc();
    } else {
        p_blocks[numOfEntries - 1] = mem_pool.try_calloc();
    }

    /* Show that memory pool block has been now allocated. */
    TEST_ASSERT_NOT_NULL(p_blocks[numOfEntries - 1]);

    /* Check if Calloc clears the block. */
    if (atype == CALLOC) {
        TEST_ASSERT_EQUAL(true, comp_is_cleared(p_blocks[numOfEntries - 1]));
    }
}

/* Template for functional tests for try_alloc(), try_calloc() functions
 * of MemoryPool object.
 *
 * Basic memory pool block type is used.
 *
 * Given MemoryPool had all blocks allocated and one block has
 * been freed (first).
 * When next block is allocated.
 * Then allocation is successful.
 *
 * */
template<typename T, const uint32_t numOfEntries>
void test_mem_pool_free_realloc_first(AllocType atype)
{
    MemoryPool<T, numOfEntries> mem_pool;
    T *p_blocks[numOfEntries];
    uint32_t i;
    osStatus status;

    /* Allocate all available blocks. */
    for (i = 0; i < numOfEntries; i++) {
        if (atype == ALLOC) {
            p_blocks[i] = mem_pool.try_alloc();
        } else {
            p_blocks[i] = mem_pool.try_calloc();
        }

        /* Init block. */
        *p_blocks[i] = 0xAB;

        /* Show that memory pool block has been allocated. */
        TEST_ASSERT_NOT_NULL(p_blocks[i]);
    }

    /* Free the last block. */
    status = mem_pool.free(p_blocks[0]);

    /* Check status. */
    TEST_ASSERT_EQUAL(osOK, status);

    /* Try to allocate another block (one block is now available). */
    if (atype == ALLOC) {
        p_blocks[0] = mem_pool.try_alloc();
    } else {
        p_blocks[0] = mem_pool.try_calloc();
    }

    /* Show that memory pool block has been now allocated. */
    TEST_ASSERT_NOT_NULL(p_blocks[0]);

    /* Check if Calloc clears the block. */
    if (atype == CALLOC) {
        TEST_ASSERT_EQUAL(0, *p_blocks[0]);
    }
}

/* Template for functional tests for try_alloc(), try_calloc() functions
 * of MemoryPool object.
 *
 * Complex memory pool block type is used.
 *
 * Given MemoryPool had all blocks allocated and one block has
 * been freed (first).
 * When next block is allocated.
 * Then allocation is successful.
 *
 * */
template<typename T, const uint32_t numOfEntries>
void test_mem_pool_free_realloc_first_complex(AllocType atype)
{
    MemoryPool<T, numOfEntries> mem_pool;
    T *p_blocks[numOfEntries];
    uint32_t i;
    osStatus status;

    /* Allocate all available blocks. */
    for (i = 0; i < numOfEntries; i++) {
        if (atype == ALLOC) {
            p_blocks[i] = mem_pool.try_alloc();
        } else {
            p_blocks[i] = mem_pool.try_calloc();
        }

        /* Init block. */
        comp_set(p_blocks[i], i + 1, i + 2, i + 3);

        /* Show that memory pool block has been allocated. */
        TEST_ASSERT_NOT_NULL(p_blocks[i]);
    }

    /* Free the last block. */
    status = mem_pool.free(p_blocks[0]);

    /* Check status. */
    TEST_ASSERT_EQUAL(osOK, status);

    /* Try to allocate another block (one block is now available). */
    if (atype == ALLOC) {
        p_blocks[0] = mem_pool.try_alloc();
    } else {
        p_blocks[0] = mem_pool.try_calloc();
    }

    /* Show that memory pool block has been now allocated. */
    TEST_ASSERT_NOT_NULL(p_blocks[0]);

    /* Check if Calloc clears the block. */
    if (atype == CALLOC) {
        TEST_ASSERT_EQUAL(true, comp_is_cleared(p_blocks[0]));
    }
}

/* Test try_alloc_for/try_alloc_until timeout
 *
 * Given a pool with one slot for int data
 * When a thread tries to allocate two blocks with @ TEST_TIMEOUT timeout
 * Then first operation succeeds immediately and second fails at the correct time.
 */
void test_mem_pool_timeout()
{
    MemoryPool<int, 1> mem_pool;

    Timer timer;
    timer.start();

    int *item = mem_pool.try_alloc_for(TEST_TIMEOUT);
    TEST_ASSERT_NOT_NULL(item);
    TEST_ASSERT_DURATION_WITHIN(TEST_TIMEOUT / 10, 0ms, timer.elapsed_time());

    item = mem_pool.try_alloc_for(TEST_TIMEOUT);
    TEST_ASSERT_NULL(item);
    TEST_ASSERT_DURATION_WITHIN(TEST_TIMEOUT / 10, TEST_TIMEOUT, timer.elapsed_time());

    auto end_time = Kernel::Clock::now() + TEST_TIMEOUT;
    item = mem_pool.try_alloc_until(end_time);
    TEST_ASSERT_NULL(item);
    TEST_ASSERT_TIME_POINT_WITHIN(TEST_TIMEOUT / 10, end_time, Kernel::Clock::now());
}

namespace {
struct free_capture {
    MemoryPool<int, 1> *pool;
    int *item;
};
}

static void free_int_item(free_capture *to_free)
{
    ThisThread::sleep_for(TEST_TIMEOUT);

    osStatus status = to_free->pool->free(to_free->item);
    TEST_ASSERT_EQUAL(osOK, status);
}

/** Test alloc wait forever
 *
 * Given two threads A & B and a pool with one slot for int data
 * When thread A allocs a block from the pool and tries to alloc a second one with @a osWaitForever timeout
 * Then thread waits for a block to become free in the pool
 * When thread B frees the first block from the pool
 * Then thread A successfully allocs a block from the pool
 */
void test_mem_pool_waitforever()
{
    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    MemoryPool<int, 1> pool;

    Timer timer;
    timer.start();

    int *item = pool.try_alloc_for(Kernel::wait_for_u32_forever);
    TEST_ASSERT_NOT_NULL(item);
    TEST_ASSERT_DURATION_WITHIN(TEST_TIMEOUT / 10, 0ms, timer.elapsed_time());

    struct free_capture to_free;
    to_free.pool = &pool;
    to_free.item = item;
    t.start(callback(free_int_item, &to_free));

    item = pool.try_alloc_for(Kernel::wait_for_u32_forever);
    TEST_ASSERT_EQUAL(item, to_free.item);
    TEST_ASSERT_DURATION_WITHIN(TEST_TIMEOUT / 10, TEST_TIMEOUT, timer.elapsed_time());

    t.join();
}

/* Robustness checks for free() function.
 * Function under test is called with invalid parameters.
 *
 * Given MemoryPool object has been successfully created.
 * When free operation is performed on NULL address.
 * Then deallocation fails with osErrorParameter error.
 *
 */
void free_block_invalid_parameter_null()
{
    MemoryPool<int, 1> mem_pool;
    osStatus status;

    /* Try to free block passing invalid parameter (NULL). */
    status = mem_pool.free(NULL);

    /* Check operation status. */
    TEST_ASSERT_EQUAL(osErrorParameter, status);
}

/* Robustness checks for free() function.
 * Function under test is called with invalid parameters.
 *
 * Given MemoryPool object has been successfully created.
 * When free operation is performed on invalid address.
 * Then deallocation fails with osErrorParameter error.
 *
 */
void free_block_invalid_parameter()
{
    MemoryPool<int, 1> mem_pool;
    osStatus status;

    /* Try to free block passing invalid parameter (variable address). */
    status = mem_pool.free(reinterpret_cast<int *>(&status));

    /* Check operation status. */
    TEST_ASSERT_EQUAL(osErrorParameter, status);
}

/* Use wrapper functions to reduce memory usage. */

template<typename T, const uint32_t numOfEntries>
void test_mem_pool_alloc_success_wrapper()
{
    test_mem_pool_alloc_success<T, numOfEntries>(ALLOC);
    test_mem_pool_alloc_success<T, numOfEntries>(CALLOC);
}

template<typename T, const uint32_t numOfEntries>
void test_mem_pool_alloc_success_complex_wrapper()
{
    test_mem_pool_alloc_success_complex<T, numOfEntries>(ALLOC);
    test_mem_pool_alloc_success_complex<T, numOfEntries>(CALLOC);
}

template<typename T, const uint32_t numOfEntries>
void test_mem_pool_free_success_wrapper()
{
    test_mem_pool_free_success<T, numOfEntries>(ALLOC);
    test_mem_pool_free_success<T, numOfEntries>(CALLOC);
}

template<typename T, const uint32_t numOfEntries>
void test_mem_pool_free_realloc_last_wrapper()
{
    test_mem_pool_free_realloc_last<T, numOfEntries>(ALLOC);
    test_mem_pool_free_realloc_last<T, numOfEntries>(CALLOC);

}

template<typename T, const uint32_t numOfEntries>
void test_mem_pool_free_realloc_first_wrapper()
{
    test_mem_pool_free_realloc_first<T, numOfEntries>(ALLOC);
    test_mem_pool_free_realloc_first<T, numOfEntries>(CALLOC);
}

template<typename T, const uint32_t numOfEntries>
void test_mem_pool_free_realloc_first_complex_wrapper()
{
    test_mem_pool_free_realloc_first_complex<T, numOfEntries>(ALLOC);
    test_mem_pool_free_realloc_first_complex<T, numOfEntries>(CALLOC);
}

template<typename T, const uint32_t numOfEntries>
void test_mem_pool_free_realloc_last_complex_wrapper()
{
    test_mem_pool_free_realloc_last_complex<T, numOfEntries>(ALLOC);
    test_mem_pool_free_realloc_last_complex<T, numOfEntries>(CALLOC);
}

template<typename T, const uint32_t numOfEntries>
void test_mem_pool_alloc_fail_wrapper()
{
    test_mem_pool_alloc_fail<T, numOfEntries>(ALLOC);
    test_mem_pool_alloc_fail<T, numOfEntries>(CALLOC);
}

Case cases[] = {
    Case("Test: try_alloc()/try_calloc() - success, 4 bytes b_type, q_size equal to 1.", test_mem_pool_alloc_success_wrapper<int, 1>),
    Case("Test: try_alloc()/try_calloc() - success, 4 bytes b_type, q_size equal to 3.", test_mem_pool_alloc_success_wrapper<int, 3>),
    Case("Test: try_alloc()/try_calloc() - success, 1 bytes b_type, q_size equal to 1.", test_mem_pool_alloc_success_wrapper<char, 1>),
    Case("Test: try_alloc()/try_calloc() - success, 1 bytes b_type, q_size equal to 3.", test_mem_pool_alloc_success_wrapper<char, 3>),
    Case("Test: try_alloc()/try_calloc() - success, complex b_type, q_size equal to 1.", test_mem_pool_alloc_success_complex_wrapper<COMPLEX_TYPE, 1>),
    Case("Test: try_alloc()/try_calloc() - success, complex b_type, q_size equal to 3.", test_mem_pool_alloc_success_complex_wrapper<COMPLEX_TYPE, 3>),

    Case("Test: free() - success, 4 bytes b_type, q_size equal to 1.", test_mem_pool_free_success_wrapper<int, 1>),
    Case("Test: free() - success, 4 bytes b_type, q_size equal to 3.", test_mem_pool_free_success_wrapper<int, 3>),
    Case("Test: free() - success, complex b_type, q_size equal to 1.", test_mem_pool_free_success_wrapper<COMPLEX_TYPE, 1>),
    Case("Test: free() - success, complex b_type, q_size equal to 3.", test_mem_pool_free_success_wrapper<COMPLEX_TYPE, 3>),

    Case("Test: re-allocation of the last block, basic type.", test_mem_pool_free_realloc_last_wrapper<int, 3>),
    Case("Test: re-allocation of the first block, basic type.", test_mem_pool_free_realloc_first_wrapper<int, 3>),
    Case("Test: re-allocation of the first block, complex type.", test_mem_pool_free_realloc_first_complex_wrapper<COMPLEX_TYPE, 3>),
    Case("Test: re-allocation of the last block, complex type.", test_mem_pool_free_realloc_last_complex_wrapper<COMPLEX_TYPE, 3>),

    Case("Test: fail (out of free blocks).", test_mem_pool_alloc_fail_wrapper<int, 3>),

    Case("Test: timeout", test_mem_pool_timeout),
    Case("Test: wait forever", test_mem_pool_waitforever),

    Case("Test: free() - robust (free called with invalid param - NULL).", free_block_invalid_parameter_null),
    Case("Test: free() - robust (free called with invalid param).", free_block_invalid_parameter)
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

#endif // !defined(MBED_CONF_RTOS_PRESENT)
