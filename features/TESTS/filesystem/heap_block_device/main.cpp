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

#include "HeapBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

/* It is not possible to build a KL25Z image with IAR including the file system if
 * stack tracking statistics are enabled. If this is the case, build dummy
 * tests.
 */
#if ! defined(__ICCARM__) && ! defined(TARGET_KL25Z) && ! defined(MBED_STACK_STATS_ENABLED)

#define BLOCK_SIZE 512
#define HEAP_BLOCK_DEVICE_TEST_01         test_read_write
uint8_t write_block[BLOCK_SIZE];
uint8_t read_block[BLOCK_SIZE];

// Simple test which reads and writes a block
void test_read_write() {
    HeapBlockDevice bd(16*BLOCK_SIZE, BLOCK_SIZE);

    int err = bd.init();
    TEST_ASSERT_EQUAL(0, err);

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = bd.program(write_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = bd.read(read_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    err = bd.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

#else   /* ! defined(__ICCARM__) && ! defined(TARGET_KL25Z) && ! defined(MBED_STACK_STATS_ENABLED) */

#define HEAP_BLOCK_DEVICE_TEST_01      heap_block_device_test_dummy

/** @brief  heap_block_device_test_dummy    Dummy test case for testing when KL25Z being built with stack statistics enabled.
 *
 * @return success always
 */
static control_t heap_block_device_test_dummy()
{
    printf("Null test\n");
    return CaseNext;
}

#endif  /* ! defined(__ICCARM__) && ! defined(TARGET_KL25Z) && ! defined(MBED_STACK_STATS_ENABLED) */

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing read write of a block", HEAP_BLOCK_DEVICE_TEST_01),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
