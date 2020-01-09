/*
 * Copyright (c) 2016-2017, ARM Limited, All Rights Reserved
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

#if defined(TARGET_CORTEX_A) || !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported.
#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mbed.h"
#include "cmsis.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using utest::v1::Case;

static const int test_timeout = 30;


// Amount to malloc for each iteration
#define MALLOC_TEST_SIZE            256
// Malloc fill pattern
#define MALLOC_FILL                 0x55

extern unsigned char *mbed_heap_start;
extern uint32_t mbed_heap_size;
extern unsigned char *mbed_stack_isr_start;
extern uint32_t mbed_stack_isr_size;

#if defined(TOOLCHAIN_GCC_ARM) && defined(MBED_SPLIT_HEAP)
extern uint32_t __mbed_sbrk_start_0;
extern uint32_t __mbed_krbs_start_0;
unsigned char *mbed_heap_start_0 = (unsigned char *) &__mbed_sbrk_start_0;;
uint32_t mbed_heap_size_0 = (uint32_t) &__mbed_krbs_start_0 - (uint32_t) &__mbed_sbrk_start_0;
#endif

struct linked_list {
    linked_list *next;
    uint8_t data[MALLOC_TEST_SIZE];
};


// Global test variables
#define TEST_VALUE      789
static struct Test {
    Test() : val(TEST_VALUE) {}
    ~Test() {}
    int val;
} t;

int test_function()
{
    return TEST_VALUE;
}
static int global_int = test_function();

/** Test global variables initialisation
 */
void test_global_variables_initialisation(void)
{
    TEST_ASSERT_EQUAL(TEST_VALUE, global_int);
    TEST_ASSERT_EQUAL(TEST_VALUE, t.val);
}

/* TODO: add memory layout test.
 *
 * The test was skipped for now since not all devices seems to comply with Mbed OS memory.
 *
 * @note Mbed OS memory model: https://os.mbed.com/docs/latest/reference/memory.html
 *
 */


/*
 * Return true if addr is in range [start:start+size)
 */
static bool inrange(uint32_t addr, uint32_t start, uint32_t size)
{
    return (addr >= start) && (addr < (start + size));
}

/*
 * Return true if [addr:addr+size] is inside [start:start+len]
 */
static bool rangeinrange(uint32_t addr, uint32_t size, uint32_t start, uint32_t len)
{
    if ((addr + size) > (start + len)) {
        return false;
    }
    if (addr < start) {
        return false;
    }
    return true;
}

/*
 * Return true if the region is filled only with the specified value
 */
static bool valid_fill(uint8_t *data, uint32_t size, uint8_t fill)
{
    for (uint32_t i = 0; i < size; i++) {
        if (data[i] != fill) {
            return false;
        }
    }
    return true;
}

static void allocate_and_fill_heap(linked_list *&head)
{
    linked_list *current;

    current = (linked_list *) malloc(sizeof(linked_list));
    TEST_ASSERT_NOT_NULL(current);

    current->next = NULL;
    memset((void *) current->data, MALLOC_FILL, sizeof(current->data));

    // Allocate until malloc returns NULL
    head = current;
    while (true) {

        // Allocate
        linked_list *temp = (linked_list *) malloc(sizeof(linked_list));

        if (NULL == temp) {
            break;
        }
        bool result = rangeinrange((uint32_t) temp, sizeof(linked_list), (uint32_t)mbed_heap_start, mbed_heap_size);
#if defined(TOOLCHAIN_GCC_ARM) && defined(MBED_SPLIT_HEAP)
        if (false == result) {
            result = rangeinrange((uint32_t) temp, sizeof(linked_list), (uint32_t)mbed_heap_start_0, mbed_heap_size_0);
        }
#endif
        TEST_ASSERT_TRUE_MESSAGE(result, "Memory allocation out of range");

        // Init
        temp->next = NULL;
        memset((void *) temp->data, MALLOC_FILL, sizeof(current->data));

        // Add to list
        current->next = temp;
        current = temp;
    }
}

