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

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"

using namespace utest::v1;

/**
 * This test is intended to gate devices that do not have enough RAM to run
 * Mbed os. Devices passing this test should have enough RAM to run all
 * other Mbed OS tests.
 *
 * If your device does not pass this test, then you should determine the
 * cause of high memory usage and fix it. If you cannot free enough memory,
 * then you should turn off Mbed OS support for this device.
 */

#define MIN_HEAP_SIZE   2048
#define MIN_DATA_SIZE   2048

volatile uint8_t test_buffer[MIN_DATA_SIZE];

static void minimum_heap_test()
{
    void *mem = malloc(MIN_HEAP_SIZE);
    TEST_ASSERT_NOT_EQUAL(NULL, mem);
    free(mem);
}

static void minimum_data_test()
{
    // Use test buffer so it is not optimized away
    for (int i = 0; i < MIN_DATA_SIZE; i++) {
        test_buffer[i] = i & 0xFF;
    }
}


utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Minimum heap test", minimum_heap_test),
    Case("Minimum data test", minimum_data_test),
};

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
