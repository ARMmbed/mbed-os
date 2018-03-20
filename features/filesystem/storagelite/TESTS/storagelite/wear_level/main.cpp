/*
* Copyright (c) 2018 ARM Limited. All rights reserved.
* SPDX-License-Identifier: Apache-2.0
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "StorageLite.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "Thread.h"
#endif
#include "Timer.h"
#include "HeapBlockDevice.h"
#include "FlashSimBlockDevice.h"
#include "ExhaustibleBlockDevice.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include <string.h>
#include <stdio.h>

// Define this if you want to run the test locally (requires SPIF/SD block device)
#undef LOCAL_TEST

#if !STORAGELITE_ENABLED
#error [NOT_SUPPORTED] StorageLite needs to be enabled for this test
#endif

// MBED_TEST_SIM_BLOCKDEVICE is used here only to filter out inappropriate boards (little RAM etc.)
#if !defined(MBED_TEST_SIM_BLOCKDEVICE) && !defined(LOCAL_TEST)
#error [NOT_SUPPORTED] StorageLite test not supported on this platform
#endif

using namespace utest::v1;

static const size_t test_num_blocks = 64;
static const size_t max_erases = 64;
static const size_t block_size = 512;

static uint64_t test_wear_leveling_size(size_t num_blocks)
{
    uint8_t *file_name, *get_buf, *set_buf;
    size_t file_name_size = 32;
    size_t data_size = 256;
    size_t num_files = 8;
    size_t actual_data_size;
    int result;

    HeapBlockDevice heap_bd(num_blocks * block_size, 1,  1, block_size);
    FlashSimBlockDevice flash_bd(&heap_bd);
    ExhaustibleBlockDevice exhaust_bd(&flash_bd, max_erases);

    StorageLite stlite;

    result = stlite.init(&exhaust_bd, 128);
    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

    result = stlite.reset();
    TEST_ASSERT_EQUAL(0, result);

    file_name = new uint8_t[file_name_size];
    set_buf = new uint8_t[data_size];
    get_buf = new uint8_t[data_size];

    printf("Testing size %ld (%ld blocks * %ld bytes each). Max erases %ld.\n",
            num_blocks * block_size, num_blocks, block_size, max_erases);
    uint64_t cycles = 0;
    for (;;) {
        memset(file_name, '0' + cycles % num_files, file_name_size);
        memset(set_buf, cycles % 256, data_size);
        result = stlite.set(file_name, file_name_size, set_buf, data_size, 0);
        if (result) {
            TEST_ASSERT_EQUAL(STORAGELITE_WRITE_ERROR, result);
            break;
        }

        cycles++;
        result = stlite.get(file_name, file_name_size, get_buf, data_size, actual_data_size);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
        TEST_ASSERT_EQUAL(data_size, actual_data_size);
        TEST_ASSERT_EQUAL(0, memcmp(set_buf, get_buf, data_size));
    }

    uint64_t gross_bytes = (data_size + file_name_size + 32) * cycles;
    uint64_t net_bytes = data_size * cycles;

    printf("Total write cycles: %lld, gross bytes %lld, net bytes %lld\n",
            cycles, gross_bytes, net_bytes);

    delete[] file_name;
    delete[] get_buf;
    delete[] set_buf;

    return cycles;
}

static void test_wear_leveling()
{
    uint64_t cycles_1 = test_wear_leveling_size(test_num_blocks / 2);
    uint64_t cycles_2 = test_wear_leveling_size(test_num_blocks);
    TEST_ASSERT(cycles_2 > cycles_1 * 2);
}


utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("StorageLite: Wear level test",     test_wear_leveling,    greentea_failure_handler),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(400, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}
