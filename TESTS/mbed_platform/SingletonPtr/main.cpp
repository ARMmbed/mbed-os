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
#include "utest/utest.h"
#include "unity/unity.h"

using utest::v1::Case;


class InstanceTest {
public:
    InstanceTest()
    {
        _instance_counter++;
    }

    static int get_instance_counter()
    {
        return _instance_counter;
    }

private:
    static int _instance_counter;
};

int InstanceTest::_instance_counter = 0;


SingletonPtr<InstanceTest> singleton;


/** Test SingletonPtr lazy initialization
 *
 *  Given a global singleton of type SingletonPtr<InstanceTest>
 *  When before first singleton use
 *  Then underneath object is yet not created
 */
void test_lazy_initialization()
{
    TEST_ASSERT_MESSAGE(InstanceTest::get_instance_counter() == 0, "Initialized before first use!!!");
}

/** Test SingletonPtr single instance
 *
 *  Given a singleton of type SingletonPtr<InstanceTest>
 *
 *  When after first singleton use
 *  Then underneath object was created exactly once
 *
 *  When after second singleton use
 *  Then underneath object was created exactly once
 *       and both (ref1 and ref2) are references to same instance
 *
 */
void test_single_instance()
{
    InstanceTest *ref1 = singleton.get();
    TEST_ASSERT_NOT_NULL(ref1);

    TEST_ASSERT_EQUAL_INT(1, InstanceTest::get_instance_counter());

    InstanceTest *ref2 = singleton.get();
    TEST_ASSERT_NOT_NULL(ref2);

    TEST_ASSERT_EQUAL_INT(1, InstanceTest::get_instance_counter());

    // same instance
    TEST_ASSERT_EQUAL_PTR(ref1, ref2);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test lazy initialization", test_lazy_initialization),
    Case("Test single instance", test_single_instance),
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
