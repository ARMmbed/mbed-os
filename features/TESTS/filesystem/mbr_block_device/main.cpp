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
#include "MBRBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

#define BLOCK_COUNT 16
#define BLOCK_SIZE 512

HeapBlockDevice bd(BLOCK_COUNT*BLOCK_SIZE, BLOCK_SIZE);

// Testing formatting of master boot record
void test_mbr_format()
{
    // Create two partitions splitting device in ~half
    int err = MBRBlockDevice::partition(&bd, 1, 0x83, 0, (BLOCK_COUNT/2)*BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = MBRBlockDevice::partition(&bd, 2, 0x83, -(BLOCK_COUNT/2)*BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Load both partitions, as well as a third to check for invalid partitions
    MBRBlockDevice part1(&bd, 1);
    err = part1.init();
    TEST_ASSERT_EQUAL(0, err);

    MBRBlockDevice part2(&bd, 2);
    err = part2.init();
    TEST_ASSERT_EQUAL(0, err);

    MBRBlockDevice part3(&bd, 3);
    err = part3.init();
    TEST_ASSERT_EQUAL(BD_ERROR_INVALID_PARTITION, err);

    // Deinit partitions
    err = part1.deinit();
    TEST_ASSERT_EQUAL(0, err);

    err = part2.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

// Testing mbr attributes
void test_mbr_attr()
{
    // Load partitions
    MBRBlockDevice part1(&bd, 1);
    int err = part1.init();
    TEST_ASSERT_EQUAL(0, err);

    MBRBlockDevice part2(&bd, 2);
    err = part2.init();
    TEST_ASSERT_EQUAL(0, err);

    // Test attributes on partitions
    printf("partition 1 partition number: %d\n",        part1.get_partition_number());
    printf("partition 1 partition start: 0x%llx\n",     part1.get_partition_start());
    printf("partition 1 partition stop: 0x%llx\n",      part1.get_partition_stop());
    printf("partition 1 partition type: 0x%02x\n",      part1.get_partition_type());
    printf("partition 1 read size: %llu bytes\n",       part1.get_read_size());
    printf("partition 1 program size: %llu bytes\n",    part1.get_program_size());
    printf("partition 1 erase size: %llu bytes\n",      part1.get_erase_size());
    printf("partition 1 size: %llu bytes\n",            part1.size());
    TEST_ASSERT_EQUAL(1,                                part1.get_partition_number());
    TEST_ASSERT_EQUAL(1*BLOCK_SIZE,                     part1.get_partition_start());
    TEST_ASSERT_EQUAL((BLOCK_COUNT/2)*BLOCK_SIZE,       part1.get_partition_stop());
    TEST_ASSERT_EQUAL(0x83,                             part1.get_partition_type());
    TEST_ASSERT_EQUAL(BLOCK_SIZE,                       part1.get_read_size());
    TEST_ASSERT_EQUAL(BLOCK_SIZE,                       part1.get_program_size());
    TEST_ASSERT_EQUAL(BLOCK_SIZE,                       part1.get_erase_size());
    TEST_ASSERT_EQUAL(((BLOCK_COUNT/2)-1)*BLOCK_SIZE,   part1.size());

    printf("partition 2 partition number: %d\n",        part2.get_partition_number());
    printf("partition 2 partition start: 0x%llx\n",     part2.get_partition_start());
    printf("partition 2 partition stop: 0x%llx\n",      part2.get_partition_stop());
    printf("partition 2 partition type: 0x%02x\n",      part2.get_partition_type());
    printf("partition 2 read size: %llu bytes\n",       part2.get_read_size());
    printf("partition 2 program size: %llu bytes\n",    part2.get_program_size());
    printf("partition 2 erase size: %llu bytes\n",      part2.get_erase_size());
    printf("partition 2 size: %llu bytes\n",            part2.size());
    TEST_ASSERT_EQUAL(2,                                part2.get_partition_number());
    TEST_ASSERT_EQUAL((BLOCK_COUNT/2)*BLOCK_SIZE,       part2.get_partition_start());
    TEST_ASSERT_EQUAL(BLOCK_COUNT*BLOCK_SIZE,           part2.get_partition_stop());
    TEST_ASSERT_EQUAL(0x83,                             part2.get_partition_type());
    TEST_ASSERT_EQUAL(BLOCK_SIZE,                       part2.get_read_size());
    TEST_ASSERT_EQUAL(BLOCK_SIZE,                       part2.get_program_size());
    TEST_ASSERT_EQUAL(BLOCK_SIZE,                       part2.get_erase_size());
    TEST_ASSERT_EQUAL((BLOCK_COUNT/2)*BLOCK_SIZE,       part2.size());

    // Deinit partitions
    err = part1.deinit();
    TEST_ASSERT_EQUAL(0, err);

    err = part2.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

// Testing mbr read write
void test_mbr_read_write()
{
    // Load partitions
    MBRBlockDevice part1(&bd, 1);
    int err = part1.init();
    TEST_ASSERT_EQUAL(0, err);

    MBRBlockDevice part2(&bd, 2);
    err = part2.init();
    TEST_ASSERT_EQUAL(0, err);

    // Test reading/writing the partitions
    uint8_t *write_block = new uint8_t[BLOCK_SIZE];
    uint8_t *read_block = new uint8_t[BLOCK_SIZE];

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = part1.erase(0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = part1.program(write_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = part1.read(read_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    // Check with original block device
    err = bd.read(read_block, 1*BLOCK_SIZE, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    // Test with second slice of block device
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = part2.erase(0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = part2.program(write_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = part2.read(read_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    // Check with original block device
    err = bd.read(read_block, (BLOCK_COUNT/2)*BLOCK_SIZE, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    // Clean up
    delete[] write_block;
    delete[] read_block;

    err = part1.deinit();
    TEST_ASSERT_EQUAL(0, err);

    err = part2.deinit();
    TEST_ASSERT_EQUAL(0, err);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing formatting of master boot record", test_mbr_format),
    Case("Testing mbr attributes", test_mbr_attr),
    Case("Testing mbr read write", test_mbr_read_write),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
