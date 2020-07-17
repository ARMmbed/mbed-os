/*
 * Copyright (c) 2020 Arm Limited
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

#include "I2CEEBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

#define TEST_PINS D14, D15
#define TEST_ADDR 0xa0
#define TEST_SIZE 32*1024
#define TEST_BLOCK_SIZE 64
#define TEST_FREQ 400000
#define TEST_BLOCK_COUNT 10
#define TEST_ERROR_MASK 16

const struct {
    const char *name;
    bd_size_t (BlockDevice::*method)() const;
} ATTRS[] = {
    {"read size",    &BlockDevice::get_read_size},
    {"program size", &BlockDevice::get_program_size},
    {"erase size",   &BlockDevice::get_erase_size},
    {"total size",   &BlockDevice::size},
};


void test_read_write()
{
    I2CEEBlockDevice bd(TEST_PINS, TEST_ADDR,
                        TEST_SIZE, TEST_BLOCK_SIZE, TEST_FREQ);

    int err = bd.init();
    TEST_ASSERT_EQUAL(0, err);

    for (unsigned a = 0; a < sizeof(ATTRS) / sizeof(ATTRS[0]); a++) {
        static const char *prefixes[] = {"", "k", "M", "G"};

        for (int i = 3; i >= 0; i--) {
            bd_size_t size = (bd.*ATTRS[a].method)();

            if (size >= (1ULL << 10 * i)) {
                printf("%s: %llu%sbytes (%llubytes)\n",
                       ATTRS[a].name, size >> 10 * i, prefixes[i], size);
                break;
            }
        }
    }

    bd_size_t block_size = bd.get_erase_size();
    uint8_t *write_block = new uint8_t[block_size];
    uint8_t *read_block = new uint8_t[block_size];
    uint8_t *error_mask = new uint8_t[TEST_ERROR_MASK];
    unsigned addrwidth = ceil(log(float(bd.size() - 1)) / log(float(16))) + 1;

    for (int b = 0; b < TEST_BLOCK_COUNT; b++) {
        // Find a random block
        bd_addr_t block = (rand() * block_size) % bd.size();

        // Use next random number as temporary seed to keep
        // the address progressing in the pseudorandom sequence
        unsigned seed = rand();

        // Fill with random sequence
        srand(seed);

        for (bd_size_t i = 0; i < block_size; i++) {
            write_block[i] = 0xff & rand();
        }

        // Write, sync, and read the block
        printf("test  %0*llx:%llu...\n", addrwidth, block, block_size);

        err = bd.erase(block, block_size);
        TEST_ASSERT_EQUAL(0, err);

        err = bd.program(write_block, block, block_size);
        TEST_ASSERT_EQUAL(0, err);

        printf("write %0*llx:%llu ", addrwidth, block, block_size);

        for (int i = 0; i < block_size && i < 16; i++) {
            printf("%02x", write_block[i]);
        }

        if (block_size > 16) {
            printf("...\n");
        }

        printf("\n");

        err = bd.read(read_block, block, block_size);
        TEST_ASSERT_EQUAL(0, err);

        printf("read  %0*llx:%llu ", addrwidth, block, block_size);

        for (int i = 0; i < block_size && i < 16; i++) {
            printf("%02x", read_block[i]);
        }

        if (block_size > 16) {
            printf("...");
        }

        printf("\n");

        // Find error mask for debugging
        memset(error_mask, 0, TEST_ERROR_MASK);
        bd_size_t error_scale = block_size / (TEST_ERROR_MASK * 8);

        srand(seed);

        for (bd_size_t i = 0; i < TEST_ERROR_MASK * 8; i++) {
            for (bd_size_t j = 0; j < error_scale; j++) {
                if ((0xff & rand()) != read_block[i * error_scale + j]) {
                    error_mask[i / 8] |= 1 << (i % 8);
                }
            }
        }

        printf("error %0*llx:%llu ", addrwidth, block, block_size);

        for (int i = 0; i < TEST_ERROR_MASK; i++) {
            printf("%02x", error_mask[i]);
        }

        printf("\n");

        // Check that the data was unmodified
        srand(seed);

        for (bd_size_t i = 0; i < block_size; i++) {
            TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
        }
    }

    err = bd.deinit();
    TEST_ASSERT_EQUAL(0, err);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing read write random blocks", test_read_write),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
