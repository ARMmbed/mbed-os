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

#include "SDBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

#ifndef SD_INSTALLED
#define SD_INSTALLED false
#endif

#ifndef SD_PINS
#if defined(TARGET_K64F)
#define SD_PINS PTE3, PTE1, PTE2, PTE4
#endif
#endif

#if !SD_INSTALLED
#error [NOT_SUPPORTED] SD card required
#endif

#define BLOCK_SIZE 512
uint8_t write_block[BLOCK_SIZE];
uint8_t read_block[BLOCK_SIZE];

// Simple test which read/writes a block
void test_read_write() {
    SDBlockDevice bd(SD_PINS);

    int err = bd.init();
    TEST_ASSERT_EQUAL(0, err);

    printf("read size: %llu\r\n", bd.get_read_size());
    printf("program size: %llu\r\n", bd.get_program_size());
    printf("erase size: %llu\r\n", bd.get_erase_size());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = bd.write(write_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = bd.read(read_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }
    
    err = bd.deinit();
    TEST_ASSERT_EQUAL(0, err);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing read write of a block", test_read_write),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
