/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 *
 */

/* The following copyright notice is reproduced from the glibc project
 * REF_LICENCE_GLIBC
 *
 * Copyright (C) 1991, 1992 Free Software Foundation, Inc.
 * This file is part of the GNU C Library.
 *
 * The GNU C Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * The GNU C Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the GNU C Library; see the file COPYING.LIB.  If
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave,
 * Cambridge, MA 02139, USA.
 */


/** @file main.cpp Basic SD Driver Test
 */
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "SDBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

#define TEST_BLOCK_COUNT        10
#define TEST_ERROR_MASK         16
#define TEST_BLOCK_SIZE         2048

const struct {
    const char *name;
    bd_size_t (BlockDevice::*method)() const;
} ATTRS[] = {
    {"read size",    &BlockDevice::get_read_size},
    {"program size", &BlockDevice::get_program_size},
    {"erase size",   &BlockDevice::get_erase_size},
    {"total size",   &BlockDevice::size},
};

void test_read_write() {
    SDBlockDevice sd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO, MBED_CONF_SD_SPI_CLK, MBED_CONF_SD_SPI_CS);

    int err = sd.init();
    TEST_ASSERT_EQUAL(0, err);

    err = sd.frequency(8000000);
    TEST_ASSERT_EQUAL(0, err);

    for (unsigned a = 0; a < sizeof(ATTRS)/sizeof(ATTRS[0]); a++) {
        static const char *prefixes[] = {"", "k", "M", "G"};
        for (int i = 3; i >= 0; i--) {
            bd_size_t size = (sd.*ATTRS[a].method)();
            if (size >= (1ULL << 10*i)) {
                printf("%s: %llu%sbytes (%llubytes)\n",
                    ATTRS[a].name, size >> 10*i, prefixes[i], size);
                break;
            }
        }
    }

    bd_size_t erase_size = sd.get_erase_size();
    bd_size_t block_size = erase_size > TEST_BLOCK_SIZE ? erase_size : TEST_BLOCK_SIZE;

    uint8_t *write_block = new uint8_t[block_size];
    uint8_t *read_block = new uint8_t[block_size];
    uint8_t *error_mask = new uint8_t[TEST_ERROR_MASK];
    unsigned addrwidth = ceil(log(float(sd.size()-1)) / log(float(16)))+1;

    for (int b = 0; b < TEST_BLOCK_COUNT; b++) {
        // Find a random block
        bd_addr_t block = (rand()*block_size) % sd.size();

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

        err = sd.trim(block, block_size);
        TEST_ASSERT_EQUAL(0, err);

        err = sd.program(write_block, block, block_size);
        TEST_ASSERT_EQUAL(0, err);

        printf("write %0*llx:%llu ", addrwidth, block, block_size);
        for (int i = 0; i < 16; i++) {
            printf("%02x", write_block[i]);
        }
        printf("...\n");

        err = sd.read(read_block, block, block_size);
        TEST_ASSERT_EQUAL(0, err);

        printf("read  %0*llx:%llu ", addrwidth, block, block_size);
        for (int i = 0; i < 16; i++) {
            printf("%02x", read_block[i]);
        }
        printf("...\n");

        // Find error mask for debugging
        memset(error_mask, 0, TEST_ERROR_MASK);
        bd_size_t error_scale = block_size / (TEST_ERROR_MASK*8);

        srand(seed);
        for (bd_size_t i = 0; i < TEST_ERROR_MASK*8; i++) {
            for (bd_size_t j = 0; j < error_scale; j++) {
                if ((0xff & rand()) != read_block[i*error_scale + j]) {
                    error_mask[i/8] |= 1 << (i%8);
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

    err = sd.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(120, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing read write random blocks", test_read_write),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
