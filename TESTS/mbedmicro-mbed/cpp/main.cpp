/* mbed Microcontroller Library
 * Copyright (c) 2017-2020 ARM Limited
 *
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

static const int test_timeout = 10;

#define PATTERN_CHECK_VALUE  0xF0F0ADAD

class Test {

private:
    const char *name;
    const int pattern;

public:
    Test(const char *_name, bool print_message = true) : name(_name), pattern(PATTERN_CHECK_VALUE)
    {
        if (print_message) {
            print("init");
        }
    }

    void print(const char *message)
    {
        printf("%s::%s\n", name, message);
    }

    bool check_init(void)
    {
        bool result = (pattern == PATTERN_CHECK_VALUE);
        print(result ? "check_init: OK" : "check_init: ERROR");
        return result;
    }

    void stack_test(void)
    {
        print("stack_test");
        Test t("Stack");
        t.hello();
    }

    void hello(void)
    {
        print("hello");
    }

    ~Test()
    {
        print("destroy");
    }
};

/* Check C++ startup initialisation */
Test s("Static", false);

/* EXPECTED OUTPUT:
*******************
Static::init
Static::stack_test
Stack::init
Stack::hello
Stack::destroy
Static::check_init: OK
Heap::init
Heap::hello
Heap::destroy
*******************/
void test_static(void)
{

    s.print("init");
    // Global stack object simple test
    s.stack_test();
    if (s.check_init() == false) {
        TEST_ASSERT_MESSAGE(false, "Global stack initialization check failed");
    }

}

void test_heap(void)
{
    // Heap test object simple test
    Test *m = new Test("Heap");
    m->hello();
    if (m->check_init() == false) {
        TEST_ASSERT_MESSAGE(false, "Heap object initialization check failed");
    }
}

// Test cases
Case cases[] = {
    Case("Test stack object", test_static),
    Case("Test heap object", test_heap),
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
