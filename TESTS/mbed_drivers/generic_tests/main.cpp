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
#include <stdio.h>
#include <string.h>
#include <utility>  // std::pair
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

using namespace utest::v1;

#define PATTERN_CHECK_VALUE  0xF0F0ADAD

class CppTestCaseHelperClass {
private:
    const char* name;
    const unsigned pattern;

public:
    CppTestCaseHelperClass(const char* _name) : name(_name), pattern(PATTERN_CHECK_VALUE)  {
        print("init");
    }

    void print(const char *message) {
        printf("%s::%s\n", name, message);
    }

    bool check_init(void) {
        bool result = (pattern == PATTERN_CHECK_VALUE);
        print(result ? "check_init: OK" : "check_init: ERROR");
        return result;
    }

    void stack_test(void) {
        print("stack_test");
        CppTestCaseHelperClass t("Stack");
        t.hello();
    }

    void hello(void) {
        print("hello");
    }

    ~CppTestCaseHelperClass() {
        print("destroy");
    }
};


void test_case_basic() {
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_FALSE(false);
    TEST_ASSERT_EQUAL_STRING("The quick brown fox jumps over the lazy dog",
        "The quick brown fox jumps over the lazy dog");
}

void test_case_blinky() {
    static DigitalOut myled(LED1);
    const int cnt_max = 1024;
    for (int cnt = 0; cnt < cnt_max; ++cnt) {
        myled = !myled;
    }
}

void test_case_cpp_stack() {
    // Check C++ start-up initialisation
    CppTestCaseHelperClass s("Static");

    // Global stack object simple test
    s.stack_test();
    TEST_ASSERT_TRUE_MESSAGE(s.check_init(), "s.check_init() failed");
}

void test_case_cpp_heap() {
    // Heap test object simple test
    CppTestCaseHelperClass *m = new CppTestCaseHelperClass("Heap");
    m->hello();
    TEST_ASSERT_TRUE_MESSAGE(m->check_init(), "m->check_init() failed");
    delete m;
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

// Generic test cases
Case cases[] = {
    Case("Basic", test_case_basic, greentea_failure_handler),
    Case("Blinky", test_case_blinky, greentea_failure_handler),
    Case("C++ stack", test_case_cpp_stack, greentea_failure_handler),
    Case("C++ heap", test_case_cpp_heap, greentea_failure_handler)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
