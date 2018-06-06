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
#include "SlicingBlockDevice.h"
#include "ChainingBlockDevice.h"
#include "ProfilingBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

// TODO HACK, replace with available ram/heap property
#if defined(TARGET_MTB_MTS_XDOT)
    #error [NOT_SUPPORTED] Insufficient heap for heap block device tests
#endif

#define BLOCK_COUNT 16
#define BLOCK_SIZE 512


// Simple test which read/writes blocks on a sliced block device
void test_slicing() {
    HeapBlockDevice bd(BLOCK_COUNT*BLOCK_SIZE, BLOCK_SIZE);
    uint8_t *write_block = new uint8_t[BLOCK_SIZE];
    uint8_t *read_block = new uint8_t[BLOCK_SIZE];

    // Test with first slice of block device
    SlicingBlockDevice slice1(&bd, 0, (BLOCK_COUNT/2)*BLOCK_SIZE);

    int err = slice1.init();
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(BLOCK_SIZE, slice1.get_program_size());
    TEST_ASSERT_EQUAL(BLOCK_SIZE, slice1.get_erase_size(BLOCK_SIZE));
    TEST_ASSERT_EQUAL((BLOCK_COUNT/2)*BLOCK_SIZE, slice1.size());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = slice1.program(write_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = slice1.read(read_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    // Check with original block device
    err = bd.read(read_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    err = slice1.deinit();
    TEST_ASSERT_EQUAL(0, err);


    // Test with second slice of block device
    SlicingBlockDevice slice2(&bd, -(BLOCK_COUNT/2)*BLOCK_SIZE);

    err = slice2.init();
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(BLOCK_SIZE, slice2.get_program_size());
    TEST_ASSERT_EQUAL((BLOCK_COUNT/2)*BLOCK_SIZE, slice2.size());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = slice2.program(write_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = slice2.read(read_block, 0, BLOCK_SIZE);
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

    delete[] write_block;
    delete[] read_block;
    err = slice2.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

// Simple test which read/writes blocks on a chain of block devices
void test_chaining() {
    HeapBlockDevice bd1((BLOCK_COUNT/2)*BLOCK_SIZE, BLOCK_SIZE);
    HeapBlockDevice bd2((BLOCK_COUNT/2)*BLOCK_SIZE, BLOCK_SIZE);
    uint8_t *write_block = new uint8_t[BLOCK_SIZE];
    uint8_t *read_block = new uint8_t[BLOCK_SIZE];

    // Test with chain of block device
    BlockDevice *bds[] = {&bd1, &bd2};
    ChainingBlockDevice chain(bds);

    int err = chain.init();
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(BLOCK_SIZE, chain.get_program_size());
    TEST_ASSERT_EQUAL(BLOCK_SIZE, chain.get_erase_size((BLOCK_COUNT/2)*BLOCK_SIZE+1));
    TEST_ASSERT_EQUAL(BLOCK_COUNT*BLOCK_SIZE, chain.size());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = chain.program(write_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = chain.read(read_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    // Write, sync, and read the block
    err = chain.program(write_block, (BLOCK_COUNT/2)*BLOCK_SIZE, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = chain.read(read_block, (BLOCK_COUNT/2)*BLOCK_SIZE, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    delete[] write_block;
    delete[] read_block;
    err = chain.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

// Simple test which read/writes blocks on a chain of block devices
void test_profiling() {
    HeapBlockDevice bd(BLOCK_COUNT*BLOCK_SIZE, BLOCK_SIZE);
    uint8_t *write_block = new uint8_t[BLOCK_SIZE];
    uint8_t *read_block = new uint8_t[BLOCK_SIZE];

    // Test under profiling
    ProfilingBlockDevice profiler(&bd);

    int err = profiler.init();
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(BLOCK_SIZE, profiler.get_erase_size());
    TEST_ASSERT_EQUAL(BLOCK_COUNT*BLOCK_SIZE, profiler.size());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = profiler.erase(0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = profiler.program(write_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = profiler.read(read_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    // Check with original block device
    err = bd.read(read_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    delete[] write_block;
    delete[] read_block;
    err = profiler.deinit();
    TEST_ASSERT_EQUAL(0, err);

    // Check that profiled operations match expectations
    bd_size_t read_count = profiler.get_read_count();
    TEST_ASSERT_EQUAL(BLOCK_SIZE, read_count);
    bd_size_t program_count = profiler.get_program_count();
    TEST_ASSERT_EQUAL(BLOCK_SIZE, program_count);
    bd_size_t erase_count = profiler.get_erase_count();
    TEST_ASSERT_EQUAL(BLOCK_SIZE, erase_count);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing slicing of a block device", test_slicing),
    Case("Testing chaining of block devices", test_chaining),
    Case("Testing profiling of block devices", test_profiling),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
