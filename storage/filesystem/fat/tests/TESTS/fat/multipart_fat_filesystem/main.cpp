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

using namespace utest::v1;

#ifndef MBED_EXTENDED_TESTS
#error [NOT_SUPPORTED] Filesystem tests not supported by default
#else

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

// Test formatting and partitioning
void test_format()
{
    TEST_SKIP_UNLESS_MESSAGE(check_heap_allocation(mem_alloc_threshold), "Not enough heap memory to run test. Test skipped.");

    HeapBlockDevice bd {BLOCK_COUNT * BLOCK_SIZE, BLOCK_SIZE};

    // Create two partitions splitting device in ~half
    int err = MBRBlockDevice::partition(&bd, 1, LINUX_FS_PARTITION_ID, 0, (BLOCK_COUNT / 2) * BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = MBRBlockDevice::partition(&bd, 2, LINUX_FS_PARTITION_ID, -(BLOCK_COUNT / 2) * BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Load both partitions
    MBRBlockDevice part1(&bd, 1);
    err = part1.init();
    TEST_ASSERT_EQUAL(0, err);

    MBRBlockDevice part2(&bd, 2);
    err = part2.init();
    TEST_ASSERT_EQUAL(0, err);

    // Format both partitions
    err = FATFileSystem::format(&part1);
    TEST_ASSERT_EQUAL(0, err);

    err = FATFileSystem::format(&part2);
    TEST_ASSERT_EQUAL(0, err);

    // Unload the partitions
    err = part1.deinit();
    TEST_ASSERT_EQUAL(0, err);

    err = part2.deinit();
    TEST_ASSERT_EQUAL(0, err);
}


// Simple multipartition test for reading/writing files
template <ssize_t TEST_SIZE>
void test_read_write()
{
    TEST_SKIP_UNLESS_MESSAGE(check_heap_allocation(mem_alloc_threshold), "Not enough heap memory to run test. Test skipped.");

    HeapBlockDevice bd {BLOCK_COUNT * BLOCK_SIZE, BLOCK_SIZE};

    format_partitions(bd);

    // Load both partitions
    MBRBlockDevice part1(&bd, 1);
    int err = part1.init();
    TEST_ASSERT_EQUAL(0, err);

    MBRBlockDevice part2(&bd, 2);
    err = part2.init();
    TEST_ASSERT_EQUAL(0, err);

    // Create fat filesystems on both partitions
    FATFileSystem fs1("fat1");
    FATFileSystem fs2("fat2");

    err = fs1.mount(&part1);
    TEST_ASSERT_EQUAL(0, err);

    err = fs2.mount(&part2);
    TEST_ASSERT_EQUAL(0, err);

    std::array<uint8_t, TEST_SIZE> buffer1 { };
    std::array<uint8_t, TEST_SIZE> buffer2 { };

    // Fill with random sequence
    srand(1);

    for (int i = 0; i < TEST_SIZE; i++) {
        buffer1[i] = 0xff & rand();
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        buffer2[i] = 0xff & rand();
    }

    // write and read files on both partitions
    File file;
    err = file.open(&fs1, "test_read_write.dat", O_WRONLY | O_CREAT);
    TEST_ASSERT_EQUAL(0, err);
    ssize_t size = file.write(buffer1.data(), TEST_SIZE);
    TEST_ASSERT_EQUAL(TEST_SIZE, size);
    err = file.close();
    TEST_ASSERT_EQUAL(0, err);

    err = file.open(&fs2, "test_read_write.dat", O_WRONLY | O_CREAT);
    TEST_ASSERT_EQUAL(0, err);
    size = file.write(buffer2.data(), TEST_SIZE);
    TEST_ASSERT_EQUAL(TEST_SIZE, size);
    err = file.close();
    TEST_ASSERT_EQUAL(0, err);

    err = file.open(&fs1, "test_read_write.dat", O_RDONLY);
    TEST_ASSERT_EQUAL(0, err);
    size = file.read(buffer1.data(), TEST_SIZE);
    TEST_ASSERT_EQUAL(TEST_SIZE, size);
    err = file.close();
    TEST_ASSERT_EQUAL(0, err);

    err = file.open(&fs2, "test_read_write.dat", O_RDONLY);
    TEST_ASSERT_EQUAL(0, err);
    size = file.read(buffer2.data(), TEST_SIZE);
    TEST_ASSERT_EQUAL(TEST_SIZE, size);
    err = file.close();
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);

    for (int i = 0; i < TEST_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), buffer1[i]);
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), buffer2[i]);
    }

    err = fs1.unmount();
    TEST_ASSERT_EQUAL(0, err);

    err = fs2.unmount();
    TEST_ASSERT_EQUAL(0, err);

    err = part1.deinit();
    TEST_ASSERT_EQUAL(0, err);

    err = part2.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

