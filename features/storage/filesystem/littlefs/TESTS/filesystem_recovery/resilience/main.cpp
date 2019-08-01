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
#include "unity.h"
#include "utest.h"
#include "test_env.h"

#include "atomic_usage.h"
#include "ObservingBlockDevice.h"

using namespace utest::v1;

// test configuration
#ifndef MBED_TEST_SIM_BLOCKDEVICE
#error [NOT_SUPPORTED] Simulation block device required for resilience tests
#else

#ifndef MBED_TEST_SIM_BLOCKDEVICE_DECL
#define MBED_TEST_SIM_BLOCKDEVICE_DECL MBED_TEST_SIM_BLOCKDEVICE bd(MBED_TEST_BLOCK_COUNT*512, 1, 1, 512)
#endif

#ifndef MBED_TEST_BLOCK_COUNT
#define MBED_TEST_BLOCK_COUNT 64
#endif

#ifndef MBED_TEST_CYCLES
#define MBED_TEST_CYCLES 10
#endif

#ifndef MBED_TEST_TIMEOUT
#define MBED_TEST_TIMEOUT 480
#endif

// declarations
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_SIM_BLOCKDEVICE)


/**
 * Check that the filesystem is valid after every change
 *
 * This test is to ensure that littlefs contains a valid filesystem at
 * all times. This property is required for handling unexpected power
 * loss.
 */
void test_resilience()
{
    MBED_TEST_SIM_BLOCKDEVICE_DECL;

    // bring up to get block size
    bd.init();
    bd_size_t block_size = bd.get_erase_size();
    bd.deinit();

    SlicingBlockDevice slice(&bd, 0, MBED_TEST_BLOCK_COUNT * block_size);

    // Setup the test
    setup_atomic_operations(&slice, true);

    // Run check on every write operation
    ObservingBlockDevice observer(&slice);
    observer.attach(check_atomic_operations);

    // Perform operations
    printf("Performing %i operations on flash\n", MBED_TEST_CYCLES);
    for (int i = 1; i <= MBED_TEST_CYCLES; i++) {
        int64_t ret = perform_atomic_operations(&observer);
        TEST_ASSERT_EQUAL(i, ret);
    }
    printf("No errors detected\n");
}

Case cases[] = {
    Case("test resilience", test_resilience),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(MBED_TEST_TIMEOUT, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif // MBED_TEST_SIM_BLOCKDEVICE
