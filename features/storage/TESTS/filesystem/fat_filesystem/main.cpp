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
#include "mbed_retarget.h"

using namespace utest::v1;

#ifndef MBED_EXTENDED_TESTS
    #error [NOT_SUPPORTED] Filesystem tests not supported by default
#endif

// Test block device
#define BLOCK_SIZE 512
HeapBlockDevice bd(128*BLOCK_SIZE, BLOCK_SIZE);


// Test formatting
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
    File file;
    err = file.open(&fs, "test_read_write.dat", O_WRONLY | O_CREAT);
    TEST_ASSERT_EQUAL(0, err);
    ssize_t size = file.write(buffer, TEST_SIZE);
    TEST_ASSERT_EQUAL(TEST_SIZE, size);
    err = file.close();
    TEST_ASSERT_EQUAL(0, err);

    err = file.open(&fs, "test_read_write.dat", O_RDONLY);
    TEST_ASSERT_EQUAL(0, err);
    size = file.read(buffer, TEST_SIZE);
    TEST_ASSERT_EQUAL(TEST_SIZE, size);
    err = file.close();
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < TEST_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), buffer[i]);
    }

    err = fs.unmount();
    TEST_ASSERT_EQUAL(0, err);
}


// Simple test for iterating dir entries
void test_read_dir() {
    FATFileSystem fs("fat");

    int err = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, err);

    err = fs.mkdir("test_read_dir", S_IRWXU | S_IRWXG | S_IRWXO);
    TEST_ASSERT_EQUAL(0, err);

    err = fs.mkdir("test_read_dir/test_dir", S_IRWXU | S_IRWXG | S_IRWXO);
    TEST_ASSERT_EQUAL(0, err);

    File file;
    err = file.open(&fs, "test_read_dir/test_file", O_WRONLY | O_CREAT);
    TEST_ASSERT_EQUAL(0, err);
    err = file.close();
    TEST_ASSERT_EQUAL(0, err);

    // Iterate over dir checking for known files
    Dir dir;
    err = dir.open(&fs, "test_read_dir");
    TEST_ASSERT_EQUAL(0, err);

    struct dirent *de;
    bool test_dir_found = false;
    bool test_file_found = true;

    while ((de = readdir(&dir))) {
        printf("d_name: %.32s, d_type: %x\n", de->d_name, de->d_type);

        if (strcmp(de->d_name, ".") == 0) {
            test_dir_found = true;
            TEST_ASSERT_EQUAL(DT_DIR, de->d_type);
        } else if (strcmp(de->d_name, "..") == 0) {
            test_dir_found = true;
            TEST_ASSERT_EQUAL(DT_DIR, de->d_type);
        } else if (strcmp(de->d_name, "test_dir") == 0) {
            test_dir_found = true;
            TEST_ASSERT_EQUAL(DT_DIR, de->d_type);
        } else if (strcmp(de->d_name, "test_file") == 0) {
            test_file_found = true;
            TEST_ASSERT_EQUAL(DT_REG, de->d_type);
        } else {
            char *buf = new char[NAME_MAX];
            snprintf(buf, NAME_MAX, "Unexpected file \"%s\"", de->d_name);
            TEST_ASSERT_MESSAGE(false, buf);
        }
    }

    TEST_ASSERT_MESSAGE(test_dir_found,  "Could not find \"test_dir\"");
    TEST_ASSERT_MESSAGE(test_file_found, "Could not find \"test_file\"");

    err = dir.close();
    TEST_ASSERT_EQUAL(0, err);

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
    Case("Testing dir iteration", test_read_dir),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
