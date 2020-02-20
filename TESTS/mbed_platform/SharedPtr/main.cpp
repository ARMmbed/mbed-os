/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "utest/utest.h"
#include "unity/unity.h"

#include "platform/SharedPtr.h"

using utest::v1::Case;

struct TestStruct {
    TestStruct()
    {
        s_count++;
        value = 42;
    }

    ~TestStruct()
    {
        s_count--;
        value = 0;
    }

    int value;
    static int s_count;
};

int TestStruct::s_count = 0;

/**
 * Test that a shared pointer correctly manages the lifetime of the underlying raw pointer
 */
void test_single_sharedptr_lifetime()
{
    // Sanity-check value of counter
    TEST_ASSERT_EQUAL(0, TestStruct::s_count);

    // Create and destroy shared pointer in given scope
    {
        SharedPtr<TestStruct> s_ptr(new TestStruct);
        TEST_ASSERT_EQUAL(1, TestStruct::s_count);
    }

    // Destroy shared pointer
    TEST_ASSERT_EQUAL(0, TestStruct::s_count);
}

/**
 * Test that multiple instances of shared pointers correctly manage the reference count
 * to release the object at the correct point
 */
void test_instance_sharing()
{
    SharedPtr<TestStruct> s_ptr1(nullptr);

    // Sanity-check value of counter
    TEST_ASSERT_EQUAL(0, TestStruct::s_count);

    // Create and destroy shared pointer in given scope
    {
        SharedPtr<TestStruct> s_ptr2(new TestStruct);
        TEST_ASSERT_EQUAL(1, TestStruct::s_count);
        s_ptr1 = s_ptr2;
        TEST_ASSERT_EQUAL(1, TestStruct::s_count);
    }

    TEST_ASSERT_EQUAL(1, TestStruct::s_count);

    s_ptr1 = nullptr;

    // Destroy shared pointer
    TEST_ASSERT_EQUAL(0, TestStruct::s_count);
}

/**
 * Test whether comparison operators operate as expected, both between
 * shared pointers managing the same object,
 * and a shared pointer and underlying raw pointer
 */
void test_equality_comparators()
{
    TestStruct *raw_ptr1 = new TestStruct;
    TestStruct *raw_ptr2 = new TestStruct;
    SharedPtr<TestStruct> s_ptr1_1(raw_ptr1);
    SharedPtr<TestStruct> s_ptr1_2 = s_ptr1_1;
    SharedPtr<TestStruct> s_ptr2(raw_ptr2);

    // Pointers that should be deemed equal
    TEST_ASSERT_TRUE(s_ptr1_1 == raw_ptr1); // Shared pointer / Raw pointer
    TEST_ASSERT_TRUE(s_ptr1_1 == s_ptr1_2); // Shared pointer / Shared pointer

    // Pointers that should be deemed different
    TEST_ASSERT_TRUE(s_ptr1_1 != raw_ptr2); // Shared pointer / Raw pointer
    TEST_ASSERT_TRUE(s_ptr1_1 != s_ptr2); // Shared pointer / Shared pointer
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test single shared pointer instance", test_single_sharedptr_lifetime),
    Case("Test instance sharing across multiple shared pointers", test_instance_sharing),
    Case("Test equality comparators", test_equality_comparators)
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
