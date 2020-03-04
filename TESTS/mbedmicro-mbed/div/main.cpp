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
#include <utility>      // std::pair
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using utest::v1::Case;

static const int test_timeout = 5;

uint32_t test_64(uint64_t ticks)
{
    ticks >>= 3; // divide by 8
    if (ticks > 0xFFFFFFFF) {
        ticks /= 3;
    } else {
        ticks = (ticks * 0x55555556) >> 32; // divide by 3
    }
    return (uint32_t)(0xFFFFFFFF & ticks);
}


void test_division(void)
{

    {
        // 0xFFFFFFFF *  8 =  0x7fffffff8
        std::pair<uint32_t, uint64_t> values = std::make_pair(0x55555555, 0x7FFFFFFF8);
        uint32_t test_ret = test_64(values.second);
        utest_printf("64bit: 0x7FFFFFFF8: expected 0x%lX got 0x%lX \r\n", values.first, test_ret);
        TEST_ASSERT_EQUAL_UINT32(values.first, test_ret);
    }

    {
        // 0xFFFFFFFF * 24 = 0x17ffffffe8
        std::pair<uint32_t, uint64_t> values = std::make_pair(0xFFFFFFFF, 0x17FFFFFFE8);
        uint32_t test_ret = test_64(values.second);
        utest_printf("64bit: 0x17FFFFFFE8: expected 0x%lX got 0x%lX \r\n", values.first, test_ret);
        TEST_ASSERT_EQUAL_UINT32(values.first, test_ret);
    }

}

// Test cases
Case cases[] = {
    Case("Test division", test_division),
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
