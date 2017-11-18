/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "mbed.h"
#include "unity.h"
#include "utest.h"
#include "test_env.h"

#include "atomic_usage.h"
#include "ObservingBlockDevice.h"

using namespace utest::v1;

// test configuration
#ifndef MBED_TEST_BLOCKDEVICE
#define MBED_TEST_BLOCKDEVICE HeapBlockDevice
#define MBED_TEST_BLOCKDEVICE_DECL MBED_TEST_BLOCKDEVICE bd(MBED_TEST_BLOCK_COUNT*512, 1, 1, 512)
#endif

#ifndef MBED_TEST_BLOCKDEVICE_DECL
#define MBED_TEST_BLOCKDEVICE_DECL MBED_TEST_BLOCKDEVICE bd
#endif

#ifndef MBED_TEST_BLOCK_COUNT
#define MBED_TEST_BLOCK_COUNT 64
#endif

#ifndef MBED_TEST_CYCLES
#define MBED_TEST_CYCLES 10
#endif

// declarations
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_BLOCKDEVICE)


/**
 * Check that the filesystem is valid after every change
 *
 * This test is to ensure that littlefs contains a valid filesystem at
 * all times. This property is required for handling unexpected power
 * loss.
 */
void test_resilience()
{
    MBED_TEST_BLOCKDEVICE_DECL;

    // bring up to get block size
    bd.init();
    bd_size_t block_size = bd.get_erase_size();
    bd.deinit();

    SlicingBlockDevice slice(&bd, 0, MBED_TEST_BLOCK_COUNT*block_size);

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
    GREENTEA_SETUP(120, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
