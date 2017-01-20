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
#include "FATFileSystem.h"
#include <stdlib.h>

using namespace utest::v1;

// Test block device
#define BLOCK_SIZE 512
HeapBlockDevice bd(128*BLOCK_SIZE, BLOCK_SIZE);


void test_format() {
    int err = FATFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, err);
}

// Simple test for reading/writing files
template <ssize_t TEST_SIZE>
void test_read_write() {
    FATFileSystem fs("fat");

    int err = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, err);

    uint8_t *buffer = (uint8_t *)malloc(TEST_SIZE);
    TEST_ASSERT(buffer);
    
    // Fill with random sequence
    srand(1);
    for (int i = 0; i < TEST_SIZE; i++) {
        buffer[i] = 0xff & rand();
    }

    // write and read file
    FileHandle *file = fs.open("test_read_write.dat", O_WRONLY | O_CREAT);
    TEST_ASSERT(file);
    ssize_t size = file->write(buffer, TEST_SIZE);
    TEST_ASSERT_EQUAL(TEST_SIZE, size);
    err = file->close();
    TEST_ASSERT_EQUAL(0, err);

    file = fs.open("test_read_write.dat", O_RDONLY);
    TEST_ASSERT(file);
    size = file->read(buffer, TEST_SIZE);
    TEST_ASSERT_EQUAL(TEST_SIZE, size);
    err = file->close();
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < TEST_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), buffer[i]);
    }

    err = fs.unmount();
    TEST_ASSERT_EQUAL(0, err);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing formating", test_format),
    Case("Testing read write < block", test_read_write<BLOCK_SIZE/2>),
    Case("Testing read write > block", test_read_write<2*BLOCK_SIZE>),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
