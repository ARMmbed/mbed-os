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

static const int test_timeout = 5;

namespace {
bool mbed_main_called = false;
}

extern "C" void mbed_main()
{
    utest_printf("MBED: mbed_main() call before main()\r\n");
    mbed_main_called = true;
}

void test_call_before_main(void)
{

    utest_printf("MBED: main() starts now!\r\n");
    TEST_ASSERT_MESSAGE(mbed_main_called, "mbed_main didn't called before main");
}

// Test cases
Case cases[] = {
    Case("Test mbed_main called before main ", test_call_before_main),
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
