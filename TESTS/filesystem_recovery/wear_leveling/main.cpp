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
#include "LittleFileSystem.h"

using namespace utest::v1;

#define ERASE_CYCLES    20
#define TEST_BD_SIZE    (8 * 1024)

static uint32_t test_wear_leveling_size(uint32_t bd_size)
{
    HeapBlockDevice hbd(bd_size, 1, 1, 512);
    ExhaustibleBlockDevice ebd(&hbd, ERASE_CYCLES);

    printf("Testing size %lu\n", bd_size);
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
    uint32_t cycles_1 = test_wear_leveling_size(TEST_BD_SIZE * 1);
    uint32_t cycles_2 = test_wear_leveling_size(TEST_BD_SIZE * 2);
    TEST_ASSERT(cycles_2 > cycles_1 * 2);
}

Case cases[] = {
    Case("test wear leveling", test_wear_leveling),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
