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

#include "HeapBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

#define TEST_BLOCK_SIZE 128
#define TEST_BLOCK_DEVICE_SIZE 32*TEST_BLOCK_SIZE
#define TEST_BLOCK_COUNT 10
#define TEST_ERROR_MASK 16

#if ((MBED_RAM_SIZE - MBED_CONF_TARGET_BOOT_STACK_SIZE) <= TEST_BLOCK_DEVICE_SIZE)
#error [NOT_SUPPORTED] Insufficient heap for heap block device tests
#endif

const struct {
    const char *name;
    bd_size_t (BlockDevice::*method)() const;
} ATTRS[] = {
    {"read size",    &BlockDevice::get_read_size},
    {"program size", &BlockDevice::get_program_size},
    {"erase size",   &BlockDevice::get_erase_size},
    {"total size",   &BlockDevice::size},
};


// Simple test that read/writes random set of blocks
void test_read_write()
{
    uint8_t *dummy = new (std::nothrow) uint8_t[TEST_BLOCK_DEVICE_SIZE];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory for test");
    delete[] dummy;

    HeapBlockDevice bd(TEST_BLOCK_DEVICE_SIZE, TEST_BLOCK_SIZE);

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

    unsigned addrwidth = ceil(log(float(bd.size() - 1)) / log(float(16))) + 1;

    bd_size_t block_size = bd.get_erase_size();
    uint8_t *write_block = new (std::nothrow) uint8_t[block_size];
    uint8_t *read_block = new (std::nothrow) uint8_t[block_size];
    uint8_t *error_mask = new (std::nothrow) uint8_t[TEST_ERROR_MASK];
    if (!write_block || !read_block || !error_mask) {
        printf("Not enough memory for test");
        goto end;
    }


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

        // erase, program, and read the block
        printf("test  %0*llx:%llu...\n", addrwidth, block, block_size);

        err = bd.erase(block, block_size);
        TEST_ASSERT_EQUAL(0, err);

        err = bd.program(write_block, block, block_size);
        TEST_ASSERT_EQUAL(0, err);

        printf("write %0*llx:%llu ", addrwidth, block, block_size);
        for (int i = 0; i < 16; i++) {
            printf("%02x", write_block[i]);
        }
        printf("...\n");

        err = bd.read(read_block, block, block_size);
        TEST_ASSERT_EQUAL(0, err);

        printf("read  %0*llx:%llu ", addrwidth, block, block_size);
        for (int i = 0; i < 16; i++) {
            printf("%02x", read_block[i]);
        }
        printf("...\n");

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
        for (int i = 0; i < 16; i++) {
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

end:
    delete[] write_block;
    delete[] read_block;
    delete[] error_mask;

}

void test_get_type_functionality()
{
    uint8_t *dummy = new (std::nothrow) uint8_t[TEST_BLOCK_DEVICE_SIZE];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory for test");
    delete[] dummy;

    HeapBlockDevice bd(TEST_BLOCK_DEVICE_SIZE, TEST_BLOCK_SIZE);

    const char *bd_type = bd.get_type();
    TEST_ASSERT_NOT_EQUAL(0, bd_type);
    TEST_ASSERT_EQUAL(0, strcmp(bd_type, "HEAP"));
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing read write random blocks", test_read_write),
    Case("Testing get type functionality", test_get_type_functionality)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
