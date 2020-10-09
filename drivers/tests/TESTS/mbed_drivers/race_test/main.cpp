/*
 * Copyright (c) 2016-2016, ARM Limited, All Rights Reserved
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
#if defined(MBED_RTOS_SINGLE_THREAD) || !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] Race test test cases require RTOS with multithread to run
#else

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] UsTicker need to be enabled for this test.
#else

#include "mbed.h"
#include "rtos.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "SingletonPtr.h"
#include <stdio.h>

using namespace utest::v1;

#define TEST_STACK_SIZE     512
static uint32_t instance_count = 0;

class TestClass {
public:
    TestClass()
    {
        ThisThread::sleep_for(500);
        instance_count++;
    }

    void do_something()
    {
        ThisThread::sleep_for(100);
    }

    ~TestClass()
    {
        instance_count--;
    }
};

static TestClass *get_test_class()
{
    static TestClass tc;
    return &tc;
}

static SingletonPtr<TestClass> test_class;

static void main_func_race()
{
    get_test_class();
    TEST_ASSERT_EQUAL_UINT32(1, instance_count);
}

static void main_class_race()
{
    test_class->do_something();
    TEST_ASSERT_EQUAL_UINT32(1, instance_count);
}

void test_case_func_race()
{
    Callback<void()> cb(main_func_race);
    Thread t1(osPriorityNormal, TEST_STACK_SIZE);
    Thread t2(osPriorityNormal, TEST_STACK_SIZE);

    // Start start first thread
    t1.start(cb);
    // Start second thread while the first is inside the constructor
    ThisThread::sleep_for(250);
    t2.start(cb);

    // Wait for the threads to finish
    t1.join();
    t2.join();

    TEST_ASSERT_EQUAL_UINT32(1, instance_count);

    // Reset instance count
    instance_count = 0;
}

void test_case_class_race()
{
    Callback<void()> cb(main_class_race);
    Thread t1(osPriorityNormal, TEST_STACK_SIZE);
    Thread t2(osPriorityNormal, TEST_STACK_SIZE);

    // Start start first thread
    t1.start(cb);
    // Start second thread while the first is inside the constructor
    ThisThread::sleep_for(250);
    t2.start(cb);

    // Wait for the threads to finish
    t1.join();
    t2.join();

    TEST_ASSERT_EQUAL_UINT32(1, instance_count);

    // Reset instance count
    instance_count = 0;
}

Case cases[] = {
    Case("function init race", test_case_func_race),
    Case("class init race", test_case_class_race),
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

#endif // !DEVICE_USTICKER
#endif // defined(MBED_RTOS_SINGLE_THREAD) || !defined(MBED_CONF_RTOS_THREAD)
