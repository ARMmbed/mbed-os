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
#include "ExhaustibleBlockDevice.h"
#include "SlicingBlockDevice.h"

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

#ifndef MBED_TEST_ERASE_CYCLES
#define MBED_TEST_ERASE_CYCLES 100
#endif

// declarations
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_BLOCKDEVICE)


static uint32_t test_wear_leveling_size(uint32_t block_count)
{
    MBED_TEST_BLOCKDEVICE_DECL;

    // bring up to get block size
    bd.init();
    bd_size_t block_size = bd.get_erase_size();
    bd.deinit();

    SlicingBlockDevice slice(&bd, 0, block_count*block_size);
    ExhaustibleBlockDevice ebd(&slice, MBED_TEST_ERASE_CYCLES);

    printf("Testing size %llu bytes (%lux%llu) blocks\n",
        block_count*block_size, block_count, block_size);
    setup_atomic_operations(&ebd, true);

    int64_t cycles = 0;
    while (true) {
        int64_t ret = perform_atomic_operations(&ebd);
        check_atomic_operations(&ebd);
        if (-1 == ret) {
            break;
        }
        cycles++;
        TEST_ASSERT_EQUAL(cycles, ret);

    }

    printf("  Simulated flash lasted %lli cylces\n", cycles);
    return cycles;
}

/**
 * Check that storage life is proportional to storage size
 *
 * This test is to ensure that littlefs is properly handling wear
 * leveling. It does this by creating a simulated flash block device
 * which can be worn out and then using it until it is exhausted.
 * It then doubles the size of the block device and runs the same
 * test. If the block device with twice the size lasts at least
 * twice as long then the test passes.
 */
void test_wear_leveling()
{
    uint32_t cycles_1 = test_wear_leveling_size(MBED_TEST_BLOCK_COUNT / 2);
    uint32_t cycles_2 = test_wear_leveling_size(MBED_TEST_BLOCK_COUNT / 1);
    TEST_ASSERT(cycles_2 > cycles_1 * 2);
}

Case cases[] = {
    Case("test wear leveling", test_wear_leveling),
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