void test_single_mbr()
{
    TEST_SKIP_UNLESS_MESSAGE(check_heap_allocation(mem_alloc_threshold), "Not enough heap memory to run test. Test skipped.");

    HeapBlockDevice bd {BLOCK_COUNT * BLOCK_SIZE, BLOCK_SIZE};

    format_partitions(bd);

    int err = bd.init();
    TEST_ASSERT_EQUAL(0, err);

    const bd_addr_t MBR_OFFSET = 0;
    const bd_addr_t FAT1_OFFSET = 1;
    const bd_addr_t FAT2_OFFSET = BLOCK_COUNT / 2;

    std::array<uint8_t, BLOCK_SIZE> buffer { };

    // Check that all three header blocks have the 0x55aa signature
    err = bd.read(buffer.data(), MBR_OFFSET * BLOCK_SIZE, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT(memcmp(&buffer[BLOCK_SIZE - 2], "\x55\xaa", 2) == 0);

    err = bd.read(buffer.data(), FAT1_OFFSET * BLOCK_SIZE, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT(memcmp(&buffer[BLOCK_SIZE - 2], "\x55\xaa", 2) == 0);

    err = bd.read(buffer.data(), FAT2_OFFSET * BLOCK_SIZE, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT(memcmp(&buffer[BLOCK_SIZE - 2], "\x55\xaa", 2) == 0);

    // Check that the headers for both filesystems contain a jump code
    // indicating they are actual FAT superblocks and not an extra MBR
    err = bd.read(buffer.data(), FAT1_OFFSET * BLOCK_SIZE, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT(buffer[0] == 0xe9 || buffer[0] == 0xeb || buffer[0] == 0xe8);

    err = bd.read(buffer.data(), FAT2_OFFSET * BLOCK_SIZE, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT(buffer[0] == 0xe9 || buffer[0] == 0xeb || buffer[0] == 0xe8);

    bd.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

void test_with_other_fs()
{
    TEST_SKIP_UNLESS_MESSAGE(check_heap_allocation(mem_alloc_threshold), "Not enough heap memory to run test. Test skipped.");

    // Stage 0 - LittleFS
    // Stage 1 - FatFS with MBR
    // Stage 2 - LittleFS
    // Make sure that at no stage we are able to mount the current file system after using the
    // previous one

    HeapBlockDevice bd {BLOCK_COUNT * BLOCK_SIZE, BLOCK_SIZE};

    format_partitions(bd);

    int err;

    for (int stage = 0; stage < 3; stage++) {

        BlockDevice *part;
        FileSystem *fs;

        if (stage == 1) {
            printf("Stage %d: FAT FS\n", stage + 1);
            err = MBRBlockDevice::partition(&bd, 1, LINUX_FS_PARTITION_ID, 0, BLOCK_COUNT * BLOCK_SIZE);
            TEST_ASSERT_EQUAL(0, err);

            part = new (std::nothrow) MBRBlockDevice(&bd, 1);
            TEST_SKIP_UNLESS_MESSAGE(part, "Not enough heap memory to run test. Test skipped.");

            err = part->init();
            TEST_ASSERT_EQUAL(0, err);

            fs = new FATFileSystem("fat");
        } else {
            printf("Stage %d: Little FS\n", stage + 1);
            part = &bd;
            fs = new LittleFileSystem("lfs");
        }
        TEST_SKIP_UNLESS_MESSAGE(fs, "Not enough heap memory to run test. Test skipped.");

        err = fs->mount(part);
        TEST_ASSERT_NOT_EQUAL(0, err);

        err = fs->reformat(part);
        TEST_ASSERT_EQUAL(0, err);

        err = fs->unmount();
        TEST_ASSERT_EQUAL(0, err);

        delete fs;
        if (stage == 1) {
            delete part;
        }
    }
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing formating", test_format),
    Case("Testing read write < block", test_read_write < BLOCK_SIZE / 2 >),
    Case("Testing read write > block", test_read_write<2 * BLOCK_SIZE>),
    Case("Testing for no extra MBRs", test_single_mbr),
    Case("Testing with other file system", test_with_other_fs),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // MBED_EXTENDED_TESTS
