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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include <memory>

#include "HeapBlockDevice.h"
#include "FATFileSystem.h"
#include "MBRBlockDevice.h"
#include "LittleFileSystem.h"
#include <stdlib.h>
#include "mbed_retarget.h"

#define TEST_MIN_REQ_ROM_SIZE 72 * 1024
#if defined(MBED_ROM_SIZE) && (MBED_ROM_SIZE < TEST_MIN_REQ_ROM_SIZE)
#error [NOT_SUPPORTED] Insufficient Flash memory for test
#else

using namespace utest::v1;

static constexpr int mem_alloc_threshold = 32 * 1024;
static constexpr int BLOCK_SIZE = 512;
static constexpr int BLOCK_COUNT = 512;
static constexpr uint8_t LINUX_FS_PARTITION_ID = 0x83;

bool check_heap_allocation(size_t bytes)
{
    unique_ptr<uint8_t[]> dummy {new (std::nothrow) uint8_t[bytes]};
    return dummy.get();
}

void format_partitions(HeapBlockDevice &bd)
{
    // Create two partitions splitting device in ~half
    TEST_ASSERT_EQUAL(0, MBRBlockDevice::partition(&bd, 1, LINUX_FS_PARTITION_ID, 0, (BLOCK_COUNT / 2) * BLOCK_SIZE));
    TEST_ASSERT_EQUAL(0, MBRBlockDevice::partition(&bd, 2, LINUX_FS_PARTITION_ID, -(BLOCK_COUNT / 2) * BLOCK_SIZE));

    // Load both partitions
    MBRBlockDevice part1(&bd, 1);
    MBRBlockDevice part2(&bd, 2);
    TEST_ASSERT_EQUAL(0, part1.init());
    TEST_ASSERT_EQUAL(0, part2.init());

    // Format both partitions
    TEST_ASSERT_EQUAL(0, FATFileSystem::format(&part1));
    TEST_ASSERT_EQUAL(0, FATFileSystem::format(&part2));

    // Unload the partitions
    TEST_ASSERT_EQUAL(0, part1.deinit());
    TEST_ASSERT_EQUAL(0, part2.deinit());
}

void reformat_and_unmount(FileSystem &fs, BlockDevice &partition)
{
    TEST_ASSERT_EQUAL(0, fs.reformat(&partition));
    TEST_ASSERT_EQUAL(0, fs.unmount());
}

void test_formatting_before_mounting_other_fs()
{
    TEST_SKIP_UNLESS_MESSAGE(check_heap_allocation(mem_alloc_threshold), "Not enough heap memory to run test. Test skipped.");

    HeapBlockDevice bd {BLOCK_COUNT * BLOCK_SIZE, BLOCK_SIZE};

    format_partitions(bd);

    LittleFileSystem lfs {"lfs"};

    int err = lfs.mount(&bd);
    TEST_ASSERT_NOT_EQUAL(0, err);
}

void test_mounting_before_mounting_other_fs()
{
    TEST_SKIP_UNLESS_MESSAGE(check_heap_allocation(mem_alloc_threshold), "Not enough heap memory to run test. Test skipped.");

    HeapBlockDevice bd {BLOCK_COUNT * BLOCK_SIZE, BLOCK_SIZE};

    int err = MBRBlockDevice::partition(&bd, 1, LINUX_FS_PARTITION_ID, 0, BLOCK_COUNT * BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    MBRBlockDevice part {&bd, 1};

    err = part.init();
    TEST_ASSERT_EQUAL(0, err);

    err = FATFileSystem::format(&part);
    TEST_ASSERT_EQUAL(0, err);

    FATFileSystem fat_fs {"fat"};

    err = fat_fs.mount(&part);
    TEST_ASSERT_EQUAL(0, err);

    LittleFileSystem lfs {"lfs"};

    err = lfs.mount(&bd);
    TEST_ASSERT_NOT_EQUAL(0, err);
}

void test_mounting_other_fs_before_mounting()
{
    TEST_SKIP_UNLESS_MESSAGE(check_heap_allocation(mem_alloc_threshold), "Not enough heap memory to run test. Test skipped.");

    HeapBlockDevice bd {BLOCK_COUNT * BLOCK_SIZE, BLOCK_SIZE};

    LittleFileSystem lfs {"lfs"};

    int err = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, err);

    err = lfs.mount(&bd);
    TEST_ASSERT_EQUAL(0, err);

    err = MBRBlockDevice::partition(&bd, 1, LINUX_FS_PARTITION_ID, 0, BLOCK_COUNT * BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    MBRBlockDevice part {&bd, 1};

    err = part.init();
    TEST_ASSERT_EQUAL(0, err);

    FATFileSystem fat_fs {"fat"};

    err = fat_fs.mount(&part);
    TEST_ASSERT_NOT_EQUAL(0, err);
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing formatting before other file system", test_formatting_before_mounting_other_fs),
    Case("Testing mounting before other file system", test_mounting_before_mounting_other_fs),
    Case("Testing mounting other file system before mounting", test_mounting_other_fs_before_mounting)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !defined(MBED_ROM_SIZE) || (MBED_ROM_SIZE >= TEST_MIN_REQ_ROM_SIZE)
