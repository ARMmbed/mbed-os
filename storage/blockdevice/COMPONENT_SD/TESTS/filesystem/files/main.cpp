/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
#include <stdlib.h>
#include <errno.h>

using namespace utest::v1;

// test configuration
#ifndef MBED_TEST_FILESYSTEM
#define MBED_TEST_FILESYSTEM FATFileSystem
#endif

#ifndef MBED_TEST_FILESYSTEM_DECL
#define MBED_TEST_FILESYSTEM_DECL MBED_TEST_FILESYSTEM fs("fs")
#endif

#ifndef MBED_TEST_BLOCKDEVICE
#define MBED_TEST_BLOCKDEVICE SDBlockDevice
#define MBED_TEST_BLOCKDEVICE_DECL SDBlockDevice bd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO, MBED_CONF_SD_SPI_CLK, MBED_CONF_SD_SPI_CS);
#endif

#ifndef MBED_TEST_BLOCKDEVICE_DECL
#define MBED_TEST_BLOCKDEVICE_DECL MBED_TEST_BLOCKDEVICE bd
#endif

#ifndef MBED_TEST_FILES
#define MBED_TEST_FILES 4
#endif

#ifndef MBED_TEST_DIRS
#define MBED_TEST_DIRS 4
#endif

#ifndef MBED_TEST_BUFFER
#define MBED_TEST_BUFFER 8192
#endif

#ifndef MBED_TEST_TIMEOUT
#define MBED_TEST_TIMEOUT 120
#endif


// declarations
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_FILESYSTEM)
#include INCLUDE(MBED_TEST_BLOCKDEVICE)

MBED_TEST_FILESYSTEM_DECL;
MBED_TEST_BLOCKDEVICE_DECL;

Dir dir[MBED_TEST_DIRS];
File file[MBED_TEST_FILES];
DIR *dd[MBED_TEST_DIRS];
FILE *fd[MBED_TEST_FILES];
struct dirent ent;
struct dirent *ed;
uint8_t buffer[MBED_TEST_BUFFER];
uint8_t rbuffer[MBED_TEST_BUFFER];
uint8_t wbuffer[MBED_TEST_BUFFER];

static char file_counter = 0;
const char *filenames[] = {"smallavacado", "mediumavacado", "largeavacado",
                           "blockfile", "bigblockfile", "hello", ".", ".."
                          };

// tests

void test_file_tests()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = MBED_TEST_FILESYSTEM::format(&bd);
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_simple_file_test()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        size_t size;
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = file[0].open(&fs, "hello", O_WRONLY | O_CREAT);
        TEST_ASSERT_EQUAL(0, res);
        size = strlen("Hello World!\n");
        memcpy(wbuffer, "Hello World!\n", size);
        res = file[0].write(wbuffer, size);
        TEST_ASSERT_EQUAL(size, res);
        res = file[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = file[0].open(&fs, "hello", O_RDONLY);
        TEST_ASSERT_EQUAL(0, res);
        size = strlen("Hello World!\n");
        res = file[0].read(rbuffer, size);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(rbuffer, wbuffer, size);
        TEST_ASSERT_EQUAL(0, res);
        res = file[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

template <int file_size, int write_size, int read_size>
void test_write_file_test()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        size_t size = file_size;
        size_t chunk = write_size;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = file[0].open(&fs, filenames[file_counter], O_WRONLY | O_CREAT);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            for (size_t b = 0; b < chunk; b++) {
                buffer[b] = rand() & 0xff;
            }
            res = file[0].write(buffer, chunk);
            TEST_ASSERT_EQUAL(chunk, res);
        }
        res = file[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        size_t size = file_size;
        size_t chunk = read_size;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = file[0].open(&fs, filenames[file_counter], O_RDONLY);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            res = file[0].read(buffer, chunk);
            TEST_ASSERT_EQUAL(chunk, res);
            for (size_t b = 0; b < chunk && i + b < size; b++) {
                res = buffer[b];
                TEST_ASSERT_EQUAL(rand() & 0xff, res);
            }
        }
        res = file[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    file_counter++;
    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_non_overlap_check()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        size_t size = 32;
        size_t chunk = 29;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = file[0].open(&fs, "smallavacado", O_RDONLY);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            res = file[0].read(buffer, chunk);
            TEST_ASSERT_EQUAL(chunk, res);
            for (size_t b = 0; b < chunk && i + b < size; b++) {
                res = buffer[b];
                TEST_ASSERT_EQUAL(rand() & 0xff, res);
            }
        }
        res = file[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        size_t size = 8192;
        size_t chunk = 29;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = file[0].open(&fs, "mediumavacado", O_RDONLY);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            res = file[0].read(buffer, chunk);
            TEST_ASSERT_EQUAL(chunk, res);
            for (size_t b = 0; b < chunk && i + b < size; b++) {
                res = buffer[b];
                TEST_ASSERT_EQUAL(rand() & 0xff, res);
            }
        }
        res = file[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        size_t size = 262144;
        size_t chunk = 29;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = file[0].open(&fs, "largeavacado", O_RDONLY);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            res = file[0].read(buffer, chunk);
            TEST_ASSERT_EQUAL(chunk, res);
            for (size_t b = 0; b < chunk && i + b < size; b++) {
                res = buffer[b];
                TEST_ASSERT_EQUAL(rand() & 0xff, res);
            }
        }
        res = file[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_dir_check()
{

    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "/");
        TEST_ASSERT_EQUAL(0, res);
        int numFiles = sizeof(filenames) / sizeof(filenames[0]);
        // Check the filenames in directory
        while (1) {
            res = dir[0].read(&ent);
            if (0 == res) {
                break;
            }
            for (int i = 0; i < numFiles ; i++) {
                res = strcmp(ent.d_name, filenames[i]);
                if (0 == res) {
                    res = ent.d_type;
                    if ((DT_REG != res) && (DT_DIR != res)) {
                        TEST_ASSERT(1);
                    }
                    break;
                } else if (i == numFiles) {
                    TEST_ASSERT_EQUAL(0, res);
                }
            }
        }
        res = dir[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}


// test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(MBED_TEST_TIMEOUT, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("File tests", test_file_tests),
    Case("Simple file test", test_simple_file_test),
    Case("Small file test", test_write_file_test<32, 31, 29>),
    Case("Medium file test", test_write_file_test<8192, 31, 29>),
    Case("Large file test", test_write_file_test<262144, 31, 29>),
    Case("Block Size file test", test_write_file_test<9000, 512, 512>),
    Case("Multiple block size file test", test_write_file_test<26215, MBED_TEST_BUFFER, MBED_TEST_BUFFER>),
    Case("Non-overlap check", test_non_overlap_check),
    Case("Dir check", test_dir_check),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
