/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "utest/utest.h"
#include "unity/unity.h"


#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#if !DEVICE_USTICKER
  #error [NOT_SUPPORTED] test not supported
#endif

using utest::v1::Case;

extern uint32_t mbed_heap_size;
static const int test_timeout = 25;
volatile bool thread_should_continue = true;
#define NUM_THREADS         4
#define THREAD_MALLOC_SIZE  100

#if defined(__CORTEX_A9)
#define THREAD_STACK_SIZE   512
#else
#define THREAD_STACK_SIZE   256
#endif


void task_using_malloc(void)
{
    void *data = NULL;

    while (thread_should_continue) {
        // Repeatedly allocate and free memory
        data = malloc(THREAD_MALLOC_SIZE);
        TEST_ASSERT_NOT_NULL(data);

        // test whole allocated memory
        memset(data, 0, THREAD_MALLOC_SIZE);

        free(data);
    }
}

/** Test for multithreaded heap allocations

    Given multiple threads are started in parallel
    When each of the threads allocate memory
    Then the memory allocation succeed and @a malloc return valid memory
 */
void test_multithread_allocation(void)
{
    // static stack for threads to reduce heap usage on devices with small RAM
    // and eliminate run out of heap memory problem
    uint8_t stack[NUM_THREADS][THREAD_STACK_SIZE];

    bool thread_alloc_failure = false;
    Thread *thread_list[NUM_THREADS];
    int test_time = 20;

    // Allocate threads for the test
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_list[i] = new Thread(osPriorityNormal, THREAD_STACK_SIZE, stack[i]);
        if (NULL == thread_list[i]) {
            thread_alloc_failure = true;
        } else {
            thread_list[i]->start(task_using_malloc);
        }
    }

    // Give the test time to run
    while (test_time--) {
        Thread::wait(1000);
    }

    // Join and delete all threads
    thread_should_continue = false;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (NULL != thread_list[i]) {
            thread_list[i]->join();
            delete thread_list[i];
            thread_list[i] = NULL;
        }
    }
    TEST_ASSERT_FALSE(thread_alloc_failure);
}

/** Test for large heap allocation

    Given a heap of size mbed_heap_size
    When try to allocate memory of size mbed_heap_size/5 (20% of whole heap)
    Then the memory is allocated and @a malloc return valid memory
 */
void test_big_allocation(void)
{
    const uint32_t alloc_size = mbed_heap_size / 5;
    void *data = NULL;

    data = malloc(alloc_size);
    TEST_ASSERT_NOT_NULL(data);

    // test whole allocated memory
    memset(data, 0, alloc_size);

    free(data);
}

/** Test if allocation of zero size does not cause any undefined behaviour

    Given a heap
    When try to allocate memory of size 0
    Then the return value of @a malloc depends on the particular library implementation
     (NULL or smallest possible allocation) and no undefined behaviour happens

    @note If allocation size is zero, the return value depends on the particular library implementation
    (it may or may not be a null pointer), but the returned pointer shall not be dereferenced
 */
void test_zero_allocation(void)
{
    void *data = NULL;

    data = malloc(0);
    if(data != NULL) {
        free(data);
    }
    TEST_ASSERT_MESSAGE(true, "malloc(0) succeed - no undefined behaviour happens");
}

/** Test if free on NULL pointer does not cause any undefined behaviour

    Given a NULL pointer
    When try to free it
    Then the function @a free does nothing and no undefined behaviour happens
 */
void test_null_free(void)
{
    void *data = NULL;
    free(data);

    TEST_ASSERT_MESSAGE(true, "free(NULL) succeed - no undefined behaviour happens");
}

// Test cases
Case cases[] = {
    Case("Test 0 size allocation", test_zero_allocation),
    Case("Test NULL pointer free", test_null_free),
    Case("Test multithreaded allocations", test_multithread_allocation),
    Case("Test large allocation", test_big_allocation)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(test_timeout, "timing_drift_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

utest::v1::Specification specification(greentea_test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
