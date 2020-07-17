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
#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] filesystem test cases require a RTOS to run
#else

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
#define MBED_TEST_BUFFER 512
#endif

#ifndef MBED_TEST_TIMEOUT
#define MBED_TEST_TIMEOUT 120
#endif

#ifndef MBED_THREAD_COUNT
#define MBED_THREAD_COUNT    MBED_TEST_FILES
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

volatile bool count_done = 0;

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

void write_file_data(char count)
{

    char filename[10];
    uint8_t wbuffer[MBED_TEST_BUFFER];
    int res;

    sprintf(filename, "%s%d", "data", count);
    res = file[count].open(&fs, filename, O_WRONLY | O_CREAT);
    TEST_ASSERT_EQUAL(0, res);

    char letter = 'A' + count;
    for (uint32_t i = 0; i < MBED_TEST_BUFFER; i++) {
        wbuffer[i] = letter++;
        if ('z' == letter) {
            letter = 'A' + count;
        }
    }

    for (uint32_t i = 0; i < 5; i++) {
        res = file[count].write(wbuffer, MBED_TEST_BUFFER);
        TEST_ASSERT_EQUAL(MBED_TEST_BUFFER, res);
    }

    res = file[count].close();
    TEST_ASSERT_EQUAL(0, res);
}

void read_file_data(char count)
{
    char filename[10];
    uint8_t rbuffer[MBED_TEST_BUFFER];
    int res;

    sprintf(filename, "%s%d", "data", count);
    res = file[count].open(&fs, filename, O_RDONLY);
    TEST_ASSERT_EQUAL(0, res);

    for (uint32_t i = 0; i < 5; i++) {
        res = file[count].read(rbuffer, MBED_TEST_BUFFER);
        TEST_ASSERT_EQUAL(MBED_TEST_BUFFER, res);
        char letter = 'A' + count;
        for (uint32_t i = 0; i < MBED_TEST_BUFFER; i++) {
            res = rbuffer[i];
            TEST_ASSERT_EQUAL(letter++, res);
            if ('z' == letter) {
                letter = 'A' + count;
            }
        }
    }

    res = file[count].close();
    TEST_ASSERT_EQUAL(0, res);
}

void test_thread_access_test()
{
    char *dummy = new (std::nothrow) char[OS_STACK_SIZE * MBED_THREAD_COUNT];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread *data[MBED_THREAD_COUNT];
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);
    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    // Write threads in parallel
    for (char thread_count = 0; thread_count < MBED_THREAD_COUNT; thread_count++) {
        data[thread_count] = new Thread(osPriorityNormal);
        data[thread_count]->start(callback((void(*)(void *))write_file_data, (void *)thread_count));
    }

    // Wait for write thread to join before creating read thread
    for (char thread_count = 0; thread_count < MBED_THREAD_COUNT; thread_count++) {
        data[thread_count]->join();
        delete data[thread_count];
        data[thread_count] = new Thread(osPriorityNormal);
        data[thread_count]->start(callback((void(*)(void *))read_file_data, (void *)thread_count));
    }

    // Wait for read threads to join
    for (char thread_count = 0; thread_count < MBED_THREAD_COUNT; thread_count++) {
        data[thread_count]->join();
        delete data[thread_count];
    }
    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);
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
    Case("Filesystem access from multiple threads", test_thread_access_test),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
#endif // !defined(MBED_CONF_RTOS_PRESENT)