static void check_and_free_heap(linked_list *head, uint32_t &max_allocation_size)
{
    uint32_t total_size = 0;
    linked_list *current = head;

    while (current != NULL) {
        total_size += sizeof(linked_list);
        bool result = valid_fill(current->data, sizeof(current->data), MALLOC_FILL);

        TEST_ASSERT_TRUE_MESSAGE(result, "Memory fill check failed");

        linked_list *next = current->next;
        free(current);
        current = next;
    }

    max_allocation_size = total_size;
}

/** Test heap allocation

    Given a heap
    When memory is allocated from heap
    Then the memory is within heap boundary

 */
void test_heap_in_range(void)
{
    char *initial_heap;

    // Sanity check malloc
    initial_heap = (char *) malloc(1);
    TEST_ASSERT_NOT_NULL(initial_heap);

    bool result = inrange((uint32_t) initial_heap, (uint32_t)mbed_heap_start, mbed_heap_size);

#if defined(TOOLCHAIN_GCC_ARM) && defined(MBED_SPLIT_HEAP)
    if (false == result) {
        result = inrange((uint32_t) initial_heap, (uint32_t)mbed_heap_start_0, mbed_heap_size_0);
    }
#endif
    TEST_ASSERT_TRUE_MESSAGE(result, "Heap in wrong location");
    free(initial_heap);
}

#if MBED_CONF_RTOS_PRESENT
/** Test for Main thread stack

    Given a Main thread and its stack
    When check Main thread stack pointer
    Then the SP is within Main stack boundary
 */
void test_main_stack_in_range(void)
{
    mbed_rtos_storage_thread_t *thread = (mbed_rtos_storage_thread_t *) osThreadGetId();

    uint32_t psp = __get_PSP();
    uint8_t *stack_mem = (uint8_t *) thread->stack_mem;
    uint32_t stack_size = thread->stack_size;

    // PSP stack should be somewhere in the middle
    bool result = inrange(psp, (uint32_t) stack_mem, stack_size);

    TEST_ASSERT_TRUE_MESSAGE(result, "Main stack in wrong location");
}
#endif // #if MBED_CONF_RTOS_PRESENT

/** Test for Scheduler/ISR thread stack

    Given a Scheduler/ISR thread and its stack
    When check Scheduler/ISR thread stack pointer
    Then the SP is within Scheduler/ISR stack boundary
 */
void test_isr_stack_in_range(void)
{
    // MSP stack should be very near end (test using within 128 bytes)
    uint32_t msp = __get_MSP();
    bool result = inrange(msp, (uint32_t)mbed_stack_isr_start + mbed_stack_isr_size - 0x400, 0x400);

    TEST_ASSERT_TRUE_MESSAGE(result, "Interrupt stack in wrong location");
}

/** Test full heap allocation

    Given a heap and linked_list data structure
    When linked_list is filled till run out of heap memory
    Then the memory is properly initialised and freed
 */
void test_heap_allocation_free(void)
{
    linked_list *head = NULL;
    uint32_t max_allocation_size = 0;

    // Fully allocate the heap and stack
    allocate_and_fill_heap(head);

    check_and_free_heap(head, max_allocation_size);

    // Force a task switch so a stack check is performed
    ThisThread::sleep_for(10);

    printf("Total size dynamically allocated: %luB\n", max_allocation_size);
}


// Test cases
Case cases[] = {
    Case("Test global variables initialisation", test_global_variables_initialisation),
    Case("Test heap in range", test_heap_in_range),
#if MBED_CONF_RTOS_PRESENT
    Case("Test main stack in range", test_main_stack_in_range),
#endif
    Case("Test isr stack in range", test_isr_stack_in_range),
    Case("Test heap allocation and free", test_heap_allocation_free)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(test_timeout, "default_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

utest::v1::Specification specification(greentea_test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}

#endif // defined(TARGET_CORTEX_A) || !DEVICE_USTICKER
