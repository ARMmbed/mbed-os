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
#include "unity.h"
#include "utest.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#if !DEVICE_USTICKER
  #error [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

#define TEST_STACK_SIZE 512
#define TEST_DELAY 10

static int change_counter = 0;
static Mutex mutex;
static ConditionVariable cond(mutex);

void increment_on_signal()
{
    mutex.lock();

    cond.wait();
    change_counter++;

    mutex.unlock();
}

void test_notify_one()
{
    Thread t1(osPriorityNormal, TEST_STACK_SIZE);
    Thread t2(osPriorityNormal, TEST_STACK_SIZE);

    change_counter = 0;
    t1.start(increment_on_signal);
    t2.start(increment_on_signal);

    wait_ms(TEST_DELAY);
    TEST_ASSERT_EQUAL(0, change_counter);

    mutex.lock();
    cond.notify_one();
    mutex.unlock();

    wait_ms(TEST_DELAY);
    TEST_ASSERT_EQUAL(1, change_counter);

    mutex.lock();
    cond.notify_one();
    mutex.unlock();

    t1.join();
    t2.join();
}

void test_notify_all()
{
    Thread t1(osPriorityNormal, TEST_STACK_SIZE);
    Thread t2(osPriorityNormal, TEST_STACK_SIZE);

    change_counter = 0;
    t1.start(increment_on_signal);
    t2.start(increment_on_signal);

    wait_ms(TEST_DELAY);
    TEST_ASSERT_EQUAL(0, change_counter);

    mutex.lock();
    cond.notify_all();
    mutex.unlock();

    wait_ms(TEST_DELAY);
    TEST_ASSERT_EQUAL(2, change_counter);

    t1.join();
    t2.join();
}


class TestConditionVariable : public ConditionVariable {

public:
    static void test_linked_list(void)
    {
        Waiter *list = NULL;
        Waiter w1;
        Waiter w2;
        Waiter w3;
        Waiter w4;

        TEST_ASSERT_EQUAL(0, validate_and_get_size(&list));

        // Add 4 nodes
        _add_wait_list(&list, &w1);
        TEST_ASSERT_EQUAL(1, validate_and_get_size(&list));
        _add_wait_list(&list, &w2);
        TEST_ASSERT_EQUAL(2, validate_and_get_size(&list));
        _add_wait_list(&list, &w3);
        TEST_ASSERT_EQUAL(3, validate_and_get_size(&list));
        _add_wait_list(&list, &w4);
        TEST_ASSERT_EQUAL(4, validate_and_get_size(&list));

        // Remove a middle node
        _remove_wait_list(&list, &w2);
        TEST_ASSERT_EQUAL(3, validate_and_get_size(&list));

        // Remove front node
        _remove_wait_list(&list, &w1);
        TEST_ASSERT_EQUAL(2, validate_and_get_size(&list));

        // remove back node
        _remove_wait_list(&list, &w4);
        TEST_ASSERT_EQUAL(1, validate_and_get_size(&list));

        // remove last node
        _remove_wait_list(&list, &w3);
        TEST_ASSERT_EQUAL(0, validate_and_get_size(&list));

        TEST_ASSERT_EQUAL_PTR(NULL, list);
    }

    /**
     * Validate the linked list an return the number of elements
     *
     * If this list is invalid then this function asserts and does not
     * return.
     *
     * Every node in a valid linked list has the properties:
     * 1. node->prev->next == node
     * 2. node->next->prev == node
     */
    static int validate_and_get_size(Waiter **list)
    {
        Waiter *first = *list;
        if (NULL == first) {
            // List is empty
            return 0;
        }

        int size = 0;
        Waiter *current = first;
        do {
            TEST_ASSERT_EQUAL_PTR(current, current->prev->next);
            TEST_ASSERT_EQUAL_PTR(current, current->next->prev);
            current = current->next;
            size++;
        } while (current != first);
        return size;
    }

};

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test notify one", test_notify_one),
    Case("Test notify all", test_notify_all),
    Case("Test linked list", TestConditionVariable::test_linked_list),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
