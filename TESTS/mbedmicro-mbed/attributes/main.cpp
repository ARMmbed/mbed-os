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
#include <stdio.h>
#include <stdint.h>

#include "mbed_toolchain.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

using namespace utest::v1;


// Test functions declared as C functions to avoid issues with name mangling
extern "C" {
    int testPacked();
    int testAlign();
    int testUnused();
    int testWeak();
    int testPure();
    int testForceInline();
    int testNoReturn();
    int testUnreachable();
    int testDeprecated();
}


// Test wrapper and test cases for utest
template <int (*F)()>
void test_wrapper() {
    TEST_ASSERT_UNLESS(F());
}

utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(5, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing PACKED attribute",        test_wrapper<testPacked>),
    Case("Testing ALIGN attribute",         test_wrapper<testAlign>),
    Case("Testing UNUSED attribute",        test_wrapper<testUnused>),
    Case("Testing WEAK attribute",          test_wrapper<testWeak>),
    Case("Testing PURE attribute",          test_wrapper<testPure>),
    Case("Testing FORCEINLINE attribute",   test_wrapper<testForceInline>),
    Case("Testing NORETURN attribute",      test_wrapper<testNoReturn>),
    Case("Testing UNREACHABLE attribute",   test_wrapper<testUnreachable>),
    Case("Testing DEPRECATED attribute",    test_wrapper<testDeprecated>),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
